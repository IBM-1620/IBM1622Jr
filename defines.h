//======================================================================================================================
//
//  defines.h - global defines
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

#ifndef DEFINES_H_
#define DEFINES_H_

#define _GNU_SOURCE 1
#define __USE_MISC 1


// =================================  Basic Defines  =================================

// Software version
#define VERSION_MAJOR 0
#define VERSION_MINOR 4

// Logical values
#define FALSE 0
#define TRUE  1

// USB port connections
#define USB_UL 1
#define USB_UR 2
#define USB_LL 3
#define USB_LR 4

// MAX and MIN functions
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


// =================================  Display Defines  =================================

// Display size
#define DISPLAY_WIDTH  1024
#define DISPLAY_HEIGHT  600

// Display timings
#define DISPLAY_REFRESH_MS 40

// Display types
#define DISPLAY_UNKNOWN       0
#define DISPLAY_SPLASH_SCREEN 1
#define DISPLAY_READER_PUNCH  2
#define DISPLAY_FILE_SELECT   3

// Audio file values
#define BUTTONDOWN_FILENAME "Audio/ButtonDown.wav"
#define BUTTONUP_FILENAME   "Audio/ButtonUp.wav"
#define READ_FILENAME       "Audio/Read.wav"
#define PUNCH_FILENAME      "Audio/Punch.wav"

// Font file values
#define FONT_FILENAME "Fonts/IBM1622.ttf"

// Graphic file values
#define SPLASH_FILENAME                 "Graphics/IBM1622.png"
#define DIRECTORY_FILENAME              "Graphics/Directory.png"
#define FILE_FILENAME                   "Graphics/File.png"
#define BACKUP_OFF_FILENAME             "Graphics/BackupOff.png"
#define BACKUP_ON_FILENAME              "Graphics/BackupOn.png"
#define BACKUP_SELECTED_FILENAME        "Graphics/BackupSelected.png"
#define UPUPARROW_OFF_FILENAME          "Graphics/UpUpArrowOff.png"
#define UPUPARROW_ON_FILENAME           "Graphics/UpUpArrowOn.png"
#define UPUPARROW_SELECTED_FILENAME     "Graphics/UpUpArrowSelected.png"
#define UPARROW_OFF_FILENAME            "Graphics/UpArrowOff.png"
#define UPARROW_ON_FILENAME             "Graphics/UpArrowOn.png"
#define UPARROW_SELECTED_FILENAME       "Graphics/UpArrowSelected.png"
#define DOWNARROW_OFF_FILENAME          "Graphics/DownArrowOff.png"
#define DOWNARROW_ON_FILENAME           "Graphics/DownArrowOn.png"
#define DOWNARROW_SELECTED_FILENAME     "Graphics/DownArrowSelected.png"
#define DOWNDOWNARROW_OFF_FILENAME      "Graphics/DownDownArrowOff.png"
#define DOWNDOWNARROW_ON_FILENAME       "Graphics/DownDownArrowOn.png"
#define DOWNDOWNARROW_SELECTED_FILENAME "Graphics/DownDownArrowSelected.png"
#define ITEM_SELECTED_FILENAME          "Graphics/ItemSelected.png"

// Button states
#define BUTTON_NONE_PRESSED  0
#define BUTTON_LOAD_PRESSED  1
#define BUTTON_RESET_PRESSED 2

// Card types
#define LEFT_CARD   0
#define MIDDLE_CARD 1
#define RIGHT_CARD  2

// Card stack locations
#define PUNCH_HOPPER        0
#define PUNCH_STACKER       1
#define PUNCH_ERROR_STACKER 2
#define MIDDLE_STACKER      3
#define READ_ERROR_STACKER  4
#define READ_STACKER        5
#define READ_HOPPER         6

// Hopper and stacker sizes
#define HOPPER_SIZE  1200
#define STACKER_SIZE 1000

// Display font sizes
#define TITLE_FONT_SIZE          30
#define LIGHT_FONT_SIZE          24
#define LOAD_FONT_SIZE           28
#define RESET_FONT_SIZE          20
#define DIRECTORY_FILE_FONT_SIZE 42
#define EXTENSION_FONT_SIZE      18

// Display values
#define THIN_LINE_WIDTH    1
#define THICK_LINE_WIDTH   5
#define TITLE_LINE_LENGTH 42
#define ITEM_LINE_LENGTH  36

// Card animation masks
#define ANIMATE_EMERGE_MASK           0x002
#define ANIMATE_NORMAL_MASK           0x03F
#define ANIMATE_NORMAL_LAST_MASK      0x020
#define ANIMATE_NORMAL_REMAINDER_MASK 0x03C
#define ANIMATE_ERROR_MASK            0x07F
#define ANIMATE_ERROR_LAST_MASK       0x040
#define ANIMATE_ERROR_REMAINDER_MASK  0x07C

// Display colors
#define DISPLAY_BLACK_R 0x00
#define DISPLAY_BLACK_G 0x00
#define DISPLAY_BLACK_B 0x00
#define DISPLAY_BLACK_RGBA_OPAQUE      0xFF000000
#define DISPLAY_BLACK_RGBA_TRANSPARENT 0x00000000

#define DISPLAY_WHITE_R 0xFF
#define DISPLAY_WHITE_G 0xFF
#define DISPLAY_WHITE_B 0xFF
#define DISPLAY_WHITE_RGBA_OPAQUE      0xFFFFFFFF
#define DISPLAY_WHITE_RGBA_TRANSPARENT 0x00FFFFFF

#define DISPLAY_BACKGROUND_R 0xE0
#define DISPLAY_BACKGROUND_G 0xE0
#define DISPLAY_BACKGROUND_B 0xE0
#define DISPLAY_BACKGROUND_RGBA_OPAQUE      0xFFE0E0E0
#define DISPLAY_BACKGROUND_RGBA_TRANSPARENT 0x00E0E0E0

#define DISPLAY_HOPPER_R 0xA0
#define DISPLAY_HOPPER_G 0xA0
#define DISPLAY_HOPPER_B 0xA0
#define DISPLAY_HOPPER_RGBA_OPAQUE      0xFFA0A0A0
#define DISPLAY_HOPPER_RGBA_TRANSPARENT 0x00A0A0A0

#define DISPLAY_STACKER_R 0xA0
#define DISPLAY_STACKER_G 0xA0
#define DISPLAY_STACKER_B 0xA0
#define DISPLAY_STACKER_RGBA_OPAQUE      0xFFA0A0A0
#define DISPLAY_STACKER_RGBA_TRANSPARENT 0x00A0A0A0

#define DISPLAY_CARD_R 0xB5
#define DISPLAY_CARD_G 0x65
#define DISPLAY_CARD_B 0x1D
#define DISPLAY_CARD_RGBA_OPAQUE      0xFF1D65B5
#define DISPLAY_CARD_RGBA_TRANSPARENT 0x001D65B5

#define DISPLAY_CARD_EDGE_R 0xA5
#define DISPLAY_CARD_EDGE_G 0x55
#define DISPLAY_CARD_EDGE_B 0x0D
#define DISPLAY_CARD_EDGE_RGBA_OPAQUE      0xFF0D55A5
#define DISPLAY_CARD_EDGE_RGBA_TRANSPARENT 0x000D55A5

#define DISPLAY_CARD_STACK_R 0x95
#define DISPLAY_CARD_STACK_G 0x45
#define DISPLAY_CARD_STACK_B 0x00
#define DISPLAY_CARD_STACK_RGBA_OPAQUE      0xFF004595
#define DISPLAY_CARD_STACK_RGBA_TRANSPARENT 0x00004595

#define DISPLAY_PANEL_R 0x43
#define DISPLAY_PANEL_G 0x43
#define DISPLAY_PANEL_B 0x3D
#define DISPLAY_PANEL_RGBA_OPAQUE      0xFF3D4343
#define DISPLAY_PANEL_RGBA_TRANSPARENT 0x003D4343

#define DISPLAY_TITLE_R 0xCF
#define DISPLAY_TITLE_G 0xCF
#define DISPLAY_TITLE_B 0xCF
#define DISPLAY_TITLE_RGBA_OPAQUE      0xFFCFCFCF
#define DISPLAY_TITLE_RGBA_TRANSPARENT 0x00CFCFCF

#define DISPLAY_LIGHT_OFF_R 0x7F
#define DISPLAY_LIGHT_OFF_G 0x7F
#define DISPLAY_LIGHT_OFF_B 0x7F
#define DISPLAY_LIGHT_OFF_RGBA_OPAQUE      0xFF7F7F7F
#define DISPLAY_LIGHT_OFF_RGBA_TRANSPARENT 0x007F7F7F

#define DISPLAY_LIGHT_ON_R 0x00
#define DISPLAY_LIGHT_ON_G 0xFF
#define DISPLAY_LIGHT_ON_B 0x00
#define DISPLAY_LIGHT_ON_RGBA_OPAQUE      0xFF00FF00
#define DISPLAY_LIGHT_ON_RGBA_TRANSPARENT 0x0000FF00

#define DISPLAY_LIGHT_ERROR_R 0xFF
#define DISPLAY_LIGHT_ERROR_G 0x00
#define DISPLAY_LIGHT_ERROR_B 0x00
#define DISPLAY_LIGHT_ERROR_RGBA_OPAQUE      0xFF0000FF
#define DISPLAY_LIGHT_ERROR_RGBA_TRANSPARENT 0x000000FF

#define DISPLAY_LOAD_TEXT_R 0x00
#define DISPLAY_LOAD_TEXT_G 0x00
#define DISPLAY_LOAD_TEXT_B 0x00
#define DISPLAY_LOAD_TEXT_RGBA_OPAQUE      0xFF000000
#define DISPLAY_LOAD_TEXT_RGBA_TRANSPARENT 0x00000000

#define DISPLAY_LOAD_BACKGROUND_R 0x62
#define DISPLAY_LOAD_BACKGROUND_G 0x7D
#define DISPLAY_LOAD_BACKGROUND_B 0xCF
#define DISPLAY_LOAD_BACKGROUND_RGBA_OPAQUE      0xFFCF7D62
#define DISPLAY_LOAD_BACKGROUND_RGBA_TRANSPARENT 0x00CF7D62

#define DISPLAY_RESET_TEXT_R 0x00
#define DISPLAY_RESET_TEXT_G 0x00
#define DISPLAY_RESET_TEXT_B 0x00
#define DISPLAY_RESET_TEXT_RGBA_OPAQUE      0xFF000000
#define DISPLAY_RESET_TEXT_RGBA_TRANSPARENT 0x00000000

#define DISPLAY_RESET_BACKGROUND_R 0xFF
#define DISPLAY_RESET_BACKGROUND_G 0xFF
#define DISPLAY_RESET_BACKGROUND_B 0x66
#define DISPLAY_RESET_BACKGROUND_RGBA_OPAQUE      0xFF66FFFF
#define DISPLAY_RESET_BACKGROUND_RGBA_TRANSPARENT 0x0066FFFF

// Touch panel locations
#define TOUCH_LOAD_X1 0.453
#define TOUCH_LOAD_Y1 0.115
#define TOUCH_LOAD_X2 0.546
#define TOUCH_LOAD_Y2 0.218

#define TOUCH_RESET_X1 0.458
#define TOUCH_RESET_Y1 0.258
#define TOUCH_RESET_X2 0.540
#define TOUCH_RESET_Y2 0.347

#define TOUCH_BACKUP_X1 0.948
#define TOUCH_BACKUP_Y1 0.000
#define TOUCH_BACKUP_X2 0.999
#define TOUCH_BACKUP_Y2 0.083

#define TOUCH_UPUPARROW_X1 0.948
#define TOUCH_UPUPARROW_Y1 0.093
#define TOUCH_UPUPARROW_X2 0.999
#define TOUCH_UPUPARROW_Y2 0.213

#define TOUCH_UPARROW_X1 0.948
#define TOUCH_UPARROW_Y1 0.223
#define TOUCH_UPARROW_X2 0.999
#define TOUCH_UPARROW_Y2 0.310

#define TOUCH_DOWNARROW_X1 0.948
#define TOUCH_DOWNARROW_Y1 0.782
#define TOUCH_DOWNARROW_X2 0.999
#define TOUCH_DOWNARROW_Y2 0.868

#define TOUCH_DOWNDOWNARROW_X1 0.948
#define TOUCH_DOWNDOWNARROW_Y1 0.878
#define TOUCH_DOWNDOWNARROW_X2 0.999
#define TOUCH_DOWNDOWNARROW_Y2 0.999

#define TOUCH_SELECTION_X1 0.000
#define TOUCH_SELECTION_Y1 0.093
#define TOUCH_SELECTION_X2 0.942
#define TOUCH_SELECTION_Y2 0.999

// File select values
#define SELECT_MAX_ENTRIES 100

#define SELECTED_NONE           0
#define SELECTED_BACKUP         1
#define SELECTED_UPUPARROW      2
#define SELECTED_UPARROW        3
#define SELECTED_DOWNARROW      4
#define SELECTED_DOWNDOWNARROW  5
#define SELECTED_ITEMS          6

// File select types
#define DIRECTORY_TYPE 0
#define FILE_TYPE      1


// =================================  Card I/O Defines  =================================

// Reader / punch status
#define STATUS_UNKNOWN   0
#define STATUS_READY     1
#define STATUS_NOT_READY 2
#define STATUS_CHECK     3

// Read / write types
#define TYPE_NONE       0
#define TYPE_NUMERIC    1
#define TYPE_ALPHAMERIC 2

// Actions
#define ACTION_IGNORE 0
#define ACTION_STORE  1
#define ACTION_ERROR  2


// =================================  Communication I/O Defines  =================================

// IBM 1620 states
#define STATE_UNKNOWN    0
#define STATE_POWER_OFF  1
#define STATE_POWER_ON   2
#define STATE_MANUAL     3
#define STATE_NOT_MANUAL 4

// Protocol actions
#define PACTION_IGNORE            0
#define PACTION_STORE             1
#define PACTION_MANUAL            2
#define PACTION_NOT_MANUAL        3
#define PACTION_READ_NUMERIC      4
#define PACTION_READ_ALPHAMERIC   5
#define PACTION_WRITE_NUMERIC     6
#define PACTION_WRITE_ALPHAMERIC  7
#define PACTION_DUMP_NUMERIC      8
#define PACTION_RESET             9
#define PACTION_POWER_OFF        10
#define PACTION_POWER_ON         11
#define PACTION_REQUEST_STATUS   12
#define PACTION_SHUTDOWN         13
#define PACTION_ERROR            14

// Command characters
#define COMMAND_LOAD             'f'
#define COMMAND_READ_NUMERIC     'u'
#define COMMAND_READ_ALPHAMERIC  'v'
#define COMMAND_WRITE_NUMERIC    'w'
#define COMMAND_WRITE_ALPHAMERIC 'x'
#define COMMAND_DUMP_NUMERIC     'y'
#define COMMAND_RESET            'z'
#define COMMAND_POWER_OFF        '>'
#define COMMAND_POWER_ON         '<'
#define COMMAND_SHUTDOWN         '_'

// Status characters
#define STATUS_READER_READY     'a'
#define STATUS_READER_NOT_READY 'b'
#define STATUS_PUNCH_READY      'c'
#define STATUS_PUNCH_NOT_READY  'd'
#define STATUS_LAST_CARD        'e'
#define STATUS_READER_BUSY      '\\'
#define STATUS_READER_NOT_BUSY  '['
#define STATUS_PUNCH_BUSY       '^'
#define STATUS_PUNCH_NOT_BUSY   '{'
#define STATUS_READER_CHECK     ':'
#define STATUS_PUNCH_CHECK      ';'
#define STATUS_REQUEST_STATUS   '#'

// Data characters
#define DATA_SPACE              ' '
#define DATA_PERIOD             '.'
#define DATA_RIGHT_PAREN        ')'
#define DATA_PLUS               '+'
#define DATA_DOLLAR             '$'
#define DATA_ASTERISK           '*'
#define DATA_HYPHEN             '-'
#define DATA_SLASH              '/'
#define DATA_COMMA              ','
#define DATA_LEFT_PAREN         '('
#define DATA_EQUAL              '='
#define DATA_AT_SIGN            '@'

#define DATA_A                  'A'
#define DATA_B                  'B'
#define DATA_C                  'C'
#define DATA_D                  'D'
#define DATA_E                  'E'
#define DATA_F                  'F'
#define DATA_G                  'G'
#define DATA_H                  'H'
#define DATA_I                  'I'
#define DATA_J                  'J'
#define DATA_K                  'K'
#define DATA_L                  'L'
#define DATA_M                  'M'
#define DATA_N                  'N'
#define DATA_O                  'O'
#define DATA_P                  'P'
#define DATA_Q                  'Q'
#define DATA_R                  'R'
#define DATA_S                  'S'
#define DATA_T                  'T'
#define DATA_U                  'U'
#define DATA_V                  'V'
#define DATA_W                  'W'
#define DATA_X                  'X'
#define DATA_Y                  'Y'
#define DATA_Z                  'Z'

#define DATA_0                  '0'
#define DATA_1                  '1'
#define DATA_2                  '2'
#define DATA_3                  '3'
#define DATA_4                  '4'
#define DATA_5                  '5'
#define DATA_6                  '6'
#define DATA_7                  '7'
#define DATA_8                  '8'
#define DATA_9                  '9'

#define DATA_FLAG_0             'i'
#define DATA_FLAG_1             'j'
#define DATA_FLAG_2             'k'
#define DATA_FLAG_3             'l'
#define DATA_FLAG_4             'm'
#define DATA_FLAG_5             'n'
#define DATA_FLAG_6             'o'
#define DATA_FLAG_7             'p'
#define DATA_FLAG_8             'q'
#define DATA_FLAG_9             'r'

#define DATA_NUMERIC_BLANK      '@'
#define DATA_FLAG_NUMERIC_BLANK '~'
#define DATA_RECORD_MARK        '|'
#define DATA_FLAG_RECORD_MARK   '!'
#define DATA_GROUP_MARK         '}'
#define DATA_FLAG_GROUP_MARK    '"'
#define DATA_INVALID_CHARACTER  '?'

#endif /* DEFINES_H_ */
