/*
 * mcu-max 0.9
 * Chess engine for low-resource MCUs
 *
 * (C) 2022 Gissio
 *
 * License: MIT
 *
 * Based on micro-Max 4.8 by H.G. Muller.
 * Compliant with FIDE laws (except for underpromotion).
 * Optimized for speed and clarity.
 */

#ifndef MCU_MAX_H
#define MCU_MAX_H

#include <stdbool.h>

#define MCUMAX_NAME "mcu-max 1.0"
#define MCUMAX_AUTHOR "Gissio"

// Invalid position
#define MCUMAX_INVALID 0x80

typedef unsigned char mcumax_square;
typedef unsigned char mcumax_piece;

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
    // For underpromotion:
    // MCUMAX_KING,
    // MCUMAX_KNIGHT,
    MCUMAX_KNIGHT,
    MCUMAX_KING,
    MCUMAX_BISHOP,
    MCUMAX_ROOK,
    MCUMAX_QUEEN,

    // Bits 3-4: 00-empty, 01-white, 10-black
    MCUMAX_WHITE = 0x8,
    MCUMAX_BLACK = 0x10,
};

/**
 * @brief Resets game state.
 */
void mcumax_reset();

/**
 * @brief Gets piece at specified square.
 *
 * @param square A square coded as 0xRF, R: rank (0-7), F: file (0-7).
 * @return The piece.
 */
mcumax_piece mcumax_get_piece(mcumax_square square);

/**
 * @brief Sets position from a FEN string.
 *
 * @param value The FEN string.
 */
void mcumax_set_fen_position(const char *value);

/**
 * @brief Gets current side.
 */
mcumax_piece mcumax_get_current_side();

/**
 * @brief Sets callback: called periodically during search.
 */
void mcumax_set_callback(mcumax_callback callback, void *userdata);

/**
 * @brief Returns a list of valid moves.
 *
 * @param valid_moves_buffer Moves buffer.
 * @param valid_moves_buffer_size Size of moves buffer.
 * @return Number of valid moves found.
 */
int mcumax_get_valid_moves(mcumax_move *valid_moves_buffer, int valid_moves_buffer_size);

/**
 * @brief Get best move.
 *
 * @param nodes_count_max Max number of nodes to analyze.
 * @param move Best move.
 */
void mcumax_get_best_move(int nodes_count_max, mcumax_move *move);

// To be removed...
bool mcumax_play_best_move(int nodes_count_max, mcumax_move *move);

/**
 * @brief Stops best move search.
 */
void mcumax_stop_search();

/**
 * @brief Play move.
 *
 * @param move The move.
 * @return Move realized.
 */
bool mcumax_play_move(mcumax_move move);

#endif
