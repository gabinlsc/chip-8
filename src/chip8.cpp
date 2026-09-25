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

void Chip8::executeOpcode(uint16_t opcode) {
    // Décomposition des nibbles pour décoder les arguments
    uint8_t x    = (opcode & 0x0F00u) >> 8u;
    uint8_t y    = (opcode & 0x00F0u) >> 4u;
    uint8_t n    = (opcode & 0x000Fu);
    uint8_t nn   = (opcode & 0x00FFu);
    uint16_t nnn = (opcode & 0x0FFFu);

    switch (opcode & 0xF000u) {
        case 0x0000:
            if (opcode == 0x00E0) {
                // 00E0 : CLS (Effacer l'écran)
                videoBuffer.fill(0);
            } else if (opcode == 0x00EE) {
                // 00EE : RET (Retour de sous-routine)
                if (sp > 0) {
                    --sp;
                    pc = stack[sp];
                }
            }
            break;

        case 0x1000:
            // 1NNN : JP addr (Saut inconditionnel)
            pc = nnn;
            break;

        case 0x2000:
            // 2NNN : CALL addr (Appel de fonction)
            if (sp < STACK_DEPTH) {
                stack[sp] = pc;
                ++sp;
                pc = nnn;
            }
            break;

        case 0x3000:
            // 3XNN : SE Vx, byte (Saute l'instruction suivante si Vx == NN)
            if (V[x] == nn) pc += 2;
            break;

        case 0x4000:
            // 4XNN : SNE Vx, byte (Saute l'instruction suivante si Vx != NN)
            if (V[x] != nn) pc += 2;
            break;

        case 0x5000:
            // 5XY0 : SE Vx, Vy (Saute l'instruction suivante si Vx == Vy)
            if (V[x] == V[y]) pc += 2;
            break;

        case 0x6000:
            // 6XNN : LD Vx, byte
            V[x] = nn;
            break;

        case 0x7000:
            // 7XNN : ADD Vx, byte (sans modifier le carry VF)
            V[x] += nn;
            break;

        case 0x8000:
            switch (n) {
                case 0x0: V[x] = V[y]; break;  // LD Vx, Vy
                case 0x1: V[x] |= V[y]; break; // OR Vx, Vy
                case 0x2: V[x] &= V[y]; break; // AND Vx, Vy
                case 0x3: V[x] ^= V[y]; break; // XOR Vx, Vy

                case 0x4: { // ADD Vx, Vy (avec retenue VF)
                    uint16_t sum = V[x] + V[y];
                    V[0xF] = (sum > 255u) ? 1 : 0;
                    V[x] = sum & 0xFFu;
                    break;
                }

                case 0x5: // SUB Vx, Vy (VF = NOT borrow)
                    V[0xF] = (V[x] >= V[y]) ? 1 : 0;
                    V[x] -= V[y];
                    break;

                case 0x6: // SHR Vx (décalage à droite)
                    V[0xF] = V[x] & 0x1u;
                    V[x] >>= 1;
                    break;

                case 0x7: // SUBN Vx, Vy
                    V[0xF] = (V[y] >= V[x]) ? 1 : 0;
                    V[x] = V[y] - V[x];
                    break;

                case 0xE: // SHL Vx (décalage à gauche)
                    V[0xF] = (V[x] & 0x80u) >> 7u;
                    V[x] <<= 1;
                    break;
            }
            break;

        case 0x9000:
            // 9XY0 : SNE Vx, Vy
            if (V[x] != V[y]) pc += 2;
            break;

        case 0xA000:
            // ANNN : LD I, addr
            indexRegister = nnn;
            break;

        case 0xB000:
            // BNNN : JP V0, addr
            pc = nnn + V[0];
            break;

        case 0xC000:
            // CXNN : RND Vx, byte
            V[x] = static_cast<uint8_t>(distByte(rng)) & nn;
            break;

        case 0xD000: {
            uint8_t xPos = V[x] % VIDEO_WIDTH;
            uint8_t yPos = V[y] % VIDEO_HEIGHT;
            V[0xF] = 0;

            for (unsigned int row = 0; row < n; ++row) {
                uint8_t spriteByte = memory[indexRegister + row];
                for (unsigned int col = 0; col < 8; ++col) {
                    if ((spriteByte & (0x80u >> col)) != 0) {
                        unsigned int targetX = xPos + col;
                        unsigned int targetY = yPos + row;

                        if (targetX < VIDEO_WIDTH && targetY < VIDEO_HEIGHT) {
                            size_t index = targetY * VIDEO_WIDTH + targetX;
                            if (videoBuffer[index] == 0xFFFFFFFF) {
                                V[0xF] = 1; 
                            }
                            videoBuffer[index] ^= 0xFFFFFFFF;
                        }
                    }
                }
            }
            break;
        }

        case 0xE000:
            if (nn == 0x9E) {
                if (keypad[V[x]]) pc += 2;
            } else if (nn == 0xA1) {
                if (!keypad[V[x]]) pc += 2;
            }
            break;

        case 0xF000:
            switch (nn) {
                case 0x07: V[x] = delayTimer; break;
                case 0x15: delayTimer = V[x]; break;
                case 0x18: soundTimer = V[x]; break;

                case 0x29:
                    indexRegister = FONTSET_START_ADDRESS + (5 * V[x]);
                    break;

                case 0x33:
                    memory[indexRegister]     = V[x] / 100;
                    memory[indexRegister + 1] = (V[x] / 10) % 10;
                    memory[indexRegister + 2] = V[x] % 10;
                    break;

                case 0x55:
                    for (uint8_t i = 0; i <= x; ++i) {
                        memory[indexRegister + i] = V[i];
                    }
                    break;

                case 0x65:
                    for (uint8_t i = 0; i <= x; ++i) {
                        V[i] = memory[indexRegister + i];
                    }
                    break;
            }
            break;
    }
}