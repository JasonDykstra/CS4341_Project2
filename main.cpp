
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

enum PieceColor{NONE, BLUE, ORANGE};


const string MOVE_FILE_NAME = "move_file.txt";
const string END_GAME_FILE_NAME = "end_game";
const string TEAM_NAME = "MountainGoats";
PieceColor pieceColor = PieceColor::NONE;

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


void gameLoop() {

    while(!fileExists(END_GAME_FILE_NAME)) {
        if(fileExists(TEAM_NAME + ".go")) {
            string opponentMove = getOpponentMove();

            //!UPDATE BOARD WITH OPPONENT MOVE

            //set piece color if nonexistant
            if(pieceColor == PieceColor::NONE) {
                pieceColor = (opponentMove == "") ? PieceColor::BLUE : PieceColor::ORANGE;
            }

            //!GET OUR AGENT'S MOVE
            string agentMove = "E 4";

            writeMoveToFile(agentMove);
        }
        sleep(1);
    }

    cout << "GAME ENDED!" << "\n";
}

int main(){
    gameLoop();
}
