#pragma once

namespace hooks {
	namespace input {
		extern void init(HWND hWindow);
		extern void remove(HWND hWindow);
		static LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}