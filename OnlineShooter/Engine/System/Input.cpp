#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::PressKey(unsigned char key)
{
	m_LastInput = (eInputType)key;
	m_Keys[key] = true;
}

void Input::ReleaseKey(unsigned char key)
{
	m_LastInput = eInputType::NONE;
	m_Keys[key] = false;
}

bool Input::IsKeyPressed(unsigned char key)
{
	return m_Keys[key];
}

eInputType Input::GetLastInput()
{
	return m_LastInput;
}
