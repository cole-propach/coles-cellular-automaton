
std::pair<int, int> toGrid(int x, int y);
int gridPosToIndex(const std::pair<int, int>& gridPos);
void simulate();
void inputs();
std::pair<int, int> indexToGridPos(int index);
void doNextGen();

struct InputStatus{
    bool mouseJustLeftClicked = false;
};

extern InputStatus* inputStatus;

void clearGrid();