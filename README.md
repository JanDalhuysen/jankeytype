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

1. Install raylib: `vcpkg install raylib`
2. Generate build files:
```
cmake . -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=C:/Apps/vcpkg/scripts/buildsystems/vcpkg.cmake
```
3. Build: `mingw32-make`

## Usage

1. Run `jankeytype` with no arguments for random easy words
2. Run `jankeytype -l my_paragraph.txt` to type specific text 

## Sound Files

- key-press-1.mp3 to key-press-4.mp3: Played when correct key is pressed
- key-wrong.mp3: Played when incorrect key is pressed

## Controls

- Type the displayed text
- Press Enter to restart the test
