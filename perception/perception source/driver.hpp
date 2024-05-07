#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <iostream>

#include <string>
#include <vector>
#include <xlocbuf>
#include <codecvt>


uintptr_t virtualaddy;
DWORD pid;
void* funcaddy;

enum request_code : int {
	Read,
	Write,
	base,
	pattern_scan,
	running,
	unload,
	remove_apc,
	restore_apc
};

typedef struct kernel_request {
	ULONG64 process_id;
	ULONG64 source_process_id;
	void* thread;

	PVOID image_base;
	LPCSTR pattern;
	LPCSTR mask;
	bool should_rva;

	PVOID target_address;
	PVOID source_address;
	ULONG64 size;

	const char* module_name;

	void* output;

	request_code code;

} kernel_request, * pkernel_request;

class Driver {
public:

	void* function_address = nullptr;
	__int64(__fastcall* function) (__int64, __int64);

	bool driver_init(const char* comm_function) {
		LoadLibraryA("user32.dll");
		LoadLibraryA("win32u.dll");

		function_address = GetProcAddress(GetModuleHandleA("win32u.dll"), comm_function); // NtGdiAddFontResourceW //NtUserReportInertia
		funcaddy = function_address;
		if (!function_address) {
			return false;
		}

		*(PVOID*)&function = function_address;
		return true;
	}

	DWORD GetProcessID(LPCWSTR process_name) {
		HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		DWORD process_id = NULL;

		if (handle == INVALID_HANDLE_VALUE)
			return process_id;

		PROCESSENTRY32W entry = { 0 };
		entry.dwSize = sizeof(PROCESSENTRY32W);

		if (Process32FirstW(handle, &entry)) {
			if (!_wcsicmp(process_name, entry.szExeFile)) {
				process_id = entry.th32ProcessID;
			}
			else
				while (Process32NextW(handle, &entry)) {
					if (!_wcsicmp(process_name, entry.szExeFile)) {
						process_id = entry.th32ProcessID;
					}
				}
		}
		CloseHandle(handle);
		return process_id;
	}

	ULONG64 GetBaseAddress(DWORD pid, LPCSTR module_name) {
		if (module_name && pid) {
			kernel_request request = { };

			request.process_id = pid;
			request.module_name = module_name;
			request.code = request_code::base;

			function((__int64)&request, 0);
			return reinterpret_cast<ULONG64> (request.output);
		}
		return 0;
	}

	template< typename t >
	bool Write(DWORD pid, ULONG64 address, t value) {
		kernel_request request;

		request.code = request_code::Write;
		request.process_id = pid;
		request.source_process_id = GetCurrentProcessId();
		request.source_address = (PVOID)&value;
		request.target_address = (PVOID)address;
		request.size = sizeof(value);

		function((__int64)&request, 0);

		return true;
	}

	template< typename t >
	t Read(DWORD pid, uintptr_t address) {
		if (address) {
			kernel_request request;

			t buffer = { };

			request.code = request_code::Read;
			request.process_id = pid;
			request.source_process_id = GetCurrentProcessId();
			request.source_address = (PVOID)address;
			request.target_address = (PVOID)&buffer;
			request.size = sizeof(t);

			function((__int64)&request, 0);

			return buffer;
		}
		return t();
	}

	bool Read(DWORD pid, PVOID Read_address, PVOID target_address, DWORD size) {
		kernel_request request;

		request.code = request_code::Read;
		request.process_id = pid;
		request.source_process_id = GetCurrentProcessId();
		request.source_address = Read_address;
		request.target_address = target_address;
		request.size = size;

		function((__int64)&request, 0);

		return true;
	}

	template <typename t>
	bool Read_Array(DWORD pid, uintptr_t Read_address, t target_address, size_t size) {
		kernel_request request;

		request.code = request_code::Read;
		request.process_id = pid;
		request.source_process_id = GetCurrentProcessId();
		request.source_address = (uintptr_t*)Read_address;
		request.target_address = target_address;
		request.size = size;

		function((__int64)&request, 0);

		return true;
	}

	bool WriteBit(DWORD pid, ULONG64 address, bool value) {
		if (address) {
			uintptr_t originalValue;
			if (Read<uintptr_t>(pid, address)) {
				// Set the 6th bit to 1 if value is true, otherwise set it to 0
				if (value) {
					originalValue |= (1ULL << 6);
				}
				else {
					originalValue &= ~(1ULL << 6);
				}

				return Write<uintptr_t>(pid, address, originalValue);
			}
		}

		return false;
	}


	bool running() {
		kernel_request request;

		request.code = request_code::running;
		function((__int64)&request, 0);

		if ((int)request.output == 0x200) {
			return true;
		}

		return false;
	}

	bool remove_apc() // GetCurrentThread();
	{
		kernel_request request;

		request.code = request_code::remove_apc;
		function((__int64)&request, 0);

		if ((int)request.output == 0x100) {
			return true;
		}

		return false;
	}

	bool restore_apc() {
		kernel_request request;

		request.code = request_code::restore_apc;
		function((__int64)&request, 0);

		if ((int)request.output == 0x100) {
			return true;
		}

		return false;
	}

	bool unload() {
		kernel_request request;

		request.code = request_code::unload;
		function((__int64)&request, 0);

		if ((int)request.output == 0x200) {
			return true;
		}

		return false;
	}

	template< typename t >
	t Read(uintptr_t address, std::vector< uint64_t > chain) {
		uintptr_t current = address;

		for (int i = 0; i < chain.size() - 1; i++) {
			current = Read< uintptr_t >(current + chain[i]);
		}

		return Read< t >(current + chain[chain.size() - 1]);
	}

	std::string get_unicode_str(DWORD pid, uintptr_t address, size_t size) {
		address = address + 0x14;
		char16_t wcharTemp[64] = { '\0' };
		Read(pid, (PVOID)address, (PVOID)wcharTemp, size * 2);
		char16_t* var = (char16_t*)wcharTemp;
		std::string u8_conv = std::wstring_convert< std::codecvt_utf8_utf16< char16_t >, char16_t >{ }.to_bytes(var);
		return u8_conv;
	}

	std::string GetUnicodeString(DWORD pid, uint64_t address, uint64_t string_length) {
		char16_t wcharTemp[64] = { };

		Read(pid, (PVOID)address, (PVOID)&wcharTemp, string_length);
		char16_t* var = (char16_t*)wcharTemp;
		std::string u8_conv = std::wstring_convert< std::codecvt_utf8_utf16< char16_t >, char16_t >{ }.to_bytes(var);

		return u8_conv;
	}

	std::string GetASCIIString(DWORD pid, uint64_t address) {
		constexpr size_t bufferSize = 256; // Adjust the buffer size as needed
		char asciiTemp[bufferSize] = { };

		size_t bytesRead = 0;
		while (bytesRead < bufferSize - 1) // Leave space for null-terminator
		{
			char currentChar = driver.Read< char >(pid, address + bytesRead);
			if (currentChar == '\0')
				break;

			asciiTemp[bytesRead] = currentChar;
			bytesRead++;
		}

		asciiTemp[bytesRead] = '\0'; // Add explicit null-terminator

		return std::string(asciiTemp);
	}

	bool is_valid(const uint64_t adress)
	{
		if (adress <= 0x400000 || adress == 0xCCCCCCCCCCCCCCCC || reinterpret_cast<void*>(adress) == nullptr || adress >
			0x7FFFFFFFFFFFFFFF) {

			return false;
		}

		return true;
	}



} inline driver;