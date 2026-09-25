#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <random>

class Chip8 {
public:
    // Constantes matérielles de la CHIP-8
    static constexpr unsigned int VIDEO_WIDTH = 64;
    static constexpr unsigned int VIDEO_HEIGHT = 32;
    static constexpr unsigned int MEMORY_SIZE = 4096;
    static constexpr unsigned int REGISTER_COUNT = 16;
    static constexpr unsigned int STACK_DEPTH = 16;
    static constexpr unsigned int KEY_COUNT = 16;
    static constexpr uint16_t START_ADDRESS = 0x200; 

    Chip8();

    bool loadRom(const std::string& filepath);
    void cycle();

    const std::array<uint32_t, VIDEO_WIDTH * VIDEO_HEIGHT>& getFrameBuffer() const;
    void setKey(uint8_t keyIndex, bool pressed);
    void decrementTimers();

private:
    // Mémoire & Registres
    std::array<uint8_t, MEMORY_SIZE> memory{};
    std::array<uint8_t, REGISTER_COUNT> V{}; 
    uint16_t indexRegister{0};              
    uint16_t pc{START_ADDRESS};             

    // Pile d'appels
    std::array<uint16_t, STACK_DEPTH> stack{};
    uint8_t sp{0}; 

    // Timers cadencés à 60 Hz
    uint8_t delayTimer{0};
    uint8_t soundTimer{0};

    // Clavier hexadécimal et tampon vidéo monochrome
    std::array<uint8_t, KEY_COUNT> keypad{};
    std::array<uint32_t, VIDEO_WIDTH * VIDEO_HEIGHT> videoBuffer{};

    // Générateur aléatoire
    std::mt19937 rng;
    std::uniform_int_distribution<unsigned int> distByte;

    void loadFontset();
    void executeOpcode(uint16_t opcode);
};