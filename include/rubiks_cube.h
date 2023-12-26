#ifndef __RUBIKS_CUBE_H__
#define __RUBIKS_CUBE_H__

#include <cstdint>
#include <string>

using namespace std;

#define BITS_PER_COLOR 3
#define NUM_SCRAMBLE_MOVES 30

// Used for printing in color on terminal, see https://stackoverflow.com/a/3219471
#define TERM_COLOR_RESET "\e[0m"
#define TERM_COLOR_RED "\e[38;5;196m"
#define TERM_COLOR_GREEN "\e[38;5;46m"
#define TERM_COLOR_YELLOW "\e[38;5;226m"
#define TERM_COLOR_BLUE "\e[38;5;63m"
#define TERM_COLOR_WHITE "\e[38;5;231m"
#define TERM_COLOR_ORANGE "\e[38;5;208m"

const uint32_t MASK_N_BITS = 0xE;
const uint32_t MASK_N_BITS_IDX_0 = MASK_N_BITS << (31 - BITS_PER_COLOR);
const uint32_t MASK_N_BITS_IDX_1 = MASK_N_BITS_IDX_0 >> BITS_PER_COLOR;
const uint32_t MASK_N_BITS_IDX_2 = MASK_N_BITS_IDX_1 >> BITS_PER_COLOR;
const uint32_t MASK_N_BITS_IDX_3 = MASK_N_BITS_IDX_2 >> BITS_PER_COLOR;
const uint32_t MASK_N_BITS_IDX_4 = MASK_N_BITS_IDX_3 >> BITS_PER_COLOR;
const uint32_t MASK_N_BITS_IDX_5 = MASK_N_BITS_IDX_4 >> BITS_PER_COLOR;
const uint32_t MASK_N_BITS_IDX_6 = MASK_N_BITS_IDX_5 >> BITS_PER_COLOR;
const uint32_t MASK_N_BITS_IDX_7 = MASK_N_BITS_IDX_6 >> BITS_PER_COLOR;
const uint32_t MASK_N_BITS_IDX_8 = MASK_N_BITS_IDX_7 >> BITS_PER_COLOR;

const uint32_t TOP_MASK = 0xFF800000;
const uint32_t NOT_TOP_MASK = ~TOP_MASK;

const uint32_t BOTTOM_MASK = 0x3FE0;
const uint32_t NOT_BOTTOM_MASK = ~BOTTOM_MASK;

const uint32_t RIGHT_MASK = 0x381C0E0;
const uint32_t NOT_RIGHT_MASK = ~RIGHT_MASK;

const uint32_t LEFT_MASK = 0xE0703800;
const uint32_t NOT_LEFT_MASK = ~LEFT_MASK;

const inline uint32_t solvedFace(uint8_t color)
{
    return color << (32 - 1 * BITS_PER_COLOR) | color << (32 - 2 * BITS_PER_COLOR) |
           color << (32 - 3 * BITS_PER_COLOR) | color << (32 - 4 * BITS_PER_COLOR) |
           color << (32 - 5 * BITS_PER_COLOR) | color << (32 - 6 * BITS_PER_COLOR) |
           color << (32 - 7 * BITS_PER_COLOR) | color << (32 - 8 * BITS_PER_COLOR) | color << (32 - 9 * BITS_PER_COLOR);
}

const uint32_t SOLVED_FACE_0 = solvedFace(0x0);
const uint32_t SOLVED_FACE_1 = solvedFace(0x1);
const uint32_t SOLVED_FACE_2 = solvedFace(0x2);
const uint32_t SOLVED_FACE_3 = solvedFace(0x3);
const uint32_t SOLVED_FACE_4 = solvedFace(0x4);
const uint32_t SOLVED_FACE_5 = solvedFace(0x5);

enum MoveType { U1 = 0, D1 = 1, R1 = 2, L1 = 3, F1 = 4, B1 = 5 };

const char* moveTypeToString[6] = {"U1", "D1", "R1", "L1", "F1", "B1"};
const MoveType availableMoves[6] = {U1, D1, R1, L1, F1, B1};
const char* colors[6] = {TERM_COLOR_WHITE, TERM_COLOR_GREEN,  TERM_COLOR_RED,
                         TERM_COLOR_BLUE,  TERM_COLOR_ORANGE, TERM_COLOR_YELLOW};

struct RubiksCube {
    uint32_t data[6];

    /**
     * @brief Construct a new Rubiks Cube object
     * The layout in memory of the rubiks cube uses 6 32-bit unsigned integers to represent each face of the cube.
     * Each color on a face uses 3 bits (9 colors on a face total). The face is layed out in row-major order.
     * The faces are labeled as follows:
     *
     *         X X X
     *         X 0 X
     *         X X X
     *
     * X X X   X X X   X X X   X X X
     * X 1 X   X 2 X   X 3 X   X 4 X
     * X X X   X X X   X X X   X X X
     *
     *         X X X
     *         X 5 X
     *         X X X
     *
     * With the center cubie number being the index into data[] for that face.
     */
    RubiksCube()
    {
        data[0] = SOLVED_FACE_0;
        data[1] = SOLVED_FACE_1;
        data[2] = SOLVED_FACE_2;
        data[3] = SOLVED_FACE_3;
        data[4] = SOLVED_FACE_4;
        data[5] = SOLVED_FACE_5;
    }

    bool operator==(const RubiksCube& other) const
    {
        for (int i = 0; i < 6; ++i) {
            if (data[i] != other.data[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const RubiksCube& other) const { return !(*this == other); }

    uint8_t operator()(int face, int index) const
    {
        uint32_t mask = MASK_N_BITS_IDX_0 >> (index * BITS_PER_COLOR);
        return (data[face] & mask) >> (32 - (index + 1) * BITS_PER_COLOR);
    }

    bool isSolved()
    {
        return data[0] == SOLVED_FACE_0 && data[1] == SOLVED_FACE_1 && data[2] == SOLVED_FACE_2 &&
               data[3] == SOLVED_FACE_3 && data[4] == SOLVED_FACE_4 && data[5] == SOLVED_FACE_5;
    }

    /**
     * @brief Rotate face clockwise by 90 degrees
     * Original face flattened: 1 2 3 4 5 6 7 8 9
     * New face flattened:      7 4 1 8 5 2 9 6 3
     */
    inline uint64_t rotateFaceClockwise(uint32_t faceData)
    {
        uint32_t rotatedFaceData = ((faceData << (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_0) |
                                   ((faceData << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_1) |
                                   ((faceData >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_2) |
                                   ((faceData << (4 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_3) |
                                   (faceData & MASK_N_BITS_IDX_4) |
                                   ((faceData >> (4 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_5) |
                                   ((faceData << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_6) |
                                   ((faceData >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_7) |
                                   ((faceData >> (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_8);

        return rotatedFaceData;

        /*
        123
        456
        789

        741
        852
        963
        */
    }

    // inline uint64_t rotateFaceCounterClockwise(uint32_t faceData) {
    //     /**
    //      * @brief Rotate face counter clockwise by 90 degrees
    //      * Original face flattened: 1 2 3 4 5 6 7 8
    //      * New face flattened:      3 5 8 2 7 1 4 6
    //      */
    //     uint32_t rotatedFaceData =    ((faceData << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_0)
    //                                 | ((faceData << (3 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_1)
    //                                 | ((faceData << (5 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_2)
    //                                 | ((faceData >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_3)
    //                                 | ((faceData << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_4)
    //                                 | ((faceData >> (5 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_5)
    //                                 | ((faceData >> (3 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_6)
    //                                 | ((faceData >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_7);

    //     return rotatedFaceData;

    //     /*
    //     123
    //     4_5
    //     678

    //     358
    //     2_7
    //     146
    //     */
    // }

    inline void scramble()
    {
        for (int i = 0; i < NUM_SCRAMBLE_MOVES; ++i) {
            move(availableMoves[rand() % 6]);
        }
    }

    inline void move(MoveType type)
    {
        switch (type) {
            case U1: {
                // Rotate top face clockwise by 90 degrees
                uint32_t rotatedTop = rotateFaceClockwise(data[0]);

                uint32_t top1 = data[1] & TOP_MASK;
                uint32_t top2 = data[2] & TOP_MASK;
                uint32_t top3 = data[3] & TOP_MASK;
                uint32_t top4 = data[4] & TOP_MASK;

                data[1] = top2 | (data[1] & NOT_TOP_MASK);
                data[2] = top3 | (data[2] & NOT_TOP_MASK);
                data[3] = top4 | (data[3] & NOT_TOP_MASK);
                data[4] = top1 | (data[4] & NOT_TOP_MASK);
                data[0] = rotatedTop;

                break;
            }
            case D1: {
                // Rotate bottom face clockwise by 90 degrees
                uint32_t rotatedBottom = rotateFaceClockwise(data[5]);

                uint32_t bottom1 = data[1] & BOTTOM_MASK;
                uint32_t bottom2 = data[2] & BOTTOM_MASK;
                uint32_t bottom3 = data[3] & BOTTOM_MASK;
                uint32_t bottom4 = data[4] & BOTTOM_MASK;

                data[1] = bottom4 | (data[1] & NOT_BOTTOM_MASK);
                data[2] = bottom1 | (data[2] & NOT_BOTTOM_MASK);
                data[3] = bottom2 | (data[3] & NOT_BOTTOM_MASK);
                data[4] = bottom3 | (data[4] & NOT_BOTTOM_MASK);
                data[5] = rotatedBottom;

                break;
            }
            case R1: {
                // Rotate right face clockwise by 90 degrees
                uint32_t rotatedRight = rotateFaceClockwise(data[3]);

                uint32_t right0 = data[0] & RIGHT_MASK;
                uint32_t right2 = data[2] & RIGHT_MASK;
                uint32_t left4 = data[4] & LEFT_MASK;
                uint32_t right5 = data[5] & RIGHT_MASK;

                data[0] = right2 | (data[0] & NOT_RIGHT_MASK);
                data[2] = right5 | (data[2] & NOT_RIGHT_MASK);
                data[5] = (left4 >> BITS_PER_COLOR * 2) | (data[5] & NOT_RIGHT_MASK);
                data[4] = (right0 << BITS_PER_COLOR * 2) | (data[4] & NOT_LEFT_MASK);
                data[3] = rotatedRight;

                break;
            }
            case L1: {
                // Rotate left face clockwise by 90 degrees
                uint32_t rotatedLeft = rotateFaceClockwise(data[1]);

                uint32_t left0 = data[0] & LEFT_MASK;
                uint32_t left2 = data[2] & LEFT_MASK;
                uint32_t right4 = data[4] & RIGHT_MASK;
                uint32_t left5 = data[5] & LEFT_MASK;

                data[0] = (right4 << BITS_PER_COLOR * 2) | (data[0] & NOT_LEFT_MASK);
                data[2] = left0 | (data[2] & NOT_LEFT_MASK);
                data[5] = left2 | (data[5] & NOT_LEFT_MASK);
                data[4] = (left5 >> BITS_PER_COLOR * 2) | (data[4] & NOT_RIGHT_MASK);
                data[1] = rotatedLeft;

                break;
            }
            case F1: {
                // Rotate front face clockwise by 90 degrees
                uint32_t rotatedFront = rotateFaceClockwise(data[2]);

                uint32_t bottom0 = data[0] & BOTTOM_MASK;
                uint32_t left3 = data[3] & LEFT_MASK;
                uint32_t top5 = data[5] & TOP_MASK;
                uint32_t right1 = data[1] & RIGHT_MASK;

                uint32_t newBottom0 = ((right1 << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_6) |
                                      ((right1 >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_7) |
                                      ((right1 >> (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_8);

                uint32_t newLeft3 = ((bottom0 << (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_0) |
                                    ((bottom0 << (4 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_3) |
                                    ((bottom0 << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_6);

                uint32_t newTop5 = ((left3 << (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_0) |
                                   ((left3 << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_1) |
                                   ((left3 >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_2);

                uint32_t newRight1 = ((top5 >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_2) |
                                     ((top5 >> (4 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_5) |
                                     ((top5 >> (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_8);

                data[0] = newBottom0 | (data[0] & NOT_BOTTOM_MASK);
                data[3] = newLeft3 | (data[3] & NOT_LEFT_MASK);
                data[5] = newTop5 | (data[5] & NOT_TOP_MASK);
                data[1] = newRight1 | (data[1] & NOT_RIGHT_MASK);
                data[2] = rotatedFront;

                break;
            }
            case B1: {
                // Rotate back face clockwise by 90 degrees
                uint32_t rotatedBack = rotateFaceClockwise(data[4]);

                uint32_t top0 = data[0] & TOP_MASK;
                uint32_t right3 = data[3] & RIGHT_MASK;
                uint32_t bottom5 = data[5] & BOTTOM_MASK;
                uint32_t left1 = data[1] & LEFT_MASK;

                uint32_t newTop0 = ((right3 << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_0) |
                                   ((right3 << (4 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_1) |
                                   ((right3 << (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_2);

                uint32_t newRight3 = ((bottom5 << (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_2) |
                                     ((bottom5 << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_5) |
                                     ((bottom5 >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_8);

                uint32_t newBottom5 = ((left1 >> (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_6) |
                                      ((left1 >> (4 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_7) |
                                      ((left1 >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_8);

                uint32_t newLeft1 = ((top0 << (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_0) |
                                    ((top0 >> (2 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_3) |
                                    ((top0 >> (6 * BITS_PER_COLOR)) & MASK_N_BITS_IDX_6);

                data[0] = newTop0 | (data[0] & NOT_TOP_MASK);
                data[3] = newRight3 | (data[3] & NOT_RIGHT_MASK);
                data[5] = newBottom5 | (data[5] & NOT_BOTTOM_MASK);
                data[1] = newLeft1 | (data[1] & NOT_LEFT_MASK);
                data[4] = rotatedBack;

                break;
            }
        }
    }
};

// clang-format off
string fmtColorCubie(uint8_t color)
{
    return colors[color] + to_string(color) + TERM_COLOR_RESET;
}

void printCube(RubiksCube c) {
    cout << ">>>> Cube State" << endl;

    // Print face 0
    for (int i = 0; i < 3; ++i) {
        cout << "      ";
        for (int j = 0; j < 3; j++) {
            cout << fmtColorCubie(c(0, i * 3 + j)) << " ";
        }
        cout << "\n";
    }

    // Print middle faces
    for (int i = 0; i < 3; ++i) {
        for (int j = 1; j <= 4; ++j) {
            for (int k = 0; k < 3; ++k) {
                cout << fmtColorCubie(c(j, i * 3 + k)) << " ";
            }
        }
        cout << "\n";
    }

    // Print face 5
    for (int i = 0; i < 3; ++i) {
        cout << "      ";
        for (int j = 0; j < 3; j++) {
            cout << fmtColorCubie(c(5, i * 3 + j)) << " ";
        }
        cout << "\n";
    }
}
// clang-format on

const RubiksCube SOLVED_CUBE = RubiksCube();

#endif