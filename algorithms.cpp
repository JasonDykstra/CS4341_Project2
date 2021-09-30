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

tuple<int,int, int> minimax(Board board, PieceColor currentColor, int depth, tuple<int,int> lastMove, bool isMaximizingPlayer){

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
        for (tuple<int,int> move: findAllValidMoves(currentColor)){
            hasNoMoves = false;
            tuple<int, int, int> value = minimax(board, -currentColor, depth-1, move, false);
            if(get<2>(bestVal) > get<2>(value)){
                bestVal = value;
            }
        }
        if(hasNoMoves){
            minimax(board, -currentColor, depth-1, lastMove, false); //TODO: Come back
        }
        return tuple<int,int,int>(get<0>(lastMove),get<1>(lastMove),get<2>(bestVal));
    }
    else {
        if(depth == 0){
            return tuple(get<0>(lastMove), get<1>(lastMove), quickHeuristc(board))
        }

        tuple<int, int, int> bestVal = tuple(-1,-1,INT_MAX);
        bool hasNoMoves = true;
        for (tuple<int,int> move: findAllValidMoves(currentColor)){
            hasNoMoves = false;
            tuple<int, int, int> value = minimax(board, -currentColor, depth-1, move, true);
            if(get<2>(bestVal) > get<2>(value)){
                bestVal = value;
            }
        }
        if(hasNoMoves){
            return minimax(board, -currentColor, depth-1, lastMove, false); //TODO: Come back
        }
        return tuple<int,int,int>(get<0>(lastMove),get<1>(lastMove),get<2>(value));
    }


}