#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SIZE_3x3 3
#define SIZE_6x6 6
#define SIZE_9x9 9
#define FILENAME "sudoku_save.txt"

// Function prototypes
void menu();
void newGame();
void continueGame();
void solveSudoku(int size, int grid[][size]);
bool solve(int grid[][SIZE_9x9], int size);
bool isValid(int grid[][SIZE_9x9], int row, int col, int num, int size);
void printGrid(int grid[][SIZE_9x9], int size);
bool findEmptyLocation(int grid[][SIZE_9x9], int *row, int *col, int size);
void copyGrid(int dest[][SIZE_9x9], int src[][SIZE_9x9], int size);
void playSudoku(int size, int grid[][SIZE_9x9]);
void saveGame(int size, int grid[][SIZE_9x9]);
bool loadGame(int size, int grid[][SIZE_9x9]);

// Predefined Sudoku puzzles for each difficulty level
int easyGrids[2][SIZE_3x3][SIZE_3x3] = {
    {{1, 0, 3}, {0, 2, 0}, {0, 0, 0}},
    {{0, 0, 3}, {1, 0, 0}, {0, 2, 0}}
};

int mediumGrids[2][SIZE_6x6][SIZE_6x6] = {
    {
        {0, 0, 0, 3, 0, 5},
        {0, 3, 0, 0, 0, 4},
        {0, 0, 0, 0, 0, 0},
        {2, 0, 0, 6, 0, 0},
        {0, 0, 4, 0, 3, 0},
        {5, 0, 6, 0, 0, 0}
    },
    {
        {0, 1, 0, 0, 0, 0},
        {4, 0, 0, 0, 0, 6},
        {0, 0, 0, 2, 0, 0},
        {0, 0, 5, 0, 0, 0},
        {0, 0, 0, 0, 3, 1},
        {0, 0, 0, 0, 0, 0}
    }
};

int hardGrids[2][SIZE_9x9][SIZE_9x9] = {
    {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    },
    {
        {0, 0, 0, 0, 6, 0, 0, 0, 0},
        {0, 4, 0, 0, 0, 0, 0, 8, 0},
        {0, 0, 0, 3, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 2, 0, 0, 0, 0},
        {0, 8, 0, 0, 0, 0, 0, 4, 0},
        {0, 0, 0, 5, 0, 3, 0, 0, 0},
        {0, 0, 0, 6, 0, 7, 0, 0, 0},
        {0, 3, 0, 0, 0, 0, 0, 5, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0}
    }
};

int main() {
    srand(time(NULL));  // Seed the random number generator
    menu();
    return 0;
}

// Function to display the menu
void menu() {
    int choice;
    while (1) {
        printf("Menu:\n");
        printf("1. New Game\n");
        printf("2. Continue\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                newGame();
                break;
            case 2:
                continueGame();
                break;
            case 3:
                printf("Exiting the program.\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

// Function to start a new game
void newGame() {
    int choice;
    printf("New Game:\n");
    printf("1. Easy (3x3 Sudoku)\n");
    printf("2. Medium (6x6 Sudoku)\n");
    printf("3. Hard (9x9 Sudoku)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    int grid[SIZE_9x9][SIZE_9x9] = {0};  // Initialize a 9x9 grid (maximum size)
    int puzzleIndex;

    switch (choice) {
        case 1:
            puzzleIndex = rand() % 2;
            copyGrid(grid, easyGrids[puzzleIndex], SIZE_3x3);
            playSudoku(SIZE_3x3, grid);
            break;
        case 2:
            puzzleIndex = rand() % 2;
            copyGrid(grid, mediumGrids[puzzleIndex], SIZE_6x6);
            playSudoku(SIZE_6x6, grid);
            break;
        case 3:
            puzzleIndex = rand() % 2;
            copyGrid(grid, hardGrids[puzzleIndex], SIZE_9x9);
            playSudoku(SIZE_9x9, grid);
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

// Function to copy a grid
void copyGrid(int dest[][SIZE_9x9], int src[][SIZE_9x9], int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            dest[row][col] = src[row][col];
        }
    }
}

// Function to play Sudoku
void playSudoku(int size, int grid[][SIZE_9x9]) {
    int row, col, num;
    bool solved = false; // Flag to indicate whether the grid has been solved
    while (1) {
        printf("Sudoku grid:\n");
        printGrid(grid, size);

        if (solved) {
            printf("This is the solved Sudoku puzzle. No further modifications allowed.\n");
            return;
        }

        printf("Enter row number (or -1 to solve, -2 to save and exit): ");
        scanf("%d", &row);

        if (row == -1) {
            int tempGrid[SIZE_9x9][SIZE_9x9];
            copyGrid(tempGrid, grid, size);
            if (solve(tempGrid, size)) {
                printf("Solved Sudoku puzzle:\n");
                printGrid(tempGrid, size);
                solved = true;
            } else {
                printf("No solution exists for the given Sudoku puzzle.\n");
            }
            continue;
        } else if (row == -2) {
            saveGame(size, grid);
            printf("Game saved. Exiting to menu.\n");
            return;
        }

        // Ensure row input is valid before prompting for column and number
        if (row < 0 || row >= size) {
            printf("Invalid row input. Try again.\n");
            continue;
        }

        printf("Enter column number: ");
        scanf("%d", &col);

        if (col < 0 || col >= size) {
            printf("Invalid column input. Try again.\n");
            continue;
        }

        printf("Enter number: ");
        scanf("%d", &num);

        if (num < 1 || num > size) {
            printf("Invalid number input. Try again.\n");
            continue;
        }

        if (grid[row][col] == 0) {
            if (isValid(grid, row, col, num, size)) {
                grid[row][col] = num;
            } else {
                printf("Invalid move. Try again.\n");
            }
        } else {
            printf("Cell already filled. Try again.\n");
        }
    }
}

// Function to save the current game state to a file
void saveGame(int size, int grid[][SIZE_9x9]) {
    FILE *file = fopen(FILENAME, "w");
    if (file == NULL) {
        printf("Error opening file for saving.\n");
        return;
    }

    fprintf(file, "%d\n", size);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            fprintf(file, "%d ", grid[row][col]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Game saved successfully.\n");
}

// Function to continue a saved game
void continueGame() {
    int size;
    int grid[SIZE_9x9][SIZE_9x9] = {0};

    if (loadGame(SIZE_9x9, grid)) {
        size = sqrt(SIZE_9x9);
        playSudoku(size, grid);
    } else {
        printf("No saved game found.\n");
    }
}

// Function to load a saved game from a file
bool loadGame(int size, int grid[][SIZE_9x9]) {
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL) {
        return false;
    }

    fscanf(file, "%d", &size);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            fscanf(file, "%d", &grid[row][col]);
        }
    }

    fclose(file);
    return true;
}

// Function to solve the Sudoku puzzle
void solveSudoku(int size, int grid[][size]) {
    if (solve(grid, size)) {
        printf("Solved Sudoku puzzle:\n");
        printGrid(grid, size);
    } else {
        printf("No solution exists for the given Sudoku puzzle.\n");
    }
}

// Recursive function to solve the Sudoku puzzle using backtracking
bool solve(int grid[][SIZE_9x9], int size) {
    int row, col;
    if (!findEmptyLocation(grid, &row, &col, size)) {
        return true; // Puzzle solved
    }

    for (int num = 1; num <= size; num++) {
        if (isValid(grid, row, col, num, size)) {
            grid[row][col] = num;
            if (solve(grid, size)) {
                return true;
            }
            grid[row][col] = 0;
        }
    }

    return false; // Trigger backtracking
}

// Function to find an empty location in the grid
bool findEmptyLocation(int grid[][SIZE_9x9], int *row, int *col, int size) {
    for (*row = 0; *row < size; (*row)++) {
        for (*col = 0; *col < size; (*col)++) {
            if (grid[*row][*col] == 0) {
                return true;
            }
        }
    }
    return false;
}

// Function to check if placing a number in a cell is valid
bool isValid(int grid[][SIZE_9x9], int row, int col, int num, int size) {
    int boxSize = sqrt(size);
    int boxRowStart = row - row % boxSize;
    int boxColStart = col - col % boxSize;

    // Check the row and column
    for (int x = 0; x < size; x++) {
        if (grid[row][x] == num || grid[x][col] == num) {
            return false;
        }
    }

    // Check the box
    for (int r = 0; r < boxSize; r++) {
        for (int c = 0; c < boxSize; c++) {
            if (grid[r + boxRowStart][c + boxColStart] == num) {
                return false;
            }
        }
    }

    return true;
}

// Function to print the Sudoku grid
void printGrid(int grid[][SIZE_9x9], int size) {
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            printf("%d ", grid[row][col]);
        }
        printf("\n");
    }
}
