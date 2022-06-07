#include "hooks.h"

#include <stdexcept>
#include <intrin.h>

#include "../dependencies/minhook/minhook.h"

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_win32.h"
#include "../dependencies/imgui/imgui_impl_dx9.h"

void hooks::setupHooks()
{
	if (MH_Initialize()) //for some reason minhook returns a 0 if it initalizes?????
		throw std::runtime_error("minhook error");

	if (MH_CreateHook(VirtualFunction(gui::device, 42), &EndScene, reinterpret_cast<void**>(&EndSceneOriginal)))
		throw std::runtime_error("unable to hook endscene");

	if (MH_CreateHook(VirtualFunction(gui::device, 16), &Reset, reinterpret_cast<void**>(&ResetOriginal)))
		throw std::runtime_error("unable to hook reset");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("unable to enable hooks");

	gui::destoryDirextX();
}
void hooks::destroyHooks() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}



long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	static const auto returnAdress = _ReturnAddress();

	const auto result = EndSceneOriginal(device, device);

	//stop endscene getting called twice
	if (_ReturnAddress() == returnAdress)
		return result;

	if (!gui::setup)
		gui::setupMenu(device);

	if (gui::open)
		gui::render();

	return result;
}


HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}