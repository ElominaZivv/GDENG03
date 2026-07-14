#include <JAZZY/Game/Game.h>
#include <Windows.h>
#include <string>

void jazzy::Game::run()
{
	// Message pump
	MSG msg{};
	while (m_isRunning)
	{
		// You can apparently get the position of the point at anytime using PeekMessage :0
		// GetMessage blocks the thread. basically, no input, program pauses
		// Since we want the game to run continuously regardless of input, PeekMessage is used
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// if msg.message = 0, then it is a quit message, otherwise, a different message
			if (msg.message == WM_QUIT)
			{
				m_isRunning = false;
				break;
			}
			// Im not too sure what translate does but its on the application side rather than on the windows side.
			TranslateMessage(&msg);	// This is a constant pointer, so it will not modify the msg. ok
			// Dispatch takes the translated message and sends it into the Window's side before it runs the... static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
			DispatchMessage(&msg);
		}
		onInternalUpdate();
	}
}
