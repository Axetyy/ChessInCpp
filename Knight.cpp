#include <windows.h>
#include <windowsx.h>
#include "GameLogic.h"
#include "IPiece.h"
#include "Knight.h"
#include "King.h"
#include <iterator>
extern Game gameInstance;
extern Board* globalBoard;
extern IPiece* gSelectedPiece;

void Knight::render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize)
{
    int drawX =boardOrgX + gridX * cellSize;
    int drawY =boardOrgY + gridY * cellSize;

    int margin = cellSize / 10;
    int x0 = drawX + margin;
    int y0 = drawY + margin;
    int w = cellSize - 2 * margin;
    int h = cellSize - 2 * margin;

    COLORREF fillColor = (faction == 1) ? RGB(255, 255, 255) : RGB(0, 0, 0);
    COLORREF borderColor = RGB(0, 0, 0);

    HBRUSH hBrush = CreateSolidBrush(fillColor);
    HPEN   hPen = CreatePen(PS_SOLID, 1.75, borderColor);
    HGDIOBJ oldBrush = SelectObject(hdc, hBrush);
    HGDIOBJ oldPen = SelectObject(hdc, hPen);

    int standTop = y0 + (int)(0.8f * h);
    int standBottom = y0 + h;
    int standLeft = x0 + (int)(0.2f * w);
    int standRight = x0 + (int)(0.8f * w);
    Ellipse(hdc, standLeft, standTop, standRight, standBottom);

    POINT bodyPts[4];
    bodyPts[0] = { x0 + (int)(0.35f * w), y0 + (int)(0.50f * h) };
    bodyPts[1] = { x0 + (int)(0.65f * w), y0 + (int)(0.50f * h) };
    bodyPts[2] = { x0 + (int)(0.70f * w), y0 + (int)(0.80f * h) };
    bodyPts[3] = { x0 + (int)(0.30f * w), y0 + (int)(0.80f * h) };
    Polygon(hdc, bodyPts, 4);

    POINT headPts[10];
    headPts[0] = { x0 + (int)(0.65f * w), y0 + (int)(0.50f * h) }; 
    headPts[1] = { x0 + (int)(0.60f * w), y0 + (int)(0.30f * h) };
    headPts[2] = { x0 + (int)(0.55f * w), y0 + (int)(0.10f * h) };
    headPts[3] = { x0 + (int)(0.45f * w), y0 + (int)(0.20f * h) };
    headPts[4] = { x0 + (int)(0.45f * w), y0 + (int)(0.20f * h) };
    headPts[5] = { x0 + (int)(0.35f * w), y0 + (int)(0.10f * h) };
    headPts[6] = { x0 + (int)(0.15f * w), y0 + (int)(0.40f * h) };
    headPts[7] = { x0 + (int)(0.20f * w), y0 + (int)(0.50f * h) }; 
    headPts[8] = { x0 + (int)(0.25f * w), y0 + (int)(0.55f * h) };
    headPts[9] = { x0 + (int)(0.35f * w), y0 + (int)(0.50f * h) };
    Polygon(hdc, headPts, 10);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
} 
void Knight::updateAvailableMoves(bool isWhite,bool incheck) {
    availableMoves.clear();

    const int dx[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    const int dy[8] = { 1, 2,  2,  1, -1, -2, -2, -1 };

    int baseX = gridX, baseY = gridY;

    for (int i = 0; i < 8; i++) {
        int candidateX = baseX + dx[i];
        int candidateY = baseY + dy[i];

        if (candidateX >= 0 && candidateX < 8 && candidateY >= 0 && candidateY < 8) {
            int pixelX = globalBoard->boardOrgX + candidateX * globalBoard->cellSize;
            int pixelY = globalBoard->boardOrgY + candidateY * globalBoard->cellSize;

            IPiece* p = gameInstance.getPieces()->getPieceAt(
                pixelX + globalBoard->cellSize / 4,
                pixelY + globalBoard->cellSize / 4,
                globalBoard->boardOrgX, globalBoard->boardOrgY,
                globalBoard->cellSize
            );

            if (p == nullptr || (p != nullptr && p->faction != this->faction)) {
                POINT candidateMove = { candidateX, candidateY };
                availableMoves.push_back(candidateMove);
            }
        }
    }
    if (!incheck)
        updateCheckMoves();
}
void Knight::renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) {
    for (const POINT& move : availableMoves) {
        int drawX = boardOrgX + move.x * cellSize ;
        int drawY = boardOrgY + move.y * cellSize ;

        IPiece* p = gameInstance.getPieces()->getPieceAt(drawX + cellSize / 4, drawY + cellSize / 4,
            boardOrgX, boardOrgY, cellSize);
        COLORREF color = (p != nullptr && p->faction != this->faction)
            ? RGB(240, 10, 10) 
            : RGB(245, 245, 245); 

        HBRUSH brush = CreateSolidBrush(color);
        SelectObject(hdc, brush);

        int margin = cellSize / 3.5;
        Ellipse(hdc, drawX + margin, drawY + margin, drawX + cellSize - margin, drawY + cellSize - margin);

        DeleteObject(brush);
    }
}
void Knight::capture(IPiece* other)
{
    gameInstance.getPieces()->removePiece(other);
    auto& checkingPieces = gameInstance.checkingPieces;
    checkingPieces.erase(
        std::remove(checkingPieces.begin(), checkingPieces.end(), other),
        checkingPieces.end()
    );

    delete other;
}
void Knight::move(int mx, int my, int fromX, int fromY, int whereX, int whereY, bool& validMove) {
    for (const POINT& move : gSelectedPiece->availableMoves) {
        if (move.x == whereX && move.y == whereY) {

            IPiece* target = gameInstance.getPieces()->getPieceAt(
                mx, my,
                globalBoard->boardOrgX, globalBoard->boardOrgY, globalBoard->cellSize);
            if (target != nullptr) {
                gSelectedPiece->capture(target);
            }
            gSelectedPiece->gridX = whereX;
            gSelectedPiece->gridY = whereY;
            gSelectedPiece->updateAvailableMoves(globalBoard->isWhite,false);
            validMove = true;
            break;
        }
    }
}
void Knight::updateCheckMoves()
{
    if (gameInstance.checkState.factionChecking == this->faction)
    {
        return;
    }

    std::vector<POINT> restrictedMoves;

    auto originalCheckState = gameInstance.checkState;
    auto originalCheckingPieces = gameInstance.checkingPieces;
    auto originalCheckingKing = gameInstance.checkState.checkedKing;

    int origX = this->gridX;
    int origY = this->gridY;

    for (auto move : this->availableMoves)
    {
        IPiece* targetPiece = gameInstance.pieces_->getPieceAtGrid(move.x, move.y);
        bool captured = false;
        IPiece* removedPiece = nullptr;

        if (targetPiece != nullptr && targetPiece->faction != this->faction)
        {
            removedPiece = targetPiece;
            gameInstance.pieces_->removePiece(removedPiece);
            captured = true;
        }

        this->gridX = move.x;
        this->gridY = move.y;

        for (auto piece : originalCheckingPieces)
        {
            piece->updateAvailableMoves(globalBoard->isWhite, true);
        }

        if (originalCheckingPieces.empty())
        {
            for (auto piece : gameInstance.getPieces()->getAllPieces())
            {
                if (piece != this && piece->faction != this->faction)
                {
                    piece->updateAvailableMoves(globalBoard->isWhite, true);
                }
            }
        }

        gameInstance.addCheckingPieces();

        bool wasInCheck = originalCheckState.isInCheck;
        bool isInCheckNow = gameInstance.checkState.isInCheck;
        bool checkCleared = (wasInCheck && !isInCheckNow);
        bool remainsSafe = (!wasInCheck && !isInCheckNow);

        if (checkCleared || remainsSafe)
        {
            restrictedMoves.push_back(move);
        }

        this->gridX = origX;
        this->gridY = origY;

        gameInstance.checkState = originalCheckState;
        gameInstance.checkingPieces = originalCheckingPieces;
        gameInstance.checkState.checkedKing = originalCheckingKing;

        if (captured && removedPiece != nullptr)
        {
            gameInstance.pieces_->addPiece(removedPiece);
        }
    }
   /* std::copy(restrictedMoves.begin(), restrictedMoves.end(),
        std::back_inserter(gameInstance.globalAvailableMoves));*/
    for (auto p : restrictedMoves)
    {
        gameInstance.globalAvailableMoves.push_back({ p,this->faction });
    }
    availableMoves = restrictedMoves;
}