#include "board.cpp";
#include "main.cpp";

using namespace std;

PieceColor findWinner(int blueCount, int orangeCount) {
    if(blueCount + orangeCount == Board::boardSize){
        if ((blueCount-orangeCount) > 0){
            return PieceColor::BLUE;
        }
        else if ((blueCount-orangeCount) < 0){
            return PieceColor::ORANGE;
        }
        else{
            return PieceColor::TIE;
        }
    }
    return PieceColor::NONE;
}


PieceColor findWinner(Board board){
    map<PieceColor,int> counts = board.get_counts();
    int blueCount = counts.at(PieceColor::BLUE);
    int orangeCount = counts.at(PieceColor::ORANGE);
    
    return findWinner(blueCount, orangeCount);
}

double mobilityHeuristic(Board board, PieceColor maximizingColor){
    int maximizingColorMoves = size(board.findAllValidMoves(maximizingColor));
    int minimizingColorMoves = size(board.findAllValidMoves(static_cast<PieceColor>(-(int)maximizingColor)));
    if(maximizingColorMoves + minimizingColorMoves != 0){
        int mobilityScore = 100 * (maximizingColorMoves - minimizingColorMoves) / (maximizingColorMoves + minimizingColorMoves);
        return mobilityScore;
    }
    return 0;
}
double cornersCapturedHeuristic(Board board, PieceColor color){
    //Count corners, then do:
    /*
    if ( Max Player Corners + Min Player Corners != 0)
	    Corner Heuristic Value = 100 * (Max Player Corners - Min Player Corners) / (Max Player Corners + Min Player Corners)
    else
	    Corner Heuristic Value = 0

    */
}
double weightedBoardHeuristic(Board board, PieceColor color){
    int weightedBoard[64] = {
        /*
        X squares - B2, B7, G2, and G7. Placing your disk there early almost certainly gives away the adjacent corner, so your heuristic should give them high negative weight, at least in the first 40 moves
        C squares - A2, A7, B1, G1, H2, H7, B8, and G8. They offer the opponent access to corners, so their value should be different from that of other squares, at least when the edge has fewer than five disks
        */
    };
}

int quickHeuristc(Board board) {
    map<PieceColor,int> counts = board.get_counts();
    int blueCount = counts.at(PieceColor::BLUE);
    int orangeCount = counts.at(PieceColor::ORANGE);

    PieceColor winner = findWinner(blueCount, orangeCount);

    //if game not over
    if(winner == PieceColor::NONE) {
        if(orangeCount > blueCount) {
            return pieceColor == PieceColor::ORANGE ? 1 : -1;
        } else if(blueCount > orangeCount){
            return pieceColor == PieceColor::ORANGE ? -1 : 1;
        } else {
            return 0;
        }

    } else {
        //game over
        if(winner == pieceColor) {
            return INT_MAX;
        } else if(winner == PieceColor::TIE) {
            return INT_MIN + 1;
        } else {
            return INT_MIN;
        }
    }
}

tuple<int,int,int> minimax(Board board, PieceColor currentColor, int depth, tuple<int,int> lastMove, bool isMaximizingPlayer, double alpha, double beta){

    if(findWinner(board) == currentColor){
        return tuple(get<0>(lastMove), get<1>(lastMove), INT_MAX);
    }
    else if (findWinner(board) == -currentColor){
        return tuple(get<0>(lastMove), get<1>(lastMove), INT_MIN);
    }

    if(isMaximizingPlayer){
        if(depth == 0){
            return tuple(get<0>(lastMove), get<1>(lastMove), quickHeuristc(board));
        }

        tuple<int, int, int> bestVal = tuple(-1,-1,INT_MIN);
        bool hasNoMoves = true;
        for (tuple<int,int> move: board.findAllValidMoves(currentColor)){
            hasNoMoves = false;
            if(board._set_piece(get<0>(move),get<1>(move), currentColor)){
                tuple<int, int, int> value = minimax(board, static_cast<PieceColor>(-(int)currentColor), depth-1, move, false, alpha, beta);
                if(get<2>(bestVal) > get<2>(value)){
                    bestVal = value;
                    alpha = max(alpha, get<2>(bestVal));
                    if (beta <= alpha) break;
                }
            }
        }
        if(hasNoMoves){
            minimax(board, (PieceColor)(-(int)currentColor), depth-1, lastMove, false, alpha, beta); //TODO: Comeback
        }
        return tuple<int,int,int>(get<0>(lastMove),get<1>(lastMove),get<2>(bestVal));
    }
    else {
        if(depth == 0){
            return tuple(get<0>(lastMove), get<1>(lastMove), quickHeuristc(board));
        }

        tuple<int, int, int> bestVal = tuple(-1,-1,INT_MAX);
        bool hasNoMoves = true;
        for (tuple<int,int> move: board.findAllValidMoves(currentColor)){
            hasNoMoves = false;
            if(board._set_piece(get<0>(move),get<1>(move), currentColor)){
                tuple<int, int, int> value = minimax(board, static_cast<PieceColor>(-(int)currentColor), depth-1, move, true, alpha, beta);
                if(get<2>(bestVal) > get<2>(value)){
                    bestVal = value;
                    beta = min(beta, get<2>(bestVal));
                    if (beta <= alpha) break;
                }
            }
        }
        if(hasNoMoves){
            return minimax(board, (PieceColor)(-(int)currentColor), depth-1, lastMove, false, alpha, beta); //TODO: Come back
        }
        return tuple<int,int,int>(get<0>(lastMove),get<1>(lastMove), get<2>(bestVal));
    }


}