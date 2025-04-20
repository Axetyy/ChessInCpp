#include <windows.h>
#include <windowsx.h>
#include "Pieces.h"
#include "GameLogic.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

void Pieces::renderAllPieces(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) {
    for (auto& piece : pieces[0]) {
        piece->render(hdc, boardOrgX, boardOrgY, cellSize);
    }
    for (auto& piece : pieces[1]) {
        piece->render(hdc, boardOrgX, boardOrgY, cellSize);
    }
}

void Pieces::addPieces(bool isWhite) {
    for (int i = 0; i < 2; i++) {
        for (auto p : pieces[i]) {
            delete p;
        }
        pieces[i].clear();
    }
    int whiteFrontRow = isWhite ? 6 : 1;
    int blackFrontRow = isWhite ? 1 : 6;

    int whiteBackRow = isWhite ? 7 : 0;
    int blackBackRow = isWhite ? 0 : 7;

    for (int i = 0; i < 8; i++) {
        Pawn* whitePawn = new Pawn(i, whiteFrontRow, 1);
        whitePawn->updateAvailableMoves(isWhite,false);
        pieces[1].push_back(whitePawn);

        Pawn* blackPawn = new Pawn(i, blackFrontRow, 0);
        blackPawn->updateAvailableMoves(isWhite,false);
        pieces[0].push_back(blackPawn);
    }

    /// Knights

    Knight* knight1 = new Knight(1, whiteBackRow, 1);
    Knight* knight2 = new Knight(6, whiteBackRow, 1);

    knight1->updateAvailableMoves(isWhite,false);
    knight2->updateAvailableMoves(isWhite,false);

    pieces[1].push_back(knight1);
    pieces[1].push_back(knight2);

    Knight* knight3 = new Knight(1, blackBackRow, 0);
    Knight* knight4 = new Knight(6, blackBackRow, 0);

    knight3->updateAvailableMoves(isWhite,false);
    knight4->updateAvailableMoves(isWhite,false);

    pieces[0].push_back(knight3);
    pieces[0].push_back(knight4);
    
    ///Bishops;

    Bishop* bishop1 = new Bishop(2, whiteBackRow, 1);
    Bishop* bishop2 = new Bishop(5, whiteBackRow, 1);

    bishop1->updateAvailableMoves(isWhite,false);
    bishop2->updateAvailableMoves(isWhite,false);

    pieces[1].push_back(bishop1);
    pieces[1].push_back(bishop2);

    Bishop* bishop3 = new Bishop(2, blackBackRow, 0);
    Bishop* bishop4 = new Bishop(5, blackBackRow, 0);

    bishop3->updateAvailableMoves(isWhite,false);
    bishop4->updateAvailableMoves(isWhite,false);

    pieces[0].push_back(bishop3);
    pieces[0].push_back(bishop4);


    ///Rooks

    Rook* rook1 = new Rook(0, whiteBackRow, 1);
    Rook* rook2 = new Rook(7, whiteBackRow, 1);

    rook1->updateAvailableMoves(isWhite,false);
    rook2->updateAvailableMoves(isWhite,false);

    pieces[1].push_back(rook1);
    pieces[1].push_back(rook2);

    Rook* rook3 = new Rook(0, blackBackRow, 0);
    Rook* rook4 = new Rook(7, blackBackRow, 0);

    rook3->updateAvailableMoves(isWhite,false);
    rook4->updateAvailableMoves(isWhite,false);

    pieces[0].push_back(rook3);
    pieces[0].push_back(rook4);
    rook1->isFirstMove = true;
    rook2->isFirstMove = true;
    rook3->isFirstMove = true;
    rook4->isFirstMove = true;


    //Queen
    if (isWhite) {
        Queen* whiteQueen = new Queen(3,whiteBackRow,1);
        Queen* blackQueen = new Queen(3, blackBackRow, 0);

        pieces[1].push_back(whiteQueen);
        pieces[0].push_back(blackQueen);

        whiteQueen->updateAvailableMoves(isWhite,false);
        blackQueen->updateAvailableMoves(isWhite,false);

        King* whiteKing = new King(4, whiteBackRow, 1);
        King* blackKing = new King(4, blackBackRow, 0);

        pieces[1].push_back(whiteKing);
        pieces[0].push_back(blackKing);

        whiteKing->updateAvailableMoves(isWhite,false);
        blackKing->updateAvailableMoves(isWhite,false);
        whiteKing->isFirstMove = true;
        blackKing->isFirstMove = true;
    }
    else
    {
        Queen* whiteQueen = new Queen(4, whiteBackRow, 1);
        Queen* blackQueen = new Queen(4, blackBackRow, 0);

        pieces[1].push_back(whiteQueen);
        pieces[0].push_back(blackQueen);

        whiteQueen->updateAvailableMoves(isWhite,false);
        blackQueen->updateAvailableMoves(isWhite,false);

        King* whiteKing = new King(3, whiteBackRow, 1);
        King* blackKing = new King(3, blackBackRow, 0);

        pieces[1].push_back(whiteKing);
        pieces[0].push_back(blackKing);

        whiteKing->updateAvailableMoves(isWhite,false);
        blackKing->updateAvailableMoves(isWhite,false);

        whiteKing->isFirstMove = true;
        blackKing->isFirstMove = true;
    }


}


IPiece* Pieces::getPieceAt(int x, int y, int boardOrgX, int boardOrgY, int cellSize) {
    for (auto p : pieces[0]) {
        int pieceX = boardOrgX + p->gridX * cellSize;
        int pieceY = boardOrgY + p->gridY * cellSize;
        if (x >= pieceX && x <= pieceX + cellSize && y >= pieceY && y <= pieceY + cellSize)
            return p;
    }
    for (auto p : pieces[1]) {
        int pieceX = boardOrgX + p->gridX * cellSize;
        int pieceY = boardOrgY + p->gridY * cellSize;
        if (x >= pieceX && x <= pieceX + cellSize && y >= pieceY && y <= pieceY + cellSize)
            return p;
    }
    return nullptr;
}

std::vector<IPiece*> Pieces::getAllPieces() {
    std::vector<IPiece*> all;
    all.insert(all.end(), pieces[0].begin(), pieces[0].end());
    all.insert(all.end(), pieces[1].begin(), pieces[1].end());
    return all;
}
void Pieces::removePiece(IPiece* piece) {
    for (int i = 0; i < 2; i++) {
        for (auto it = pieces[i].begin(); it != pieces[i].end(); ++it) {
            if (*it == piece) {
                pieces[i].erase(it);
                return;
            }
        }
    }
}
void Pieces::addPiece(IPiece* piece)
{
    if (piece->faction) {
        pieces[1].push_back(piece);
    }
    else {
        pieces[0].push_back(piece);
    }
}

IPiece* Pieces::getPieceAtGrid(int gridX, int gridY) {
    for (auto factionPieces : pieces) {
        for (auto piece : factionPieces) {
            if (piece->gridX == gridX && piece->gridY == gridY) {
                return piece;
            }
        }
    }
    return nullptr;
}