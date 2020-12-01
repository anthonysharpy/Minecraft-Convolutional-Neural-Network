#pragma once

extern const int BotViewScreenWidth;
extern const int BotViewScreenHeight;

extern RGBQUAD *Pixels;

inline BYTE GetRedOfInputPixel(int x, int y)
{
	return Pixels[x + (y* BotViewScreenWidth)].rgbRed;
}

inline BYTE GetGreenOfInputPixel(int x, int y)
{
	return Pixels[x + (y * BotViewScreenWidth)].rgbGreen;
}

inline BYTE GetBlueOfInputPixel(int x, int y)
{
	return Pixels[x + (y * BotViewScreenWidth)].rgbBlue;
}

inline bool RedGreenAndBlueOfInputPixelEquals(int x, int y, int value)
{
	return (Pixels[x + (y * BotViewScreenWidth)].rgbBlue == value &&
		Pixels[x + (y * BotViewScreenWidth)].rgbRed == value &&
		Pixels[x + (y * BotViewScreenWidth)].rgbGreen == value);
}

inline bool RedGreenAndBlueOfInputPixelDoesNotEqual(int x, int y, int value)
{
	return !(Pixels[x + (y * BotViewScreenWidth)].rgbBlue == value &&
		Pixels[x + (y * BotViewScreenWidth)].rgbRed == value &&
		Pixels[x + (y * BotViewScreenWidth)].rgbGreen == value);
}

int GetSlotQuantity(int slot) // 0 = first slow
{
	int posx = 314 + (40 * slot);
	int posy = 6; // bottom left

	int firstnum = 0;
	int lastnum = 0;

	// ----- FIRST NUM ----- //
	
	// 1
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 0, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 9, posy + 0, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 10, 252))
	{
		//cout << "firstnum is 10" << endl;
		firstnum = 10;
	}

	// 2
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 2, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 2, posy + 4, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 6, 252) &&
		RedGreenAndBlueOfInputPixelDoesNotEqual(posx + 0, posy + 7, 252))
	{
		//cout << "firstnum is 20" << endl;
		firstnum = 20;
	}

	// 3
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 3, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 10, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 6, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 8, posy + 4, 252) &&
		RedGreenAndBlueOfInputPixelDoesNotEqual(posx + 0, posy + 4, 252))
	{
		//cout << "firstnum is 30" << endl;
		firstnum = 30;
	}

	// 4
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 7, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 2, posy + 9, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 11, 252))
	{
		//cout << "firstnum is 40" << endl;
		firstnum = 40;
	}

	// 5
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 3, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 9, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 9, posy + 2, 252))
	{
		//cout << "firstnum is 50" << endl;
		firstnum = 50;
	}

	// 6
	if (RedGreenAndBlueOfInputPixelEquals(posx + 7, posy + 13, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 3, posy + 11, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 9, posy + 5, 252))
	{
		//cout << "firstnum is 60" << endl;
		firstnum = 60;
	}
	
	// ----- SECOND NUM ----- //

	posx += 12; // bottom left
	
	// 1
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 0, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 9, posy + 0, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 2, posy + 10, 252))
	{
		//cout << "firstnum is 10" << endl;
		lastnum = 1;
	}

	// 2
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 2, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 2, posy + 4, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 6, 252) &&
		RedGreenAndBlueOfInputPixelDoesNotEqual(posx + 0, posy + 7, 252))
	{
		//cout << "firstnum is 20" << endl;
		lastnum = 2;
	}
	
	// 3
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 3, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 10, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 6, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 8, posy + 4, 252) &&
		RedGreenAndBlueOfInputPixelDoesNotEqual(posx + 0, posy + 4, 252))
	{
	//cout << "firstnum is 30" << endl;
		lastnum = 3;
	}

	// 4
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 7, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 2, posy + 9, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 11, 252))
	{
		//cout << "firstnum is 40" << endl;
		lastnum = 4;
	}

	// 5
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 3, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 9, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 9, posy + 2, 252))
	{
		//cout << "firstnum is 50" << endl;
		lastnum = 5;
	}

	// 6
	if (RedGreenAndBlueOfInputPixelEquals(posx + 7, posy + 13, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 3, posy + 11, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 9, posy + 5, 252))
	{
		//cout << "firstnum is 60" << endl;
		lastnum = 6;
	}

	// 7
	if (RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 2, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 10, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 6, posy + 6, 252))
	{
		//cout << "secondnum is 7" << endl;
		lastnum = 7;
	}

	// 8
	if (RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 4, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 0, posy + 8, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 4, posy + 6, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 2, posy + 6, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 8, posy + 8, 252))
	{
		//cout << "secondnum is 8" << endl;
		lastnum = 8;
	}

	// 9
	if (RedGreenAndBlueOfInputPixelEquals(posx + 6, posy + 2, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 3, posy + 6, 252) &&
		RedGreenAndBlueOfInputPixelEquals(posx + 9, posy + 5, 252))
	{
		//cout << "secondnum is 9" << endl;
		lastnum = 9;
	}
	
	return firstnum + lastnum;
}

string GetSlotItemType(int slot)
{
	int posx = 314 + (40 * slot);
	int posy = 21; // needs to be above the number text

	if (GetRedOfInputPixel(posx, posy) == 252 && GetGreenOfInputPixel(posx, posy) == 171 && GetBlueOfInputPixel(posx, posy) == 171) return "Raw Porkchop";

	return "UNKNOWN";
}

int HowMuchUncookedPork()
{
	int total = 0;

	for (int i = 0; i < 10; i++)
	{
		if (GetSlotItemType(i) == "Raw Porkchop") total += GetSlotQuantity(i);
	}

	return total;
}

int GetTotalItems()
{
	return GetSlotQuantity(0) +
		GetSlotQuantity(1) +
		GetSlotQuantity(2) +
		GetSlotQuantity(3) +
		GetSlotQuantity(4) +
		GetSlotQuantity(5) +
		GetSlotQuantity(6) +
		GetSlotQuantity(7) +
		GetSlotQuantity(8);
}