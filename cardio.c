//======================================================================================================================
//
//  cardio.c - card i/o
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
#include "audio.h"
#include "cardio.h"
#include "commio.h"
#include "display.h"
#include "select.h"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>


// Function declarations
void *CardIO(void *);
void ReaderOpen(void);
void ReaderClose(void);
void PunchOpen(void);
void PunchClose(void);


// Data
bool cardIORunning = FALSE;
pthread_t cardIOThread = 0;


// CardIOStart
void CardIOStart(void) {
    int rc;

    cardIORunning = TRUE;

    if ((rc = pthread_create(&cardIOThread, NULL, CardIO, NULL)) != 0) {
        (void)printf("Failed to create card I/O thread: %s\n", strerror(rc));
        exit(-1);
    }
}

// CardIOStop
void CardIOStop(void) {

    cardIORunning = FALSE;

    (void)pthread_kill(cardIOThread, SIGUSR1);
    (void)pthread_join(cardIOThread, NULL);
}

// CardIO
void *CardIO(void *arg) {

    // Name this thread
    pthread_t threadId = pthread_self();
    (void)pthread_setname_np(threadId, "CardIO");

    // Lock this thread on CPU 2
    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(2, &cpus);
    if (pthread_setaffinity_np(threadId, sizeof(cpu_set_t), &cpus) != 0) {
        (void)printf("CardIO cpu affinity failed: %s\n", strerror(errno));
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

    (void)umount(readerMount);
    (void)umount(punchMount);

    readerStatus = STATUS_NOT_READY;
    SendChar(STATUS_READER_NOT_READY);
    SendChar(STATUS_READER_NOT_BUSY);
    punchStatus = STATUS_NOT_READY;
    SendChar(STATUS_PUNCH_NOT_READY);
    SendChar(STATUS_PUNCH_NOT_BUSY);

    display = DISPLAY_READER_PUNCH;

    while (cardIORunning) {

        if (access(readerDev, F_OK) == 0) {
            if (!readerUSBPresent) {
                readerUSBPresent = TRUE;
                ReaderOpen();
            }
        } else {
            if (readerUSBPresent) {
                readerUSBPresent = FALSE;
                ReaderClose();
            }
        }

        if (access(punchDev, F_OK) == 0) {
            if (!punchUSBPresent) {
                punchUSBPresent = TRUE;
                PunchOpen();
            }
        } else {
            if (punchUSBPresent) {
                punchUSBPresent = FALSE;
                PunchClose();
            }
        }

        if ((readerStatus == STATUS_READY) && (readTrigger != TYPE_NONE)) {
            ReadCard(readTrigger);
            readTrigger = TYPE_NONE;
        }

        if ((punchStatus == STATUS_READY) && (punchTrigger != TYPE_NONE)) {
            PunchCard(punchTrigger);
            punchTrigger = TYPE_NONE;
        }

        (void)usleep(100);
    }

    display = DISPLAY_UNKNOWN;

    ReaderClose();
    PunchClose();

    pthread_exit(NULL);
}

// ReaderOpen
void ReaderOpen(void) {
    char rbuf[101];
    int nchar;
    int cards = 0;
    char buf[1024];
    int cnt;
    bool more = FALSE;

    if ((nchar = readlink(readerDev, rbuf, 100)) > 0) {

        rbuf[nchar] = 0;
        (void)strcpy(readerDisk, "/dev/");
        (void)strcat(readerDisk, &rbuf[6]);

        if (mount(readerDisk, readerMount, "vfat", 0, NULL) == 0) {
            if (SelectFile()) {
                if ((readerFile = open(readerFilename, O_RDONLY)) != -1) {
                    while ((cnt = read(readerFile, buf, 1024)) > 0) {
                        for (int i = 0; i < cnt; ++i) {
                            if (buf[i] == '\n') ++cards;
                            if (buf[i] < 32) {
                            more = FALSE;
                            } else {
                            more = TRUE;
                            }
                        }
                    }
                    (void)close(readerFile);
                    readerFile = -1;
                    if (more) ++cards;
                    rHopperCount = cards;
                    readerPosition = 0;
                    readerLastCard = FALSE;
                    rAnimateControl = 0;
                    if (rHopperCount > 0) {
                    readerStatus = STATUS_READY;
                    SendChar(STATUS_READER_READY);
                    }
                } else {
                    printf("ReaderOpen open error = %s\n", strerror(errno));
                }
            } else {
                printf("ReaderOpen no file\n");
            }

            (void)umount(readerMount);

        } else {
            printf("ReaderOpen mount error = %s\n", strerror(errno));
        }

    } else {
        printf("ReaderOpen readlink error = %s\n", strerror(errno));
    }
}

// ReaderClose
void ReaderClose(void) {

    readerStatus = STATUS_NOT_READY;
    if (cardIORunning) SendChar(STATUS_READER_NOT_READY);

    rHopperCount = 0;
    rStackerCount = 0;
    rErrStackerCount = 0;
    rAnimateControl = 0;
    rErrAnimateControl = 0;
}

// PunchOpen
void PunchOpen(void) {
    char rbuf[101];
    int nchar;

    if ((nchar = readlink(punchDev, rbuf, 100)) > 0) {

        rbuf[nchar] = 0;
        (void)strcpy(punchDisk, "/dev/");
        (void)strcat(punchDisk, &rbuf[6]);

        if (mount(punchDisk, punchMount, "vfat", 0, NULL) == 0) {

            (void)strcpy(punchFilename, punchMount);
            (void)strcat(punchFilename, "/output.crd");
            if ((punchFile = open(punchFilename, O_WRONLY | O_TRUNC | O_CREAT, 0644)) != -1) {
                (void)close(punchFile);
                punchFile = -1;
                pHopperCount = HOPPER_SIZE;
                punchPosition = 0;
                pAnimateControl = 0;
                punchStatus = STATUS_READY;
                SendChar(STATUS_PUNCH_READY);
            } else {
                printf("PunchOpen open error = %s\n", strerror(errno));
            }

            (void)umount(punchMount);

        } else {
            printf("PunchOpen mount error = %s\n", strerror(errno));
        }

    } else {
        printf("PunchOpen readlink error = %s\n", strerror(errno));
    }
}

// PunchClose
void PunchClose(void) {

    punchStatus = STATUS_NOT_READY;
    if (cardIORunning) SendChar(STATUS_PUNCH_NOT_READY);

    pHopperCount = 0;
    pStackerCount = 0;
    pErrStackerCount = 0;
    pAnimateControl = 0;
    pErrAnimateControl = 0;
}

// ReadCard
void ReadCard(int type) {
    char chr;
    char chr2;
    int act;
    int cnt;
    int bptr;
    int cptr;
    char buf[130];
    bool err;

    if (mount(readerDisk, readerMount, "vfat", 0, NULL) == -1) {
        printf("ReadCard mount error = %s\n", strerror(errno));
        readerStatus = STATUS_CHECK;
        SendChar(STATUS_READER_CHECK);
        return;
    }

    if ((readerFile = open(readerFilename, O_RDONLY)) == -1) {
        printf("ReadCard open error = %s\n", strerror(errno));
        (void)umount(readerMount);
        readerStatus = STATUS_CHECK;
        SendChar(STATUS_READER_CHECK);
        return;
    }

    if (lseek(readerFile, readerPosition, SEEK_SET) == -1) {
        printf("ReadCard lseek error = %s\n", strerror(errno));
        (void)close(readerFile);
        (void)umount(readerMount);
        readerStatus = STATUS_CHECK;
        SendChar(STATUS_READER_CHECK);
        return;
    }

    cnt = read(readerFile, buf, 128);
    (void)close(readerFile);
    (void)umount(readerMount);

    if (cnt == 0) {
        readerStatus = STATUS_NOT_READY;
        SendChar(STATUS_READER_NOT_READY);
        return;
    }

    cptr = 0;
    if (type == TYPE_NUMERIC) {
        memset((void *)readerCard, '0', 80);
    } else /* type == TYPE_ALPHAMERIC */ {
        memset((void *)readerCard, ' ', 80);
    }
    readerCard[80] = 0;

    buf[cnt] = '\n';
    buf[cnt + 1] = 0;
    err = FALSE;
    bptr = 0;
    while ((chr = buf[bptr++] & 0x7f) != '\n') {

        if (type == TYPE_NUMERIC) {
            act = numericReaderActions[(int)chr][0];
            chr2 = numericReaderActions[(int)chr][1];
        } else /* type == TYPE_ALPHAMERIC */ {
            act = alphamericReaderActions[(int)chr][0];
            chr2 = alphamericReaderActions[(int)chr][1];
        }

        switch (act) {

            case ACTION_IGNORE:
                break;

            case ACTION_STORE:
                if (cptr < 80) {
                    readerCard[cptr++] = chr2;
                } else {
                    err = TRUE;
                }
                break;

            case ACTION_ERROR:
                if (cptr < 80) {
                    readerCard[cptr++] = DATA_INVALID_CHARACTER;
                }
                err = TRUE;
                break;

            default:
                (void)printf("Mapping table error\n");
                break;
        }

        ++readerPosition;
    }

    ++readerPosition;

    if (rHopperCount == 1) {
        readerLastCard = TRUE;
        SendChar(STATUS_LAST_CARD);
        readerStatus = STATUS_NOT_READY;
        SendChar(STATUS_READER_NOT_READY);
    }

    SendChars(readerCard);

    if (!err) {
        AnimateCardRead();
    } else {
        AnimateCardReadError();
        readerStatus = STATUS_CHECK;
        SendChar(STATUS_READER_CHECK);
    }
    PlayReadCard();
}

// PunchCard
void PunchCard(int type) {
    int last;
    char buf[82];
    bool err;

    if (mount(punchDisk, punchMount, "vfat", 0, NULL) == -1) {
        printf("PunchCard mount error = %s\n", strerror(errno));
        punchStatus = STATUS_CHECK;
        SendChar(STATUS_PUNCH_CHECK);
        return;
    }

    if ((punchFile = open(punchFilename, O_WRONLY | O_APPEND)) == -1) {
        printf("PunchCard open error = %s\n", strerror(errno));
        (void)umount(punchMount);
        punchStatus = STATUS_CHECK;
        SendChar(STATUS_PUNCH_CHECK);
        return;
    }

    err = FALSE;
    last = -1;
    for (int i = 0; i < 80; ++i) {
        if (type == TYPE_NUMERIC) {
            buf[i] = numericPunchMap[punchCard[i] & 0x7f];
        } else {
            buf[i] = alphamericPunchMap[punchCard[i] & 0x7f];
        }
        if (buf[i] == '?') err = TRUE;
        if (buf[i] != ' ') last = i;
    }
    buf[++last] = '\n';
    buf[last + 1] = 0;

    (void)write(punchFile, buf, last + 1);
    (void)close(punchFile);
    (void)umount(punchMount);

    if (!err) {
        AnimateCardPunch();
    } else {
        AnimateCardPunchError();
        punchStatus = STATUS_CHECK;
        SendChar(STATUS_PUNCH_CHECK);
    }
    PlayPunchCard();
}
