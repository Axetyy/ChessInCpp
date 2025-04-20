#include <windows.h>
#include <algorithm>
#include "GameLogic.h"
#include "King.h"
#include "Rook.h"

extern Game gameInstance;
extern Board* globalBoard;
extern IPiece* gSelectedPiece;

void King::render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize)
{
    int drawX = (boardOrgX + gridX * cellSize);
    int drawY = (boardOrgY + gridY * cellSize);

    int margin = cellSize / 10;
    int x0 = drawX + margin;
    int y0 = drawY + margin;
    int w = cellSize - 2 * margin;
    int h = cellSize - 2 * margin;

    COLORREF fillColor = (faction == 1) ? RGB(255, 255, 255) : RGB(0, 0, 0);
    COLORREF borderColor = RGB(0, 0, 0);

    HBRUSH hBrush = CreateSolidBrush(fillColor);
    HPEN   hPen = CreatePen(PS_SOLID, 1.75, borderColor);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    HGDIOBJ oldPen = SelectObject(hdc, hPen);

    int baseStartY = y0 + (int)(0.70f * h);
    int baseLeft = x0 + (int)(0.15f * w);
    int baseRight = x0 + (int)(0.85f * w);

    Ellipse(hdc,
        baseLeft,
        y0 + (int)(0.95f * h),
        baseRight,
        y0 + (int)(1.00f * h));

    Ellipse(hdc,
        baseLeft,
        y0 + (int)(0.85f * h),
        baseRight,
        y0 + (int)(0.95f * h));

    Ellipse(hdc,
        baseLeft,
        y0 + (int)(0.75f * h),
        baseRight,
        y0 + (int)(0.85f * h));

    int bodyTop = y0 + (int)(0.40f * h);
    int bodyBottom = y0 + (int)(0.80f * h);
    int bodyLeft = x0 + (int)(0.20f * w);
    int bodyRight = x0 + (int)(0.80f * w);
    Ellipse(hdc, bodyLeft, bodyTop, bodyRight, bodyBottom);

    int headCenterX = x0 + (w / 2);
    int headCenterY = y0 + (int)(0.25f * h);
    int headRadius = (int)(0.18f * h); 

    Ellipse(hdc,
        headCenterX - headRadius, headCenterY - headRadius,
        headCenterX + headRadius, headCenterY + headRadius);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);

    COLORREF crossColor = (faction == 0) ? RGB(255, 255, 255) : RGB(0, 0, 0);
    borderColor = (faction == 1) ? RGB(255, 255, 255) : RGB(0, 0, 0);
    HPEN crossPen = CreatePen(PS_SOLID, 2, borderColor);
    HPEN oldPen2 = (HPEN)SelectObject(hdc, crossPen);

    int crossSize = cellSize / 20 +1;

    int crossCenterY = headCenterY - headRadius - (crossSize / 2);
    MoveToEx(hdc, headCenterX - crossSize, crossCenterY, NULL);
    LineTo(hdc, headCenterX + crossSize, crossCenterY);

    MoveToEx(hdc, headCenterX, crossCenterY - crossSize, NULL);
    LineTo(hdc, headCenterX, crossCenterY + crossSize);

    SelectObject(hdc, oldPen2);
    DeleteObject(crossPen);
}
void King::updateAvailableMoves(bool isWhite,bool incheck) {
    availableMoves.clear();
 

    IPiece* otherKing = nullptr;
    for (auto piece : gameInstance.getPieces()->getAllPieces()) {
        if (piece->faction != this->faction) {
            if (typeid(*piece) == typeid(King)) {
                otherKing = piece;
            }
        }
    }

    int directions[8][2] = { {1, 0}, {1, 1}, {0, 1}, {-1, 1},
                            {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
    for (auto& dir : directions) {
        int newX = this->gridX + dir[0];
        int newY = this->gridY + dir[1];
        if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8)
            continue;

        bool adjacentToEnemyKing = false;
        if (otherKing) {
            for (auto& dir1 : directions)
            {
                int otherKingX = otherKing->gridX + dir1[0];
                int otherKingY = otherKing->gridY + dir1[1];
                if (otherKingX < 0 || otherKingX >= 8 || otherKingY < 0 || otherKingY >= 8)
                    continue;

                if (newX == otherKingX && newY == otherKingY)
                    adjacentToEnemyKing = true;
            }
        }
        if (adjacentToEnemyKing)
            continue;

        if (isSquareUnderAttack(newX, newY))
            continue;

        IPiece* p = gameInstance.getPieces()->getPieceAtGrid(newX, newY);
        if (p == nullptr || p->faction != this->faction) {
            availableMoves.push_back({ newX, newY });
        }
    }

    if (this->isFirstMove == true && gameInstance.checkState.isInCheck==false)
    {
        bool canKingside = canCastleKingside();
        if (canKingside) {
            availableMoves.push_back({ gridX + 2, gridY });
        }

        bool canQueenside = canCastleQueenside();
        if (canQueenside) {
            availableMoves.push_back({ gridX - 2, gridY });
        }
    }

}
bool King::isSquareUnderAttack(int x, int y) const {
    return faction == 1
        ? globalBoard->isSquareAttackedByBlack(x, y)
        : globalBoard->isSquareAttackedByWhite(x, y);
}
bool King::canCastleKingside() {
    int rookX = 7;
    int rookY = this->gridY;

    IPiece* rook = gameInstance.getPieces()->getPieceAtGrid(rookX, rookY);
    if (!rook || typeid(*rook) != typeid(Rook) || !rook->isFirstMove)
        return false;

    for (int x = gridX + 1; x < rookX; x++) {
        if (gameInstance.getPieces()->getPieceAtGrid(x, rookY) != nullptr)
            return false;
    }

    if (isSquareUnderAttack(gridX, gridY))
        return false;

    for (int x = this->gridX + 1; x <= gridX + 2; x++) {
        if (isSquareUnderAttack(x, gridY))
            return false;
    }

    return true;
}
bool King::canCastleQueenside() {
    int rookX = 0;
    int rookY = gridY;

    IPiece* rook = gameInstance.getPieces()->getPieceAtGrid(rookX, rookY);
    if (!rook || typeid(*rook) != typeid(Rook) || !rook->isFirstMove)
        return false;

    for (int x = gridX - 1; x > rookX; x--) {
        if (gameInstance.getPieces()->getPieceAtGrid(x, rookY) != nullptr)
            return false;
    }

    if (isSquareUnderAttack(gridX, gridY))
        return false;

    for (int x = gridX - 1; x >= gridX - 2; x--) {
        if (isSquareUnderAttack(x, gridY))
            return false;
    }

    return true;
}
void King::renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize)
{
    for (const POINT& move : availableMoves) {
        int drawX = boardOrgX + move.x * cellSize;
        int drawY = boardOrgY + move.y * cellSize;

        IPiece* p = gameInstance.getPieces()->getPieceAt(drawX + cellSize / 4, drawY + cellSize / 4,
            boardOrgX, boardOrgY, cellSize);
        COLORREF color = (p != nullptr && p->faction != this->faction)
            ? RGB(240, 10, 10)
            : RGB(245, 245, 245);

        HBRUSH brush = CreateSolidBrush(color);
        SelectObject(hdc, brush);

        int margin = cellSize / 3.5;
        Ellipse(hdc, drawX + margin, drawY + margin, drawX + cellSize - margin, drawY + cellSize - margin);

        DeleteObject(brush);
    }
}
void King::move(int mx, int my, int fromX, int fromY, int whereX, int whereY, bool& validMove)
{
    for (const POINT& move : gSelectedPiece->availableMoves) {
        if (move.x == whereX && move.y == whereY) {
            if (abs(whereX - fromX) == 2) {
                bool kingside = (whereX > fromX);
                int rookOriginalX = kingside ? 7 : 0;
                int rookNewX = kingside ? fromX + 1 : fromX - 1;

                IPiece* rook = gameInstance.getPieces()->getPieceAtGrid(rookOriginalX, fromY);
                if (rook && typeid(*rook) == typeid(Rook)) {
                    rook->gridX = rookNewX;
                    rook->gridY = fromY;
                    rook->isFirstMove = false;
                }
            }

            IPiece* target = gameInstance.getPieces()->getPieceAt(
                mx, my,
                globalBoard->boardOrgX, globalBoard->boardOrgY, globalBoard->cellSize);
            if (target != nullptr) {
                gSelectedPiece->capture(target);
            }

            gSelectedPiece->gridX = whereX;
            gSelectedPiece->gridY = whereY;


            gSelectedPiece->updateAvailableMoves(globalBoard->isWhite,false);
            validMove = true;
            break;
        }
    }
}
void King::capture(IPiece* other) {
    gameInstance.getPieces()->removePiece(other);
    auto& checkingPieces = gameInstance.checkingPieces;
    checkingPieces.erase(
        std::remove(checkingPieces.begin(), checkingPieces.end(), other),
        checkingPieces.end()
    );

    delete other;
}