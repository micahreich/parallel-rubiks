#include <cassert>
#include <iostream>

#include "rubiks_cube.h"
#include "cycle_timer.h"

using namespace std;

int main(int argc, char **argv) {
    srand(time(0));
    RubiksCube cube;

    // TEST: initial solved cube
    printf(">>>>>>>> Initial Solved Cube\n");
    cube.printCube();

    // TEST: try all rotations
    printf(">>>>>>>> Cube Rotations\n");
    for (auto m : availableMoves) {
        cube = RubiksCube();
        cube.move(m);
        for (int i = 0; i < 3; ++i) cube.move(m);
        assert(cube == SOLVED_CUBE && "Cube is not re-solved after cyclic 4 moves");
    }

    // TEST: time how long executing moves takes
    printf(">>>>>>>> Cube Rotations Timing\n");
    const long long NUM_MOVES = 10000000;
    double startTime = CycleTimer::currentSeconds();

    for (int i = 0; i < NUM_MOVES; ++i) {
        for (auto m : availableMoves) cube.move(m);
    }

    double endTime = CycleTimer::currentSeconds();
    double duration = endTime - startTime;

    cout << "Time to execute " << NUM_MOVES << " moves: " << duration << " s" << endl;
    cout << "Average time per move " << 1e9 * duration / (6 * NUM_MOVES) << " ns" << endl;
    
    // TEST: scramble the cube and make sure the centers dont move
    printf(">>>>>>>> Cube Scramble\n");

    cube = RubiksCube();
    cube.scramble();

    assert(cube.getCubieColor(0, 4) == 0);
    assert(cube.getCubieColor(1, 4) == 1);
    assert(cube.getCubieColor(2, 4) == 2);
    assert(cube.getCubieColor(3, 4) == 3);
    assert(cube.getCubieColor(4, 4) == 4);
    assert(cube.getCubieColor(5, 4) == 5);

    return 0;
}