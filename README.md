# Sudoku Game

A simple and interactive Sudoku game implemented in C++. This project provides a command-line interface to play the classic Sudoku puzzle game.

## Features

- **Interactive Gameplay** - Play Sudoku puzzles with an easy-to-use interface
- **Input Validation** - Automatic validation of moves according to Sudoku rules
- **Game Board Display** - Clear visualization of the game board
- **Multiple Difficulty Levels** - Choose from different puzzle difficulties
- **Hint System** - Get hints to help solve the puzzle

## Getting Started

### Prerequisites

- C++ compiler (C++11 or later)
- Make (optional, for compilation)
- Terminal/Command Prompt

### Installation

1. Clone the repository:
```bash
git clone https://github.com/DuaAliAnsari/Sudoku-game.git
cd Sudoku-game
```

2. Compile the project:
```bash
g++ -o sudoku *.cpp
```

## Usage

Run the compiled executable:
```bash
./sudoku
```

Follow the on-screen prompts to:
- Select difficulty level
- Enter row and column numbers
- Input the digit (1-9) you want to place
- Use the hint system if needed

## Game Rules

- Fill the 9x9 grid so that each row, column, and 3x3 box contains all digits from 1 to 9
- Each digit can appear only once in each row, column, and 3x3 box
- Start with some pre-filled numbers and fill in the remaining empty cells
- No number can be repeated in the same row, column, or 3x3 sub-grid

## Technologies

- **Language**: C++
- **Paradigm**: Object-Oriented Programming

## How to Play

1. The game displays a Sudoku board with some numbers already filled
2. Select an empty cell by entering its row and column
3. Enter a number from 1-9
4. The game validates your move and updates the board
5. Continue until all cells are filled correctly
6. You win when the entire board is solved!