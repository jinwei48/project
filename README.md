# Snake Game

A classic Snake game implementation with modern interface and level system.

## Game Introduction

This is a Snake game developed in C language with the following features:

- Colorful graphical interface for better visual experience
- Level system that automatically upgrades as score increases
- Progressive difficulty with faster game speed at higher levels
- Information panel on the right side showing real-time game status
- Direction control hints and game operation guide

## Features

### Basic Features
- Use WASD keys to control the snake's movement direction
- Eat food to grow the snake and earn points
- Game ends when colliding with boundaries or itself
- ESC key to exit the game anytime

### Enhanced Features
- **Level System**: Automatically level up after earning a certain score
- **Dynamic Difficulty**: Snake movement speed gradually increases with levels
- **Information Display**: Shows current score, level, and points needed for next level
- **Visual Enhancement**: Colorful interface with direction arrows for snake head and special characters for body segments and food

## Installation and Running

### Compilation
Use the following command to compile the game:

```bash
gcc snake.c -o output\snake.exe
```

### Running
After compilation, run the generated executable file directly:

```bash
output\snake.exe
```

## Game Controls

- **W** - Move Up
- **S** - Move Down
- **A** - Move Left
- **D** - Move Right
- **ESC** - Exit Game

## Game Mechanics

### Scoring System
- Earn 10 points for each food eaten
- Automatically level up every 30 points

### Level System
- Start at Level 1
- Game speed increases with each level
- Final score and reached level are displayed when game ends

### Difficulty Adjustment
- Snake movement speed increases as level increases
- Higher levels offer greater challenge

## File Structure

- `snake.c` - Main game code file containing all game logic and function implementations
- `snake.h` - Game header file defining constants, structures, and function declarations
- `output/snake.exe` - Compiled executable file
- `教程.md` - Detailed game development tutorial (in Chinese)

## Game Interface

The game interface is divided into two parts:

1. **Left Side**: Game area showing snake, food, and boundaries
   - Snake Head: Uses direction arrows (> < ^ v) to indicate movement direction
   - Snake Body: Uses special characters to distinguish between straight and turning segments
   - Snake Tail: Uses special symbols
   - Food: Represented by $ symbol

2. **Right Side**: Information display area
   - Current Score
   - Current Level
   - Points needed for next level
   - Game control instructions

## Development Notes

This game is developed in C language and mainly relies on the following libraries:
- `<stdio.h>` - Standard input/output
- `<stdlib.h>` - Standard library functions
- `<time.h>` - Time functions for random number generation
- `<windows.h>` - Windows API for console operations and cursor control
- `<conio.h>` - Console input/output for keyboard input capture

## Extension Suggestions

To further enhance the game, consider adding these features:

1. **Custom Difficulty Settings**: Allow players to select difficulty level at game start
2. **Multiple Food Types**: Implement different types of food with varying point rewards
3. **Obstacles**: Add fixed obstacles at higher levels to increase challenge
4. **Lives System**: Allow players to collide a few times before game over
5. **High Score Record**: Save high scores to a file
6. **Sound Effects**: Add game sound effects to enhance experience

## License

This project is developed for learning purposes and is open for use and modification.

## Development Environment

- Operating System: Windows
- Compiler: GCC
- Programming Language: C

## 作者

开发者：[Your Name]