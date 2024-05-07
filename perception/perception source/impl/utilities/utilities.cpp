#pragma once 
#include "../../impl/inc.hpp"

auto utilities::c_utilities::get_process_id(
	const std::wstring_view process_name ) -> const std::uint32_t
{
	HANDLE handle = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, NULL );
	DWORD procID = NULL;

	if ( handle == INVALID_HANDLE_VALUE )
		return procID;

	PROCESSENTRY32W entry = { 0 };
	entry.dwSize = sizeof( PROCESSENTRY32W );

	if ( Process32FirstW( handle, &entry ) ) {
		if ( !_wcsicmp( process_name.data( ), entry.szExeFile ) )
		{
			procID = entry.th32ProcessID;
		}
		else while ( Process32NextW( handle, &entry ) ) {
			if ( !_wcsicmp( process_name.data( ), entry.szExeFile ) ) {
				procID = entry.th32ProcessID;
			}
		}
	}

	CloseHandle( handle );
	return procID;
}

auto utilities::c_utilities::get_gamebar_sid(
	std::uint32_t process_id ) -> const std::wstring {

	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, process_id );
	if ( hProcess == NULL ) {
		return L"";
	}

	HANDLE hToken;
	if ( !OpenProcessToken( hProcess, TOKEN_QUERY, &hToken ) ) {
		CloseHandle( hProcess );
		return L"";
	}

	DWORD tokenInfoLength = 0;
	if ( !GetTokenInformation( hToken, TokenAppContainerSid, NULL, 0, &tokenInfoLength ) && GetLastError( ) != ERROR_INSUFFICIENT_BUFFER ) {
		CloseHandle( hToken );
		CloseHandle( hProcess );
		return L"";
	}

	PTOKEN_APPCONTAINER_INFORMATION pAppContainerInfo = reinterpret_cast< PTOKEN_APPCONTAINER_INFORMATION >( new BYTE[ tokenInfoLength ] );
	if ( !GetTokenInformation( hToken, TokenAppContainerSid, pAppContainerInfo, tokenInfoLength, &tokenInfoLength ) ) {
		delete[ ] reinterpret_cast< BYTE* >( pAppContainerInfo );
		CloseHandle( hToken );
		CloseHandle( hProcess );
		return L"";
	}

	LPWSTR pSidString;
	if ( !ConvertSidToStringSidW( pAppContainerInfo->TokenAppContainer, &pSidString ) ) {
		delete[ ] reinterpret_cast< BYTE* >( pAppContainerInfo );
		CloseHandle( hToken );
		CloseHandle( hProcess );
		return L"";
	}

	std::wstring appSID( pSidString );
	LocalFree( pSidString );

	delete[ ] reinterpret_cast< BYTE* >( pAppContainerInfo );
	CloseHandle( hToken );
	CloseHandle( hProcess );
	return appSID;
}