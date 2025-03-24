
#include <windows.h>
#include <windowsx.h>
#include "IPiece.h"
#include "GameLogic.h"
#include "Rook.h"
extern Game gameInstance;
extern Board* globalBoard;
extern IPiece* gSelectedPiece;

void Rook::render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize)
{
    int drawX = boardOrgX + gridX * cellSize;
    int drawY = boardOrgY + gridY * cellSize;

    int margin = cellSize / 10;
    int x0 = drawX + margin;
    int y0 = drawY + margin;
    int w = cellSize - 2 * margin;
    int h = cellSize - 2 * margin;

    COLORREF fillColor = (faction == 1) ? RGB(255, 255, 255) : RGB(0, 0, 0);
    COLORREF borderColor = RGB(0, 0, 0);

    HBRUSH hBrush = CreateSolidBrush(fillColor);
    HPEN   hPen = CreatePen(PS_SOLID, 2, borderColor);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    HGDIOBJ oldPen = SelectObject(hdc, hPen);

    int baseLeft = x0 + (int)(0.3f * w);
    int baseRight = x0 + (int)(0.7f * w);
    int baseTop = y0 + (int)(0.35f * h);
    int baseBottom = y0 + h;
    Rectangle(hdc, baseLeft, baseTop, baseRight, baseBottom);

    POINT topPts[12];

    int topHeight = (int)(0.15f * h);
    int protrusionWidth = (int)(0.1f * w);
    int midLeft = (baseLeft + baseRight) / 2 - protrusionWidth / 2;
    // Left protrusion
    topPts[0] = { baseLeft-2,baseTop };
    topPts[1] = { baseLeft-2,baseTop - topHeight };
    topPts[2] = { baseLeft + protrusionWidth, baseTop - topHeight };
    topPts[3] = { baseLeft + protrusionWidth, baseTop };

    // Middle protrusion
    topPts[4] = { midLeft, baseTop };
    topPts[5] = { midLeft,baseTop - topHeight };
    topPts[6] = { midLeft + protrusionWidth, baseTop - topHeight };
    topPts[7] = { midLeft + protrusionWidth, baseTop };

    // Right protrusion
    topPts[8] = { baseRight - protrusionWidth, baseTop };
    topPts[9] = { baseRight - protrusionWidth, baseTop - topHeight };
    topPts[10] = { baseRight+2,baseTop - topHeight};
    topPts[11] = { baseRight+2, baseTop }; 

    Polygon(hdc, topPts, 12);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}
void Rook::updateAvailableMoves(bool isWhite) {
    this->availableMoves.clear();
    int directions[4][2] = {
        {1, 0},   {0, 1},
        {-1, 0},  {0, -1}
    };

    for(int k = 0; k < 4; k++) {
        int dx = directions[k][0];
        int dy = directions[k][1];

        bool obstructed = false;
        for (int step = 1; step <= 8 && !obstructed; step++) {
            int newX = gridX + dx * step;
            int newY = gridY + dy * step;

            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8) {
                obstructed = true;
                break;
            }
            int pixelX = globalBoard->boardOrgX + newX * globalBoard->cellSize;
            int pixelY = globalBoard->boardOrgY + newY * globalBoard->cellSize;
            IPiece* p = gameInstance.getPieces()->getPieceAt(
                pixelX + globalBoard->cellSize / 4, pixelY + globalBoard->cellSize / 4,
                globalBoard->boardOrgX, globalBoard->boardOrgY,
                globalBoard->cellSize);

            if (p == nullptr) {
                availableMoves.push_back({ newX, newY });
            }
            else {
                if (p->faction != this->faction) {
                    availableMoves.push_back({ newX, newY });
                }
                obstructed = true;
            }
        }
    }
}
void Rook::renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) {
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
void Rook::move(int mx, int my, int fromX, int fromY, int whereX, int whereY, bool& validMove)
{
    for (const POINT& move : gSelectedPiece->availableMoves) {
        if (move.x == whereX && move.y == whereY) {

            IPiece* target = gameInstance.getPieces()->getPieceAt(
                mx, my,
                globalBoard->boardOrgX, globalBoard->boardOrgY, globalBoard->cellSize);
            if (target != nullptr) {
                gSelectedPiece->capture(target);
            }
            gSelectedPiece->gridX = whereX;
            gSelectedPiece->gridY = whereY;
            gSelectedPiece->updateAvailableMoves(globalBoard->isWhite);
            validMove = true;
            break;
        }
    }
}
void Rook::capture(IPiece* other) {
    gameInstance.getPieces()->removePiece(other);
    delete other;
}