#pragma once
#include <windows.h>

class IArrowManager
{
public:
		virtual ~IArrowManager() {}
		virtual void startDrag(int gridX, int gridY) = 0;
		virtual bool endDrag(int gridX, int gridY) = 0;
		virtual void updateDrag(int gridX, int gridY) = 0;
		virtual void clear() = 0;
		virtual void render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize, int gridSize) = 0;
};
