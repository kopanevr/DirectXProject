#include <iostream>

#include <Windows.h>

#include "Window.h"
#include "D3D.h"

int main(int argc, char* argv[])
{
	Window wnd = {};

	wnd.Loop();

	return 0;
}
