#include <iostream>
#include <cassert>

#include "rubiks_cube.h"
#include "cycle_timer.h"

using namespace std;

void printBinary(uint32_t num) {
    for (int i = 31; i >= 0; i--) {
        int k = num >> i;
        if (k & 1)
            cout << "1";
        else
            cout << "0";
        
        if (i % BITS_PER_COLOR == BITS_PER_COLOR - 1) cout << " ";
    }
    cout << endl;
}

int main() {
    RubiksCube cube, solvedCube;

    // TEST: initial solved cube
    cube.printCube();

    // TEST: try all rotations
    for (auto m : availableMoves) {
        cube = RubiksCube();
        printf(">>>>>> Testing Move Type %s\n", moveTypeToString[m]);
        cube.move(m);
        for (int i = 0; i < 3; ++i) cube.move(m);
        assert(cube == solvedCube && "Cube is not re-solved after cyclic 4 moves");

        cube.printCube();
    }

    // TEST: time how long executing moves takes
    const long long NUM_MOVES = 10000000;
    double startTime = CycleTimer::currentSeconds();

    for (int i = 0; i < NUM_MOVES; ++i) {
        for (auto m : availableMoves)
            cube.move(m);
    }

    double endTime = CycleTimer::currentSeconds();
    double duration = endTime - startTime;

    cout << "Time to execute " << NUM_MOVES << " moves: " << duration << " s" << endl;
    cout << "Average time per move " << 1e9 * duration / (6 * NUM_MOVES) << " ns" << endl;
    
    // // TEST: rotation U1
    // cube.move(U1);
    // cube.printCube("U1 (1x)");

    // for (int i = 0; i < 3; ++i) cube.move(U1);
    // cube.printCube("U1 (4x)");

    // // TEST: rotation D1
    // cube.move(D1);
    // cube.printCube("D1 (1x)");

    // for (int i = 0; i < 3; ++i) cube.move(D1);
    // cube.printCube("D1 (4x)");
    
    return 0;
}