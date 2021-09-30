#include <tuple>;
#include <string>;
#include <array>;
#include <list>;
#include <map>;

using namespace std;

class Direction{
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

    Direction(){
    };
};

tuple<int,int> interpret_coords(int row, char col){
    return make_tuple(8 - row, (int)col - (int)'A');
}

tuple<int,int> transform_coords(int row, char col){
    return make_tuple(8 - row, int('A') + col);
}
bool out_of_bounds(int row, int col){
    if(row < 0 || row > 7 || col < 0 || col > 7)
        return true;
    return false;
}

enum PieceColor{NONE, BLUE = 1, ORANGE = -1, TIE = 2};
namespace Direction{
    const tuple<int,int> directions[8] = {UP, UP_LEFT, UP_RIGHT, DOWN, DOWN_LEFT,DOWN_RIGHT,LEFT, RIGHT};
}
class Board{
    public:
        const static int boardSize = 64;
        PieceColor board[boardSize];

    Board(){
        board[boardSize] = {PieceColor::NONE};
        set_piece(5, 'D', PieceColor::BLUE);
        set_piece(5, 'E', PieceColor::ORANGE);
        set_piece(4, 'D', PieceColor::ORANGE);
        set_piece(4, 'E', PieceColor::BLUE);
    };
    public:
    PieceColor _get_piece(int row, int col){
        return board[row*8 + col];
    }

    PieceColor get_piece(int row, char col){
        
        tuple<int,int> coords = interpret_coords(row,col);
        return _get_piece(get<0>(coords), get<1>(coords));
    }

    list<tuple<int,int>> _get_enveloped_pieces(int row, int col, PieceColor color){
        list<tuple<int,int>> enveloped = (list<tuple<int,int>>());

        for(tuple<int,int> off: Direction::directions){
            int row_off = get<0>(off), col_off = get<1>(off);
            int row_curr = row, col_curr = col;

            list<tuple<int,int>> potential_flip = (list<tuple<int,int>>());
            bool envelop = false;
            while(!out_of_bounds(row_curr + row_off, col_curr+col_off)){
                row_curr += row_off;
                col_curr += col_off;

                PieceColor color_curr = _get_piece(row_curr,col_curr);

                if (color_curr== NONE){
                    break;
                }
                else if(color_curr == color){
                    if(size(potential_flip)> 0){
                        envelop=true;
                        break;
                    }
                }
                else{
                    potential_flip.push_front(tuple<int,int>(row_curr,col_curr));
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

        board[row*8 + col] = color;

        list<tuple<int,int>> envelop = _get_enveloped_pieces(row, col, color);

        for (tuple<int,int> coords: envelop){
            board[get<0>(coords) * 8 + get<1>(coords)] = color;
        }

        if(size(envelop) == 0){
            return false;
        }
        return true;
    }

    

    bool set_piece(int row, char col, PieceColor color){
        tuple<int,int> coords = interpret_coords(row, col);
        return _set_piece(get<0>(coords), get<1>(coords), color);
    }

    bool is_full(){
        int count = 0;

        for(int i=0; i < boardSize; i++){
            if(board[i]== NONE){
                count++;
            }
        }

        return (count == 0);
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
                if(piece == NONE && size(_get_enveloped_pieces(row,col,color)) > 0){
                    return true;
                }
            }
        }
        return false;
    }
    list<tuple<int,int>> findAllValidMoves(PieceColor color){
        list<tuple<int,int>> moves = list<tuple<int,int>>();
        for(int row = 0; row < 8; row++){
            for (int col = 0; col < 8; col++){
                PieceColor piece = _get_piece(row, col);
                if(piece == NONE && size(_get_enveloped_pieces(row,col,color)) > 0){
                    moves.push_front(tuple(row,col));
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
                switch (color){
                case NONE:
                    out+="X";
                    break;
                case ORANGE:
                    out+="O";
                    break;
                case BLUE:
                    out+="B";
                    break;
                }
                out+= "  ";
            }
        out+= "   A  B  C  D  E  F  G  H";
        }
        return out;
    }
};

