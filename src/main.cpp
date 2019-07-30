#include <iostream>
#include "Grid.h"
int main()
{
    Grid xd(300,300);
    xd.Fill();
    xd.Draw("maze");
    return 0;
}