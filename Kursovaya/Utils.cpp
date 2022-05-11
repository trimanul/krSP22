#include <windows.h>

bool CALLBACK SetFont(HWND child, LPARAM font) {
    SendMessage(child, WM_SETFONT, font, true);
    return true;
}

void SetDefFont(HWND hwndParent, const wchar_t* fontName, int fontSize) {
    HFONT hfont = CreateFont(fontSize, NULL, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, MAKELPARAM(VARIABLE_PITCH, FF_MODERN), fontName);
    EnumChildWindows(hwndParent, (WNDENUMPROC)SetFont, (LPARAM)MAKELPARAM(hfont, 0));
}

