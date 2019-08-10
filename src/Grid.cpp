#include <ctime>
#include <cstdlib>
#include <iostream>

#include "Grid.h"
#include "Image.h"

Grid::Grid(unsigned int widthArg, unsigned int heightArg)
:width(widthArg), height(heightArg), gridW(2*width +1), gridH(2*height + 1)
{
    array = new tileState[gridW * gridH];
    SetStates();
}
Grid::Grid(const Grid& arg)
{
    width = arg.GetWidth();
    height = arg.GetHeight();
    gridW = 2 * width + 1;
    gridH = 2 * height + 1;
    array = new tileState[gridW * gridH];
    SetStates();
}
Grid::~Grid()
{
    delete[] array;
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
//sets the initial maze parameters
{
    srand(time(NULL));
    for(unsigned int y = 1; y < gridH - 1; ++y)
        for(unsigned int x = 1; x < gridW - 1; ++x) //for each tile inside of the maze:
        {
            array[y * gridW + x] = WALL;
        }
    for(unsigned int y = 0; y < gridH; ++y)//each tile on the border, its a wall
    {
        array[y * gridW] = BORDER;
        array[y * gridW + gridW - 1] = BORDER;
    }
    for(unsigned int x = 0; x < gridW; ++x)//each tile on the border, its a wall
    {
        array[x] = BORDER;
        array[gridH * gridW - 1 - x] = BORDER;
    }
}
void Grid::Dfs(unsigned int xPos, unsigned int yPos)
//recursively generates the maze
{
    array[yPos*gridW + xPos] = CORRIDOR;
    short dx;
    short dy;
    short index;
    short freeNeigh;
    while((freeNeigh = CheckSteps(xPos, yPos)) > 0)
    {
        index = rand()%freeNeigh;
        for(short i = 0; i < 4; ++i)
        {
            dx = (i%2)  *  (2-i);
            dy = ((i+1)%2)*(i-1);
            if(array[(yPos + dy) * gridW + xPos + dx] == WALL)
                if(array[(yPos + 2*dy) * gridW + xPos + 2*dx] == WALL)
                {
                    if(index < 1) break;
                    else --index;
                }
        }
        array[(yPos + dy) * gridW + xPos + dx] = CORRIDOR;
        Dfs(xPos + 2*dx, yPos + 2*dy);
    }
}
unsigned int Grid::CheckSteps(unsigned int xPos, unsigned int yPos) const
//checks how many neighbours can be visited
{
    unsigned int sum = 0;
    for(unsigned int i = 0; i < 4; ++i)
    {
        int dx = (i%2)  *  (2-i);
        int dy = ((i+1)%2)*(i-1);
        if(array[(yPos + dy) * gridW + xPos + dx] == WALL)
            if(array[(yPos + 2*dy) * gridW + xPos + 2*dx] == WALL)
                ++sum;
    }
    return sum;
}
void Grid::Draw(std::string fileName) const
//draws the maze exporting to a bitmap
{
    Image result(gridW, gridH);
    result.SetAll(128, 128, 128);
    for(unsigned int y = 0; y < gridH; ++y)
        for(unsigned int x = 0; x < gridW; ++x)
        {
            tileState& ref = array[y * gridW + x];
            switch(ref)
            {
            case WALL:
                result.SetPixel(x,y,0,0,0);
                break;
            case CORRIDOR:
                result.SetPixel(x,y,-1,-1,-1);
                break;
            case BORDER:
                result.SetPixel(x,y,0,0,-1);
                break;
            }
        }
    result.Export(fileName);
}
void Grid::Generate()
{
    Dfs(width, height);
}
Grid Grid::operator=(const Grid& arg)
{
    delete[] array;
    width = arg.GetWidth();
    height = arg.GetHeight();
    gridW = 2 * width + 1;
    gridH = 2 * height + 1;
    array = new tileState[gridH * gridW];
    SetStates();
    return *this;
}