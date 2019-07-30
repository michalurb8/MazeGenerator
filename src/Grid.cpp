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
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    for(unsigned int y = 1; y < gridH - 1; ++y)
        for(unsigned int x = 1; x < gridW - 1; ++x) //for each tile inside of the maze:
        {
            tileState& ref = structure[y * gridW + x];
            if(x%2==0 && y%2==0) 
                ref = Grid::PILLAR;
            else
                ref = Grid::CORRIDOR; //if both coordinates are even, tile is a pillar
        }
    for(unsigned int y = 0; y < gridH; ++y)//each tile on the border, its a wall
    {
        structure[y * gridW] = Grid::WALL;
        structure[y * gridW + gridW - 1] = Grid::WALL;
    }
    for(unsigned int x = 0; x < gridW; ++x)//each tile on the border, its a wall
    {
        structure[x] = Grid::WALL;
        structure[gridH * gridW - 1 - x] = Grid::WALL;
    }
}
unsigned int Grid::CheckSteps(unsigned int xPos, unsigned int yPos) const
{
    unsigned int gridW = 2*width + 1, sum = 0;
    if(structure[yPos * gridW + xPos] != PILLAR) return 0;
    for(unsigned int i = 0; i < 4; ++i)
    {
        unsigned int dx = (i%2)  *  (2-i);
        unsigned int dy = ((i+1)%2)*(i-1);
        if(structure[(yPos + dy) * gridW + xPos + dx] == CORRIDOR)
            if(structure[(yPos + dy + dy) * gridW + xPos + dx + dx] != PATH)
                ++sum;
    }
    return sum;
}
bool Grid::CheckStuck(unsigned int xPos, unsigned int yPos) const
{
    unsigned int gridW = 2*width + 1;
    if(structure[yPos * gridW + xPos] != PILLAR)
    {
        std::cout << "NOT A PILLAR??? CHECK STUCK" << std::endl;
        return false;
    }
    for(unsigned int i = 0; i < 4; ++i)
    {
        unsigned int dx = (i%2)  *  (2-i);
        unsigned int dy = ((i+1)%2)*(i-1);
        if(structure[(yPos + dy + dy) * gridW + xPos + dx + dx] != PATH) return false;
    }
    return true;
}
Grid::direction Grid::TakeStep(unsigned int xPos, unsigned int yPos)
{
    unsigned int gridW = 2*width + 1;
    unsigned int sum = 0, index = 0;
    sum = CheckSteps(xPos, yPos);
    index = rand()%sum;
    structure[yPos * gridW + xPos] = PATH;
    for(unsigned int i = 0; i < 4; ++i)
    {
        unsigned int dx = (i%2)*(2-i);
        unsigned int dy = ((i+1)%2)*(i-1);
        if(structure[(yPos + dy) * gridW + (xPos + dx)] == CORRIDOR)
            if(structure[(yPos + dy + dy) * gridW + (xPos + dx + dx)] != PATH)
            {
                if(index < 1)
                {
                    structure[(yPos + dy) * gridW + (xPos + dx)] = PATH;
                    return direction(i);
                }
                else --index;
            }
    }
    std::cout << "DIDNT PICK DIRECTION???" << std::endl;
    return UP;
}
bool Grid::CreatePath()
{
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    unsigned int xPos = rand()%(width  - 1);
    unsigned int yPos = rand()%(height - 1);
    xPos = xPos*2 + 2;
    yPos = yPos*2 + 2; //Choosing random pillar/wall coordinates
    while(!CheckSteps(xPos, yPos))//Find first non wall coordinates
    {
        xPos += 2;
        if(xPos == gridW - 1)
        {
            yPos += 2;
            if(yPos == gridH - 1)
            {
                yPos = 2;
            }
            xPos = 2;
        }
    }
    while(structure[yPos*gridW + xPos] != WALL)//Closed doors become passages in a chain until wall hit
    {
        if(CheckStuck(xPos, yPos)) //If can't move and didn't touch wall, pick furthest path tile in random dir and go on
        {
            structure[yPos*gridW + xPos] = PATH;
            direction jump = direction(rand()%4);
            switch(jump)
            {
            case UP:
                for(unsigned int y = 0; y <= yPos; y+=2)
                    if(structure[y * gridW + xPos] == PATH)
                    {
                        yPos = y;
                        structure[yPos * gridW + xPos] = PILLAR;
                        break;
                    }
                break;
            case RIGHT:
                for(unsigned int x = gridW-1; x >= xPos; x-=2)
                    if(structure[yPos * gridW + x] == PATH)
                    {
                        xPos = x;
                        structure[yPos * gridW + xPos] = PILLAR;
                        break;
                    }
                break;
            case DOWN:
                for(unsigned int y = gridH-1; y >= yPos; y-=2)
                    if(structure[y * gridW + xPos] == PATH)
                    {
                        yPos = y;
                        structure[yPos * gridW + xPos] = PILLAR;
                        break;
                    }
                break;
            case LEFT:
                for(unsigned int x = 0; x <= xPos; x+=2)
                    if(structure[yPos * gridW + x] == PATH)
                    {
                        xPos = x;
                        structure[yPos * gridW + xPos] = PILLAR;
                        break;
                    }
                break;
            }
        }
        direction next = TakeStep(xPos, yPos);
        switch(next)
        {
        case UP:
            yPos = (yPos-2);
            break;
        case RIGHT:
            xPos = (xPos+2);
            break;
        case DOWN:
            yPos = (yPos+2);
            break;
        case LEFT:
            xPos = (xPos-2);
        }
    }
    return true;
}
void Grid::ClearPath()
{
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    for(unsigned int y = 0; y < gridH; ++y)
        for(unsigned int x = 0; x < gridW; ++x)
        {
            tileState& ref = structure[y * gridW + x];
            if(ref == PATH) ref = WALL;
        }
}
void Grid::Draw(std::string fileName) const
{
    unsigned int gridW = 2*width + 1;
    unsigned int gridH = 2*height + 1;
    Image result(gridW, gridH);
    result.SetAll(128, 128, 128);
    for(unsigned int y = 0; y < gridH; ++y)
        for(unsigned int x = 0; x < gridW; ++x)
        {
            tileState& ref = structure[y * gridW + x];
            switch(ref)
            {
            case WALL:
                result.SetPixel(x,y,50,50,50);
                break;
            case CORRIDOR:
                result.SetPixel(x,y,-1,-1,-1);
                break;
            case PILLAR:
                result.SetPixel(x,y,120,120,-1);
                break;
            case PATH:
                result.SetPixel(x,y,0,0,-1);
                break;
            }
        }
    result.Export(fileName);
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