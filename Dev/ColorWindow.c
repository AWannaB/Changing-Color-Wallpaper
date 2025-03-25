#include <windows.h>
#include <stdlib.h>
#include <time.h>

HBRUSH hBackgroundBrush = NULL;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int ranTime() {
    int min = 100, max = 10000;
    int ranNum = rand() % (max - min + 1) + min;
    return ranNum;
}

HBRUSH color() {
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;

    if (hBackgroundBrush) {
        DeleteObject(hBackgroundBrush);
    }
    hBackgroundBrush = CreateSolidBrush(RGB(r, g, b));
    return hBackgroundBrush;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char CLASS_NAME[] = "BlueWindowClass";

    srand(time(NULL));

    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = color();

    RegisterClass(&wc);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Window",
        WS_POPUP,
        0, 0, screenWidth, screenHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    ShowCursor(FALSE);

    SetTimer(hwnd, 1, ranTime(), NULL);

    // Message loop
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (hBackgroundBrush) {
        DeleteObject(hBackgroundBrush);
    }
    return 0;
    }



// Window procedure to handle messages

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_TIMER:
            color(); // Generate new color
            InvalidateRect(hwnd, NULL, TRUE); // Trigger repaint
            break;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) {
                PostQuitMessage(0); // Exit on ESC
                } else {
                color(); // Change color on any other key
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, hBackgroundBrush);
            EndPaint(hwnd, &ps);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}