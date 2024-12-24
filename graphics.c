#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

char board[3][3];
char player = 'X', computer = 'O';

void initializeBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
}

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

char checkWinner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
            return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')
            return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')
        return board[0][2];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return ' ';
        }
    }
    return 'D';
}

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

void computerMove() {
    int row, col;
    do {
        row = rand() % 3;
        col = rand() % 3;
    } while (board[row][col] != ' ');
    board[row][col] = computer;
}

void renderBoard() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw the grid
    for (int i = 1; i < 3; i++) {
        glVertex3f(-1.0f, (i - 1) / 1.5f - 0.5f, 0.0f);
        glVertex3f(1.0f, (i - 1) / 1.5f - 0.5f, 0.0f);
        glVertex3f((i - 1) / 1.5f - 0.5f, -1.0f, 0.0f);
        glVertex3f((i - 1) / 1.5f - 0.5f, 1.0f, 0.0f);
    }

    glEnd();

    // Draw X's and O's
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 'X') {
                glBegin(GL_LINES);
                glColor3f(1.0f, 0.0f, 0.0f);
                glVertex3f(j / 1.5f - 0.75f, i / 1.5f - 0.75f, 0.0f);
                glVertex3f((j + 1) / 1.5f - 0.75f, (i + 1) / 1.5f - 0.75f, 0.0f);
                glVertex3f((j + 1) / 1.5f - 0.75f, i / 1.5f - 0.75f, 0.0f);
                glVertex3f(j / 1.5f - 0.75f, (i + 1) / 1.5f - 0.75f, 0.0f);
                glEnd();
            } else if (board[i][j] == 'O') {
                glBegin(GL_LINE_LOOP);
                glColor3f(0.0f, 0.0f, 1.0f);
                for (int k = 0; k < 360; k++) {
                    float theta = k * 3.14159f / 180;
                    float x = cos(theta) * 0.25f;
                    float y = sin(theta) * 0.25f;
                    glVertex3f(j / 1.5f - 0.75f + x, i / 1.5f - 0.75f + y, 0.0f);
                }
                glEnd();
            }
        }
    }

    glfwSwapBuffers(glfwGetCurrentContext());
}

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "Tic Tac Toe 3D", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return -1;
    }

    initializeBoard();
    char winner = ' ';
    while (!glfwWindowShouldClose(window)) {
        renderBoard();
        if (winner != ' ') break;
        
        playerMove();
        winner = checkWinner();
        renderBoard();
        if (winner != ' ') break;

        computerMove();
        winner = checkWinner();
        renderBoard();
    }

    displayBoard();
    if (winner == 'D') {
        printf("It's a draw!\n");
    } else {
        printf("%c wins!\n", winner);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
