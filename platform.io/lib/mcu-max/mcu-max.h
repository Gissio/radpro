/*
 * mcu-max 1.0.1
 * Chess engine for low-resource MCUs
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 *
 * Based on micro-Max 4.8 by H.G. Muller.
 * Compliant with FIDE laws (except for underpromotion).
 */

#if !defined(MCU_MAX_H)
#define MCU_MAX_H

#include <stdbool.h>
#include <stdint.h>

#define MCUMAX_ID "mcu-max 1.0"
#define MCUMAX_AUTHOR "Gissio"

#define MCUMAX_SQUARE_INVALID 0x80

#define MCUMAX_MOVE_INVALID \
    (mcumax_move) { MCUMAX_SQUARE_INVALID, MCUMAX_SQUARE_INVALID }

typedef uint8_t mcumax_square;
typedef uint8_t mcumax_piece;

typedef struct
{
    mcumax_square from;
    mcumax_square to;
} mcumax_move;

typedef void (*mcumax_callback)(void *);

/**
 * Piece types
 */
enum
{
    // Bits 0-2: piece
    MCUMAX_EMPTY,
    MCUMAX_PAWN_UPSTREAM,
    MCUMAX_PAWN_DOWNSTREAM,
    MCUMAX_KNIGHT,
    MCUMAX_KING,
    MCUMAX_BISHOP,
    MCUMAX_ROOK,
    MCUMAX_QUEEN,

    // Bits 3: color
    MCUMAX_BLACK = 0x8,
};

/**
 * @brief Resets the engine state.
 */
void mcumax_init(void);

/**
 * @brief Sets position from a FEN string.
 *
 * @param value The FEN string.
 */
void mcumax_set_fen_position(const char *value);

/**
 * @brief Returns the piece at the specified square.
 *
 * @param square A square coded as 0xRF, R: rank (0-7), F: file (0-7).
 * @return The piece.
 */
mcumax_piece mcumax_get_piece(mcumax_square square);

/**
 * @brief Returns the current side.
 */
mcumax_piece mcumax_get_current_side(void);

/**
 * @brief Searches valid moves.
 *
 * @param buffer A buffer for storing valid moves.
 * @param buffer_size The buffer size for storing valid moves.
 *
 * @return The number of valid moves.
 */
uint32_t mcumax_search_valid_moves(mcumax_move *buffer, uint32_t buffer_size);

/**
 * @brief Searches the best move.
 *
 * @param node_max The maximum number of nodes to search.
 * @param depth_max The maximum depth to search.
 *
 * @return The best move (MCUMAX_SQUARE_INVALID, MCUMAX_SQUARE_INVALID if none found).
 */
mcumax_move mcumax_search_best_move(uint32_t node_max, uint32_t depth_max);

/**
 * @brief Plays a move.
 *
 * @param move The move.
 * @return The move was played.
 */
bool mcumax_play_move(mcumax_move move);

/**
 * @brief Sets the user callback, which is called periodically during search.
 */
void mcumax_set_callback(mcumax_callback callback, void *userdata);

/**
 * @brief Stops the current search. To be called from the user callback.
 */
void mcumax_stop_search(void);

#endif
