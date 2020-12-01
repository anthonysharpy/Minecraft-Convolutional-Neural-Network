#pragma once

INPUT ip;
INPUT ipmouse;
INPUT shiftinput;

void KeyDown(string key)
{
	if (key == "w")
	{
		//cout << "w" << endl;
		
		ip.ki.wScan = 17; // 0x57; //'w';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	else if (key == "s")
	{
		//cout << "s" << endl;

		ip.ki.wScan = 0x1F;//'s';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	else if (key == "a")
	{
		//cout << "a" << endl;

		ip.ki.wScan = 30;//0x41;// 'a';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	else if (key == "d")
	{
		//cout << "d" << endl;

		ip.ki.wScan = 32;// 0x44; //'d';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	else if (key == "space")
	{
		//cout << "space" << endl;

		ip.ki.wScan = 57;// 0x44; //'d';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
}

void KeyUp(string key)
{
	if (key == "w")
	{
		//cout << "wup" << endl;

		ip.ki.wScan = 0;
		ip.ki.wScan = 17; // 0x57; //'w';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	else if (key == "s")
	{
		//cout << "sup" << endl;

		ip.ki.wScan = 0;
		ip.ki.wScan = 0x1F;//'s';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	else if (key == "a")
	{
		//cout << "aup" << endl;

		ip.ki.wScan = 0;
		ip.ki.wScan = 30;//0x41;// 'a';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	else if (key == "d")
	{
		//cout << "dup" << endl;

		ip.ki.wScan = 0;
		ip.ki.wScan = 32;// 0x44; //'d';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	else if (key == "space")
	{
		//cout << "spaceup" << endl;

		ip.ki.wScan = 0;
		ip.ki.wScan = 57;// 0x44; //'d';
		ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
}

bool DoesCharRequireShiftPress(char c)
{
	UINT VKCode = HIBYTE(VkKeyScanA(c));

	if (VKCode == 1)
	{
		return true;
	}
	return false;
}

int GetScanCodeForChar(char c)
{
	UINT VKCode = LOBYTE(VkKeyScanA(c));
	return MapVirtualKey(VKCode, 0);
}

void PressLetterKeys(string keys)
{
	bool first = true;

	for (int i = 0; i < keys.length(); i++)
	{
		if (DoesCharRequireShiftPress(keys[i]))
		{
			shiftinput.type = INPUT_KEYBOARD;
			shiftinput.ki.wScan = 42;
			shiftinput.ki.dwFlags = KEYEVENTF_SCANCODE;
			SendInput(1, &shiftinput, sizeof(INPUT));
		}

		if(first) Sleep(75);
		else
		{
			first = false;
			Sleep(50);
		}

		ip.ki.wScan = GetScanCodeForChar(keys[i]);
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		ip.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE; // KEYEVENTF_KEYUP for key release
		SendInput(1, &ip, sizeof(INPUT));
		shiftinput.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE; // KEYEVENTF_KEYUP for key release
		SendInput(1, &shiftinput, sizeof(INPUT));
	}
}

void PressSpecialKey(string key)
{
	if (key == "enter") // inventory
	{
		ip.ki.wScan = 28;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	if (key == "_") // inventory
	{
		ip.ki.wScan = 28;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	if (key == ":") // inventory
	{
		ip.ki.wScan = 28;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	if (key == "tab")
	{
		ip.ki.wScan = 15;
		ip.ki.dwFlags = KEYEVENTF_SCANCODE; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));
	}
	
	Sleep(50);

	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
}

void LeftClick()
{
	ipmouse.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &ipmouse, sizeof(INPUT));

	// left up
	ZeroMemory(&ipmouse, sizeof(INPUT));
	ipmouse.type = INPUT_MOUSE;
	ipmouse.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &ipmouse, sizeof(INPUT));
}

void LeftClickDown()
{
	//cout << "LeftClickDown()";
	ipmouse.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &ipmouse, sizeof(INPUT));
}

void LeftClickUp()
{
	//cout << "LeftClickUp()" << endl;

	// left up
	ZeroMemory(&ipmouse, sizeof(INPUT));
	ipmouse.type = INPUT_MOUSE;
	ipmouse.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &ipmouse, sizeof(INPUT));
}

void MoveMouseDown()
{
	ZeroMemory(&ipmouse, sizeof(INPUT));
	ipmouse.type = INPUT_MOUSE;
	ipmouse.mi.dy = 100;
	ipmouse.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &ipmouse, sizeof(INPUT));

	//cout << "MoveMouseUp()" << endl;
}

void MoveMouseUp()
{
	ZeroMemory(&ipmouse, sizeof(INPUT));
	ipmouse.type = INPUT_MOUSE;
	ipmouse.mi.dy = -100;
	ipmouse.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &ipmouse, sizeof(INPUT));

	//cout << "MoveMouseDown()" << endl;
}

void MoveMouseLeft()
{
	ZeroMemory(&ipmouse, sizeof(INPUT));
	ipmouse.type = INPUT_MOUSE;
	ipmouse.mi.dx = -100;
	ipmouse.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &ipmouse, sizeof(INPUT));

	//cout << "MoveMouseLeft()" << endl;
}

void MoveMouseRight()
{
	ZeroMemory(&ipmouse, sizeof(INPUT));
	ipmouse.type = INPUT_MOUSE;
	ipmouse.mi.dx = 100;
	ipmouse.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &ipmouse, sizeof(INPUT));

	//cout << "MoveMouseRight()" << endl;
}
void RightClick()
{
	ipmouse.mi.dwFlags = // MOUSEVENTF //MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &ipmouse, sizeof(INPUT));

	// left up
	ZeroMemory(&ipmouse, sizeof(INPUT));
	ipmouse.type = INPUT_MOUSE;
	ipmouse.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &ipmouse, sizeof(INPUT));
}

void InitialiseInput()
{
	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	//ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ipmouse.type = INPUT_MOUSE;
}

void AllKeysUp()
{
	KeyUp("space");
	KeyUp("w");
	KeyUp("s");
	KeyUp("a");
	KeyUp("d");
	LeftClickUp();
}