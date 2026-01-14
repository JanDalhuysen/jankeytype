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

// Levenshtein distance implementation
int LevenshteinDistance(const std::string &s1, const std::string &s2)
{
    const size_t m = s1.length();
    const size_t n = s2.length();
    if (m == 0)
        return n;
    if (n == 0)
        return m;

    std::vector<std::vector<int>> d(m + 1, std::vector<int>(n + 1));

    for (size_t i = 0; i <= m; ++i)
        d[i][0] = i;
    for (size_t j = 0; j <= n; ++j)
        d[0][j] = j;

    for (size_t j = 1; j <= n; ++j)
    {
        for (size_t i = 1; i <= m; ++i)
        {
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;
            // std::min requires algorithm
            int min1 = d[i - 1][j] + 1;
            int min2 = d[i][j - 1] + 1;
            int min3 = d[i - 1][j - 1] + cost;
            d[i][j] = std::min({min1, min2, min3});
        }
    }
    return d[m][n];
}

int main(int argc, char *argv[])
{
    const int window_height = 800;
    const int window_width = 800;

    InitWindow(window_width, window_height, "jankeytype");

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

    std::vector<std::string> dictionary;
    // Always load 200.txt for autocorrect dictionary
    {
        FILE *file = fopen("200.txt", "r");
        if (file)
        {
            char buffer[256];
            while (fgets(buffer, sizeof(buffer), file))
            {
                char *token = strtok(buffer, " \n");
                while (token)
                {
                    dictionary.push_back(token);
                    token = strtok(NULL, " \n");
                }
            }
            fclose(file);
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
        for (int i = 0; i < 10 && !all_words.empty(); i++)
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
    int mistakes = 0;
    int total_keypresses = 0;

    // Autocorrect variables
    bool autocorrect = false;
    std::string user_word_buffer = "";
    int word_start_index = 0;

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
                mistakes = 0;
                total_keypresses = 0;
                for (int i = 0; i < letters.size(); i++)
                {
                    letter_colors[i] = WHITE;
                }
            }

            // Toggle Autocorrect
            if (IsKeyPressed(KEY_F1))
            {
                autocorrect = !autocorrect;
            }

            int key = GetKeyPressed();

            // Ignore modifier keys (Shift, Control, Alt, Super, etc.)
            // Raylib key codes for modifiers are usually > 340
            if (key != 0 && key != KEY_ENTER && key != KEY_F1 && key < 340)
            {
                first_key_press = true;
                bool handled_by_autocorrect = false;

                // Autocorrect Logic
                if (autocorrect)
                {
                    if (key == KEY_SPACE)
                    {
                        handled_by_autocorrect = true;

                        // Find best match
                        std::string best_match = user_word_buffer;
                        int min_dist = 1000;

                        // Only autocorrect if buffer not empty
                        if (!user_word_buffer.empty())
                        {
                            // Convert user buffer to lowercase for dictionary search
                            std::string lower_buffer = user_word_buffer;
                            for (auto &c : lower_buffer)
                                c = tolower(c);

                            for (const auto &word : dictionary)
                            {
                                int dist = LevenshteinDistance(lower_buffer, word);
                                if (dist < min_dist)
                                {
                                    min_dist = dist;
                                    best_match = word;
                                }
                            }
                        }

                        // If distance is reasonable (e.g., <= 2), apply correction
                        if (min_dist > 2)
                        {
                            // Too far, keep original (or if buffer was empty)
                            best_match = user_word_buffer;
                        }

                        // Apply the word (corrected or original)
                        // This overwrites/fills the game state from word_start_index
                        index = word_start_index;

                        // Type the characters of the matched word
                        for (char c : best_match)
                        {
                            // Safety check
                            if (index >= letters.size())
                                break;

                            total_keypresses++;
                            int target_char_code = (int)toupper(letters[index][0]);
                            int input_char_code = (int)toupper(c);

                            if (input_char_code == target_char_code)
                            {
                                letter_colors[index] = GREEN;
                            }
                            else
                            {
                                letter_colors[index] = RED;
                                mistakes++;
                            }
                            index++;
                        }

                        // Now handle the Space itself
                        if (index < letters.size())
                        {
                            total_keypresses++;
                            if (letters[index] == " ")
                            {
                                letter_colors[index] = GREEN;
                                int randValue = GetRandomValue(1, 4);
                                if (randValue == 1)
                                    PlaySound(key_press_1);
                                else if (randValue == 2)
                                    PlaySound(key_press_2);
                                else if (randValue == 3)
                                    PlaySound(key_press_3);
                                else
                                    PlaySound(key_press_4);
                            }
                            else
                            {
                                letter_colors[index] = RED;
                                mistakes++;
                                PlaySound(key_wrong);
                            }
                            index++;
                        }

                        // Reset buffer and start ref
                        user_word_buffer = "";
                        word_start_index = index;
                    }
                    else if (key == KEY_BACKSPACE)
                    {
                        if (index > word_start_index && !user_word_buffer.empty())
                        {
                            user_word_buffer.pop_back();
                        }
                        else
                        {
                            // Crossed boundary or empty, reset tracking
                            if (index < word_start_index)
                            {
                                user_word_buffer = "";
                                word_start_index = index;
                            }
                        }
                        // Let standard backspace handling run to update colors/index
                    }
                    else
                    {
                        // Append to buffer
                        user_word_buffer += (char)key; // key is usually uppercase from GetKeyPressed
                    }
                }

                if (!handled_by_autocorrect)
                {
                    // Handle backspace
                    if (key == KEY_BACKSPACE && index > 0)
                    {
                        index--;
                        letter_colors[index] = WHITE;
                    }
                    else if (key != KEY_BACKSPACE)
                    {
                        total_keypresses++;
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
                            mistakes++;
                        }
                        index++;
                    }
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

            DrawText(TextFormat("Time: %.2f s", elapsed_time), 400 - 200, 10, 20, WHITE);

            // Calculate live stats
            float wpm = 0.0f;
            if (elapsed_time > 0)
            {
                wpm = (index / 5.0f) / (elapsed_time / 60.0f);
            }

            float accuracy = 100.0f;
            if (total_keypresses > 0)
            {
                accuracy = 100.0f * (float)(total_keypresses - mistakes) / (float)total_keypresses;
            }

            DrawText(TextFormat("WPM: %.1f", wpm), 400 - 50, 10, 20, WHITE);
            DrawText(TextFormat("Acc: %.1f%%", accuracy), 400 + 100, 10, 20, WHITE);

            // Draw Autocorrect status
            DrawText(TextFormat("Autocorrect (F1): %s", autocorrect ? "ON" : "OFF"), 10, 50, 20,
                     autocorrect ? GREEN : RED);

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

                    // Final stats display
                    DrawText("FINISHED!", 400 - 50, 40, 30, GREEN);
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
