#pragma once
class Joypad
{
public:
	Joypad();
	~Joypad();
	char ReadJoypad();
	void WriteJoyPad(int value);
private:
	// I/O port 0xFF00
	char JOYP;
	bool ButtonsSelected = false;
	bool DirectionsSelected = false;

	// Joypad states. 0=Pressed, 1=Not Pressed
	char A;
	char B;
	char Start;
	char Select;
	char Right;
	char Left;
	char Up;
	char Down;
};

