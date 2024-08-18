#include "raylib.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

class Sudoku {
public:
    const int gridSize = 9;
    const int cellSize = 80;
    const int screenWidth = gridSize * cellSize;
    const int screenHeight = gridSize * cellSize;
    const int buttonWidth = 140;
    const int buttonHeight = 40;
    const int padding = 10;
    int array[9][9];
    int solutionArray[9][9];
    bool isFixed[9][9];
    bool hints[9][9];
    int selectedRow = -1;
    int selectedCol = -1;
    int hintCount = 0;
    const int maxHints = 3;
    bool gameWon = false;
    bool gameLost = false;
    bool hintMessageDisplayed = false;
    float hintMessageTimer = 0.0f;

    Sudoku() {
        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                array[row][column] = 0;
                isFixed[row][column] = false;
                hints[row][column] = false;
            }
        }
    }

    void drawBoard() {
        InitWindow(screenWidth + buttonWidth + padding * 2, screenHeight, "Sudoku Grid");
        SetTargetFPS(60);

        while (!WindowShouldClose()) {
            float deltaTime = GetFrameTime();
            updateInput();
            updateHintMessageTimer(deltaTime);

            BeginDrawing();
            ClearBackground(RAYWHITE);

            drawCells();
            drawLines();
            drawButtons();

            if (gameWon) {
                displayVictoryMessage();
            } else if (gameLost) {
                displayLossMessage();
                displaySolution();
            } else if (hintMessageDisplayed) {
                displayHintMessage();
            }

            EndDrawing();
        }

        CloseWindow();
    }

    void drawCells() {
        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                Color cellColor = WHITE;  // Default color for empty cells
                if (isFixed[row][column]) {
                    cellColor = ORANGE;
                } else if (row == selectedRow && column == selectedCol) {
                    cellColor = LIGHTGRAY;
                }

                DrawRectangle(column * cellSize, row * cellSize, cellSize, cellSize, cellColor);

                if (array[row][column] > 0) {
                    Color textColor = hints[row][column] ? RED : BLACK;
                    int textWidth = MeasureText(TextFormat("%d", array[row][column]), 30);
                    int textX = column * cellSize + (cellSize - textWidth) / 2;
                    int textY = row * cellSize + (cellSize - 30) / 2;
                    DrawText(TextFormat("%d", array[row][column]), textX, textY, 30, textColor);
                }
            }
        }
    }

    void drawLines() {
        for (int i = 0; i <= gridSize; i++) {
            if (i % 3 == 0) {
                DrawLineEx({static_cast<float>(i * cellSize), 0}, {static_cast<float>(i * cellSize), static_cast<float>(gridSize * cellSize)}, 4, BLACK);
            } else {
                DrawLine(i * cellSize, 0, i * cellSize, gridSize * cellSize, BLACK);
            }

            if (i % 3 == 0) {
                DrawLineEx({0, static_cast<float>(i * cellSize)}, {static_cast<float>(screenWidth), static_cast<float>(i * cellSize)}, 4, BLACK);
            } else {
                DrawLine(0, i * cellSize, screenWidth, i * cellSize, BLACK);
            }
        }
        DrawRectangleLines(0, 0, screenWidth, screenHeight, BLACK);
    }

    void drawButtons() {
        int buttonX = screenWidth + padding;
        int buttonY = padding;

        DrawRectangle(buttonX, buttonY, buttonWidth, buttonHeight, LIGHTGRAY);
        DrawRectangle(buttonX, buttonY + buttonHeight + padding, buttonWidth, buttonHeight, LIGHTGRAY);
        DrawRectangle(buttonX, buttonY + 2 * (buttonHeight + padding), buttonWidth, buttonHeight, LIGHTGRAY);
        DrawRectangle(buttonX, buttonY + 3 * (buttonHeight + padding), buttonWidth, buttonHeight, LIGHTGRAY);

        DrawText("Hint", buttonX + 20, buttonY + 10, 20, BLACK);
        DrawText("Restart", buttonX + 20, buttonY + buttonHeight + padding + 10, 20, BLACK);
        DrawText("Exit", buttonX + 20, buttonY + 2 * (buttonHeight + padding) + 10, 20, BLACK);
        DrawText("Check", buttonX + 20, buttonY + 3 * (buttonHeight + padding) + 10, 20, BLACK);
    }

    void updateInput() {
        Vector2 mousePosition = GetMousePosition();
        int row = mousePosition.y / cellSize;
        int col = mousePosition.x / cellSize;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (row >= 0 && row < gridSize && col >= 0 && col < gridSize && !isFixed[row][col]) {
                selectedRow = row;
                selectedCol = col;
            } else if (mousePosition.x >= screenWidth + padding && mousePosition.x <= screenWidth + padding + buttonWidth) {
                if (mousePosition.y >= padding && mousePosition.y <= padding + buttonHeight) {
                    provideHint();
                } else if (mousePosition.y >= padding + buttonHeight + padding && mousePosition.y <= padding + 2 * (buttonHeight + padding)) {
                    restartGame();
                } else if (mousePosition.y >= padding + 2 * (buttonHeight + padding) && mousePosition.y <= padding + 3 * (buttonHeight + padding)) {
                    CloseWindow();
                } else if (mousePosition.y >= padding + 3 * (buttonHeight + padding) && mousePosition.y <= padding + 4 * (buttonHeight + padding)) {
                    checkSolution();
                }
            }
        }

        if (selectedRow != -1 && selectedCol != -1 && !isFixed[selectedRow][selectedCol]) {
            for (int num = 1; num <= 9; num++) {
                if (IsKeyPressed(KEY_ONE + num - 1)) {
                    array[selectedRow][selectedCol] = num;
                    break;
                }
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                array[selectedRow][selectedCol] = 0;
            }
        }
    }

    void updateHintMessageTimer(float deltaTime) {
        if (hintMessageDisplayed) {
            hintMessageTimer += deltaTime;
            if (hintMessageTimer >= 3.0f) {
                hintMessageDisplayed = false;
                hintMessageTimer = 0.0f;
            }
        }
    }

    void generateSudoku() {
        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                array[row][column] = 0;
                isFixed[row][column] = false;
                hints[row][column] = false;
            }
        }

        bool isValid = generateSudokuHelper(0, 0);

        if (!isValid) {
            std::cout << "No valid Sudoku grid can be generated." << std::endl;
        }

        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                solutionArray[row][column] = array[row][column];
            }
        }

        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                if (array[row][column] != 0) {
                    isFixed[row][column] = true;
                }
            }
        }
    }

    bool generateSudokuHelper(int row, int column) {
        if (row == gridSize) {
            return true;
        }

        if (column == gridSize) {
            return generateSudokuHelper(row + 1, 0);
        }

        if (array[row][column] > 0) {
            return generateSudokuHelper(row, column + 1);
        }

        for (int num = 1; num <= 9; num++) {
            if (isSafe(num, row, column)) {
                array[row][column] = num;
                if (generateSudokuHelper(row, column + 1)) {
                    return true;
                }
                array[row][column] = 0;
            }
        }

        return false;
    }

    bool isSafe(int val, int row, int col) {
        for (int i = 0; i < gridSize; i++) {
            if (array[row][i] == val || array[i][col] == val) {
                return false;
            }
        }

        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = startRow; i < startRow + 3; i++) {
            for (int j = startCol; j < startCol + 3; j++) {
                if (array[i][j] == val) {
                    return false;
                }
            }
        }

        return true;
    }

    void removeNumbers() {
        int numCellsToRemove = gridSize * gridSize / 2;
        for (int i = 0; i < numCellsToRemove; i++) {
            int row = rand() % gridSize;
            int column = rand() % gridSize;
            if (array[row][column] != 0) {
                array[row][column] = 0;
                isFixed[row][column] = false;
            }
        }
    }

    void provideHint() {
        if (hintCount >= maxHints) {
            hintMessageDisplayed = true;
            return;
        }

        std::vector<std::pair<int, int>> emptyCells;
        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                if (array[row][column] == 0) {
                    emptyCells.push_back({row, column});
                }
            }
        }

        if (!emptyCells.empty()) {
            std::pair<int, int> cell = emptyCells[rand() % emptyCells.size()];
            int row = cell.first;
            int column = cell.second;

            int correctValue = solutionArray[row][column];
            array[row][column] = correctValue;
            hints[row][column] = true;
            hintCount++;
        }
    }

    void restartGame() {
        gameWon = false;
        gameLost = false;
        hintCount = 0;
        hintMessageDisplayed = false;

        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                hints[row][column] = false;
                if (!isFixed[row][column]) {
                    array[row][column] = 0;
                }
            }
        }

        generateSudoku();
        removeNumbers();
    }

    void checkSolution() {
        bool isCorrect = true;
        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                if (array[row][column] != solutionArray[row][column]) {
                    isCorrect = false;
                    break;
                }
            }
            if (!isCorrect) break;
        }

        if (isCorrect) {
            gameWon = true;
        } else {
            gameLost = true;
        }
    }

    void displayVictoryMessage() {
        DrawText("Victory! You solved the Sudoku!", screenWidth / 2 - MeasureText("Victory! You solved the Sudoku!", 30) / 2, screenHeight / 2 - 15, 30, GREEN);
    }

    void displayLossMessage() {
        DrawText("You lost! Here is the correct solution:", screenWidth / 2 - MeasureText("You lost! Here is the correct solution:", 30) / 2, screenHeight / 2 - 60, 30, RED);
    }

    void displayHintMessage() {
        DrawText("You have exceeded the maximum number of hints!", screenWidth / 2 - MeasureText("You have exceeded the maximum number of hints!", 30) / 2, screenHeight / 2 - 15, 30, RED);
    }

    void displaySolution() {
        for (int row = 0; row < gridSize; row++) {
            for (int column = 0; column < gridSize; column++) {
                if (solutionArray[row][column] != 0) {
                    int textWidth = MeasureText(TextFormat("%d", solutionArray[row][column]), 30);
                    int textX = column * cellSize + (cellSize - textWidth) / 2;
                    int textY = row * cellSize + (cellSize - 30) / 2;
                    DrawText(TextFormat("%d", solutionArray[row][column]), textX, textY, 30, BLACK);
                }
            }
        }
    }
};

int main() {
    srand(time(0));

    Sudoku game;
    game.generateSudoku();
    game.removeNumbers();
    game.drawBoard();

    return 0;
}
