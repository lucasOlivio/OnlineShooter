#pragma once

#include <common.h>

enum eInputType
{
	NONE = 0,
	FORWARD = 119,    // W
	BACKWARD = 115,   // S
	TURN_LEFT = 97,   // A
	TURN_RIGHT = 100, // D
	ACTION = 32,      // SPACE
	EXIT = 27         // ESC
};

class Input
{
public:
	Input();
	~Input();

	void PressKey(unsigned char key);
	void ReleaseKey(unsigned char key);

	bool IsKeyPressed(unsigned char key);

	eInputType GetLastInput();

private:
	bool m_Keys[255] = { false };
	eInputType m_LastInput;
};
