#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "algorithms.cpp"
#include <windows.h>
#include <sys/stat.h>
#include <sstream>

using namespace std;

const string MOVE_FILE_NAME = "move_file";
const string END_GAME_FILE_NAME = "end_game";
const string TEAM_NAME = "Got";
PieceColor pieceColor = PieceColor::NONE;
PieceColor oppPieceColor = PieceColor::NONE;

string getOpponentMove()
{
    //open file in read mode
    ifstream moveFile(MOVE_FILE_NAME);
    //Check file is opened?

    //read file line
    string opponentMove = "";
    getline(moveFile, opponentMove);

    moveFile.close();
    return opponentMove;
}

//moveCoordinates -> e.x "E 1"
void writeMoveToFile(string moveCoordinates) //NEED TO FIX
{
    //Open and Clear File
    ofstream moveFile(MOVE_FILE_NAME, ios::out | ios::trunc);

    //write to file and close
    moveFile << TEAM_NAME + " " + moveCoordinates;
    moveFile.close();
    //cout << "wrote move: " << TEAM_NAME + " " + moveCoordinates << endl;

}

inline bool fileExists(const string &fileName)
{
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}

// Tuple: <team name, column, row>
tuple<string, string, string> parseMoveLine(string line)
{
    tuple<string, string, string> ans = tuple<string,string,string>();

    // Find index of first space
    int first_space = line.find(" ");

    // Set first element of tuple to the team name (string before the space)
    get<0>(ans) = line.substr(0, first_space);
    
    // Get index of second space
    int second_space = line.find(" ", first_space + 1);
    // Set second element of tuple to column
    get<1>(ans) = line.substr(first_space + 1, 1);
    // Set last element of tuple to row
    get<2>(ans) = line.substr(second_space + 1, 1);

    return ans;
    
}

bool makeOppMove(Board* board, string move)
{
    // Get the parsed move line
    tuple<string, string, string> parsedMove = (parseMoveLine(move));

    if(get<0>(parsedMove) == TEAM_NAME)
        return true;
    // get the string representations of row and col from the parsed move tuple
    string str_row = get<2>(parsedMove);
    string str_column = get<1>(parsedMove);

    // Convert row and col into proper types to be passed into interpret coords
    int row = stoi(str_row);
    char col;
    // Set char to first entry in column (e.g. "C")
    col = str_column[0];

    // Call interpret coords to translate "A 5" into (0, 4) for example

    // Make the move
    if(col != 'P')
        (*board).set_piece(row, col, oppPieceColor);
    //cout << "Move made!" << endl;
    return false;
}

void gameLoop()
{
    Board board = Board();
    while (!fileExists(END_GAME_FILE_NAME))
    {
        if (fileExists(TEAM_NAME + ".go"))
        { 
            //cout << "teamname.go file found!!" << endl;

            //Update board with opponentMove
            string opponentMove = getOpponentMove();
            //cout << "OPP MADE MOVE: " << opponentMove << endl;
            if (pieceColor == PieceColor::NONE)
            {
                if(opponentMove == "") {
                    pieceColor = PieceColor::BLUE;
                    oppPieceColor = PieceColor::ORANGE;
                } else {
                    pieceColor = PieceColor::ORANGE;
                    oppPieceColor = PieceColor::BLUE;
                }
            }
            
            bool cancelMove = false;
            if (opponentMove != "")
            {
                cancelMove = makeOppMove(&board, opponentMove);
            }

            if(!cancelMove) {
               // cout << board.__str__() << endl;
                //set piece color if nonexistant
                

                //!GET OUR AGENT'S MOVE
                // PROBLEM IS THAT REFEREE DOESNT SEE OUR MOVE WHEN WE WRITE A PASS FOR WHATEVER REASON
                // WE NEED TO FIND OUT WHY PASSES ARE WRITING TO FILE DIFFERENTLY OR SLOWER?
                string agentMove = getMoveMiniMax(&board, pieceColor, oppPieceColor);
                //cout << "Made Move: " << agentMove << endl;
                writeMoveToFile(agentMove);
                //cout << board.__str__() << endl;

                sleep(1);
                
            }
        }
        sleep(.01);
    }
    
    cout << "GAME ENDED!" << "\n";
    sleep(100000);
}

void testGameLoop()
{
    string c;
    cout << "Choose Color(O/B): ";
    cin >> c;
    if(c == "O") {
        cout << "Player Color: ORANGE\n";
        pieceColor = PieceColor::BLUE;
        oppPieceColor = PieceColor::ORANGE;
    } else {
        pieceColor = PieceColor::ORANGE;
        oppPieceColor = PieceColor::BLUE;
    }

    Board board = Board();

    cout << board.__str__() << "\n";

    bool agentTurn = pieceColor == PieceColor::BLUE;

    while(findWinner(board) == PieceColor::NONE) {
        if(agentTurn) {
            cout << "AI MAKING MOVE...\n";
            string bestMove = getMoveMiniMax(&board, pieceColor, oppPieceColor);

            agentTurn = false;
            cout << board.__str__();
        } else {
            cout << "Enter Move(\"COL ROW\"): ";
            string move;
            getline(cin >> ws, move);
            
            cout << move << endl;

            makeOppMove(&board, "us " + move);
            
            agentTurn = true;
            cout << board.__str__();
        }
        cout << "-------------" << "\n";
        Sleep(1);
    }

    cout << "Winner found, exiting loop" << endl;
}

int main()
{   
    cout << "AI: " << TEAM_NAME << " Starting up..." << endl;
    //testGameLoop();
    gameLoop();
}
