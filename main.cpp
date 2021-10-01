#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include "algorithms.cpp"
// #include <windows.h>
#include <sys/stat.h>
#include <sstream>

using namespace std;

const string MOVE_FILE_NAME = "move_file.txt";
const string END_GAME_FILE_NAME = "end_game";
const string TEAM_NAME = "MountainGoats";
PieceColor pieceColor = PieceColor::NONE;
PieceColor oppPieceColor = PieceColor::NONE;

string getOpponentMove()
{
    //open file in read mode
    ifstream moveFile(MOVE_FILE_NAME);
    //Check file is opened?

    //read file line
    string opponentMove;
    getline(moveFile, opponentMove);

    moveFile.close();
    return opponentMove;
}

//moveCoordinates -> e.x "E 1"
void writeMoveToFile(string moveCoordinates)
{
    //Open and Clear File
    ofstream moveFile(MOVE_FILE_NAME, ios::out | ios::trunc);

    //write to file and close
    moveFile << TEAM_NAME + " " + moveCoordinates;
    moveFile.close();
}

inline bool fileExists(const string &fileName)
{
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}

// Tuple: <team name, column, row>
tuple<string, string, string> parseMoveLine(string line)
{
    printf("line: %s", line);
    tuple<string, string, string> ans = tuple<string,string,string>();

    // Find index of first space
    int first_space = line.find(" ");

    // Set first element of tuple to the team name (string before the space)
    get<0>(ans) = line.substr(0, first_space);
    // Get index of second space
    int second_space = line.find(" ", first_space + 1);
    // Set second element of tuple to column
    get<1>(ans) = line.substr(first_space + 1, second_space);
    // Set last element of tuple to row
    get<2>(ans) = line.substr(second_space + 1, line.length());

    return ans;
    
}

void makeOppMove(Board* board, string move)
{
    cout << "In make opp move." << endl;
    // Get the parsed move line
    printf(" move: %s", move);
    tuple<string, string, string> parsedMove = (parseMoveLine(move));
    printf("tuple: %s, %s, %s",  get<0>(parsedMove), get<1>(parsedMove), get<2>(parsedMove));
    // get the string representations of row and col from the parsed move tuple
    string str_row = get<2>(parsedMove);
    string str_column = get<1>(parsedMove);
    // Convert the string representations into int and char respectrively to pass into interpret coords function
    // int row = stoi(str_row);
    // char col = str_column.at(0);

    // // Get the interpreted coords as ints in internal graph as 0-7 to manipulate array
    // tuple<int, int> coords = interpret_coords(row, col);
    

    // (*board).set_piece(get<0>(coords), get<1>(coords), oppPieceColor);
}

void gameLoop()
{
    Board board;
    while (!fileExists(END_GAME_FILE_NAME))
    {
        if (fileExists(TEAM_NAME + ".go"))
        {

            //Update board with opponentMove
            string opponentMove = getOpponentMove();
            
            if (opponentMove != "")
            {
                makeOppMove(&board, opponentMove);
            }

            //set piece color if nonexistant
            if (pieceColor == PieceColor::NONE)
            {
                pieceColor = (opponentMove == "") ? PieceColor::BLUE : PieceColor::ORANGE;
                oppPieceColor = (opponentMove == "") ? PieceColor::ORANGE : PieceColor::BLUE;
            }

            //!GET OUR AGENT'S MOVE
            string agentMove = getMoveMiniMax(&board, pieceColor, oppPieceColor);

            writeMoveToFile(agentMove);
            sleep(50);
        }
        sleep(1);
    }

    cout << "GAME ENDED!"
         << "\n";
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
            cout << "Enter Move: ";
            string move;
            cin >> move;
            printf("move from testgameloop: %s", move);
            makeOppMove(&board, "us " + move);
            agentTurn = true;
            cout << board.__str__();
        }
        cout << "-------------" << "\n";
        sleep(1);
    }
}

int main()
{
    testGameLoop();
    //gameLoop();
}
