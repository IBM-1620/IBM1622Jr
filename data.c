//======================================================================================================================
//
//  data.c - global data
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

#include <limits.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


// =================================  Shared Data  =================================

// Version
volatile int versionMajor  = VERSION_MAJOR;
volatile int versionMinor  = VERSION_MINOR;
volatile int versionBuild  = 0;
volatile char *versionDate = (char*)__DATE__;

// Card counts
volatile int pHopperCount     = 0;
volatile int pStackerCount    = 0;
volatile int pErrStackerCount = 0;
volatile int mStackerCount    = 0;
volatile int rErrStackerCount = 0;
volatile int rStackerCount    = 0;
volatile int rHopperCount     = 0;

// IBM 1620 states
volatile int ibm1620PowerState = STATE_UNKNOWN;
volatile int ibm1620RunState   = STATE_UNKNOWN;

// IBM 1622 status
volatile bool running = FALSE;

// Display synchronization
volatile int display                 = DISPLAY_UNKNOWN;
volatile pthread_mutex_t displayLock = PTHREAD_MUTEX_INITIALIZER;
volatile int selectedItem            = SELECTED_NONE;
volatile int selectedFile            = SELECTED_NONE;



// =================================  Display Data  =================================

// Textures
SDL_Texture *splashTexture = NULL;

SDL_Texture *backgroundTexture             = NULL;

SDL_Texture *punchHopperSideTexture        = NULL;
SDL_Texture *punchHopperSideNoCoverTexture = NULL;
SDL_Texture *punchStackerSideTexture       = NULL;
SDL_Texture *readStackerSideTexture        = NULL;
SDL_Texture *readHopperSideTexture         = NULL;
SDL_Texture *readHopperSideNoCoverTexture  = NULL;

SDL_Texture *leftCardTexture         = NULL;
SDL_Texture *leftCardCornerTexture   = NULL;
SDL_Texture *middleCardTexture       = NULL;
SDL_Texture *middleCardCornerTexture = NULL;
SDL_Texture *rightCardTexture        = NULL;
SDL_Texture *rightCardCornerTexture  = NULL;

SDL_Texture *leftCardStackTexture   = NULL;
SDL_Texture *middleCardStackTexture = NULL;
SDL_Texture *rightCardStackTexture  = NULL;

SDL_Texture *loadOffTexture  = NULL;
SDL_Texture *loadOnTexture   = NULL;
SDL_Texture *readerTexture   = NULL;
SDL_Texture *punchTexture    = NULL;
SDL_Texture *readyOffTexture = NULL;
SDL_Texture *readyOnTexture  = NULL;
SDL_Texture *checkOffTexture = NULL;
SDL_Texture *checkOnTexture  = NULL;
SDL_Texture *resetOffTexture = NULL;
SDL_Texture *resetOnTexture  = NULL;

SDL_Texture *directoryTexture             = NULL;
SDL_Texture *fileTexture                  = NULL;
SDL_Texture *backupOffTexture             = NULL;
SDL_Texture *backupOnTexture              = NULL;
SDL_Texture *backupSelectedTexture        = NULL;
SDL_Texture *upUpArrowOffTexture          = NULL;
SDL_Texture *upUpArrowOnTexture           = NULL;
SDL_Texture *upUpArrowSelectedTexture     = NULL;
SDL_Texture *upArrowOffTexture            = NULL;
SDL_Texture *upArrowOnTexture             = NULL;
SDL_Texture *upArrowSelectedTexture       = NULL;
SDL_Texture *downArrowOffTexture          = NULL;
SDL_Texture *downArrowOnTexture           = NULL;
SDL_Texture *downArrowSelectedTexture     = NULL;
SDL_Texture *downDownArrowOffTexture      = NULL;
SDL_Texture *downDownArrowOnTexture       = NULL;
SDL_Texture *downDownArrowSelectedTexture = NULL;
SDL_Texture *itemSelectedTexture          = NULL;

// Fonts
TTF_Font *directoryFileFont = NULL;
TTF_Font *extensionFont     = NULL;

// Card values
// cleanup spacing  DJB
const Sint16 leftCardX[5]           = { 2, 24, 134, 116, 82};
const Sint16 leftCardY[5]           = {65,  2,   2,  50, 65};
const Sint16 leftCardLines[15][4]   = {{  1, 66,  23,  1}, { 23,  1, 135,  1}, {135, 1, 117, 51}, {117, 51, 83, 66},
                                       { 83, 66,   1, 66}, {  2, 65,  24,  2}, {24,  2, 134,  2}, {134, 2, 116, 50},
                                       {116, 50,  82, 65}, { 82, 65,   2, 65}, { 3, 64,  25,  3}, { 25, 3, 133,  3},
                                       {133,  3, 115, 49}, {115, 49,  81, 64}, {81, 64,   3, 64}};
const Sint16 leftCardCornerX[3]     = { 0, 34, 34};
const Sint16 leftCardCornerY[3]     = {15,  0, 15};

const Sint16 middleCardX[5]         = { 2, 2, 112, 112, 82};
const Sint16 middleCardY[5]         = {65, 2,   2,  50, 65};
const Sint16 middleCardLines[15][4] = {{1, 66, 1, 1}, { 1, 1, 113, 1}, {113, 1, 113, 51}, {113, 51, 83, 66},
                                       {83, 66, 1, 66}, {2, 65, 2, 2}, { 2, 2, 112, 2}, {112, 2, 112, 50},
                                       {112, 50, 82, 65}, {82, 65, 2, 65}, {3, 64, 3, 3}, { 3, 3, 111, 3},
                                       {111, 3, 111, 49}, {111, 49, 81, 64}, {81, 64, 3, 64}};
const Sint16 middleCardCornerX[3]   = { 0, 30, 30};
const Sint16 middleCardCornerY[3]   = {15,  0, 15};

const Sint16 rightCardX[5]          = {24, 2, 112, 130, 104};
const Sint16 rightCardY[5]          = {65, 2,   2,  50,  65};
const Sint16 rightCardLines[15][4]  = {{23, 66, 1, 1}, {1, 1, 113, 1}, {113, 1, 131, 50}, {131, 50, 105, 66},
                                       {105, 66, 23, 66}, {24, 65, 2, 2}, {2, 2, 112, 2}, {112, 2, 130, 50},
                                       {130, 50, 104, 65}, {104, 65, 24, 65}, {25, 64, 3, 3}, {3, 3, 111, 3},
                                       {111, 3, 129, 50}, {129, 50, 103, 64}, {103, 64, 25, 64}};
const Sint16 rightCardCornerX[3]    = { 0, 26, 26};
const Sint16 rightCardCornerY[3]    = {15,  0, 15};

// Hopper and stacker values
const Sint16 punchHopperX1[6] = { 26, 141,  141,  119,   4,   4};
const Sint16 punchHopperY1[6] = {  4,   4,  251,  316, 316,  69};
const Sint16 stackerX1[6]     = {224, 799,  821,  821, 202, 202};
const Sint16 stackerY1[6]     = {329, 329,  394,  592, 592, 394};
const Sint16 readHopperX1[6]  = {882, 997, 1019, 1019, 904, 882};
const Sint16 readHopperY1[6]  = {  4,   4,   69,  316, 316, 251};
const Sint16 hopperStackerLines1[26][4] =
    {{  4,  69,    4, 316}, {  4,  69,   26,   4}, {   4, 316,   26, 251}, {  4, 316,  119, 316}, { 26,   4,  26, 251},
     { 26,   4,  141,   4}, { 26, 251,  119, 251},
     {202, 394,  202, 592}, {202, 394,  224, 329}, { 202, 592,  224, 527}, {202, 592,  821, 592}, {224, 329, 224, 527},
     {224, 329,  799, 329}, {224, 527,  799, 527}, { 799, 329,  799, 527}, {799, 329,  821, 394}, {799, 527, 821, 592},
     {821, 394,  821, 592},
     {882,   4,  997,   4}, {882, 251,  904, 316}, { 904, 251,  997, 251}, {904, 316, 1019, 316}, {997,   4, 997, 251},
     {997,   4, 1019,  69}, {997, 251, 1019, 316}, {1019,  69, 1019, 316}};

const Sint16   punchHopperSideX3[4]         = {  2,  24,  24,   2};
const Sint16   punchHopperSideY3[4]         = { 67,   2, 249, 314};
const SDL_Rect punchHopperSideRect3         = {117,   2,  27, 317};
const Sint16   punchHopperSideLines3[4][4]  = {{2, 67, 24, 2}, {24, 2, 24, 249}, {24, 249, 2, 314}, {2, 314, 2, 67}};
const Sint16   punchHopperCoverX3[3]        = {  2,  24,  24};
const Sint16   punchHopperCoverY3[3]        = {311, 249, 311};

const Sint16   punchStackerSideX3[4]        = {  2,  24,  24,   2};
const Sint16   punchStackerSideY3[4]        = { 67,   2, 200, 265};
const SDL_Rect punchStackerSide2Rect3       = {315, 327,  27, 268};
const Sint16   punchStackerSideLines3[4][4] = {{2, 67, 24, 2}, {24, 2, 24, 200}, {24, 200, 2, 265}, {2, 265, 2, 67}};
const Sint16   punchStackerCoverX3[3]       = {  2,  24,  24};
const Sint16   punchStackerCoverY3[3]       = {262, 200, 262};

const Sint16   readStackerSideX3[4]         = {  2,  24,  24,   2};
const Sint16   readStackerSideY3[4]         = {  2,  67, 265, 200};
const SDL_Rect readStackerSide5Rect3        = {682, 327,  27, 268};
const Sint16   readStackerSideLines3[4][4]  = {{2, 2, 24, 67}, {24, 67, 24, 265}, {24, 265, 2, 200}, {2, 200, 2, 2}};
const Sint16   readStackerCoverX3[3]        = {  2,   2,  24};
const Sint16   readStackerCoverY3[3]        = {262, 200, 262};

const Sint16   readHopperSideX3[4]          = {  2,  24,  24,   2};
const Sint16   readHopperSideY3[4]          = {  2,  67, 314, 249};
const SDL_Rect readHopperSideRect3          = {880,   2,  27, 317};
const Sint16   readHopperSideLines3[4][4]   = {{2, 2, 24, 67}, {24, 67, 24, 314}, {24, 314, 2, 249}, {2, 249, 2, 2}};
const Sint16   readHopperCoverX3[3]         = {  2,   2,  24};
const Sint16   readHopperCoverY3[3]         = {311, 249, 311};

const SDL_Rect punchStackerSide3Rect5 = {430, 327,  27, 268};
const SDL_Rect readStackerSide4Rect5  = {567, 327,  27, 268};

// Card stack values
const struct stack_struct stacks[7] = {{&leftCardStackTexture,   138,   4, 316, 1200, 0.2025, &leftCardCornerTexture,
                                        86, 300},
                                       {&leftCardStackTexture,   138, 202, 592, 1000, 0.1940, &leftCardCornerTexture,
                                        285, 576},
                                       {&leftCardStackTexture,   138, 317, 592, 1000, 0.1940, &leftCardCornerTexture,
                                        400, 576},
                                       {&middleCardStackTexture, 116, 454, 592, 1000, 0.1940, &middleCardCornerTexture,
                                        537, 576},
                                       {&rightCardStackTexture,  134, 569, 592, 1000, 0.1940, &rightCardCornerTexture,
                                        676, 576},
                                       {&rightCardStackTexture,  134, 684, 592, 1000, 0.1940, &rightCardCornerTexture,
                                        791, 576},
                                       {&rightCardStackTexture,  134, 882, 316, 1200, 0.2025, &rightCardCornerTexture,
                                        989, 300}};

// Card animation values
volatile int pAnimateControl    = 0;
volatile int pErrAnimateControl = 0;
volatile int rErrAnimateControl = 0;
volatile int rAnimateControl    = 0;

// Card animation locations
const int pAnimateLocations[5][2]    = {{ 64, 316}, {202, 316}, {202, 385}, {202, 454}, {202, 523}};
const int pErrAnimateLocations[6][2] = {{ 64, 316}, {191, 316}, {317, 316}, {317, 385}, {317, 454}, {317, 523}};
const int rErrAnimateLocations[6][2] = {{830, 316}, {709, 316}, {569, 316}, {569, 385}, {569, 454}, {569, 523}};
const int rAnimateLocations[5][2]    = {{830, 316}, {684, 316}, {684, 385}, {684, 454}, {684, 523}};

// Control panel values
const Sint16 panel1X[4]       = {220, 234, 790, 804};
const Sint16 panel1Y[4]       = { 44,   4,   4,  44};
const Sint16 panel2X[4]       = {220, 220, 804, 804};
const Sint16 panel2Y[4]       = {234,  44,  44, 234};
const Sint16 panelLines[7][4] = {{234, 4, 790, 4}, {220, 44, 804, 44}, {220, 234, 804, 234}, {234, 4, 220, 44},
                                 {220, 44, 220, 234}, {790, 4, 804, 44}, {804, 44, 804, 234}};

// Color values
const SDL_Color panelColor           = {DISPLAY_PANEL_R, DISPLAY_PANEL_G, DISPLAY_PANEL_B, SDL_ALPHA_OPAQUE};
const SDL_Color titleColor           = {DISPLAY_TITLE_R, DISPLAY_TITLE_G, DISPLAY_TITLE_B, SDL_ALPHA_OPAQUE};
const SDL_Color lightOffColor        = {DISPLAY_LIGHT_OFF_R, DISPLAY_LIGHT_OFF_G, DISPLAY_LIGHT_OFF_B,
                                        SDL_ALPHA_OPAQUE};
const SDL_Color lightOnColor         = {DISPLAY_LIGHT_ON_R, DISPLAY_LIGHT_ON_G, DISPLAY_LIGHT_ON_B, SDL_ALPHA_OPAQUE};
const SDL_Color lightErrorColor      = {DISPLAY_LIGHT_ERROR_R, DISPLAY_LIGHT_ERROR_G, DISPLAY_LIGHT_ERROR_B,
                                        SDL_ALPHA_OPAQUE};
const SDL_Color loadTextColor        = {DISPLAY_LOAD_TEXT_R, DISPLAY_LOAD_TEXT_G, DISPLAY_LOAD_TEXT_B,
                                        SDL_ALPHA_OPAQUE};
const SDL_Color loadBackgroundColor  = {DISPLAY_LOAD_BACKGROUND_R, DISPLAY_LOAD_BACKGROUND_G, DISPLAY_LOAD_BACKGROUND_B,
                                       SDL_ALPHA_OPAQUE};
const SDL_Color resetTextColor       = {DISPLAY_RESET_TEXT_R, DISPLAY_RESET_TEXT_G, DISPLAY_RESET_TEXT_B,
                                        SDL_ALPHA_OPAQUE};
const SDL_Color resetBackgroundColor = {DISPLAY_RESET_BACKGROUND_R, DISPLAY_RESET_BACKGROUND_G,
                                        DISPLAY_RESET_BACKGROUND_B, SDL_ALPHA_OPAQUE};
const SDL_Color fileSelectColor      = {DISPLAY_BLACK_R, DISPLAY_BLACK_G, DISPLAY_BLACK_B, SDL_ALPHA_OPAQUE};

// Load button values
const SDL_Rect loadRect           = {464, 69, 96, 63};
const Sint16   loadOff1X[4]       = {  0,  4, 91, 95};
const Sint16   loadOff1Y[4]       = {  9,  0,  0,  9};
const Sint16   loadOff2X[4]       = {  0, 95, 95,  0};
const Sint16   loadOff2Y[4]       = {  9,  9, 61, 61};
const SDL_Rect loadOffRect        = { 10, 20, 76, 33};
const Sint16   loadOffLines[7][4] = {{4, 0, 91, 0}, {0, 9, 95, 9}, {0, 61, 95, 61}, {4, 0, 0, 9}, {0, 9, 0, 61},
                                     {91, 0, 95,  9}, {95,  9, 95, 61}};
const Sint16   loadOnX[4]         = {  0, 95, 95,  0};
const Sint16   loadOnY[4]         = {  0,  0, 52, 52};
const SDL_Rect loadOnRect         = { 10, 10, 76, 33};
const Sint16   loadOnLines[4][4]  = {{0, 0, 95, 0}, {95, 0, 95, 52}, {95, 52, 0, 52}, {0, 52, 0, 0}};

// Reset button values
const SDL_Rect resetRect           = {469, 155, 85, 54};
const Sint16   resetOff1X[4]       = {  0,   4, 80, 84};
const Sint16   resetOff1Y[4]       = {  9,   0,  0,  9};
const Sint16   resetOff2X[4]       = {  0,  84, 84,  0};
const Sint16   resetOff2Y[4]       = {  9,   9, 52, 52};
const SDL_Rect resetOffRect        = { 10,  20, 65, 24};
const Sint16   resetOffLines[7][4] = {{4, 0, 80, 0}, {0, 9, 84, 9}, {0, 52, 84, 52}, {4, 0, 0, 9}, {0, 9, 0, 52},
                                      {80, 0, 84, 9}, {84, 9, 84, 52}};
const Sint16   resetOnX[4]         = {  0,  84, 84,  0};
const Sint16   resetOnY[4]         = {  0,   0, 43, 43};
const SDL_Rect resetOnRect         = { 10,  10, 65, 24};
const Sint16   resetOnLines[4][4]  = {{0, 0, 84, 0}, {84, 0, 84, 43}, {84, 43, 0, 43}, {0, 43, 0, 0}};

// Indicator light values
const SDL_Rect punchRect       = {289,  69, 106,  35};
const Sint16   punchLine[4]    = {289, 105, 394, 105};
const SDL_Rect punchReadyRect  = {299, 130,  85,  28};
const SDL_Rect punchCheckRect  = {300, 181,  84,  28};

const SDL_Rect readerRect      = {620,  69, 124,  35};
const Sint16   readerLine[4]   = {620, 105, 743, 105};
const SDL_Rect readerReadyRect = {639, 130,  85,  28};
const SDL_Rect readerCheckRect = {640, 181,  84,  28};

// Button values
volatile int buttonState = BUTTON_NONE_PRESSED;

const SDL_Rect backupRect        = {971,   0, 53, 51};
const SDL_Rect upUpArrowRect     = {971,  56, 53, 73};
const SDL_Rect upArrowRect       = {971, 134, 53, 53};
const SDL_Rect downArrowRect     = {971, 469, 53, 53};
const SDL_Rect downDownArrowRect = {971, 527, 53, 73};

// File select layout values
const struct layout_struct layout[11] = {{ 4,  56, 28,  80, 60,  56, 0.000, 0.093, 0.942, 0.174},
                                         { 4, 105, 28, 129, 60, 105, 0.000, 0.175, 0.942, 0.256},
                                         { 4, 154, 28, 178, 60, 154, 0.000, 0.257, 0.942, 0.337},
                                         { 4, 203, 28, 227, 60, 203, 0.000, 0.338, 0.942, 0.419},
                                         { 4, 252, 28, 276, 60, 252, 0.000, 0.420, 0.942, 0.501},
                                         { 4, 301, 28, 325, 60, 301, 0.000, 0.502, 0.942, 0.582},
                                         { 4, 350, 28, 374, 60, 350, 0.000, 0.583, 0.942, 0.664},
                                         { 4, 399, 28, 423, 60, 399, 0.000, 0.665, 0.942, 0.746},
                                         { 4, 448, 28, 472, 60, 448, 0.000, 0.747, 0.942, 0.827},
                                         { 4, 497, 28, 521, 60, 497, 0.000, 0.828, 0.942, 0.909},
                                         { 4, 546, 28, 570, 60, 546, 0.000, 0.910, 0.942, 0.999}};

// File select item values
char title[TITLE_LINE_LENGTH + 1];
int numEntries  = 0;
int firstEntry = -1;
struct item_struct items[SELECT_MAX_ENTRIES];


// =================================  Card I/O Data  =================================

#if READER_USB == USB_UL
const char readerDev[] = "/dev/disk/by-path/platform-3f980000.usb-usb-0:1.1.2:1.0-scsi-0:0:0:0-part1";
#elif READER_USB == USB_UR
const char readerDev[] = "/dev/disk/by-path/platform-3f980000.usb-usb-0:1.3:1.0-scsi-0:0:0:0-part1";
#elif READER_USB == USB_LL
const char readerDev[] = "/dev/disk/by-path/platform-3f980000.usb-usb-0:1.1.3:1.0-scsi-0:0:0:0-part1";
#elif READER_USB == USB_LR
const char readerDev[] = "/dev/disk/by-path/platform-3f980000.usb-usb-0:1.2:1.0-scsi-0:0:0:0-part1";
#endif

#if PUNCH_USB == USB_UL
const char punchDev[] = "/dev/disk/by-path/platform-3f980000.usb-usb-0:1.1.2:1.0-scsi-0:0:0:0-part1";
#elif PUNCH_USB == USB_UR
const char punchDev[] = "/dev/disk/by-path/platform-3f980000.usb-usb-0:1.3:1.0-scsi-0:0:0:0-part1";
#elif PUNCH_USB == USB_LL
const char punchDev[] = "/dev/disk/by-path/platform-3f980000.usb-usb-0:1.1.3:1.0-scsi-0:0:0:0-part1";
#elif PUNCH_USB == USB_LR
const char punchDev[] = "/dev/disk/by-path/platform-3f980000.usb-usb-0:1.2:1.0-scsi-0:0:0:0-part1";
#endif

const char readerMount[] = "/media/usb-reader";
const char punchMount[] = "/media/usb-punch";

volatile bool readerUSBPresent = FALSE;
volatile bool readerOpened = FALSE;
volatile char readerDisk[16];
volatile char readerFilename[PATH_MAX + 1];
volatile int readerFile = -1;
volatile long readerPosition = 0;
volatile int readRequest = TYPE_NONE;
volatile int readRequestCounter = 0;
volatile int readTrigger = TYPE_NONE;
volatile char readerCard[81];
volatile int readerStatus = STATUS_NOT_READY;
volatile bool readerLastCard = FALSE;

volatile bool punchUSBPresent = FALSE;
volatile bool punchOpened = FALSE;
volatile char punchDisk[16];
volatile char punchFilename[PATH_MAX + 1];
volatile int punchFile = -1;
volatile long punchPosition = 0;
volatile int punchRequest = TYPE_NONE;
volatile int punchRequestCounter = 0;
volatile int punchTrigger = TYPE_NONE;
volatile char punchCard[81];
volatile int punchStatus = STATUS_NOT_READY;

const int numericReaderActions[128][2] =
//                 <NUL>                 <SOH>                 <STX>                 <ETX>
    {{ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<EOT>', '<ENQ>', '<ACK>', '<BEL>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   //  '<BS>',  '<HT>',  '<LF>',  '<VT>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   //  '<FF>',  '<CR>',  '<SO>',  '<SI>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<DLE>', '<DC1>', '<DC2>', '<DC3>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<DC4>', '<NAK>', '<SYN>', '<ETB>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<CAN>',  '<EM>', '<SUB>', '<ESC>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   //  '<FS>',  '<GS>',  '<RS>',  '<US>'
     { ACTION_STORE, ' '}, { ACTION_STORE, '!'}, { ACTION_STORE, '"'}, { ACTION_ERROR,   0},   //     ' ',     '!',     '"',     '#'
     { ACTION_STORE, '$'}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0},   //     '$',     '%',     '&',     '''
     { ACTION_STORE, '('}, { ACTION_STORE, ')'}, { ACTION_STORE, '*'}, { ACTION_STORE, '+'},   //     '(',     ')',     '*',     '+'
     { ACTION_STORE, ','}, { ACTION_STORE, '-'}, { ACTION_STORE, '.'}, { ACTION_STORE, '/'},   //     ',',     '-',     '.',     '/'
     { ACTION_STORE, '0'}, { ACTION_STORE, '1'}, { ACTION_STORE, '2'}, { ACTION_STORE, '3'},   //     '0',     '1',     '2',     '3'
     { ACTION_STORE, '4'}, { ACTION_STORE, '5'}, { ACTION_STORE, '6'}, { ACTION_STORE, '7'},   //     '4',     '5',     '6',     '7'
     { ACTION_STORE, '8'}, { ACTION_STORE, '9'}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0},   //     '8',     '9',     ':',     ';'
     { ACTION_ERROR,   0}, { ACTION_STORE, '='}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0},   //     '<',     '=',     '>',     '?'
     { ACTION_STORE, '@'}, { ACTION_STORE, 'A'}, { ACTION_STORE, 'B'}, { ACTION_STORE, 'C'},   //     '@',     'A',     'B',     'C'
     { ACTION_STORE, 'D'}, { ACTION_STORE, 'E'}, { ACTION_STORE, 'F'}, { ACTION_STORE, 'G'},   //     'D',     'E',     'F',     'G'
     { ACTION_STORE, 'H'}, { ACTION_STORE, 'I'}, { ACTION_STORE, 'J'}, { ACTION_STORE, 'K'},   //     'H',     'I',     'J',     'K'
     { ACTION_STORE, 'L'}, { ACTION_STORE, 'M'}, { ACTION_STORE, 'N'}, { ACTION_STORE, 'O'},   //     'L',     'M',     'N',     'O'
     { ACTION_STORE, 'P'}, { ACTION_STORE, 'Q'}, { ACTION_STORE, 'R'}, { ACTION_STORE, 'S'},   //     'P',     'Q',     'R',     'S'
     { ACTION_STORE, 'T'}, { ACTION_STORE, 'U'}, { ACTION_STORE, 'V'}, { ACTION_STORE, 'W'},   //     'T',     'U',     'V',     'W'
     { ACTION_STORE, 'X'}, { ACTION_STORE, 'Y'}, { ACTION_STORE, 'Z'}, { ACTION_ERROR,   0},   //     'X',     'Y',     'Z',     '['
     { ACTION_ERROR,   0}, { ACTION_STORE, ']'}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0},   //     '\',     ']',     '^',     '_'
     { ACTION_ERROR,   0}, { ACTION_STORE, 'A'}, { ACTION_STORE, 'B'}, { ACTION_STORE, 'C'},   //     '`',     'a',     'b',     'c'
     { ACTION_STORE, 'D'}, { ACTION_STORE, 'E'}, { ACTION_STORE, 'F'}, { ACTION_STORE, 'G'},   //     'd',     'e',     'f',     'g'
     { ACTION_STORE, 'H'}, { ACTION_STORE, 'I'}, { ACTION_STORE, 'J'}, { ACTION_STORE, 'K'},   //     'h',     'i',     'j',     'k'
     { ACTION_STORE, 'L'}, { ACTION_STORE, 'M'}, { ACTION_STORE, 'N'}, { ACTION_STORE, 'O'},   //     'l',     'm',     'n',     'o'
     { ACTION_STORE, 'P'}, { ACTION_STORE, 'Q'}, { ACTION_STORE, 'R'}, { ACTION_STORE, 'S'},   //     'p',     'q',     'r',     's'
     { ACTION_STORE, 'T'}, { ACTION_STORE, 'U'}, { ACTION_STORE, 'V'}, { ACTION_STORE, 'W'},   //     't',     'u',     'v',     'w'
     { ACTION_STORE, 'X'}, { ACTION_STORE, 'Y'}, { ACTION_STORE, 'Z'}, { ACTION_ERROR,   0},   //     'x',     'y',     'z',     '{'
     { ACTION_STORE, '|'}, { ACTION_STORE, '}'}, { ACTION_STORE, '~'}, {ACTION_IGNORE,   0}};  //     '|',     '}',     '~', '<DEL>'

const int alphamericReaderActions[128][2] =
    {{ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<NUL>', '<SOH>', '<STX>', '<ETX>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<EOT>', '<ENQ>', '<ACK>', '<BEL>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   //  '<BS>',  '<HT>',  '<LF>',  '<VT>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   //  '<FF>',  '<CR>',  '<SO>',  '<SI>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<DLE>', '<DC1>', '<DC2>', '<DC3>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<DC4>', '<NAK>', '<SYN>', '<ETB>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   // '<CAN>',  '<EM>', '<SUB>', '<ESC>'
     {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0}, {ACTION_IGNORE,   0},   //  '<FS>',  '<GS>',  '<RS>',  '<US>'
     { ACTION_STORE, ' '}, { ACTION_STORE, '!'}, { ACTION_STORE, '"'}, { ACTION_ERROR,   0},   //     ' ',     '!',     '"',     '#'
     { ACTION_STORE, '$'}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0},   //     '$',     '%',     '&',     '''
     { ACTION_STORE, '('}, { ACTION_STORE, ')'}, { ACTION_STORE, '*'}, { ACTION_STORE, '+'},   //     '(',     ')',     '*',     '+'
     { ACTION_STORE, ','}, { ACTION_STORE, '-'}, { ACTION_STORE, '.'}, { ACTION_STORE, '/'},   //     ',',     '-',     '.',     '/'
     { ACTION_STORE, '0'}, { ACTION_STORE, '1'}, { ACTION_STORE, '2'}, { ACTION_STORE, '3'},   //     '0',     '1',     '2',     '3'
     { ACTION_STORE, '4'}, { ACTION_STORE, '5'}, { ACTION_STORE, '6'}, { ACTION_STORE, '7'},   //     '4',     '5',     '6',     '7'
     { ACTION_STORE, '8'}, { ACTION_STORE, '9'}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0},   //     '8',     '9',     ':',     ';'
     { ACTION_ERROR,   0}, { ACTION_STORE, '='}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0},   //     '<',     '=',     '>',     '?'
     { ACTION_STORE, '@'}, { ACTION_STORE, 'A'}, { ACTION_STORE, 'B'}, { ACTION_STORE, 'C'},   //     '@',     'A',     'B',     'C'
     { ACTION_STORE, 'D'}, { ACTION_STORE, 'E'}, { ACTION_STORE, 'F'}, { ACTION_STORE, 'G'},   //     'D',     'E',     'F',     'G'
     { ACTION_STORE, 'H'}, { ACTION_STORE, 'I'}, { ACTION_STORE, 'J'}, { ACTION_STORE, 'K'},   //     'H',     'I',     'J',     'K'
     { ACTION_STORE, 'L'}, { ACTION_STORE, 'M'}, { ACTION_STORE, 'N'}, { ACTION_STORE, 'O'},   //     'L',     'M',     'N',     'O'
     { ACTION_STORE, 'P'}, { ACTION_STORE, 'Q'}, { ACTION_STORE, 'R'}, { ACTION_STORE, 'S'},   //     'P',     'Q',     'R',     'S'
     { ACTION_STORE, 'T'}, { ACTION_STORE, 'U'}, { ACTION_STORE, 'V'}, { ACTION_STORE, 'W'},   //     'T',     'U',     'V',     'W'
     { ACTION_STORE, 'X'}, { ACTION_STORE, 'Y'}, { ACTION_STORE, 'Z'}, { ACTION_ERROR,   0},   //     'X',     'Y',     'Z',     '['
     { ACTION_ERROR,   0}, { ACTION_STORE, ']'}, { ACTION_ERROR,   0}, { ACTION_ERROR,   0},   //     '\',     ']',     '^',     '_'
     { ACTION_ERROR,   0}, { ACTION_STORE, 'A'}, { ACTION_STORE, 'B'}, { ACTION_STORE, 'C'},   //     '`',     'a',     'b',     'c'
     { ACTION_STORE, 'D'}, { ACTION_STORE, 'E'}, { ACTION_STORE, 'F'}, { ACTION_STORE, 'G'},   //     'd',     'e',     'f',     'g'
     { ACTION_STORE, 'H'}, { ACTION_STORE, 'I'}, { ACTION_STORE, 'J'}, { ACTION_STORE, 'K'},   //     'h',     'i',     'j',     'k'
     { ACTION_STORE, 'L'}, { ACTION_STORE, 'M'}, { ACTION_STORE, 'N'}, { ACTION_STORE, 'O'},   //     'l',     'm',     'n',     'o'
     { ACTION_STORE, 'P'}, { ACTION_STORE, 'Q'}, { ACTION_STORE, 'R'}, { ACTION_STORE, 'S'},   //     'p',     'q',     'r',     's'
     { ACTION_STORE, 'T'}, { ACTION_STORE, 'U'}, { ACTION_STORE, 'V'}, { ACTION_STORE, 'W'},   //     't',     'u',     'v',     'w'
     { ACTION_STORE, 'X'}, { ACTION_STORE, 'Y'}, { ACTION_STORE, 'Z'}, { ACTION_ERROR,   0},   //     'x',     'y',     'z',     '{'
     { ACTION_STORE, '|'}, { ACTION_STORE, '}'}, { ACTION_STORE, '~'}, {ACTION_IGNORE,   0}};  //     '|',     '}',     '~', '<DEL>'

const int numericPunchMap[128] = 
    {'0', '0', '0', '0', '0', '0', '0', '0',   // '<NUL>', '<SOH>', '<STX>', '<ETX>', '<EOT>', '<ENQ>', '<ACK>', '<BEL>'
     '0', '0', '0', '0', '0', '0', '0', '0',   //  '<BS>',  '<HT>',  '<LF>',  '<VT>',  '<FF>',  '<CR>',  '<SO>',  '<SI>'
     '0', '0', '0', '0', '0', '0', '0', '0',   // '<DLE>', '<DC1>', '<DC2>', '<DC3>', '<DC4>', '<NAK>', '<SYN>', '<ETB>'
     '0', '0', '0', '0', '0', '0', '0', '0',   // '<CAN>',  '<EM>', '<SUB>', '<ESC>',  '<FS>',  '<GS>',  '<RS>',  '<US>'
     ' ', '!', '"', '?', '$', '?', '?', '?',   //     ' ',     '!',     '"',     '#',     '$',     '%',     '&',     '''
     '?', '?', '-', '?', '?', '-', '?', '?',   //     '(',     ')',     '*',     '+',     ',',     '-',     '.',     '/'
     '0', '1', '2', '3', '4', '5', '6', '7',   //     '0',     '1',     '2',     '3',     '4',     '5',     '6',     '7'
     '8', '9', '?', '?', '?', '=', '?', '?',   //     '8',     '9',     ':',     ';',     '<',     '=',     '>',     '?'
     ' ', '?', '?', '?', '?', '?', '?', '?',   //     '@',     'A',     'B',     'C',     'D',     'E',     'F',     'G'
     '?', '?', 'J', 'K', 'L', 'M', 'N', 'O',   //     'H',     'I',     'J',     'K',     'L',     'M',     'N',     'O'
     'P', 'Q', 'R', '?', '?', '?', '?', '?',   //     'P',     'Q',     'R',     'S',     'T',     'U',     'V',     'W'
     '?', '?', '?', '?', '?', ']', '?', '?',   //     'X',     'Y',     'Z',     '[',     '\',     ']',     '^',     '_'
     '?', '?', '?', '?', '?', '?', '?', '?',   //     '`',     'a',     'b',     'c',     'd',     'e',     'f',     'g'
     '?', ']', 'J', 'K', 'L', 'M', 'N', 'O',   //     'h',     'i',     'j',     'k',     'l',     'm',     'n',     'o'
     'P', 'Q', 'R', '?', '?', '?', '?', '?',   //     'p',     'q',     'r',     's',     't',     'u',     'v',     'w'
     '?', '?', '?', '?', '|', '}', '-', '?'};  //     'x',     'y',     'z',     '{',     '|',     '}',     '~', '<DEL>'


const int alphamericPunchMap[128] =
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // '<NUL>', '<SOH>', '<STX>', '<ETX>', '<EOT>', '<ENQ>', '<ACK>', '<BEL>'
     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   //  '<BS>',  '<HT>',  '<LF>',  '<VT>',  '<FF>',  '<CR>',  '<SO>',  '<SI>'
     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // '<DLE>', '<DC1>', '<DC2>', '<DC3>', '<DC4>', '<NAK>', '<SYN>', '<ETB>'
     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',   // '<CAN>',  '<EM>', '<SUB>', '<ESC>',  '<FS>',  '<GS>',  '<RS>',  '<US>h
     ' ', '|', '}', '?', '$', '?', '?', '?',   //     ' ',     '!',     '"',     '#',     '$',     '%',     '&',     '''
     '(', ')', '*', '+', ',', '-', '.', '/',   //     '(',     ')',     '*',     '+',     ',',     '-',     '.',     '/'
     '0', '1', '2', '3', '4', '5', '6', '7',   //     '0',     '1',     '2',     '3',     '4',     '5',     '6',     '7'
     '8', '9', '?', '?', '?', '=', '?', '?',   //     '8',     '9',     ':',     ';',     '<',     '=',     '>',     '?'
     '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',   //     '@',     'A',     'B',     'C',     'D',     'E',     'F',     'G'
     'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',   //     'H',     'I',     'J',     'K',     'L',     'M',     'N',     'O'
     'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',   //     'P',     'Q',     'R',     'S',     'T',     'U',     'V',     'W'
     'X', 'Y', 'Z', '?', '?', ']', '?', '?',   //     'X',     'Y',     'Z',     '[',     '\',     ']',     '^',     '_'
     '?', '?', '?', '?', '?', '?', '?', '?',   //     '`',     'a',     'b',     'c',     'd',     'e',     'f',     'g'
     '?', ']', '?', '?', '?', '?', '?', '?',   //     'h',     'i',     'j',     'k',     'l',     'm',     'n',     'o'
     '?', '?', '?', '?', '?', '?', '?', '?',   //     'p',     'q',     'r',     's',     't',     'u',     'v',     'w'
     '?', '?', '?', '?', '|', '}', '@', '?'};  //     'x',     'y',     'z',     '{',     '|',     '}',     '~', '<DEL>'


// =================================  Communication I/O Data  =================================

const int protocolCDActions[128] =
    {          PACTION_IGNORE,       PACTION_IGNORE,          PACTION_IGNORE,         PACTION_IGNORE,   // '<NUL>', '<SOH>', '<STX>', '<ETX>'
               PACTION_IGNORE,       PACTION_IGNORE,          PACTION_IGNORE,         PACTION_IGNORE,   // '<EOT>', '<ENQ>', '<ACK>', '<BEL>'
               PACTION_IGNORE,       PACTION_IGNORE,          PACTION_IGNORE,         PACTION_IGNORE,   //  '<BS>',  '<HT>',  '<LF>',  '<VT>'
               PACTION_IGNORE,       PACTION_IGNORE,          PACTION_IGNORE,         PACTION_IGNORE,   //  '<FF>',  '<CR>',  '<SO>',  '<SI>'
               PACTION_IGNORE,       PACTION_IGNORE,          PACTION_IGNORE,         PACTION_IGNORE,   // '<DLE>', '<DC1>', '<DC2>', '<DC3>'
               PACTION_IGNORE,       PACTION_IGNORE,          PACTION_IGNORE,         PACTION_IGNORE,   // '<DC4>', '<NAK>', '<SYN>', '<ETB>'
               PACTION_IGNORE,       PACTION_IGNORE,          PACTION_IGNORE,         PACTION_IGNORE,   // '<CAN>',  '<EM>', '<SUB>', '<ESC>'
               PACTION_IGNORE,       PACTION_IGNORE,          PACTION_IGNORE,         PACTION_IGNORE,   //  '<FS>',  '<GS>',  '<RS>',  '<US>'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE, PACTION_REQUEST_STATUS,   //     ' ',     '!',     '"',     '#'
                PACTION_STORE,        PACTION_ERROR,           PACTION_ERROR,          PACTION_ERROR,   //     '$',     '%',     '&',     '''
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     '(',     ')',     '*',     '+'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     ',',     '-',     '.',     '/'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     '0',     '1',     '2',     '3'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     '4',     '5',     '6',     '7'
                PACTION_STORE,        PACTION_STORE,           PACTION_ERROR,          PACTION_ERROR,   //     '8',     '9',     ':',     ';'
             PACTION_POWER_ON,        PACTION_STORE,       PACTION_POWER_OFF,          PACTION_STORE,   //     '<',     '=',     '>',     '?'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     '@',     'A',     'B',     'C'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     'D',     'E',     'F',     'G'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     'H',     'I',     'J',     'K'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     'L',     'M',     'N',     'O'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     'P',     'Q',     'R',     'S'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     'T',     'U',     'V',     'W'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_ERROR,   //     'X',     'Y',     'Z',     '['
                PACTION_ERROR,        PACTION_STORE,           PACTION_ERROR,       PACTION_SHUTDOWN,   //     '\',     ']',     '^',     '_'
                PACTION_ERROR,        PACTION_ERROR,           PACTION_ERROR,          PACTION_ERROR,   //     '`',     'a',     'b',     'c'
                PACTION_ERROR,        PACTION_ERROR,           PACTION_ERROR,          PACTION_ERROR,   //     'd',     'e',     'f',     'g'
                PACTION_ERROR,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     'h',     'i',     'j',     'k'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_STORE,   //     'l',     'm',     'n',     'o'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,         PACTION_MANUAL,   //     'p',     'q',     'r',     's'
           PACTION_NOT_MANUAL, PACTION_READ_NUMERIC, PACTION_READ_ALPHAMERIC,  PACTION_WRITE_NUMERIC,   //     't',     'u',     'v',     'w'
     PACTION_WRITE_ALPHAMERIC, PACTION_DUMP_NUMERIC,           PACTION_RESET,          PACTION_ERROR,   //     'x',     'y',     'z',     '{'
                PACTION_STORE,        PACTION_STORE,           PACTION_STORE,          PACTION_ERROR};  //     '|',     '}',     '~', '<DEL>'
