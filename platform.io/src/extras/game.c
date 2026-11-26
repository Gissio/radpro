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
#include "../system/power.h"
#include "../system/settings.h"
#include "../ui/game.h"
#include "../ui/menu.h"
#include "../ui/view.h"

// Search depth limit. The search uses up to depth + 2 recursion levels of
// 144 bytes of stack, on top of up to 1.6 kB (monochrome) or 2.2 kB (color)
// for the search callback (events, view drawing) and interrupts.
// STM32F0 (8 kB RAM): 3.9 kB of stack, so 12. Other devices: at least 8 kB
// of stack; 32 is never reached in practice.
#if defined(STM32F0)
#define GAME_DEPTH_MAX 12
#else
#define GAME_DEPTH_MAX 32
#endif

// Maximum number of legal moves in any chess position
#define GAME_VALID_MOVES_NUM_MAX 218

// Board square flag for drawGame(): square is highlighted
#define GAME_SQUARE_SELECTED 0x10

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
    bool searchRunning;
    uint32_t moveIndex;
    uint32_t playerIndex;
    uint32_t playerTime[2];
    uint8_t board[8][8];

    // Legal moves of the side to move, in board order (so moves from the
    // same square are contiguous)
    int32_t validMovesNum;
    int32_t validMovesIndex;
    mcumax_move validMoves[GAME_VALID_MOVES_NUM_MAX];

    mcumax_move previousMove;
} game;

static const Menu gameStartMenu;
static const Menu gameContinueMenu;
static const Menu gameStrengthMenu;

void resetGame(void)
{
    memset(&game, 0, sizeof(game));

    selectMenuItem(&gameStartMenu, 0);
    selectMenuItem(&gameContinueMenu, 0);
    selectMenuItem(&gameStrengthMenu, settings.gameStrength);
}

// Runs mcu-max: must not be called while a search is running
static void updateGameBoard(void)
{
    mcumax_move move;

    switch (game.state)
    {
    case GAME_SHOWING_LAST_MOVE:
    case GAME_OVER:
        move = game.previousMove;

        break;

    case GAME_SELECTING_FROM:
    case GAME_SELECTING_TO:
        move = game.validMoves[game.validMovesIndex];
        if (game.state == GAME_SELECTING_FROM)
            move.to = MCUMAX_SQUARE_INVALID;

        break;

    default:
        move = MCUMAX_MOVE_INVALID;

        break;
    }

    // Highlight the king of the side to move if in check
    mcumax_piece checkedKing = (mcumax_get_game_state() & MCUMAX_GAME_CHECK)
                                   ? (MCUMAX_KING | mcumax_get_current_side())
                                   : 0xff;

    // Rotate the board when playing black
    uint32_t flip = game.playerIndex ? 7 : 0;

    for (uint32_t y = 0; y < 8; y++)
    {
        for (uint32_t x = 0; x < 8; x++)
        {
            mcumax_square square = (y << 4) | x;
            uint8_t piece = mcumax_get_piece(square);

            if ((square == move.from) ||
                (square == move.to) ||
                (piece == checkedKing))
                piece |= GAME_SQUARE_SELECTED;

            // Empty squares are drawn with the black bit set
            if (!MCUMAX_PIECE_TYPE(piece))
                piece |= MCUMAX_BLACK;

            game.board[y ^ flip][x ^ flip] = piece;
        }
    }

    requestViewUpdate();
}

static void onGameCallback(void *userdata)
{
#if !defined(__EMSCRIPTEN__)
    sleep(0); // Reloads watchdog (smaller than inlined reloadWatchdog)

    updateEvents();
#endif
}

static uint32_t searchValidMoves(void)
{
    uint32_t validMovesNum = mcumax_search_valid_moves(game.validMoves, GAME_VALID_MOVES_NUM_MAX);

    // Only the first GAME_VALID_MOVES_NUM_MAX moves are stored
    if (validMovesNum > GAME_VALID_MOVES_NUM_MAX)
        validMovesNum = GAME_VALID_MOVES_NUM_MAX;

    return validMovesNum;
}

static void updateValidMoves(void)
{
    game.validMovesNum = searchValidMoves();
    game.validMovesIndex = game.playerIndex ? (game.validMovesNum - 1) : 0;
}

static void playMove(mcumax_move move)
{
    mcumax_play_move(move);

    game.moveIndex++;
    game.previousMove = move;
}

static void startGame(uint32_t playerIndex)
{
    mcumax_init();

    game.moveIndex = 0;
    game.playerIndex = playerIndex;
    game.playerTime[0] = 0;
    game.playerTime[1] = 0;
    game.previousMove = MCUMAX_MOVE_INVALID;

    if (playerIndex)
        game.state = GAME_SEARCHING;
    else
    {
        game.state = GAME_SELECTING_FROM;

        updateValidMoves();
    }

    updateGameBoard();
}

void updateGame(void)
{
    if (game.state != GAME_SEARCHING)
        return;

    uint32_t strength = settings.gameStrength;
    mcumax_move move = MCUMAX_MOVE_INVALID;

    // Levels 1-3: random move with 75%, 50%, 25% probability
    if (strength < GAME_STRENGTH_4)
    {
        uint32_t random = 0;
        for (uint32_t i = 0; i < 16; i++)
            random = (random << 1) | getRandomBit();

        if ((random & 0x3) < (GAME_STRENGTH_4 - strength))
        {
            uint32_t validMovesNum = searchValidMoves();

            if (validMovesNum)
                move = game.validMoves[(random * validMovesNum) >> 16];
        }
    }

    if (move.from == MCUMAX_SQUARE_INVALID)
    {
        // Levels 1-4: 1 node; levels 5-8: 4096, 16384, 65536, 262144 nodes
        mcumax_set_callback(onGameCallback, NULL);
        game.searchRunning = true;
        move = mcumax_search_best_move((strength < GAME_STRENGTH_5) ? 1 : (1 << (2 * strength + 4)), GAME_DEPTH_MAX);
        game.searchRunning = false;
        mcumax_set_callback(NULL, NULL);

        // Search stopped (back key or power off), or game reset (power on)
        if (game.state != GAME_SEARCHING)
        {
            if ((game.state == GAME_SEARCH_STOPPED) && isPoweredOn())
                showGameMenu();

            return;
        }
    }

    if (move.from == MCUMAX_SQUARE_INVALID)
        game.state = GAME_OVER;
    else
    {
        playMove(move);
        updateValidMoves();

        game.state = game.validMovesNum ? GAME_SHOWING_LAST_MOVE : GAME_OVER;
    }

    updateGameBoard();
}

void stopGameSearch(void)
{
    if (game.state == GAME_SEARCHING)
    {
        mcumax_stop_search();

        game.state = GAME_SEARCH_STOPPED;
    }
}

// Returns the index of the last move, going in direction from index, that
// moves from the same square as the move at index
static int32_t getMoveGroupEnd(int32_t index, int32_t direction)
{
    mcumax_square from = game.validMoves[index].from;

    while (true)
    {
        int32_t nextIndex = index + direction;

        if ((nextIndex < 0) ||
            (nextIndex >= game.validMovesNum) ||
            (game.validMoves[nextIndex].from != from))
            return index;

        index = nextIndex;
    }
}

// Selects the next square with moves, wrapping around
static void selectNextMoveFrom(int32_t direction)
{
    // Follow the rotated board when playing black
    if (game.playerIndex)
        direction = -direction;

    int32_t index = getMoveGroupEnd(game.validMovesIndex, direction) + direction;

    if (index < 0)
        index = game.validMovesNum - 1;
    else if (index >= game.validMovesNum)
        index = 0;

    game.validMovesIndex = index;
}

// Selects the next move from the selected square, wrapping around
static void selectNextMoveTo(int32_t direction)
{
    int32_t index = game.validMovesIndex;

    game.validMovesIndex = (getMoveGroupEnd(index, direction) == index)
                               ? getMoveGroupEnd(index, -direction)
                               : (index + direction);
}

// Game view

static void onGameViewEvent(ViewEvent event)
{
    switch (event)
    {
    case EVENT_KEY_BACK:
        if (game.state == GAME_SEARCHING)
        {
            bool searchRunning = game.searchRunning;

            stopGameSearch();

            // A running search shows the menu when it returns; if the
            // search has not started yet, show it here
            if (!searchRunning)
                showGameMenu();
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
        if (game.state == GAME_SELECTING_FROM)
        {
            // Start at the first move from the selected square
            game.state = GAME_SELECTING_TO;
            game.validMovesIndex = getMoveGroupEnd(game.validMovesIndex, -1);
        }
        else if (game.state == GAME_SELECTING_TO)
        {
            playMove(game.validMoves[game.validMovesIndex]);

            game.state = GAME_SEARCHING;
        }
        else
            break;

        updateGameBoard();

        break;

    case EVENT_KEY_UP:
    case EVENT_KEY_DOWN:
    {
        int32_t direction = (event == EVENT_KEY_UP) ? -1 : 1;

        if (game.state == GAME_SHOWING_LAST_MOVE)
            game.state = GAME_SELECTING_FROM;
        else if (game.state == GAME_SELECTING_FROM)
            selectNextMoveFrom(direction);
        else if (game.state == GAME_SELECTING_TO)
            selectNextMoveTo(direction);
        else
            break;

        updateGameBoard();

        break;
    }

    case EVENT_DRAW:
    {
        // Player time first, then opponent time
        char time[2][16];

        for (uint32_t i = 0; i < 2; i++)
        {
            strclr(time[i]);
            strcatTime(time[i], game.playerTime[game.playerIndex ^ i]);
        }

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
    if (index < 2)
    {
        // A search may still run in the background (the view was switched
        // by lock mode or an alert): mcumax_init() would corrupt it, and the
        // search would then corrupt the new board. Stop it; it returns at
        // once and shows the game menu again.
        if (game.searchRunning)
        {
            stopGameSearch();

            return;
        }

        // 0: play white, 1: play black
        startGame(index);

        showView(onGameViewEvent);
    }
    else
        showMenu(&gameStrengthMenu);
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
    ARRAY_SIZE(gameContinueMenuOptions),
    onGameContinueMenuGetOption,
    onGameContinueMenuSelect,
    showSettingsMenu,
};

// Game menu

void showGameMenu(void)
{
    showMenu(game.moveIndex ? &gameContinueMenu : &gameStartMenu);
}
