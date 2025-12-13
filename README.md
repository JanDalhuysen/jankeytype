# jankeytype

<img width="791" height="790" alt="image" src="https://github.com/user-attachments/assets/3e798f31-803b-47bb-a4eb-300dcc571082" />

A typing speed test game made with raylib.

## Features

- Real-time typing feedback
- Random key press sounds
- WPM (Words Per Minute) calculation
- Color-coded correct/incorrect letters

## Requirements

- raylib library
- C++ compiler
- CMake

## Installation

> raylib will be automatically detected or built by CMake if not found.

1. Clone the repository: `git clone https://github.com/JanDalhuysen/jankeytype.git`
2. Navigate to the project directory: `cd jankeytype`
3. Generate build files: `cmake .`
4. Build: `make`

## Usage

1. Run `jankeytype` with no arguments for random easy words
2. Run `jankeytype -l my_paragraph.txt` to type specific text

## Sound Files

- key-press-1.mp3 to key-press-4.mp3: Played when correct key is pressed
- key-wrong.mp3: Played when incorrect key is pressed

## Controls

- Type the displayed text
- Press Enter to restart the test

## Building with Docker

You can build the project in a clean, isolated Linux environment using Docker. This creates a Linux executable.

1.  **Ensure Docker is running**
2.  **Run the build script:**
    ```bash
    ./build_docker.sh
    ```
3.  **Run the output:**
    The binary will be placed in the current directory.
    Run it with `./jankeytype`.
    _(Note: This is a Linux binary. To run it on Windows, you will need WSL)_
