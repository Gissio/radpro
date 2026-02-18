/*
 * Rad Pro
 * Game
 *
 * (C) 2022-2026 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>
#include <stdint.h>

#include <mcu-max.h>

#include "../extras/game.h"
#include "../system/cmath.h"
#include "../system/events.h"
#include "../system/settings.h"
#include "../ui/game.h"
#include "../ui/menu.h"
#include "../ui/view.h"

#if defined(STM32F0)
#define GAME_DEPTH_MAX 16
#else
#define GAME_DEPTH_MAX 32
#endif

#define GAME_VALID_MOVES_NUM_MAX 181

typedef enum
{
    GAME_SHOWING_LAST_MOVE,
    GAME_SELECTING_FROM,
    GAME_SELECTING_TO,
    GAME_SEARCHING,
    GAME_SEARCH_STOPPED,
    GAME_OVER,
} GameState;

static struct
{
    GameState state;
    uint32_t moveIndex;
    uint32_t playerIndex;
    uint32_t playerTime[2];
    uint8_t board[8][8];

    int32_t validMovesNum;
    int32_t validMovesIndex;
    mcumax_move validMoves[GAME_VALID_MOVES_NUM_MAX];

    mcumax_move lastMove;
} game;

static const uint32_t gameStrengthToNodesCount[] = {
    1,
    4096,
    8192,
    16384,
    32768,
    65536,
    131072,
    262144,
};

static const Menu gameStartMenu;
static const Menu gameContinueMenu;
static const Menu gameStrengthMenu;

void setupGame(void)
{
    memset(&game, 0, sizeof(game));

    selectMenuItem(&gameStartMenu, 0);
    selectMenuItem(&gameContinueMenu, 0);
    selectMenuItem(&gameStrengthMenu, settings.gameStrength);
}

static void updateGameBoard(void)
{
    mcumax_move move = MCUMAX_MOVE_INVALID;

    switch (game.state)
    {
    case GAME_SHOWING_LAST_MOVE:
        if (game.lastMove.from != MCUMAX_SQUARE_INVALID)
            move = game.lastMove;

        break;

    case GAME_SELECTING_FROM:
        move = (mcumax_move){
            game.validMoves[game.validMovesIndex].from,
            MCUMAX_SQUARE_INVALID};

        break;

    case GAME_SELECTING_TO:
        move = game.validMoves[game.validMovesIndex];

        break;

    default:
        break;
    }

    for (uint32_t y = 0; y < 8; y++)
    {
        for (uint32_t x = 0; x < 8; x++)
        {
            mcumax_square square = ((0x10 * y) + x);

            uint8_t xb = game.playerIndex ? (7 - x) : x;
            uint8_t yb = game.playerIndex ? (7 - y) : y;

            uint8_t piece = mcumax_get_piece(square);

            if ((square == move.from) || (square == move.to))
                piece |= 0x10;

            game.board[yb][xb] = piece;
        }
    }

    requestViewUpdate();
}

static void onGameCallback(void *userdata)
{
#if !defined(__EMSCRIPTEN__)
    updateEvents();
#endif
}

static void updateValidMoves(void)
{
    game.validMovesNum = mcumax_search_valid_moves(game.validMoves, GAME_VALID_MOVES_NUM_MAX);
    game.validMovesIndex = game.playerIndex ? (game.validMovesNum - 1) : 0;
}

static void startGame(uint32_t playerIndex)
{
    mcumax_init();

    game.moveIndex = 0;
    game.playerIndex = playerIndex;
    game.playerTime[0] = 0;
    game.playerTime[1] = 0;

    game.lastMove = MCUMAX_MOVE_INVALID;

    if (playerIndex == 0)
    {
        game.state = GAME_SELECTING_FROM;

        updateValidMoves();
    }
    else
        game.state = GAME_SEARCHING;

    updateGameBoard();
}

void updateGame(void)
{
    if (game.state == GAME_SEARCHING)
    {
        mcumax_set_callback(onGameCallback, NULL);
        mcumax_move move = mcumax_search_best_move(gameStrengthToNodesCount[settings.gameStrength], GAME_DEPTH_MAX);
        mcumax_set_callback(NULL, NULL);

        if (game.state == GAME_SEARCH_STOPPED)
            showGameMenu();
        else
        {
            if (move.from == MCUMAX_SQUARE_INVALID)
                game.state = GAME_OVER;
            else
            {
                mcumax_play_move(move);

                game.moveIndex++;
                game.lastMove = move;

                updateValidMoves();

                if (game.validMovesNum != 0)
                    game.state = GAME_SHOWING_LAST_MOVE;
                else
                    game.state = GAME_OVER;
            }

            updateGameBoard();
        }
    }
}

static bool stepValidMove(int32_t direction)
{
    game.validMovesIndex += direction;

    if (game.validMovesIndex < 0)
        game.validMovesIndex = game.validMovesNum - 1;
    else if (game.validMovesIndex > (game.validMovesNum - 1))
        game.validMovesIndex = 0;
    else
        return false;

    return true;
}

static void stepValidMoveFrom(int32_t direction)
{
    mcumax_square moveFrom = game.validMoves[game.validMovesIndex].from;

    while (true)
    {
        if (stepValidMove(direction))
            return;

        if (game.validMoves[game.validMovesIndex].from != moveFrom)
            return;
    }
}

static void selectNextMoveFrom(int32_t direction)
{
    if (game.playerIndex)
        direction = -direction;

    stepValidMoveFrom(direction);

    updateGameBoard();
}

static void selectNextMoveTo(int32_t direction)
{
    mcumax_square moveFrom = game.validMoves[game.validMovesIndex].from;

    stepValidMove(direction);

    if (moveFrom != game.validMoves[game.validMovesIndex].from)
    {
        stepValidMove(-direction);
        stepValidMoveFrom(-direction);
        stepValidMove(direction);
    }

    updateGameBoard();
}

// Game view

static void onGameViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        if (game.state == GAME_SEARCHING)
        {
            mcumax_stop_search();

            game.state = GAME_SEARCH_STOPPED;
        }
        else if (game.state == GAME_SELECTING_TO)
        {
            game.state = GAME_SELECTING_FROM;

            updateGameBoard();
        }
        else if (game.state != GAME_SEARCH_STOPPED)
            showGameMenu();

        break;

    case EVENT_KEY_SELECT:
        switch (game.state)
        {
        case GAME_SELECTING_FROM:
            if (game.validMovesNum)
            {
                game.state = GAME_SELECTING_TO;

                stepValidMoveFrom(-1);
                stepValidMove(1);

                updateGameBoard();
            }

            break;

        case GAME_SELECTING_TO:
        {
            mcumax_move move = game.validMoves[game.validMovesIndex];

            mcumax_play_move(move);

            game.moveIndex++;
            game.lastMove = move;

            game.state = GAME_SEARCHING;

            updateGameBoard();

            break;
        }

        default:
            break;
        }

        break;

    case EVENT_KEY_UP:
    case EVENT_KEY_DOWN:
        if (game.state == GAME_SHOWING_LAST_MOVE)
        {
            game.state = GAME_SELECTING_FROM;

            updateGameBoard();
        }
        else if (game.state == GAME_SELECTING_FROM)
            selectNextMoveFrom((event == EVENT_KEY_UP) ? -1 : 1);

        else if (game.state == GAME_SELECTING_TO)
            selectNextMoveTo((event == EVENT_KEY_UP) ? -1 : 1);

        break;

    case EVENT_DRAW:
    {
        // Time
        char time[2][16];

        strclr(time[0]);
        strcatTime(time[0], game.playerTime[game.playerIndex]);

        strclr(time[1]);
        strcatTime(time[1], game.playerTime[!game.playerIndex]);

        // Moves
        drawGame(game.board, time);

        break;
    }

    case EVENT_HEARTBEAT:
        if (game.state != GAME_OVER)
            game.playerTime[game.moveIndex & 0x1]++;

        break;

    default:
        break;
    }
}

// Game strength menu

static const char *onGameStrengthMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.gameStrength);

    strcpy(menuOption, getString(STRING_GAME_LEVEL));
    strcatChar(menuOption, ' ');
    strcatUInt32(menuOption, index + 1, 0);

    return menuOption;
}

static void onGameStrengthMenuSelect(menu_size_t index)
{
    settings.gameStrength = index;
}

static MenuState gameStrengthMenuState;

static const Menu gameStrengthMenu = {
    STRING_STRENGTH,
    &gameStrengthMenuState,
    GAME_STRENGTH_NUM,
    onGameStrengthMenuGetOption,
    onGameStrengthMenuSelect,
    showGameMenu,
};

// Game start menu

static cstring gameStartMenuOptions[] = {
    STRING_PLAY_WHITE,
    STRING_PLAY_BLACK,
    STRING_STRENGTH,
};

static const char *onGameStartMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    return getString(gameStartMenuOptions[index]);
}

static void onGameStartMenuSelect(menu_size_t index)
{
    switch (index)
    {
    case 0:
    case 1:
        startGame(index);

        showView(onGameViewEvent);

        break;

    case 2:
        showMenu(&gameStrengthMenu);

        break;
    }
}

static MenuState gameStartMenuState;

static const Menu gameStartMenu = {
    STRING_GAME,
    &gameStartMenuState,
    ARRAY_SIZE(gameStartMenuOptions),
    onGameStartMenuGetOption,
    onGameStartMenuSelect,
    showSettingsMenu,
};

// Game continue menu

static cstring gameContinueMenuOptions[] = {
    STRING_CONTINUE_GAME,
    STRING_NEW_GAME,
    STRING_STRENGTH,
};

static const char *onGameContinueMenuGetOption(menu_size_t index, MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    if (!game.moveIndex)
        return getString(gameStartMenuOptions[index]);
    else
        return getString(gameContinueMenuOptions[index]);
}

static void onGameContinueMenuSelect(menu_size_t index)
{
    switch (index)
    {
    case 0:
        if (game.state == GAME_SEARCH_STOPPED)
            game.state = GAME_SEARCHING;

        showView(onGameViewEvent);

        break;

    case 1:
        showMenu(&gameStartMenu);

        break;

    case 2:
        showMenu(&gameStrengthMenu);

        break;
    }
}

static MenuState gameContinueMenuState;

static const Menu gameContinueMenu = {
    STRING_GAME,
    &gameContinueMenuState,
    ARRAY_SIZE(gameStartMenuOptions),
    onGameContinueMenuGetOption,
    onGameContinueMenuSelect,
    showSettingsMenu,
};

// Game menu

void showGameMenu(void)
{
    if (!game.moveIndex)
        showMenu(&gameStartMenu);
    else
        showMenu(&gameContinueMenu);
}
