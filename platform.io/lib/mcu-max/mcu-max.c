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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcu-max.h"

/*
 * Configuration: uncomment here, or define when compiling (see README)
 */

// Hash table: avoids repetitions (needed to win won endgames) and speeds
// up search. Size in entries (a power of 2), 12 bytes of RAM each.
// #define MCUMAX_HASHING_ENABLED
// #define MCUMAX_HASH_TABLE_SIZE 1024

// Removes the pawn structure evaluation: about 100 bytes smaller, about
// 10-40 Elo weaker
// #define MCUMAX_PAWN_EVAL_DISABLED

// Types for 8-bit values (squares, pieces, steps) in the search: int on
// MCUs where wider types are faster (e.g. 32-bit ARM, avoiding byte
// extensions), uint8_t/int8_t otherwise (e.g. AVR). Both behave the same,
// as 8-bit values are promoted to int in expressions.
#if UINT_FAST8_MAX > UINT8_MAX
typedef int mcumax_byte;
typedef int mcumax_sbyte;
#else
typedef uint8_t mcumax_byte;
typedef int8_t mcumax_sbyte;
#endif

/*
 * Board representation
 *
 * The board is a 0x88 board: square = 0x10 * row + column, with row 0 being
 * rank 8 and column 0 being file a. Any square with a bit of 0x88 set is off
 * the board. The left half of each 16-byte row holds the pieces; the right
 * half (square + MCUMAX_WEIGHTS_OFFSET) holds a centralization weight used
 * for positional scoring.
 *
 * Each board byte encodes a piece:
 *   bits 0-2: piece type (MCUMAX_PAWN_UPSTREAM...MCUMAX_QUEEN_TYPE)
 *   bit 3:    white (MCUMAX_SIDE_WHITE)
 *   bit 4:    black (MCUMAX_SIDE_BLACK)
 *   bit 5:    piece has moved (no castling / pawn double step)
 *   bit 6:    pawn on the 6th/7th rank (scoring bonus)
 *   bit 7:    promoted piece (scoring bonus)
 */

// Internal piece types (public types are mapped by mcumax_get_piece)
enum
{
    MCUMAX_PAWN_UPSTREAM = 1, // White pawn
    MCUMAX_PAWN_DOWNSTREAM,   // Black pawn
    MCUMAX_KNIGHT_TYPE,
    MCUMAX_KING_TYPE,
    MCUMAX_BISHOP_TYPE,
    MCUMAX_ROOK_TYPE,
    MCUMAX_QUEEN_TYPE,
};

#define MCUMAX_OFF_BOARD 0x88
#define MCUMAX_WEIGHTS_OFFSET 0x08
#define MCUMAX_ROW_STEP 0x10

#define MCUMAX_PIECE_TYPE_MASK 0x07
#define MCUMAX_SIDE_WHITE 0x08
#define MCUMAX_SIDE_BLACK 0x10
#define MCUMAX_SIDE_SWITCH (MCUMAX_SIDE_WHITE | MCUMAX_SIDE_BLACK)
#define MCUMAX_PIECE_MOVED 0x20
#define MCUMAX_PIECE_BONUS_MASK 0xc0

// Scores
#define MCUMAX_SCORE_MAX 8000 // King capture
#define MCUMAX_SCORE_MATE (MCUMAX_SCORE_MAX - 0x88)
#define MCUMAX_PIECE_VALUE_SCALE 37
#define MCUMAX_MATERIAL_SHIFT 7 // Piece value -> game phase units

// Game phase thresholds (captured non-pawn material; 40 = all captured)
#define MCUMAX_PHASE_NULL_MOVE_OFF 35 // No null-move pruning above
#define MCUMAX_PHASE_KING_ACTIVE 30   // Kings may move freely, check extension off

// Search
#define MCUMAX_DEPTH_MAX 99
#define MCUMAX_DEPTH_MIN 3 // Iterative deepening runs at least to this depth

// Index into mcumax_steps preceding the king's first (lateral) step
#define MCUMAX_KING_LATERAL_STEP_INDEX 7

// Search modes (order matters: modes up to MCUMAX_SEARCH_BEST_MOVE may
// stop deepening early)
enum mcumax_mode
{
    MCUMAX_INTERNAL_NODE,
    // Root: search best move; then select mcumax.move_from/to
    MCUMAX_SEARCH_BEST_MOVE,
    // Root: list legal moves, and play mcumax.move_from/to if legal
    MCUMAX_LEGAL_MOVES,
};

struct
{
    // Pieces and weights (0x88 layout), plus a dummy square at 0x80 that
    // absorbs writes to MCUMAX_SQUARE_INVALID
    uint8_t board[0x80 + 1];
    // MCUMAX_SIDE_WHITE or MCUMAX_SIDE_BLACK
    uint8_t current_side;

    // Evaluation of the current position, from the side to move's view
    int32_t score;
    // Square skipped by the last move: e.p. target after a pawn double
    // step, rook square after castling; MCUMAX_SQUARE_INVALID otherwise
    uint8_t en_passant_square;
    // Captured non-pawn material, used as game phase indicator
    int32_t captured_material;

#ifdef MCUMAX_HASHING_ENABLED
    uint32_t hash_key;  // Hash table index
    uint32_t hash_lock; // Hash entry verification
#endif

    // Root move: move to play, or best move found
    uint8_t move_from;
    uint8_t move_to;


    // Search limits
    uint32_t node_count;
    uint32_t node_max;
    uint32_t ply;     // Current recursion depth
    uint32_t ply_max; // Recursion depth limit (bounds stack usage)

    bool stop_search;

    // User callback
    mcumax_callback user_callback;
    void *user_data;

    // Valid moves output
    mcumax_move *valid_moves_buffer;
    uint32_t valid_moves_buffer_size;
    uint32_t valid_moves_num;
} mcumax;

// Piece values by type (scaled by MCUMAX_PIECE_VALUE_SCALE).
// The king's negative value flags a king capture.
static const int8_t mcumax_piece_values[] = {
    0,  // Empty
    2,  // Pawn (upstream)
    2,  // Pawn (downstream)
    7,  // Knight
    -1, // King
    8,  // Bishop
    12, // Rook
    23, // Queen
};

// Index into mcumax_steps preceding each piece type's first step vector
static const int8_t mcumax_step_list_start[] = {
    0,  // Empty (unused)
    7,  // Pawn (upstream): -16, -15, -17
    -1, // Pawn (downstream): 16, 15, 17
    11, // Knight
    6,  // King
    8,  // Bishop
    3,  // Rook
    6,  // Queen
};

// Zero-terminated step-vector lists. Vectors are negated when read; for
// pieces other than pawns, each vector is then also tried reversed.
// The first vector of each pawn list is the straight step.
static const int8_t mcumax_steps[] = {
    // Downstream pawn
    -16, -15, -17, 0,
    // Rook
    1, 16, 0,
    // King, queen (bishop: last two); upstream pawn: last three
    1, 16, 15, 17, 0,
    // Knight
    14, 18, 31, 33, 0};

#ifdef MCUMAX_HASHING_ENABLED

#define MCUMAX_HASH_SCRAMBLE_TABLE_SIZE 1035
// Hash table entries (12 bytes each): must be defined explicitly, as the
// right size depends on the available RAM
#ifndef MCUMAX_HASH_TABLE_SIZE
#error "Define MCUMAX_HASH_TABLE_SIZE (number of entries, a power of 2)"
#elif (MCUMAX_HASH_TABLE_SIZE) & ((MCUMAX_HASH_TABLE_SIZE) - 1)
#error "MCUMAX_HASH_TABLE_SIZE must be a power of 2"
#endif

// Flags stored in mcumax_hash_entry.from (bound type)
#define MCUMAX_HASH_ABOVE_ALPHA 0x08 // Score is a lower bound
#define MCUMAX_HASH_BELOW_BETA 0x80  // Score is an upper bound

// Zobrist-like random bytes, read as overlapping 32-bit words
static uint8_t mcumax_scramble_table[MCUMAX_HASH_SCRAMBLE_TABLE_SIZE];

// Returns the hash contribution of a piece on a square.
// (memcpy: unaligned word access faults on many MCUs)
static uint32_t mcumax_hash_scramble(uint32_t square, uint8_t piece)
{
    uint32_t value;
    memcpy(&value,
           mcumax_scramble_table + square + (piece & MCUMAX_SIDE_WHITE) +
               MCUMAX_SQUARE_INVALID * (piece & MCUMAX_PIECE_TYPE_MASK),
           sizeof(value));

    return value;
}

// Hash change of the move being made (offset 0: key, 8: lock)
#define MCUMAX_HASH_DELTA(offset)                                 \
    (mcumax_hash_scramble(to + (offset), mcumax.board[to]) -      \
     mcumax_hash_scramble(from + (offset), piece) -               \
     mcumax_hash_scramble(capture_square + (offset), captured))

struct mcumax_hash_entry
{
    uint32_t lock;
    int32_t score;
    uint8_t from; // Best move origin | bound flags
    uint8_t to;   // Best move destination | MCUMAX_SQUARE_INVALID if normal
    uint8_t depth;
};

static struct mcumax_hash_entry mcumax_hash_table[MCUMAX_HASH_TABLE_SIZE];

#endif

/*
 * Recursive alpha-beta search with iterative deepening.
 *
 * alpha, beta:       search window
 * score:             evaluation of the current position (side to move)
 * en_passant_square: square skipped by the opponent's last move (e.p.
 *                    target, or castling rook square to detect castling
 *                    out of/through check), or MCUMAX_SQUARE_INVALID
 * depth:             remaining depth
 * mode:              MCUMAX_INTERNAL_NODE, or a root-level operation
 *
 * Returns the score of the position.
 */
static int32_t mcumax_search(int32_t alpha,
                             int32_t beta,
                             int32_t score,
                             uint8_t en_passant_square,
                             uint8_t depth,
                             enum mcumax_mode mode)
{
    // Recursion limit (bounds stack usage): return static evaluation
    if (mcumax.ply >= mcumax.ply_max)
        return score;
    mcumax.ply++;

    if (mcumax.user_callback)
        mcumax.user_callback(mcumax.user_data);

    // Iterative deepening state
    mcumax_byte iter_depth;
    int32_t best_score;
    mcumax_byte best_from;
    mcumax_byte best_to; // | MCUMAX_SQUARE_INVALID if not a double-step/castling

#ifdef MCUMAX_HASHING_ENABLED
    uint32_t saved_hash_key;
    uint32_t saved_hash_lock;
    uint32_t node_lock;
#endif

    mcumax_byte scan_start;

    mcumax_byte from;
    mcumax_byte to;

    // Non-zero while the previous iteration's best move is being searched
    // first (holds the MCUMAX_SQUARE_INVALID flag to strip from best_to)
    mcumax_byte replay_best;
    int32_t null_move_score;

    mcumax_byte piece;
    mcumax_byte piece_type;

    mcumax_sbyte step;
    mcumax_sbyte step_index;

    mcumax_byte skip_square;          // Square skipped by this move
    mcumax_byte castling_rook_square; // Rook origin when castling

    mcumax_byte capture_square;
    mcumax_byte captured;
    int32_t capture_value;
    int32_t promotion_bonus;

    mcumax_byte reply_depth;
    int32_t reply_alpha;
    int32_t move_score;
    int32_t reply_score;

    // Adjust window for the delayed-loss bonus (see end of function)
    alpha -= alpha < score;
    beta -= beta <= score;

#ifdef MCUMAX_HASHING_ENABLED
    // Look up position in hash table
    struct mcumax_hash_entry *hash_entry =
        mcumax_hash_table +
        ((mcumax.hash_key + mcumax.current_side * en_passant_square) &
         (MCUMAX_HASH_TABLE_SIZE - 1));

    // Entry verification: the position, side to move and e.p. square
    // (the table index alone cannot tell them apart in small tables)
    node_lock = mcumax.hash_lock + mcumax.current_side * en_passant_square;

    // Position of the game history (locked by mcumax_play_move()):
    // a repetition, so a draw
    if ((mode == MCUMAX_INTERNAL_NODE) &&
        (hash_entry->depth == MCUMAX_DEPTH_MAX) &&
        (hash_entry->lock == node_lock))
    {
        best_score = 0;
        goto exit;
    }

    // Resume at stored depth...
    iter_depth = hash_entry->depth;
    best_score = hash_entry->score;
    best_from = hash_entry->from;
    best_to = hash_entry->to;

    // ...unless the entry belongs to another position, we are at the root,
    // or the stored bound is not usable with the current window
    if ((hash_entry->lock != node_lock) ||
        (mode != MCUMAX_INTERNAL_NODE) ||
        !(((best_score <= alpha) ||
           (best_from & MCUMAX_HASH_ABOVE_ALPHA)) &&
          ((best_score >= beta) ||
           (best_from & MCUMAX_HASH_BELOW_BETA))))
    {
        // Start iteration from scratch
        iter_depth =
            best_to = 0;
    }

    // Start at best-move hint
    best_from &= ~MCUMAX_OFF_BOARD;

    saved_hash_key = mcumax.hash_key;
    saved_hash_lock = mcumax.hash_lock;
#else
    iter_depth =
        best_score =
            best_from =
                best_to = 0;
#endif

    // Legal moves: a single iteration at depth 3 (where all moves are
    // made), scanning from square 0 (board order), without replay
    if (mode == MCUMAX_LEGAL_MOVES)
    {
        iter_depth = MCUMAX_DEPTH_MIN - 1;
#ifdef MCUMAX_HASHING_ENABLED
        best_from = 0;
#endif
    }

    // Iterative deepening loop. At the root of a best-move search, keep
    // deepening while within limits; then search the best move once more
    // at minimum depth with it as the root move, so it gets selected.
    // A stopped search breaks out, so no move gets selected.
    while ((iter_depth++ < depth) ||
           (iter_depth < MCUMAX_DEPTH_MIN) ||
           ((mode == MCUMAX_SEARCH_BEST_MOVE) &&
            (mcumax.move_from == MCUMAX_SQUARE_INVALID) &&
            (((mcumax.node_count < mcumax.node_max) &&
              ((uint32_t)iter_depth <= mcumax.ply_max + 1)) ||
             (mcumax.move_from = best_from,
              mcumax.move_to = best_to & ~MCUMAX_OFF_BOARD,
              iter_depth = MCUMAX_DEPTH_MIN))))
    {
        if (mcumax.stop_search)
            break;

        // Start board scan at the previous best move
        from = scan_start = best_from;

        // Search the previous best move first, unless it was a double
        // move (pawn double step/castling), which needs normal generation.
        // (When listing legal moves, best_from and best_to are 0: the scan
        // starts at square 0, so moves are listed in board order, and
        // nothing is replayed.)
        replay_best = best_to & MCUMAX_SQUARE_INVALID;

        // Null-move search: let the opponent move twice
        mcumax.current_side ^= MCUMAX_SIDE_SWITCH;

        null_move_score = (iter_depth > 2) && (beta != -MCUMAX_SCORE_MAX)
                              ? mcumax_search(-beta,
                                              1 - beta,
                                              -score,
                                              MCUMAX_SQUARE_INVALID,
                                              iter_depth - 3,
                                              MCUMAX_INTERNAL_NODE)
                              : MCUMAX_SCORE_MAX;

        mcumax.current_side ^= MCUMAX_SIDE_SWITCH;

        // Null move fails high: prune. Otherwise start from -infinity, or
        // from the static evaluation (stand pat) in quiescence (depth 2).
        // (null_move_score == MCUMAX_SCORE_MAX means we are in check.)
        best_score = (-null_move_score < beta) ||
                             (mcumax.captured_material > MCUMAX_PHASE_NULL_MOVE_OFF)
                         ? (iter_depth - 2)
                               ? -MCUMAX_SCORE_MAX
                               : score
                         : -null_move_score;

        // Node count (for search limits)
        mcumax.node_count++;

        // Scan the board for own pieces
        do
        {
            piece = mcumax.board[from];

            if (piece & mcumax.current_side)
            {
                // Initialize step to a positive value (see loop below)
                step = piece_type = (piece & MCUMAX_PIECE_TYPE_MASK);

                step_index = mcumax_step_list_start[piece_type];

                // Loop over step directions: for non-pawns, a step vector
                // is followed by its reverse
                while ((step = ((piece_type > MCUMAX_PAWN_DOWNSTREAM) &&
                                (step < 0))
                                   ? -step
                                   : -mcumax_steps[++step_index]))
                {
                replay:
                    to = from;

                    skip_square =
                        castling_rook_square = MCUMAX_SQUARE_INVALID;

                    // Follow the ray (sliders), or take a single step
                    do
                    {
                        // Next destination, or the previous best move
                        capture_square =
                            to =
                                replay_best
                                    ? (best_to ^ replay_best)
                                    : (to + step);

                        // Board edge hit
                        if (to & MCUMAX_OFF_BOARD)
                            break;

                        // Opponent castled out of/through check: a move
                        // reaching the king's origin, passed or destination
                        // square counts as a king capture (holds at any
                        // depth, so it is not lost to later cutoffs)
                        if ((en_passant_square != MCUMAX_SQUARE_INVALID) &&
                            mcumax.board[en_passant_square] &&
                            ((to - en_passant_square) < 2) &&
                            ((en_passant_square - to) < 2))
                        {
                            best_score = MCUMAX_SCORE_MAX;
                            iter_depth = MCUMAX_DEPTH_MAX - 1;
                        }

                        // En passant: captured pawn is behind destination
                        if ((piece_type <= MCUMAX_PAWN_DOWNSTREAM) &&
                            (to == en_passant_square))
                            capture_square ^= MCUMAX_ROW_STEP;

                        captured = mcumax.board[capture_square];

                        // Own piece, or pawn moving straight onto a piece
                        // (1 - 0) or diagonally onto an empty square (0 - 1)
                        if ((captured & mcumax.current_side) ||
                            ((piece_type <= MCUMAX_PAWN_DOWNSTREAM) &&
                             !((to - from) & 0x7) - !captured))
                            break;

                        capture_value = MCUMAX_PIECE_VALUE_SCALE *
                                            mcumax_piece_values[captured & MCUMAX_PIECE_TYPE_MASK] +
                                        (captured & MCUMAX_PIECE_BONUS_MASK);

                        // King capture: previous move was illegal
                        if (capture_value < 0)
                        {
                            best_score = MCUMAX_SCORE_MAX;
                            iter_depth = MCUMAX_DEPTH_MAX - 1;
                        }

                        // Beta cutoff
                        if ((best_score >= beta) &&
                            (iter_depth > 1))
                            goto cutoff;

                        // Depth 1: MVV/LVA move ordering score only
                        move_score = (iter_depth != 1)
                                         ? score
                                         : capture_value - piece_type;

                        // Make the move (depth 2: captures only)
                        if ((iter_depth - !captured) > 1)
                        {
                            // Centralization
                            move_score = (piece_type < MCUMAX_ROOK_TYPE)
                                             ? mcumax.board[from + MCUMAX_WEIGHTS_OFFSET] -
                                                   mcumax.board[to + MCUMAX_WEIGHTS_OFFSET]
                                             : 0;

                            mcumax.board[castling_rook_square] =
                                mcumax.board[capture_square] =
                                    mcumax.board[from] = 0;

                            mcumax.board[to] = piece | MCUMAX_PIECE_MOVED;

                            // Castling: place rook, bonus
                            if (!(castling_rook_square & MCUMAX_OFF_BOARD))
                            {
                                mcumax.board[skip_square] = mcumax.current_side + MCUMAX_ROOK_TYPE;
                                move_score += 50;
                            }

                            // Keep king in place during the middle game
                            move_score -= ((piece_type != MCUMAX_KING_TYPE) ||
                                           (mcumax.captured_material > MCUMAX_PHASE_KING_ACTIVE))
                                              ? 0
                                              : 20;

                            if (piece_type <= MCUMAX_PAWN_DOWNSTREAM)
                            {
#ifndef MCUMAX_PAWN_EVAL_DISABLED
                                // Pawn structure: penalize leaving pawns
                                // two files away undefended, stepping away
                                // from the own king; end-game push bonus
                                move_score -=
                                    9 * ((((from - 2) & MCUMAX_OFF_BOARD) ||
                                          mcumax.board[from - 2] - piece) +
                                         (((from + 2) & MCUMAX_OFF_BOARD) ||
                                          mcumax.board[from + 2] - piece) -
                                         1 +
                                         (mcumax.board[from ^ MCUMAX_ROW_STEP] ==
                                          (mcumax.current_side + MCUMAX_KING_TYPE + MCUMAX_PIECE_MOVED))) -
                                    (mcumax.captured_material >> 2);
#endif

                                // Promotion to queen (type 1/2 + 646/645
                                // wraps to type 7 | bonus bit 7), or 6th/7th
                                // rank bonus (bit 6)
                                promotion_bonus =
                                    ((to + step + 1) & MCUMAX_SQUARE_INVALID)
                                        ? (647 - piece_type)
                                        : 2 * (piece & (to + MCUMAX_ROW_STEP) & 0x20);
                                capture_value += promotion_bonus;

                                mcumax.board[to] += promotion_bonus;
                            }

#ifdef MCUMAX_HASHING_ENABLED
                            mcumax.hash_key += MCUMAX_HASH_DELTA(0);
                            mcumax.hash_lock += MCUMAX_HASH_DELTA(8) +
                                                castling_rook_square - MCUMAX_SQUARE_INVALID;
#endif

                            move_score += score + capture_value;
                            reply_alpha = best_score > alpha
                                              ? best_score
                                              : alpha;

                            // Late move reduction for quiet moves
                            reply_depth = iter_depth - 1 -
                                          ((iter_depth > 5) &&
                                           (piece_type > MCUMAX_PAWN_DOWNSTREAM) &&
                                           !captured &&
                                           !replay_best);

                            // Check extension
                            if (!((mcumax.captured_material > MCUMAX_PHASE_KING_ACTIVE) ||
                                  (null_move_score != MCUMAX_SCORE_MAX) ||
                                  (iter_depth < 3) ||
                                  (captured &&
                                   (piece_type != MCUMAX_KING_TYPE))))
                                reply_depth = iter_depth;

                            // Search reply (futility: skip in quiescence
                            // if the move cannot raise alpha); re-search
                            // unreduced if a reduced search did not fail low
                            do
                            {
                                mcumax.current_side ^= MCUMAX_SIDE_SWITCH;

                                reply_score = ((mode == MCUMAX_LEGAL_MOVES) ||
                                               (reply_depth > 2) ||
                                               (move_score > reply_alpha))
                                                  ? -mcumax_search(-beta,
                                                                   -reply_alpha,
                                                                   -move_score,
                                                                   skip_square,
                                                                   reply_depth,
                                                                   MCUMAX_INTERNAL_NODE)
                                                  : move_score;

                                mcumax.current_side ^= MCUMAX_SIDE_SWITCH;
                            } while ((reply_score > alpha) &&
                                     (++reply_depth < iter_depth));

                            move_score = reply_score;

                            // Legal moves: at depth 3 (all moves made),
                            // a move is legal if the reply cannot capture
                            // the king
                            if ((mode == MCUMAX_LEGAL_MOVES) &&
                                (move_score != -MCUMAX_SCORE_MAX))
                            {
                                // Requested move: play it (leave it on the
                                // board)
                                if ((from == mcumax.move_from) &&
                                    (to == mcumax.move_to))
                                {
                                    mcumax.score = -score - capture_value;
                                    mcumax.en_passant_square = skip_square;

#ifdef MCUMAX_HASHING_ENABLED
                                    // Lock game history in hash as draw
                                    // (repetition detection)
                                    hash_entry->lock = node_lock;
                                    hash_entry->depth = MCUMAX_DEPTH_MAX;
                                    hash_entry->score = 0;
#endif

                                    mcumax.captured_material += capture_value >> MCUMAX_MATERIAL_SHIFT;

                                    mcumax.current_side ^= MCUMAX_SIDE_SWITCH;

                                    best_score = beta;
                                    goto exit;
                                }

                                // List it
                                if (mcumax.valid_moves_num < mcumax.valid_moves_buffer_size)
                                    mcumax.valid_moves_buffer[mcumax.valid_moves_num] = (mcumax_move){from, to};

                                mcumax.valid_moves_num++;
                            }

#ifdef MCUMAX_HASHING_ENABLED
                            mcumax.hash_key = saved_hash_key;
                            mcumax.hash_lock = saved_hash_lock;
#endif

                            // Undo move (castling_rook_square may be the
                            // dummy square)
                            mcumax.board[castling_rook_square] = mcumax.current_side + MCUMAX_ROOK_TYPE;
                            mcumax.board[skip_square] = mcumax.board[to] = 0;
                            mcumax.board[from] = piece;
                            mcumax.board[capture_square] = captured;

                            // Best move search: final pass found the move
                            if ((mode == MCUMAX_SEARCH_BEST_MOVE) &&
                                (move_score != -MCUMAX_SCORE_MAX) &&
                                (from == mcumax.move_from) &&
                                (to == mcumax.move_to))
                            {
                                best_score = beta;
                                goto exit;
                            }
                        }

                        // New best move (destination flagged if normal)
                        if (move_score > best_score)
                        {
                            best_score = move_score;
                            best_from = from;
                            best_to = to |
                                      (skip_square & MCUMAX_SQUARE_INVALID);
                        }

                        // Previous best move done: resume normal generation
                        if (replay_best)
                        {
                            replay_best = 0;

                            goto replay;
                        }

                        // Stop the ray unless this is the first step of a
                        // virgin pawn (double step), or a virgin king's
                        // lateral step towards a virgin rook with two empty
                        // squares next to it (castling)
                        if ((from + step - to) ||
                            (piece & MCUMAX_PIECE_MOVED) ||
                            ((piece_type > MCUMAX_PAWN_DOWNSTREAM) &&
                             (((piece_type != MCUMAX_KING_TYPE) ||
                               (step_index != MCUMAX_KING_LATERAL_STEP_INDEX) ||
                               (mcumax.board[castling_rook_square =
                                                 ((from + 3) ^
                                                  ((step >> 1) & 0x7))] -
                                mcumax.current_side - MCUMAX_ROOK_TYPE) ||
                               mcumax.board[castling_rook_square ^ 1] ||
                               mcumax.board[castling_rook_square ^ 2]))))
                            // Fake a capture for non-sliding pieces
                            captured += (piece_type < MCUMAX_BISHOP_TYPE);
                        else
                            // Double move: remember skipped square
                            skip_square = to;

                        // Continue the ray while no capture
                    } while (!captured);
                }
            }

            // Next square of board, wrap
        } while ((from = ((from + 9) &
                          ~MCUMAX_OFF_BOARD)) != scan_start);

    cutoff:
        // All moves lose the king: checkmate if in check (null move
        // loses king), otherwise stalemate
        if ((best_score == -MCUMAX_SCORE_MAX) &&
            (null_move_score != MCUMAX_SCORE_MAX))
            best_score = 0;

#ifdef MCUMAX_HASHING_ENABLED
        // Store result, unless position is part of the game history
        if (hash_entry->depth < MCUMAX_DEPTH_MAX)
        {
            hash_entry->lock = node_lock;
            hash_entry->score = best_score;
            hash_entry->depth = iter_depth;

            hash_entry->from = best_from |
                               MCUMAX_HASH_ABOVE_ALPHA * (best_score > alpha) |
                               MCUMAX_HASH_BELOW_BETA * (best_score < beta);
            hash_entry->to = best_to;
        }
#endif

        // Kibitz (uncomment for search progress)
        // if (mode != MCUMAX_INTERNAL_NODE)
        //     printf("%2d %6d %10d %c%c%c%c\n",
        //            iter_depth - 2,
        //            best_score,
        //            mcumax.node_count,
        //            'a' + (best_from & 0x7),
        //            '8' - (best_from >> 4),
        //            'a' + (best_to & 0x7),
        //            '8' - ((best_to >> 4) & 0x7));
    }

    // Delayed-loss bonus: prefer slower losses and faster wins
    if (best_score < score)
        best_score++;

exit:
    mcumax.ply--;

    return best_score;
}

/***************************************************************************/

// Clears the board and resets the engine state
static inline void mcumax_reset(void)
{
    for (uint8_t square = 0; square < 0x80; square++)
    {
        // Pieces (left half): empty; centralization weights (right half):
        // (x - 4)^2 + (y - 3.5)^2
        int32_t x = square & 0x7;
        int32_t y = square >> 4;

        mcumax.board[square] = (square & MCUMAX_WEIGHTS_OFFSET)
                                   ? (x - 4) * (x - 4) + (y - 4) * (y - 3)
                                   : MCUMAX_EMPTY;
    }

    mcumax.current_side = MCUMAX_SIDE_WHITE;
    mcumax.score = 0;
    mcumax.en_passant_square = MCUMAX_SQUARE_INVALID;
    mcumax.captured_material = 0;

#ifdef MCUMAX_HASHING_ENABLED
    mcumax.hash_key = 0;
    mcumax.hash_lock = 0;

    memset(mcumax_hash_table, 0, sizeof(mcumax_hash_table));

    srand(1);
    // Entries below 0x88 (empty squares) stay 0, so the incremental hash
    // keys depend only on the position (as in micro-Max): repetitions and
    // transpositions get the same keys
    for (uint32_t i = MCUMAX_OFF_BOARD; i < MCUMAX_HASH_SCRAMBLE_TABLE_SIZE; i++)
        mcumax_scramble_table[i] = rand() & 0xff;
#endif
}

static const uint8_t mcumax_board_setup[] = {
    MCUMAX_ROOK_TYPE,
    MCUMAX_KNIGHT_TYPE,
    MCUMAX_BISHOP_TYPE,
    MCUMAX_QUEEN_TYPE,
    MCUMAX_KING_TYPE,
    MCUMAX_BISHOP_TYPE,
    MCUMAX_KNIGHT_TYPE,
    MCUMAX_ROOK_TYPE,
};

void mcumax_init(void)
{
    mcumax_reset();

    for (uint8_t x = 0; x < 8; x++)
    {
        mcumax.board[0x00 + x] = MCUMAX_SIDE_BLACK | mcumax_board_setup[x];
        mcumax.board[0x10 + x] = MCUMAX_SIDE_BLACK | MCUMAX_PAWN_DOWNSTREAM;
        mcumax.board[0x60 + x] = MCUMAX_SIDE_WHITE | MCUMAX_PAWN_UPSTREAM;
        mcumax.board[0x70 + x] = MCUMAX_SIDE_WHITE | mcumax_board_setup[x];
    }
}

// Public piece type of each internal piece type
static const uint8_t mcumax_public_types[] = {
    MCUMAX_EMPTY,
    MCUMAX_PAWN,
    MCUMAX_PAWN,
    MCUMAX_KNIGHT,
    MCUMAX_KING,
    MCUMAX_BISHOP,
    MCUMAX_ROOK,
    MCUMAX_QUEEN,
};

// FEN support (for testing). Not called by the engine, so the linker
// drops it from builds that don't use it (with -ffunction-sections and
// --gc-sections).

// FEN letter of each internal piece type
static const char mcumax_fen_symbols[] = "?PPNKBRQ";

void mcumax_set_fen_position(const char *fen_string)
{
    mcumax_init();

    // Clear pieces
    for (uint8_t square = 0; square < 0x80; square++)
        if (!(square & MCUMAX_WEIGHTS_OFFSET))
            mcumax.board[square] = MCUMAX_EMPTY;

    // Non-pawn material on the board is subtracted from the initial
    // 4 * (N 2 + B 2 + R 3) + 2 * Q 6 = 40 (see MCUMAX_MATERIAL_SHIFT)
    int32_t captured_material = 40;

    // Material balance (white's view), for the evaluation
    int32_t material = 0;

    // FEN fields: 0 placement, 1 side to move, 2 castling, 3 en passant
    uint32_t field_index = 0;
    // uint8_t: any value with bit 7 clear is a valid board index
    uint8_t square = 0;

    char c;
    while ((c = *fen_string++))
    {
        // Upper case letter; for lower case letters: black piece/row 0
        char upper_c = c & ~0x20;
        uint8_t is_black = c & 0x20;

        if (c == ' ')
            field_index++;
        else if (field_index == 0)
        {
            if (c == '/')
            {
                // Next row
                if (!(square & MCUMAX_SQUARE_INVALID))
                    square = (square & 0x70) + MCUMAX_ROW_STEP;
            }
            else if ((c >= '1') && (c <= '8'))
                // Empty squares (the board was cleared above)
                square += c - '0';
            else if (!(square & MCUMAX_OFF_BOARD))
            {
                uint8_t piece_type = MCUMAX_QUEEN_TYPE;
                while (piece_type && (mcumax_fen_symbols[piece_type] != upper_c))
                    piece_type--;

                if (piece_type)
                {
                    // Game phase value, (37 * value) >> 7, equals
                    // (value + 1) >> 2: N, B 2; R 3; Q 6; P, K 0
                    captured_material -= (mcumax_piece_values[piece_type] + 1) >> 2;

                    int32_t value = MCUMAX_PIECE_VALUE_SCALE * mcumax_piece_values[piece_type];
                    material += is_black ? -value : value;

                    // Mark as moved; castling rights are restored below
                    uint8_t moved = MCUMAX_PIECE_MOVED;

                    if (piece_type <= MCUMAX_PAWN_DOWNSTREAM)
                    {
                        piece_type = is_black ? MCUMAX_PAWN_DOWNSTREAM : MCUMAX_PAWN_UPSTREAM;

                        // Pawns on their initial rank keep their double
                        // step right
                        if ((square & 0x70) == (is_black ? 0x10 : 0x60))
                            moved = 0;
                    }

                    mcumax.board[square] = (is_black ? MCUMAX_SIDE_BLACK : MCUMAX_SIDE_WHITE) |
                                           moved | piece_type;
                }

                square++;
            }
        }
        else if (field_index == 1)
            mcumax.current_side = (c == 'b') ? MCUMAX_SIDE_BLACK : MCUMAX_SIDE_WHITE;
        else if (field_index == 2)
        {
            // Castling rights: mark king and rook as not moved
            if ((upper_c == 'K') || (upper_c == 'Q'))
            {
                uint8_t row = is_black ? 0x00 : 0x70;

                mcumax.board[row + 4] &= ~MCUMAX_PIECE_MOVED;
                mcumax.board[row + ((upper_c == 'K') ? 7 : 0)] &= ~MCUMAX_PIECE_MOVED;
            }
        }
        else if (field_index == 3)
        {
            // En passant square
            if ((c >= 'a') && (c <= 'h'))
                mcumax.en_passant_square = c - 'a';
            else if ((c >= '1') && (c <= '8'))
                mcumax.en_passant_square = (mcumax.en_passant_square & 0x07) |
                                           (MCUMAX_ROW_STEP * ('8' - c));
        }
    }

    mcumax.captured_material = (captured_material > 0) ? captured_material : 0;

    // Evaluation of the position, from the side to move's view. Search
    // scores are relative to it, so e.g. a draw (score 0) is only avoided
    // when ahead if this reflects the material balance.
    mcumax.score = (mcumax.current_side == MCUMAX_SIDE_WHITE) ? material : -material;
}

void mcumax_get_fen_position(char *fen_string)
{
    char *p = fen_string;

    // Piece placement
    for (uint8_t row = 0; row < 0x80; row += MCUMAX_ROW_STEP)
    {
        uint8_t empty_num = 0;

        for (uint8_t square = row; square < row + 8; square++)
        {
            uint8_t piece = mcumax.board[square];
            uint8_t piece_type = piece & MCUMAX_PIECE_TYPE_MASK;

            if (!piece_type)
                empty_num++;
            else
            {
                if (empty_num)
                    *p++ = '0' + empty_num;
                empty_num = 0;

                // Black: lower case
                *p++ = mcumax_fen_symbols[piece_type] |
                       ((piece & MCUMAX_SIDE_BLACK) ? 0x20 : 0);
            }
        }

        if (empty_num)
            *p++ = '0' + empty_num;
        *p++ = (row < 0x70) ? '/' : ' ';
    }

    // Side to move
    *p++ = (mcumax.current_side == MCUMAX_SIDE_BLACK) ? 'b' : 'w';
    *p++ = ' ';

    // Castling rights: king and rook not moved
    char *castling = p;
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t side = (i & 2) ? MCUMAX_SIDE_BLACK : MCUMAX_SIDE_WHITE;
        uint8_t row = (i & 2) ? 0x00 : 0x70;

        if ((mcumax.board[row + 4] == (side | MCUMAX_KING_TYPE)) &&
            (mcumax.board[row + ((i & 1) ? 0 : 7)] == (side | MCUMAX_ROOK_TYPE)))
            *p++ = "KQkq"[i];
    }
    if (p == castling)
        *p++ = '-';
    *p++ = ' ';

    // En passant square: square skipped by a pawn double step (rows 2 and
    // 5; castling skips squares in rows 0 and 7)
    uint8_t square = mcumax.en_passant_square;
    if (((square & 0xf0) == 0x20) || ((square & 0xf0) == 0x50))
    {
        *p++ = 'a' + (square & 0x7);
        *p++ = '8' - (square >> 4);
    }
    else
        *p++ = '-';

    // Halfmove clock and fullmove number are not tracked
    strcpy(p, " 0 1");
}

mcumax_piece mcumax_get_piece(mcumax_square square)
{
    if (square & MCUMAX_OFF_BOARD)
        return MCUMAX_EMPTY;

    uint8_t piece = mcumax.board[square];

    return mcumax_public_types[piece & MCUMAX_PIECE_TYPE_MASK] |
           ((piece >> 1) & MCUMAX_BLACK);
}

mcumax_piece mcumax_get_current_side(void)
{
    return (mcumax.current_side >> 1) & MCUMAX_BLACK;
}

// Starts a root search. ply_max (at least 2: the root and the replies
// that detect king captures, i.e. illegal moves) bounds the recursion
// depth.
static int32_t mcumax_start_search(enum mcumax_mode mode,
                                   mcumax_move move,
                                   uint32_t ply_max,
                                   uint32_t node_max)
{
    mcumax.move_from = move.from;
    mcumax.move_to = move.to;

    mcumax.node_max = node_max;
    mcumax.node_count = 0;
    mcumax.ply = 0;
    mcumax.ply_max = ply_max;

    mcumax.stop_search = false;

    return mcumax_search(-MCUMAX_SCORE_MAX,
                         MCUMAX_SCORE_MAX,
                         mcumax.score,
                         mcumax.en_passant_square,
                         MCUMAX_DEPTH_MIN,
                         mode);
}

uint32_t mcumax_search_valid_moves(mcumax_move *valid_moves_buffer, uint32_t valid_moves_buffer_size)
{
    mcumax.valid_moves_num = 0;
    mcumax.valid_moves_buffer = valid_moves_buffer;
    mcumax.valid_moves_buffer_size = valid_moves_buffer_size;

    // Legality only needs the root and the replies
    mcumax_start_search(MCUMAX_LEGAL_MOVES, MCUMAX_MOVE_INVALID, 2, 0);

    return mcumax.valid_moves_num;
}

mcumax_move mcumax_search_best_move(uint32_t node_max, uint32_t depth_max)
{
    // depth_max must be at most 90 (see mcu-max.h), which keeps search
    // depths below MCUMAX_DEPTH_MAX - 1 (king capture marker) and
    // MCUMAX_DEPTH_MAX (game history marker in the hash)
    // Search returns beta (MCUMAX_SCORE_MAX) when the final pass found
    // the best move
    int32_t score = mcumax_start_search(MCUMAX_SEARCH_BEST_MOVE,
                                        MCUMAX_MOVE_INVALID,
                                        depth_max + 2,
                                        node_max);

    if (score == MCUMAX_SCORE_MAX)
        return (mcumax_move){mcumax.move_from, mcumax.move_to};
    else
        return MCUMAX_MOVE_INVALID;
}

mcumax_game_state mcumax_get_game_state(void)
{
    uint32_t valid_moves_num = mcumax_search_valid_moves(NULL, 0);

    // In check: the opponent, if to move, could capture the king
    // (a depth-1 search returns MCUMAX_SCORE_MAX on king capture)
    mcumax.current_side ^= MCUMAX_SIDE_SWITCH;
    bool in_check = (mcumax_search(-MCUMAX_SCORE_MAX,
                                   MCUMAX_SCORE_MAX,
                                   0,
                                   MCUMAX_SQUARE_INVALID,
                                   1,
                                   MCUMAX_INTERNAL_NODE) == MCUMAX_SCORE_MAX);
    mcumax.current_side ^= MCUMAX_SIDE_SWITCH;

    return (valid_moves_num ? MCUMAX_GAME_ONGOING : MCUMAX_GAME_STALEMATE) |
           in_check;
}

bool mcumax_play_move(mcumax_move move)
{
    // Don't list moves
    mcumax.valid_moves_buffer_size = 0;

    // Search returns beta (MCUMAX_SCORE_MAX) when the move was played
    return mcumax_start_search(MCUMAX_LEGAL_MOVES, move, 2, 0) == MCUMAX_SCORE_MAX;
}

void mcumax_set_callback(mcumax_callback callback, void *userdata)
{
    mcumax.user_callback = callback;
    mcumax.user_data = userdata;
}

void mcumax_stop_search(void)
{
    mcumax.stop_search = true;
}
