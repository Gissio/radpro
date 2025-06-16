/*
 * Rad Pro
 * Nuclear chess
 *
 * (C) 2022-2025 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>
#include <stdint.h>

#include <mcu-max.h>

#include "cstring.h"
#include "display.h"
#include "events.h"
#include "game.h"
#include "menu.h"
#include "settings.h"

#define GAME_MENU_OPTIONS_NUM 3

#if defined(STM32F0)
#define GAME_DEPTH_MAX 16
#else
#define GAME_DEPTH_MAX 32
#endif

#define GAME_HISTORY_MOVE_NUM (GAME_HISTORY_TURN_NUM * 2)
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

    mcumax_move history[GAME_HISTORY_TURN_NUM * 2];
} game;

static const uint16_t gameStrengthToNodesCount[] = {
    1,
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
};

static const Menu gameMenu;
static const Menu gameStrengthMenu;

static void onGameCallback(void *userdata);

void resetGame(void)
{
    memset(&game, 0, sizeof(game));

    selectMenuItem(&gameMenu,
                   0,
                   0);
    selectMenuItem(&gameStrengthMenu,
                   settings.gameStrength,
                   GAMESTRENGTH_NUM);
}

static void recordGameMove(mcumax_move move)
{
    if (game.moveIndex < GAME_HISTORY_MOVE_NUM)
        game.history[game.moveIndex] = move;
    else
    {
        uint32_t index = game.moveIndex & 0x1;
        if (index == 0)
        {
            for (uint32_t i = 2; i < GAME_HISTORY_MOVE_NUM; i++)
                game.history[i - 2] = game.history[i];

            game.history[GAME_HISTORY_MOVE_NUM - 1] = MCUMAX_MOVE_INVALID;
        }

        game.history[GAME_HISTORY_MOVE_NUM - 2 + index] = move;
    }

    game.moveIndex++;
}

static void updateGameBoard(void)
{
    mcumax_move move = MCUMAX_MOVE_INVALID;

    switch (game.state)
    {
    case GAME_SHOWING_LAST_MOVE:
        for (uint32_t i = 0; i < GAME_HISTORY_MOVE_NUM; i++)
            if (game.history[i].from != MCUMAX_SQUARE_INVALID)
                move = game.history[i];

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

            uint8_t xb, yb;
            if (game.playerIndex)
            {
                xb = 7 - x;
                yb = 7 - y;
            }
            else
            {
                xb = x;
                yb = y;
            }

            uint8_t piece = mcumax_get_piece(square);

            if ((square == move.from) ||
                (square == move.to))
                piece |= 0x10;

            game.board[yb][xb] = piece;
        }
    }

    requestViewUpdate();
}

static void onGameCallback(void *userdata)
{
#if !__EMSCRIPTEN__
    dispatchEvents();
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

    for (uint32_t i = 0; i < GAME_HISTORY_MOVE_NUM; i++)
        game.history[i] = MCUMAX_MOVE_INVALID;

    if (playerIndex == 0)
    {
        game.state = GAME_SELECTING_FROM;

        updateValidMoves();
    }
    else
        game.state = GAME_SEARCHING;

    updateGameBoard();
}

void dispatchGameEvents(void)
{
    if (game.state == GAME_SEARCHING)
    {
        mcumax_set_callback(onGameCallback, NULL);
        mcumax_move move = mcumax_search_best_move(
            gameStrengthToNodesCount[settings.gameStrength],
            GAME_DEPTH_MAX);
        mcumax_set_callback(NULL, NULL);

        if (game.state == GAME_SEARCH_STOPPED)
            setView(&gameMenuView);
        else
        {
            if (move.from == MCUMAX_SQUARE_INVALID)
                game.state = GAME_OVER;
            else
            {
                mcumax_play_move(move);

                recordGameMove(move);

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

static void formatGameMove(char *buffer, mcumax_move move)
{
    if (move.from == MCUMAX_SQUARE_INVALID)
        strclr(buffer);
    else
    {
        buffer[0] = 'a' + (move.from & 0x7);
        buffer[1] = '1' + 7 - ((move.from & 0x70) >> 4);
        buffer[2] = '-';
        buffer[3] = 'a' + (move.to & 0x7);
        buffer[4] = '1' + 7 - ((move.to & 0x70) >> 4);
        buffer[5] = '\0';
    }
}

static void onGameViewEvent(const View *view, Event event)
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
            setView(&gameMenuView);

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

            recordGameMove(move);

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
        strcatTime(time[0], game.playerTime[!game.playerIndex]);

        strclr(time[1]);
        strcatTime(time[1], game.playerTime[game.playerIndex]);

        // Moves
        char history[GAME_HISTORY_TURN_NUM][2][6];

        for (uint32_t y = 0; y < GAME_HISTORY_TURN_NUM; y++)
            for (uint32_t x = 0; x < 2; x++)
                formatGameMove(history[y][x], game.history[2 * y + x]);

        drawGame(game.board, time, history);

        break;
    }

    case EVENT_PERIOD:
        if (game.state != GAME_OVER)
        {
            uint32_t side = (game.moveIndex & 0x1);
            game.playerTime[side] = game.playerTime[side] + 1;
        }

        break;

    default:
        break;
    }
}

const View gameView = {
    onGameViewEvent,
    NULL,
};

// Game strength menu

static const char *onGameStrengthMenuGetOption(const Menu *menu,
                                               uint32_t index,
                                               MenuStyle *menuStyle)
{
    *menuStyle = (index == settings.gameStrength);

    if (index < GAMESTRENGTH_NUM)
    {
        strcpy(menuOption, getString(STRING_GAME_LEVEL));
        strcatChar(menuOption, ' ');
        strcatUInt32(menuOption, index + 1, 0);

        return menuOption;
    }
    else
        return NULL;
}

static void onGameStrengthMenuSelect(const Menu *menu)
{
    settings.gameStrength = menu->state->selectedIndex;
}

static void onGameStrengthMenuBack(const Menu *menu)
{
    setView(&gameMenuView);
}

static MenuState gameStrengthMenuState;

static const Menu gameStrengthMenu = {
    getString(STRING_STRENGTH),
    &gameStrengthMenuState,
    onGameStrengthMenuGetOption,
    onGameStrengthMenuSelect,
    onGameStrengthMenuBack,
};

static const View gameStrengthMenuView = {
    onMenuEvent,
    &gameStrengthMenu,
};

// Game menu

static const View gameStrengthMenuView;

static const char *const gameStartMenuOptions[] = {
    getString(STRING_PLAY_WHITE),
    getString(STRING_PLAY_BLACK),
    getString(STRING_STRENGTH),
    NULL,
};

static const char *const gameContinueMenuOptions[] = {
    getString(STRING_CONTINUE_GAME),
    getString(STRING_NEW_GAME),
    getString(STRING_STRENGTH),
    NULL,
};

static const char *onGameMenuGetOption(const Menu *menu,
                                       uint32_t index,
                                       MenuStyle *menuStyle)
{
    *menuStyle = MENUSTYLE_SUBMENU;

    if (!game.moveIndex)
        return gameStartMenuOptions[index];
    else
        return gameContinueMenuOptions[index];
}

static void onGameMenuSelect(const Menu *menu)
{
    switch (menu->state->selectedIndex)
    {
    case 0:
        if (game.moveIndex)
        {
            if (game.state == GAME_SEARCH_STOPPED)
                game.state = GAME_SEARCHING;
        }
        else
            startGame(0);

        setView(&gameView);

        break;

    case 1:
        selectMenuItem(&gameMenu,
                       0,
                       0);

        if (!game.moveIndex)
        {
            startGame(1);

            setView(&gameView);
        }
        else
        {
            startGame(0);

            setView(&gameMenuView);
        }

        break;

    case 2:
        setView(&gameStrengthMenuView);

        break;
    }
}

static MenuState gameMenuState;

static const Menu gameMenu = {
    getString(STRING_GAME),
    &gameMenuState,
    onGameMenuGetOption,
    onGameMenuSelect,
    onSettingsSubMenuBack,
};

const View gameMenuView = {
    onMenuEvent,
    &gameMenu,
};
