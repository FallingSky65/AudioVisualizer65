#include <iostream>
#include <raylib.h>

const int screenWidth = 800;
const int screenHeight = 450;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Invalid # of arguments: " << argc << std::endl;
        return 1;
    }
    //std::cout << "hello world" << std::endl;

    InitWindow(screenWidth, screenHeight, "AudioVisualizer65");

    InitAudioDevice();

    Music music = LoadMusicStream(argv[1]);
    std::cout << "Loaded " << argv[1] << std::endl;

    music.looping = true;
    bool paused = false;

    PlayMusicStream(music);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        if (IsKeyPressed(KEY_P)) {
            paused = !paused;
            if (paused) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        //DrawText("made a window lmao", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}