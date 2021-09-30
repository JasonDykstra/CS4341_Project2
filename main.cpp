#include <iostream>;
#include <fstream>;
#include <string>;
//#include <unistd.h>;
#include "board.cpp";
#include <windows.h>;
#include <sys/stat.h>;
#include <sstream>;

using namespace std;

const string MOVE_FILE_NAME = "move_file.txt";
const string END_GAME_FILE_NAME = "end_game";
const string TEAM_NAME = "MountainGoats";
PieceColor pieceColor = PieceColor::NONE;
PieceColor oppPieceColor = PieceColor::NONE;

string getOpponentMove() {
    //open file in read mode
    ifstream moveFile (MOVE_FILE_NAME);
    //Check file is opened?

    //read file line
    string opponentMove;
    getline(moveFile, opponentMove);

    moveFile.close();
    return opponentMove;
}

//moveCoordinates -> e.x "E 1"
void writeMoveToFile(string moveCoordinates) {
    //Open and Clear File
    ofstream moveFile (MOVE_FILE_NAME, ios::out | ios:: trunc);

    //write to file and close
    moveFile << TEAM_NAME + " " + moveCoordinates;
    moveFile.close();
}

inline bool fileExists (const string& fileName) {
  struct stat buffer;
  return (stat (fileName.c_str(), &buffer) == 0);
}

string* parseMoveLine(string line) {
    string parsedMove[3];
    int i = 0;

    stringstream ss(line);
    while(ss.good() && i<3) {
        ss >> parsedMove[i];
        ++i;
    }
    return parsedMove;
}


void makeOppMove(Board board, string move) {
    string* parsedMove = parseMoveLine(move);
    int row = (int) move[1] - 65;
    int column = move[2];

    board._set_piece(row, column, oppPieceColor);
}


void gameLoop() {
    Board board;
    while(!fileExists(END_GAME_FILE_NAME)) {
        if(fileExists(TEAM_NAME + ".go")) {

            //Update board with opponentMove
            string opponentMove = getOpponentMove();
            if(opponentMove != "") {
                makeOppMove(board, opponentMove);
            }

            //set piece color if nonexistant
            if(pieceColor == PieceColor::NONE) {
                pieceColor = (opponentMove == "") ? PieceColor::BLUE : PieceColor::ORANGE;
                oppPieceColor = (opponentMove == "") ? PieceColor::ORANGE : PieceColor::BLUE;
            }

            //!GET OUR AGENT'S MOVE
            string agentMove = getBestMove(board);

            writeMoveToFile(agentMove);
            Sleep(50);
        }
        Sleep(1);
    }

    cout << "GAME ENDED!" << "\n";
}

void testGameLoop() {
    string c;
    cin >> c;
}

int main(){
    testGameLoop();
    //gameLoop();
}
