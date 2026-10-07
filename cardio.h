//======================================================================================================================
//
//  cardio.h - card i/o
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

#ifndef CARDIO_H_
#define CARDIO_H_

#include "defines.h"

// Function declarations
void CardIOStart(void);
void CardIOStop(void);

void ReadCard(int);
void PunchCard(int);

#endif /* CARDIO_H_ */
