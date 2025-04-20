#pragma once
#include "IPiece.h"
#include "Pieces.h"

class Pawn : public IPiece {
public:
    using IPiece::IPiece;
    virtual void render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) override;
    virtual void updateAvailableMoves(bool isWhite,bool incheck) override;
    virtual void renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) override;
    virtual void move(int mx, int my, int fromX, int fromY, int whereX, int whereY, bool& validMove) override;
    virtual void capture(IPiece* other) override;
    virtual void updateCheckMoves() override;
};