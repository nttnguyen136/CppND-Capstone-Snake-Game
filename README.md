# Capstone Snake Game

Welcome to the Capstone project of the Udacity C++ Nanodegree Program. This repository serves as a starting point where you can either build a C++ application from scratch or enhance this Snake game. The goal is to showcase your ability to independently develop applications using various C++ features.

## Dependencies

To build and run this project on your local machine, you need to install the following dependencies:

- CMake: Version 3.7 or higher.
- Make:
  - Linux/Mac: Generally pre-installed. For macOS, it can be installed via Xcode command line tools.
  - Windows: Can be installed via GnuWin32 Make.
- SDL2: Version 2.0 or higher.
- gcc/g++: Version 5.4 or higher.
  - Linux: Typically pre-installed.
  - Mac: Installed via Xcode command line tools.
  - Windows: Recommended to use MinGW.

## Basic Build Instructions

To get started, follow these steps:

1. Clone the repository:
    ```bash
    git clone <repository-url>
    ```

2. Create a build directory and navigate into it:
    ```bash
    mkdir build && cd build
    ```

3. Compile the project:
    ```bash
    cmake .. && make
    ```

4. Execute the game:
    ```bash
    ./SnakeGame
    ```

## New Features

1. Score Logging with Multi-threading
Logs game details (date, score, size) in real-time without interrupting gameplay.
2.	Special Food
Randomly appears every 10 seconds; adds points without increasing snake size.
3.	Pause/Resume
Press ESC to pause the game. Resume by pressing ESC again or using any arrow key.
4.	Smart Pointers
Prevents memory leaks by managing SDL resources automatically.
5.	Game Manager
Centralized class for managing game state and logic.
6.	Concurrency
Uses multi-threading, promises, mutexes, and condition variables for performance.

## Rubric Points Addressed

This project demonstrates:

1. C++ Functions and Control Structures: `src/game.cpp`
2. Object-Oriented Programming: `src/GameManager.cpp`, `src/Renderer.cpp`
3. File I/O: `src/GameSessionLogger.cpp`
4. Smart Pointers: `src/Renderer.cpp`
5. Concurrency: `src/GameSessionLogger.cpp`, `src/GameManager.cpp`

## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg