//======================================================================================================================
//
//  data.h - global data
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

#ifndef DATA_H_
#define DATA_H_

#include "defines.h"

#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


// =================================  Common Types  =================================

// Miscellaneous types
typedef unsigned char bool;


// =================================  Shared Data  =================================

// Version
extern volatile int versionMajor;
extern volatile int versionMinor;
extern volatile int versionBuild;
extern volatile char *versionDate;

// Card counts
extern volatile int pHopperCount;
extern volatile int pStackerCount;
extern volatile int pErrStackerCount;
extern volatile int mStackerCount;
extern volatile int rErrStackerCount;
extern volatile int rStackerCount;
extern volatile int rHopperCount;

// IBM 1620 states
extern volatile int ibm1620PowerState;
extern volatile int ibm1620RunState;

// IBM 1622 status
extern volatile bool running;

// Display synchronization
extern volatile int display;
extern volatile pthread_mutex_t displayLock;
extern volatile int selectedItem;
extern volatile int selectedFile;


// =================================  Display Data  =================================

// Textures
extern SDL_Texture *splashTexture;

extern SDL_Texture *backgroundTexture;

extern SDL_Texture *punchHopperSideTexture;
extern SDL_Texture *punchHopperSideNoCoverTexture;
extern SDL_Texture *punchStackerSideTexture;
extern SDL_Texture *readStackerSideTexture;
extern SDL_Texture *readHopperSideTexture;
extern SDL_Texture *readHopperSideNoCoverTexture;

extern SDL_Texture *leftCardTexture;
extern SDL_Texture *leftCardCornerTexture;
extern SDL_Texture *middleCardTexture;
extern SDL_Texture *middleCardCornerTexture;
extern SDL_Texture *rightCardTexture;
extern SDL_Texture *rightCardCornerTexture;

extern SDL_Texture *leftCardStackTexture;
extern SDL_Texture *middleCardStackTexture;
extern SDL_Texture *rightCardStackTexture;

extern SDL_Texture *loadOffTexture;
extern SDL_Texture *loadOnTexture;
extern SDL_Texture *readerTexture;
extern SDL_Texture *punchTexture;
extern SDL_Texture *readyOffTexture;
extern SDL_Texture *readyOnTexture;
extern SDL_Texture *checkOffTexture;
extern SDL_Texture *checkOnTexture;
extern SDL_Texture *resetOffTexture;
extern SDL_Texture *resetOnTexture;

extern SDL_Texture *directoryTexture;
extern SDL_Texture *fileTexture;
extern SDL_Texture *backupOffTexture;
extern SDL_Texture *backupOnTexture;
extern SDL_Texture *backupSelectedTexture;
extern SDL_Texture *upUpArrowOffTexture;
extern SDL_Texture *upUpArrowOnTexture;
extern SDL_Texture *upUpArrowSelectedTexture;
extern SDL_Texture *upArrowOffTexture;
extern SDL_Texture *upArrowOnTexture;
extern SDL_Texture *upArrowSelectedTexture;
extern SDL_Texture *downArrowOffTexture;
extern SDL_Texture *downArrowOnTexture;
extern SDL_Texture *downArrowSelectedTexture;
extern SDL_Texture *downDownArrowOffTexture;
extern SDL_Texture *downDownArrowOnTexture;
extern SDL_Texture *downDownArrowSelectedTexture;
extern SDL_Texture *itemSelectedTexture;

// Fonts
extern TTF_Font *directoryFileFont;
extern TTF_Font *extensionFont;

// Card values
extern const Sint16 leftCardX[5];
extern const Sint16 leftCardY[5];
extern const Sint16 leftCardLines[15][4];
extern const Sint16 leftCardCornerX[3];
extern const Sint16 leftCardCornerY[3];
extern const Sint16 middleCardX[5];
extern const Sint16 middleCardY[5];
extern const Sint16 middleCardLines[15][4];
extern const Sint16 middleCardCornerX[3];
extern const Sint16 middleCardCornerY[3];
extern const Sint16 rightCardX[5];
extern const Sint16 rightCardY[5];
extern const Sint16 rightCardLines[15][4];
extern const Sint16 rightCardCornerX[3];
extern const Sint16 rightCardCornerY[3];

// Hopper and stacker values
extern const Sint16 punchHopperX1[6];
extern const Sint16 punchHopperY1[6];
extern const Sint16 stackerX1[6];
extern const Sint16 stackerY1[6];
extern const Sint16 readHopperX1[6];
extern const Sint16 readHopperY1[6];
extern const Sint16 hopperStackerLines1[26][4];

extern const Sint16   punchHopperSideX3[4];
extern const Sint16   punchHopperSideY3[4];
extern const SDL_Rect punchHopperSideRect3;
extern const Sint16   punchHopperSideLines3[4][4];
extern const Sint16   punchHopperCoverX3[3];
extern const Sint16   punchHopperCoverY3[3];

extern const Sint16   punchStackerSideX3[4];
extern const Sint16   punchStackerSideY3[4];
extern const SDL_Rect punchStackerSide2Rect3;
extern const Sint16   punchStackerSideLines3[4][4];
extern const Sint16   punchStackerCoverX3[3];
extern const Sint16   punchStackerCoverY3[3];

extern const Sint16   readStackerSideX3[4];
extern const Sint16   readStackerSideY3[4];
extern const SDL_Rect readStackerSide5Rect3;
extern const Sint16   readStackerSideLines3[4][4];
extern const Sint16   readStackerCoverX3[3];
extern const Sint16   readStackerCoverY3[3];

extern const Sint16   readHopperSideX3[4];
extern const Sint16   readHopperSideY3[4];
extern const SDL_Rect readHopperSideRect3;
extern const Sint16   readHopperSideLines3[4][4];
extern const Sint16   readHopperCoverX3[3];
extern const Sint16   readHopperCoverY3[3];

extern const SDL_Rect punchStackerSide3Rect5;
extern const SDL_Rect readStackerSide4Rect5;

// Card stack values
struct stack_struct {
    SDL_Texture **stack;
    Sint16        width;
    Sint16        x;
    Sint16        y;
    Sint16        max;
    float         ratio;
    SDL_Texture **corner;
    Sint16        cornerX;
    Sint16        cornerY;
};
extern const struct stack_struct stacks[7];

// Card animation values
extern volatile int pAnimateControl;
extern volatile int pErrAnimateControl;
extern volatile int rErrAnimateControl;
extern volatile int rAnimateControl;

// Card animation locations
extern const int pAnimateLocations[5][2];
extern const int pErrAnimateLocations[6][2];
extern const int rErrAnimateLocations[6][2];
extern const int rAnimateLocations[5][2];

// Control panel values
extern const Sint16 panel1X[4];
extern const Sint16 panel1Y[4];
extern const Sint16 panel2X[4];
extern const Sint16 panel2Y[4];
extern const Sint16 panelLines[7][4];

// Color values
extern const SDL_Color panelColor;
extern const SDL_Color titleColor;
extern const SDL_Color lightOffColor;
extern const SDL_Color lightOnColor;
extern const SDL_Color lightErrorColor;
extern const SDL_Color loadTextColor;
extern const SDL_Color loadBackgroundColor;
extern const SDL_Color resetTextColor;
extern const SDL_Color resetBackgroundColor;
extern const SDL_Color fileSelectColor;

// Load button values
extern const SDL_Rect loadRect;
extern const Sint16   loadOff1X[4];
extern const Sint16   loadOff1Y[4];
extern const Sint16   loadOff2X[4];
extern const Sint16   loadOff2Y[4];
extern const Sint16   loadOffLines[7][4];
extern const SDL_Rect loadOffRect;
extern const Sint16   loadOnX[4];
extern const Sint16   loadOnY[4];
extern const Sint16   loadOnLines[4][4];
extern const SDL_Rect loadOnRect;

// Reset button values
extern const SDL_Rect resetRect;
extern const Sint16   resetOff1X[4];
extern const Sint16   resetOff1Y[4];
extern const Sint16   resetOff2X[4];
extern const Sint16   resetOff2Y[4];
extern const Sint16   resetOffLines[7][4];
extern const SDL_Rect resetOffRect;
extern const Sint16   resetOnX[4];
extern const Sint16   resetOnY[4];
extern const Sint16   resetOnLines[4][4];
extern const SDL_Rect resetOnRect;

// Indicator light values
extern const SDL_Rect punchRect;
extern const Sint16   punchLine[4];
extern const SDL_Rect punchReadyRect;
extern const SDL_Rect punchCheckRect;

extern const SDL_Rect readerRect;
extern const Sint16   readerLine[4];
extern const SDL_Rect readerReadyRect;
extern const SDL_Rect readerCheckRect;

// Button values
extern volatile int buttonState;

// File select values
extern const SDL_Rect backupRect;
extern const SDL_Rect upUpArrowRect;
extern const SDL_Rect upArrowRect;
extern const SDL_Rect downArrowRect;
extern const SDL_Rect downDownArrowRect;

// File select layout values
struct layout_struct {
    Sint16        icon_x;
    Sint16        icon_y;
    Sint16        icon_center_x;
    Sint16        icon_center_y;
    Sint16        text_x;
    Sint16        text_y;
    float         touch_x1;
    float         touch_y1;
    float         touch_x2;
    float         touch_y2;
};
extern const struct layout_struct layout[11];

// File select entry values
struct item_struct {
    char type;
    char extension[4];
    char filename[ITEM_LINE_LENGTH + 1];
};
extern char title[TITLE_LINE_LENGTH + 1];
extern int numEntries;
extern int firstEntry;
extern struct item_struct items[SELECT_MAX_ENTRIES];


// =================================  Card I/O Data  =================================

extern const char readerDev[];
extern const char punchDev[];

extern const char readerMount[];
extern const char punchMount[];

extern volatile bool readerUSBPresent;
extern volatile bool readerOpened;
extern volatile char readerDisk[16];
extern volatile char readerFilename[PATH_MAX + 1];
extern volatile int readerFile;
extern volatile long readerPosition;
extern volatile int readRequest;
extern volatile int readRequestCounter;
extern volatile int readTrigger;
extern volatile char readerCard[81];
extern volatile int readerStatus;
extern volatile bool readerLastCard;

extern volatile bool punchUSBPresent;
extern volatile bool punchOpened;
extern volatile char punchDisk[16];
extern volatile char punchFilename[PATH_MAX + 1];
extern volatile int punchFile;
extern volatile long punchPosition;
extern volatile int punchRequest;
extern volatile int punchRequestCounter;
extern volatile int punchTrigger;
extern volatile char punchCard[81];
extern volatile int punchStatus;

extern const int numericReaderActions[128][2];
extern const int alphamericReaderActions[128][2];

extern const int numericPunchMap[128];
extern const int alphamericPunchMap[128];


// =================================  Communication I/O Data  =================================

extern const int protocolCDActions[128];

#endif /* DATA_H_ */
