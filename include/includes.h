#pragma once
#include <windows.h>
#include <cstdio>
#include <iostream>
#include <sstream>      // std::stringstream

#include <DirectXMath.h>

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include "classes.h"

#include "hooks/hooks.h"
#include "hooks/d3d11/d3d11_impl.h"
#include "hooks/input.h"

#include "utils/globals.h"
#include "utils/settings.h"
#include "utils/utils.h"
#include "utils/Renderer.h"

#include "menu/menu.h"


typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;
