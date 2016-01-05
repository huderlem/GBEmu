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

	// Joypad states. 1=On, 0=Off
	char A		= 0;
	char B		= 0;
	char Start	= 0;
	char Select	= 0;
	char Right	= 0;
	char Left	= 0;
	char Up		= 0;
	char Down	= 0;
};

