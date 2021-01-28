#include "includes.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT(__stdcall* ResizeBuffers)(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

namespace hooks {
	namespace d3d11 {

		Present oPresent;
		ResizeBuffers oResizeBuffers;
		HWND window = NULL;
		WNDPROC oWndProc;
		ID3D11Device* pDevice = NULL;
		ID3D11DeviceContext* pContext = NULL;
		ID3D11RenderTargetView* mainRenderTargetView;

		void InitImGui()
		{
			std::cout << "Init IMGUI" << std::endl;
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
			ImGui_ImplWin32_Init(window);
			ImGui_ImplDX11_Init(pDevice, pContext);
			Renderer::Initialize();
		}

		LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

			if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
				return true;

			return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
		}

		bool _init = false;
		HRESULT __stdcall hkPresent11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
		{
			if (GetAsyncKeyState(globals::uninjectKey) & 0x1) {

				hooks::clean();

				Beep(220, 100);

				FreeConsole();

				FreeLibraryAndExitThread(globals::mainModule, 0);

				return oPresent(pSwapChain, SyncInterval, Flags);
			}

			if (GetAsyncKeyState(globals::openMenuKey) & 0x1) {
				menu::isOpen ? menu::isOpen = false : menu::isOpen = true;
			}

			if (!_init)
			{
				std::cout << "START - Init hook DX11" << std::endl;

				if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
				{
					pDevice->GetImmediateContext(&pContext);
					DXGI_SWAP_CHAIN_DESC sd;
					pSwapChain->GetDesc(&sd);
					window = sd.OutputWindow;
					ID3D11Texture2D* pBackBuffer;
					pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
					pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
					pBackBuffer->Release();
					oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
					InitImGui();
					//hooks::input::init(window);
					_init = true;
					std::cout << "END - Init hook DX11" << std::endl;
				}

				else
					return oPresent(pSwapChain, SyncInterval, Flags);
			}

			if (!globals::canDraw) return oPresent(pSwapChain, SyncInterval, Flags);

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			Renderer::BeginScene();
			Renderer::DrawScene();
			Renderer::EndScene();

			menu::init();

			ImGui::Render();

			pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			return oPresent(pSwapChain, SyncInterval, Flags);
		}

		
		HRESULT hkResizeBuffers(IDXGISwapChain* pThis, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) {
			if (mainRenderTargetView) {
				pContext->OMSetRenderTargets(0, 0, 0);
				mainRenderTargetView->Release();
			}

			HRESULT hr = oResizeBuffers(pThis, BufferCount, Width, Height, NewFormat, SwapChainFlags);

			ID3D11Texture2D* pBuffer;
			pThis->GetBuffer(0, __uuidof(ID3D11Texture2D),
				(void**)&pBuffer);
			// Perform error handling here!

			pDevice->CreateRenderTargetView(pBuffer, NULL,
				&mainRenderTargetView);
			// Perform error handling here!
			pBuffer->Release();

			pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

			// Set up the viewport.
			D3D11_VIEWPORT vp;
			vp.Width = Width;
			vp.Height = Height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			pContext->RSSetViewports(1, &vp);

			globals::wRect.right = vp.Width;
			globals::wRect.left = 0;

			globals::wRect.bottom = vp.Height;
			globals::wRect.top = 0;

			return hr;
		}

		void init()
		{
			std::cout << "START - DX binds" << std::endl;
			if(kiero::bind(8, (void**)&oPresent, hkPresent11) != kiero::Status::Success) std::cout << "DX11 hkPresent11 bind failed" << std::endl;
			if(kiero::bind(13, (void**)&oResizeBuffers, hkResizeBuffers) != kiero::Status::Success) std::cout << "DX11 hkResizeBuffers bind failed" << std::endl;
			std::cout << "END - DX binds" << std::endl;
		}

		void release() {
			pDevice->Release();
		}

	}
}
