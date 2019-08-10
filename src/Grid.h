#pragma once

class Grid
{
    enum tileState {WALL = 0, BORDER, CORRIDOR};
    enum direction {UP = 0, RIGHT, DOWN, LEFT};
    unsigned int width, height, gridW, gridH;
    tileState* array; 
public:
    Grid() = delete;
    Grid(unsigned int widthArg, unsigned int heightArg);
    Grid(const Grid& arg);
    ~Grid();
    void SetStates();

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;
    tileState& GetElem(unsigned int x, unsigned int y);

    void Dfs(unsigned int xPos, unsigned int yPos);
    unsigned int CheckSteps(unsigned int xPos, unsigned int yPos) const;
    void Generate();

    void Draw(std::string fileName = "default") const;

    Grid operator=(const Grid& arg);
};