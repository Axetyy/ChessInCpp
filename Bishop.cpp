#include <windows.h>
#include <windowsx.h>
#include "GameLogic.h"
#include "IPiece.h"
#include "Bishop.h"

extern Game gameInstance;
extern Board* globalBoard;
extern IPiece* gSelectedPiece;

void Bishop::render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize)
{
    int drawX = (boardOrgX + gridX * cellSize);
    int drawY =(boardOrgY + gridY * cellSize);

    int margin = cellSize / 10;
    int x0 = drawX + margin;
    int y0 = drawY + margin;
    int w = cellSize - 2 * margin;
    int h = cellSize - 2 * margin;

    COLORREF fillColor = (faction == 1) ? RGB(255, 255, 255) : RGB(0, 0, 0);
    COLORREF borderColor =RGB(0, 0, 0);

    HBRUSH hBrush = CreateSolidBrush(fillColor);
    HPEN   hPen = CreatePen(PS_SOLID, 1.75, borderColor);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    HGDIOBJ oldPen = SelectObject(hdc, hPen);

    int baseTop = y0 + (int)(0.80f * h);
    int baseBottom = y0 + h;
    int baseLeft = x0 + (int)(0.15f * w);
    int baseRight = x0 + (int)(0.85f * w);
    Ellipse(hdc, baseLeft, baseTop, baseRight, baseBottom);

    int bodyTop = y0 + (int)(0.40f * h);
    int bodyBottom = y0 + (int)(0.80f * h);
    int bodyLeft = x0 + (int)(0.25f * w);
    int bodyRight = x0 + (int)(0.75f * w);
    Ellipse(hdc, bodyLeft, bodyTop, bodyRight, bodyBottom);

    int headCenterX = x0 + (w / 2);
    int headCenterY = y0 + (int)(0.20f * h);
    int headRadius = (int)(0.225f * h);
    Ellipse(hdc,
        headCenterX - headRadius, headCenterY - headRadius,
        headCenterX + headRadius, headCenterY + headRadius);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);

    COLORREF crossColor = (faction == 0) ? RGB(255, 255, 255) : RGB(0, 0, 0);
    HPEN crossPen = CreatePen(PS_SOLID, 2, crossColor);
    HPEN oldPen2 = (HPEN)SelectObject(hdc, crossPen);

    int crossSize = cellSize / 20;
    MoveToEx(hdc, headCenterX - crossSize, headCenterY, NULL);
    LineTo(hdc, headCenterX + crossSize, headCenterY);
    MoveToEx(hdc, headCenterX, headCenterY - crossSize, NULL);
    LineTo(hdc, headCenterX, headCenterY + crossSize);

    SelectObject(hdc, oldPen2);
    DeleteObject(crossPen);
}
void Bishop::capture(IPiece* other)
{
    gameInstance.getPieces()->removePiece(other);
    delete other;
}
void Bishop::renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) {
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
void Bishop::move(int mx, int my, int fromX, int fromY, int whereX, int whereY, bool& validMove) {
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
void Bishop::updateAvailableMoves(bool isWhite) {

    this->availableMoves.clear();

    int directions[4][2] = {
        {1, 1},   {1, -1},
        {-1, 1},  {-1, -1}
    };

    for (int k = 0; k < 4; k++) {
        int dx = directions[k][0];
        int dy = directions[k][1];

        bool obstructed = false;
        for (int step = 1; step <=8 && !obstructed; step++) {
            int newX = gridX + dx * step;
            int newY = gridY + dy * step;

            if (newX < 0 || newX >= 8 || newY < 0 || newY >= 8) {
                obstructed = true;
                break;
            }
            int pixelX = globalBoard->boardOrgX + newX * globalBoard->cellSize;
            int pixelY = globalBoard->boardOrgY + newY * globalBoard->cellSize;
            IPiece* p = gameInstance.getPieces()->getPieceAt(
                pixelX + globalBoard->cellSize/4, pixelY + globalBoard->cellSize/4,
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
