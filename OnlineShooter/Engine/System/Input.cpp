#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::PressKey(unsigned char key)
{
	m_Keys[key] = true;
}

void Input::ReleaseKey(unsigned char key)
{
	m_Keys[key] = false;
}

bool Input::IsKeyPressed(unsigned char key)
{
	return m_Keys[key];
}
