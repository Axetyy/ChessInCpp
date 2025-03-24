#include <windows.h>
#include "GameLogic.h"
#include "King.h"


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
void King::updateAvailableMoves(bool isWhite)
{

}
void King::renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize)
{

}
void King::move(int mx, int my, int fromX, int fromY, int whereX, int whereY, bool& validMove)
{

}
void King::capture(IPiece* other) {

}