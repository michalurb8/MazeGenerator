#include <ctime>
#include <cstdlib>
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
    srand(time(NULL));
    doorsOpen = 2*width*height - height - width - 1;
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    for(unsigned int y = 0; y < gridH; ++y)
        for(unsigned int x = 0; x < gridW; ++x)
        {
            tileState& ref = structure[y * gridW + x];
            if(!x || !y || x == gridW - 1 || y == gridH - 1) //setting borders to walls
                ref = Grid::WALL;
            if((x + y) % 2)
                ref = Grid::DOOR_OPENED; //if coordinates have oppostie parity, tile is a closed door
            if(x%2 && y%2)
                ref = Grid::CORRIDOR; //if both coordinates are odd, tile is a corridor
            if(x%2==0 && y%2==0)
                ref = Grid::WALL; //if both coordinates are even, tile is a wall
        }
}
bool Grid::CheckStep(unsigned int mazeX, unsigned int mazeY) const
{
    unsigned int xPos = 2*mazeX + 1;
    unsigned int yPos = 2*mazeY + 1;
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    if(structure[yPos * gridW + xPos] != CORRIDOR) return false;
    if(structure[(yPos - 1) * gridW + xPos] == DOOR_OPENED) return true;
    if(structure[yPos * gridW + (xPos + 1)] == DOOR_OPENED) return true;
    if(structure[(yPos + 1) * gridW + xPos] == DOOR_OPENED) return true;
    if(structure[yPos * gridW + (xPos - 1)] == DOOR_OPENED) return true;
    return false;
}
Grid::direction Grid::TakeStep(unsigned int mazeX, unsigned int mazeY)
{
    unsigned int xPos = 2*mazeX + 1;
    unsigned int yPos = 2*mazeY + 1;
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    int sum = 0, index = 0;
    if(structure[(yPos - 1) * gridW + xPos] == DOOR_OPENED) ++sum;
    if(structure[yPos * gridW + (xPos + 1)] == DOOR_OPENED) ++sum;
    if(structure[(yPos + 1) * gridW + xPos] == DOOR_OPENED) ++sum;
    if(structure[yPos * gridW + (xPos - 1)] == DOOR_OPENED) ++sum;
    std::cout << "Free neighbours: " << sum << std::endl;
    index = rand()%sum;
    if(structure[(yPos - 1) * gridW + xPos] == DOOR_OPENED)
    {
        if(index < 1)
        {
            structure[(yPos+1) * gridW + xPos] = PASSING;
            --doorsOpen;
            return UP;
        }
        else --index;
    }
    if(structure[yPos * gridW + (xPos + 1)] == DOOR_OPENED)
    {
        if(index < 1)
        {
            structure[yPos * gridW + xPos + 1] = PASSING;
            --doorsOpen;
            return RIGHT;
        }
        else --index;
    }
    if(structure[(yPos + 1) * gridW + xPos] == DOOR_OPENED)
    {
        if(index < 1)
        {
            structure[(yPos+1) * gridW + xPos] = PASSING;
            --doorsOpen;
            return DOWN;
        }
        else --index;
    }
    if(structure[yPos * gridW + (xPos - 1)] == DOOR_OPENED)
    {
        if(index < 1)
        {
            structure[yPos * gridW + xPos - 1] = PASSING;
            --doorsOpen;
            return LEFT;
        }
        else --index;
    }
    return UP;
}
bool Grid::CreatePath()
{
    if(!doorsOpen) return false;
    unsigned int xPos = rand()%width;
    unsigned int yPos = rand()%height;
    std::cout << "Random position: " << xPos << " " << yPos << std::endl;
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    std::cout << "Choosing path-start:" << std::endl;
    while(!CheckStep(xPos, yPos))
    {
        xPos = xPos + 1;
        yPos = (yPos + (xPos/width)) % height;
        xPos = xPos%width;
    }
    std::cout << "Chosen: " << xPos << " " << yPos << std::endl;
    std::cout << "Following a path:" << std::endl;
    while(CheckStep(xPos, yPos))
    {
        std::cout << "X: " << xPos << ", Y: " << yPos << std::endl;
        direction next = TakeStep(xPos, yPos);
        std::cout << "Next step direction: " << (int)next << std::endl;
        switch(next)
        {
        case UP:
            yPos = (yPos-1)%height;
            break;
        case RIGHT:
            xPos = (xPos+1)%width;
            break;
        case DOWN:
            yPos = (yPos+1)%height;
            break;
        case LEFT:
            xPos = (xPos-1)%width;
        }
        std::cout << "Next step hopefully: X: " << xPos << ", Y: " << yPos << std::endl;
    }
}
void Grid::ClearPath()
{

}
void Grid::Draw() const
{
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    Image result(gridW, gridH);
    result.SetAll(128, 128, 128);
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
            case PASSING:
                result.SetPixel(x,y,0,0,-1);
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