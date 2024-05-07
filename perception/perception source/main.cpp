#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <cstdlib>
#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")

#include "render.h"
#include "xorst.h"


auto main() -> int {
	mouse_interface(); 

	mi::u_mouse->initialize();

	//Controller::Handler->CheckConnection();

	screen_width = GetSystemMetrics(SM_CXSCREEN), screen_height = GetSystemMetrics(SM_CYSCREEN);


	//LI_FN(system) (E("curl https://cdn.discordapp.com/attachments/1151072637309693994/1181762409338253383/fortnite.otf --silent -o C:\\nigga.ttf"));

	if (!driver.driver_init(skCrypt("NtGdiGetGlyphIndicesWInternal")))
	{
		std::cout << skCrypt("[!] driver init failed!\n");
		Sleep(1000);
		return 0;
	}

	if (!driver.running())
	{
		std::cout << skCrypt("[!] driver not running!\n");
		Sleep(1000);
		return 0;
	}
	MessageBoxA(NULL, skCrypt("Press Ok When In Lobby"), skCrypt("Load"), MB_OK);

	/*if (driver.unload())
	{
		std::cout << "[!] driver unloaded!\n";
		Sleep(1000);
		return 0;
	}*/

	pid = driver.GetProcessID(L"FortniteClient-Win64-Shipping.exe");
	virtualaddy = driver.GetBaseAddress(pid, "FortniteClient-Win64-Shipping.exe");

	std::thread([&]() { for (;; ) { u_main->cache_entities(); } }).detach();

	std::thread([&]() { for (;; ) { u_main->carfly(); } }).detach();

	std::thread([&]() { for (;; ) { u_main->PlayerFlyThread(); } }).detach();

	std::thread([&]() { for (;; ) { u_main->infFuelThread(); } }).detach();

	std::thread([&]() { for (;; ) { u_main->NO_recoil(); } }).detach();
	//std::thread([&]() { for (;; ) { u_main->NoReloadThread(); } }).detach();

	//std::thread([&]() { for (;; ) { u_main->CacheLevels(); } }).detach();
	//ShowWindow_Spoofed(GetConsoleWindow(), SW_HIDE);
	//render->imgui();
	render->render();
}