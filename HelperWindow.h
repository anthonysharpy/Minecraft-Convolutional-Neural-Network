#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <objidl.h>
#pragma comment (lib, "Gdiplus.lib")

extern float ActivationNumber;

ULONG_PTR gdiplusToken;
Gdiplus::Bitmap* b;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    InvalidateRect(hwnd, NULL, TRUE);

    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // draw output image

        Gdiplus::Graphics graphics(hdc);
        // Create an Image object.

        Gdiplus::Bitmap b(118, 73, PixelFormat24bppRGB);

        for (int y = 0; y < 73; y++)
        {
            for (int x = 0; x < 118; x++)
            {
                b.SetPixel(x, 72-y, Gdiplus::Color((BYTE)1, (int)Layer3PooledActivationMaps[0][x + (y * 118)].rgbRed,
                    (int)Layer3PooledActivationMaps[0][x + (y * 118)].rgbGreen,
                    (int)Layer3PooledActivationMaps[0][x + (y * 118)].rgbBlue));
            }
        }

        Gdiplus::Status res = graphics.DrawImage(&b, 1200, 50);

        if (res != Gdiplus::Status::Ok)
        {
            MessageBox(NULL, L"FAILED TO DRAW OUTPUT IMAGE", L"ERROR", MB_OK);
        }

        
        // All painting occurs here, between BeginPaint and EndPaint.

        TextOut(hdc, 85, 10, L"W                        S                         A                         D                       Jump                   Attack              MouseLeft           MouseRight        MouseDown        MouseUp", 207);

        for (int n = 0; n < OUTPUTLAYERSIZE; n++)
        {
            RECT r2;
            r2.left = 75 + (110*n);
            r2.right = 105 + (110*n);
            r2.top = 30;
            r2.bottom = 60;

            if(OutputNeurons[n] >= ActivationNumber) FillRect(hdc, &r2, (HBRUSH)(COLOR_BTNFACE + 1));
            else FillRect(hdc, &r2, (HBRUSH)(COLOR_DESKTOP + 1));
        }

        wstring vals = L"VAL: " +
            to_wstring(OutputNeurons[0]) + L"     " +
            to_wstring(OutputNeurons[1]) + L"     " +
            to_wstring(OutputNeurons[2]) + L"     " +
            to_wstring(OutputNeurons[3]) + L"      " +
            to_wstring(OutputNeurons[4]) + L"   " +
            to_wstring(OutputNeurons[5]) + L"    " +
            to_wstring(OutputNeurons[6]) + L"     " +
            to_wstring(OutputNeurons[7]) + L"     " +
            to_wstring(OutputNeurons[8]) + L"     " +
            to_wstring(OutputNeurons[9]) + L"     ";

        wstring biases = L"BIAS: " +
            to_wstring(OutputBiases[0]) + L"        " +
            to_wstring(OutputBiases[1]) + L"           " +
            to_wstring(OutputBiases[2]) + L"         " +
            to_wstring(OutputBiases[3]) + L"           " +
            to_wstring(OutputBiases[4]) + L"          " +
            to_wstring(OutputBiases[5]) + L"          " +
            to_wstring(OutputBiases[6]) + L"         " +
            to_wstring(OutputBiases[7]) + L"          " +
            to_wstring(OutputBiases[8]) + L"         " +
            to_wstring(OutputBiases[9]) + L"         ";

        TextOut(hdc, 15, 75, biases.c_str(), (int)biases.length());
        TextOut(hdc, 15, 90, vals.c_str(), (int)vals.length());

        wstring tweakrate = L"TWEAK RATE = " + to_wstring((float)timestweaked / (timestweaked + timesnottweaked) *100.0f) + L"%";
        TextOut(hdc, 15, 200, tweakrate.c_str(), (int)tweakrate.length());

        EndPaint(hwnd, &ps);

        return 0;
    }
    case WM_CREATE:
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    case WM_QUIT:
    {
        Gdiplus::GdiplusShutdown(gdiplusToken);
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    default:
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }
   
   return 0;
}

void CreateHelperWindow()
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Minecraft Bot Helper Window";

    WNDCLASS wc = { 0 };

    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = NULL;
    wc.lpszClassName = CLASS_NAME;
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClass(&wc))
    {
        MessageBox(NULL, L"FAILED TO REGISTER CLASS", L"ERROR", MB_OK);
        //return;
    }

    // Create the window.

    _Post_ _Notnull_ HWND hwnd = CreateWindowEx(
        NULL,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Minecraft Bot Helper Window",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        0, 720, 1800, 300,

        NULL,       // Parent window    
        NULL,       // Menu
        NULL,  // Instance handle
        NULL        // Additional application data
    );

    if (!hwnd)
    {
        MessageBox(NULL, L"FAILED TO CREATE HELPER WINDOW", L"ERROR", MB_OK);
        return;
    }

    ShowWindow(hwnd, SW_SHOW);

    // Run the message loop.

    //return 0;
}


