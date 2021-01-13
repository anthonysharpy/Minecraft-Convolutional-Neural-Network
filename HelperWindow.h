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

        Gdiplus::Graphics graphics(hdc);
        // Create an Image object.

        // draw layer 2 image

        Gdiplus::Bitmap b3(238, 148, PixelFormat24bppRGB);

        for (int y = 0; y < 148; y++)
        {
            for (int x = 0; x < 238; x++)
            {
                b3.SetPixel(x, 148 - y, Gdiplus::Color((BYTE)1, (int)Layer2PooledOutput[x + (y * 238)].rgbRed,
                    (int)Layer2PooledOutput[x + (y * 238)].rgbGreen,
                    (int)Layer2PooledOutput[x + (y * 238)].rgbBlue));
            }
        }

        Gdiplus::Status res = graphics.DrawImage(&b3, 1300, 50);

        if (res != Gdiplus::Status::Ok)
        {
            MessageBox(NULL, L"FAILED TO DRAW OUTPUT IMAGE 2", L"ERROR", MB_OK);
        }

        // draw layer 3 image

        Gdiplus::Bitmap b(118, 73, PixelFormat24bppRGB);

        for (int y = 0; y < 73; y++)
        {
            for (int x = 0; x < 118; x++)
            {
                b.SetPixel(x, 72-y, Gdiplus::Color((BYTE)1, (int)Layer3PooledOutput[x + (y * 118)].rgbRed,
                    (int)Layer3PooledOutput[x + (y * 118)].rgbGreen,
                    (int)Layer3PooledOutput[x + (y * 118)].rgbBlue));
            }
        }

        res = graphics.DrawImage(&b, 1300 + 10 + 238, 50);

        if (res != Gdiplus::Status::Ok)
        {
            MessageBox(NULL, L"FAILED TO DRAW OUTPUT IMAGE 3", L"ERROR", MB_OK);
        }

        // draw layer 4 image

        Gdiplus::Bitmap b2(58, 35, PixelFormat24bppRGB);

        for (int y = 0; y < 35; y++)
        {
            for (int x = 0; x < 58; x++)
            {
                b2.SetPixel(x, 35 - y, Gdiplus::Color((BYTE)1, (int)Layer4PooledOutput[x + (y * 58)].rgbRed,
                    (int)Layer4PooledOutput[x + (y * 58)].rgbGreen,
                    (int)Layer4PooledOutput[x + (y * 58)].rgbBlue));
            }
        }

        res = graphics.DrawImage(&b2, 1300 + 10 + 238 + 10 + 118, 50);

        if (res != Gdiplus::Status::Ok)
        {
            MessageBox(NULL, L"FAILED TO DRAW OUTPUT IMAGE 4", L"ERROR", MB_OK);
        }
        
        // draw layer 5 image

        Gdiplus::Bitmap b4(28, 16, PixelFormat24bppRGB);

        for (int y = 0; y < 16; y++)
        {
            for (int x = 0; x < 28; x++)
            {
                b4.SetPixel(x, 16 - y, Gdiplus::Color((BYTE)1, (int)Layer5PooledOutput[x + (y * 28)].rgbRed,
                    (int)Layer5PooledOutput[x + (y * 28)].rgbGreen,
                    (int)Layer5PooledOutput[x + (y * 28)].rgbBlue));
            }
        }

        res = graphics.DrawImage(&b4, 1300 + 10 + 238 + 10 + 118 + 10 + 58, 50);

        if (res != Gdiplus::Status::Ok)
        {
            MessageBox(NULL, L"FAILED TO DRAW OUTPUT IMAGE 5", L"ERROR", MB_OK);
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

        // output layer values text 

        TextOut(hdc, 15, 90, L"VAL: ", 6);

        for (int i = 0; i < 10; i++)
        {
            wstring strn = to_wstring(OutputNeurons[i]);
            TextOut(hdc, 75 + (110*i), 90, strn.c_str(), (int)strn.length());
        }

        // output layer biases text 

        TextOut(hdc, 15, 75, L"BIAS: ", 7);

        for (int i = 0; i < 10; i++)
        {
            wstring strn = to_wstring(OutputBiases[i]);
            TextOut(hdc, 75 + (110 * i), 75, strn.c_str(), (int)strn.length());
        }

        wstring tweakrate = L"ACTUAL TWEAK RATE = " + to_wstring((float)timestweaked / (timestweaked + timesnottweaked) *100.0f) + L"%";
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
        0, 720, 1900, 300,

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


