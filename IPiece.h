#pragma once
#include <windows.h>
#include <vector>
class IPiece {
public:
    int gridX, gridY;  // ex. (0,0)
    bool faction; // 1-white, 0-black
    std::vector<POINT> availableMoves;

    IPiece(int x, int y, bool f)
        : gridX(x), gridY(y), faction(f)

    {
    }
    virtual ~IPiece() = default;
    virtual void render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) = 0;
    virtual void updateAvailableMoves(bool isWhite) {};
    virtual void renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) {};
    virtual void move(int mx,int my,int fromX, int fromY, int whereX, int whereY,bool &validMove) {};
    virtual void capture(IPiece* other){};
};