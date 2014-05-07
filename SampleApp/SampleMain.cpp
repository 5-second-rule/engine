#include <windows.h>

#include "engine-renderer/RenderableObject.h"
#include "engine-renderer/RenderingEngine.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	MessageBoxA(0, "You compiled fine :D", "", MB_OK);
	return 0;

}

