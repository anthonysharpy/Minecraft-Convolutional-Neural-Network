#pragma once

void Screenshot1DArray(RGBQUADFLOAT* map, int width, int height, string filename, float scale)
{
	BMP image;
	image.SetSize(width, height);
	image.SetBitDepth(32);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			image(x, (height - 1) - y)->Red = (ebmpBYTE)(map[x + (y*width)].rgbRed * scale);
			image(x, (height - 1) - y)->Green = (ebmpBYTE)(map[x + (y * width)].rgbGreen * scale);
			image(x, (height - 1) - y)->Blue = (ebmpBYTE)(map[x + (y * width)].rgbBlue * scale);
		}
	}

	string name = "C:/Users/Anthony/Desktop/MinecraftBot/x64/Debug/screenshots/" + filename + ".bmp";
	image.WriteToFile(name.c_str());
}

void ScreenshotInput(string filename)
{
	BMP image;
	image.SetSize(BotViewScreenWidth, BotViewScreenHeight);
	image.SetBitDepth(32);

	for (int y = 0; y < BotViewScreenHeight; y++)
	{	
		for (int x = 0; x < BotViewScreenWidth; x++)
		{
			image(x, (BotViewScreenHeight -1)-y)->Red = (byte)(Pixels[x + (y* BotViewScreenWidth)].rgbRed);
			image(x, (BotViewScreenHeight -1) - y)->Green = (byte)(Pixels[x + (y* BotViewScreenWidth)].rgbGreen);
			image(x, (BotViewScreenHeight -1) - y)->Blue = (byte)(Pixels[x + (y* BotViewScreenWidth)].rgbBlue);
		}
	}

	string name = "C:/Users/Anthony/Desktop/MinecraftBot/x64/Debug/screenshots/" + filename + ".bmp";
	image.WriteToFile(name.c_str());
}

void ScreenshotPooledStuff()
{
	for (int f = 0; f < LAYER2NUMFILTERS; f++)
	{
		BMP image;

		image.SetSize(LAYER2WIDTH / 2, LAYER2HEIGHT / 2);
		image.SetBitDepth(32);

		for (int y = 0; y < LAYER2HEIGHT/2; y++)
		{
			for (int x = 0; x < LAYER2WIDTH/2; x++)
			{
				//image(x, ((LAYER2HEIGHT / 2) - 1) - y)->Red = (byte)(Layer2PooledActivationMaps[f][x + (y*LAYER2WIDTH / 2)].rgbRed);
			//	image(x, ((LAYER2HEIGHT / 2) - 1) - y)->Green = (byte)(Layer2PooledActivationMaps[f][x + (y*LAYER2WIDTH / 2)].rgbGreen);
				//image(x, ((LAYER2HEIGHT / 2) - 1) - y)->Blue = (byte)(Layer2PooledActivationMaps[f][x + (y*LAYER2WIDTH / 2)].rgbBlue);
			}
		}

		string name = "C:/Users/Anthony/Desktop/MinecraftBot/x64/Debug/screenshots/layer2pooled_" + to_string(f) + ".bmp";
		image.WriteToFile(name.c_str());
	}
}

/*void ScreenshotViewPixelsBlackandWhite(string filename)
{
	BMP image;
	image.SetSize(TrueWidth, TrueHeight);
	image.SetBitDepth(32);

	for (int y = 0; y < TrueHeight; y++)
	{
		for (int x = 0; x < TrueWidth; x++)
		{
			float res = (Pixels[x + (y*TrueWidth)].rgbRed *0.3f) +
				(Pixels[x + (y*TrueWidth)].rgbGreen*0.59f) +
				(Pixels[x + (y*TrueWidth)].rgbBlue *0.11f);

			image(x, (TrueHeight -1)-y)->Red = (int)res;
			image(x, (TrueHeight -1) - y)->Green = (int)res;
			image(x, (TrueHeight -1) - y)->Blue = (int)res;
		}
	}

	string name = "screenshots/" + filename + ".bmp";
	image.WriteToFile(name.c_str());
}*/