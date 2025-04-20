#include <windows.h>
#include <windowsx.h> 
#include "GameLogic.h"
#include "Pieces.h"
#include <string>
#include <vector>
#include "Knight.h"

Game::Game() {
	gBoard_ = std::make_unique<Board>();
	pieces_ = std::make_unique<Pieces>();
	currentTurn = gBoard_->isWhite ? 1 : 0;
    this->checkState.checkedKing = nullptr;
    this->checkState.factionChecking = -1;
    this->checkState.isInCheck = false;
}

void Game::updatePieces() {
	pieces_->addPieces(gBoard_->isWhite);
}
#include <string>
#include <codecvt> 
#include <locale>  

void Game::render(HDC hdc) {
    int box = gBoard_->boardOrgX;
    int boy = gBoard_->boardOrgY;
    int cell = gBoard_->cellSize;

    if (gBoard_->visible) {
        gBoard_->render(hdc);
        pieces_->renderAllPieces(hdc, box, boy, cell);
        gBoard_->getArrowManager()->render(hdc, gBoard_->boardOrgX, gBoard_->boardOrgY, gBoard_->cellSize, gBoard_->gridSize);
    }

    if (gBoard_->firstPieceMove) {
        IPiece* lastMovedPiece = nullptr;
        for (auto& piece : pieces_->getAllPieces()) {
            if (piece->gridX == gBoard_->lastPMcurrX && piece->gridY == gBoard_->lastPMcurrY) {
                lastMovedPiece = piece;
                break;
            }
        }
        bool isFirstMove = lastMovedPiece ? lastMovedPiece->isFirstMove : false;

        std::wstring debugText = L"Last Move: (" +
            std::to_wstring(gBoard_->lastPMprevX) + L", " +
            std::to_wstring(gBoard_->lastPMprevY) + L") -> (" +
            std::to_wstring(gBoard_->lastPMcurrX) + L", " +
            std::to_wstring(gBoard_->lastPMcurrY) + L") | First Move: " +
            (isFirstMove ? L"True" : L"False");

        SetTextColor(hdc, RGB(255, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, 10, 10, debugText.c_str(), debugText.length());
    }
    int offsetY = 30; 
    for (int y = 0; y < 9; ++y) {
        std::wstring rowTextWhite = L"";
        std::wstring rowTextBlack = L"";
        for (int x = 0; x < 9; ++x) {
            rowTextWhite += std::to_wstring(gBoard_->attackedByWhite[y][x]) + L" ";
            rowTextBlack += std::to_wstring(gBoard_->attackedByBlack[y][x]) + L" ";
        }

        TextOut(hdc, 10, offsetY + (y * 15), rowTextWhite.c_str(), rowTextWhite.length());
        TextOut(hdc, 200, offsetY + (y * 15), rowTextBlack.c_str(), rowTextBlack.length());
    }

    int checkOffsetY = offsetY + 150; 
    std::wstring checkText = L"Checking Pieces:\n";

    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; 
    std::wstring emptyText = L"EMPTY:\n";
    if(checkingPieces.empty())
        TextOut(hdc, 20, checkOffsetY+20, emptyText.c_str(), emptyText.length());
    for (IPiece* piece : checkingPieces) {
        std::wstring pieceType = converter.from_bytes(typeid(*piece).name()); 
        std::wstring pieceInfo = L"Type: " + pieceType + L", Faction: " +
            std::to_wstring(piece->faction) + L"\n";
        checkText += pieceInfo;
    }

    TextOut(hdc, 10, checkOffsetY, checkText.c_str(), checkText.length());
}
void Game::addCheckingPieces()
{
    this->checkingPieces.clear();
    this->checkState.isInCheck = false;
    this->checkState.factionChecking = -1;
    this->checkState.checkedKing = nullptr;

    for (auto p : this->pieces_->getAllPieces())
    {
        if (isAttackingEnemyKing(p))
        {
            checkingPieces.push_back(p);
            this->checkState.factionChecking = p->faction;
            IPiece* enemyKing = nullptr;
            for (auto piece : this->getPieces()->pieces[!p->faction])
            {
                if (dynamic_cast<King*>(piece) != nullptr)
                {
                    enemyKing = piece;
                    break;
                }
            }
            this->checkState.checkedKing = enemyKing;
            this->checkState.isInCheck = true;
        }
    }
}

bool Game::isAttackingEnemyKing(IPiece* attacker)
{
    IPiece* enemyKing = nullptr;
    for (auto piece : this->getPieces()->pieces[!attacker->faction])
    {
        if (dynamic_cast<King*>(piece) != nullptr)
        {
            enemyKing = piece;
            break;
        }
    }

    if (enemyKing == nullptr)
        return false;

    for (auto move : attacker->availableMoves)
    {
        if (move.x == enemyKing->gridX && move.y == enemyKing->gridY)
        {
            return true;
        }
    }
    return false;
}
int Game::checkLoss()
{
    if (this->checkState.isInCheck==true)
    {
        int cnt = 0;
        bool factionChecked = !checkState.factionChecking;
        for (auto i : this->globalAvailableMoves)
        {
            if (i.second == factionChecked)
                cnt++;
        }
        if (cnt == 0)
        {
            return factionChecked;
        }
    }
    else
    {
        bool currentFactionTurn = this->currentTurn;
        int cnt = 0;
        for (auto i : this->globalAvailableMoves)
        {
            if (i.second == currentFactionTurn)
                cnt++;
        }
        if (cnt)
            return -1;
        else
            return 2;
    }
}