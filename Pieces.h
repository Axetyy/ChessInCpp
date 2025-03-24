#pragma once
#include <windows.h>
#include <vector>
#include "IPiece.h"
struct LastPawnMove {
    int origX, origY;
    int curX, curY;
    IPiece* pieceThatMoved;

};
class Pieces {
    std::vector<IPiece*> pieces[2]; // 0 for black, 1 for white
public:
    LastPawnMove move; 
    void renderAllPieces(HDC hdc, int boardOrgX, int boardOrgY, int cellSize);
    void addPieces(bool isWhite);
    IPiece* getPieceAt(int x, int y, int boardOrgX, int boardOrgY, int cellSize);
    std::vector<IPiece*> getAllPieces();
    void removePiece(IPiece* piece);
};
