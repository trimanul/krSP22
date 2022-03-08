#ifndef UNICODE
#define UNICODE
#endif


#include <windows.h>
#include "resource.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, INT nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"MainWindowClass";

    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc)) 
        return -1;

    HMENU hMainMenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU1));


    HWND hwndMain = CreateWindow(
        CLASS_NAME,                     // Window class
        L"WINAPI",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

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

    HWND hwndEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT | WS_HSCROLL | WS_VSCROLL | ES_AUTOVSCROLL, 0, 0, MainWitdh - 20, MainHeight - 100, hwndMain, (HMENU)ID_EDIT, hInstance, NULL);
    HWND hwndBtn = CreateWindow(L"BUTTON", L"END", WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, 10, MainHeight - 95, 100, 30, hwndMain, (HMENU)ID_BTN, hInstance, NULL);

    


    ShowWindow(hwndMain, nCmdShow);
    ShowWindow(hwndBtn, nCmdShow);
    ShowWindow(hwndEdit, nCmdShow);


    //Message loop
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }




    return 0;

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { //hwnd - window handle; uMsg - message code; wParam & lParam - additional data
    
    switch (uMsg) {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        const COLORREF clr = RGB(255, 255, 255);

        HBRUSH brush = CreateSolidBrush(clr);
        FillRect(hdc, &ps.rcPaint, brush);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_COMMAND:
    {
        if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ID_BTN) {
            HWND hwndChildEdit = FindWindowEx(hwnd, NULL, L"EDIT", NULL);
            SetWindowText(hwndChildEdit, L"Hello World!");
        }

        if (LOWORD(wParam) == ID_FILE_OPEN) {
            HWND hwndChildEdit = FindWindowEx(hwnd, NULL, L"EDIT", NULL);
            wchar_t filePath[128];
            filePath[0] = NULL;
            OPENFILENAME FileStruct = {NULL};
            FileStruct.lStructSize = sizeof(OPENFILENAME);
            FileStruct.lpstrFile = filePath;
            FileStruct.nMaxFile = 128;
            GetOpenFileName(&FileStruct);

            std::wifstream wifs(filePath);
            if (wifs.is_open()) {
                std::wstringstream buff;
                buff << wifs.rdbuf();
                std::wstring ws = buff.str();

                SetWindowText(hwndChildEdit, ws.c_str());
            }
            wifs.close();
            

        }

        if (LOWORD(wParam) == ID_FILE_SAVE) {
            HWND hwndChildEdit = FindWindowEx(hwnd, NULL, L"EDIT", NULL);
            wchar_t filePath[128];
            filePath[0] = NULL;
            OPENFILENAME FileStruct = { NULL };
            FileStruct.lStructSize = sizeof(OPENFILENAME);
            FileStruct.lpstrFile = filePath;
            FileStruct.nMaxFile = 128;
            GetSaveFileName(&FileStruct);

            wchar_t buff[1024];
            GetWindowText(hwndChildEdit, buff, 1024);


            std::wofstream wofs(filePath, std::ios_base::out);
            if (wofs.is_open()) {
                wofs << buff;
            }
            wofs.close();


        }
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);;

}




