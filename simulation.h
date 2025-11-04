
std::pair<int, int> toGrid(int x, int y);
int gridPosToIndex(const std::pair<int, int>& gridPos);
void simulate();
void inputs();

struct InputStatus{
    bool mouseJustLeftClicked = false;
};

extern InputStatus* inputStatus;

void clearGrid();