#include "includes.h"
#include "kiero/minhook/include/MinHook.h"

namespace hooks {

	void init()
	{
		std::cout << "START - Hooks init" << std::endl;

		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success) {
			hooks::d3d11::init();
			hooks::screenshotCleaner::init();
		}

		std::cout << "END - Hooks init" << std::endl;

	}

	void clean() {
		d3d11::release();
		kiero::shutdown();

		if (MH_Uninitialize() != MH_OK) {
			std::cout << "ERROR - MH_Uninitialize" << std::endl;
		}
	}

	namespace screenshotCleaner {

		typedef BOOL(WINAPI* BITBLT)(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
		BITBLT oBitBlt = nullptr;

		bool hkBitBlt(const HDC hdc, const int x, const int y, const int cx, const int cy, const HDC hdcSrc, const int x1, const int y1, const DWORD rop)
		{
			globals::canDraw = false;
			Sleep(5); // just in case...
			auto result = oBitBlt(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
			globals::canDraw = true;
			std::cout << "Unbelievable, screenshot taken by FairFight." << std::endl;
			return result;
		}

		void init() {
			MH_CreateHook(&BitBlt, &hkBitBlt, reinterpret_cast<LPVOID*>(&oBitBlt));
			MH_EnableHook(&BitBlt);
		}
	}
}