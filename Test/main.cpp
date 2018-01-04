#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "Application.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{

//#ifdef DEBUG
	//add a Debug Console to which cout is directed whilst in DEBUG
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
		SetConsoleTitle("Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

		std::cout << "Advanced Tech - Debug Console" << std::endl;
	}
//#endif DEBUG

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pScmdline);

	Application app;

	if (FAILED(app.initWindow(hInstance, iCmdshow)))
		return 0;

	if (FAILED(app.initDevice()))
	{
		app.cleanupDevice();
		return 0;
	}

	// Main message loop
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

			if (app.update()) //update next logical tick of the App
			{
				app.render();
			}
			else
			{
				break; //if App sends back an exit single quit program
			}
		}
	}

	app.cleanupDevice();

	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	}
}