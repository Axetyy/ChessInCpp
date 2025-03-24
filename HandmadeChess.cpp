

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <ctime>
#include "GameLogic.h"
#include <string>

#define ID_PLAY_BUTTON  1
#define ID_EXIT_BUTTON  2
#define ID_PLAYASBLACK_BUTTON 3
#define ID_PLAYASWHITE_BUTTON 4
#define ID_RANDOM_BUTTON 5
#define IDI_MAINICON 3002

Game gameInstance;
Board* globalBoard = gameInstance.getBoard();
IPiece* gSelectedPiece = nullptr;

HANDLE hStockfishInput, hStockfishOutput;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {


	// Define and register the window class.
	const TCHAR CLASS_NAME[] = _T("ChesssWindowClass");
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAINICON));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, _T("Window Registration Failed!"), _T("Error"), MB_ICONERROR);
		return 0;
	}

	// Create the main window with WS_CLIPCHILDREN to protect child controls.

	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		_T("Chess"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,       // Parent window.
		NULL,       // Menu.
		hInstance,  // Instance handle.
		NULL        // Additional application data.
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, _T("Window Creation Failed!"), _T("Error"), MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
	{
	case WM_ERASEBKGND:
		return 0;
	case WM_CREATE:
	{
		CreateWindowEx(
			0,
			_T("BUTTON"),
			_T("Play"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			50, 50, 300, 100,      // x, y, width, height
			hwnd,
			(HMENU)ID_PLAY_BUTTON,
			((LPCREATESTRUCT)lParam)->hInstance,
			NULL);
		CreateWindowEx(
			0,
			_T("BUTTON"),
			_T("Exit"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			50, 90, 300, 100,      // x, y, width, height
			hwnd,
			(HMENU)ID_EXIT_BUTTON,
			((LPCREATESTRUCT)lParam)->hInstance,
			NULL);
		CreateWindowEx(
			0,
			_T("BUTTON"),
			_T("Play as Black"),
			WS_CHILD | BS_PUSHBUTTON,
			50, 50, 210, 70,      // x, y, width, height
			hwnd,
			(HMENU)ID_PLAYASBLACK_BUTTON,
			((LPCREATESTRUCT)lParam)->hInstance,
			NULL);
		CreateWindowEx(
			0,
			_T("BUTTON"),
			_T("Play as White"),
			WS_CHILD | BS_PUSHBUTTON,
			50, 50, 210, 70,      // x, y, width, height
			hwnd,
			(HMENU)ID_PLAYASWHITE_BUTTON,
			((LPCREATESTRUCT)lParam)->hInstance,
			NULL);
		CreateWindowEx(
			0,
			_T("BUTTON"),
			_T("Random selection"),
			WS_CHILD | BS_PUSHBUTTON,
			50, 50, 210, 70,      // x, y, width, height
			hwnd,
			(HMENU)ID_RANDOM_BUTTON,
			((LPCREATESTRUCT)lParam)->hInstance,
			NULL);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_PLAY_BUTTON:
		{
			HWND hPlayButton = GetDlgItem(hwnd, ID_PLAY_BUTTON);
			ShowWindow(GetDlgItem(hwnd, 1001), SW_SHOW);
			ShowWindow(hPlayButton, SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, ID_PLAYASBLACK_BUTTON), SW_SHOW);
			ShowWindow(GetDlgItem(hwnd, ID_PLAYASWHITE_BUTTON), SW_SHOW);
			ShowWindow(GetDlgItem(hwnd, ID_RANDOM_BUTTON), SW_SHOW);
			HWND hExitButton = GetDlgItem(hwnd, ID_EXIT_BUTTON);


			RECT rc;
			GetClientRect(hwnd, &rc);
			SetWindowPos(hExitButton, NULL, rc.right - 110, 20, 100, 30, SWP_NOZORDER);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}

		case ID_PLAYASBLACK_BUTTON:
		{
			globalBoard->toggleBoard();
			globalBoard->setBlack();
			gameInstance.updatePieces();
			ShowWindow(GetDlgItem(hwnd, ID_PLAYASBLACK_BUTTON), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, ID_PLAYASWHITE_BUTTON), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, ID_RANDOM_BUTTON), SW_HIDE);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_PLAYASWHITE_BUTTON:
		{
			globalBoard->toggleBoard();
			globalBoard->setWhite();
			gameInstance.updatePieces();
			ShowWindow(GetDlgItem(hwnd, ID_PLAYASBLACK_BUTTON), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, ID_PLAYASWHITE_BUTTON), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, ID_RANDOM_BUTTON), SW_HIDE);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_RANDOM_BUTTON:
		{
			globalBoard->toggleBoard();
			srand(std::time(NULL));
			(rand() % 2==0) ? globalBoard->setBlack() : globalBoard->setWhite();
			gameInstance.updatePieces();
			ShowWindow(GetDlgItem(hwnd, ID_PLAYASBLACK_BUTTON), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, ID_PLAYASWHITE_BUTTON), SW_HIDE);
			ShowWindow(GetDlgItem(hwnd, ID_RANDOM_BUTTON), SW_HIDE);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}

		case ID_EXIT_BUTTON:
			PostQuitMessage(0);
			break;
		break;
		}
		break;
	}
	case WM_SIZE:
	{
		HWND hPlayButton = GetDlgItem(hwnd, ID_PLAY_BUTTON);
		HWND hExitButton = GetDlgItem(hwnd, ID_EXIT_BUTTON);

		HWND hPlayAsBlackButton = GetDlgItem(hwnd, ID_PLAYASBLACK_BUTTON);
		HWND hPlayAsWhiteButton = GetDlgItem(hwnd, ID_PLAYASWHITE_BUTTON);
		HWND hRandomSelectionButton = GetDlgItem(hwnd, ID_RANDOM_BUTTON);

		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		RECT rc = {};
		GetClientRect(hPlayButton, &rc);
		int x = (int)(width - (rc.right - rc.left)) / 2;
		int y = (int)(height - (rc.bottom - rc.top)) / 2;
		MoveWindow(hPlayButton, x, y - 100, rc.right - rc.left, rc.bottom - rc.top, 1);
		MoveWindow(hExitButton, x, y, rc.right - rc.left, rc.bottom - rc.top, 1);
		MoveWindow(hPlayAsBlackButton, x, y - 190, rc.right - rc.left, rc.bottom - rc.top, 1);
		MoveWindow(hPlayAsWhiteButton, x, y - 80, rc.right - rc.left, rc.bottom - rc.top, 1);
		MoveWindow(hRandomSelectionButton, x, y+30, rc.right - rc.left, rc.bottom - rc.top, 1);
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		HDC hdcMem = CreateCompatibleDC(hdc);
		RECT rc;
		GetClientRect(hwnd, &rc);
		HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
		HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);

		HBRUSH hbrBkGnd = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
		FillRect(hdcMem, &rc, hbrBkGnd);
		DeleteObject(hbrBkGnd);

		gameInstance.render(hdcMem);

		if (gSelectedPiece!=nullptr) {
			gSelectedPiece->renderAvailableMoves(hdcMem, globalBoard->boardOrgX, globalBoard->boardOrgY, globalBoard->cellSize);
		}
		

		BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, hbmOld);
		DeleteObject(hbmMem);
		DeleteDC(hdcMem);

		EndPaint(hwnd, &ps);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		if (globalBoard->visible)
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			int gridMouseX = globalBoard->pixelToGridX(x);
			int gridMouseY = globalBoard->pixelToGridY(y);
			
			if (gSelectedPiece) {
				bool validMove = false;
				int lastX = gSelectedPiece->gridX;
				int lastY = gSelectedPiece->gridY;
				gSelectedPiece->move(x, y, gSelectedPiece->gridX, gSelectedPiece->gridY, gridMouseX, gridMouseY, validMove);
				if (validMove) {
					if (globalBoard->firstPieceMove == false) globalBoard->firstPieceMove = true;
					gameInstance.toggleTurn();
					globalBoard->updateLastMoved(gridMouseX, gridMouseY, lastX, lastY);
				}
				gSelectedPiece = nullptr;
			}
			else {
				IPiece* clickedPiece = gameInstance.getPieces()->getPieceAt(x, y, globalBoard->boardOrgX, globalBoard->boardOrgY, globalBoard->cellSize);
				if (clickedPiece != nullptr && clickedPiece->faction == gameInstance.currentTurn) {
					gSelectedPiece = clickedPiece;
					gSelectedPiece->updateAvailableMoves(globalBoard->isWhite);
				}
				globalBoard->clearRed();
				globalBoard->getArrowManager()->clear();
			}
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
	}

	case WM_LBUTTONUP:
	{
		break;
	}
	case WM_MOUSEMOVE:
	{
		if ((wParam & MK_RBUTTON) && globalBoard->visible)
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			int gridX = globalBoard->pixelToGridX(x);
			int gridY = globalBoard->pixelToGridY(y);
			globalBoard->getArrowManager()->updateDrag(gridX, gridY);
			InvalidateRect(hwnd, NULL, FALSE);
		}
		break;
	}

	case WM_RBUTTONDOWN:
	{
		if (globalBoard->visible)
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			int gridX = globalBoard->pixelToGridX(x);
			int gridY = globalBoard->pixelToGridY(y);
			globalBoard->getArrowManager()->startDrag(gridX, gridY);
			InvalidateRect(hwnd, NULL, FALSE);
		}
		break;
	}
	case WM_RBUTTONUP: {
		if (globalBoard->visible)
		{
			int x = GET_X_LPARAM(lParam);
			int y = GET_Y_LPARAM(lParam);
			int gridX = globalBoard->pixelToGridX(x);
			int gridY = globalBoard->pixelToGridY(y);
			bool isSimpleClick = globalBoard->getArrowManager()->endDrag(gridX, gridY);
			if (isSimpleClick)
			{
				globalBoard->handleRightClick(x, y);
			}
			InvalidateRect(hwnd, NULL, FALSE);
		}
		break;
	}


	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}