#pragma once

void Screenshot1DArray(RGBQUADFLOAT* map, int width, int height, string filename)
{
	BMP image;
	image.SetSize(width, height);
	image.SetBitDepth(32);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			image(x, (height - 1) - y)->Red = (ebmpBYTE)(map[x + (y*width)].rgbRed);
			image(x, (height - 1) - y)->Green = (ebmpBYTE)(map[x + (y * width)].rgbGreen);
			image(x, (height - 1) - y)->Blue = (ebmpBYTE)(map[x + (y * width)].rgbBlue);
		}
	}

	string name = "C:/Users/antho/Desktop/MinecraftBot/x64/Debug/screenshots/" + filename + ".bmp";
	image.WriteToFile(name.c_str());
}