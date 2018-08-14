#pragma once

#include <windows.h>
#include <Psapi.h>
#include <iostream>

class SignatureScan
{
public:
	SignatureScan( HMODULE hModule = GetModuleHandleW( NULL ) );
	~SignatureScan( );

	uintptr_t FindSignature( const char *pSignature );

private:
	void *m_pModule;
	size_t m_ulModuleSize;
};

