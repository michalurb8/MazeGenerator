#include <iostream>
#include "Grid.h"
#include "Image.h"
Grid::Grid(unsigned int widthArg, unsigned int heightArg)
:width(widthArg), height(heightArg)
{
    structure = new tileState[(2*width + 1) * (2*height + 1)];
    SetStates();
}
Grid::Grid(const Grid& arg)
{
    width = arg.GetWidth();
    height = arg.GetHeight();
    structure = new tileState[(2*width + 1) * (2*height + 1)];
    SetStates();
}
Grid::~Grid()
{
    delete[] structure;
}
unsigned int Grid::GetWidth() const
{
    return width;
}
unsigned int Grid::GetHeight() const
{
    return height;
}
void Grid::SetStates()
{
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    for(unsigned int y = 0; y < gridH; ++y)
        for(unsigned int x = 0; x < gridW; ++x)
        {
            tileState& ref = structure[y * gridW + x];
            if(!x || !y || x == gridW - 1 || y == gridH - 1) //setting borders to walls
                ref = Grid::WALL;
            if((x + y) % 2)
                ref = Grid::DOOR_CLOSED; // if coordinates have oppostie parity, tile is a closed door
            if(x%2 && y%2)
                ref = Grid::CORRIDOR; // if both coordinates are odd, tile is a corridor
        }
    for(unsigned int y = 0; y < gridH; ++y)
        for(unsigned int x = 0; x < gridW; ++x)
        {
            tileState& ref = structure[y * gridW + x];
            if(ref == Grid::DOOR_CLOSED)
                ref = rand()%2 ? Grid::DOOR_CLOSED : DOOR_OPENED;
        }
}
bool Grid::CheckStep(unsigned int mazeX, unsigned int mazeY) const
{
    unsigned int xPos = 2*mazeX + 1;
    unsigned int yPos = 2*mazeY + 1;
    if(structure[yPos * width + xPos] != CORRIDOR) return false;
    if(structure[(yPos - 1) * width + xPos] == DOOR_OPENED) return true;
    if(structure[yPos * width + (xPos + 1)] == DOOR_OPENED) return true;
    if(structure[(yPos + 1) * width + xPos] == DOOR_OPENED) return true;
    if(structure[yPos * width + (xPos - 1)] == DOOR_OPENED) return true;
    return false;
}
Grid::direction Grid::TakeStep(unsigned int mazeX, unsigned int mazeY)
{
    unsigned int xPos = 2*mazeX + 1;
    unsigned int yPos = 2*mazeY + 1;
    unsigned int sum = 0;
    if(structure[(yPos - 1) * width + xPos] == DOOR_OPENED) ++sum;
    if(structure[yPos * width + (xPos + 1)] == DOOR_OPENED) ++sum;
    if(structure[(yPos + 1) * width + xPos] == DOOR_OPENED) ++sum;
    if(structure[yPos * width + (xPos - 1)] == DOOR_OPENED) ++sum;
    return UP;
}
void Grid::Draw() const
{
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    Image result(gridW, gridH);
    result.SetAll(-1, -1, -1);
    for(unsigned int x = 0; x < gridW; ++x)
    {
        for(unsigned int y = 0; y < gridH; ++y)
        {
            tileState& ref = structure[y * gridW + x];
            switch(ref)
            {
            case WALL:
                result.SetPixel(x,y,0,0,0);
                break;
            case DOOR_CLOSED:
                result.SetPixel(x,y,-1,0,0);
                break;
            case DOOR_OPENED:
                result.SetPixel(x,y,0,-1,0);
                break;
            case CORRIDOR:
                result.SetPixel(x,y,-1,-1,-1);
                break;
            }
        }
    }
    result.Export("maze");
}
Grid Grid::operator=(const Grid& arg)
{
    delete[] structure;
    width = arg.GetWidth();
    height = arg.GetHeight();
    structure = new tileState[(2*width + 1) * (2*height + 1)];
    SetStates();
    return *this;
}