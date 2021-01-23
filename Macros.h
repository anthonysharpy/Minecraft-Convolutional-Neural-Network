#pragma once

void Suicide()
{
	PressLetterKeys("t/kill");
	PressSpecialKey("enter");
	SetCursorPos(1428, 345);
	Sleep(1800);
	LeftClick();
	Sleep(1000);
}


void ClearInventory()
{
	PressLetterKeys("t/c");
	PressSpecialKey("tab");
	PressSpecialKey("enter");
}

void EnchantSharpness5()
{
	PressLetterKeys("t/en");
	PressSpecialKey("tab");
	PressLetterKeys(" sq"); 
	PressSpecialKey("tab");
	PressLetterKeys(" sh");
	PressSpecialKey("tab");
	PressLetterKeys(" 5");
	PressSpecialKey("enter");
}

void EnchantMending()
{
	PressLetterKeys("t/en");
	PressSpecialKey("tab");
	PressLetterKeys(" sq");
	PressSpecialKey("tab");
	PressLetterKeys(" me");
	PressSpecialKey("tab");
	PressSpecialKey("enter");
}

void GiveDiamondSword()
{
	PressLetterKeys("t/gi");
	PressSpecialKey("tab");
	PressLetterKeys(" sq");
	PressSpecialKey("tab");
	PressLetterKeys(" swo");
	PressSpecialKey("tab");
	PressSpecialKey("enter");
}