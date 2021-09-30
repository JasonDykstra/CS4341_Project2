#include "board.cpp"

using namespace std;
const int MINIMAX_DEPTH = 5;

PieceColor findWinner(int blueCount, int orangeCount)
{
    if (blueCount + orangeCount == Board::boardSize)
    {
        if ((blueCount - orangeCount) > 0)
        {
            return PieceColor::BLUE;
        }
        else if ((blueCount - orangeCount) < 0)
        {
            return PieceColor::ORANGE;
        }
        else
        {
            return PieceColor::TIE;
        }
    }
    return PieceColor::NONE;
}

PieceColor findWinner(Board board)
{
    map<PieceColor, int> counts = board.get_counts();
    int blueCount = counts.at(PieceColor::BLUE);
    int orangeCount = counts.at(PieceColor::ORANGE);

    return findWinner(blueCount, orangeCount);
}

double mobilityHeuristic(Board board, PieceColor maximizingColor)
{
    int maximizingColorMoves = (board.findAllValidMoves(maximizingColor)).size();
    int minimizingColorMoves = (board.findAllValidMoves(static_cast<PieceColor>(-(int)maximizingColor))).size();
    if (maximizingColorMoves + minimizingColorMoves != 0)
    {
        int mobilityScore = 100 * (maximizingColorMoves - minimizingColorMoves) / (maximizingColorMoves + minimizingColorMoves);
        return mobilityScore;
    }
    return 0;
}

double weightedBoardHeuristic(Board board, PieceColor color)
{
    int weightedBoard[64] = {
        /*
        X squares - B2, B7, G2, and G7. Placing your disk there early almost certainly gives away the adjacent corner, so your heuristic should give them high negative weight, at least in the first 40 moves
        C squares - A2, A7, B1, G1, H2, H7, B8, and G8. They offer the opponent access to corners, so their value should be different from that of other squares, at least when the edge has fewer than five disks
        */
        4, -3, 2, 2, 2, 2, -3, 4,
        -3, -4, -1, -1, -1, -1, -4, -3,
        2, -1, 1, 0, 0, 1, -1, 2,
        2, -1, 0, 1, 1, 0, -1, 2,
        2, -1, 0, 1, 1, 0, -1, 2,
        2, -1, 1, 0, 0, 1, -1, 2,
        -3, -4, -1, -1, -1, -1, -4, -3,
        4, -3, 2, 2, 2, 2, -3, 4};

    // Multiply our tiles by the heuristic of the board space, sum them up, subtract the opponent's score
    double weight = 0;
    for (int i = 0; i < Board::boardSize; i++)
    {
        if (board.board[i] == color)
        {
            weight += weightedBoard[i];
        }
        else
        {
            weight -= weightedBoard[i];
        }
    }

    // Return weight, higher score - better
    return weight;
}

int quickHeuristc(Board board, PieceColor pieceColor)
{
    map<PieceColor, int> counts = board.get_counts();
    int blueCount = counts.at(PieceColor::BLUE);
    int orangeCount = counts.at(PieceColor::ORANGE);

    PieceColor winner = findWinner(blueCount, orangeCount);

    //if game not over
    if (winner == PieceColor::NONE)
    {
        if (orangeCount > blueCount)
        {
            return pieceColor == PieceColor::ORANGE ? 1 : -1;
        }
        else if (blueCount > orangeCount)
        {
            return pieceColor == PieceColor::ORANGE ? -1 : 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        //game over
        if (winner == pieceColor)
        {
            return INT_MAX;
        }
        else if (winner == PieceColor::TIE)
        {
            return INT_MIN + 1;
        }
        else
        {
            return INT_MIN;
        }
    }
}

tuple<int, int, int> minimax(Board board, PieceColor currentColor, int depth, tuple<int, int> lastMove, bool isMaximizingPlayer, double alpha, double beta)
{

    if (findWinner(board) == currentColor)
    {
        return make_tuple(get<0>(lastMove), get<1>(lastMove), INT_MAX);
    }
    else if (findWinner(board) == -currentColor)
    {
        return make_tuple(get<0>(lastMove), get<1>(lastMove), INT_MIN);
    }

    if (isMaximizingPlayer)
    {
        if (depth == 0)
        {
            return make_tuple(get<0>(lastMove), get<1>(lastMove), quickHeuristc(board, currentColor));
        }

        tuple<int, int, int> bestVal = make_tuple(-1, -1, INT_MIN);
        bool hasNoMoves = true;
        for (tuple<int, int> move : board.findAllValidMoves(currentColor))
        {
            hasNoMoves = false;
            if (board._set_piece(get<0>(move), get<1>(move), currentColor))
            {
                tuple<int, int, int> value = minimax(board, static_cast<PieceColor>(-(int)currentColor), depth - 1, move, false, alpha, beta);
                if (get<2>(bestVal) > get<2>(value))
                {
                    bestVal = value;
                    alpha = max(alpha, (double)(get<2>(bestVal)));
                    if (beta <= alpha)
                        break;
                }
            }
        }
        if (hasNoMoves)
        {
            minimax(board, (PieceColor)(-(int)currentColor), depth - 1, lastMove, false, alpha, beta); //TODO: Comeback
        }
        return tuple<int, int, int>(get<0>(lastMove), get<1>(lastMove), get<2>(bestVal));
    }
    else
    {
        if (depth == 0)
        {
            return make_tuple(get<0>(lastMove), get<1>(lastMove), quickHeuristc(board, currentColor));
        }

        tuple<int, int, int> bestVal = make_tuple(-1, -1, INT_MAX);
        bool hasNoMoves = true;
        for (tuple<int, int> move : board.findAllValidMoves(currentColor))
        {
            hasNoMoves = false;
            if (board._set_piece(get<0>(move), get<1>(move), currentColor))
            {
                tuple<int, int, int> value = minimax(board, static_cast<PieceColor>(-(int)currentColor), depth - 1, move, true, alpha, beta);
                if (get<2>(bestVal) > get<2>(value))
                {
                    bestVal = value;
                    beta = min(beta, (double)(get<2>(bestVal)));
                    if (beta <= alpha)
                        break;
                }
            }
        }
        if (hasNoMoves)
        {
            return minimax(board, (PieceColor)(-(int)currentColor), depth - 1, lastMove, false, alpha, beta); //TODO: Come back
        }
        return tuple<int, int, int>(get<0>(lastMove), get<1>(lastMove), get<2>(bestVal));
    }
}

string getBestMove(Board board, PieceColor agentColor)
{
    tuple<int, int, int> move = minimax(board, agentColor, 10, make_tuple(-1, -1), true, 0, 0);
    int row = get<0>(move);
    int column = get<1>(move);

    return (char)(row + 65) + " " + column;
}
