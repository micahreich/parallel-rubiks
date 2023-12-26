#include <cassert>
#include <iostream>

#include "cycle_timer.h"
#include "rubiks_cube.h"

using namespace std;

int main()
{
    RubiksCube cube;
    printCube(cube);

    cube.move(F1);
    printCube(cube);

    return 0;
}