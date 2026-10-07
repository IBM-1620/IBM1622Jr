//======================================================================================================================
//
//  display.h - display support
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

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "defines.h"

// Function declarations
void DisplayOpen(void);
void DisplayClose(void);

void ShowScreen(int);

void AnimateCardRead(void);
void AnimateCardReadError(void);
void AnimateCardPunch(void);
void AnimateCardPunchError(void);

#endif /* DISPLAY_H_ */
