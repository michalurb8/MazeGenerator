#pragma once

class Grid
{
    enum tileState {CORRIDOR = 0, WALL, DOOR_OPENED, DOOR_CLOSED};
    enum direction {UP = 0, RIGHT, DOWN, LEFT};
    unsigned int width, height;
    tileState* structure; 
public:
    Grid() = delete;
    Grid(unsigned int widthArg, unsigned int heightArg);
    Grid(const Grid& arg);
    ~Grid();
    void SetStates();

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;


    bool CheckStep(unsigned int mazeX, unsigned int mazeY) const;
    direction TakeStep(unsigned int mazeX, unsigned int mazeY);

    void Draw() const;

    Grid operator=(const Grid& arg);
};