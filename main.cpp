#include <iostream>
#include <vector>

#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <raylib.h>
#include <raymath.h>

#include <time.h>
#include <string.h>

// Function to compare two Color objects
bool ColorsEqual(Color a, Color b)
{
    return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

int main()
{
    const int window_height = 800;
    const int window_width = 800;

    InitWindow(window_width, window_height, "JankeyType");

    SetTargetFPS(120);

    InitAudioDevice();

    // Load your desired sound file
    Sound key_press_1 = LoadSound("key-press-1.mp3");
    Sound key_press_2 = LoadSound("key-press-2.mp3");
    Sound key_press_3 = LoadSound("key-press-3.mp3");
    Sound key_press_4 = LoadSound("key-press-4.mp3");

    Sound key_wrong = LoadSound("key-wrong.mp3");

    // DisableCursor();

    // const char msg[256] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";

    // CaskaydiaCoveNerdFont-Regular.ttf
    Font fontTtf = LoadFont("CaskaydiaCoveNerdFont-Regular.ttf");

    int font_size = 30;

    // std::string text = "hallo world my naam is";
    // std::string text = "the quick brown fox jumps";

    // load text from file
    std::string text = "";
    FILE *file = fopen("text.txt", "r");
    if (file)
    {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file))
        {
            text += buffer;
        }
        fclose(file);
    }

    // split string into vector of strings
    std::vector<std::string> letters;
    for (int i = 0; i < text.size(); i++)
    {
        letters.push_back(text.substr(i, 1));
    }

    // Words - count the number of spaces
    int num_spaces = std::count(text.begin(), text.end(), ' ');
    int num_words = num_spaces + 1;

    // create a vector of colors with the same size as the letters vector
    std::vector<Color> letter_colors(letters.size(), WHITE);

    float elapsed_time = 0.0f;
    bool first_key_press = false;

    int index = 0;
    while (!WindowShouldClose())
    {
        // FPS
        float dt = GetFrameTime();
        char fps[8];
        sprintf(fps, "%d", GetFPS());
        SetWindowTitle(fps);

        if (first_key_press)
        {
            elapsed_time += dt;
        }

        // Drawing
        BeginDrawing();
        {
            // if enter is pressed, reset the game
            if (IsKeyPressed(KEY_ENTER))
            {
                elapsed_time = 0.0f;
                first_key_press = false;
                index = 0;
                for (int i = 0; i < letters.size(); i++)
                {
                    letter_colors[i] = WHITE;
                }
            }

            int key = GetKeyPressed();

            if (key != 0 and key != 257)
            {
                first_key_press = true;

                int letter_key = (int)toupper(letters[index][0]);

                if (key == letter_key)
                {
                    letter_colors[index] = GREEN;

                    int randValue = GetRandomValue(1, 4);

                    if (randValue == 1)
                        PlaySound(key_press_1);
                    else if (randValue == 2)
                        PlaySound(key_press_2);
                    else if (randValue == 3)
                        PlaySound(key_press_3);
                    else if (randValue == 4)
                        PlaySound(key_press_4);
                }
                else
                {
                    letter_colors[index] = RED;
                    PlaySound(key_wrong);
                }
                index++;
            }

            for (int i = 0; i < 28; i++) // letters.size(); i++)
            {
                // DrawText(letters[i].c_str(), 100 + i * 20, 400, 32, letter_colors[i]);
                DrawTextEx(fontTtf, letters[i].c_str(), (Vector2){100 + i * 20.0f, 100.0f}, (float)fontTtf.baseSize, 2, letter_colors[i]);
            }

            for (int i = 28; i < 56; i++)
            {
                DrawTextEx(fontTtf, letters[i].c_str(), (Vector2){100 + (i - 28) * 20.0f, 150.0f}, (float)fontTtf.baseSize, 2, letter_colors[i]);
            }

            for (int i = 56; i < 84; i++)
            {
                DrawTextEx(fontTtf, letters[i].c_str(), (Vector2){100 + (i - 56) * 20.0f, 200.0f}, (float)fontTtf.baseSize, 2, letter_colors[i]);
            }

            for (int i = 84; i < 112; i++)
            {
                DrawTextEx(fontTtf, letters[i].c_str(), (Vector2){100 + (i - 84) * 20.0f, 250.0f}, (float)fontTtf.baseSize, 2, letter_colors[i]);
            }

            for (int i = 112; i < 140; i++)
            {
                DrawTextEx(fontTtf, letters[i].c_str(), (Vector2){100 + (i - 112) * 20.0f, 300.0f}, (float)fontTtf.baseSize, 2, letter_colors[i]);
            }

            for (int i = 140; i < 168; i++)
            {
                DrawTextEx(fontTtf, letters[i].c_str(), (Vector2){100 + (i - 140) * 20.0f, 350.0f}, (float)fontTtf.baseSize, 2, letter_colors[i]);
            }

            for (int i = 168; i < 196; i++)
            {
                DrawTextEx(fontTtf, letters[i].c_str(), (Vector2){100 + (i - 168) * 20.0f, 400.0f}, (float)fontTtf.baseSize, 2, letter_colors[i]);
            }

            // DrawTextEx(fontTtf, msg, (Vector2){20.0f, 100.0f}, (float)fontTtf.baseSize, 2, LIME);
            // DrawText("Using TTF font generated", 20, GetScreenHeight() - 30, 20, GRAY);

            DrawText(TextFormat("Time: %.2f s", elapsed_time), 400 - 150, 10, 20, WHITE);

            // check if all the letters are green
            for (int i = 0; i < letters.size(); i++)
            {
                if (!ColorsEqual(letter_colors[i], GREEN))
                {
                    break;
                }
                if (i == letters.size() - 1)
                {
                    first_key_press = false;

                    float wpm = (float)num_words / (elapsed_time / 60.0f);
                    DrawText(TextFormat("WPM: %.2f s", wpm), 400 - 150, 30, 20, WHITE);
                }
            }

            ClearBackground((Color){30, 30, 30, UINT8_MAX});
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

// cmake . -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=C:/Apps/vcpkg/scripts/buildsystems/vcpkg.cmake

// mingw32-make
