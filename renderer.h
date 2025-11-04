

void drawRect(int x, int y, int w, int h, int r, int g, int b);
void drawLine(int x1, int y1, int x2, int y2, int thickness, int r, int g, int b);
void drawGrid();
std::pair<int, int> toGrid(int x, int y);
void drawCells();
int gridPosToIndex(const std::pair<int, int>& gridPos);