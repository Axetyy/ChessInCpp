#pragma once
#include <windows.h>
#include <vector>
class IPiece {
public:
    int gridX, gridY;  // ex. (0,0)
    bool faction; // 1-white, 0-black
    bool isFirstMove;
    std::vector<POINT> availableMoves;

    IPiece(int x, int y, bool f)
        : gridX(x), gridY(y), faction(f),isFirstMove(true)

    {
    }
    virtual ~IPiece() = default;
    virtual void render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) = 0;
    virtual void updateAvailableMoves(bool isWhite,bool incheck) {};
    virtual void renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) {};
    virtual void move(int mx,int my,int fromX, int fromY, int whereX, int whereY,bool &validMove) {};
    virtual void updateCheckMoves(){};
    virtual void capture(IPiece* other){};
};