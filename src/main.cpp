#include <iostream>
#include "Grid.h"
int main()
{
    const int size = 800;
    Grid grid(size, size);
    grid.Generate();
    grid.Draw("maze");
    return 0;
}