#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

/*  temp vars before physical input buttons get placed
    inputs from de2 needed:
    3 buttons - row, column, enter, maybe start?
    switches = emotes, but not in this program
    also 2 switches to select the difficulty (00, 01, 10)
    NOT DONE: enter button starts next player's timer like chess
*/

// each minesweeper object = one game. all the data of each game
// is stored within the class
class Minesweeper
{
private:
    // creates a Tile object that stores our info abt each tile
    struct Tile
    {

        int minesAround = 0;  // num of mines surrounding a tile
        bool revealed = 0;    // bool = true when the tile has been chosen already
        bool flagged = 0;     // bool = true when a tile has been flagged, not chosen
        bool isMine = 0;      // bool = if a certain tile is a mine
        char tileChar = '\0'; // purely for output, if a tile is unknown to the player, it is NULL
    };
    int size = 0;          // size of the game array
    int numMines = 0;      // num of mines on board
    int revealedTiles = 0; // num of revealed tiles
    bool winner = 0;       // bool is true when player selects all tiles other than mines
    Tile **gameArr;        // dynamic game array

public:
    bool endAll = 0; // bool true if player uncovers mine, ending the game
    // constructor initalizes game array size/difficulty & the number of mines
    // all games are square (height & length = width)
    Minesweeper(int width)
    {
        gameArr = new Tile *[width];
        for (int i = 0; i < width; i++)
        {
            gameArr[i] = new Tile[width]();
        }
        size = width;
        // approx 15% of tiles are mines
        numMines = width * width * 0.15;
        placeMines(); // calls placeMines function automatically, so we don't have to in main
    }
    // destructor
    ~Minesweeper()
    {
        for (int i = 0; i < size; i++)
        {
            delete[] gameArr[i];
        }
        delete[] gameArr;
    }
    // randomly assigns appropriate numMines to game Array
    void placeMines()
    {
        int mineCount = 0;           // temp var makes sure we have the right number of mines
        while (mineCount < numMines) // makes sure we have right num of UNIQUE mine placements
        {
            int x = rand() % size;
            int y = rand() % size;

            // makes sure that if a tile is already a mine, we don't increment numMines/minecount
            if (!(gameArr[x][y]).isMine)
            {
                gameArr[x][y].tileChar = 'm'; // assigns tile char to m - probs not needed
                gameArr[x][y].isMine = 1;     // updates tile to label it as a mine
                mineCount++;                  // increments mineCount by 1 for the while loop

                // the following logic increments the num of mines directly bordering the new mine
                if (x > 0 && y < size - 1)
                    gameArr[x - 1][y + 1].minesAround += 1;
                if (y < size - 1)
                    gameArr[x][y + 1].minesAround += 1;
                if (x < size - 1 && y < size - 1)
                    gameArr[x + 1][y + 1].minesAround += 1;
                if (x > 0)
                    gameArr[x - 1][y].minesAround += 1;
                if (x < size - 1)
                    gameArr[x + 1][y].minesAround += 1;
                if (x > 0 && y > 0)
                    gameArr[x - 1][y - 1].minesAround += 1;
                if (y > 0)
                    gameArr[x][y - 1].minesAround += 1;
                if (x < size - 1 && y > 0)
                    gameArr[x + 1][y - 1].minesAround += 1;
            }
        }
    }
    // returns the number of mines on the board for display purposes in the visual portion
    int getNumMines()
    {
        return numMines;
    }
    // returns true if a tile actually exists on the board, or hasn't already been selected by player
    bool isValidTile(int x, int y)
    {
        if (x < 1 || x > size || y < 1 || y > size || (gameArr[x - 1][y - 1].revealed))
        {
            cout << "**** INVALID TILE ****" << endl;
            return 0;
        }
        return 1;
    }
    // x, y = position vars, action(0) = flag, action(1) = tile selection
    void selectTile(int x, int y, int action)
    {
        if (!isValidTile(x, y))
        {
            return;
        }
        if (!action) // if action = 0, tile gets flagged
        {
            gameArr[x - 1][y - 1].flagged = true;
        }
        if (action) // if action = 1, tile gets revealed
        {
            gameArr[x - 1][y - 1].revealed = 1;
            // if revealed tile is a mine, ends the game
            if (gameArr[x - 1][y - 1].isMine)
            {
                endAll = true;
                return;
            }
            revealedTiles++; // increments the number of revealed tiles

            // if the number of revealed tiles = the number of mineless tiles, game ends, player wins
            if (revealedTiles == (size * size) - numMines)
            {
                winner = true;
                endAll = true;
            }
        }
    }
    // starts the game and runs until the game ends
    void gameStart()
    {
        while (!endGame())
        {
            // **** temp game input logic, will replace when de2 inputs are added ****
            // **** as of now: it prompts the user to select a row and column to reveal ****
            printBoardPlayer(); // prints board before each player's turn
            int x, y;
            cout << "row: ";
            cin >> x;
            // INPUT -1 FOR ROW TO END TEST GAME IMMEDIATELY
            if (x == -1)
            {
                cout << "**** TEST GAME ENDING ****" << endl;
                return;
            }
            cout << "col: ";
            cin >> y;

            // **** temp input is hard coded only reveal tiles, not flag them ****
            // **** will update when de2 inputs are added ****
            selectTile(x, y, 1); // calls select tile function to reveal a valid tile w/ row and col inputs
        }
    }
    // prints the locations of the mines for testing purposes only
    // move to private before final submission
    void printBoardAdmin()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (gameArr[i][j].isMine)
                {
                    cout << " * ";
                }
                else
                {
                    cout << " " << gameArr[i][j].minesAround << " ";
                }
            }
            cout << endl;
        }
    }
    // prints the board the player can see to make their next selection
    // NEEDS TO BE UPDATED TO ACTUAL COLORED DISPLAY ONCE FINISHED
    void printBoardPlayer()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (gameArr[i][j].revealed)
                {
                    cout << " " << gameArr[i][j].minesAround << " ";
                }
                else
                {
                    cout << " _ ";
                }
            }
            cout << endl;
        }
        return;
    }
    // if mine is triggered, or player wins, prints the admin board to show where the mines were
    // can update this to print the mines sequentially when a player uses
    // can also do something cool when player wins
    bool endGame()
    {
        if (endAll)
        {
            printBoardAdmin();
            return true;
        }
        return false;
    }
    void adminTest()
    {
        for (int i = 1; i < 6; i++)
        {
            for (int j = 1; j < 6; j++)
            {
                cout << "tile: " << i << j << endl;
                selectTile(i, j, 1);
            }
        }
    }
};

int main()
{
    // rng that creates nums for random mine locations
    srand(time(0));

    Minesweeper game(6); // creates a minesweeper game w/ board 6x6

    // **** can add a switch/button to officially start the game for timing purposes ****
    game.gameStart(); // officially starts the game

    // for testing only, outputs locations of each mine
    // game.printBoardAdmin();

    // uncomment the following command to test the game logic without selecting each tile
    // game.adminTest();

    return 0;
}

/* What needs to be finished sooner
    -   figuring out how to reveal neighboring tiles that have 0 mines around for quicker gameplay
    -   figuring out how to play two games simultaneously- might need to take functions out of class & put
        into main / global function
    -   adding timers that updates the amt of total time each player takes down to milliseconds
    -   remove / private admin functions when project is complete
*/

/* What needs to be done:
MOST IMPORTANT:
    - physically connecting de2 inputs to arduino
    - reading in signals from arduino into this c++ code
    - graphics: cool looking board, something happening when player wins or loses, smth special for
      2-player mode
    - if an invalid tile is selected, all the LEDS flash red 3 times, allow player to select again
    - creating a clk display / timer display of each player and in singleplayer mode

LESS IMPORTANT (multiplayer mode) - but cool
    - emotes: each switch can output some emote / messages on 7seg display (System Verilog Code)
    - if two player mode, either display boards side by side, or display the board of the player
    - in two player mode, prompt player to enter a name
    - 7seg displays player number of whose turn it is
    - maybe figure out how to give each player their own set of inputs
    - reset switch / button
    - maybe get more buttons
*/