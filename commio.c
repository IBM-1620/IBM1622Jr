//======================================================================================================================
//
//  commio.c - communication i/o
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
//======================================================================================================================

#include "defines.h"
#include "data.h"
#include "display.h"
#include "commio.h"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <termios.h>
#include <unistd.h>

// Function declarations
void *CommIO(void *);

// Data
int commFile = -1;
char *commFilename = (char*)"/dev/ttyUSB0";

pthread_t commIOThread;
pthread_t commOutputThread;
pthread_mutex_t commLock;

bool commIORunning = FALSE;
int idx = 0;


// CommIOStart
void CommIOStart(void) {
    int rc;

    // Create communication thread
    if ((rc = pthread_create(&commIOThread, NULL, CommIO, NULL)) != 0) {
        (void)printf("Failed to create communication input thread: %s\n", strerror(rc));
        exit(-1);
    }

    // Open communication
    (void)pthread_mutex_init(&commLock, NULL);
    (void)OpenCommIO();

    commIORunning = TRUE;
}

// CommIOStop
void CommIOStop(void) {

    commIORunning = FALSE;

    // Close communication
    CloseCommIO();
    (void)pthread_mutex_destroy(&commLock);

    // Terminate communication thread
    (void)pthread_kill(commIOThread, SIGUSR1);
    (void)pthread_join(commIOThread, NULL);
}

// OpenCommIO
bool OpenCommIO(void) {
    int file;
    struct termios tio;

    // Lock communication open & close
    (void)pthread_mutex_lock(&commLock);

    // Test if communication already open
    if (commFile >= 0) {
        (void)pthread_mutex_unlock(&commLock);
        return TRUE;
    }

    // Attempt to open communication
    file = open(commFilename, O_RDWR | O_NOCTTY);
    if (file < 0) {
        (void)pthread_mutex_unlock(&commLock);
        (void)usleep(100000);
        return FALSE;
    }

    // Set punched card port characteristics
    bzero(&tio, sizeof(tio));
    tio.c_iflag = IGNBRK | IGNPAR | IGNCR;
    tio.c_oflag = ONLRET;
    tio.c_cflag = CS8 | CREAD | CLOCAL | B115200;
    tio.c_lflag = 0;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 0;
    tio.c_ispeed = B115200;
    tio.c_ospeed = B115200;
    (void)tcflush(file, TCIFLUSH);
    (void)tcsetattr(file, TCSANOW, &tio);

    commFile = file;

    // Unlock communication open & close
    (void)pthread_mutex_unlock(&commLock);

    return TRUE;
}

// CloseCommIO
void CloseCommIO() {

    // Lock communication open & close
    (void)pthread_mutex_lock(&commLock);

    // Close communication
    if (commFile >= 0) {
        (void)close(commFile);
        commFile = -1;
    }

    // Unlock communication open & close
    (void)pthread_mutex_unlock(&commLock);
}

void *CommIO(void *arg) {
    int chr;

    // Name this thread
    pthread_t threadId = pthread_self();
    (void)pthread_setname_np(threadId, "CommIO");

    // Lock this thread on CPU 1
    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(1, &cpus);
    if (pthread_setaffinity_np(threadId, sizeof(cpu_set_t), &cpus) != 0) {
        (void)printf("CommIO cpu affinity failed: %s\n", strerror(errno));
        exit(-1);
    }

    // Adjust signal mask for this thread
    sigset_t blockSignals;
    sigemptyset(&blockSignals);
    sigaddset(&blockSignals, SIGABRT);
    sigaddset(&blockSignals, SIGHUP);
    sigaddset(&blockSignals, SIGINT);
    sigaddset(&blockSignals, SIGQUIT);
    sigaddset(&blockSignals, SIGTERM);
    sigaddset(&blockSignals, SIGUSR2);
    pthread_sigmask(SIG_BLOCK, &blockSignals, NULL);

    // Wait until commio is running
    while (!commIORunning) {
        (void)usleep(1000);
    }

    // Request current 1620 status
    SendChar(STATUS_REQUEST_STATUS);

    // Process incoming character
    while (commIORunning) {
        chr = ReceiveChar() & 0x7f;
        if (!commIORunning) break;

        switch (protocolCDActions[chr]) {

            case PACTION_IGNORE:
                break;

            case PACTION_STORE:
                if (idx < 80) punchCard[idx++] = chr;
                break;

            case PACTION_MANUAL:
                ibm1620RunState = STATE_MANUAL;
                break;

            case PACTION_NOT_MANUAL:
                ibm1620RunState = STATE_NOT_MANUAL;
                break;

            case PACTION_READ_NUMERIC:
                readRequest = TYPE_NUMERIC;
                SendChar(STATUS_READER_BUSY);
                break;

            case PACTION_READ_ALPHAMERIC:
                readRequest = TYPE_ALPHAMERIC;
                SendChar(STATUS_READER_BUSY);
                break;

            case PACTION_WRITE_NUMERIC:
                for (int i = idx; i < 80; ++i) punchCard[idx++] = '0';
                punchRequest = TYPE_NUMERIC;
                SendChar(STATUS_PUNCH_BUSY);
                idx = 0;
                break;

            case PACTION_WRITE_ALPHAMERIC:
                for (int i = idx; i < 80; ++i) punchCard[idx++] = ' ';
                punchRequest = TYPE_ALPHAMERIC;
                SendChar(STATUS_PUNCH_BUSY);
                idx = 0;
                break;

            case PACTION_DUMP_NUMERIC:
                for (int i = idx; i < 80; ++i) punchCard[idx++] = '0';
                punchRequest = TYPE_NUMERIC;
                SendChar(STATUS_PUNCH_BUSY);
                idx = 0;
                break;

            case PACTION_RESET:
                if (rHopperCount > 0) {
                    readerStatus = STATUS_READY;
                    SendChar(STATUS_READER_READY);
                } else {
                    readerStatus = STATUS_NOT_READY;
                    SendChar(STATUS_READER_NOT_READY);
                }
                if (pHopperCount > 0) {
                    punchStatus = STATUS_READY;
                    SendChar(STATUS_PUNCH_READY);
                } else {
                    punchStatus = STATUS_NOT_READY;
                    SendChar(STATUS_PUNCH_NOT_READY);
                }
                idx = 0;
                break;

            case PACTION_POWER_OFF:
                ibm1620PowerState = STATE_POWER_OFF;
                break;

            case PACTION_POWER_ON:
                ibm1620PowerState = STATE_POWER_ON;
                break;

            case PACTION_REQUEST_STATUS:
                SendStatus();
                break;

            case PACTION_SHUTDOWN:
                kill(getpid(), SIGTERM);
                break;

            case PACTION_ERROR:
                printf("CommIO bad received character = %c (0x%2x)\n", chr, chr);
                break;

            default:
                printf("CommIO invalid protocol action table entry\n");
                break;
        }
    }

    pthread_exit(NULL);
}

// ReceiveChar
char ReceiveChar(void) {
    char achar;

    while (TRUE) {
        if (!commIORunning) return 0;
        if ((commFile < 0) && !OpenCommIO()) continue;
        if (read(commFile, &achar, 1) <= 0) {
            CloseCommIO();
            continue;
        }
        if ((achar < 32) || (achar > 127)) continue;
        break;
    }

    return achar;
}

// SendChar
void SendChar(char achar) {

    while (TRUE) {
        if (!commIORunning) return;
        if ((commFile < 0) && !OpenCommIO()) continue;
        if (write(commFile, &achar, 1) <= 0) {
            CloseCommIO();
            continue;
        }
        break;
    }
}

// SendChars
void SendChars(char *achars) {
    size_t len = strlen(achars);

    if (len == 0) return;
    while (TRUE) {
        if (!commIORunning) return;
        if ((commFile < 0) && !OpenCommIO()) continue;
        if (write(commFile, achars, len) <= 0) {
            CloseCommIO();
            continue;
        }
        break;
    }
}

// SendStatus
void SendStatus(void) {

    // Send reader status
    if (readerStatus == STATUS_READY) {
        SendChar(STATUS_READER_READY);
    } else if (readerStatus == STATUS_NOT_READY) {
        SendChar(STATUS_READER_NOT_READY);
    } else if (readerStatus == STATUS_CHECK) {
        SendChar(STATUS_READER_CHECK);
    }

    // Send last card status
    if (readerLastCard) {
        SendChar(STATUS_LAST_CARD);
    }

    // Send punch status
    if (punchStatus == STATUS_READY) {
        SendChar(STATUS_PUNCH_READY);
    } else if (punchStatus == STATUS_NOT_READY) {
        SendChar(STATUS_PUNCH_NOT_READY);
    } else if (punchStatus == STATUS_CHECK) {
        SendChar(STATUS_PUNCH_CHECK);
    }
}
