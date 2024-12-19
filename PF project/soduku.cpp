#include <iostream>
#include <ctime>
#include <cstdlib>
#include <limits>  // For handling incorrect input

#define N 9
#define MAX_INCORRECT_ATTEMPTS 5  // Max number of incorrect attempts before game ends

using namespace std;

int board[N][N];
int solutionBoard[N][N];
int hintsUsed = 0;
const int maxHints = 3;
int incorrectAttempts = 0;  // Counter for incorrect attempts

// Function to display the rules of the game
void displayRules() {
    cout << "\n--- SUDOKU GAME RULES ---\n";
    cout<<endl;
    cout << "1. The grid is 9x9 and divided into 3x3 smaller grids.\n";
    cout << "2. The numbers 1-9 must appear in each row, column, and 3x3 grid exactly once.\n";
    cout << "3. You can enter a number by selecting a row, column, and the number (1-9).\n";
    cout << "4. You have a maximum of " << MAX_INCORRECT_ATTEMPTS << " incorrect attempts before the game ends.\n";
    cout << "5. You can use a hint to fill in an empty cell, but you have a maximum of " << maxHints << " hints.\n";
    cout << "6. If you quit the game, you can see the solution.\n";
    cout<<endl;
    cout << "GOOD LUCK AND HAVE FUN!\n\n";
}

// Function to check if a number can be placed at board[row][col]
bool isValid(int row, int col, int num) {
    for (int x = 0; x < N; x++) {
        if (board[row][x] == num || board[x][col] == num) 
            return false;  // Number exists in the same row or column
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) 
                return false;  // Number exists in the same 3x3 grid
        }
    }
    return true;
}

// Solve the Sudoku puzzle using recursion
bool solveSudoku() {
    int row, col;
    bool unassigned = false;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                unassigned = true;
                break;
            }
        }
        if (unassigned) break;
    }
    
    if (!unassigned) return true;  // Puzzle is solved

    for (int num = 1; num <= 9; num++) {
        if (isValid(row, col, num)) {
            board[row][col] = num;

            if (solveSudoku()) 
                return true;

            board[row][col] = 0;  // Backtrack
        }
    }
    return false;
}

// Randomly fill the board with valid values
void fillValues() {
    srand(time(0));
    for (int i = 0; i < 20; i++) {
        int row = rand() % N;
        int col = rand() % N;
        int num = rand() % 9 + 1;
        if (board[row][col] == 0 && isValid(row, col, num)) 
            board[row][col] = num;
    }
}

// Generate the initial Sudoku board and the solution
void generateBoard() {
    fillValues();
    solveSudoku();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            solutionBoard[i][j] = board[i][j];
        }
    }
    for (int i = 0; i < 40; i++) {
        int row = rand() % N;
        int col = rand() % N;
        board[row][col] = 0;  // Remove numbers to create a puzzle
    }
}

// Display the current Sudoku board
void displayBoard() {
    cout << "\n  Sudoku Board:\n\n";
    cout << "    1 2 3   4 5 6   7 8 9 \n";
    cout << "  +-------+-------+-------+\n";
    for (int i = 0; i < N; i++) {
        cout << i + 1 << " | ";
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) 
                cout << "_ ";
            else 
                cout << board[i][j] << " ";

            if ((j + 1) % 3 == 0) cout << "| ";
        }
        cout << "\n";
        if ((i + 1) % 3 == 0) cout << "  +-------+-------+-------+\n";
    }
    cout << "\n";
}

// Check if the Sudoku board is completely filled
bool isBoardFull() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) 
                return false;
        }
    }
    return true;
}

// Apply a hint by filling a first empty cell
void giveHint() {
    if (hintsUsed >= maxHints) {
        cout << "No hints left!\n";
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 0) {
                board[i][j] = solutionBoard[i][j];
                hintsUsed++;
                cout << "Hint applied successfully at position (" << i + 1 << ", " << j + 1 << ")! " << maxHints - hintsUsed << " hints remaining.\n";
                return;
            }
        }
    }
}

// Display the solution of the Sudoku puzzle
void displaySolution() {
    cout << "\nSolution:\n\n";
    cout << "    1 2 3   4 5 6   7 8 9 \n";
    cout << "  +-------+-------+-------+\n";
    for (int i = 0; i < N; i++) {
        cout << i + 1 << " | ";
        for (int j = 0; j < N; j++) {
            if (solutionBoard[i][j] == 0) 
                cout << "_ ";
            else 
                cout << solutionBoard[i][j] << " ";

            if ((j + 1) % 3 == 0) cout << "| ";
        }
        cout << "\n";
        if ((i + 1) % 3 == 0) cout << "  +-------+-------+-------+\n";
    }
    cout << "\n";
}
// Function for the user to solve the Sudoku puzzle
void userSolve() {
    int row, col, num;
    time_t startTime;  // Variable to store the start time of each move
    while (true) {
        displayBoard();
        
        // Start a timer for each move
        startTime = time(0);

        cout << "\nEnter row (1-9), column (1-9), and number (1-9) to place, or enter '0' to use a hint, or '-1' to quit: ";
        cin >> row;

        // Check for invalid input (non-integer)
        if (cin.fail()) {
            cin.clear();  // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
            cout << "Invalid input. Please enter integers only.\n";
            continue;
        }

        // Quit and display solution if requested
        if (row == -1) {
            char displaySol;
            cout << "Would you like to see the solution? (y/n): ";
            cin >> displaySol;
            if (displaySol == 'y' || displaySol == 'Y') {
                displaySolution();
            }
            break;
        }

        // Use a hint if the user enters 0
        if (row == 0) {
            giveHint();
            continue;
        }

        cin >> col >> num;
        row--, col--;  // Adjust for 0-based indexing

        if (row < 0 || row >= N || col < 0 || col >= N || num < 1 || num > 9) {
            cout << "Invalid input. Try again.\n";
            continue;
        }

        if (board[row][col] != 0) {
            cout << "This cell is already filled. Try another one.\n";
            continue;
        }

        // Check if the move is valid
        if (isValid(row, col, num)) {
            board[row][col] = num;
            cout << "Correct!\n";
        } else {
            incorrectAttempts++;
            cout << "Incorrect! You have " << MAX_INCORRECT_ATTEMPTS - incorrectAttempts << " incorrect attempts left.\n";
            if (incorrectAttempts >= MAX_INCORRECT_ATTEMPTS) {
                cout << "Game over! You've reached the maximum incorrect attempts.\n";
                break;
            }
        }
        // Check if the Sudoku board is full (puzzle solved)
        if (isBoardFull()) {
            cout << "Congratulations! You solved the Sudoku.\n";
            break;
        }
    }
}

int main() {
    char seeRules;
    cout << "WELCOME TO SUDOKU!\n";
    cout<<endl;

    // Ask the user if they want to see the rules
    cout << "Do you want to see the rules of the game? (y/n): ";
    cin >> seeRules;

    if (seeRules == 'y' || seeRules == 'Y') {
        displayRules();  // Display the rules if user says "yes"
    }

    
    generateBoard();
    userSolve();
    return 0;
}