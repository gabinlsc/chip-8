#include "raylib.h"
#include "chip8.hpp"
#include <iostream>

void handleKeyboard(Chip8& chip8) {
    // Mappage clavier AZERTY standard 
    // 1 2 3 4   ->  1 2 3 C
    // A Z E R   ->  4 5 6 D
    // Q S D F   ->  7 8 9 E
    // W X C V   ->  A 0 B F
    chip8.setKey(0x1, IsKeyDown(KEY_ONE));
    chip8.setKey(0x2, IsKeyDown(KEY_TWO));
    chip8.setKey(0x3, IsKeyDown(KEY_THREE));
    chip8.setKey(0xC, IsKeyDown(KEY_FOUR));

    chip8.setKey(0x4, IsKeyDown(KEY_A));
    chip8.setKey(0x5, IsKeyDown(KEY_Z));
    chip8.setKey(0x6, IsKeyDown(KEY_E));
    chip8.setKey(0xD, IsKeyDown(KEY_R));

    chip8.setKey(0x7, IsKeyDown(KEY_Q));
    chip8.setKey(0x8, IsKeyDown(KEY_S));
    chip8.setKey(0x9, IsKeyDown(KEY_D));
    chip8.setKey(0xE, IsKeyDown(KEY_F));

    chip8.setKey(0xA, IsKeyDown(KEY_W));
    chip8.setKey(0x0, IsKeyDown(KEY_X));
    chip8.setKey(0xB, IsKeyDown(KEY_C));
    chip8.setKey(0xF, IsKeyDown(KEY_V));
}

int main(int argc, char* argv[]) {
    const std::string romPath = (argc > 1) ? argv[1] : "pong.ch8";

    Chip8 chip8;
    if (!chip8.loadRom(romPath)) {
        std::cerr << "Erreur: impossible de charger la ROM " << romPath << "\n";
        return 1;
    }

    constexpr int scale = 15;
    constexpr int screenWidth = Chip8::VIDEO_WIDTH * scale;
    constexpr int screenHeight = Chip8::VIDEO_HEIGHT * scale;

    InitWindow(screenWidth, screenHeight, "CHIP-8 Interpreter - C++20");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        handleKeyboard(chip8);

        // Fréquence standard : environ 10 instructions par frame (600 Hz)
        for (int i = 0; i < 10; ++i) {
            chip8.cycle();
        }
        chip8.decrementTimers();

        const auto& fb = chip8.getFrameBuffer();

        BeginDrawing();
        ClearBackground(BLACK);

        for (unsigned int y = 0; y < Chip8::VIDEO_HEIGHT; ++y) {
            for (unsigned int x = 0; x < Chip8::VIDEO_WIDTH; ++x) {
                if (fb[y * Chip8::VIDEO_WIDTH + x] != 0) {
                    DrawRectangle(x * scale, y * scale, scale, scale, RAYWHITE);
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}