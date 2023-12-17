#pragma once

#include <common.h>

class Input
{
public:
	Input();
	~Input();

	void PressKey(unsigned char key);
	void ReleaseKey(unsigned char key);

	bool IsKeyPressed(unsigned char key);

private:
	bool m_Keys[255] = { false };
};
