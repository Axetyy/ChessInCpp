#pragma once
#include <windows.h>
#include <windowsx.h>

#include "Board.h"
#include "Pieces.h"
#include "IPiece.h"

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

class Game {
private:
public:
	std::unique_ptr<Board> gBoard_;
	std::unique_ptr < Pieces > pieces_;
	
		int currentTurn;

	Game();

	Board* getBoard() { return gBoard_.get(); }
	Pieces* getPieces() { return pieces_.get(); }

	void toggleTurn() { currentTurn = (currentTurn == 0) ? 1 : 0; }

	void render(HDC hdc);
	void updatePieces();
};

#endif




