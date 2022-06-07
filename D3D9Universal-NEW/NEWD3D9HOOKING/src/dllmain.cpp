#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>
#include <cstdint>

#include "hooks.h"
#include <iostream>

//xemm - 6/7/2022 7:41AM 'idk why i havent slept'

//tried to make this as clean as i could but damn im tired man.

//should work for every game only tried on cs tho so who knows cba rn

//setup
void Setup(const HMODULE instance)
{
	try
	{
		//AllocConsole();
		gui::xsetup();
		hooks::setupHooks();
	} 
	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(0, error.what(), "error", MB_OK | MB_ICONEXCLAMATION);

		goto UNLOAD; //sorry for comitting this cardinal sin
	}

	while (!GetAsyncKeyState(VK_DELETE))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

UNLOAD:
	hooks::destroyHooks();
	gui::destroy();

	FreeLibraryAndExitThread(instance, 0);
}

//entry
BOOL WINAPI DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup), instance, 0, nullptr);

		if (thread)
			CloseHandle(thread);
	}

	return TRUE;
}