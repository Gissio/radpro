/*
 * Rad Pro
 * Nuclear chess
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 */

#include <string.h>

#include <mcu-max.h>

#include "display.h"
#include "format.h"
#include "game.h"
#include "menu.h"
#include "settings.h"

#define GAME_HISTORY_SIZE 128
#define GAME_VALID_MOVES_NUM_MAX 128

enum GameState
{
    GAME_SELECT_FIRST_MOVE,
    GAME_SELECT_SECOND_MOVE,
    GAME_PLAY_MOVE,
    GAME_UNDO_MOVE,
};

static struct
{
    uint8_t playerIndex;

    enum GameState state;
    char board[8][9];
    uint32_t time[2];
    mcumax_move move;

    int32_t moveIndex;

    mcumax_move history[GAME_HISTORY_SIZE];

    int32_t validMovesNum;
    int32_t validMovesIndex;
    mcumax_move validMoves[GAME_VALID_MOVES_NUM_MAX];

    bool isButtonSelected;
} game;

static const char *const gamePieceMap = "@AACFBDEHIIKNJLM";

static const uint32_t gameSkillToNodesCount[] = {
    250,
    500,
    1000,
    2000,
    4000,
    8000,
    16000,
    32000,
};

static const char *const gameSkillOptions[] = {
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

static const struct Menu gameSkillMenu;
static const struct Menu gameMenu;
static const struct View gameView;

void initGame(void)
{
    selectMenuIndex(&gameSkillMenu, settings.gameSkillLevel);
}

static bool isGameStart(void)
{
    return ((game.moveIndex == 0) ||
            ((game.state == GAME_SELECT_FIRST_MOVE) &&
             (game.validMovesNum == 0)));
}

static bool isGameTimerRunning(void)
{
    return (getView() == &gameView) &&
           !((game.state == GAME_SELECT_FIRST_MOVE) &&
             (game.validMovesNum == 0));
}

static uint8_t getGamePlayerIndex(void)
{
    return (game.moveIndex & 0x1);
}

static bool isGamePlayerMove(void)
{
    return (getGamePlayerIndex() == game.playerIndex);
}

static bool isGameUndoPossible(void)
{
    return isGamePlayerMove() &&
           (game.moveIndex > 1) &&
           (game.moveIndex < GAME_HISTORY_SIZE);
}

static void recordGameMove(mcumax_move move)
{
    if (game.moveIndex >= GAME_VALID_MOVES_NUM_MAX)
        return;

    game.history[game.moveIndex++] = move;
}

static void undoGameMove(void)
{
    game.moveIndex -= 2;
    if (game.moveIndex < 0)
        game.moveIndex = 0;

    mcumax_reset();

    for (int32_t i = 0; i < game.moveIndex; i++)
        mcumax_play_move(game.history[i]);
}

static void updateGameBoard(void)
{
    for (uint32_t y = 0; y < 8; y++)
    {
        for (uint32_t x = 0; x < 8; x++)
        {
            mcumax_square square = ((0x10 * y) + x);

            uint8_t alt = (((x + y) & 0x1) << 4);
            if ((square == game.move.from) || (square == game.move.to))
                alt = (1 << 5);

            uint8_t piece = mcumax_get_piece(square) & 0xf;

            if (game.playerIndex)
                game.board[7 - y][7 - x] = alt + gamePieceMap[piece];
            else
                game.board[y][x] = alt + gamePieceMap[piece];
        }

        game.board[y][8] = '\0';
    }
}

static void onGameCallback(void *userdata)
{
    updateView();
}

static void updateValidMoves(void)
{
    game.validMovesNum = mcumax_get_valid_moves(game.validMoves, GAME_VALID_MOVES_NUM_MAX);
    game.validMovesIndex = game.playerIndex ? (game.validMovesNum - 1) : 0;

    if (game.moveIndex == 0)
        game.move = (mcumax_move){game.validMoves[game.validMovesIndex].from, MCUMAX_INVALID};

    game.isButtonSelected = (game.validMovesNum == 0);

    game.state = GAME_SELECT_FIRST_MOVE;
}

static void resetGame(uint32_t playerIndex)
{
    mcumax_reset();
    mcumax_set_callback(onGameCallback, NULL);

    game.playerIndex = playerIndex;
    game.moveIndex = 0;
    game.validMovesIndex = 0;

    game.time[0] = 0;
    game.time[1] = 0;
    game.move = (mcumax_move){MCUMAX_INVALID, MCUMAX_INVALID};
    game.isButtonSelected = (playerIndex == 1);

    if (playerIndex == 0)
    {
        game.state = GAME_SELECT_FIRST_MOVE;
        updateValidMoves();
    }
    else
        game.state = GAME_PLAY_MOVE;

    updateGameBoard();
}

void updateGame(void)
{
    switch (game.state)
    {
    case GAME_PLAY_MOVE:
        if (game.move.from != MCUMAX_INVALID)
        {
            mcumax_play_move(game.move);

            recordGameMove(game.move);

            game.move = (mcumax_move){MCUMAX_INVALID, MCUMAX_INVALID};

            updateGameBoard();
            refreshView();
        }

        if (!isGamePlayerMove())
        {
            if (!mcumax_play_best_move(gameSkillToNodesCount[settings.gameSkillLevel], &game.move))
            {
                game.state = GAME_SELECT_FIRST_MOVE;

                return;
            }

            recordGameMove(game.move);
        }

        updateValidMoves();
        updateGameBoard();
        refreshView();

        break;

    case GAME_UNDO_MOVE:
        undoGameMove();

        updateValidMoves();
        updateGameBoard();
        refreshView();

        break;

    default:
        break;
    }
}

void updateGameTimer(void)
{
    if (!isGameTimerRunning())
        return;

    uint8_t side = getGamePlayerIndex();

    uint32_t time = game.time[side] + 1;
    if (time < 3600)
        game.time[side] = time;
}

static void formatGameMove(mcumax_move move, char *buffer)
{
    buffer[0] = 'a' + (move.from & 0x7);
    buffer[1] = '1' + 7 - ((move.from & 0x70) >> 4);
    buffer[2] = '-';
    buffer[3] = 'a' + (move.to & 0x7);
    buffer[4] = '1' + 7 - ((move.to & 0x70) >> 4);
    buffer[5] = '\0';
}

static void traverseGameMoveTo(int32_t direction)
{
    mcumax_square from = game.validMoves[game.validMovesIndex].from;

    while (true)
    {
        game.validMovesIndex += direction;

        if ((game.validMovesIndex < 0) || (game.validMovesIndex >= game.validMovesNum))
            break;

        if (game.validMoves[game.validMovesIndex].from != from)
            break;
    }

    game.validMovesIndex -= direction;
}

static void selectGameMoveFrom(int32_t direction)
{
    if (!game.validMovesNum)
        return;

    if (game.playerIndex)
        direction = -direction;

    if (game.isButtonSelected)
    {
        game.isButtonSelected = false;

        if (direction == 1)
            game.validMovesIndex = 0;
        else
            game.validMovesIndex = game.validMovesNum - 1;
    }
    else
    {
        traverseGameMoveTo(direction);

        game.validMovesIndex += direction;

        if (game.validMovesIndex >= game.validMovesNum)
        {
            game.validMovesIndex = 0;

            if (isGameUndoPossible())
                game.isButtonSelected = true;
        }
        else if (game.validMovesIndex < 0)
        {
            game.validMovesIndex = game.validMovesNum - 1;

            if (isGameUndoPossible())
                game.isButtonSelected = true;
        }
    }

    traverseGameMoveTo(game.playerIndex ? 1 : -1);

    if (game.isButtonSelected)
        game.move.from = MCUMAX_INVALID;
    else
        game.move.from = game.validMoves[game.validMovesIndex].from;
}

static void selectGameMoveTo(int32_t direction)
{
    if (game.playerIndex)
        direction = -direction;

    if (game.isButtonSelected)
    {
        game.isButtonSelected = false;

        traverseGameMoveTo(-direction);
    }
    else
    {
        mcumax_square moveFrom = game.validMoves[game.validMovesIndex].from;

        int32_t index = game.validMovesIndex + direction;
        if ((index < 0) ||
            (index >= (int32_t)game.validMovesNum) ||
            (game.validMoves[index].from != moveFrom))
        {
            traverseGameMoveTo(-direction);

            game.isButtonSelected = true;
        }
        else
            game.validMovesIndex = index;
    }

    game.move.from = game.validMoves[game.validMovesIndex].from;
    if (game.isButtonSelected)
        game.move.to = MCUMAX_INVALID;
    else
        game.move.to = game.validMoves[game.validMovesIndex].to;
}

static void setGameMoveFrom(void)
{
    if (game.validMovesNum)
    {
        if (game.playerIndex)
            game.move.from = game.validMoves[game.validMovesNum - 1].from;
        else
            game.move.from = game.validMoves[0].from;
    }
    else
        game.move.from = MCUMAX_INVALID;
    game.move.to = MCUMAX_INVALID;
}

// Game view

static void onGameViewDraw(const struct View *view)
{
    // Time
    char time[2][6];
    time[0][0] = '\0';
    strcatTime(time[0], game.time[!game.playerIndex]);
    time[1][0] = '\0';
    strcatTime(time[1], game.time[game.playerIndex]);

    // Moves
    int32_t start = ((game.moveIndex + 1) & ~0x1) - 2 * GAME_MOVES_LINE_NUM;
    if (start < 0)
        start = 0;

    char moves[GAME_MOVES_LINE_NUM][2][6];

    for (uint32_t y = 0; y < GAME_MOVES_LINE_NUM; y++)
    {
        for (uint32_t x = 0; x < 2; x++)
        {
            int32_t index = start + 2 * y + x;

            if ((index < game.moveIndex) &&
                (index < GAME_HISTORY_SIZE) &&
                game.history[index].from != MCUMAX_INVALID)
                formatGameMove(game.history[index], moves[y][x]);
            else
                moves[y][x][0] = '\0';
        }
    }

    char *buttonText;
    if (isGameUndoPossible() ||
        (game.state == GAME_SELECT_SECOND_MOVE))
        buttonText = "Undo";
    else
        buttonText = NULL;

    bool buttonSelected = game.isButtonSelected;

    drawGameBoard(game.board, time, moves, buttonText, buttonSelected);
}

static void onGameViewKey(const struct View *view, KeyEvent keyEvent)
{
    switch (keyEvent)
    {
    case KEY_EVENT_UP:
        if (game.state == GAME_SELECT_FIRST_MOVE)
        {
            if (game.move.to != MCUMAX_INVALID)
                setGameMoveFrom();
            else
                selectGameMoveFrom(-1);

            updateGameBoard();
            refreshView();
        }
        else if (game.state == GAME_SELECT_SECOND_MOVE)
        {
            selectGameMoveTo(-1);

            updateGameBoard();
            refreshView();
        }

        break;

    case KEY_EVENT_DOWN:
        if (game.state == GAME_SELECT_FIRST_MOVE)
        {
            if (game.move.to != MCUMAX_INVALID)
                setGameMoveFrom();
            else
                selectGameMoveFrom(1);

            updateGameBoard();
            refreshView();
        }
        else if (game.state == GAME_SELECT_SECOND_MOVE)
        {
            selectGameMoveTo(1);

            updateGameBoard();
            refreshView();
        }

        break;

    case KEY_EVENT_ENTER:
        if (isGamePlayerMove())
        {
            if (game.isButtonSelected)
            {
                if (game.state == GAME_SELECT_FIRST_MOVE)
                    game.state = GAME_UNDO_MOVE;
                else if (game.state == GAME_SELECT_SECOND_MOVE)
                {
                    game.state = GAME_SELECT_FIRST_MOVE;
                    game.move.to = MCUMAX_INVALID;
                    game.isButtonSelected = false;
                }
            }
            else if (game.state == GAME_SELECT_FIRST_MOVE)
            {
                if (game.move.to != MCUMAX_INVALID)
                    setGameMoveFrom();
                else
                {
                    game.state = GAME_SELECT_SECOND_MOVE;
                    game.move.to = game.validMoves[game.validMovesIndex].to;
                }

                updateGameBoard();
                refreshView();
            }
            else if (game.state == GAME_SELECT_SECOND_MOVE)
            {
                game.state = GAME_PLAY_MOVE;
                game.isButtonSelected = true;
            }
        }

        break;

    case KEY_EVENT_BACK:
        mcumax_stop_search();

        setView(&gameMenuView);
    }
}

static const struct View gameView = {
    onGameViewDraw,
    onGameViewKey,
    NULL,
};

// Game skill menu

static void onGameSkillMenuSelect(const struct Menu *menu)
{
    settings.gameSkillLevel = menu->state->selectedIndex;
}

static void onGameSkillMenuBack(const struct Menu *menu)
{
    setView(&gameMenuView);
}

static struct MenuState gameSkillMenuState;

static const struct Menu gameSkillMenu = {
    "Skill level",
    &gameSkillMenuState,
    onMenuGetOption,
    gameSkillOptions,
    onGameSkillMenuSelect,
    NULL,
    onGameSkillMenuBack,
};

static const struct View gameSkillMenuView = {
    onMenuViewDraw,
    onMenuViewKey,
    &gameSkillMenu,
};

// Game menu

static void onGameMenuEnter(const struct Menu *menu)
{
    switch (menu->state->selectedIndex)
    {
    case 0:
        if (isGameStart())
            resetGame(0);

        setView(&gameView);

        break;

    case 1:
        selectMenuIndex(&gameMenu, 0);

        if (isGameStart())
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
        setView(&gameSkillMenuView);

        break;
    }
}

static const char *const gameStartMenuOptions[] = {
    "Play white",
    "Play black",
    "Skill level",
    NULL,
};

static const char *const gameContinueMenuOptions[] = {
    "Continue game",
    "New game",
    "Skill level",
    NULL,
};

static const char *onGameMenuGetOption(const struct Menu *menu, uint32_t index)
{
    if (isGameStart())
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
    onMenuViewDraw,
    onMenuViewKey,
    &gameMenu,
};
