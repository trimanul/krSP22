#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "resource.h"
#include "CommCtrl.h"
#include <vector>
#include <algorithm>
#include <string>

LRESULT CALLBACK FontWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK EnumFontFamExProc(const LOGFONT* lpelfe, const TEXTMETRIC* lpntme, DWORD FontType, LPARAM lParam);

LOGFONTW logfont = { 0 };
std::wstring selFont;
LONG selHeight = 0;


 bool CreateFontWndClass(HINSTANCE hInstance) {
    WNDCLASS wc = { 0 };
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpfnWndProc = FontWindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"FontWndClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc))
        return false;

    else
        return true;
}

HWND CreateFontWindow(HINSTANCE hInstance, HWND hwndParent) {

	HWND hwndFont = CreateWindow(
        L"FontWndClass",                     // Window class
        L"Font",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 600,

        hwndParent,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwndFont == 0) {
        return 0;
    }
    else {
        return hwndFont;
    }
}

LRESULT CALLBACK FontWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    std::vector<std::wstring> fnvec;
    logfont.lfHeight = 16;
    logfont.lfWeight = FW_DONTCARE;
    logfont.lfCharSet = DEFAULT_CHARSET;
    logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    logfont.lfQuality = DRAFT_QUALITY;
    logfont.lfPitchAndFamily = MAKELPARAM(VARIABLE_PITCH, FF_MODERN);
    wcscpy_s(logfont.lfFaceName, L"");
    HWND hwndChildEdit = FindWindowEx(FindWindowEx(NULL, NULL, L"MainWindowClass", NULL), NULL, L"EDIT", NULL);
    
    HWND hwndFontComboBox = NULL;
    HWND hwndSizeComboBox = NULL;


    switch (uMsg) {
    case WM_CREATE:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        const COLORREF clr = RGB(255, 255, 255);

        HBRUSH brush = CreateSolidBrush(clr);
        FillRect(hdc, &ps.rcPaint, brush);
        EndPaint(hwnd, &ps);

        hwndFontComboBox = CreateWindow(WC_COMBOBOX, TEXT("Size"),
            CBS_SIMPLE | CBS_HASSTRINGS | CBS_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
            70, 50, 300, 400, hwnd, (HMENU)ID_FONTCOMBOBOX, hInstance,
            NULL);

        hwndSizeComboBox = CreateWindow(WC_COMBOBOX, TEXT("Font"),
            CBS_SIMPLE | CBS_HASSTRINGS | CBS_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
            530, 50, 300, 400, hwnd, (HMENU)ID_SIZECOMBOBOX, hInstance,
            NULL);

        HWND hwndBtnOK = CreateWindow(WC_BUTTON, TEXT("OK"), WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 100, 500, 100, 25, hwnd, (HMENU)ID_BTN_OK, hInstance, NULL);
        ShowWindow(hwndBtnOK, SW_NORMAL);

        HWND hwndBtnCancel = CreateWindow(WC_BUTTON, TEXT("CANCEL"), WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 225, 500, 100, 25, hwnd, (HMENU)ID_BTN_CANCEL, hInstance, NULL);
        ShowWindow(hwndBtnCancel, SW_NORMAL);


        EnumFontFamiliesEx(GetDC(NULL), &logfont, EnumFontFamExProc, (LPARAM) &fnvec, NULL);

        for (size_t i = 0; i < fnvec.size(); i++)
        {
            SendMessage(hwndFontComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)fnvec[i].c_str());
        }
        SendMessage(hwndFontComboBox, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

        return 0;

    }
    case WM_COMMAND: {

        if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == ID_FONTCOMBOBOX)
        {
            wchar_t  ListItem[256];
            int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
                (WPARAM)0, (LPARAM)0);
            (TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
                (WPARAM)ItemIndex, (LPARAM)ListItem);
            selFont = ListItem;

            std::vector<std::wstring> lfvec = {
                L"11",
                L"12",
                L"14",
                L"16",
                L"18",
                L"20",
                L"22",
                L"24",
                L"26",
                L"28",
                L"36",
                L"48",
                L"72"
            };

            HWND hwndSizeComboBox = FindWindowEx(hwnd, (HWND)lParam, WC_COMBOBOX, NULL);
            SendMessage(hwndSizeComboBox, CB_RESETCONTENT, NULL, NULL);
            for (size_t i = 0; i < lfvec.size(); i++)
            {
                SendMessage(hwndSizeComboBox, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)lfvec[i].c_str());
            }
            SendMessage(hwndSizeComboBox, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);
            
        }


        if (HIWORD(wParam) == CBN_SELCHANGE && LOWORD(wParam) == ID_SIZECOMBOBOX)
        {
            wchar_t  ListItem[256];
            int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
                (WPARAM)0, (LPARAM)0);
            (TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
                (WPARAM)ItemIndex, (LPARAM)ListItem);
            selHeight = _wtol(ListItem);
        }
         
        if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ID_BTN_OK) {
            wcscpy_s(logfont.lfFaceName, selFont.c_str());
            logfont.lfHeight = selHeight;
            HFONT font = CreateFontIndirect(&logfont);
            SendMessage(hwndChildEdit, WM_SETFONT, MAKEWPARAM(font, 0), MAKELPARAM(TRUE, 0));
            SendMessage(hwnd, WM_DESTROY, NULL, NULL);

        }

        if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ID_BTN_CANCEL) {
            SendMessage(hwnd, WM_DESTROY, NULL, NULL);

        }

        return 0;
    }

    case WM_DESTROY:
        DestroyWindow(hwnd);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

int CALLBACK EnumFontFamExProc(const LOGFONT* lpelfe, const TEXTMETRIC* lpntme, DWORD FontType, LPARAM fvec) {
    std::vector<std::wstring>* fnvec = (std::vector<std::wstring>*)fvec;
    if (!lpelfe)
        return 0;
    else {
        if (std::find(fnvec->begin(), fnvec->end(), std::wstring(lpelfe->lfFaceName)) != fnvec->end()) {
            return 1;
        }
        else {
            fnvec->push_back(std::wstring(lpelfe->lfFaceName));
            return 1;
        }

    }
}