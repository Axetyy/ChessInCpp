#pragma once
#include "IPiece.h"
#include "Pieces.h"

class King : public IPiece {
private:
    bool isSquareUnderAttack(int x, int y) const;
    bool canCastleKingside();
    bool canCastleQueenside();
public:

    bool isFirstMove = true;

    using IPiece::IPiece;
    virtual void render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) override;
    virtual void updateAvailableMoves(bool isWhite,bool incheck) override;
    virtual void renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) override;
    virtual void move(int mx, int my, int fromX, int fromY, int whereX, int whereY, bool& validMove) override;
    virtual void capture(IPiece* other) override;

};