#include <windows.h>
#include <windowsx.h> 
#include "GameLogic.h"
#include "Pieces.h"
#include <string>

Game::Game() {
	gBoard_ = std::make_unique<Board>();
	pieces_ = std::make_unique<Pieces>();
	currentTurn = gBoard_->isWhite ? 1 : 0;
}

void Game::updatePieces() {
	pieces_->addPieces(gBoard_->isWhite);
}
void Game::render(HDC hdc) {
	int box = gBoard_->boardOrgX;
	int boy = gBoard_->boardOrgY;
	int cell = gBoard_->cellSize;
	if (gBoard_->visible) {
		gBoard_->render(hdc);
		pieces_->renderAllPieces(hdc, box, boy, cell);
		gBoard_->getArrowManager()->render(hdc, gBoard_->boardOrgX, gBoard_->boardOrgY, gBoard_->cellSize, gBoard_->gridSize);
	}
	
	/*if (gBoard_->firstPieceMove)
	{
		std::wstring debugText = L"Last Move: (" +
			std::to_wstring(gBoard_->lastPMcurrX) + L", " +
			std::to_wstring(gBoard_->lastPMcurrY) + L") -> (" +
			std::to_wstring(gBoard_->lastPMprevX) + L", " +
			std::to_wstring(gBoard_->lastPMprevX) + L")";

		SetTextColor(hdc, RGB(255, 0, 0));
		SetBkMode(hdc, TRANSPARENT);

		TextOut(hdc, 10, 10, debugText.c_str(), debugText.length());
	}*/
}