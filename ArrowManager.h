#pragma once
#include "IArrowManager.h"
#include "Arrow.h"
#include <set>
#include <tuple>
#include <windows.h>

class ArrowManager : public IArrowManager {
private:
    std::set<Arrow> arrows;
    bool dragging;
    int dragStartX, dragStartY;
    int dragCurrentX, dragCurrentY;
    DWORD dragStartTime; 

    bool inside(int x, int y, int n) {
        return (x >= 0 && x < n && y >= 0 && y < n);
    }
public:
    ArrowManager();
    virtual ~ArrowManager();

    virtual void startDrag(int gridX, int gridY) override;
    virtual void updateDrag(int gridX, int gridY) override;
    virtual bool endDrag(int gridX, int gridY) override;
    virtual void clear() override;
    virtual void render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize, int gridSize) override;
};
