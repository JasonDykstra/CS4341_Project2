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
    if(winner == PieceColor::none) {
        if(orangeCount > blueCount) {
            return pieceColor == PieceColor.orange; ? 1 : -1;
        } else if(blueCount > orangeCount){
            return pieceColor == PieceColor.orange; ? -1 : 1;
        } else {
            return 0;
        }

    } else {
        //game over
        if(winner == pieceColor) {
            return 5;
        } else if(winner == PieceColor::TIE) {
            return -INT.IN;
        } else {
            return -5;
        }
    }
}

tuple<int,int> minimax(int depth, Board board, bool isMax, int scores[], int h){
    

}