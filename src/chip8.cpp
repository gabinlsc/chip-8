#include "chip8.hpp"
#include <fstream>
#include <chrono>

namespace {
    const std::array<uint8_t, 80> FONTSET = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    constexpr uint16_t FONTSET_START_ADDRESS = 0x50;
}

Chip8::Chip8() 
    : rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count())),
      distByte(0, 255) {
    loadFontset();
}

void Chip8::loadFontset() {
    for (size_t i = 0; i < FONTSET.size(); ++i) {
        memory[FONTSET_START_ADDRESS + i] = FONTSET[i];
    }
}

bool Chip8::loadRom(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize size = file.tellg();
    if (size > static_cast<std::streamsize>(MEMORY_SIZE - START_ADDRESS)) {
        return false;
    }

    file.seekg(0, std::ios::beg);
    file.read(reinterpret_cast<char*>(&memory[START_ADDRESS]), size);
    return true;
}

const std::array<uint32_t, Chip8::VIDEO_WIDTH * Chip8::VIDEO_HEIGHT>& Chip8::getFrameBuffer() const {
    return videoBuffer;
}

void Chip8::setKey(uint8_t keyIndex, bool pressed) {
    if (keyIndex < KEY_COUNT) {
        keypad[keyIndex] = pressed ? 1 : 0;
    }
}

void Chip8::decrementTimers() {
    if (delayTimer > 0) --delayTimer;
    if (soundTimer > 0) --soundTimer;
}

void Chip8::cycle() {
    // 1. Fetch 
    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;

    // 2. Decode & Execute 
    executeOpcode(opcode);
}

void Chip8::executeOpcode(uint16_t /*opcode*/) {
}