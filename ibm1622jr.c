//======================================================================================================================
//
//  ibm1622jr.c - main program
//
//  IBM 1620 Jr Project, Computer History Museum, 2017-2026
//
//  To recreate the experience (visual, auditory, tactile, visceral) of running historic software on a 1960s-era
//  computer.
//
//    Dave Babcock  - project lead, software, website
//   David Brock    - CHM sponsor
//   Steve Casner   - hardware, software
//     Joe Fredrick - hardware, firmware
//     Len Shustek  - CHM advisor
//     Dag Spicer   - CHM advisor
//   David Wise     - IBM 1620 expert
//
//   Versions:
//      1.0     06/09/2025     Initial version.
//
//======================================================================================================================

#include "defines.h"
#include "data.h"
#include "audio.h"
#include "cardio.h"
#include "commio.h"
#include "display.h"
#include "select.h"

#include <errno.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


// Function declarations
void InterruptHandler(int);
void ComputeVersionBuild(void);
void ParseCommandLine(int, char *[], char *, char *);


// Main program
int main(int argc, char *argv[]) {
    SDL_Event event;
    float x;
    float y;

    ComputeVersionBuild();
    (void)printf("\nIBM 1622 Jr. (version %d.%d.%d)\n", versionMajor, versionMinor, versionBuild);

    setbuf(stdout, NULL);

    (void)signal(SIGABRT, InterruptHandler);
    (void)signal(SIGHUP,  InterruptHandler);
    (void)signal(SIGINT,  InterruptHandler);
    (void)signal(SIGQUIT, InterruptHandler);
    (void)signal(SIGTERM, InterruptHandler);
    (void)signal(SIGUSR1, InterruptHandler);
    (void)signal(SIGUSR2, InterruptHandler);

    running = TRUE;

    (void)pthread_mutex_init(&displayLock, NULL);

    DisplayOpen();
    ShowScreen(DISPLAY_SPLASH_SCREEN);

    CommIOStart();
    CardIOStart();
    AudioStart();

    SDL_Delay(5000);

    while (running) {

        while (SDL_WaitEvent(&event) != 0) {

            switch (event.type) {

                case SDL_USEREVENT:
                    if (display == DISPLAY_READER_PUNCH) {
                        if ((readerStatus == STATUS_READY) && (readRequest != TYPE_NONE) &&
                            (readRequestCounter == 0)) {
                            readTrigger = readRequest;
                            readRequestCounter = 3;
                            readRequest = TYPE_NONE;
                        } else if (readRequestCounter > 0) {
                            if (--readRequestCounter == 0) {
                                SendChar(STATUS_READER_NOT_BUSY);
                            }
                        }
                        if ((punchStatus == STATUS_READY) && (punchRequest != TYPE_NONE) &&
                            (punchRequestCounter == 0)) {
                            punchTrigger = punchRequest;
                            punchRequestCounter = 6;
                            punchRequest = TYPE_NONE;
                        } else if (punchRequestCounter > 0) {
                            if (--punchRequestCounter == 0) {
                                SendChar(STATUS_PUNCH_NOT_BUSY);
                            }
                        }
                    }
                    ShowScreen(display);
					break;

                case SDL_FINGERDOWN:
                    x = event.tfinger.x;
                    y = event.tfinger.y;
                    if (display == DISPLAY_READER_PUNCH) {
                        if ((x >= TOUCH_LOAD_X1) && (x <= TOUCH_LOAD_X2) &&
                            (y >= TOUCH_LOAD_Y1) && (y <= TOUCH_LOAD_Y2) &&
                            (readerStatus == STATUS_READY) && (ibm1620RunState == STATE_MANUAL)) {
                            buttonState = BUTTON_LOAD_PRESSED;
                            SendChar(COMMAND_LOAD);
                            PlayButtonDown();
                        } else if ((x >= TOUCH_RESET_X1) && (x <= TOUCH_RESET_X2) &&
                                   (y >= TOUCH_RESET_Y1) && (y <= TOUCH_RESET_Y2)) {
                            if (readerStatus == STATUS_CHECK) {
                                buttonState = BUTTON_RESET_PRESSED;
                                if (rHopperCount > 0) {
                                    readerStatus = STATUS_READY;
                                    SendChar(STATUS_READER_READY);
                                } else {
                                    readerStatus = STATUS_NOT_READY;
                                    SendChar(STATUS_READER_NOT_READY);
                                }
                                PlayButtonDown();
                            }
                            if (punchStatus == STATUS_CHECK) {
                                buttonState = BUTTON_RESET_PRESSED;
                                if (pHopperCount > 0) {
                                    punchStatus = STATUS_READY;
                                    SendChar(STATUS_PUNCH_READY);
                                } else {
                                    punchStatus = STATUS_NOT_READY;
                                    SendChar(STATUS_PUNCH_NOT_READY);
                                }
                                PlayButtonDown();
                            }
                        }
                    } else if (display == DISPLAY_FILE_SELECT) {
                        if ((strlen(title) > 2) &&
                            (x >= TOUCH_BACKUP_X1) && (x <= TOUCH_BACKUP_X2) &&
                            (y >= TOUCH_BACKUP_Y1) && (y <= TOUCH_BACKUP_Y2)) {
                            selectedItem = SELECTED_BACKUP;
                        } else if ((firstEntry > 0) &&
                                   (x >= TOUCH_UPUPARROW_X1) && (x <= TOUCH_UPUPARROW_X2) &&
                                   (y >= TOUCH_UPUPARROW_Y1) && (y <= TOUCH_UPUPARROW_Y2)) {
                            selectedItem = SELECTED_UPUPARROW;
                        } else if ((firstEntry > 0) &&
                                   (x >= TOUCH_UPARROW_X1) && (x <= TOUCH_UPARROW_X2) &&
                                   (y >= TOUCH_UPARROW_Y1) && (y <= TOUCH_UPARROW_Y2)) {
                            selectedItem = SELECTED_UPARROW;
                        } else if ((numEntries > (firstEntry + 11)) &&
                                   (x >= TOUCH_DOWNARROW_X1) && (x <= TOUCH_DOWNARROW_X2) &&
                                   (y >= TOUCH_DOWNARROW_Y1) && (y <= TOUCH_DOWNARROW_Y2)) {
                            selectedItem = SELECTED_DOWNARROW;
                        } else if ((numEntries > (firstEntry + 11)) &&
                                   (x >= TOUCH_DOWNDOWNARROW_X1) && (x <= TOUCH_DOWNDOWNARROW_X2) &&
                                   (y >= TOUCH_DOWNDOWNARROW_Y1) && (y <= TOUCH_DOWNDOWNARROW_Y2)) {
                            selectedItem = SELECTED_DOWNDOWNARROW;
                        } else if ((numEntries > 0) &&
                                   (x >= TOUCH_SELECTION_X1) && (x <= TOUCH_SELECTION_X2) &&
                                   (y >= TOUCH_SELECTION_Y1) && (y <= TOUCH_SELECTION_Y2)) {
                            for (int i = 0; i < MIN(numEntries, 11); ++i) {
                                if (y <= layout[i].touch_y2) {
                                    selectedItem = SELECTED_ITEMS + i;
                                    break;
                                }
                            }
                        } else {
                            selectedItem = SELECTED_NONE;
                        }
                    }
                    break;

                case SDL_FINGERMOTION:
                    x = event.tfinger.x;
                    y = event.tfinger.y;
                    if (display == DISPLAY_FILE_SELECT) {
                        if ((strlen(title) > 2) &&
                            (x >= TOUCH_BACKUP_X1) && (x <= TOUCH_BACKUP_X2) &&
                            (y >= TOUCH_BACKUP_Y1) && (y <= TOUCH_BACKUP_Y2)) {
                            selectedItem = SELECTED_BACKUP;
                        } else if ((firstEntry > 0) &&
                                   (x >= TOUCH_UPUPARROW_X1) && (x <= TOUCH_UPUPARROW_X2) &&
                                   (y >= TOUCH_UPUPARROW_Y1) && (y <= TOUCH_UPUPARROW_Y2)) {
                            selectedItem = SELECTED_UPUPARROW;
                        } else if ((firstEntry > 0) &&
                                   (x >= TOUCH_UPARROW_X1) && (x <= TOUCH_UPARROW_X2) &&
                                   (y >= TOUCH_UPARROW_Y1) && (y <= TOUCH_UPARROW_Y2)) {
                            selectedItem = SELECTED_UPARROW;
                        } else if ((numEntries > (firstEntry + 11)) &&
                                   (x >= TOUCH_DOWNARROW_X1) && (x <= TOUCH_DOWNARROW_X2) &&
                                   (y >= TOUCH_DOWNARROW_Y1) && (y <= TOUCH_DOWNARROW_Y2)) {
                            selectedItem = SELECTED_DOWNARROW;
                        } else if ((numEntries > (firstEntry + 11)) &&
                                   (x >= TOUCH_DOWNDOWNARROW_X1) && (x <= TOUCH_DOWNDOWNARROW_X2) &&
                                   (y >= TOUCH_DOWNDOWNARROW_Y1) && (y <= TOUCH_DOWNDOWNARROW_Y2)) {
                            selectedItem = SELECTED_DOWNDOWNARROW;
                        } else if ((numEntries > 0) &&
                                   (x >= TOUCH_SELECTION_X1) && (x <= TOUCH_SELECTION_X2) &&
                                   (y >= TOUCH_SELECTION_Y1) && (y <= TOUCH_SELECTION_Y2)) {
                            for (int i = 0; i < MIN(numEntries, 11); ++i) {
                                if (y <= layout[i].touch_y2) {
                                    selectedItem = SELECTED_ITEMS + i;
                                    break;
                                }
                            }
                        } else {
                            selectedItem = SELECTED_NONE;
                        }
                    }
                    break;

                case SDL_FINGERUP:
                    if (display == DISPLAY_READER_PUNCH) {
                        if (buttonState != BUTTON_NONE_PRESSED) {
                            PlayButtonUp();
                        }
                        buttonState = BUTTON_NONE_PRESSED;
                    } else if (display == DISPLAY_FILE_SELECT) {
                        if (selectedItem == SELECTED_BACKUP) {
                            selectedFile = SELECTED_BACKUP;
                        } else if (selectedItem == SELECTED_UPUPARROW) {
                            firstEntry = MAX(firstEntry - 11, 0);
                        } else if (selectedItem == SELECTED_UPARROW) {
                            firstEntry = MAX(firstEntry - 1, 0);
                        } else if (selectedItem == SELECTED_DOWNARROW) {
                            firstEntry = MIN(firstEntry + 1, numEntries - 11);
                        } else if (selectedItem == SELECTED_DOWNDOWNARROW) {
                            firstEntry = MIN(firstEntry + 11, numEntries - 11);
                        } else if (selectedItem >= SELECTED_ITEMS) {
                            selectedFile = firstEntry + selectedItem;
                        } else {
                            selectedFile = SELECTED_NONE;
                        }
                        selectedItem = SELECTED_NONE;
                    }
                    break;

                case SDL_QUIT:
                    running = FALSE;
                    break;

                default:
                    // (void)printf("SDL event = %d\n", event.type);
                    break;
            }

            if (!running) break;
        }
    }

    AudioStop();
    CardIOStop();
    CommIOStop();

    DisplayClose();

    (void)pthread_mutex_destroy(&displayLock);

    (void)printf("\nEnd of IBM 1622 Jr.\n");

    exit(162);
}

void InterruptHandler(int sig) {
    SDL_Event event;

    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

void ComputeVersionBuild(void) {

    // Local data
    int month = 0;

    if      ((versionDate[0] == 'J') && (versionDate[1] == 'a') && (versionDate[2] == 'n')) month = 1;
    else if ((versionDate[0] == 'F') && (versionDate[1] == 'e') && (versionDate[2] == 'b')) month = 2;
    else if ((versionDate[0] == 'M') && (versionDate[1] == 'a') && (versionDate[2] == 'r')) month = 3;
    else if ((versionDate[0] == 'A') && (versionDate[1] == 'p') && (versionDate[2] == 'r')) month = 4;
    else if ((versionDate[0] == 'M') && (versionDate[1] == 'a') && (versionDate[2] == 'y')) month = 5;
    else if ((versionDate[0] == 'J') && (versionDate[1] == 'u') && (versionDate[2] == 'n')) month = 6;
    else if ((versionDate[0] == 'J') && (versionDate[1] == 'u') && (versionDate[2] == 'l')) month = 7;
    else if ((versionDate[0] == 'A') && (versionDate[1] == 'u') && (versionDate[2] == 'g')) month = 8;
    else if ((versionDate[0] == 'S') && (versionDate[1] == 'e') && (versionDate[2] == 'p')) month = 9;
    else if ((versionDate[0] == 'O') && (versionDate[1] == 'c') && (versionDate[2] == 't')) month = 10;
    else if ((versionDate[0] == 'N') && (versionDate[1] == 'o') && (versionDate[2] == 'v')) month = 11;
    else if ((versionDate[0] == 'D') && (versionDate[1] == 'e') && (versionDate[2] == 'c')) month = 12;
    int day = 10 * (int)(((versionDate[4] == ' ') ? '0' : versionDate[4]) - '0') + (int)(versionDate[5] - '0');
    int year = 10 * (int)(versionDate[9] - '0') + (int)(versionDate[10] - '0');
    versionBuild = 10000 * year + 100 * month + day;
}
