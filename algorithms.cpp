#include "board.cpp"
#include <chrono>

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
    int maximizingColorMoves = (board.find_all_valid_moves(maximizingColor)).size();
    int minimizingColorMoves = (board.find_all_valid_moves(static_cast<PieceColor>(-(int)maximizingColor))).size();
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

int minimax(Board board, bool isMaximizingPlayer, int depth, PieceColor maxPC, PieceColor minPC, int alpha, int beta) {

    //END GAME CHECK
    PieceColor winner = findWinner(board);
    if(winner != PieceColor::NONE) {
        if(winner == maxPC) {
            return INT_MAX;
        } else if(winner == minPC) {
            return INT_MIN;
        } else {
            return INT_MIN + 1;
        }
    }

    //Bottom of Board CHECK
    if(depth == 0) {
        if(isMaximizingPlayer) {
            return quickHeuristc(board, maxPC);
        } else {
            return -quickHeuristc(board, minPC);
        }
    }

    PieceColor workingPiece = isMaximizingPlayer ? maxPC : minPC;

    //(row, column), value
    list<tuple<int,int>> allMoves = board.find_all_valid_moves(workingPiece);

    //going down the tree
    if(isMaximizingPlayer) {
        int bestHeuristic = INT_MIN;

        Board copiedBoard;
        if(allMoves.size() == 0) {
            //If player has to pass
            copiedBoard = board.clone();
            bestHeuristic = minimax(copiedBoard, !isMaximizingPlayer, (depth -1), maxPC, minPC, alpha, beta);
        } else {
            for(tuple<int, int> move : board.find_all_valid_moves(workingPiece)) {
                copiedBoard = board.clone();
                copiedBoard._set_piece(get<0>(move), get<1>(move), workingPiece);
                bestHeuristic = max(bestHeuristic, minimax(copiedBoard, !isMaximizingPlayer, (depth - 1), maxPC, minPC, alpha, beta));
                alpha = max(alpha, bestHeuristic);
                if(beta <= alpha){
                    break;
                }
            }
        }
        return bestHeuristic;
    } else {
        int worstHeuristic = INT_MAX;

        Board copiedBoard;
        if(allMoves.size() == 0) {
            //If player has to pass
            copiedBoard = board.clone();
            worstHeuristic = minimax(copiedBoard, !isMaximizingPlayer, (depth -1), maxPC, minPC, alpha, beta);
        } else {
            for(tuple<int, int> move : board.find_all_valid_moves(workingPiece)) {
                copiedBoard = board.clone();
                copiedBoard._set_piece(get<0>(move), get<1>(move), workingPiece);
                worstHeuristic = min(worstHeuristic, minimax(copiedBoard, !isMaximizingPlayer, (depth - 1), maxPC, minPC, alpha, beta));
                beta = min(beta, worstHeuristic);
                if(beta <= alpha){
                    break;
                }
            }
            return worstHeuristic;
        }
    }
}

string getMoveMiniMax(Board* board, PieceColor agentPC, PieceColor oppPC) {
    auto started = chrono::high_resolution_clock::now();
    
    list<tuple<int,int>> allMoves = (*board).find_all_valid_moves(agentPC);

    //No move avaible
    if(allMoves.size() == 0) {
        return "PASS";
    } else {
        cout << "MOVE FOUND\n";
    }

    //determing our best move
    tuple<int, int> bestMove;
    int bestHeuristic = INT_MIN;
    for(tuple<int, int> move : allMoves) {
         tuple<int, char> transformedCoords = transform_coords(get<0>(move), get<1>(move));
        cout << "Move: " << get<1>(transformedCoords) << " " << get<0>(transformedCoords) << "\n";
    }
    
    for(tuple<int, int> move : allMoves) {
        Board copiedBoard = (*board).clone();
        copiedBoard._set_piece(get<0>(move), get<1>(move), agentPC);
        int tempHeuristic = minimax(copiedBoard, false, MINIMAX_DEPTH, agentPC, oppPC, INT_MIN, INT_MAX);

        if(tempHeuristic >= bestHeuristic) {
            bestHeuristic = tempHeuristic;
            bestMove = move;

            //win state found
            if(bestHeuristic == INT_MAX)
                break;
        }
    }

        int row = get<0>(bestMove);
        int column = get<1>(bestMove);
        
        //update board
        (*board)._set_piece(row, column, agentPC);
        
        auto done = std::chrono::high_resolution_clock::now();

        tuple<int, char> boardCoords= transform_coords(row, column);
        cout << "BEST MOVE RETURN: " << get<1>(boardCoords) << " " << get<0>(boardCoords) << "\n";

        std::cout << "Agent took: " << chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << "ms\n";
        return get<1>(boardCoords) + " " + get<0>(boardCoords);
}
