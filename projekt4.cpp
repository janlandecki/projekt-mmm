#include <windows.h>
#include <Gdiplus.h>
#include <fstream>
#include <tchar.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include <cstring>

using namespace Gdiplus;

#define N 4 // rząd systemu
#define h 0.002 // krok obliczeń
#define T 180// całkowity czas symulacji – przedział [0 , T]
#define NoV 14 // liczba zmiennych
#define PI 3.14159265 // liczba PI

enum { APPLY = 101, SOLVE, SIN, REC, TRI };

const int WINHEIGHT = 600;
const int WINWIDTH = 1000;
double u[(int)(1.0 * T / h) + 1]; // sygnał wejściowy
double y[(int)(1.0 * T / h) + 1]; // sygnał wyjściowy
int input = SIN;

double var[NoV] = { 1,1,1,1,1,1,1,1,1,1,1,1,50,10 };
HWND hwndvar[NoV]; // tablice do przetrzymywania zadanych zmiennych

// kolejnosc przetrzymywania zmiennych: a0, a1, b0, ... , amplituda sygnalu, czestotliwosc, czas trwania prostokata

static TCHAR szWindowClass[] = _T("DesktopApp");
static TCHAR szTitle[] = _T("Projekt MMM");
HINSTANCE hInst;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
bool checkNumber(char text[], HWND hWnd);
typedef struct { double n[N]; } Vect;
typedef struct { double n[N][N]; } Matr;

Vect operator+(const Vect& U, const Vect& V) {
    Vect W;
    for (int i = 0; i < N; i++)
        W.n[i] = U.n[i] + V.n[i];
    return W;
}

Vect operator*(const Vect& U, const double& d) {
    Vect W;
    for (int i = 0; i < N; i++)
        W.n[i] = U.n[i] * d;
    return W;
}

Vect operator*(const Matr& A, const Vect& V) {
    Vect W;
    for (int i = 0; i < N; i++) {
        W.n[i] = 0;
        for (int j = 0; j < N; j++)
            W.n[i] += A.n[i][j] * V.n[j];
    }
    return W;
}

double operator*(const Vect& U, const Vect& V) {
    double s = 0;
    for (int i = 0; i < N; i++)
        s += U.n[i] * V.n[i];
    return s;
}

void MyOnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    SolidBrush black(Color(255, 0, 0, 0));
    Pen blackPen(Color(255, 0, 0, 0), 3);
    Pen arrowPen(Color(255, 0, 0, 0), 3);
    FontFamily fontFamily(L"Arial");
    Font font(&fontFamily, 16);


    SolidBrush red(Color(255, 255, 0, 0));
    graphics.DrawString(L"a1 =", 4, &font, PointF(10.0f, 20.0f), &black);
    graphics.DrawString(L"a0 =", 4, &font, PointF(10.0f, 60.0f), &black);
    graphics.DrawString(L"b2 =", 4, &font, PointF(10.0f, 100.0f), &black);
    graphics.DrawString(L"b1 =", 4, &font, PointF(10.0f, 140.0f), &black);
    graphics.DrawString(L"b0 =", 4, &font, PointF(10.0f, 180.0f), &black);
    graphics.DrawString(L"c2 =", 4, &font, PointF(10.0f, 220.0f), &black);
    graphics.DrawString(L"c1 =", 4, &font, PointF(10.0f, 260.0f), &black);
    graphics.DrawString(L"c0 =", 4, &font, PointF(10.0f, 300.0f), &black);
    graphics.DrawString(L"d2 =", 4, &font, PointF(10.0f, 340.0f), &black);
    graphics.DrawString(L"d1 =", 4, &font, PointF(10.0f, 380.0f), &black);
    graphics.DrawString(L"d0 =", 4, &font, PointF(10.0f, 420.0f), &black);

    graphics.DrawString(L"Amplitude =", 12, &font, PointF(235.0f, 20.0f), &black);
    graphics.DrawString(L"Frequency[mHz] =", 17, &font, PointF(175.0f, 60.0f), &black);
    graphics.DrawString(L"Duration of the", 16, &font, PointF(165.0f, 100.0f), &black);
    graphics.DrawString(L"rectangular signal[s] =", 24, &font, PointF(135.0f, 130.0f), &black);

    graphics.DrawString(L"Select input signal:", 21, &font, PointF(160.0f, 250.0f), &black);
    switch (input) {
    case SIN:
        graphics.FillEllipse(&red, RectF(300.0f, 315.0f, 10.0f, 10.0f));
        break;
    case REC:
        graphics.FillEllipse(&red, RectF(300.0f, 365.0f, 10.0f, 10.0f));
        break;
    case TRI:
        graphics.FillEllipse(&red, RectF(300.0f, 415.0f, 10.0f, 10.0f));
        break;
    }

    float offsetX = 400.0f;

    graphics.DrawEllipse(&blackPen, 100.0f + offsetX, 150.0f, 20.0f, 20.0f);
    graphics.DrawString(L"U(s)", -1, &font, PointF(45.0f + offsetX, 150.0f), &black);

    graphics.DrawRectangle(&blackPen, 150.0f + offsetX, 140.0f, 100.0f, 40.0f);
    graphics.DrawString(L"G(s)", -1, &font, PointF(180.0f + offsetX, 150.0f), &black);

    graphics.DrawRectangle(&blackPen, 300.0f + offsetX, 140.0f, 100.0f, 40.0f);
    graphics.DrawString(L"Gc(s)", -1, &font, PointF(315.0f + offsetX, 150.0f), &black);

    graphics.DrawLine(&arrowPen, 120.0f + offsetX, 160.0f, 150.0f + offsetX, 160.0f);

    graphics.DrawLine(&arrowPen, 250.0f + offsetX, 160.0f, 300.0f + offsetX, 160.0f);

    graphics.DrawLine(&arrowPen, 400.0f + offsetX, 160.0f, 450.0f + offsetX, 160.0f);
    graphics.DrawString(L"Y(s)", -1, &font, PointF(460.0f + offsetX, 150.0f), &black);

    graphics.DrawLine(&arrowPen, 450.0f + offsetX, 160.0f, 450.0f + offsetX, 250.0f);
    graphics.DrawLine(&arrowPen, 450.0f + offsetX, 250.0f, 110.0f + offsetX, 250.0f);
    graphics.DrawLine(&arrowPen, 110.0f + offsetX, 250.0f, 110.0f + offsetX, 170.0f);

    graphics.DrawString(L"-", -1, &font, PointF(120.0f + offsetX, 160.0f), &black);

    // Rysowanie licznika
    graphics.DrawString(L"(a1 * s + a0)", -1, &font, PointF(200.0f + offsetX, 300.0f), &black);
    graphics.DrawString(L"(c2 * s^2 + c1 * s + c0)", -1, &font, PointF(160.0f + offsetX, 400.0f), &black);

    // Rysowanie kreski ułamkowej
    graphics.DrawLine(&blackPen, 180.0f + offsetX, 325.0f, 360.0f + offsetX, 325.0f);
    graphics.DrawLine(&blackPen, 160.0f + offsetX, 425.0f, 385.0f + offsetX, 425.0f);

    graphics.DrawString(L"G(s) = ", -1, &font, PointF(85.0f + offsetX, 315.0f), &black);
    graphics.DrawString(L"Gc(s) = ", -1, &font, PointF(80.0f + offsetX, 415.0f), &black);

    // Rysowanie mianownika
    graphics.DrawString(L"(b2 * s^2 + b1 * s + b0)", -1, &font, PointF(160.0f + offsetX, 330.0f), &black);
    graphics.DrawString(L"(d2 * s^2 + d1 * s + d0)", -1, &font, PointF(160.0f + offsetX, 430.0f), &black);
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
    PAINTSTRUCT ps;
    HDC hdc;
    int total = sizeof(u) / sizeof(u[0]); // rozmiar wektorów danych
    double a32, a22, a12, a02, b42, b32, b22, b12, b02;
    Matr A;
    Vect B, C, Ax, Bu, xi, xi_1;
    double D, Cx, Du, w;
    switch (message)
    {
    case WM_CREATE:
        for (int i = 0; i < NoV; ++i) {
            if (i == 11)
                hwndvar[i] = CreateWindow(TEXT("edit"), TEXT("1"), WS_VISIBLE | WS_CHILD | WS_BORDER, 360, 20, 50, 22, hWnd, NULL, NULL, NULL);
            else if (i == 12)
                hwndvar[i] = CreateWindow(TEXT("edit"), TEXT("50"), WS_VISIBLE | WS_CHILD | WS_BORDER, 360, 60, 50, 22, hWnd, NULL, NULL, NULL);
            else if (i == 13)
                hwndvar[i] = CreateWindow(TEXT("edit"), TEXT("10"), WS_VISIBLE | WS_CHILD | WS_BORDER, 360, 130, 50, 22, hWnd, NULL, NULL, NULL);
            else
                hwndvar[i] = CreateWindow(TEXT("edit"), TEXT("1"), WS_VISIBLE | WS_CHILD | WS_BORDER, 65, 20 + i * 40, 50, 22, hWnd, NULL, NULL, NULL);
        }

        CreateWindow(TEXT("button"), TEXT("Apply"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50, 460, 120, 60, hWnd, (HMENU)APPLY, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("Solve"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 50, 530, 120, 60, hWnd, (HMENU)SOLVE, NULL, NULL);

        CreateWindow(TEXT("button"), TEXT("Sine wave"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 300, 90, 40, hWnd, (HMENU)SIN, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("Rectangular"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 350, 90, 40, hWnd, (HMENU)REC, NULL, NULL);
        CreateWindow(TEXT("button"), TEXT("Triangular"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 200, 400, 90, 40, hWnd, (HMENU)TRI, NULL, NULL);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case SIN:
            input = SIN;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case REC:
            input = REC;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case TRI:
            input = TRI;
            InvalidateRect(hWnd, NULL, TRUE);
            break;
        case APPLY:
            for (int i = 0; i < NoV; ++i) {
                char buffer[20];
                GetWindowText(hwndvar[i], &buffer[0], 20);
                if (checkNumber(buffer, hWnd))
                    var[i] = atof(buffer);
            }
            break;
        case SOLVE:
            double a1 = var[0], a0 = var[1], b2 = var[2], b1 = var[3], b0 = var[4], c2 = var[5], c1 = var[6], c0 = var[7], d2 = var[8], d1 = var[9], d0 = var[10];
            double amp = var[11], freq = var[12] / 1000, time = var[13];
            a32 = a1 * c2;
            a22 = (a1 * c1) + (a0 * c2);
            a12 = (a1 * c0) + (a0 * c1);
            a02 = a0 * c0;

            b42 = b2 * d2;
            b32 = (b2 * d1) + (b1 * d2) + (a1 * c2);
            b22 = (b2 * d0) + (b1 * d1) + (b0 * d2) + (a1 * c1) + (a0 * c2);
            b12 = (b1 * d0) + (b0 * d1) + (a1 * c0) + (a0 * c1);
            b02 = (b0 * d0) + (a0 * c0);

            if (b2 != 0 && d2 != 0) {
                A.n[0][0] = 0; A.n[0][1] = 1; A.n[0][2] = 0; A.n[0][3] = 0;
                A.n[1][0] = 0; A.n[1][1] = 0; A.n[1][2] = 1; A.n[1][3] = 0;
                A.n[2][0] = 0; A.n[2][1] = 0; A.n[2][2] = 0; A.n[2][3] = 1;
                A.n[3][0] = -b02 / b42; A.n[3][1] = -b12 / b42; A.n[3][2] = -b22 / b42; A.n[3][3] = -b32 / b42;
                B.n[0] = 0; B.n[1] = 0; B.n[2] = 0; B.n[3] = 1 / b42;
                C.n[0] = a02; C.n[1] = a12; C.n[2] = a22; C.n[3] = a32;
                D = 0;
            }

            else {
                A.n[0][0] = 0; A.n[0][1] = 1; A.n[0][2] = 0; A.n[0][3] = 0;
                A.n[1][0] = 0; A.n[1][1] = 0; A.n[1][2] = 1; A.n[1][3] = 0;
                A.n[2][0] = (-b02 / b32);; A.n[2][1] = (-b12 / b32); A.n[2][2] = (-b22 / b32);; A.n[2][3] = 0;///albo 0
                A.n[3][0] = 0; A.n[3][1] = 0; A.n[3][2] = 0; A.n[3][3] = 1;
                B.n[0] = 0; B.n[1] = 0; B.n[2] = 1 / b32; B.n[3] = 0;
                C.n[0] = (a02 - (a32 * b02 / b32)); C.n[1] = (a12 - (a32 * b12 / b32)); C.n[2] = (a22 - (a32 * b22 / b32)); C.n[3] = 0;
                D = (a32 / b32);
            }
            w = 2.0 * PI * freq;
            for (int i = 0; i < total; i++) // obliczenie pobudzenia – sinus lub fala prostokątna
            {
                switch (input) {
                case SIN:
                    u[i] = amp * sin(w * i * h);
                    break;
                case REC:
                    if (i * h <= time)
                        u[i] = amp;
                    else
                        u[i] = 0;
                    break;
                case TRI:
                    u[i] = -4 * amp * freq * fabs(fmod(i * h + 1 / freq / 4, 1 / freq) - 1 / freq / 2) + amp;
                    break;
                }
            }

            // zerowe warunki początkowe
            xi_1.n[0] = 0; xi_1.n[1] = 0; xi_1.n[2] = 0; xi_1.n[3] = 0;

            // główna pętla obliczeń
            std::ofstream outfile("output.txt");
            for (int i = 0; i < total; i++) {
                Ax = A * xi_1; Bu = B * u[i]; Cx = C * xi_1; Du = D * u[i];
                xi = Ax + Bu; xi = xi * h; xi = xi_1 + xi; xi_1 = xi; y[i] = Cx + Du;
                outfile << i * h << " " << y[i] << std::endl; // zapisujemy wyniki do pliku
            }
            outfile.close();

            system("gnuplot -p -e \"plot 'output.txt' using 1:2 with lines title 'Output Signal'\"");
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
    bool dot = false;
    for (int i = 0; i < NoV; ++i) {
        if (((text[i] < 48 or text[i] > 57) and text[i] != '\0') or (text[i] == '\0' and i == 0)) {
            if (i == 0 and text[i] == '-')
                continue;
            if (i > 0 and text[i] == '.' and !dot) {
                dot = true;
                continue;
            }
            ::MessageBox(hWnd, "Incorrect values", szTitle, MB_OK);
            return false;
        }
        else if (text[i] == '\0') {
            return true;
        }
    }
    ::MessageBox(hWnd, "Error", szTitle, MB_OK);
    return false;
}
