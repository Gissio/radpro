/*
 * Rad Pro
 * Nuclear chess
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

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

#define GAME_HISTORY_SIZE (GAME_MOVES_LINE_NUM * 2)
#define GAME_VALID_MOVES_NUM_MAX 181

enum GameState
{
    GAME_SHOWING_LAST_MOVE,
    GAME_SELECTING_FROM,
    GAME_SELECTING_TO,
    GAME_SEARCHING,
    GAME_CANCELLING_SEARCH,
    GAME_OVER,
};

static struct
{
    enum GameState state;
    uint32_t moveIndex;
    uint32_t playerIndex;
    uint32_t playerTime[2];
    uint8_t board[8][8];

    int32_t validMovesNum;
    int32_t validMovesIndex;
    mcumax_move validMoves[GAME_VALID_MOVES_NUM_MAX];

    mcumax_move history[GAME_HISTORY_SIZE];

    bool isUndoSelected;
} game;

static const char *const gamePieceMap = "@AACFBDE";

static const uint32_t gameStrengthToNodesCount[] = {
    1,
    256,
    512,
    1024,
    2048,
    4096,
    8192,
    16384,
};

static const char *const gameStrengthOptions[] = {
    "Level 1",
    "Level 2",
    "Level 3",
    "Level 4",
    "Level 5",
    "Level 6",
    "Level 7",
    "Level 8",
    NULL,
};

static const struct Menu gameStrengthMenu;
static const struct Menu gameMenu;
static const struct View gameView;

static void onGameCallback(void *userdata);

void initGame(void)
{
    selectMenuIndex(&gameStrengthMenu, settings.gameStrength, GAME_STRENGTH_NUM);
}

static void recordGameMove(mcumax_move move)
{
    if (game.moveIndex < GAME_HISTORY_SIZE)
        game.history[game.moveIndex] = move;
    else
    {
        uint32_t index = game.moveIndex & 0x1;
        if (index == 0)
        {
            for (uint32_t i = 2; i < GAME_HISTORY_SIZE; i++)
                game.history[i - 2] = game.history[i];

            game.history[GAME_HISTORY_SIZE - 1] = MCUMAX_MOVE_INVALID;
        }

        game.history[GAME_HISTORY_SIZE - 2 + index] = move;
    }

    game.moveIndex++;
}

static void updateGameBoard(void)
{
    mcumax_move move = MCUMAX_MOVE_INVALID;

    switch (game.state)
    {
    case GAME_SHOWING_LAST_MOVE:
        for (int i = 0; i < GAME_HISTORY_SIZE; i++)
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

        if (game.isUndoSelected)
            move.to = MCUMAX_SQUARE_INVALID;

        break;

    default:
        break;
    }

    for (uint32_t y = 0; y < 8; y++)
    {
        for (uint32_t x = 0; x < 8; x++)
        {
            mcumax_square square = ((0x10 * y) + x);

            uint8_t piece = mcumax_get_piece(square);
            uint8_t pieceType = piece & 0x7;
            uint8_t pieceColor = !(piece >> 3);
            uint8_t squareColor = ((x + y) & 0x1);

            uint8_t modifiers;
            if ((square == move.from) ||
                (square == move.to))
                modifiers = (pieceColor << 7) | 0x10;
            else
                modifiers = (squareColor << 7) | ((pieceColor ^ squareColor) << 3);

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

            game.board[yb][xb] = modifiers + gamePieceMap[pieceType];
        }
    }

    refreshView();
}

static void onGameCallback(void *userdata)
{
    updateEvents();
}

static void updateValidMoves(void)
{
    game.validMovesNum = mcumax_search_valid_moves(game.validMoves, GAME_VALID_MOVES_NUM_MAX);
    game.validMovesIndex = game.playerIndex ? (game.validMovesNum - 1) : 0;
}

static void resetGame(uint32_t playerIndex)
{
    mcumax_init();

    game.moveIndex = 0;
    game.playerIndex = playerIndex;
    game.playerTime[0] = 0;
    game.playerTime[1] = 0;

    for (uint32_t i = 0; i < GAME_HISTORY_SIZE; i++)
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

void updateGame(void)
{
    if (game.state == GAME_SEARCHING)
    {
        mcumax_set_callback(onGameCallback, NULL);
        mcumax_move move = mcumax_search_best_move(
            gameStrengthToNodesCount[settings.gameStrength],
            GAME_DEPTH_MAX);
        mcumax_set_callback(NULL, NULL);

        if (game.state == GAME_CANCELLING_SEARCH)
        {
            game.state = GAME_SEARCHING;

            setView(&gameMenuView);
        }
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

void updateGameTimer(void)
{
    if ((getView() == &gameView) &&
        (game.state != GAME_OVER))
    {
        uint32_t side = (game.moveIndex & 0x1);

        uint32_t time = game.playerTime[side] + 1;
        if (time < 10 * 3600)
            game.playerTime[side] = time;
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
    if (game.isUndoSelected)
    {
        stepValidMoveFrom(-direction);
        stepValidMove(direction);

        game.isUndoSelected = false;
    }
    else
    {
        mcumax_square moveFrom = game.validMoves[game.validMovesIndex].from;

        if (stepValidMove(direction) ||
            (moveFrom != game.validMoves[game.validMovesIndex].from))
        {
            stepValidMove(-direction);

            game.isUndoSelected = true;
        }
    }

    updateGameBoard();
}

// Game view

static void formatGameMove(char *buffer, mcumax_move move)
{
    if (move.from == MCUMAX_SQUARE_INVALID)
        strcpy(buffer, "");
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

static void onGameViewEvent(const struct View *view, enum Event event)
{
    switch (event)
    {
    case EVENT_UP:
    case EVENT_DOWN:
        if (game.state == GAME_SHOWING_LAST_MOVE)
        {
            game.state = GAME_SELECTING_FROM;

            updateGameBoard();
        }
        else if (game.state == GAME_SELECTING_FROM)
            selectNextMoveFrom((event == EVENT_UP) ? -1 : 1);

        else if (game.state == GAME_SELECTING_TO)
            selectNextMoveTo((event == EVENT_UP) ? -1 : 1);

        break;

    case EVENT_ENTER:
        switch (game.state)
        {
        case GAME_SELECTING_FROM:
            if (game.validMovesNum)
            {
                game.state = GAME_SELECTING_TO;

                stepValidMoveFrom(-1);
                stepValidMove(1);

                game.isUndoSelected = false;

                updateGameBoard();
            }

            break;

        case GAME_SELECTING_TO:
            if (game.isUndoSelected)
                game.state = GAME_SELECTING_FROM;
            else
            {
                mcumax_move move = game.validMoves[game.validMovesIndex];

                mcumax_play_move(move);

                recordGameMove(move);

                game.state = GAME_SEARCHING;
            }

            updateGameBoard();

            break;

        default:
            break;
        }

        break;

    case EVENT_BACK:
        if (game.state == GAME_SEARCHING)
        {
            mcumax_stop_search();

            game.state = GAME_CANCELLING_SEARCH;
        }
        else if (game.state != GAME_CANCELLING_SEARCH)
            setView(&gameMenuView);

        break;

    case EVENT_DRAW:
    {
        // Time
        char time[2][8];

        strcpy(time[0], "");
        strcatTime(time[0], game.playerTime[!game.playerIndex]);

        strcpy(time[1], "");
        strcatTime(time[1], game.playerTime[game.playerIndex]);

        // Moves
        char moves[GAME_MOVES_LINE_NUM][2][6];

        for (uint32_t y = 0; y < GAME_MOVES_LINE_NUM; y++)
            for (uint32_t x = 0; x < 2; x++)
                formatGameMove(moves[y][x], game.history[2 * y + x]);

        const char *buttonText;
        bool isButtonSelected;

        if (game.state == GAME_SELECTING_TO)
        {
            buttonText = "Undo";
            isButtonSelected = game.isUndoSelected;
        }
        else
        {
            buttonText = NULL;
            isButtonSelected = false;
        }

        drawGameBoard(game.board, time, moves, buttonText, isButtonSelected);

        break;
    }

    default:
        break;
    }
}

static const struct View gameView = {
    onGameViewEvent,
    NULL,
};

// Game strength menu

static void onGameStrengthMenuSelect(const struct Menu *menu)
{
    settings.gameStrength = menu->state->selectedIndex;
}

static void onGameStrengthMenuBack(const struct Menu *menu)
{
    setView(&gameMenuView);
}

static struct MenuState gameStrengthMenuState;

static const struct Menu gameStrengthMenu = {
    "Strength",
    &gameStrengthMenuState,
    onMenuGetOption,
    gameStrengthOptions,
    onGameStrengthMenuSelect,
    NULL,
    onGameStrengthMenuBack,
};

static const struct View gameStrengthMenuView = {
    onMenuEvent,
    &gameStrengthMenu,
};

// Game menu

static void onGameMenuEnter(const struct Menu *menu)
{
    switch (menu->state->selectedIndex)
    {
    case 0:
        if (!game.moveIndex)
            resetGame(0);

        setView(&gameView);

        break;

    case 1:
        selectMenuIndex(&gameMenu, 0, GAME_MENU_OPTIONS_NUM);

        if (!game.moveIndex)
        {
            resetGame(1);

            setView(&gameView);
        }
        else
        {
            resetGame(0);

            setView(&gameMenuView);
        }

        break;

    case 2:
        setView(&gameStrengthMenuView);

        break;
    }
}

static const char *const gameStartMenuOptions[] = {
    "Play white",
    "Play black",
    "Strength",
    NULL,
};

static const char *const gameContinueMenuOptions[] = {
    "Continue game",
    "New game",
    "Strength",
    NULL,
};

static const char *onGameMenuGetOption(const struct Menu *menu, uint32_t index)
{
    if (!game.moveIndex)
        return gameStartMenuOptions[index];
    else
        return gameContinueMenuOptions[index];
}

static struct MenuState gameMenuState;

static const struct Menu gameMenu = {
    "Game",
    &gameMenuState,
    onGameMenuGetOption,
    NULL,
    NULL,
    onGameMenuEnter,
    onSettingsSubMenuBack,
};

const struct View gameMenuView = {
    onMenuEvent,
    &gameMenu,
};
