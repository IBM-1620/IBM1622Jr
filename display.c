//======================================================================================================================
//
//  display.c - display support
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

#include <pthread.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

// Function declarations
void BuildTextures(void);
void OpenFileSelectFonts(void);
void DestroyTextures(void);
void CloseFileSelectFonts(void);
void ShowSplashScreen(void);
void ShowReaderPunch(void);
Uint32 DisplayCallback(Uint32 interval, void* param);
void DrawCard(int x, int y);
int DrawCardStack(int stack, int cnt);
void ShowFileSelect(void);

// Local Data
SDL_Renderer *renderer;
SDL_Window   *window;
SDL_TimerID   timer;


// DisplayOpen
void DisplayOpen(void) {

    (void)SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_RENDERER_ACCELERATED);
    SDL_CreateWindowAndRenderer(DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, &window, &renderer);

    SDL_ShowCursor(SDL_DISABLE);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_BACKGROUND_R, DISPLAY_BACKGROUND_G, DISPLAY_BACKGROUND_B,
                           SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    BuildTextures();
    OpenFileSelectFonts();

    timer = SDL_AddTimer(DISPLAY_REFRESH_MS, DisplayCallback, NULL);
}

// DisplayClose
void DisplayClose(void) {

    SDL_RemoveTimer(timer);

    CloseFileSelectFonts();
    DestroyTextures();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

// BuildTextures
void BuildTextures(void) {
    TTF_Font *font;
    SDL_Surface *image;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect     rect;

    image = IMG_Load(SPLASH_FILENAME);
    splashTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    (void)TTF_Init();

    font = TTF_OpenFont(FONT_FILENAME, TITLE_FONT_SIZE);
    (void)TTF_SetFontHinting(font, TTF_HINTING_MONO);

    surface = TTF_RenderText_Shaded(font, "Reader", titleColor, panelColor);
    readerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Shaded(font, "Punch", titleColor, panelColor);
    punchTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);

    font = TTF_OpenFont(FONT_FILENAME, LIGHT_FONT_SIZE);
    (void)TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    (void)TTF_SetFontHinting(font, TTF_HINTING_MONO);

    surface = TTF_RenderText_Shaded(font, "Ready", lightOffColor, panelColor);
    readyOffTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = TTF_RenderText_Shaded(font, "Ready", lightOnColor, panelColor);
    readyOnTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Shaded(font, "Check", lightOffColor, panelColor);
    checkOffTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = TTF_RenderText_Shaded(font, "Check", lightErrorColor, panelColor);
    checkOnTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);

    font = TTF_OpenFont(FONT_FILENAME, LOAD_FONT_SIZE);
    (void)TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    (void)TTF_SetFontHinting(font, TTF_HINTING_MONO);

    loadOffTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 96, 63);
    (void)SDL_SetRenderTarget(renderer, loadOffTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_PANEL_R, DISPLAY_PANEL_G, DISPLAY_PANEL_B, SDL_ALPHA_OPAQUE);
    (void)SDL_RenderClear(renderer);
    (void)filledPolygonColor(renderer, loadOff1X, loadOff1Y, 4, DISPLAY_HOPPER_RGBA_OPAQUE);
    (void)filledPolygonColor(renderer, loadOff2X, loadOff2Y, 4, DISPLAY_LOAD_BACKGROUND_RGBA_OPAQUE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < (sizeof(loadOffLines) / (4 * sizeof(Sint16))); ++i) {
        (void)SDL_RenderDrawLine(renderer, loadOffLines[i][0], loadOffLines[i][1], loadOffLines[i][2],
                                 loadOffLines[i][3]);
    }
    surface = TTF_RenderText_Shaded(font, "Load", loadTextColor, loadBackgroundColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    (void)SDL_RenderCopy(renderer, texture, NULL, &loadOffRect);
    SDL_DestroyTexture(texture);
    (void)SDL_SetRenderTarget(renderer, NULL);

    loadOnTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 96, 63);
    (void)SDL_SetRenderTarget(renderer, loadOnTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_PANEL_R, DISPLAY_PANEL_G, DISPLAY_PANEL_B, SDL_ALPHA_OPAQUE);
    (void)SDL_RenderClear(renderer);
    (void)filledPolygonColor(renderer, loadOnX, loadOnY, 4, DISPLAY_LOAD_BACKGROUND_RGBA_OPAQUE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < (sizeof(loadOnLines) / (4 * sizeof(Sint16))); ++i) {
        (void)SDL_RenderDrawLine(renderer, loadOnLines[i][0], loadOnLines[i][1], loadOnLines[i][2], loadOnLines[i][3]);
    }
    surface = TTF_RenderText_Shaded(font, "Load", loadTextColor, loadBackgroundColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    (void)SDL_RenderCopy(renderer, texture, NULL, &loadOnRect);
    SDL_DestroyTexture(texture);
    (void)SDL_SetRenderTarget(renderer, NULL);

    TTF_CloseFont(font);

    font = TTF_OpenFont(FONT_FILENAME, RESET_FONT_SIZE);
    (void)TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    (void)TTF_SetFontHinting(font, TTF_HINTING_MONO);

    resetOffTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 85, 54);
    (void)SDL_SetRenderTarget(renderer, resetOffTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_PANEL_R, DISPLAY_PANEL_G, DISPLAY_PANEL_B, SDL_ALPHA_OPAQUE);
    (void)SDL_RenderClear(renderer);
    (void)filledPolygonColor(renderer, resetOff1X, resetOff1Y, 4, DISPLAY_HOPPER_RGBA_OPAQUE);
    (void)filledPolygonColor(renderer, resetOff2X, resetOff2Y, 4, DISPLAY_RESET_BACKGROUND_RGBA_OPAQUE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < (sizeof(resetOffLines) / (4 * sizeof(Sint16))); ++i) {
        (void)SDL_RenderDrawLine(renderer, resetOffLines[i][0], resetOffLines[i][1], resetOffLines[i][2],
                                 resetOffLines[i][3]);
    }
    surface = TTF_RenderText_Shaded(font, "Reset", resetTextColor, resetBackgroundColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    (void)SDL_RenderCopy(renderer, texture, NULL, &resetOffRect);
    SDL_DestroyTexture(texture);
    (void)SDL_SetRenderTarget(renderer, NULL);

    resetOnTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 85, 54);
    (void)SDL_SetRenderTarget(renderer, resetOnTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_PANEL_R, DISPLAY_PANEL_G, DISPLAY_PANEL_B, SDL_ALPHA_OPAQUE);
    (void)SDL_RenderClear(renderer);
    (void)filledPolygonColor(renderer, resetOnX, resetOnY, 4, DISPLAY_RESET_BACKGROUND_RGBA_OPAQUE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < (sizeof(resetOnLines) / (4 * sizeof(Sint16))); ++i) {
        (void)SDL_RenderDrawLine(renderer, resetOnLines[i][0], resetOnLines[i][1], resetOnLines[i][2],
                                 resetOnLines[i][3]);
    }
    surface = TTF_RenderText_Shaded(font, "Reset", resetTextColor, resetBackgroundColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    (void)SDL_RenderCopy(renderer, texture, NULL, &resetOnRect);
    SDL_DestroyTexture(texture);
    (void)SDL_SetRenderTarget(renderer, NULL);

    TTF_CloseFont(font);

    leftCardTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 138, 68);
    SDL_SetRenderTarget(renderer, leftCardTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(leftCardTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, leftCardX, leftCardY, 5, DISPLAY_CARD_RGBA_OPAQUE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_CARD_EDGE_R, DISPLAY_CARD_EDGE_G, DISPLAY_CARD_EDGE_B,
                                 SDL_ALPHA_OPAQUE);
    for (int i = 0; i < (sizeof(leftCardLines) / (4 * sizeof(Sint16))); ++i) {
        (void)SDL_RenderDrawLine(renderer, leftCardLines[i][0], leftCardLines[i][1], leftCardLines[i][2],
                                 leftCardLines[i][3]);
    }
    SDL_SetRenderTarget(renderer, NULL);

    leftCardCornerTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 35, 16);
    SDL_SetRenderTarget(renderer, leftCardCornerTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(leftCardCornerTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, leftCardCornerX, leftCardCornerY, 3, DISPLAY_HOPPER_RGBA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);

    leftCardStackTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 138, 320);
    SDL_SetRenderTarget(renderer, leftCardStackTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(leftCardStackTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (int i = 252; i >= 0; --i) {
        rect.x = 0;
        rect.y = i;
        rect.w = 138;
        rect.h = 68;
        SDL_RenderCopy(renderer, leftCardTexture, NULL, (const SDL_Rect *)&rect);
    }
    (void)thickLineColor(renderer, 82, 65, 82, 318, THICK_LINE_WIDTH, DISPLAY_CARD_STACK_RGBA_OPAQUE);
    (void)thickLineColor(renderer, 115, 50, 115, 303, THICK_LINE_WIDTH, DISPLAY_CARD_STACK_RGBA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);

    middleCardTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 116, 68);
    SDL_SetRenderTarget(renderer, middleCardTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(middleCardTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, middleCardX, middleCardY, 5, DISPLAY_CARD_RGBA_OPAQUE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_CARD_EDGE_R, DISPLAY_CARD_EDGE_G, DISPLAY_CARD_EDGE_B,
                                 SDL_ALPHA_OPAQUE);
    for (int i = 0; i < (sizeof(middleCardLines) / (4 * sizeof(Sint16))); ++i) {
        (void)SDL_RenderDrawLine(renderer, middleCardLines[i][0], middleCardLines[i][1], middleCardLines[i][2],
                                 middleCardLines[i][3]);
    }
    SDL_SetRenderTarget(renderer, NULL);

    middleCardCornerTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 35, 16);
    SDL_SetRenderTarget(renderer, middleCardCornerTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(middleCardCornerTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, middleCardCornerX, middleCardCornerY, 3, DISPLAY_HOPPER_RGBA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);

    middleCardStackTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 116, 320);
    SDL_SetRenderTarget(renderer, middleCardStackTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(middleCardStackTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (int i = 252; i >= 0; --i) {
        rect.x = 0;
        rect.y = i;
        rect.w = 116;
        rect.h = 68;
        SDL_RenderCopy(renderer, middleCardTexture, NULL, (const SDL_Rect *)&rect);
    }
    (void)thickLineColor(renderer, 82, 65, 82, 318, THICK_LINE_WIDTH, DISPLAY_CARD_STACK_RGBA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);

    rightCardTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 134, 68);
    SDL_SetRenderTarget(renderer, rightCardTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(rightCardTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, rightCardX, rightCardY, 5, DISPLAY_CARD_RGBA_OPAQUE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_CARD_EDGE_R, DISPLAY_CARD_EDGE_G, DISPLAY_CARD_EDGE_B,
                                 SDL_ALPHA_OPAQUE);
    for (int i = 0; i < (sizeof(rightCardLines) / (4 * sizeof(Sint16))); ++i) {
        (void)SDL_RenderDrawLine(renderer, rightCardLines[i][0], rightCardLines[i][1], rightCardLines[i][2],
                                 rightCardLines[i][3]);
    }
    SDL_SetRenderTarget(renderer, NULL);

    rightCardCornerTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 35, 16);
    SDL_SetRenderTarget(renderer, rightCardCornerTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(rightCardCornerTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, rightCardCornerX, rightCardCornerY, 3, DISPLAY_HOPPER_RGBA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);

    rightCardStackTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 134, 320);
    SDL_SetRenderTarget(renderer, rightCardStackTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(rightCardStackTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    for (int i = 252; i >= 0; --i) {
        rect.x = 0;
        rect.y = i;
        rect.w = 134;
        rect.h = 68;
        SDL_RenderCopy(renderer, rightCardTexture, NULL, (const SDL_Rect *)&rect);
    }
    (void)thickLineColor(renderer, 104, 65, 104, 318, THICK_LINE_WIDTH, DISPLAY_CARD_STACK_RGBA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);

    backgroundTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH,
                                          DISPLAY_HEIGHT);
    SDL_SetRenderTarget(renderer, backgroundTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_BACKGROUND_R, DISPLAY_BACKGROUND_G, DISPLAY_BACKGROUND_B,
                                 SDL_ALPHA_OPAQUE);
    (void)SDL_RenderClear(renderer);
    (void)filledPolygonColor(renderer, punchHopperX1, punchHopperY1, sizeof(punchHopperX1) / sizeof(Sint16),
                             DISPLAY_HOPPER_RGBA_OPAQUE);
    (void)filledPolygonColor(renderer, stackerX1, stackerY1, sizeof(stackerX1) / sizeof(Sint16),
                             DISPLAY_STACKER_RGBA_OPAQUE);
    (void)filledPolygonColor(renderer, readHopperX1, readHopperY1, sizeof(readHopperX1) / sizeof(Sint16),
                             DISPLAY_HOPPER_RGBA_OPAQUE);
    for (int i = 0; i < (sizeof(hopperStackerLines1) / (4 * sizeof(Sint16))); ++i) {
        (void)thickLineColor(renderer, hopperStackerLines1[i][0], hopperStackerLines1[i][1], hopperStackerLines1[i][2],
                             hopperStackerLines1[i][3], THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    }
    (void)filledPolygonColor(renderer, panel1X, panel1Y, 4, DISPLAY_HOPPER_RGBA_OPAQUE);
    (void)filledPolygonColor(renderer, panel2X, panel2Y, 4, DISPLAY_PANEL_RGBA_OPAQUE);
    for (int i = 0; i < (sizeof(panelLines) / (4 * sizeof(Sint16))); ++i) {
        (void)thickLineColor(renderer, panelLines[i][0], panelLines[i][1], panelLines[i][2], panelLines[i][3],
                             THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    }
    SDL_RenderCopy(renderer, punchTexture, NULL, &punchRect);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_TITLE_R, DISPLAY_TITLE_G, DISPLAY_TITLE_B, SDL_ALPHA_OPAQUE);
    (void)SDL_RenderDrawLine(renderer, punchLine[0], punchLine[1], punchLine[2], punchLine[3]);
    SDL_RenderCopy(renderer, readerTexture, NULL, &readerRect);
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_TITLE_R, DISPLAY_TITLE_G, DISPLAY_TITLE_B, SDL_ALPHA_OPAQUE);
    (void)SDL_RenderDrawLine(renderer, readerLine[0], readerLine[1], readerLine[2], readerLine[3]);
    SDL_SetRenderTarget(renderer, NULL);

    punchHopperSideNoCoverTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 27,
                                                      317);
    SDL_SetRenderTarget(renderer, punchHopperSideNoCoverTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(punchHopperSideNoCoverTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, punchHopperSideX3, punchHopperSideY3, 4, DISPLAY_HOPPER_RGBA_OPAQUE);
    for (int i = 0; i < (sizeof(punchHopperSideLines3) / (4 * sizeof(Sint16))); ++i) {
        (void)thickLineColor(renderer, punchHopperSideLines3[i][0], punchHopperSideLines3[i][1],
                             punchHopperSideLines3[i][2], punchHopperSideLines3[i][3], THICK_LINE_WIDTH,
                             DISPLAY_BLACK_RGBA_OPAQUE);
    }
    SDL_SetRenderTarget(renderer, NULL);

    punchHopperSideTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 27, 317);
    SDL_SetRenderTarget(renderer, punchHopperSideTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderCopy(renderer, punchHopperSideNoCoverTexture, NULL, NULL);
    SDL_SetTextureBlendMode(punchHopperSideTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, punchHopperCoverX3, punchHopperCoverY3, 3, DISPLAY_BACKGROUND_RGBA_OPAQUE);
    (void)thickLineColor(renderer, punchHopperSideLines3[2][0], punchHopperSideLines3[2][1],
                         punchHopperSideLines3[2][2], punchHopperSideLines3[2][3], THICK_LINE_WIDTH,
                         DISPLAY_BLACK_RGBA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);

    punchStackerSideTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 27, 268);
    SDL_SetRenderTarget(renderer, punchStackerSideTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(punchStackerSideTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, punchStackerSideX3, punchStackerSideY3, 4, DISPLAY_HOPPER_RGBA_OPAQUE);
    (void)filledPolygonColor(renderer, punchStackerCoverX3, punchStackerCoverY3, 3, DISPLAY_HOPPER_RGBA_OPAQUE);
    for (int i = 0; i < (sizeof(punchStackerSideLines3) / (4 * sizeof(Sint16))); ++i) {
        (void)thickLineColor(renderer, punchStackerSideLines3[i][0], punchStackerSideLines3[i][1],
                             punchStackerSideLines3[i][2], punchStackerSideLines3[i][3], THICK_LINE_WIDTH,
                             DISPLAY_BLACK_RGBA_OPAQUE);
    }
    SDL_SetRenderTarget(renderer, NULL);

    readStackerSideTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 27, 268);
    SDL_SetRenderTarget(renderer, readStackerSideTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(readStackerSideTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, readStackerSideX3, readStackerSideY3, 4, DISPLAY_HOPPER_RGBA_OPAQUE);
    (void)filledPolygonColor(renderer, readStackerCoverX3, readStackerCoverY3, 3, DISPLAY_HOPPER_RGBA_OPAQUE);
    for (int i = 0; i < (sizeof(readStackerSideLines3) / (4 * sizeof(Sint16))); ++i) {
        (void)thickLineColor(renderer, readStackerSideLines3[i][0], readStackerSideLines3[i][1],
                             readStackerSideLines3[i][2], readStackerSideLines3[i][3], THICK_LINE_WIDTH,
                             DISPLAY_BLACK_RGBA_OPAQUE);
    }
    SDL_SetRenderTarget(renderer, NULL);

    readHopperSideNoCoverTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 27,
                                                     317);
    SDL_SetRenderTarget(renderer, readHopperSideNoCoverTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, DISPLAY_WHITE_R, DISPLAY_WHITE_G, DISPLAY_WHITE_B, SDL_ALPHA_TRANSPARENT);
    SDL_RenderClear(renderer);
    SDL_SetTextureBlendMode(readHopperSideNoCoverTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, readHopperSideX3, readHopperSideY3, 4, DISPLAY_HOPPER_RGBA_OPAQUE);
    for (int i = 0; i < (sizeof(readHopperSideLines3) / (4 * sizeof(Sint16))); ++i) {
        (void)thickLineColor(renderer, readHopperSideLines3[i][0], readHopperSideLines3[i][1],
                             readHopperSideLines3[i][2], readHopperSideLines3[i][3], THICK_LINE_WIDTH,
                             DISPLAY_BLACK_RGBA_OPAQUE);
    }
    SDL_SetRenderTarget(renderer, NULL);

    readHopperSideTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 27, 317);
    SDL_SetRenderTarget(renderer, readHopperSideTexture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderCopy(renderer, readHopperSideNoCoverTexture, NULL, NULL);
    SDL_SetTextureBlendMode(readHopperSideTexture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    (void)filledPolygonColor(renderer, readHopperCoverX3, readHopperCoverY3, 3, DISPLAY_BACKGROUND_RGBA_OPAQUE);
    (void)thickLineColor(renderer, readHopperSideLines3[2][0], readHopperSideLines3[2][1], readHopperSideLines3[2][2],
                         readHopperSideLines3[2][3], THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    SDL_SetRenderTarget(renderer, NULL);

    image = IMG_Load(DIRECTORY_FILENAME);
    directoryTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(FILE_FILENAME);
    fileTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(BACKUP_OFF_FILENAME);
    backupOffTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(BACKUP_ON_FILENAME);
    backupOnTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(BACKUP_SELECTED_FILENAME);
    backupSelectedTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(UPUPARROW_OFF_FILENAME);
    upUpArrowOffTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(UPUPARROW_ON_FILENAME);
    upUpArrowOnTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(UPUPARROW_SELECTED_FILENAME);
    upUpArrowSelectedTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(UPARROW_OFF_FILENAME);
    upArrowOffTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(UPARROW_ON_FILENAME);
    upArrowOnTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(UPARROW_SELECTED_FILENAME);
    upArrowSelectedTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(DOWNARROW_OFF_FILENAME);
    downArrowOffTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(DOWNARROW_ON_FILENAME);
    downArrowOnTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(DOWNARROW_SELECTED_FILENAME);
    downArrowSelectedTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(DOWNDOWNARROW_OFF_FILENAME);
    downDownArrowOffTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(DOWNDOWNARROW_ON_FILENAME);
    downDownArrowOnTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(DOWNDOWNARROW_SELECTED_FILENAME);
    downDownArrowSelectedTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    image = IMG_Load(ITEM_SELECTED_FILENAME);
    itemSelectedTexture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_FreeSurface(image);
    IMG_Quit();

    SDL_SetRenderTarget(renderer, NULL);
}

// OpenFileSelectFonts
void OpenFileSelectFonts(void) {
    directoryFileFont = TTF_OpenFont(FONT_FILENAME, DIRECTORY_FILE_FONT_SIZE);
    (void)TTF_SetFontHinting(directoryFileFont, TTF_HINTING_MONO);

    extensionFont = TTF_OpenFont(FONT_FILENAME, EXTENSION_FONT_SIZE);
    (void)TTF_SetFontHinting(extensionFont, TTF_HINTING_MONO);
}

// DestroyTextures
void DestroyTextures(void) {
    SDL_DestroyTexture(splashTexture);

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(punchHopperSideTexture);
    SDL_DestroyTexture(punchHopperSideNoCoverTexture);
    SDL_DestroyTexture(punchStackerSideTexture);
    SDL_DestroyTexture(readStackerSideTexture);
    SDL_DestroyTexture(readHopperSideTexture);
    SDL_DestroyTexture(readHopperSideNoCoverTexture);

    SDL_DestroyTexture(leftCardTexture);
    SDL_DestroyTexture(leftCardCornerTexture);
    SDL_DestroyTexture(middleCardTexture);
    SDL_DestroyTexture(middleCardCornerTexture);
    SDL_DestroyTexture(rightCardTexture);
    SDL_DestroyTexture(rightCardCornerTexture);

    SDL_DestroyTexture(leftCardStackTexture);
    SDL_DestroyTexture(middleCardStackTexture);
    SDL_DestroyTexture(rightCardStackTexture);

    SDL_DestroyTexture(loadOffTexture);
    SDL_DestroyTexture(loadOnTexture);
    SDL_DestroyTexture(readerTexture);
    SDL_DestroyTexture(punchTexture);
    SDL_DestroyTexture(readyOffTexture);
    SDL_DestroyTexture(readyOnTexture);
    SDL_DestroyTexture(checkOffTexture);
    SDL_DestroyTexture(checkOnTexture);
    SDL_DestroyTexture(resetOffTexture);
    SDL_DestroyTexture(resetOnTexture);

    SDL_DestroyTexture(directoryTexture);
    SDL_DestroyTexture(fileTexture);
    SDL_DestroyTexture(backupOffTexture);
    SDL_DestroyTexture(backupOnTexture);
    SDL_DestroyTexture(backupSelectedTexture);
    SDL_DestroyTexture(upUpArrowOffTexture);
    SDL_DestroyTexture(upUpArrowOnTexture);
    SDL_DestroyTexture(upUpArrowSelectedTexture);
    SDL_DestroyTexture(upArrowOffTexture);
    SDL_DestroyTexture(upArrowOnTexture);
    SDL_DestroyTexture(upArrowSelectedTexture);
    SDL_DestroyTexture(downArrowOffTexture);
    SDL_DestroyTexture(downArrowOnTexture);
    SDL_DestroyTexture(downArrowSelectedTexture);
    SDL_DestroyTexture(downDownArrowOffTexture);
    SDL_DestroyTexture(downDownArrowOnTexture);
    SDL_DestroyTexture(downDownArrowSelectedTexture);
    SDL_DestroyTexture(itemSelectedTexture);
}

// CloseFileSelectFonts
void CloseFileSelectFonts(void) {
    TTF_CloseFont(directoryFileFont);
    TTF_CloseFont(extensionFont);
}

// ShowScreen
void ShowScreen(int disp) {

    display = disp;

    if (running) {

        if (ibm1620PowerState != STATE_POWER_ON) {
            ShowSplashScreen();
        } else {

            switch (display) {

                case DISPLAY_UNKNOWN:
                    break;

                case DISPLAY_SPLASH_SCREEN:
                    ShowSplashScreen();
                    break;

                case DISPLAY_READER_PUNCH:
                    ShowReaderPunch();
                    break;

                case DISPLAY_FILE_SELECT:
                    ShowFileSelect();
                    break;

                default:
                    break;
            }
        }
    }
}

// ShowSplashScreen
void ShowSplashScreen(void) {

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderCopy(renderer, splashTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// ShowReaderPunch
void ShowReaderPunch(void) {
    int top;
    int mask;

    // Clear background
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_BACKGROUND_R, DISPLAY_BACKGROUND_G,
                                 DISPLAY_BACKGROUND_B, SDL_ALPHA_OPAQUE);
    (void)SDL_RenderClear(renderer);

    // Update animation controls
    if (pAnimateControl != 0) {
        if ((pAnimateControl & ANIMATE_NORMAL_LAST_MASK) != 0) ++pStackerCount;
        pAnimateControl = (pAnimateControl << 1) & ANIMATE_NORMAL_MASK;
    }
    if (pErrAnimateControl != 0) {
        if ((pErrAnimateControl & ANIMATE_ERROR_LAST_MASK) != 0) ++pErrStackerCount;
        pErrAnimateControl = (pErrAnimateControl << 1) & ANIMATE_ERROR_MASK;
    }
    if (rErrAnimateControl != 0) {
        if ((rErrAnimateControl & ANIMATE_ERROR_LAST_MASK) != 0) ++rErrStackerCount;
        rErrAnimateControl = (rErrAnimateControl << 1) & ANIMATE_ERROR_MASK;
    }
    if (rAnimateControl != 0) {
        if ((rAnimateControl & ANIMATE_NORMAL_LAST_MASK) != 0) ++rStackerCount;
        rAnimateControl = (rAnimateControl << 1) & ANIMATE_NORMAL_MASK;
    }

    // Stage 1 - hopper & stacker backs, bottoms, and outer lines

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Stage 2 - draw hopper, normal stacker, and normal animation cards

    if (punchUSBPresent) {
        if (((pAnimateControl & ANIMATE_EMERGE_MASK) != 0) || ((pErrAnimateControl & ANIMATE_EMERGE_MASK) != 0)) {
            DrawCard(pErrAnimateLocations[0][0], pErrAnimateLocations[0][1]);
        }
        (void)DrawCardStack(PUNCH_HOPPER, MAX(pHopperCount, 1));
        top = DrawCardStack(PUNCH_STACKER, pStackerCount);
        if ((pAnimateControl & ANIMATE_NORMAL_REMAINDER_MASK) != 0) {
            mask = 0x004;
            for (int i = 1; i < 5; ++i) {
                if (pAnimateLocations[i][1] >= top) break;
                if ((pAnimateControl & mask) != 0) {
                    DrawCard(pAnimateLocations[i][0], pAnimateLocations[i][1]);
                }
                mask = mask << 1;
            }
        }
    }

    if (readerUSBPresent) {
        if (((rAnimateControl & ANIMATE_EMERGE_MASK) != 0) || ((rErrAnimateControl & ANIMATE_EMERGE_MASK) != 0)) {
            DrawCard(rErrAnimateLocations[0][0], rErrAnimateLocations[0][1]);
        }
        top = DrawCardStack(READ_STACKER, rStackerCount);
        (void)DrawCardStack(READ_HOPPER, rHopperCount);
        if ((rAnimateControl & ANIMATE_NORMAL_REMAINDER_MASK) != 0) {
            mask = 0x004;
            for (int i = 1; i < 5; ++i) {
                if (rAnimateLocations[i][1] >= top) break;
                if ((rAnimateControl & mask) != 0) {
                    DrawCard(rAnimateLocations[i][0], rAnimateLocations[i][1]);
                }
                mask = mask << 1;
            }
        }
    }

    // Stage 3 - draw hopper and normal stacker sides

    if (((pAnimateControl & ANIMATE_EMERGE_MASK) != 0) || ((pErrAnimateControl & ANIMATE_EMERGE_MASK) != 0)) {
        SDL_RenderCopy(renderer, punchHopperSideNoCoverTexture, NULL, &punchHopperSideRect3);
    } else {
        SDL_RenderCopy(renderer, punchHopperSideTexture, NULL, &punchHopperSideRect3);
    }
    SDL_RenderCopy(renderer, punchStackerSideTexture, NULL, &punchStackerSide2Rect3);
    SDL_RenderCopy(renderer, readStackerSideTexture, NULL, &readStackerSide5Rect3);
    if (((rAnimateControl & ANIMATE_EMERGE_MASK) != 0) || ((rErrAnimateControl & ANIMATE_EMERGE_MASK) != 0)) {
        SDL_RenderCopy(renderer, readHopperSideNoCoverTexture, NULL, &readHopperSideRect3);
    } else {
        SDL_RenderCopy(renderer, readHopperSideTexture, NULL, &readHopperSideRect3);
    }

    // Stage 4 - draw error stacker and error animation cards

    if (punchUSBPresent) {
        top = DrawCardStack(PUNCH_ERROR_STACKER, pErrStackerCount);
        if ((pErrAnimateControl & ANIMATE_ERROR_REMAINDER_MASK) != 0) {
            mask = 0x004;
            for (int i = 1; i < 6; ++i) {
                if (pErrAnimateLocations[i][1] >= top) break;
                if ((pErrAnimateControl & mask) != 0) {
                    DrawCard(pErrAnimateLocations[i][0], pErrAnimateLocations[i][1]);
                }
                mask = mask << 1;
            }
        }
    }

    if (readerUSBPresent) {
        top = DrawCardStack(READ_ERROR_STACKER, rErrStackerCount);
        if ((rErrAnimateControl & ANIMATE_ERROR_REMAINDER_MASK) != 0) {
            mask = 0x004;
            for (int i = 1; i < 6; ++i) {
                if (rErrAnimateLocations[i][1] >= top) break;
                if ((rErrAnimateControl & mask) != 0) {
                    DrawCard(rErrAnimateLocations[i][0], rErrAnimateLocations[i][1]);
                }
                mask = mask << 1;
            }
        }
    }

    // Stage 5 - draw error stacker sides

    SDL_RenderCopy(renderer, punchStackerSideTexture, NULL, &punchStackerSide3Rect5);
    SDL_RenderCopy(renderer, readStackerSideTexture, NULL, &readStackerSide4Rect5);

    // Stage 6 - draw middle stacker cards

    (void)DrawCardStack(MIDDLE_STACKER, mStackerCount);

    // Stage 7 - draw control panel lights and buttons

    if (punchStatus == STATUS_READY) {
        (void)SDL_RenderCopy(renderer, readyOnTexture, NULL, &punchReadyRect);
        (void)SDL_RenderCopy(renderer, checkOffTexture, NULL, &punchCheckRect);
    } else if (punchStatus == STATUS_CHECK) {
        (void)SDL_RenderCopy(renderer, readyOffTexture, NULL, &punchReadyRect);
        (void)SDL_RenderCopy(renderer, checkOnTexture, NULL, &punchCheckRect);
    } else {
        (void)SDL_RenderCopy(renderer, readyOffTexture, NULL, &punchReadyRect);
        (void)SDL_RenderCopy(renderer, checkOffTexture, NULL, &punchCheckRect);
    }

    if (readerStatus == STATUS_READY) {
        (void)SDL_RenderCopy(renderer, readyOnTexture, NULL, &readerReadyRect);
        (void)SDL_RenderCopy(renderer, checkOffTexture, NULL, &readerCheckRect);
    } else if (readerStatus == STATUS_CHECK) {
        (void)SDL_RenderCopy(renderer, readyOffTexture, NULL, &readerReadyRect);
        (void)SDL_RenderCopy(renderer, checkOnTexture, NULL, &readerCheckRect);
    } else {
        (void)SDL_RenderCopy(renderer, readyOffTexture, NULL, &readerReadyRect);
        (void)SDL_RenderCopy(renderer, checkOffTexture, NULL, &readerCheckRect);
    }

    // Draw control panel
    if (buttonState == BUTTON_LOAD_PRESSED) {
        (void)SDL_RenderCopy(renderer, loadOnTexture, NULL, &loadRect);
    } else {
        (void)SDL_RenderCopy(renderer, loadOffTexture, NULL, &loadRect);
    }

    if (buttonState == BUTTON_RESET_PRESSED) {
        (void)SDL_RenderCopy(renderer, resetOnTexture, NULL, &resetRect);
    } else {
        (void)SDL_RenderCopy(renderer, resetOffTexture, NULL, &resetRect);
    }

    SDL_RenderPresent(renderer);
}

// AnimateCardRead
void AnimateCardRead(void) {
    --rHopperCount;
    rAnimateControl |= 0x1;
}

// AnimateCardReadError
void AnimateCardReadError(void) {
    --rHopperCount;
    rErrAnimateControl |= 0x1;
}

// AnimateCardPunch
void AnimateCardPunch(void) {
    --pHopperCount;
    pAnimateControl |= 0x1;
}

// AnimateCardPunchError
void AnimateCardPunchError(void) {
    --pHopperCount;
    pErrAnimateControl |= 0x1;
}

// DisplayCallback
Uint32 DisplayCallback(Uint32 interval, void* param) {
    SDL_Event event;
    SDL_UserEvent uevent;

    uevent.type = SDL_USEREVENT;
    uevent.code = 0;
    uevent.data1 = NULL;
    uevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = uevent;

    SDL_PushEvent(&event);

    return DISPLAY_REFRESH_MS;
}

// DrawCard
void DrawCard(int x, int y) {
    SDL_Rect rect;

    if (x < 454) {
        rect.w = 138;
        rect.h = 68;
        rect.x = x - 2;
        rect.y = y - 68;
        SDL_RenderCopy(renderer, leftCardTexture, NULL, (const SDL_Rect *)&rect);

    } else if (x < 569) {
        rect.w = 116;
        rect.h = 68;
        rect.x = x - 2;
        rect.y = y - 68;
        SDL_RenderCopy(renderer, middleCardTexture, NULL, (const SDL_Rect *)&rect);

    } else {
        rect.w = 134;
        rect.h = 68;
        rect.x = x - 2;
        rect.y = y - 68;
        SDL_RenderCopy(renderer, rightCardTexture, NULL, (const SDL_Rect *)&rect);
    }
}

// DrawCardStack
int DrawCardStack(int stack, int cnt) {
    int      count;
    Sint16   height;
    SDL_Rect sRect;
    SDL_Rect dRect;

    count = MIN(MAX(cnt, 0), stacks[stack].max);
    if (count == 0) return stacks[stack].y;

    height = (Sint16)((float)(count) * stacks[stack].ratio + 0.5);
    if (height == 0) {
        DrawCard(stacks[stack].x, stacks[stack].y);
        return stacks[stack].y;
    }

    sRect.x = 0;
    sRect.y = 0;
    sRect.w = stacks[stack].width;
    sRect.h = height + 65;
    dRect.x = stacks[stack].x + 2;
    dRect.y = stacks[stack].y - height - 67;
    dRect.w = stacks[stack].width;
    dRect.h = height + 65;
    SDL_RenderCopy(renderer, *(stacks[stack].stack), (const SDL_Rect *)&sRect, (const SDL_Rect *)&dRect);

    dRect.x = stacks[stack].cornerX + 2;
    dRect.y = stacks[stack].cornerY - 2;
    dRect.w = 35;
    dRect.h = 16;
    SDL_RenderCopy(renderer, *(stacks[stack].corner), NULL, (const SDL_Rect *)&dRect);

    return stacks[stack].y - height;
}

// ShowFileSelect
void ShowFileSelect(void) {
    int w;
    int h;
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;

    // Clear background
    (void)SDL_SetRenderDrawColor(renderer, DISPLAY_BACKGROUND_R, DISPLAY_BACKGROUND_G,
                                 DISPLAY_BACKGROUND_B, SDL_ALPHA_OPAQUE);
    (void)SDL_RenderClear(renderer);

    // Draw separators
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    (void)thickLineColor(renderer,   0,  53, 1023,  53, THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    (void)thickLineColor(renderer, 968,   0,  968, 599, THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    (void)thickLineColor(renderer, 971,  53, 1023,  53, THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    (void)thickLineColor(renderer, 971, 131, 1023, 131, THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    (void)thickLineColor(renderer, 971, 189, 1023, 189, THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    (void)thickLineColor(renderer, 971, 466, 1023, 466, THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    (void)thickLineColor(renderer, 971, 524, 1023, 524, THICK_LINE_WIDTH, DISPLAY_BLACK_RGBA_OPAQUE);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Draw action buttons
    if (selectedItem == SELECTED_BACKUP) {
        (void)SDL_RenderCopy(renderer, backupSelectedTexture, NULL, &backupRect);
    } else if (strlen(title) > 2) {
        (void)SDL_RenderCopy(renderer, backupOnTexture, NULL, &backupRect);
    } else {
        (void)SDL_RenderCopy(renderer, backupOffTexture, NULL, &backupRect);
    }

    if (selectedItem == SELECTED_UPUPARROW) {
        (void)SDL_RenderCopy(renderer, upUpArrowSelectedTexture, NULL, &upUpArrowRect);
    } else if (firstEntry > 0) {
        (void)SDL_RenderCopy(renderer, upUpArrowOnTexture, NULL, &upUpArrowRect);
    } else {
        (void)SDL_RenderCopy(renderer, upUpArrowOffTexture, NULL, &upUpArrowRect);
    }

    if (selectedItem == SELECTED_UPARROW) {
        (void)SDL_RenderCopy(renderer, upArrowSelectedTexture, NULL, &upArrowRect);
    } else if (firstEntry > 0) {
        (void)SDL_RenderCopy(renderer, upArrowOnTexture, NULL, &upArrowRect);
    } else {
        (void)SDL_RenderCopy(renderer, upArrowOffTexture, NULL, &upArrowRect);
    }

    if (selectedItem == SELECTED_DOWNARROW) {
        (void)SDL_RenderCopy(renderer, downArrowSelectedTexture, NULL, &downArrowRect);
    } else if (numEntries > (firstEntry + 11)) {
        (void)SDL_RenderCopy(renderer, downArrowOnTexture, NULL, &downArrowRect);
    } else {
        (void)SDL_RenderCopy(renderer, downArrowOffTexture, NULL, &downArrowRect);
    }

    if (selectedItem == SELECTED_DOWNDOWNARROW) {
        (void)SDL_RenderCopy(renderer, downDownArrowSelectedTexture, NULL, &downDownArrowRect);
    } else if (numEntries > (firstEntry + 11)) {
        (void)SDL_RenderCopy(renderer, downDownArrowOnTexture, NULL, &downDownArrowRect);
    } else {
        (void)SDL_RenderCopy(renderer, downDownArrowOffTexture, NULL, &downDownArrowRect);
    }

    (void)pthread_mutex_lock(&displayLock);

    // Add directory name
    surface = TTF_RenderText_Solid(directoryFileFont, title, fileSelectColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    (void)SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    rect.x = 4;
    rect.y = 0;
    rect.w = w;
    rect.h = h;
    (void)SDL_RenderCopy(renderer, texture, NULL, &rect);
    (void)SDL_DestroyTexture(texture);

    // Add directory contents
    for (int i = 0; (i < 11) && ((firstEntry + i) < numEntries); ++i) {
        if (selectedItem == (SELECTED_ITEMS + i)) {
            rect.x = 0;
            rect.y = layout[i].icon_y;
            rect.w = 965;
            rect.h = 49;
            (void)SDL_RenderCopy(renderer, itemSelectedTexture, NULL, (const SDL_Rect *)&rect);
        }
        rect.x = layout[i].icon_x;
        rect.y = layout[i].icon_y;
        rect.w = 49;
        rect.h = 49;
        if (items[firstEntry + i].type == DIRECTORY_TYPE) {
            (void)SDL_RenderCopy(renderer, directoryTexture, NULL, (const SDL_Rect *)&rect);
        } else {
            (void)SDL_RenderCopy(renderer, fileTexture, NULL, (const SDL_Rect *)&rect);
            if (items[firstEntry + i].extension[0] != (char)0) {
                surface = TTF_RenderText_Solid(extensionFont, items[firstEntry + i].extension, fileSelectColor);
                texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                (void)SDL_QueryTexture(texture, NULL, NULL, &w, &h);
                rect.x = layout[i].icon_center_x - (w / 2);
                rect.y = layout[i].icon_center_y - (h / 2);
                rect.w = w;
                rect.h = h;
                (void)SDL_RenderCopy(renderer, texture, NULL, (const SDL_Rect *)&rect);
                (void)SDL_DestroyTexture(texture);
            }
        }
        if (items[firstEntry + i].filename[0] != (char)0) {
            surface = TTF_RenderText_Solid(directoryFileFont, items[firstEntry + i].filename, fileSelectColor);
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            (void)SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            rect.x = layout[i].text_x;
            rect.y = layout[i].text_y;
            rect.w = w;
            rect.h = h;
            (void)SDL_RenderCopy(renderer, texture, NULL, (const SDL_Rect *)&rect);
            (void)SDL_DestroyTexture(texture);
        }
        (void)thickLineColor(renderer, 0, layout[i].text_y + 49, 965, layout[i].text_y + 49, THIN_LINE_WIDTH,
                             DISPLAY_BLACK_RGBA_OPAQUE);
    }

    (void)pthread_mutex_unlock(&displayLock);

    SDL_RenderPresent(renderer);
}
