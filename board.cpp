#include <tuple>
#include <string>
#include <array>
#include <list>
#include <map>
#include <vector>

using namespace std;

class BoardDirection{
    //Tuple for possible directions
    public:
        static constexpr tuple<int, int> UP = make_tuple(-1,0);
        static constexpr tuple<int, int> UP_LEFT = make_tuple(-1,-1);
        static constexpr tuple<int, int> UP_RIGHT = make_tuple(-1,1);
        static constexpr tuple<int, int> DOWN = make_tuple(1,0);
        static constexpr tuple<int, int> DOWN_LEFT = make_tuple(1,-1);
        static constexpr tuple<int, int> DOWN_RIGHT = make_tuple(1,1);
        static constexpr tuple<int, int> LEFT = make_tuple(0,-1);
        static constexpr tuple<int, int> RIGHT = make_tuple(0,1);

    BoardDirection(){
    };
};

tuple<int,int> interpret_coords(int row, char col){
    //tuple<int, int> test = make_tuple(8 - row, (int)col - (int)'A');
    //cout << "interp test: " << get<0>(test) << " " << get<1>(test) << " OLD:" << row << " " << col; 
    return make_tuple(8 - row, (int)col - (int)'A');
}

tuple<int,char> transform_coords(int row, int col){

    return make_tuple(8 - row, (char)(int('A') + col));
}

bool out_of_bounds(int row, int col){
    if(row < 0 || row > 7 || col < 0 || col > 7)
        return true;
    return false;
}

enum PieceColor{NONE, BLUE = 1, ORANGE = -1, TIE = 2};

const tuple<int,int> directions[8] = {BoardDirection::UP, BoardDirection::UP_LEFT, BoardDirection::UP_RIGHT, BoardDirection::DOWN, BoardDirection::DOWN_LEFT, BoardDirection::DOWN_RIGHT, BoardDirection::LEFT, BoardDirection::RIGHT};

class Board{
    public:
        const static int boardSize = 64;
        PieceColor board[boardSize];

    Board(){
        for(int i = 0; i < boardSize; i++) {
            board[i] = {PieceColor::NONE};
        }
        set_piece(5, 'D', PieceColor::ORANGE);
        set_piece(5, 'E', PieceColor::BLUE);
        set_piece(4, 'D', PieceColor::BLUE);
        set_piece(4, 'E', PieceColor::ORANGE);
    };

    public:
     Board clone() {
         Board newBoard = Board();
         for(int i= 0; i<Board::boardSize;i++){
             newBoard.board[i] = board[i];
         }
         return newBoard;
     }

    PieceColor _get_piece(int row, int col){
        // cout << "get piece returning: " << board[row*8 + col] << " which is i = " << (row*8+col) << "\n";
        return board[row*8 + col];
    }

    PieceColor get_piece(int row, char col){
        
        tuple<int,int> coords = interpret_coords(row,col);
        return _get_piece(get<0>(coords), get<1>(coords));
    }

    list<tuple<int,int>> _get_enveloped_pieces(int row, int col, PieceColor color){
        list<tuple<int,int>> enveloped = (list<tuple<int,int>>());
        // cout << "Coordinates passed into Get_enveloped_pieces: " << row << " " << col << "\n";
        
        for(tuple<int,int> off: directions){
            int row_off = get<0>(off), col_off = get<1>(off);
            int row_curr = row, col_curr = col;

            list<tuple<int,int>> potential_flip = (list<tuple<int,int>>());
            bool envelop = false;
            while(!out_of_bounds(row_curr + row_off, col_curr + col_off)){
                // cout << "Pos: " << row_curr << " " << col_curr << " checking: " << (row_curr + row_off) << " " << (col_curr + col_off) << "\n";
                row_curr += row_off;
                col_curr += col_off;

                PieceColor color_curr = _get_piece(row_curr,col_curr);
                // cout << "Piece color at " << row_curr << " " << col_curr << " is " << color_curr << "\n";

                // End condition where we hit a none piece, meaning not enveloped
                if (color_curr== PieceColor::NONE){
                    break;
                }
                // End condition where we hit a tile of same color
                else if(color_curr == color){
                    if(potential_flip.size()> 0){
                        // cout << "Found path with direction: (" << get<0>(off) << ", " << get<1>(off) << ") hit cell in " << row_curr << ", " << col_curr << endl;
                        envelop=true;
                    }
                    break;
                }
                // Middle condition where we hit tile of opposite color
                else{
                    potential_flip.push_front(make_tuple(row_curr,col_curr));
                }

            }  
            if (envelop) {
                enveloped.merge(potential_flip);
            }   
        }
        return enveloped;
    }

    bool _set_piece(int row, int col, PieceColor color){
        if(out_of_bounds(row,col)){
            return false;
        }
        if (board[row * 8 + col] != NONE){
            return false;
        }

        board[row * 8 + col] = color;

        list<tuple<int,int>> envelop = _get_enveloped_pieces(row, col, color);

        for (tuple<int,int> coords: envelop){
            board[get<0>(coords) * 8 + get<1>(coords)] = color;
        }

        if(envelop.size() == 0){
            return false;
        }
        return true;
    }

    

    bool set_piece(int row, char col, PieceColor color){
        tuple<int,int> coords = interpret_coords(row, col);
        return _set_piece(get<0>(coords), get<1>(coords), color);
    }

    bool is_full(){
        for(int i=0; i < boardSize; i++){
            if(board[i] != NONE){
                return false;
            }
        }

        return true;
    }

    map<PieceColor,int> get_counts(){
        int blue = 0, orange = 0;

        for (PieceColor color : board){
            if(color == BLUE){
                blue++;
            }
            else if (color == ORANGE){
                orange++;
            }
        }
        map<PieceColor,int> output = {{PieceColor::BLUE,blue}, {PieceColor::ORANGE,orange}}; //, {NONE, boardSize-(blue+orange)}
        return output;
    }

    bool has_valid_move(PieceColor color){
        for(int row = 0; row < 8; row++){
            for (int col =0; col < 8; col++){
                PieceColor piece = _get_piece(row, col);
                if(piece == NONE && _get_enveloped_pieces(row,col,color).size() > 0){
                    return true;
                }
            }
        }
        return false;
    }
    list<tuple<int,int>> find_all_valid_moves(PieceColor color){
        list<tuple<int,int>> moves = (list<tuple<int,int>>());; 

        for(int row = 0; row < 8; row++){
            for (int col = 0; col < 8; col++){
                PieceColor piece = _get_piece(row, col);


                if(piece == NONE && (_get_enveloped_pieces(row,col,color)).size() > 0) {
                    moves.push_front(make_tuple(row,col));
                }
            }
        }
        return moves;

    }
    string __str__(){
        string out = "";
        for(int i=0; i<8; i++){
            out+= to_string(8-i) + "  ";
            for(int j=0; j<8; j++){
                PieceColor color= _get_piece(i,j);
                if(color == ORANGE) {
                    out+="O";
                } else if(color == BLUE) {
                    out+="B";
                } else {
                    out += "*";
                }
               
                out+= "  ";
            }
            out += "\n";
        }
        out+= "   A  B  C  D  E  F  G  H\n";
        return out;
    }
};

