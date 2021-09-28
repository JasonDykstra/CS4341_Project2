#include <tuple>;
#include <string>;
using namespace std;

class Board{

}

enum Direction{
    UP = make_tuple(-1, 0),
    UP_LEFT = make_tuple(-1, -1),
    UP_RIGHT = make_tuple(-1, 1),
    DOWN = make_tuple(1, 0),
    DOWN_LEFT = make_tuple(1, -1),
    DOWN_RIGHT = make_tuple(1, 1),
    LEFT = make_tuple(0, -1),
    RIGHT = make_tuple(0, 1)
}