#include <iostream>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <unordered_map>
#include <climits>
#include <cmath>
#include <raylib.h>
#include <fftw3.h>

const unsigned int N = 128;
fftw_complex in[N], out[N];
fftw_plan plan;
float freqs[N];

const int screenWidth = 800;
const int screenHeight = 450;

struct Frame {
    float left;
    float right;
};

void callback(void *bufferData, unsigned int frames) {
    if (frames < N) return;
    Frame *frameBuffer = (Frame*)bufferData;
    for (size_t i = 0; i < N; i++) {
        in[i][0] = (double)frameBuffer[i].left;
        in[i][1] = 0.0;
    }
    
    fftw_execute(plan);

    for (size_t i = 0; i < N; i++) {
        freqs[i] = 30*sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1]);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Invalid # of arguments: " << argc << std::endl;
        return 1;
    }
    //std::cout << "hello world" << std::endl;

    plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_MEASURE);

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
        
        for (size_t i = 1; i*2 < N; i++) {
            DrawLine(
                (log((float)(i-1)*2/N)/log(10)+1)*screenWidth, screenHeight - freqs[i-1],
                (log((float)i*2/N)/log(10)+1)*screenWidth, screenHeight - freqs[i],
                RED
            );
        }
        
        
        for (size_t i = 0; i*2 < N; i++) {
            DrawLine(
                (log((float)i*2/N)/log(10)+1)*screenWidth, screenHeight - freqs[i],
                (log((float)i*2/N)/log(10)+1)*screenWidth, screenHeight,
                RED
            );
        }
        
        /*
        for (size_t i = 0; 2*i < N; i++) {
            DrawRectangle(
                (float)2*i/N*screenWidth, screenHeight - freqs[i],
                (float)2*screenWidth/N + 1, freqs[i],
                RED
            );
        }
        */
        EndDrawing();
    }

    fftw_destroy_plan(plan);

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}