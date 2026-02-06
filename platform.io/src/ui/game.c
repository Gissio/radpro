/*
 * Rad Pro
 * UI game
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#if defined(GAME)

#include "../system/cstring.h"
#include "../ui/draw.h"
#include "../ui/game.h"
#include "../ui/system.h"

#if defined(DISPLAY_128X64)
#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP 0
#define GAME_SQUARE_WIDTH 9
#define GAME_SQUARE_HEIGHT 8
#elif defined(DISPLAY_320X240)
#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP 0
#define GAME_SQUARE_WIDTH 30
#define GAME_SQUARE_HEIGHT 30
#elif defined(DISPLAY_240X320)
#define GAME_BOARD_LEFT 0
#define GAME_BOARD_TOP CONTENT_TOP
#define GAME_SQUARE_WIDTH 30
#define GAME_SQUARE_HEIGHT 30
#endif

#define GAME_BOARD_WIDTH (8 * GAME_SQUARE_WIDTH)
#define GAME_BOARD_HEIGHT (8 * GAME_SQUARE_HEIGHT)
#define GAME_BOARD_RIGHT (GAME_BOARD_LEFT + GAME_BOARD_WIDTH)
#define GAME_BOARD_BOTTOM (GAME_BOARD_TOP + GAME_BOARD_HEIGHT)

#if defined(DISPLAY_LANDSCAPE)
#define GAME_PLAYER_TIMER_LEFT GAME_BOARD_RIGHT
#define GAME_PLAYER_TIMER_TOP (GAME_COMPUTER_TIMER_TOP + GAME_COMPUTER_TIMER_HEIGHT)
#define GAME_PLAYER_TIMER_WIDTH GAME_COMPUTER_TIMER_WIDTH
#define GAME_PLAYER_TIMER_HEIGHT (GAME_BOARD_HEIGHT - GAME_COMPUTER_TIMER_HEIGHT)
#define GAME_PLAYER_TIMER_OFFSETX (GAME_PLAYER_TIMER_WIDTH / 2)
#define GAME_PLAYER_TIMER_OFFSETY (GAME_PLAYER_TIMER_HEIGHT - 2 * GAME_SQUARE_HEIGHT + GAME_COMPUTER_TIMER_OFFSETY)

#define GAME_COMPUTER_TIMER_LEFT GAME_BOARD_RIGHT
#define GAME_COMPUTER_TIMER_TOP GAME_BOARD_TOP
#define GAME_COMPUTER_TIMER_WIDTH (CONTENT_RIGHT - GAME_BOARD_RIGHT)
#define GAME_COMPUTER_TIMER_HEIGHT (CONTENT_HEIGHT / 2)
#define GAME_COMPUTER_TIMER_OFFSETX (GAME_COMPUTER_TIMER_WIDTH / 2)
#define GAME_COMPUTER_TIMER_OFFSETY ((2 * GAME_SQUARE_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)
#elif defined(DISPLAY_PORTRAIT)
#define GAME_PLAYER_TIMER_LEFT GAME_BOARD_LEFT
#define GAME_PLAYER_TIMER_TOP GAME_BOARD_BOTTOM
#define GAME_PLAYER_TIMER_WIDTH (GAME_BOARD_WIDTH / 2)
#define GAME_PLAYER_TIMER_HEIGHT (CONTENT_BOTTOM - GAME_BOARD_BOTTOM)
#define GAME_PLAYER_TIMER_OFFSETX (GAME_PLAYER_TIMER_WIDTH / 2)
#define GAME_PLAYER_TIMER_OFFSETY ((GAME_PLAYER_TIMER_HEIGHT - FONT_SMALL_LINE_HEIGHT) / 2)

#define GAME_COMPUTER_TIMER_LEFT (GAME_PLAYER_TIMER_LEFT + GAME_PLAYER_TIMER_WIDTH)
#define GAME_COMPUTER_TIMER_TOP GAME_BOARD_BOTTOM
#define GAME_COMPUTER_TIMER_WIDTH (GAME_BOARD_WIDTH - GAME_PLAYER_TIMER_WIDTH)
#define GAME_COMPUTER_TIMER_HEIGHT GAME_PLAYER_TIMER_HEIGHT
#define GAME_COMPUTER_TIMER_OFFSETX (GAME_COMPUTER_TIMER_WIDTH / 2)
#define GAME_COMPUTER_TIMER_OFFSETY GAME_PLAYER_TIMER_OFFSETY
#endif

void drawGame(const uint8_t board[8][8], const char time[2][16])
{
    mr_rectangle_t rectangle;

#if defined(DISPLAY_COLOR) && defined(DISPLAY_240X320)
    drawTitleBar(getString(STRING_GAME));
#endif

    // Board
    static const char *const gamePieceMap = "@AACFBDE";

    setFont(font_symbols);

    rectangle.width = GAME_SQUARE_WIDTH;
    rectangle.height = GAME_SQUARE_HEIGHT;

    const mr_point_t gamePieceOffset = {
        (GAME_SQUARE_WIDTH / 2),
        (GAME_SQUARE_HEIGHT - FONT_SYMBOLS_LINE_HEIGHT) / 2,
    };

    for (uint32_t y = 0; y < 8; y++)
    {
        for (uint32_t x = 0; x < 8; x++)
        {
            uint8_t pieceCode = board[y][x];

            uint8_t pieceType = (pieceCode >> 0) & 0x7;
            bool pieceBlack = (pieceCode >> 3) & 0x1;
            bool pieceSelected = (pieceCode >> 4) & 0x1;
            bool squareBlack = (x + y) & 0x1;

            char pieceChar = gamePieceMap[pieceType];

#if defined(DISPLAY_MONOCHROME)
            bool isInverted;
            if (pieceSelected)
            {
                pieceChar |= 0x10;
                isInverted = !pieceBlack;
            }
            else
            {
                pieceChar |= (pieceBlack ^ squareBlack ^ 1) << 3;
                isInverted = squareBlack;
            }
            setFillColor(isInverted
                             ? COLOR_GAME_SQUARE_BLACK
                             : COLOR_GAME_SQUARE_WHITE);
#elif defined(DISPLAY_COLOR)
            setStrokeColor(pieceBlack ? COLOR_GAME_PIECE_BLACK : COLOR_GAME_PIECE_WHITE);
            setFillColor(pieceSelected ? COLOR_SELECTED_ENABLED_BACKGROUND : squareBlack ? COLOR_GAME_SQUARE_BLACK
                                                                                         : COLOR_GAME_SQUARE_WHITE);
#endif

            char pieceString[2];
            strclr(pieceString);
            strcatChar(pieceString, pieceChar);

            rectangle.x = GAME_BOARD_LEFT + GAME_SQUARE_WIDTH * x;
            rectangle.y = GAME_BOARD_TOP + GAME_SQUARE_HEIGHT * y;

            drawCenteredText(pieceString, &rectangle, &gamePieceOffset);
        }
    }

    // Timers common
    setFont(font_small);
    setStrokeColor(COLOR_ELEMENT_NEUTRAL);
    setFillColor(COLOR_CONTAINER_BACKGROUND);

    // Player timer
    drawCenteredText(time[0],
                     &(mr_rectangle_t){GAME_PLAYER_TIMER_LEFT, GAME_PLAYER_TIMER_TOP, GAME_PLAYER_TIMER_WIDTH, GAME_PLAYER_TIMER_HEIGHT},
                     &(mr_point_t){GAME_PLAYER_TIMER_OFFSETX, GAME_PLAYER_TIMER_OFFSETY});

    // Computer timer
    drawCenteredText(time[1],
                     &(mr_rectangle_t){GAME_COMPUTER_TIMER_LEFT, GAME_COMPUTER_TIMER_TOP, GAME_COMPUTER_TIMER_WIDTH, GAME_COMPUTER_TIMER_HEIGHT},
                     &(mr_point_t){GAME_COMPUTER_TIMER_OFFSETX, GAME_COMPUTER_TIMER_OFFSETY});
}

#endif
