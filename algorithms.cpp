#include "Board.cpp";
using namespace std;



PieceColor findWinner(Board board){
    map<PieceColor,int> counts = board.get_counts();
    int blueCount = counts.at(PieceColor::BLUE);
    int orangeCount = counts.at(PieceColor::ORANGE);
    if(blueCount + orangeCount == Board::boardSize){
        if ((blueCount-orangeCount) > 0){
            return PieceColor::BLUE;
        }
        else if ((blueCount-orangeCount) < 0){
            return PieceColor::ORANGE;
        }
        else{
            return PieceColor::NONE;
        }
}