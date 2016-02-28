#include "SLGE.h"

#define DLL_API __declspec(dllexport) 
using namespace SLGE;


DLL_API Window_t::Keyboard_t::Keyboard_t()
{
	TextInput = new std::string();
}

DLL_API Window_t::Keyboard_t::~Keyboard_t()
{
	delete TextInput;
}

void DLL_API Window_t::Keyboard_t::StartCollectInputText(bool in_CollectInput)
{
	CollectTextInput = in_CollectInput;
}

std::string DLL_API Window_t::Keyboard_t::GetInputText(bool in_ClearData)
{
	std::string ReturnString = *TextInput;

	if (in_ClearData)
		TextInput->clear();

	return ReturnString;
}

const Window_t::Keyboard_t::Key_t DLL_API Window_t::Keyboard_t::operator[](KEY::Keys in_Key) const
{
	return Key[in_Key];
}

#undef DLL_API