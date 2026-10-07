//======================================================================================================================
//
//  commio.h - communications i/o
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

#ifndef COMMIO_H_
#define COMMIO_H_

#include "defines.h"

// Function declarations
void CommIOStart(void);
void CommIOStop(void);
bool OpenCommIO(void);
void CloseCommIO(void);
char ReceiveChar(void);
void SendChar(char);
void SendChars(char *);
void SendStatus(void);

#endif /* COMMIO_H_ */
