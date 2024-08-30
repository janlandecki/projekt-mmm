#include <windows.h>
#include <Gdiplus.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <assert.h>

using namespace Gdiplus;

const int WINHEIGHT = 900;
const int WINWIDTH = 1600;
enum {A0 = 1000, A1, B0, B1, B2, C0, C1, C2, D0, D1, D2};

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Projekt MMM");
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND TextBoxa0, TextBoxa1, TextBoxb0, TextBoxb1, TextBoxb2, TextBoxc0, TextBoxc1, TextBoxc2, TextBoxd0, TextBoxd1, TextBoxd2;
bool checkNumber(char text[], HWND hWnd);
int a0 = 1, a1 = 0, b0 = 1, b1 = 0, b2 = 0, c0 = 1, c1 = 0, c2 = 0, d0 = 1, d1 = 0, d2 = 0;
void MyOnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    SolidBrush black(Color(255, 0, 0, 0));
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, 16);

    graphics.DrawString(L"a0 =", 4, &font, PointF(10.0f, 20.0f), &black);
    graphics.DrawString(L"a1 =", 4, &font, PointF(10.0f, 60.0f), &black);
    graphics.DrawString(L"b0 =", 4, &font, PointF(10.0f, 100.0f), &black);
    graphics.DrawString(L"b1 =", 4, &font, PointF(10.0f, 140.0f), &black);
    graphics.DrawString(L"b2 =", 4, &font, PointF(10.0f, 180.0f), &black);
    graphics.DrawString(L"c0 =", 4, &font, PointF(10.0f, 220.0f), &black);
    graphics.DrawString(L"c1 =", 4, &font, PointF(10.0f, 260.0f), &black);
    graphics.DrawString(L"c2 =", 4, &font, PointF(10.0f, 300.0f), &black);
    graphics.DrawString(L"d0 =", 4, &font, PointF(10.0f, 340.0f), &black);
    graphics.DrawString(L"d1 =", 4, &font, PointF(10.0f, 380.0f), &black);
    graphics.DrawString(L"d2 =", 4, &font, PointF(10.0f, 420.0f), &black);
}

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }
    hInst = hInstance;
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        0, 0,
        WINWIDTH, WINHEIGHT + 50,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId;
    double number;
    PAINTSTRUCT ps;
    HDC hdc;
    std::vector<double> x{1, 2, 3}, y{ 3, 5, 1 };
    char buffer[256] = "0";
    switch (message)
    {
    case WM_CREATE: 
        TextBoxa0 = CreateWindow(TEXT("edit"), TEXT("1"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 20, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 20, 22, 22, hWnd, (HMENU)A0, NULL, NULL);

        TextBoxa1 = CreateWindow(TEXT("edit"), TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 60, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 60, 22, 22, hWnd, (HMENU)A1, NULL, NULL);

        TextBoxb0 = CreateWindow(TEXT("edit"), TEXT("1"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 100, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 100, 22, 22, hWnd, (HMENU)B0, NULL, NULL);

        TextBoxb1 = CreateWindow(TEXT("edit"), TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 140, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 140, 22, 22, hWnd, (HMENU)B1, NULL, NULL);

        TextBoxb2 = CreateWindow(TEXT("edit"), TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 180, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 180, 22, 22, hWnd, (HMENU)B2, NULL, NULL);

        TextBoxc0 = CreateWindow(TEXT("edit"), TEXT("1"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 220, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 220, 22, 22, hWnd, (HMENU)C0, NULL, NULL);

        TextBoxc1 = CreateWindow(TEXT("edit"), TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 260, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 260, 22, 22, hWnd, (HMENU)C1, NULL, NULL);

        TextBoxc2 = CreateWindow(TEXT("edit"), TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 300, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 300, 22, 22, hWnd, (HMENU)C2, NULL, NULL);

        TextBoxd0 = CreateWindow(TEXT("edit"), TEXT("1"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 340, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 340, 22, 22, hWnd, (HMENU)D0, NULL, NULL);

        TextBoxd1 = CreateWindow(TEXT("edit"), TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 380, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 380, 22, 22, hWnd, (HMENU)D1, NULL, NULL);

        TextBoxd2 = CreateWindow(TEXT("edit"), TEXT("0"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 420, 50, 22, hWnd, NULL, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("ok"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 120, 420, 22, 22, hWnd, (HMENU)D2, NULL, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case A0:
            GetWindowText(TextBoxa0, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                a0 = atoi(buffer);
            break;
        case A1:
            GetWindowText(TextBoxa1, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                a1 = atoi(buffer);
            break;
        case B0:
            GetWindowText(TextBoxb0, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                b0 = atoi(buffer);
            break;
        case B1:
            GetWindowText(TextBoxb1, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                b1 = atoi(buffer);
            break;
        case B2:
            GetWindowText(TextBoxb2, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                b2 = atoi(buffer);
            break;
        case C0:
            GetWindowText(TextBoxc0, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                c0 = atoi(buffer);
            break;
        case C1:
            GetWindowText(TextBoxc1, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                c1 = atoi(buffer);
            break;
        case C2:
            GetWindowText(TextBoxc2, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                c2 = atoi(buffer);
            break;
        case D0:
            GetWindowText(TextBoxd0, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
              d0 = atoi(buffer);
            break;
        case D1:
            GetWindowText(TextBoxd1, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                d1 = atoi(buffer);
            break;
        case D2:
            GetWindowText(TextBoxd2, &buffer[0], 256);
            if (checkNumber(buffer, hWnd))
                d2 = atoi(buffer);
            break;
        }
       break;
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        MyOnPaint(hdc);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

bool checkNumber(char text[], HWND hWnd)
{
    for (int i = 0; i < 255; ++i) {
        if (((text[i] < 48 or text[i] > 57) and text[i] != '\0') or (text[i] == '\0' and i == 0)) {
            ::MessageBox(hWnd, "Podaj liczbe", szTitle, MB_OK);
            return false;
        }
        else if (text[i] == '\0') {
            ::MessageBox(hWnd, text, szTitle, MB_OK);
            return true;
        }
    }
    ::MessageBox(hWnd, "Blad", szTitle, MB_OK);
    return false;
}
