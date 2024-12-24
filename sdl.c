#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define CELL_SIZE (SCREEN_WIDTH / 3)
#define MAX_NAME_LEN 50

typedef enum { False, True } Bool;
typedef enum { P1TURN, P2TURN, P1WON, P2WON, DRAW } Status;
typedef Bool boolean;

char board[3][3];
char playerNames[2][MAX_NAME_LEN];
int currentPlayer;
Status gameStatus;
boolean gameFinished;

void initializeBoard();
void drawBoard(SDL_Renderer* renderer);
void drawMarks(SDL_Renderer* renderer);
void handleMouseClick(int x, int y);
char checkWinner();
void drawText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color, TTF_Font* font);
void playSound(const char* file);
void initializeGame(char* name1, char* name2);

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Retro Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    TTF_Font* font = TTF_OpenFont("retro.ttf", 28);
    if (!font) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        return -1;
    }

    initializeGame("Player 1", "Player 2");

    int quit = 0;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleMouseClick(x, y);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawBoard(renderer);
        drawMarks(renderer);

        char winner = checkWinner();
        if (winner != ' ') {
            SDL_Color white = { 255, 255, 255, 255 };
            if (winner == 'D') {
                drawText(renderer, "Draw!", SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT / 2 - 20, white, font);
            } else {
                char message[20];
                sprintf(message, "%c Wins!", winner);
                drawText(renderer, message, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 20, white, font);
            }
            SDL_RenderPresent(renderer);
            SDL_Delay(3000);
            initializeGame(playerNames[0], playerNames[1]);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    font = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

void initializeBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
    gameStatus = P1TURN;
    currentPlayer = 'X';
    gameFinished = False;
}

void drawBoard(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 1; i < 3; i++) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE);
    }
}

void drawMarks(SDL_Renderer* renderer) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 'X' || board[i][j] == 'O') {
                SDL_Rect rect = { j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                if (board[i][j] == 'X') {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                    SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h);
                    SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x, rect.y + rect.h);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    for (int k = 0; k < 360; k++) {
                        float theta = k * 3.14159f / 180;
                        float x = rect.x + rect.w / 2 + cos(theta) * rect.w / 2;
                        float y = rect.y + rect.h / 2 + sin(theta) * rect.h / 2;
                        SDL_RenderDrawPoint(renderer, x, y);
                    }
                }
            }
        }
    }
}

void handleMouseClick(int x, int y) {
    int row = y / CELL_SIZE;
    int col = x / CELL_SIZE;

    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
        board[row][col] = currentPlayer;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        playSound("move.wav");
    }
}

char checkWinner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') return board[0][2];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') return ' ';
        }
    }
    return 'D';
}

void drawText(SDL_Renderer* renderer, const char* text, int x, int y, SDL_Color color, TTF_Font* font) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void playSound(const char* file) {
    Mix_Chunk* sound = Mix_LoadWAV(file);
    if (sound == NULL) {
        printf("Failed to load sound effect! SDL[_{{{CITATION{{{_1{](https://github.com/lostapplesauce/Tic-Tac-Toe/tree/d3a6275f2d183c23a7f5ff6cbe71ccf633fc220a/main.cpp)[_{{{CITATION{{{_2{](https://github.com/ShubhamBrody/CPP-Solo-Projects/tree/c3df8afc9e45d755c4c2996272910642598809a7/TicTacToe.cpp)[_{{{CITATION{{{_3{](https://github.com/Slope123/TicTacToe/tree/1d7b01c0710688f6a78e7a36226c5bd5b031cafd/GameCanvas.cpp)[_{{{CITATION{{{_4{](https://github.com/yihangfan2095/CSFINALPROJECT/tree/eec8231cc940efe3d3f1991825adcbceb0611418/Minigame.java)[_{{{CITATION{{{_5{](https://github.com/bitmuzi/test/tree/36c696382ea75735c65f328944ad4b6168047d0f/game_2_7%2Fthree_chess.c)[_{{{CITATION{{{_6{](https://github.com/chgogos/ceteiep_ai/tree/ffcecd1bedfe76cf8e7e030dea7a644a6dded406/puzzles%2Ftic_tac_toe%2Fttt_minimax.cpp)[_{{{CITATION{{{_7{](https://github.com/zzzsyyy/learn/tree/17a38e4a6739b177ece24d528e57882e8449ee7d/C%2Fcgame%2Ftic-tac-toe%2Fgame.c)[_{{{CITATION{{{_8{](https://github.com/miao-hu/CODE/tree/d0372d68c14b196676692925fa8d710cf076c416/C%E4%BD%9C%E4%B8%9A%2Fday08.c)[_{{{CITATION{{{_9{](https://github.com/humflelump/MyFirstCode/tree/a4498fdfa2df803d439e620593f6f15b4095865f/JavaCode%2FNeuralNetwork%2Fsrc%2FTicTacToe.java)[_{{{CITATION{{{_10{](https://github.com/18792720221/C-language-study/tree/caa2e107894d5c254c94945fbf5e4d65ef331442/sanziqi%2Fsanziqi%2FJudge.cpp)[_{{{CITATION{{{_11{](https://github.com/SlientD/usual_practice_c/tree/ac23cdd704c1084e17c1b7ede86d4739a0e1893c/%E4%B8%89%E5%AD%90%E6%A3%8B%E7%AE%80%E5%8D%95%E5%AE%9E%E7%8E%B0%2Fthree_chess.c)[_{{{CITATION{{{_12{](https://github.com/BlazerHeat/Tic-Tac-Toe/tree/a1aad2a1ab1ae2ec4f957f48b91fd31d15087899/main.c)
