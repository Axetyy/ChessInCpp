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
public:
    std::vector<IPiece*> pieces[2]; // 0 for black, 1 for white
    LastPawnMove move; 

    void renderAllPieces(HDC hdc, int boardOrgX, int boardOrgY, int cellSize);
    void addPieces(bool isWhite);
    void addPiece(IPiece* piece);
    IPiece* getPieceAt(int x, int y, int boardOrgX, int boardOrgY, int cellSize);
    IPiece* getPieceAtGrid(int gridX, int gridY);
    std::vector<IPiece*> getAllPieces();
    void removePiece(IPiece* piece);
};
