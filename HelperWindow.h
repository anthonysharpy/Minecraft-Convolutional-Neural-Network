#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <objidl.h>
#pragma comment (lib, "Gdiplus.lib")

extern float ActivationNumber;

ULONG_PTR gdiplusToken;

float ClampFloat0(float n)
{
    if (n < 0) return 0;
    return n;
}

inline void EasyDrawText(int xpos, int ypos, LPCWSTR text, int strlen, Gdiplus::Graphics* graphicsobj)
{
    Gdiplus::FontFamily fontFamily(L"Times New Roman");
    Gdiplus::Font font(&fontFamily, 14, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
    Gdiplus::SolidBrush solidBrush(Gdiplus::Color(255, 0, 0, 0));
    graphicsobj->DrawString(text, strlen, &font, Gdiplus::PointF(xpos, ypos), &solidBrush);
}

float GetFilterStrengthScaleFactor()
{
    float highest = 0.0f;

    for (int i = 0; i < 9; i++)
    {
        if (Layer1Filter.OurFilter.Weights[i].redWeight > highest) 
            highest = Layer1Filter.OurFilter.Weights[i].redWeight;
        if (Layer1Filter.OurFilter.Weights[i].greenWeight > highest)
            highest = Layer1Filter.OurFilter.Weights[i].greenWeight;
        if (Layer1Filter.OurFilter.Weights[i].blueWeight > highest)
            highest = Layer1Filter.OurFilter.Weights[i].blueWeight;

        if (Layer2Filter.OurFilter.Weights[i].redWeight > highest)
            highest = Layer2Filter.OurFilter.Weights[i].redWeight;
        if (Layer2Filter.OurFilter.Weights[i].greenWeight > highest)
            highest = Layer2Filter.OurFilter.Weights[i].greenWeight;
        if (Layer2Filter.OurFilter.Weights[i].blueWeight > highest)
            highest = Layer2Filter.OurFilter.Weights[i].blueWeight;

        if (Layer3Filter.OurFilter.Weights[i].redWeight > highest)
            highest = Layer3Filter.OurFilter.Weights[i].redWeight;
        if (Layer3Filter.OurFilter.Weights[i].greenWeight > highest)
            highest = Layer3Filter.OurFilter.Weights[i].greenWeight;
        if (Layer3Filter.OurFilter.Weights[i].blueWeight > highest)
            highest = Layer3Filter.OurFilter.Weights[i].blueWeight;

        if (Layer4Filter.OurFilter.Weights[i].redWeight > highest)
            highest = Layer4Filter.OurFilter.Weights[i].redWeight;
        if (Layer4Filter.OurFilter.Weights[i].greenWeight > highest)
            highest = Layer4Filter.OurFilter.Weights[i].greenWeight;
        if (Layer4Filter.OurFilter.Weights[i].blueWeight > highest)
            highest = Layer4Filter.OurFilter.Weights[i].blueWeight;

        if (Layer5Filter.OurFilter.Weights[i].redWeight > highest)
            highest = Layer5Filter.OurFilter.Weights[i].redWeight;
        if (Layer5Filter.OurFilter.Weights[i].greenWeight > highest)
            highest = Layer5Filter.OurFilter.Weights[i].greenWeight;
        if (Layer5Filter.OurFilter.Weights[i].blueWeight > highest)
            highest = Layer5Filter.OurFilter.Weights[i].blueWeight;
    }

    return highest;
}

inline void DrawFilter(int posx, int posy, FilterRGBGroup filter, Gdiplus::Graphics* graphicsobj)
{
    float scalefactor = GetFilterStrengthScaleFactor();

    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            // Red.
            Gdiplus::SolidBrush* redbrush = new Gdiplus::SolidBrush(Gdiplus::Color(
                (BYTE)(ClampFloat0(filter.OurFilter.Weights[x + (y * 3)].redWeight / scalefactor) * 255.0f), 0, 0));

            graphicsobj->FillRectangle(redbrush,
                Gdiplus::Rect(posx + (x * 10), posy + (y * 10), 10, 10));

            float sum = GetFilterWeightsSum(filter, Red);
            EasyDrawText(posx, posy + 30, to_wstring(sum).c_str(), 4, graphicsobj);

            // Green.
            Gdiplus::SolidBrush* greenbrush = new Gdiplus::SolidBrush(Gdiplus::Color(
                0, (BYTE)(ClampFloat0(filter.OurFilter.Weights[x + (y * 3)].greenWeight / scalefactor) * 255.0f), 0));

            graphicsobj->FillRectangle(greenbrush,
                Gdiplus::Rect(posx + (x * 10), posy + (y * 10) + 50, 10, 10));

            sum = GetFilterWeightsSum(filter, Green);
            EasyDrawText(posx, posy + 30 + 50, to_wstring(sum).c_str(), 4, graphicsobj);

            // Blue.
            Gdiplus::SolidBrush* bluebrush = new Gdiplus::SolidBrush(Gdiplus::Color(
                0, 0, (BYTE)(ClampFloat0(filter.OurFilter.Weights[x + (y * 3)].blueWeight / scalefactor) * 255.0f)));

            graphicsobj->FillRectangle(bluebrush,
                Gdiplus::Rect(posx + (x * 10), posy + (y * 10) + 100, 10, 10));

            sum = GetFilterWeightsSum(filter, Blue);
            EasyDrawText(posx, posy + 30 + 100, to_wstring(sum).c_str(), 4, graphicsobj);
        }
    }
}

inline void DrawLayerOutputImage(int xpos, int ypos, int sizex, int sizey, RGBQUADFLOAT* imagearray, Gdiplus::Graphics* graphicsobj)
{
    Gdiplus::Bitmap b(sizex, sizey, PixelFormat24bppRGB);

    for (int y = 0; y < sizey; y++)
    {
        for (int x = 0; x < sizex; x++)
        {
            b.SetPixel(x, sizey - y, Gdiplus::Color((BYTE)255, (int)imagearray[x + (y * sizex)].rgbRed,
                (int)imagearray[x + (y * sizex)].rgbGreen,
                (int)imagearray[x + (y * sizex)].rgbBlue));
        }
    }

    graphicsobj->DrawImage(&b, xpos, ypos);
}

inline void DrawNeuron(int xpos, int ypos, wstring label, HDC* hdc, bool on, float value, float bias, Gdiplus::Graphics* graphicsobj)
{
    // Draw label
    EasyDrawText(xpos, ypos, label.c_str(), label.length(), graphicsobj);

    // Draw square
    RECT r;
    r.left = xpos;
    r.right = xpos+30;
    r.top = ypos+20;
    r.bottom = ypos+50;

    if (on) FillRect(*hdc, &r, (HBRUSH)(COLOR_BTNFACE + 1));
    else FillRect(*hdc, &r, (HBRUSH)(COLOR_DESKTOP + 1));
    
    // Draw value text
    wstring strn = to_wstring((int)value);
    EasyDrawText(xpos, ypos + 80, strn.c_str(), (int)strn.length(), graphicsobj);
    
    // Draw bias text
    strn = to_wstring((int)bias);
    EasyDrawText(xpos, ypos+100, strn.c_str(), (int)strn.length(), graphicsobj);
}

inline void ClearScreen(Gdiplus::Graphics* graphicsobj)
{
    Gdiplus::SolidBrush* whitebrush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255));

    graphicsobj->FillRectangle(whitebrush,
        Gdiplus::Rect(0, 0, 900, 220));
}

inline void ClearAllScreen(Gdiplus::Graphics* graphicsobj)
{
    Gdiplus::SolidBrush* whitebrush = new Gdiplus::SolidBrush(Gdiplus::Color(255, 255, 255));

    graphicsobj->FillRectangle(whitebrush,
        Gdiplus::Rect(0, 0, 900, 420));
}

extern bool imagedrawdirty;
extern bool otherdrawdirty;
extern bool textdrawdirty;
extern float drawimagestime;
extern float drawtexttime;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Gdiplus::Graphics graphics(hdc);

        ClearScreen(&graphics);

        if (otherdrawdirty)
        {
            ClearAllScreen(&graphics);

            // Draw filters
            DrawFilter(40, 220, Layer1Filter, &graphics);
            DrawFilter(120, 220, Layer2Filter, &graphics);
            DrawFilter(200, 220, Layer3Filter, &graphics);
            DrawFilter(280, 220, Layer4Filter, &graphics);
            DrawFilter(360, 220, Layer5Filter, &graphics);

            otherdrawdirty = false;
        }

        if (imagedrawdirty)
        {
            long predrawimages = GetTime();

            // Draw layer output images
            //DrawLayerOutputImage(900, 15, 479, 299, Layer1PooledOutput, &graphics);
            DrawLayerOutputImage(100 + 1300, 15, 238, 148, Layer2PooledOutput, &graphics);
            DrawLayerOutputImage(100 + 1300 + 10 + 238, 15, 118, 72, Layer3PooledOutput, &graphics);
            DrawLayerOutputImage(100 + 1300 + 10 + 238 + 10 + 118, 15, 58, 35, Layer4PooledOutput, &graphics);
            DrawLayerOutputImage(100 + 1300 + 10 + 238 + 10 + 118 + 10 + 58, 15, 28, 16, Layer5PooledOutput, &graphics);
        
            imagedrawdirty = false;

            long postdrawimages = GetTime();
            drawimagestime = (postdrawimages - predrawimages) / 1000000.0f;
        }

        if (textdrawdirty)
        {
            long predrawtext = GetTime();
            // All painting occurs here, between BeginPaint and EndPaint.

            EasyDrawText(15, 105, L"VAL: ", 5, &graphics);
            EasyDrawText(15, 125, L"BIAS: ", 6, &graphics);

            // Draw neurons

            DrawNeuron(50, 25, L"W", &hdc, OutputNeurons[0] >= ActivationNumber, OutputNeurons[0], OutputBiases[0], &graphics);
            DrawNeuron(135, 25, L"S", &hdc, OutputNeurons[1] >= ActivationNumber, OutputNeurons[1], OutputBiases[1], &graphics);
            DrawNeuron(220, 25, L"A", &hdc, OutputNeurons[2] >= ActivationNumber, OutputNeurons[2], OutputBiases[2], &graphics);
            DrawNeuron(305, 25, L"D", &hdc, OutputNeurons[3] >= ActivationNumber, OutputNeurons[3], OutputBiases[3], &graphics);
            DrawNeuron(390, 25, L"Jump", &hdc, OutputNeurons[4] >= ActivationNumber, OutputNeurons[4], OutputBiases[4], &graphics);
            DrawNeuron(475, 25, L"Attack", &hdc, OutputNeurons[5] >= ActivationNumber, OutputNeurons[5], OutputBiases[5], &graphics);
            DrawNeuron(560, 25, L"MouseLeft", &hdc, OutputNeurons[6] >= ActivationNumber, OutputNeurons[6], OutputBiases[6], &graphics);
            DrawNeuron(645, 25, L"MouseRight", &hdc, OutputNeurons[7] >= ActivationNumber, OutputNeurons[7], OutputBiases[7], &graphics);
            DrawNeuron(730, 25, L"MouseDown", &hdc, OutputNeurons[8] >= ActivationNumber, OutputNeurons[8], OutputBiases[8], &graphics);
            DrawNeuron(815, 25, L"MouseUp", &hdc, OutputNeurons[9] >= ActivationNumber, OutputNeurons[9], OutputBiases[9], &graphics);
        
            // Tweak rate output
            wstring tweakrate = L"ACTUAL TWEAK RATE = " + to_wstring((float)timestweaked / (timestweaked + timesnottweaked) * 100.0f) + L"%";
            EasyDrawText(15, 170, tweakrate.c_str(), (int)tweakrate.length(), &graphics);

            textdrawdirty = false;
            long postdrawtext = GetTime();

            drawtexttime = (postdrawtext - predrawtext) / 1000000.0f;
        }

        EndPaint(hwnd, &ps);

        return 1;
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
   
   return true;
}

HWND CreateHelperWindow()
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
        0, 620, 1900, 420,

        NULL,       // Parent window    
        NULL,       // Menu
        NULL,  // Instance handle
        NULL        // Additional application data
    );

    if (!hwnd)
    {
        MessageBox(NULL, L"FAILED TO CREATE HELPER WINDOW", L"ERROR", MB_OK);
        return NULL;
    }

    ShowWindow(hwnd, SW_SHOW);

    return hwnd;
}