/*
 * mcu-max
 * Chess game engine for low-resource MCUs
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 *
 * Based on micro-Max 4.8 by H.G. Muller.
 * Compliant with FIDE laws (except for underpromotion and draw rules).
 */

#if !defined(MCU_MAX_H)
#define MCU_MAX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define MCUMAX_ID "mcu-max 2.0.0"
#define MCUMAX_AUTHOR "Gissio"

/**
 * Squares
 *
 * A square is coded as 0xRF (0x88 layout), with R the row (0 = rank 8,
 * 7 = rank 1) and F the file (0 = file a, 7 = file h). E.g. e2 = 0x64.
 * Use the macros below instead of relying on the coding.
 */
typedef uint8_t mcumax_square;

#define MCUMAX_SQUARE_INVALID 0x80

/** Makes a square from file (0-7: a-h) and rank (0-7: 1-8). */
#define MCUMAX_SQUARE(file, rank) \
    ((mcumax_square)(((7 - (rank)) << 4) | (file)))
/** Returns the file (0-7: a-h) of a square. */
#define MCUMAX_SQUARE_FILE(square) ((square) & 0x7)
/** Returns the rank (0-7: 1-8) of a square. */
#define MCUMAX_SQUARE_RANK(square) (7 - (((square) >> 4) & 0x7))

/**
 * Moves
 *
 * Pawn moves to the last rank always promote to a queen.
 */
typedef struct
{
    mcumax_square from;
    mcumax_square to;
} mcumax_move;

#ifdef __cplusplus
#define MCUMAX_MOVE_INVALID \
    (mcumax_move{MCUMAX_SQUARE_INVALID, MCUMAX_SQUARE_INVALID})
#else
#define MCUMAX_MOVE_INVALID \
    ((mcumax_move){MCUMAX_SQUARE_INVALID, MCUMAX_SQUARE_INVALID})
#endif

/**
 * Pieces: piece type | color
 */
typedef uint8_t mcumax_piece;

enum
{
    // Bits 0-2: piece type
    MCUMAX_EMPTY,
    MCUMAX_PAWN,
    MCUMAX_KNIGHT,
    MCUMAX_BISHOP,
    MCUMAX_ROOK,
    MCUMAX_QUEEN,
    MCUMAX_KING,

    // Bit 3: color
    MCUMAX_WHITE = 0x0,
    MCUMAX_BLACK = 0x8,
};

#define MCUMAX_PIECE_TYPE(piece) ((piece) & 0x7)
#define MCUMAX_PIECE_COLOR(piece) ((piece) & MCUMAX_BLACK)

/**
 * Game states: bit 0 = in check, bit 1 = no legal moves
 */
typedef enum
{
    MCUMAX_GAME_ONGOING,
    MCUMAX_GAME_CHECK,
    MCUMAX_GAME_STALEMATE,
    MCUMAX_GAME_CHECKMATE,
} mcumax_game_state;

typedef void (*mcumax_callback)(void *userdata);

/**
 * @brief Resets the engine to the standard starting position.
 */
void mcumax_init(void);

/**
 * FEN support (for testing)
 */

/** Buffer size for mcumax_get_fen_position(), including the terminator. */
#define MCUMAX_FEN_SIZE 90

/**
 * @brief Sets the position from a FEN string.
 *
 * The halfmove clock and fullmove number fields are ignored.
 *
 * @param fen_string The FEN string.
 */
void mcumax_set_fen_position(const char *fen_string);

/**
 * @brief Gets the position as a FEN string.
 *
 * The halfmove clock and fullmove number are not tracked, and are
 * reported as "0 1".
 *
 * @param fen_string A buffer of at least MCUMAX_FEN_SIZE characters.
 */
void mcumax_get_fen_position(char *fen_string);

/**
 * @brief Returns the piece at a square.
 *
 * @param square The square.
 * @return The piece (type | color), or MCUMAX_EMPTY.
 */
mcumax_piece mcumax_get_piece(mcumax_square square);

/**
 * @brief Returns the side to move.
 *
 * @return MCUMAX_WHITE or MCUMAX_BLACK.
 */
mcumax_piece mcumax_get_current_side(void);

/**
 * @brief Lists the legal moves of the side to move.
 *
 * @param buffer A buffer for storing the moves (may be NULL if
 *        buffer_size is 0).
 * @param buffer_size The number of moves the buffer can hold.
 *
 * @return The number of legal moves. May exceed buffer_size, in which case
 *         only the first buffer_size moves are stored.
 */
uint32_t mcumax_search_valid_moves(mcumax_move *buffer, uint32_t buffer_size);

/**
 * @brief Returns the game state of the side to move.
 *
 * Costs about as much as mcumax_search_valid_moves(). Draws by repetition,
 * the 50-move rule or insufficient material are not detected.
 *
 * @return The game state.
 */
mcumax_game_state mcumax_get_game_state(void);

/**
 * @brief Searches the best move for the side to move.
 *
 * The position is not changed; use mcumax_play_move() to play the move.
 * The search ends when either limit is reached, or when stopped with
 * mcumax_stop_search().
 *
 * @param node_max The maximum number of nodes to search (strength).
 * @param depth_max The maximum search depth in plies. Must be at most 90
 *        (larger values break the search; not checked). Recursion
 *        is limited to depth_max + 2 levels, which bounds stack usage to
 *        about (depth_max + 2) * 144 bytes on Cortex-M (176 with hashing).
 *
 * @return The best move, or MCUMAX_MOVE_INVALID if there is no legal move
 *         or the search was stopped.
 */
mcumax_move mcumax_search_best_move(uint32_t node_max, uint32_t depth_max);

/**
 * @brief Plays a move, if legal.
 *
 * Legality is verified with a shallow search.
 *
 * @param move The move.
 * @return true if the move was legal and played.
 */
bool mcumax_play_move(mcumax_move move);

/**
 * @brief Sets a callback, called at every search node.
 *
 * Use it to feed watchdogs, process events or stop the search.
 *
 * @param callback The callback, or NULL to disable it.
 * @param userdata The value passed to the callback.
 */
void mcumax_set_callback(mcumax_callback callback, void *userdata);

/**
 * @brief Stops the current search. To be called from the callback.
 *
 * A stopped mcumax_search_best_move() returns MCUMAX_MOVE_INVALID.
 */
void mcumax_stop_search(void);

#ifdef __cplusplus
}
#endif

#endif
