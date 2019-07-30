#include <iostream>
#include "Grid.h"
int main()
{
    Grid xd(50,50);
    xd.CreatePath();
    xd.ClearPath();
    xd.CreatePath();
    xd.ClearPath();
    xd.CreatePath();
    xd.Draw("maze");
    return 0;
}