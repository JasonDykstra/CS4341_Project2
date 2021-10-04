# CS 4341 Project 2 Team Unnecessary Struggle
Team members: Jason Dykstra, Travis Thompson, Eren Eroglu

---

## Compiling and Running the Program
Our program is written in C++. To compile, simply navigate to the project directory in a command line and use `g++` to compile by typing `g++ main.cpp`. This should create an executable (if there is not one already present) which you can run by executing `./UnnecessaryStruggle.exe`

---

## Utility Function, Evaluation, Node Expansion
Our program uses a weighted board utility function, where we use a static map of the board with pre-determined weights that correspond to each space as shown below:

    int weightedBoard[64] = {
        4, -3, 2, 2, 2, 2, -3, 4,
        -3, -4, -1, -1, -1, -1, -4, -3,
        2, -1, 1, 0, 0, 1, -1, 2,
        2, -1, 0, 1, 1, 0, -1, 2,
        2, -1, 0, 1, 1, 0, -1, 2,
        2, -1, 1, 0, 0, 1, -1, 2,
        -3, -4, -1, -1, -1, -1, -4, -3,
        4, -3, 2, 2, 2, 2, -3, 4};

In order to apply this heuristic, we sum up the weights in the tiles that our team has posession of in any given board, and subtract the sum of the weights for tiles which the opponenets own. This gives us an integer which represents the "score" of any given board which we can use to determine how good one move is compared to another. As seen above, we represented priority over corners and edges by giving them a relatively large positive weight, whereas spaces adjacent to corners or spaces where we don't want to move, we give a large negative weight.

When looking at each node (or board), our program uses alpha beta pruning in order to stop expanding nodes as they become worthless to continue expanding. This may happen when a best solution is already found, thus eliminating the need to explore further nodes.

---

## Results
In order to test our program we compile two different versions of it (only changing team name) and had them play each other. Surprisingly, these games were not all identical, however it seemed as if the bot that move first would typically win. We were initially using a quick heuristic (algorithms.cpp:82) to test whether the bot could successfully interact with the referee. After seeing that this worked, we created two new heuristics: weighted board (algorithms.cpp:48) and mobility (algorithms.cpp:36). We compiled one bot to use a mobility heuristic and one to use a weighted board heuristic, and the bot which used the weighted board seemed to perform much better. After investigating this phenomenon, we realized that the depth of the mobility heuristic was much more limited by the time limit than the other bot. This proved to us that a slightly less intelligent heuristic with larger depth performed better than a complex heuristic with low depth.

Because of our findings above, our bot is strong when placed against an opponent which uses a lower depth for minimax than us, however if an opponent is able to use a larger depth we will most likely lose.

---

## Why These Heuristics?
We first did some research on how to play Othello since none of us were too familiar, and found that most guides for improvement told players to prioritize corners over almost anything else, and to try and limit the amount of moves your opponent can make. This inspired the weighted board and mobility heuristics, which we hope to refine in order to combine them in the future. The quick heuristic was one of the most simple solutions we could think of to test the bot, and unsurprisingly proved to perform worse than the others. This lined up with our research as we came across many discussions regarding the nievety of using greedy heuristics.