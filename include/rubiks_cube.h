#ifndef __RUBIKS_CUBE_H__
#define __RUBIKS_CUBE_H__

#include <cstdint>

enum MoveType { U1 = 0, D1 = 1, R1 = 2, L1 = 3, F1 = 4, B1 = 5 };

extern const char* moveTypeToString[6];
extern const MoveType availableMoves[6];

class RubiksCube {
private:
    uint32_t data[6];
public:
    RubiksCube();
    bool isSolved();
    void scramble();
    void move(MoveType type);

    bool operator==(const RubiksCube& other);
    bool operator!=(const RubiksCube& other);
    uint8_t operator()(int face, int index) const;
};

extern const RubiksCube SOLVED_CUBE;

void printCube(const RubiksCube& cube);

#endif