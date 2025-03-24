#include "ImageCache.h"
#pragma comment(lib, "msimg32.lib")

HBITMAP g_hPawnWhite = nullptr;
HBITMAP g_hPawnBlack = nullptr;

void LoadPawnImages(HINSTANCE hInstance)
{
    g_hPawnWhite = (HBITMAP)LoadImage(hInstance, L"pawn_white.png", IMAGE_BITMAP, 0, 0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    g_hPawnBlack = (HBITMAP)LoadImage(hInstance, L"pawn_black.png", IMAGE_BITMAP, 0, 0,
        LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}

void UnloadPawnImages()
{
    if (g_hPawnWhite)
    {
        DeleteObject(g_hPawnWhite);
        g_hPawnWhite = nullptr;
    }
    if (g_hPawnBlack)
    {
        DeleteObject(g_hPawnBlack);
        g_hPawnBlack = nullptr;
    }
}
