#include "ArrowManager.h"
#include <windows.h>
#include <cmath>
#include <chrono>

ArrowManager::ArrowManager()
    : dragging(false), dragStartX(-1), dragStartY(-1), dragCurrentX(-1), dragCurrentY(-1), dragStartTime(0)
{
}

ArrowManager::~ArrowManager() {}

void ArrowManager::startDrag(int gridX, int gridY) {
    if (!inside(gridX, gridY, 8)) 
        return;
    dragging = true;
    dragStartX = gridX;
    dragStartY = gridY;
    dragCurrentX = gridX;
    dragCurrentY = gridY;
    dragStartTime = GetTickCount();
}

void ArrowManager::updateDrag(int gridX, int gridY) {
    if (dragging && inside(gridX, gridY, 8)) {
        dragCurrentX = gridX;
        dragCurrentY = gridY;
    }
}

bool ArrowManager::endDrag(int gridX, int gridY) {
    if (!dragging)
        return false;
    dragging = false;
    DWORD dragEndTime = GetTickCount();
    DWORD elapsed = dragEndTime - dragStartTime;

    if (inside(gridX, gridY, 8)) {
        dragCurrentX = gridX;
        dragCurrentY = gridY;
    }
    const DWORD clickThresholdMs = 500;
    bool isClick =(dragStartX == dragCurrentX) && (dragStartY == dragCurrentY) && (elapsed < clickThresholdMs);

    if (isClick) {
        return true;
    }
    else {
        Arrow newArrow;
        newArrow.startX = dragStartX;
        newArrow.startY = dragStartY;
        newArrow.endX = dragCurrentX;
        newArrow.endY = dragCurrentY;
        auto it = arrows.find(newArrow);
        if (it != arrows.end()) {
            arrows.erase(it);
        }
        else {
            arrows.insert(newArrow);
        }
        return false;
    }
}

void ArrowManager::clear() {
    arrows.clear();
}

void ArrowManager::render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize, int gridSize) {

    HPEN hArrowPen = CreatePen(PS_SOLID, 6, RGB(249, 194, 85));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hArrowPen);

    auto drawRigidKnightArrow = [&](int startGridX, int startGridY, int endGridX, int endGridY) {

        int startX = boardOrgX + startGridX * cellSize + cellSize / 2;
        int startY = boardOrgY + startGridY * cellSize + cellSize / 2;
        int endX = boardOrgX + endGridX * cellSize + cellSize / 2;
        int endY = boardOrgY + endGridY * cellSize + cellSize / 2;

        int dx = endGridX - startGridX;
        int dy = endGridY - startGridY;

        auto sign = [](int val) -> int { return (val > 0) ? 1 : -1; };

        POINT pts[3]; 
        pts[0].x = startX;
        pts[0].y = startY;
        if (std::abs(dx) == 2 && std::abs(dy) == 1) {
            pts[1].x = startX + sign(dx) * 2 * cellSize;
            pts[1].y = startY;
        }
        else if (std::abs(dy) == 2 && std::abs(dx) == 1) {
            pts[1].x = startX;
            pts[1].y = startY + sign(dy) * 2 * cellSize;
        }
        else {
            pts[1].x = startX;
            pts[1].y = startY;
        }
        pts[2].x = endX;
        pts[2].y = endY;

        Polyline(hdc, pts, 3);

        double angle = atan2((double)(pts[2].y - pts[1].y), (double)(pts[2].x - pts[1].x));
        const double arrowLength = 15.0;
        const double arrowAngle = 30 * 3.14159 / 180.0;
        int arrowX1 = (int)(pts[2].x - arrowLength * cos(angle - arrowAngle));
        int arrowY1 = (int)(pts[2].y - arrowLength * sin(angle - arrowAngle));
        int arrowX2 = (int)(pts[2].x - arrowLength * cos(angle + arrowAngle));
        int arrowY2 = (int)(pts[2].y - arrowLength * sin(angle + arrowAngle));

        MoveToEx(hdc, pts[2].x, pts[2].y, NULL);
        LineTo(hdc, arrowX1, arrowY1);
        MoveToEx(hdc, pts[2].x, pts[2].y, NULL);
        LineTo(hdc, arrowX2, arrowY2);
        };

    auto drawArrow = [&](int startGridX, int startGridY, int endGridX, int endGridY) {
        if (((std::abs(endGridX - startGridX) == 2) && (std::abs(endGridY - startGridY) == 1)) ||
            ((std::abs(endGridX - startGridX) == 1) && (std::abs(endGridY - startGridY) == 2))) {
            drawRigidKnightArrow(startGridX, startGridY, endGridX, endGridY);
        }
        else {
            int startX = boardOrgX + startGridX * cellSize + cellSize / 2;
            int startY = boardOrgY + startGridY * cellSize + cellSize / 2;
            int endX = boardOrgX + endGridX * cellSize + cellSize / 2;
            int endY = boardOrgY + endGridY * cellSize + cellSize / 2;
            MoveToEx(hdc, startX, startY, NULL);
            LineTo(hdc, endX, endY);

            double angle = atan2((double)endY - startY, (double)endX - startX);
            const double arrowLength = 15.0;
            const double arrowAngle = 30 * 3.14159 / 180.0;
            int arrowX1 = (int)(endX - arrowLength * cos(angle - arrowAngle));
            int arrowY1 = (int)(endY - arrowLength * sin(angle - arrowAngle));
            int arrowX2 = (int)(endX - arrowLength * cos(angle + arrowAngle));
            int arrowY2 = (int)(endY - arrowLength * sin(angle + arrowAngle));

            MoveToEx(hdc, endX, endY, NULL);
            LineTo(hdc, arrowX1, arrowY1);
            MoveToEx(hdc, endX, endY, NULL);
            LineTo(hdc, arrowX2, arrowY2);
        }
        };

    for (const auto& arrow : arrows) {
        drawArrow(arrow.startX, arrow.startY, arrow.endX, arrow.endY);
    }

    if (dragging) {
        drawArrow(dragStartX, dragStartY, dragCurrentX, dragCurrentY);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hArrowPen);
}