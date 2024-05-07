#include <windows.h>
#include <string>
#include <processthreadsapi.h>
#include "includes.h"


//inline bool GetKey(int key)
//{
//	realkey = key;
//	return true;
//}
//inline void ChangeKey(void* blank)
//{
//	keystatus = 1;
//	while (true)
//	{
//		for (int i = 0; i < 0x87; i++)
//		{
//			if (GetAsyncKeyState_Spoofed(i) & 0x8000)
//			{
//				aimkey = i;
//				keystatus = 0;
//				return;
//			}
//		}
//	}
//}
//
//inline bool GetKey1(int key)
//{
//	realkey1 = key;
//	return true;
//}
//inline void ChangeKey1(void* blank)
//{
//	keystatus1 = 1;
//	while (true)
//	{
//		for (int i = 0; i < 0x87; i++)
//		{
//			if (GetAsyncKeyState_Spoofed(i) & 0x8000)
//			{
//				TBkey = i;
//				keystatus1 = 0;
//				return;
//			}
//		}
//	}
//}
//
//inline bool GetKey2(int key)
//{
//	realkey1 = key;
//	return true;
//}
//inline void ChangeKey2(void* blank)
//{
//	keystatus2 = 1;
//	while (true)
//	{
//		for (int i = 0; i < 0x87; i++)
//		{
//			if (GetAsyncKeyState_Spoofed(i) & 0x8000)
//			{
//				smg::aimkey = i;
//				keystatus2 = 0;
//				return;
//			}
//		}
//	}
//}
//
//inline void ChangeKey3(void* blank)
//{
//	keystatus3 = 1;
//	while (true)
//	{
//		for (int i = 0; i < 0x87; i++)
//		{
//			if (GetAsyncKeyState_Spoofed(i) & 0x8000)
//			{
//				shotgun::aimkey = i;
//				keystatus3 = 0;
//				return;
//			}
//		}
//	}
//}
//
//inline void ChangeKey4(void* blank)
//{
//	keystatus4 = 1;
//	while (true)
//	{
//		for (int i = 0; i < 0x87; i++)
//		{
//			if (GetAsyncKeyState_Spoofed(i) & 0x8000)
//			{
//				sniper::aimkey = i;
//				keystatus4 = 0;
//				return;
//			}
//		}
//	}
//}




inline static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
	const char* const* items = (const char* const*)data;
	if (out_text)
		*out_text = items[idx];
	return true;
}
//inline void HotkeyButton(int aimkey, void* changekey, int status)
//{
//	const char* preview_value = NULL;
//	if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
//		Items_ArrayGetter(keyNames, aimkey, &preview_value);
//	std::string aimkeys;
//	if (preview_value == NULL)
//		aimkeys = (E("Select Key"));
//	else
//		aimkeys = preview_value;
//
//	if (status == 1)
//	{
//
//		aimkeys = (E("Press the Key"));
//	}
//	if (ImGui::Button(aimkeys.c_str(), ImVec2(155, 40)))
//	{
//		if (status == 0)
//		{
//			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
//		}
//	}
//}
//
//
//inline void HotkeyButtonTB(int& hotkey, void(*changekey)(void*), int status) {
//	const char* preview_value = NULL;
//	if (hotkey >= 0 && hotkey < IM_ARRAYSIZE(keyNames))
//		Items_ArrayGetter(keyNamesTB, hotkey, &preview_value);
//	std::string hotkeys;
//	if (preview_value == NULL)
//		hotkeys = "Select Key";
//	else
//		hotkeys = preview_value;
//
//	if (status == 1) {
//		hotkeys = "Press the Key";
//	}
//	if (ImGui::Button(hotkeys.c_str(), ImVec2(155, 40))) {
//		if (status == 0) {
//			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
//		}
//	}
//}

