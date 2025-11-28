#include <iostream>
#include <vector>

#include <algorithm>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>

#include <string.h>
#include <time.h>

// Function to compare two Color objects
bool ColorsEqual(Color a, Color b)
{
    return (a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a);
}

int main(int argc, char *argv[])
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

    // const char msg[256] =
    // "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";

    // CaskaydiaCoveNerdFont-Regular.ttf
    Font fontTtf = LoadFont("CaskaydiaCoveNerdFont-Regular.ttf");

    int font_size = 30;

    // std::string text = "hallo world my naam is";
    // std::string text = "the quick brown fox jumps";

    // Use command line argument for filename
    const char *filename = "200.txt";
    bool literal_mode = false;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0)
        {
            literal_mode = true;
        }
        else
        {
            filename = argv[i];
        }
    }

    std::string text = "";

    if (literal_mode)
    {
        FILE *file = fopen(filename, "r");
        if (file)
        {
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), file))
            {
                text += buffer;
            }
            fclose(file);
        }
    }
    else
    {
        // Load words from file into a vector
        std::vector<std::string> all_words;
        FILE *file = fopen(filename, "r");
        if (file)
        {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file))
            {
                char *token = strtok(buffer, " \n");
                while (token)
                {
                    all_words.push_back(token);
                    token = strtok(NULL, " \n");
                }
            }
            fclose(file);
        }

        // Pick 30 random words
        std::vector<std::string> chosen_words;
        srand((unsigned int)time(NULL));
        for (int i = 0; i < 30 && !all_words.empty(); i++)
        {
            int idx = rand() % all_words.size();
            chosen_words.push_back(all_words[idx]);
            all_words.erase(all_words.begin() + idx);
        }

        // Build the text string from chosen words
        for (int i = 0; i < chosen_words.size(); i++)
        {
            text += chosen_words[i];
            if (i < chosen_words.size() - 1)
                text += " ";
        }
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

            if (key != 0 && key != 257)
            {
                first_key_press = true;

                // Handle backspace
                if (key == KEY_BACKSPACE && index > 0)
                {
                    index--;
                    letter_colors[index] = WHITE;
                }
                else
                {
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
            }

            float x = 100.0f;
            float y = 100.0f;
            float line_height = (float)fontTtf.baseSize + 10;

            for (int i = 0; i < letters.size();)
            {
                // Find the end of the current word
                int word_end = i;
                while (word_end < letters.size() && letters[word_end] != " " && letters[word_end] != "\n")
                {
                    word_end++;
                }

                // Get the word as a string
                std::string word = "";
                for (int k = i; k < word_end; k++)
                {
                    word += letters[k];
                }

                // Measure the word
                Vector2 word_size = MeasureTextEx(fontTtf, word.c_str(), (float)fontTtf.baseSize, 2);

                // Check if the word fits on the current line
                if (x + word_size.x > window_width - 100)
                {
                    x = 100.0f;
                    y += line_height;
                }

                // Draw the word character by character
                for (int k = i; k < word_end; k++)
                {
                    DrawTextEx(fontTtf, letters[k].c_str(), (Vector2){x, y}, (float)fontTtf.baseSize, 2,
                               letter_colors[k]);
                    x += MeasureTextEx(fontTtf, letters[k].c_str(), (float)fontTtf.baseSize, 2).x;
                }

                // Move to the next word
                i = word_end;

                // Handle spaces and newlines
                if (i < letters.size())
                {
                    if (letters[i] == "\n")
                    {
                        x = 100.0f;
                        y += line_height;
                    }
                    else if (letters[i] == " ")
                    {
                        DrawTextEx(fontTtf, " ", (Vector2){x, y}, (float)fontTtf.baseSize, 2, letter_colors[i]);
                        x += MeasureTextEx(fontTtf, " ", (float)fontTtf.baseSize, 2).x;
                    }
                    i++;
                }
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

            // Draw the WPM when all letters are green
            bool all_green = true;
            for (int i = 0; i < letters.size(); i++)
            {
                if (!ColorsEqual(letter_colors[i], GREEN))
                {
                    all_green = false;
                    break;
                }
            }

            if (all_green)
            {
                first_key_press = false;
                float wpm = (float)num_words / (elapsed_time / 60.0f);
                DrawText(TextFormat("WPM: %.2f", wpm), 400 - 150, 30, 20, WHITE);
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
