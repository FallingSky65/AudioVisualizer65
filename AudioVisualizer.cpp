#include <iostream>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <raylib.h>

uint32_t audioData[1024];
size_t audioDataLen = 0;

const int screenWidth = 800;
const int screenHeight = 450;

void callback(void *bufferData, unsigned int frames) {
    std::memcpy(audioData, bufferData, frames*sizeof(uint32_t));
    audioDataLen = frames;
}

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

    assert(music.stream.channels == 2 && music.stream.sampleSize == 16);

    music.looping = true;
    bool paused = false;

    PlayMusicStream(music);
    AttachAudioStreamProcessor(music.stream, callback);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        if (IsKeyPressed(KEY_P)) {
            paused = !paused;
            if (paused) PauseMusicStream(music);
            else ResumeMusicStream(music);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (size_t i = 0; i < audioDataLen; i++) {
            uint16_t leftChannel = audioData[i] % (1 << 16);
            uint16_t rightChannel = audioData[i] >> 16;
            float normalizedAverage = ((float)leftChannel+(float)rightChannel)/(1 << 17);
            DrawLine((0.5f+i)/audioDataLen*screenWidth, screenHeight - normalizedAverage*screenHeight, (0.5f+i)/audioDataLen*screenWidth, screenHeight, RED);
        }
        EndDrawing();
    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}