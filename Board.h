#pragma once
#include <windows.h>
#include <windowsx.h>
#include <memory>
#include "IArrowManager.h"
#include "Arrow.h"
#ifndef BOARD
#define BOARD
class Board {
private:

	int board[9][9] = { 0 };
	int redChecked[9][9] = { 0 };

	std::unique_ptr<IArrowManager> AM_;
public:
	
	bool isWhite =true; // 1-white, 0-black
	const int boardOrgX= 450;
	const int boardOrgY= 50;
	const int cellSize = 70;
	const int gridSize = 8;

	int lastPMcurrX=0,lastPMcurrY=0;
	int lastPMprevX=0, lastPMprevY=0;

	bool firstPieceMove = false;
	bool visible = false;

	Board();
	void render(HDC hdc);
	void initBoards();
	void toggleBoard()
	{
		visible = !visible;
	}

	void clearRed() { memset(redChecked, 0, sizeof(redChecked)); }

	void setBlack() { this->isWhite = false;}
	void setWhite() { this->isWhite = true;}
	void updateLastMoved(int currX, int currY, int prevX, int prevY);
	void handleRightClick(int x,int y);

	int pixelToGridX(int x) const { return (x - boardOrgX) / cellSize; }
	int pixelToGridY(int y) const { return (y - boardOrgY) / cellSize; }

	IArrowManager* getArrowManager() { return AM_.get(); }
};

#endif
