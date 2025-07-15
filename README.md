# JankeyType

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
2. Generate build files: `cmake . -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=C:/Apps/vcpkg/scripts/buildsystems/vcpkg.cmake`
3. Build: `mingw32-make`

## Usage

1. Place your typing test text in `text.txt`
2. Put your desired font in `CaskaydiaCoveNerdFont-Regular.ttf`
3. Run `jankeytype.exe`

## Sound Files

- key-press-1.mp3 to key-press-4.mp3: Played when correct key is pressed
- key-wrong.mp3: Played when incorrect key is pressed

## Controls

- Type the displayed text
- Press Enter to restart the test
