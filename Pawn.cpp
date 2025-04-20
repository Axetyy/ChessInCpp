#include "Pieces.h"
#include "Pawn.h"
#include "GameLogic.h"
#include <windows.h>
#include <string>
#include <sstream>
#include "King.h"

extern Game gameInstance;
extern Board* globalBoard;
extern IPiece* gSelectedPiece;

void Pawn::render(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) {
    int drawX, drawY;
    drawX = boardOrgX + gridX * cellSize;
    drawY = boardOrgY + gridY * cellSize;


    COLORREF fillColor = (faction == 1) ? RGB(255, 255, 255) : RGB(0, 0, 0);
    COLORREF borderColor = RGB(0, 0, 0);

    HBRUSH hBrush = CreateSolidBrush(fillColor);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 1.75, borderColor);
    HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
    int margin = cellSize / 10; 
    int centerX = drawX + cellSize / 2;
    int baseY = drawY + cellSize - margin;

    int headRadius = cellSize / 5;  
    int bodyWidth = cellSize / 3;      
    int bodyHeight = cellSize / 3;     
    int baseRadius = cellSize / 3;     

    Ellipse(hdc, centerX - headRadius, drawY + margin, centerX + headRadius, drawY + margin + 2 * headRadius);

    int bodyTop = drawY + margin + 2 * headRadius;
    Rectangle(hdc, centerX - bodyWidth / 2, bodyTop, centerX + bodyWidth / 2, bodyTop + bodyHeight);

    int baseTop = bodyTop + bodyHeight;
    Ellipse(hdc, centerX - baseRadius, baseTop, centerX + baseRadius, baseY);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

void Pawn::updateAvailableMoves(bool isWhite,bool incheck) {
    this->availableMoves.clear();

    int direction = 0;
    int startingRow = 0;

    if (isWhite) {
        direction = (faction == 1) ? -1 : 1;
        startingRow = (faction == 1) ? 6 : 1;
    }
    else {
        direction = (faction == 0) ? -1 : 1;
        startingRow = (faction == 0) ? 6 : 1;
    }

    std::vector<IPiece*> allPieces = gameInstance.getPieces()->getAllPieces();

    bool forwardBlocked = false;
    for (auto p : allPieces) {
        if (p != this && p->gridX == gridX && p->gridY == gridY + direction) {
            forwardBlocked = true;
            break;
        }
    }

    if (!forwardBlocked) {
        this->availableMoves.push_back({ gridX, gridY + direction });

        if (gridY == startingRow) {
            bool doubleBlocked = false;
            for (auto p : allPieces) {
                if (p != this && p->gridX == gridX && p->gridY == gridY + 2 * direction) {
                    doubleBlocked = true;
                    break;
                }
            }
            if (!doubleBlocked) {
                this->availableMoves.push_back({ gridX, gridY + 2 * direction });
            }
        }
    }
    int diag1X = this->gridX + 1;
    int diag1Y = this->gridY + direction;
    int diag2X = this->gridX - 1;
    int diag2Y = this->gridY + direction;
    for (auto p : allPieces)
    {
        if (p != this &&  p->faction!=this->faction && p->gridX == diag1X && p->gridY == diag1Y)
        {
            availableMoves.push_back({ diag1X,diag1Y });
        }
        if (p != this && p->faction !=this->faction && p->gridX == diag2X && p->gridY == diag2Y)
        {
            availableMoves.push_back({ diag2X,diag2Y });
        }
    }

    LastPawnMove last = gameInstance.getPieces()->move;
    if (last.pieceThatMoved != nullptr && last.pieceThatMoved != this) {
        if (abs(last.origY - last.curY) == 2 && last.pieceThatMoved->faction != this->faction) {
            if (this->gridY == last.curY) {
                if (abs(this->gridX - last.curX) == 1) {
                    int epY = last.curY + direction;
                    availableMoves.push_back({ last.curX, epY });
                }
            }
        }
    }
    if (!incheck)
        updateCheckMoves();

}

void Pawn::renderAvailableMoves(HDC hdc, int boardOrgX, int boardOrgY, int cellSize) {
    for (const POINT& move : this->availableMoves)
    {

        int drawX = boardOrgX + move.x * cellSize;
        int drawY = boardOrgY + move.y * cellSize;
        bool isDiagonal = (abs(move.x - this->gridX) == 1 && (move.y - this->gridY) != 0);
        HBRUSH brush;
        if (isDiagonal)
            brush = CreateSolidBrush(RGB(240, 10, 10)); 
        else
            brush = CreateSolidBrush(RGB(245, 245, 245)); 

        SelectObject(hdc, brush);
        int margin = cellSize / 3.5;
        Ellipse(hdc, drawX + margin, drawY + margin, drawX + cellSize - margin, drawY + cellSize - margin);
        DeleteObject(brush);
    }
}

void Pawn::move(int mx,int my,int fromX, int fromY, int whereX, int whereY, bool& validMove)
{
    bool enPassantMove = false;
    for (const POINT& move : gSelectedPiece->availableMoves) {
        if (move.x == whereX && move.y == whereY) {

            IPiece* target = gameInstance.getPieces()->getPieceAt(
                mx, my,
                globalBoard->boardOrgX, globalBoard->boardOrgY, globalBoard->cellSize);
            if (target != nullptr) {
                gSelectedPiece->capture(target);
            }
            else {
                LastPawnMove last = gameInstance.getPieces()->move;
                if (last.pieceThatMoved != nullptr &&
                    gSelectedPiece->faction != last.pieceThatMoved->faction)
                {
                    if (abs(gSelectedPiece->gridX - last.curX) == 1 &&
                        gSelectedPiece->gridY == last.curY)
                    {
                        int enemyDirection = (globalBoard->isWhite) ? (last.pieceThatMoved->faction == 1) ? +1 : -1 : (last.pieceThatMoved->faction == 1) ? -1 : +1;
                        int enPassantDestY = last.curY + enemyDirection;
                        if (whereY == enPassantDestY) {
                            gSelectedPiece->capture(last.pieceThatMoved);
                            enPassantMove = true;
                        }
                    }
                }
            }
            if (typeid(*gSelectedPiece).name() == typeid(Pawn).name()) {
                int startingRow = globalBoard->isWhite
                    ? ((gSelectedPiece->faction == 1) ? 6 : 1)
                    : ((gSelectedPiece->faction == 0) ? 6 : 1);
                if (gSelectedPiece->gridY == startingRow &&
                    abs(gSelectedPiece->gridY - whereY) == 2) {
                    gameInstance.getPieces()->move.origX = gSelectedPiece->gridX;
                    gameInstance.getPieces()->move.origY = gSelectedPiece->gridY;
                    gameInstance.getPieces()->move.curX = whereX;
                    gameInstance.getPieces()->move.curY = whereY;
                    gameInstance.getPieces()->move.pieceThatMoved = gSelectedPiece;
                }
                else {
                    gameInstance.getPieces()->move.pieceThatMoved = nullptr;
                }
            }

            gSelectedPiece->gridX = whereX;
            gSelectedPiece->gridY = whereY;
            gSelectedPiece->updateAvailableMoves(globalBoard->isWhite,false);
            validMove = true;
            break;
        }
    }
}
void Pawn::updateCheckMoves()
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
    //std::copy(restrictedMoves.begin(), restrictedMoves.end(),
    //    std::back_inserter(gameInstance.globalAvailableMoves));
    for (auto p : restrictedMoves)
    {
        gameInstance.globalAvailableMoves.push_back({ p,this->faction });
    }

    availableMoves = restrictedMoves;
}
void Pawn::capture(IPiece* other) {
    gameInstance.getPieces()->removePiece(other);
    auto& checkingPieces = gameInstance.checkingPieces;
    checkingPieces.erase(
        std::remove(checkingPieces.begin(), checkingPieces.end(), other),
        checkingPieces.end()
    );

    delete other;
}
