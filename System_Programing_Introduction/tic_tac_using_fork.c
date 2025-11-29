#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

char board[3][3];

// Load the board from file
void loadBoard(int fd) {
    lseek(fd, 0, SEEK_SET);
    read(fd, board, sizeof(board));
}

// Save the board to file
void saveBoard(int fd) {
    lseek(fd, 0, SEEK_SET);
    write(fd, board, sizeof(board));
}

// Initialize empty board
void initBoard(int fd) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = ' ';
    saveBoard(fd);
}

// Print the board
void printBoard() {
    printf("\n");
    for (int i = 0; i < 3; i++) {
        printf(" %c | %c | %c ", board[i][0], board[i][1], board[i][2]);
        if (i < 2) printf("\n---+---+---\n");
    }
    printf("\n");
}

// Check win condition
int checkWin() {
    // Rows & columns
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2]) return 1;
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i]) return 1;
    }
    // Diagonals
    if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return 1;
    if (board[0][2] != ' ' && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return 1;
    return 0;
}

int main() {
    int fd = open("board.dat", O_RDWR | O_CREAT, 0666);
    if (fd < 0) {
        perror("Failed to open board file");
        return 1;
    }

    initBoard(fd); // create empty board

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0) {
        // CHILD process → Player O
        char player = 'O';
        int row, col;

        while (1) {
            sleep(1); // let parent go first
            loadBoard(fd);
            printBoard();

            printf("[Child %d] Player O, enter row col (1-3): ", getpid());
            scanf("%d %d", &row, &col);
            row--; col--;

            if (board[row][col] == ' ') {
                board[row][col] = player;
                saveBoard(fd);
            }

            if (checkWin()) {
                printBoard();
                printf("Player O wins! (Child process)\n");
                exit(0);
            }
        }
    }
    else {
        // PARENT process → Player X
        char player = 'X';
        int row, col;

        while (1) {
            loadBoard(fd);
            printBoard();

            printf("[Parent %d] Player X, enter row col (1-3): ", getpid());
            scanf("%d %d", &row, &col);
            row--; col--;

            if (board[row][col] == ' ') {
                board[row][col] = player;
                saveBoard(fd);
            }

            if (checkWin()) {
                printBoard();
                printf("Player X wins! (Parent process)\n");
                kill(pid, 9); // stop child
                break;
            }
        }

        wait(NULL); // wait for child
    }

    close(fd);
    return 0;
}
