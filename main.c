#include <stdio.h>
#include <stdlib.h>

char board[3][3];
char player = 'X', computer = 'O';

// Function to initialize the game board
void initializeBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

// Function to display the game board
void displayBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf(" %c ", board[i][j]);
            if (j < 2) printf("|");
        }
        printf("\n");
        if (i < 2) printf("---|---|---\n");
    }
    printf("\n");
}

// Function to check if there is a winner
char checkWinner() {
    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
            return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')
            return board[0][i];
    }
    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')
        return board[0][2];
    // Check for draw
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return ' ';
        }
    }
    return 'D'; // Draw
}

// Function to handle player's move
void playerMove() {
    int row, col;
    printf("Enter your move (row and column): ");
    scanf("%d %d", &row, &col);
    if (row >= 1 && row <= 3 && col >= 1 && col <= 3 && board[row - 1][col - 1] == ' ') {
        board[row - 1][col - 1] = player;
    } else {
        printf("Invalid move. Try again.\n");
        playerMove();
    }
}

// Function to handle computer's move
void computerMove() {
    int row, col;
    do {
        row = rand() % 3;
        col = rand() % 3;
    } while (board[row][col] != ' ');
    board[row][col] = computer;
}

// Main game loop
int main() {
    initializeBoard();
    char winner = ' ';
    while (winner == ' ') {
        displayBoard();
        playerMove();
        winner = checkWinner();
        if (winner != ' ') break;
        computerMove();
        winner = checkWinner();
    }
    displayBoard();
    if (winner == 'D') {
        printf("It's a draw!\n");
    } else {
        printf("%c wins!\n", winner);
    }
    return 0;
}
