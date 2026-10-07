//======================================================================================================================
//
//  audio.h - sound effects
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

#ifndef AUDIO_H_
#define AUDIO_H_

#include "defines.h"

// Function declarations
void AudioStart(void);
void AudioStop(void);
void PlayButtonDown(void);
void PlayButtonUp(void);
void PlayReadCard(void);
void PlayPunchCard(void);

#endif /* AUDIO_H_ */
