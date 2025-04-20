#pragma once
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <vector>
#include <utility>
#include "Board.h"
#include "Pieces.h"
#include "IPiece.h"
#include "King.h"
#include "Knight.h"


class Game {
private:
	bool isDoubleCheck = false;
public:
	std::unique_ptr<Board> gBoard_;
	std::unique_ptr < Pieces > pieces_;
	std::vector<IPiece*>checkingPieces;
	std::vector<std::pair<POINT,int>> globalAvailableMoves;
	
	int currentTurn;
	struct check {
		bool isInCheck;
		int factionChecking;
		IPiece* checkedKing;
	}checkState;


	Game();

	Board* getBoard() { return gBoard_.get(); }
	Pieces* getPieces() { return pieces_.get(); }

	void toggleTurn() { currentTurn = (currentTurn == 0) ? 1 : 0; }
	void render(HDC hdc);
	void addCheckingPieces();
	bool isAttackingEnemyKing(IPiece* attacker);
	int checkLoss();
	void updatePieces();
};



