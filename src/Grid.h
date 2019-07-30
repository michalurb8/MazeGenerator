#pragma once

class Grid
{
    enum tileState {CORRIDOR = 0, WALL, PILLAR, PATH};
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

    unsigned int CheckSteps(unsigned int xPos, unsigned int yPos) const;
    bool CheckStuck(unsigned int xPos, unsigned int yPos) const;
    direction TakeStep(unsigned int xPos, unsigned int yPos);
    bool CreatePath();
    void ClearPath();

    void Draw(std::string fileName = "default") const;

    Grid operator=(const Grid& arg);
};