//======================================================================================================================
//
//  audio.c - sound effects
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

#include <SDL2/SDL_mixer.h>


// Data
bool audioRunning = FALSE;
Mix_Chunk *buttonDownSound = NULL;
Mix_Chunk *buttonUpSound = NULL;
Mix_Chunk *readSound = NULL;
Mix_Chunk *punchSound = NULL;


// AudioStart
void AudioStart(void) {
  
    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 512) < 0) {
        printf("Mix_OpenAudio() failed: %s\n", Mix_GetError());
        return;
    }

    if (Mix_AllocateChannels(16) < 0) {
        printf("Mix_AllocateChannels() failed: %s\n", Mix_GetError());
        return;
    }

    buttonDownSound = Mix_LoadWAV(BUTTONDOWN_FILENAME);
    if (buttonDownSound == NULL) {
        printf("Load ButtonDown.wav failed: %s\n", Mix_GetError());
        return;
    }

    buttonUpSound = Mix_LoadWAV(BUTTONUP_FILENAME);
    if (buttonUpSound == NULL) {
        printf("Load ButtonUp.wav failed: %s\n", Mix_GetError());
        Mix_FreeChunk(buttonDownSound);
        return;
    }

    readSound = Mix_LoadWAV(READ_FILENAME);
    if (readSound == NULL) {
        printf("Load Read.wav failed: %s\n", Mix_GetError());
        Mix_FreeChunk(buttonDownSound);
        Mix_FreeChunk(buttonUpSound);
        return;
    }

    punchSound = Mix_LoadWAV(PUNCH_FILENAME);
    if (punchSound == NULL) {
        printf("Load Punch.wav failed: %s\n", Mix_GetError());
        Mix_FreeChunk(buttonDownSound);
        Mix_FreeChunk(buttonUpSound);
        Mix_FreeChunk(readSound);
        return;
    }

    audioRunning = TRUE;
}

// AudioStop
void AudioStop(void) {

    audioRunning = FALSE;

    Mix_FreeChunk(buttonDownSound);
    Mix_FreeChunk(buttonUpSound);
    Mix_FreeChunk(readSound);
    Mix_FreeChunk(punchSound);

    Mix_CloseAudio();
}

// PlayButtonDown
void PlayButtonDown(void) {

    if (audioRunning) {
        (void)Mix_PlayChannel(-1, buttonDownSound, 0);
    }
}

// PlayButtonUp
void PlayButtonUp(void) {

    if (audioRunning) {
        (void)Mix_PlayChannel(-1, buttonUpSound, 0);
    }
}

// PlayReadCard
void PlayReadCard(void) {

    if (audioRunning) {
        (void)Mix_PlayChannel(-1, readSound, 0);
    }
}

// PlayPunchCard
void PlayPunchCard(void) {

    if (audioRunning) {
        (void)Mix_PlayChannel(-1, punchSound, 0);
    }
}
