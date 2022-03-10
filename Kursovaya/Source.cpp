#ifndef UNICODE
#define UNICODE
#endif


#include <windows.h>
#include "resource.h"

#include <thread>
#include <locale>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void PrintToFile(wchar_t* filePath, wchar_t* buff);



int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"MainWindowClass";

    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

    if (!RegisterClass(&wc)) 
        return -1;

    HMENU hMainMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU1));


    HWND hwndMain = CreateWindow(
        CLASS_NAME,                     // Window class
        L"Empty.txt",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 600,

        NULL,       // Parent window    
        hMainMenu,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwndMain == 0) {
        return 0;
    }

    RECT MainRect = {};
    GetWindowRect(hwndMain, &MainRect);

    int MainWitdh = MainRect.right - MainRect.left;
    int MainHeight = MainRect.bottom - MainRect.top;

    HWND hwndEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | WS_HSCROLL | WS_VSCROLL | ES_AUTOVSCROLL, 0, 0, MainWitdh - 20, MainHeight - 60, hwndMain, (HMENU)ID_EDIT, hInstance, NULL);
    
    RegisterHotKey(hwndMain, ID_CTRLS, MOD_CONTROL, 0x53);
    RegisterHotKey(hwndMain, ID_CTRLO, MOD_CONTROL, 0x4F);


    ShowWindow(hwndMain, nCmdShow);
    ShowWindow(hwndEdit, nCmdShow);


    //Message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }




    return 0;

}

//hwnd - window handle; uMsg - message code; wParam & lParam - additional data
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { 
    HWND hwndChildEdit = FindWindowEx(hwnd, NULL, L"EDIT", NULL);
    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    
    
    switch (uMsg) {

    case WM_PAINT:
    {
        HFONT hfont = CreateFont(16, 7, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, MAKELPARAM(VARIABLE_PITCH, FF_MODERN), NULL);
        SendMessage(hwndChildEdit, WM_SETFONT, MAKEWPARAM(hfont, 0), MAKELPARAM(TRUE, 0));
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        const COLORREF clr = RGB(255, 255, 255);

        HBRUSH brush = CreateSolidBrush(clr);
        FillRect(hdc, &ps.rcPaint, brush);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_SIZE:
    {
       
        int nwidth = LOWORD(lParam);
        int nheight = HIWORD(lParam);
        SetWindowPos(hwndChildEdit, NULL, 0, 0, nwidth, nheight, NULL);


    }

    case WM_COMMAND:
    {
        if (HIWORD(wParam) == EN_CHANGE) {
            wchar_t buff[128];
            GetWindowText(hwnd, buff, 128);
            if (wcschr(buff, L'*') == NULL) {
                int len = wcslen(buff);
                buff[len] = L'*';
                buff[len + 1] = L'\0';
                SetWindowText(hwnd, buff);
            }
        }
        if (LOWORD(wParam) == ID_FILE_OPEN) {
            wchar_t filePath[128];
            filePath[0] = NULL;
            OPENFILENAME FileStruct = {NULL};
            FileStruct.lStructSize = sizeof(OPENFILENAME);
            FileStruct.lpstrFile = filePath;
            FileStruct.nMaxFile = 128;
            GetOpenFileName(&FileStruct);

            std::wifstream wifs(filePath);
            if (wifs.is_open()) {
                SetWindowText(hwnd, filePath);
                wifs.imbue(utf8_locale);
                std::wstringstream buff;
                buff << wifs.rdbuf();
                std::wstring ws = buff.str();
                SetWindowText(hwndChildEdit, ws.c_str());
            }
            wifs.close();
            

        }

        if (LOWORD(wParam) == ID_FILE_SAVE) {
            wchar_t filePath[128];
            wchar_t curFile[128];
            GetWindowText(hwnd, curFile, 128);

            if ((wcscmp(curFile, L"Empty.txt") == 0) || (wcscmp(curFile, L"Empty.txt*") == 0)) {

                filePath[0] = NULL;
                OPENFILENAME FileStruct = { NULL };
                FileStruct.lStructSize = sizeof(OPENFILENAME);
                FileStruct.lpstrFile = filePath;
                FileStruct.nMaxFile = 128;
                GetSaveFileName(&FileStruct);
            }
            else {
                GetWindowText(hwnd, filePath, 128);
            }
            wchar_t buff[1024];
            GetWindowText(hwndChildEdit, buff, 1024);

            if (wcscmp(filePath, L"") != 0)
                if (wcschr(filePath, L'*') != NULL) {
                    filePath[wcslen(filePath) - 1] = L'\0';
                    SetWindowText(hwnd, filePath);
                }

            std::thread to(PrintToFile, filePath, buff);
            to.join();


        }

        if (LOWORD(wParam) == ID_FILE_SAVEAS) {
            wchar_t filePath[128];
            filePath[0] = NULL;
            OPENFILENAME FileStruct = { NULL };
            FileStruct.lStructSize = sizeof(OPENFILENAME);
            FileStruct.lpstrFile = filePath;
            FileStruct.nMaxFile = 128;
            GetSaveFileName(&FileStruct);
            SetWindowText(hwnd, filePath);
            wchar_t buff[1024];
            GetWindowText(hwndChildEdit, buff, 1024);

            std::thread to(PrintToFile, filePath, buff);
            to.join();
        }

        if (LOWORD(wParam) == ID_FILE_EXIT)
            PostQuitMessage(0);

        return 0;
    }

    case WM_HOTKEY:
    {
        if (wParam == ID_CTRLS) {
            SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_FILE_SAVE, NULL), NULL);
        }

        if (wParam == ID_CTRLO) {
            SendMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_FILE_OPEN, NULL), NULL);
        }
    }
    return 0;


    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);;

}


void PrintToFile(wchar_t* filePath, wchar_t* buff) {
    const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());

    std::wofstream wofs(filePath, std::ios_base::out);
    if (wofs.is_open()) {
        wofs.imbue(utf8_locale);
        wofs << buff;
    }
    wofs.close();
}





