/* 
 * File:   Lab6.c
 * Author: Fox
 *
 * Created on November 3, 2015, 11:09 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * TO MAKE CODE RUN THROUGH AN ENIRE GAME
 * 1- IN PRINTVALIDMOVES, RESET VALID MOVES TO ALL NULL ENTIRES
 * 2- CALL NEEDED FUNCTIONS
 * 3- LOOP IN MAIN
 */

typedef struct {
        char color;
        bool hasMove;     
    } Player;
    
    static Player computerPlayer;
    static Player humanPlayer;
    static Player *currentPlayer;

int getBoardSize(char gameBoard[][26]) {
    //This function simply gets the dimension of the nxn board
    int n;
    printf("Enter the board dimension: ");
    scanf("%d", &n);
    
    return n;    
}

void printPointerInfo() {
    //printf("currentPlayer points to %d\n", currentPlayer);
    //printf("computer player is at address %d\n", &computerPlayer);
    //printf("humanPlayer is at address %d\n", &humanPlayer);
    printf("Comp status: %d Human Status: %d\n", computerPlayer.hasMove, humanPlayer.hasMove);
    printf("CHANGING %c PLAYERS STATUS TO FALSE\n", currentPlayer->color, currentPlayer->hasMove);
    printf("Comp status: %d Human Status: %d\n", computerPlayer.hasMove, humanPlayer.hasMove);
}

void newTurn(char gameBoard[][26], int n, char validSpaces[][26], int scoreSpaces[][26]) {   
    if (currentPlayer == &computerPlayer) currentPlayer = &humanPlayer;
    else currentPlayer = &computerPlayer;
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            validSpaces[i][j] = 'U';
            scoreSpaces[i][j] = 0;
            
        }
    }
}

void configureBoard(char gameBoard[][26], int n, char validSpaces[][26], int scoreSpaces[][26]) {
    //This function will set up the initial conditions of the board and the
    //valid spaces array
    computerPlayer.hasMove = true;
    humanPlayer.hasMove = true;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            gameBoard[i][j] = 'U';
            validSpaces[i][j] = 'U';
            scoreSpaces[i][j] = 0;
        }
    }
    
    //Setting the inital coordinates to be in the middle of the nxn matrix
    gameBoard[n/2 - 1][n/2 -1] = 'W';
    gameBoard[n/2 - 1][n/2] = 'B';
    gameBoard[n/2][n/2 -1] = 'B';
    gameBoard[n/2][n/2] = 'W';
    
    printf("Computer plays (B/W) : ");
    scanf(" %c", &computerPlayer.color);
    
    if (computerPlayer.color == 'W') humanPlayer.color = 'B';
    else humanPlayer.color = 'W';
    
}

void initalizeBoardValue(char gameBoard[][26], int n, int boardValues[][26]) {
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j++) {
           boardValues[i][j] = 0; 
        }
    }
    
    int bounds = n/2;
    boardValues[0][0] = 10 * n;
    boardValues[1][0] = -5 * n;
    boardValues[0][1] = boardValues[1][0];
    boardValues[1][1] = -10 * n;
    
    for (int i = 2; i < bounds; i ++) {
        boardValues[0][i] = (10 * n) - i;
        boardValues[i][0] = boardValues[0][i];
        boardValues[1][i] = (-1 * n) - i;
        boardValues[i][1] = boardValues[1][i];
        for (int j = 2; j < bounds; j ++) {
            boardValues[i][j] = 1 * n;
        }
        boardValues[i][i] = 2 * n - i; 
    }
    
    for (int i = 0; i < bounds; i ++) {
        for (int j = 0; j < bounds; j++) {
            boardValues[i][n - j - 1] = boardValues[i][j];
            boardValues[n - i - 1][n - j - 1] = boardValues[i][j];
            boardValues[n - i - 1][j] = boardValues[i][j];
        }
        
    }   
    
}

void printBoard(char gameBoard[][26], int n) {
    printf("  ");
    // top row
    for (int i = 0; i < n; i++) {
        printf("%c", ('a' + i));
    }
    printf("\n");
    // rest of the rows
    for (int i = 0; i < n; i++) {
        printf("%c ", ('a' + i));
        for (int j = 0; j < n; j++) {
            printf("%c", gameBoard[i][j]);
        }
        printf("\n");
    }
}

void printValid(char gameBoard[][26], int n) {
    printf("  ");
    // top row
    for (int i = 0; i < n; i++) {
        printf("%c", ('a' + i));
    }
    printf("\n");
    // rest of the rows
    for (int i = 0; i < n; i++) {
        printf("%c ", ('a' + i));
        for (int j = 0; j < n; j++) {
            printf("%c", gameBoard[i][j]);
        }
        printf("\n");
    }
}

bool checkLegalInDirection(char gameBoard[][26], int n, int row, int col,
        char color, int deltaRow, int deltaCol, bool edit, int scoreSpaces[][26]) {
    //diffColor is used to ensure we arent changing the space between two
    //of the same color
    bool diffColor = false;
    //oppositeColor is used to check when diff color is true
    char oppositeColor;
    if (color == 'W') oppositeColor = 'B';
    else oppositeColor = 'W';
    
    //Here we make sure the inital position is blank, unless we are editing 
    //the board
    if (gameBoard[row][col] != 'U' && !edit) return false;
    //This counter keeps track of both row increments and column increments
    //We add delta row/column to i/j every iteration to check all 8 possible 
    //lines
    for (int i = row + deltaRow, j = col + deltaCol; i < n &&
            i > -1 && j < n && j > -1; i += deltaRow, j += deltaCol) {
        //if we have already hit a different color and we found our color
        //We know this is a legal move
        
        if (gameBoard[i][j] == color && diffColor) {
            //If we are editing the board (we are making a move)
            //We run this block
                //This places the inital tile
                if (edit) {
                    gameBoard[row][col] = color;
                }
                //Using a structure similar to the one above, we traverse the
                //array, changing everything in our path to our origional color
                for (int rowChange = row + deltaRow, colChange = col + deltaCol;
                       true; rowChange += deltaRow, colChange += deltaCol) {
                    //once we get to the end, we stop changing values
                   if (rowChange == i && colChange == j) break;
                   if (edit) gameBoard[rowChange][colChange] = color;
                   if (!edit) scoreSpaces[row][col] ++;
               }
            
            return true;

        }
        //once we have found a piece of opposite color, we take note of it
        if (gameBoard[i][j] == oppositeColor) {
            diffColor = true;
        } else break;

    }
    return false;
}

void printLegalPositions(char gameBoard[][26], int n, char validSpaces[][26], int scoreSpaces[][26]) {
    char color = 'W';
    bool edit = false;
    //First we loop through legal moves for W
    //This loop changes our color
    for (int i = 0; i < n; i ++) {
       for (int j = 0; j < n; j ++) {
           validSpaces[i][j] = 'U';
        }
    }
    
    for (int colorSwitch = 0; colorSwitch < 2; colorSwitch ++) {  
        //This loop goes through rows
        for (int i = 0; i < n; i++) {
            //This loop goes through cols
            for (int j = 0; j < n; j++) {
                //These two nested loops go through 8 diff directions
                for (int deltaRow = -1; deltaRow < 2; deltaRow++) {
                    //found makes sure we dont return true for every direction
                    //if a move satisfies multiple directions
                    for (int deltaCol = -1; deltaCol < 2; deltaCol++) {
                        //Not calling the function in the case of both increments
                        //being 0
                        if (!(deltaCol == 0 && deltaRow == 0)) {
                            //Calling the check lnDirection function
                            if (checkLegalInDirection(gameBoard, n, i, j, color,
                                    deltaRow, deltaCol, edit, scoreSpaces)) {
                                //If the space is already valid for the other color
                                //We then set it to * which means the space is 
                                //valid for both players
                                if ((color == 'A' && validSpaces[i][j] != 'U' && validSpaces[i][j] != 'A') || (color == 'B' && validSpaces[i][j] != 'U' && validSpaces[i][j] != 'B')) {
                                    validSpaces[i][j] = '*';
                                } else {
                                    validSpaces[i][j] = color;
                                }
                            }
                        }                        
                    }   
                }
            }
        }
        color = 'B';
    }
}

void moveMaker(char gameBoard[][26], int n, char validSpaces[][26], int color, 
    int row, int col, int scoreSpaces[][26]) {
    bool edit = true;
    for (int deltaRow = -1; deltaRow < 2; deltaRow++) {
        for (int deltaCol = -1; deltaCol < 2; deltaCol++) {
            if (!(deltaCol == 0 && deltaRow == 0)) {
                //We loop through all directions for the space provided, and
                //call our check lndirection function with the permission to edit
                checkLegalInDirection(gameBoard, n, row, col, color, deltaRow, deltaCol, edit, scoreSpaces);
            }
        }
    }        
}

void playerMoveChecker(char gameBoard[][26], int n, char validSpaces[][26], int scoreSpaces[][26]) {
    printLegalPositions(gameBoard, n, validSpaces, scoreSpaces);
    //printValid(validSpaces, n);
    currentPlayer->hasMove = true;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (validSpaces[i][j] == currentPlayer->color || validSpaces[i][j] == '*') {
               return;
            }
        }
    }
    //printPointerInfo();
    //printValid(validSpaces, n);
    currentPlayer->hasMove = false;
}

bool moveValidator(char gameBoard[][26], int n, char validSpaces[][26], int scoreSpaces[][26]) {
    char inputString[2];
    int row;
    int col;    
    bool validInput = true;
    
    if (!(currentPlayer->hasMove)) {
        return true;
    }
    
    printf("Enter a move for colour %c (RowCol): ", currentPlayer->color);
    for (int i = 0; i < 2; i++) {
        //Getting user input
        scanf(" %c", &inputString[i]);
    }
    
    row = inputString[0] - 'a';
    col = inputString[1] - 'a';
    //ensuring valid characters were input    
    if ((inputString[1] < 'a' || inputString[1] > 'a' + n || inputString[0] < 'a'
            || inputString[0] > 'a' + n )) {
        
        validInput = false;
        //If the input is correct, we assign our color to the one given
    }
    
    if (validInput) {
        //We now check if the input is one of the valid spaces, and that it
        //matches the color at that space or if the space is available to both
        //players
        //if not, we set it to false
        if (!(validSpaces[row][col] == currentPlayer->color)) {
                
            if (!(validSpaces[row][col] == '*')) {
            validInput = false;
                
            }
                
        }
    }
    
    if (!validInput) {
        printf("Invalid move.\n");
        return false;
    } else {
        //We now call move maker
        moveMaker(gameBoard, n, validSpaces, currentPlayer->color, inputString[0] - 'a', 
                inputString[1] - 'a', scoreSpaces);
    }
    return true;
    
}

bool doMovesExist(char gameBoard[][26], int n, char validSpaces[][26], int scoreSpaces[][26]) {
    playerMoveChecker(gameBoard, n, validSpaces, scoreSpaces);
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if (validSpaces[i][j] != 'U') return true;
        }
    }
    return false;
}

void gameIsOver(char gameBoard[][26], int n) {
    int whiteScore = 0;
    int blackScore = 0;
    for (int i = 0; i < n; i++) {
        for(int j = 0; j < n; j++) {
            if (gameBoard[i][j] == 'W') whiteScore ++;
            if (gameBoard[i][j] == 'B') blackScore ++;
        }
    }
    if (blackScore == whiteScore) {
        printf("Draw!");
    } else if (whiteScore < blackScore) {
        printf("B player wins.");
    } else printf("W player wins.");
}

void computerMove(char gameBoard[][26], int n, char validSpaces[][26],
        int scoreSpaces[][26], int boardValues[][26]) {
    if (!(computerPlayer.hasMove)) return;
    int maxValue = 0, maxRow = 0, maxCol = 0;
    int maxWeight = 0;
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < n; j ++) {
            if (validSpaces[i][j] == '*' || validSpaces[i][j] == computerPlayer.color) {
                if (maxWeight = 0) {
                    maxWeight = boardValues[i][j];
                    maxRow = i;
                    maxCol = j;
                } else if (maxWeight < boardValues[i][j]) {
                    maxWeight = boardValues[i][j];
                    maxRow = i;
                    maxCol = j;
                } else if (maxWeight = boardValues[i][j]) {
                    if (scoreSpaces[maxRow][maxCol] < scoreSpaces[i][j]) {
                        maxRow = i;
                        maxCol = j;
                    }
                        
                }
            }
        }
    }
    printf("Computer places %c at %c%c.\n", computerPlayer.color, ('a' + maxRow), ('a' + maxCol));
    moveMaker(gameBoard, n, validSpaces, computerPlayer.color, maxRow, maxCol, scoreSpaces);
}

void playGame(char gameBoard[][26], int n, char validSpaces[][26], int scoreSpaces[][26], int boardValues[][26]) {
        
    configureBoard(gameBoard, n, validSpaces, scoreSpaces);
    initalizeBoardValue(gameBoard, n, boardValues);
    printBoard(gameBoard, n);
    
    if (computerPlayer.color == 'B') currentPlayer = &computerPlayer;
    else currentPlayer = &humanPlayer;
    
    do {
        if (!(doMovesExist(gameBoard, n, validSpaces, scoreSpaces))) break;
       
        playerMoveChecker(gameBoard, n, validSpaces, scoreSpaces);
        if (currentPlayer == &computerPlayer) 
            computerMove(gameBoard, n, validSpaces, scoreSpaces, boardValues);
        else if (!(moveValidator(gameBoard, n, validSpaces, scoreSpaces))) break;
        printBoard(gameBoard, n);
        
        if (computerPlayer.hasMove == false && humanPlayer.hasMove == false)
            break;
        
        if (!(currentPlayer->hasMove)) {
            printf("%c player has no valid move.\n", currentPlayer->color);
        }
        
        newTurn(gameBoard, n, validSpaces, scoreSpaces);
    } while (true);
    gameIsOver(gameBoard, n);
    
}

int main(int argc, char** argv) {
    //Simple var declaration
    //freopen("input.txt","r",stdin);
    static char gameBoard[26][26];
    static char validSpaces[26][26];
    static int scoreSpaces[26][26];
    static int boardValues[26][26];
    int n = getBoardSize(gameBoard);
    /* The premise of the valid spaces array is to hold information as to whether
     * Which Color can go where 
     */
    int computerMoveScore[26][26];
    
    //First we call configure board to set up the board
    playGame(gameBoard, n, validSpaces, scoreSpaces, boardValues);
    
    return (EXIT_SUCCESS);
}

