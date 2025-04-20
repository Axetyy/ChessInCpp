

#include "Board.h"
#include "ArrowManager.h"
#include <memory>
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <tchar.h>
#include <string>
#include "Pieces.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Bishop.h"
#include "Rook.h"
#include "King.h"

bool inside(int x, int y,int n)
{
	return (x >= 0 && x < n && y >= 0 && y < n);
}

Board::Board() {

	AM_ = std::make_unique<ArrowManager>();

}
void Board::initBoards()
{
	memset(board, 0, sizeof(board));
	memset(redChecked, 0, sizeof(board));
}
void Board::render(HDC hdc)
{
	// Colors
	for (int i = 0; i < gridSize; ++i) {
		for (int j = 0; j < gridSize; ++j) {
			int x = boardOrgX + i * cellSize;
			int y = boardOrgY + j * cellSize;
			COLORREF color;
			if (!redChecked[i][j]) color = ((i + j) % 2 == 0) ? RGB(238, 239, 210) : RGB(114, 145, 83);
			else color = ((i + j) % 2 == 0) ? RGB(235, 125, 106) : RGB(211, 108, 81);
			HBRUSH brush = CreateSolidBrush(color);
			RECT rect = { x, y, x + cellSize, y + cellSize };
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
	}

	// Highlights
	if (firstPieceMove)
	{
		if (lastPMcurrX >= 0 && lastPMcurrX < gridSize && lastPMcurrY >= 0 && lastPMcurrY < gridSize)
		{
			int x = boardOrgX + lastPMcurrX * cellSize;
			int y = boardOrgY + lastPMcurrY * cellSize;
			COLORREF highlightColor = ((lastPMcurrX + lastPMcurrY) % 2 == 0) ? RGB(245, 247, 114) : RGB(185, 203, 58);
			HBRUSH brush = CreateSolidBrush(highlightColor);
			RECT rect = { x, y, x + cellSize, y + cellSize };
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}

		if (lastPMprevX >= 0 && lastPMprevX < gridSize && lastPMprevY >= 0 && lastPMprevY < gridSize)
		{
			int x = boardOrgX + lastPMprevX * cellSize;
			int y = boardOrgY + lastPMprevY * cellSize;
			COLORREF highlightColor = ((lastPMprevX + lastPMprevY) % 2 == 0) ? RGB(245, 247, 114) : RGB(185, 203, 58);;
			HBRUSH brush = CreateSolidBrush(highlightColor);
			RECT rect = { x, y, x + cellSize, y + cellSize };
			FillRect(hdc, &rect, brush);
			DeleteObject(brush);
		}
	}

	// Lines
	for (int i = 0; i <= gridSize; ++i) {
		MoveToEx(hdc, boardOrgX + i * cellSize, boardOrgY, NULL);
		LineTo(hdc, boardOrgX + i * cellSize, boardOrgY + gridSize * cellSize);
		MoveToEx(hdc, boardOrgX, boardOrgY + i * cellSize, NULL);
		LineTo(hdc, boardOrgX + gridSize * cellSize, boardOrgY + i * cellSize);
	}
	///Labels
	SetBkMode(hdc, TRANSPARENT);
	SetTextAlign(hdc, TA_LEFT);		
	HFONT hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, VARIABLE_PITCH, L"Arial");
	HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
	for (int j = 0;j < gridSize;j++){
		wchar_t letter = (isWhite) ? L'a' + j : L'h' -j;
		wchar_t num =(isWhite) ? L'8' - j : L'1' + j;
		std::wstring alLabel(1, letter);
		std::wstring numLabel(1, num);
		COLORREF colorAl= (j % 2 == 0) ? RGB(238, 239, 210) : RGB(114, 145, 83);
		COLORREF colorNum = (j % 2 == 1) ? RGB(238, 239, 210) : RGB(114, 145, 83);
		SetTextColor(hdc, colorAl);
		TextOut(hdc, boardOrgX + j * cellSize + cellSize-12, boardOrgY + gridSize * cellSize - 15,alLabel.c_str(), 1);
		SetTextColor(hdc, colorNum);
		TextOut(hdc, boardOrgX + 6, boardOrgY + j * cellSize + 6, numLabel.c_str(), 1);
	}
	

	SelectObject(hdc, hOldFont);
	DeleteObject(hFont);
}
void Board::handleRightClick(int x, int y)
{
	int gridX = (x - boardOrgX) / cellSize;
	int gridY = (y - boardOrgY) / cellSize;

	if(inside(gridX,gridY,gridSize)) {
		redChecked[gridX][gridY] ^= 1;
	}
}
void Board::updateLastMoved(int currX, int currY, int prevX, int prevY)
{
	this->lastPMcurrX = currX;
	this->lastPMcurrY = currY;

	this->lastPMprevX = prevX;
	this->lastPMprevY = prevY;

}
void Board::updateAttackedSquares(Pieces* pieces) {
	memset(attackedByWhite, 0, sizeof(attackedByWhite));
	memset(attackedByBlack, 0, sizeof(attackedByBlack));

	auto getMatrix = [this](bool isWhite) -> int (*)[9] {
		return isWhite ? attackedByWhite : attackedByBlack;
	};
	for (IPiece* piece : pieces->getAllPieces()) {
		int x = piece->gridX;
		int y = piece->gridY;
		bool isWhite = (piece->faction == 1);
		int (*attackedMatrix)[9] = getMatrix(isWhite);
		if (typeid(*piece) == typeid(Pawn)) {
			int direction = isWhite ? -1 : 1;
			if (x > 0 && y + direction >= 0 && y + direction < 8)
				attackedMatrix[x - 1][y + direction] = 1;
			if (x < 7 && y + direction >= 0 && y + direction < 8)
				attackedMatrix[x + 1][y + direction] = 1;
		}
		else if (typeid(*piece) == typeid(Knight)) {
			const int moves[8][2] = { {2,1}, {1,2}, {-1,2}, {-2,1}, {-2,-1}, {-1,-2}, {1,-2}, {2,-1} };
			for (const auto& move : moves) {
				int nx = x + move[0], ny = y + move[1];
				if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
					attackedMatrix[nx][ny] = 1;
			}
		}
		if (typeid(*piece) == typeid(Bishop) || typeid(*piece) == typeid(Queen)) {
			const int diagDirs[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
			for (const auto& dir : diagDirs) {
				for (int step = 1;; step++) {
					int nx = x + dir[0] * step, ny = y + dir[1] * step;
					if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break;
					attackedMatrix[nx][ny] = 1;
					if (pieces->getPieceAtGrid(nx, ny)) break;
				}
			}
		}
		if (typeid(*piece) == typeid(Rook) || typeid(*piece) == typeid(Queen)) {
			const int straightDirs[4][2] = { {1,0}, {0,1}, {-1,0}, {0,-1} };
			for (const auto& dir : straightDirs) {
				for (int step = 1;; step++) {
					int nx = x + dir[0] * step, ny = y + dir[1] * step;
					if (nx < 0 || nx >= 8 || ny < 0 || ny >= 8) break;
					attackedMatrix[nx][ny] = 1;
					if (pieces->getPieceAtGrid(nx, ny)) break;
				}
			}
		}
		else if (typeid(*piece) == typeid(King)) {
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (dx == 0 && dy == 0)
						continue;
					int nx = x + dx, ny = y + dy;
					if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8)
						attackedMatrix[nx][ny] = 1;
				}
			}
		}
	}
}
