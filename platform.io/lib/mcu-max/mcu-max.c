/*
 * mcu-max 0.9.1
 * Chess engine for low-resource MCUs
 *
 * (C) 2022-2023 Gissio
 *
 * License: MIT
 *
 * Based on micro-Max 4.8 by H.G. Muller.
 * Compliant with FIDE laws (except for underpromotion).
 * Optimized for speed and clarity.
 */

#include <stdlib.h>

#include "mcu-max.h"

// Configuration
// #define MCUMAX_HASHING_ENABLED
#define MCUMAX_HASH_TABLE_SIZE (1 << 24)

// Constants
#define MCUMAX_BOARD_MASK 0x88

#define MCUMAX_MAX_VALUE 1000000

#define MCUMAX_MOVED 0x20

typedef signed char mcumax_direction;

typedef bool (*mcumax_move_callback)(mcumax_move move);

// The board is 16x8 + dummy; first half: pieces, second half: square weights
struct
{
    // State
    mcumax_square board[0x10 * 0x8 + 1];
    unsigned char current_side; // Either MCUMAX_WHITE or MCUMAX_BLACK
    mcumax_square en_passant_square;

    // User callback
    mcumax_callback user_callback;
    void *user_callback_userdata;

    // Move callback
    mcumax_move_callback move_callback;

    // Valid moves
    int valid_moves_buffer_size;
    mcumax_move *valid_moves_buffer;
    int valid_moves_num;

    // Best move
    int depth_max;
    int nodes_count;
    int nodes_count_max;
    mcumax_move best_move;

    // Play move
    mcumax_move play_move;

    // Stop search
    volatile bool stop_search;
} mcumax;

// Piece values (king is marked negative as it loses game)
static const signed char mcumax_piece_values[] = {
    // 0, 2, 2, -1, 7, 8, 12, 23}; // For underpromotion
    0, 2, 2, 7, -1, 8, 12, 23};

// Step-vector lists
static const unsigned char mcumax_step_vectors_indices[] = {
    // 0, 0, 4, 17, 8, 26, 31, 17}; // For underpromotion
    0, 0, 4, 8, 17, 26, 31, 17};

static const mcumax_direction mcumax_step_vectors[] = {
    // Upstream pawn
    -16, -15, -17, 0,
    // Downstream pawn
    16, 15, 17, 0,
    // Knight
    -31, -14, 18, 33, 31, 14, -18, -33, 0,
    // King, queen
    -16, -15, 1, 17, 16, 15, -1, -17, 0,
    // Bishop
    -15, 17, 15, -17, 0,
    // Rook
    -16, 1, 16, -1, 0};

static const unsigned char mcumax_board_setup[] =
    {
        MCUMAX_ROOK,
        MCUMAX_KNIGHT,
        MCUMAX_BISHOP,
        MCUMAX_QUEEN,
        MCUMAX_KING,
        MCUMAX_BISHOP,
        MCUMAX_KNIGHT,
        MCUMAX_ROOK,
};

static inline void change_side(void)
{
    mcumax.current_side ^= (MCUMAX_WHITE | MCUMAX_BLACK);
}

static inline bool is_off_board(mcumax_square square)
{
    return (square & MCUMAX_BOARD_MASK) != 0;
}

static inline mcumax_square get_next_board_square(mcumax_square square)
{
    return ((square + 9) & ~MCUMAX_BOARD_MASK);
}

static inline bool is_moving_sideways(mcumax_direction move_direction)
{
    return (move_direction == -1) || (move_direction == 1);
}

static inline bool is_pawn_capture_direction(mcumax_direction direction)
{
    return (direction & 0xf) != 0;
}

static inline bool is_pawn_promotion(mcumax_square move_to)
{
    unsigned char move_to_rank = (move_to & 0x70);

    return (move_to_rank == 0x0) || (move_to_rank == 0x70);
}

static inline bool is_pawn_double_move(mcumax_square move_to_square)
{
    return (move_to_square & 0x70) == (mcumax.current_side == MCUMAX_WHITE ? 0x50 : 0x20);
}

static inline mcumax_square get_en_passant_capture_square(mcumax_square capture_square)
{
    return capture_square ^ 0x10;
}

static inline mcumax_square get_castling_from_square(mcumax_square move_to_square)
{
    return ((move_to_square & 0x70) | (((move_to_square & 0x7) == 0x5) ? 0x7 : 0x0));
}

static inline mcumax_square get_castling_neighbor1_square(mcumax_square castling_move_from)
{
    return (castling_move_from ^ 1);
}

static inline mcumax_square get_castling_neighbor2_square(mcumax_square castling_move_from)
{
    return (castling_move_from ^ 2);
}

static inline mcumax_piece get_piece_type(mcumax_piece piece)
{
    return (piece & 0x7);
}

static inline mcumax_piece set_piece_type(mcumax_piece piece, mcumax_piece pieceType)
{
    return ((piece & ~0x7) | pieceType);
}

static inline bool is_empty(mcumax_piece piece)
{
    return !piece;
}

static inline bool is_pawn(mcumax_piece piece)
{
    return ((piece & 0x7) <= MCUMAX_PAWN_DOWNSTREAM);
}

static inline bool is_leaper(mcumax_piece piece)
{
    return ((piece & 0x7) < MCUMAX_BISHOP);
}

static inline bool is_own_piece(mcumax_piece piece)
{
    return (piece & mcumax.current_side) != 0;
}

static inline bool is_unmoved_king(mcumax_piece piece)
{
    return ((piece & ~(MCUMAX_WHITE | MCUMAX_BLACK)) == MCUMAX_KING);
}

static inline bool is_unmoved_rook(mcumax_piece piece)
{
    return ((piece & ~(MCUMAX_WHITE | MCUMAX_BLACK)) == MCUMAX_ROOK);
}

// Traverse the game tree
static int mcumax_traverse_game_tree(int depth, int alpha, int beta)
{
    if (mcumax.user_callback)
        mcumax.user_callback(mcumax.user_callback_userdata);

    mcumax_square move_from_start = 0;
    int node_count_start = mcumax.nodes_count_max;

    // Traverse squares
    mcumax_move move;
    move.from = move_from_start;
    do
    {
        mcumax_piece move_from_piece = mcumax.board[move.from];

        if (!is_own_piece(move_from_piece))
            continue;

        // For underpromotion:
        // mcumax_piece pawn_promotion_piece = MCUMAX_QUEEN;

        // Traverse directions
        int step_vector_index = mcumax_step_vectors_indices[get_piece_type(move_from_piece)];
        mcumax_direction step_vector;
        while ((step_vector = mcumax_step_vectors[step_vector_index++]))
        {
            move.to = move.from;
            mcumax_square en_passant_square = MCUMAX_INVALID;
            mcumax_move castling_move = {MCUMAX_INVALID, MCUMAX_INVALID};

            // Traverse ray
            while (true)
            {
                move.to += step_vector;

                // Off board?
                if (is_off_board(move.to))
                    break;

                // En passant capture?
                mcumax_square capture_square;
                if (is_pawn(move_from_piece) && (move.to == mcumax.en_passant_square))
                    capture_square = get_en_passant_capture_square(mcumax.en_passant_square);
                else
                    capture_square = move.to;

                mcumax_piece capture_piece = mcumax.board[capture_square];

                // Own piece?
                if (is_own_piece(capture_piece))
                    break;

                // Pawn capture
                if (is_pawn(move_from_piece))
                {
                    // Non-optimized:
                    if (is_pawn_capture_direction(step_vector))
                    {
                        if (is_empty(capture_piece))
                            break;
                    }
                    else
                    {
                        if (!is_empty(capture_piece))
                            break;
                    }

                    // Optimized:
                    // if (!(is_pawn_capture_direction(move_direction) ^ is_empty(capture_piece)))
                    //     break;
                }

                // Capture piece value
                int capture_value = 37 * mcumax_piece_values[get_piece_type(capture_piece)];

                // King in check?
                if (capture_value < 0)
                {
                    mcumax.nodes_count_max = node_count_start;
                    return MCUMAX_MAX_VALUE;
                }

                // Promote pawn?
                mcumax_piece move_to_piece;
                if (is_pawn(move_from_piece) && is_pawn_promotion(move.to))
                {
                    move_to_piece = set_piece_type(move_from_piece, MCUMAX_QUEEN);

                    // For underpromotion:
                    // move_to_piece = set_piece_type(move_from_piece, pawn_promotion_piece);
                    // if (pawn_promotion_piece > MCUMAX_KNIGHT)
                    //     step_vector_index--;
                    // pawn_promotion_piece--;
                }
                else
                    move_to_piece = move_from_piece;

                bool move_in_check = false;
                if (depth > 0)
                {
                    // Make move
                    mcumax.board[move.from] = MCUMAX_EMPTY;
                    mcumax.board[capture_square] = MCUMAX_EMPTY;
                    mcumax.board[move.to] = MCUMAX_MOVED | move_to_piece;
                    mcumax.board[castling_move.from] = MCUMAX_EMPTY;
                    mcumax.board[castling_move.to] = mcumax.current_side | MCUMAX_MOVED | MCUMAX_ROOK;

                    change_side();

                    // Negamax recursion
                    unsigned char prev_en_passant_square = mcumax.en_passant_square;
                    mcumax.en_passant_square = en_passant_square;
                    int score = -mcumax_traverse_game_tree(depth - 1, -beta, -alpha);
                    mcumax.en_passant_square = prev_en_passant_square;

                    move_in_check = (score == -MCUMAX_MAX_VALUE);
                    if (!move_in_check)
                    {
                        mcumax.nodes_count_max++;

                        if (mcumax.move_callback && mcumax.move_callback(move))
                            return 0;
                    }

                    // Undo move
                    change_side();

                    mcumax.board[castling_move.to] = MCUMAX_EMPTY;
                    mcumax.board[castling_move.from] = mcumax.current_side | MCUMAX_ROOK;
                    mcumax.board[move.to] = MCUMAX_EMPTY;
                    mcumax.board[capture_square] = capture_piece;
                    mcumax.board[move.from] = move_from_piece;

                    // Alpha-beta pruning
                    score += capture_value;
                    if (score >= beta)
                        return beta;
                    if (score > alpha)
                    {
                        alpha = score;

                        if (depth == mcumax.depth_max)
                            mcumax.best_move = move;
                    }
                }

                // Piece captured?
                if (!is_empty(capture_piece))
                    break;

                // Pawn double move
                if (is_pawn(move_from_piece))
                {
                    if (!is_pawn_double_move(move.to))
                        break;

                    en_passant_square = move.to;
                }
                // Castling
                else if (is_unmoved_king(move_from_piece) &&
                         is_moving_sideways(step_vector) &&
                         is_off_board(castling_move.from))
                {
                    // Rook moved?
                    castling_move.from = get_castling_from_square(move.to);
                    if (!is_unmoved_rook(mcumax.board[castling_move.from]))
                        break;

                    // Two empty squares next to rook?
                    if (!(is_empty(mcumax.board[get_castling_neighbor1_square(castling_move.from)]) &&
                          is_empty(mcumax.board[get_castling_neighbor2_square(castling_move.from)])))
                        break;

                    // From check or through check?
                    bool currently_in_check = false;
                    if (depth > 0)
                    {
                        change_side();

                        currently_in_check = (mcumax_traverse_game_tree(0, -MCUMAX_MAX_VALUE, MCUMAX_MAX_VALUE) == -MCUMAX_MAX_VALUE);

                        change_side();
                    }

                    if (currently_in_check || move_in_check)
                        break;

                    castling_move.to = move.to;
                }
                // Leaper?
                else if (is_leaper(move_from_piece))
                    break;
            };
        }
    } while ((move.from = get_next_board_square(move.from)) != move_from_start);

    if (depth <= 1)
    {
        // To-do: evaluate
        alpha = 0;
    }

    return alpha;
}

/***************************************************************************/
/*                               micro-Max,                                */
/* A chess program smaller than 2KB (of non-blank source), by H.G. Muller  */
/***************************************************************************/
/* version 4.8 (1953 characters) features:                                 */
/* - recursive negamax search                                              */
/* - all-capture MVV/LVA quiescence search                                 */
/* - (internal) iterative deepening                                        */
/* - best-move-first 'sorting'                                             */
/* - a hash table storing score and best move                              */
/* - futility pruning                                                      */
/* - king safety through magnetic, frozen king in middle-game              */
/* - R=2 null-move pruning                                                 */
/* - keep hash and repetition-draw detection                               */
/* - better defense against passers through gradual promotion              */
/* - extend check evasions in inner nodes                                  */
/* - reduction of all non-Pawn, non-capture moves except hash move (LMR)   */
/* - full FIDE rules (expt under-promotion) and move-legality checking     */

#include <stdio.h>
#include <stdlib.h>

#define M 0x88
#define S 0x80
#define INF 8000

#define K(A, B) *(int *)(Zobrist + A + (B & 8) + S * (B & 7))
#define J(A) K(ToSqr + A, mcumax.board[ToSqr]) - K(FromSqr + A, Piece) - K(CaptSqr + A, Victim)

static int InputFrom;
static unsigned char InputTo;

/* board: half of 16x8+dummy */
static int NonPawnMaterial;
static int Side;

static int RootEval;
static int RootEP;

/* hash table, 16M+8 entries */
#ifdef MCUMAX_HASHING_ENABLED

static struct HashEntry
{
    int Key;
    int Score;
    unsigned char From;
    unsigned char To;
    unsigned char Draft;
} HashTab[MCUMAX_HASH_TABLE_SIZE];

static int HashKeyLo;
static int HashKeyHi;

/* hash translation table */
unsigned char Zobrist[1035];
#endif

/* relative piece values */
const signed char PieceVal[] = {0, 2, 2, 7, -1, 8, 12, 23};

/* 1st dir. in StepVecs[] per piece */
const signed char StepVecsIndices[] = {0, 7, -1, 11, 6, 8, 3, 6};

/* step-vector lists */
const signed char StepVecs[] = {-16, -15, -17, 0, 1, 16, 0, 1, 16, 15, 17, 0, 14, 18, 31, 33, 0};

/* initial piece setup */
const signed char PieceSetup[] = {6, 3, 5, 7, 4, 5, 3, 6};

/* recursive minimax search, Side=moving side, Depth=depth */
/* (Alpha,Beta)=window, Eval=current eval. score, epSqr=e.p. sqr. */
/* Eval=score, LastTo=prev.dest; HashKeyLo,HashKeyHi=hashkeys; return score */
static int Search(int Alpha, int Beta, int Eval, int epSqr, int LastTo, int Depth)
{
    int StepVec;

    int BestScore;

    int Score;

    int IterDepth;

    int h;

    int SkipSqr;
    int RookSqr;

    int NewAlpha;

#ifdef MCUMAX_HASHING_ENABLED
    int LocalHashKeyLo = HashKeyLo;
    int LocalHashKeyHi = HashKeyHi;
#endif
    unsigned char Victim;
    unsigned char PieceType;
    unsigned char Piece;

    unsigned char FromSqr;
    unsigned char ToSqr;

    unsigned char BestFrom;
    unsigned char BestTo;

    unsigned char CaptSqr;
    unsigned char StartSqr;

    // +++ mcu-max changed
    if (mcumax.user_callback)
        mcumax.user_callback(mcumax.user_callback_userdata);
    // --- mcu-max changed

#ifdef MCUMAX_HASHING_ENABLED
    /* lookup pos. in hash table */
    struct HashEntry *Hash = HashTab + ((HashKeyLo + Side * epSqr) & MCUMAX_HASH_TABLE_SIZE - 1);
#endif
    /* adj. window: delay bonus */
    Alpha--;

    /* change sides */
    Side ^= 0x18;

#ifdef MCUMAX_HASHING_ENABLED
    IterDepth = Hash->Draft;
    BestScore = Hash->Score;
    BestFrom = Hash->From;
    BestTo = Hash->To;                                                                /* resume at stored depth */
    if ((Hash->Key - HashKeyHi) | LastTo |                                            /* miss: other pos. or empty*/
        !((BestScore <= Alpha) | BestFrom & 8 && (BestScore >= Beta) | BestFrom & S)) /* or window incompatible */
        IterDepth = BestTo = 0;                                                       /* start iter. from scratch */
    BestFrom &= ~M;                                                                   /* start at best-move hint */
#else
    IterDepth = BestScore = BestFrom = BestTo = 0;
#endif

    /* iterative deepening loop */
    while ((IterDepth++ < Depth) || (IterDepth < 3) ||
           LastTo & (InputFrom == INF) &&
               ((mcumax.nodes_count < mcumax.nodes_count_max) & (IterDepth < 98) || /* root: deepen upto time */
                (InputFrom = BestFrom, InputTo = (BestTo & ~M), IterDepth = 3)))    /* time's up: go do best */
    {
        // +++ mcu-max changed
        if (mcumax.stop_search)
            break;
        // --- mcu-max changed

        /* start scan at prev. best */
        FromSqr = StartSqr = BestFrom;

        /* request try noncastl. 1st */
        h = BestTo & S;

        /* Search null move */
        int P = (IterDepth < 3)
                    ? INF
                    : Search(-Beta, 1 - Beta, -Eval, S, 0, IterDepth - 3);

        /* Prune or stand-pat  */
        BestScore = (-P < Beta) | (NonPawnMaterial > 35) ? (IterDepth > 2) ? -INF : Eval : -P;

        /* node count (for timing) */
        mcumax.nodes_count++;

        do
        {
            /* scan board looking for */
            Piece = mcumax.board[FromSqr];

            /* own piece (inefficient!) */
            if (Piece & Side)
            {
                /* PieceType = piece type (set StepVec > 0) */
                StepVec = PieceType = (Piece & 7);

                /* first step vector f. piece */
                int j = StepVecsIndices[PieceType];

                /* loop over directions StepVecs[] */
                while ((StepVec = ((PieceType > 2) & (StepVec < 0)) ? -StepVec : -StepVecs[++j]))
                {
                    /* resume normal after best */
                replay:
                    /* (FromSqr,ToSqr)=move, (SkipSqr,RookSqr)=castl.R */
                    ToSqr = FromSqr;
                    SkipSqr = RookSqr = S;

                    do
                    {
                        /* ToSqr traverses ray, or: sneak in prev. best move */
                        CaptSqr = ToSqr = h ? (BestTo ^ h) : (ToSqr + StepVec);
                        if (ToSqr & M)
                            break;
                        /* board edge hit */
                        BestScore = (epSqr - S) & mcumax.board[epSqr] && ((ToSqr - epSqr) < 2) & ((epSqr - ToSqr) < 2) ? INF : BestScore; /* bad castling             */
                                                                                                                                          /* shift capt.sqr. CaptSqr if e.p.*/
                        if ((PieceType < 3) & (ToSqr == epSqr))
                            CaptSqr ^= 0x10;

                        Victim = mcumax.board[CaptSqr];

                        /* capt. own, bad pawn mode */
                        if (Victim & Side | (PieceType < 3) & !(ToSqr - FromSqr & 7) - !Victim)
                            break;

                        /* value of capt. piece Victim */
                        int i = 37 * PieceVal[Victim & 7] + (Victim & 0xc0);

                        /* K capture */
                        BestScore = (i < 0) ? INF : BestScore;

                        /* abort on fail high */
                        if ((BestScore >= Beta) & (IterDepth > 1))
                            goto cutoff;

                        /* MVV/LVA scoring */
                        Score = (IterDepth - 1) ? Eval : i - PieceType;
                        /* remaining depth */
                        if ((IterDepth - !Victim) > 1)
                        {
                            Score = (PieceType < 6) ? mcumax.board[FromSqr + 8] - mcumax.board[ToSqr + 8] : 0; /* center positional pts. */
                            mcumax.board[RookSqr] = mcumax.board[CaptSqr] = mcumax.board[FromSqr] = 0;

                            /* do move, set non-virgin */
                            mcumax.board[ToSqr] = Piece | 0x20;
                            if (!(RookSqr & M))
                            {
                                /* castling: put R & score */
                                mcumax.board[SkipSqr] = Side + 6;
                                Score += 50;
                            }

                            /* penalize mid-game K move */
                            Score -= (PieceType - 4) | (NonPawnMaterial > 29) ? 0 : 20;

                            /* pawns: */
                            if (PieceType < 3)
                            {
                                Score -= 9 * (((FromSqr - 2) & M || mcumax.board[FromSqr - 2] - Piece) +   /* structure, undefended    */
                                              ((FromSqr + 2) & M || mcumax.board[FromSqr + 2] - Piece) - 1 /* squares plus bias */
                                              + (mcumax.board[FromSqr ^ 16] == Side + 36))                 /* kling to non-virgin King */
                                         - (NonPawnMaterial >> 2);                                         /* end-game Pawn-push bonus */

                                /* promotion or 6/7th bonus */
                                NewAlpha = ToSqr + StepVec + 1 & S
                                               ? (647 - PieceType)
                                               : 2 * (Piece & (ToSqr + 0x10) & 0x20);
                                mcumax.board[ToSqr] += NewAlpha;

                                /* change piece, add score */
                                i += NewAlpha;
                            }
                            Score += Eval + i;

                            /* new eval and alpha */
                            NewAlpha = BestScore > Alpha ? BestScore : Alpha;

                            /* update hash key */
#ifdef MCUMAX_HASHING_ENABLED
                            HashKeyLo += J(0);
                            HashKeyHi += J(8) + RookSqr - S;
#endif

                            int C = IterDepth - 1 - ((IterDepth > 5) & (PieceType > 2) & !Victim & !h);
                            /* extend 1 ply if in check */
                            C = (NonPawnMaterial > 29) | (IterDepth < 3) | (P - INF) ? C : IterDepth;

                            int s;
                            do
                                /* recursive eval. of reply */
                                s = (C > 2) | (Score > NewAlpha)
                                        ? -Search(-Beta, -NewAlpha, -Score, SkipSqr, 0, C)
                                        : Score; /* or fail low if futile */
                            while ((s > Alpha) & (++C < IterDepth));

                            Score = s;
                            /* move pending & in root: */
                            if (LastTo &&
                                (InputFrom - INF) &&
                                (Score + INF) &&
                                (FromSqr == InputFrom) & (ToSqr == InputTo))
                            {
                                RootEval = -Eval - i;

                                /* exit if legal & found */
                                RootEP = SkipSqr;

                                /* lock game in hash as draw */
#ifdef MCUMAX_HASHING_ENABLED
                                Hash->Draft = 99;
                                Hash->Score = 0;
#endif
                                NonPawnMaterial += i >> 7;

                                /* captured non-P material */
                                return Beta;
                            }

                            /* restore hash key */
#ifdef MCUMAX_HASHING_ENABLED
                            HashKeyLo = LocalHashKeyLo;
                            HashKeyHi = LocalHashKeyHi;
#endif

                            /* undo move,RookSqr can be dummy */
                            mcumax.board[RookSqr] = Side + 6;
                            mcumax.board[SkipSqr] = mcumax.board[ToSqr] = 0;
                            mcumax.board[FromSqr] = Piece;
                            mcumax.board[CaptSqr] = Victim;
                        }

                        if (Score > BestScore)
                        {
                            /* new best, update max,best */
                            BestScore = Score;
                            BestFrom = FromSqr;
                            /* mark double move with S */
                            BestTo = ToSqr | S & SkipSqr;
                        }

                        if (h)
                        {
                            /* redo after doing old best */
                            h = 0;
                            goto replay;
                        }

                        if ((FromSqr + StepVec - ToSqr) | (Piece & 0x20) |                                           /* not 1st step, moved before */
                            (PieceType > 2) & ((PieceType - 4) | (j - 7) ||                                          /* no P & no lateral K move, */
                                               mcumax.board[RookSqr = (FromSqr + 3) ^ (StepVec >> 1) & 7] - Side - 6 /* no virgin R in corner RookSqr, */
                                               || mcumax.board[RookSqr ^ 1] | mcumax.board[RookSqr ^ 2])             /* no 2 empty sq. next to R */
                        )
                            /* fake capt. for nonsliding */
                            Victim += PieceType < 5;
                        else
                            /* enable e.p. */
                            SkipSqr = ToSqr;

                        /* if not capt. continue ray */
                    } while (!Victim);
                }
            }
            /* next sqr. of board, wrap */
        } while ((FromSqr = (FromSqr + 9) & ~M) - StartSqr);

    cutoff:
        /* mate holds to any depth */
        if ((BestScore > (INF - M)) | (BestScore < (M - INF)))
            IterDepth = 98;

        /* best loses K: (stale)mate */
        BestScore = (BestScore + INF) | (P == INF) ? BestScore : 0;

        /* protect game history */
#ifdef MCUMAX_HASHING_ENABLED
        if (Hash->Draft < 99)
        {
            /* always store in hash tab */
            Hash->Key = HashKeyHi;
            Hash->Score = BestScore;
            Hash->Draft = IterDepth;

            /* move, type (bound/exact),*/
            Hash->From = BestFrom | 8 * (BestScore > Alpha) | S * (BestScore < Beta);
            Hash->To = BestTo;

            /* uncomment for Kibitz */
            /*if(LastTo)printf("%2d ply, %9d searched, score=%6d by %c%c%c%c\depth",IterDepth-1,Nodes-S,BestScore,
                'Hash'+(BestFrom&7),'8'-(BestFrom>>4),'Hash'+(BestTo&7),'8'-(BestTo>>4&7)); */
        }
#endif

        /* encoded in BestFrom S,8 bits */
    }

    /* change sides back */
    Side ^= 0x18;

    /* delayed-loss bonus */
    return BestScore += (BestScore < Eval);
}

/***************************************************************************/

// mcu-max wrapper

static void update_to_micromax(void)
{
    Side = mcumax.current_side ^ (MCUMAX_WHITE | MCUMAX_BLACK);
    RootEP = mcumax.en_passant_square;
}

static void update_from_micromax(void)
{
    mcumax.current_side = Side ^ (MCUMAX_WHITE | MCUMAX_BLACK);
    mcumax.en_passant_square = RootEP;
}

void mcumax_reset()
{
    for (int x = 0; x < 8; x++)
    {
        // Setup pieces (left side)
        mcumax.board[0x10 * 0 + x] = MCUMAX_BLACK | mcumax_board_setup[x];
        mcumax.board[0x10 * 1 + x] = MCUMAX_BLACK | MCUMAX_PAWN_DOWNSTREAM;
        for (int y = 2; y < 6; y++)
            mcumax.board[0x10 * y + x] = MCUMAX_EMPTY;
        mcumax.board[0x10 * 6 + x] = MCUMAX_WHITE | MCUMAX_PAWN_UPSTREAM;
        mcumax.board[0x10 * 7 + x] = MCUMAX_WHITE | mcumax_board_setup[x];

        // Setup weights (right side)
        for (int y = 0; y < 8; y++)
            mcumax.board[16 * y + x + 8] = (x - 4) * (x - 4) + (y - 4) * (y - 3);
    }

    mcumax.current_side = MCUMAX_WHITE;
    mcumax.en_passant_square = MCUMAX_INVALID;

    // To be removed:
    update_to_micromax();
    RootEval = 0;
    NonPawnMaterial = 0;

#ifdef MCUMAX_HASHING_ENABLED
    memset(HashTab, 0, sizeof(HashTab));

    srand(1);
    for (int i = sizeof(Zobrist); i > MCUMAX_BOARD_MASK; i--)
        Zobrist[i] = rand() >> 9;
#endif
}

static mcumax_square mcumax_set_piece(mcumax_square square, mcumax_piece piece)
{
    if (square & MCUMAX_BOARD_MASK)
        return square;

    mcumax.board[square] = piece ? piece | MCUMAX_MOVED : piece;

    return square + 1;
}

mcumax_piece mcumax_get_piece(mcumax_square square)
{
    if (square & MCUMAX_BOARD_MASK)
        return MCUMAX_EMPTY;

    return mcumax.board[square] & 0x1f;
}

void mcumax_set_fen_position(const char *fen_string)
{
    mcumax_reset();

    int field_index = 0;
    int board_index = 0;

    char c;
    while ((c = *fen_string++))
    {
        if (c == ' ')
        {
            if (field_index < 4)
                field_index++;

            continue;
        }

        switch (field_index)
        {
        case 0:
            if (board_index < 0x80)
            {
                switch (c)
                {
                case '8':
                    board_index = mcumax_set_piece(board_index, MCUMAX_EMPTY);

                case '7':
                    board_index = mcumax_set_piece(board_index, MCUMAX_EMPTY);

                case '6':
                    board_index = mcumax_set_piece(board_index, MCUMAX_EMPTY);

                case '5':
                    board_index = mcumax_set_piece(board_index, MCUMAX_EMPTY);

                case '4':
                    board_index = mcumax_set_piece(board_index, MCUMAX_EMPTY);

                case '3':
                    board_index = mcumax_set_piece(board_index, MCUMAX_EMPTY);

                case '2':
                    board_index = mcumax_set_piece(board_index, MCUMAX_EMPTY);

                case '1':
                    board_index = mcumax_set_piece(board_index, MCUMAX_EMPTY);
                    break;

                case 'P':
                    board_index = mcumax_set_piece(board_index, MCUMAX_PAWN_UPSTREAM | MCUMAX_WHITE);
                    break;

                case 'N':
                    board_index = mcumax_set_piece(board_index, MCUMAX_KNIGHT | MCUMAX_WHITE);
                    break;

                case 'B':
                    board_index = mcumax_set_piece(board_index, MCUMAX_BISHOP | MCUMAX_WHITE);
                    break;

                case 'R':
                    board_index = mcumax_set_piece(board_index, MCUMAX_ROOK | MCUMAX_WHITE);
                    break;

                case 'Q':
                    board_index = mcumax_set_piece(board_index, MCUMAX_QUEEN | MCUMAX_WHITE);
                    break;

                case 'K':
                    board_index = mcumax_set_piece(board_index, MCUMAX_KING | MCUMAX_WHITE);
                    break;

                case 'p':
                    board_index = mcumax_set_piece(board_index, MCUMAX_PAWN_DOWNSTREAM | MCUMAX_BLACK);
                    break;

                case 'n':
                    board_index = mcumax_set_piece(board_index, MCUMAX_KNIGHT | MCUMAX_BLACK);
                    break;

                case 'b':
                    board_index = mcumax_set_piece(board_index, MCUMAX_BISHOP | MCUMAX_BLACK);
                    break;

                case 'r':
                    board_index = mcumax_set_piece(board_index, MCUMAX_ROOK | MCUMAX_BLACK);
                    break;

                case 'q':
                    board_index = mcumax_set_piece(board_index, MCUMAX_QUEEN | MCUMAX_BLACK);
                    break;

                case 'k':
                    board_index = mcumax_set_piece(board_index, MCUMAX_KING | MCUMAX_BLACK);
                    break;

                case '/':
                    board_index = (board_index < 0x80) ? (board_index & 0xf0) + 0x10 : board_index;
                    break;
                }
            }
            break;

        case 1:
            switch (c)
            {
            case 'w':
                mcumax.current_side = MCUMAX_WHITE;
                break;

            case 'b':
                mcumax.current_side = MCUMAX_BLACK;
                break;
            }
            break;

        case 2:
            switch (c)
            {
            case 'K':
                mcumax.board[0x74] &= ~MCUMAX_MOVED;
                mcumax.board[0x77] &= ~MCUMAX_MOVED;
                break;

            case 'Q':
                mcumax.board[0x74] &= ~MCUMAX_MOVED;
                mcumax.board[0x70] &= ~MCUMAX_MOVED;
                break;

            case 'k':
                mcumax.board[0x04] &= ~MCUMAX_MOVED;
                mcumax.board[0x07] &= ~MCUMAX_MOVED;
                break;

            case 'q':
                mcumax.board[0x04] &= ~MCUMAX_MOVED;
                mcumax.board[0x00] &= ~MCUMAX_MOVED;
                break;
            }

            break;

        case 3:
            switch (c)
            {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
                mcumax.en_passant_square &= 0x7f;
                mcumax.en_passant_square |= (c - 'a');
                break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                mcumax.en_passant_square &= 0x7f;
                mcumax.en_passant_square |= 16 * ('8' - c);
                break;
            }

            break;
        }
    }

    // To be removed...
    update_to_micromax();
}

mcumax_piece mcumax_get_current_side(void)
{
    return mcumax.current_side;
}

void mcumax_set_callback(mcumax_callback callback, void *userdata)
{
    mcumax.user_callback = callback;
    mcumax.user_callback_userdata = userdata;
}

static bool mcumax_add_valid_move_callback(mcumax_move move)
{
    if (mcumax.valid_moves_num < mcumax.valid_moves_buffer_size)
        mcumax.valid_moves_buffer[mcumax.valid_moves_num++] = move;

    return false;
}

int mcumax_get_valid_moves(mcumax_move *valid_moves_buffer, int valid_moves_buffer_size)
{
    update_from_micromax();
    mcumax.move_callback = mcumax_add_valid_move_callback;

    mcumax.valid_moves_buffer_size = valid_moves_buffer_size;
    mcumax.valid_moves_buffer = valid_moves_buffer;
    mcumax.valid_moves_num = 0;

    mcumax_traverse_game_tree(1, -MCUMAX_MAX_VALUE, MCUMAX_MAX_VALUE);

    return mcumax.valid_moves_num;
}

void mcumax_get_best_move(int nodes_count_max, mcumax_move *move)
{
    return;
}

bool mcumax_play_best_move(int nodes_count_max, mcumax_move *move)
{
    mcumax.nodes_count_max = nodes_count_max;
    mcumax.nodes_count = 0;

    mcumax.valid_moves_buffer = NULL;

    mcumax.stop_search = false;

    InputFrom = INF;
    update_to_micromax();
    int Eval = Search(-INF, INF, RootEval, RootEP, 1, 3);
    if (!mcumax.stop_search)
    {
        update_from_micromax();

        if (Eval > (-INF + 1))
            *move = (mcumax_move){InputFrom, InputTo};
        else
            *move = (mcumax_move){MCUMAX_INVALID, MCUMAX_INVALID};
    }

    return !mcumax.stop_search;
}

void mcumax_stop_search(void)
{
    mcumax.stop_search = true;
}

bool mcumax_play_move(mcumax_move move)
{
    mcumax.nodes_count = 0;

    mcumax.valid_moves_buffer = NULL;

    mcumax.stop_search = false;

    InputFrom = move.from;
    InputTo = move.to;

    update_to_micromax();
    Search(-INF, INF, RootEval, RootEP, 1, 3);
    if (!mcumax.stop_search)
        update_from_micromax();

    return !mcumax.stop_search;
}
