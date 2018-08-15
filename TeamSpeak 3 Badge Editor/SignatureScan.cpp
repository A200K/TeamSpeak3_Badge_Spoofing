#include "SignatureScan.h"

SignatureScan::SignatureScan( HMODULE hModule )
{
	MODULEINFO info = { 0 };
	if( GetModuleInformation( GetCurrentProcess( ), hModule, &info, sizeof( info ) ) )
	{
		this->m_pModule = info.lpBaseOfDll;
		this->m_ulModuleSize = info.SizeOfImage;
	}
}

SignatureScan::~SignatureScan( )
{
}

uintptr_t SignatureScan::FindSignature( const char *pSignature )
{
	typedef struct
	{
		BYTE m_Byte;
		bool m_IsWildcard;
	} SignatureByte;

	// Prepare signature
	bool bRelativeCall = false;
	bool bDoDeref = false;
	size_t ulDerefOffsetStart = 0;
	size_t ulDerefLength = 0;
	size_t ulAddOffset = 0;

	size_t ulSignatureStringLength = strlen( pSignature );
	size_t ulSignatureLength = 0;

	SignatureByte *pSignatureBuffer = new SignatureByte[ulSignatureStringLength]( );
	for( size_t i = 0; i < ulSignatureStringLength; i++ )
	{
		if( pSignature[i] == ' ' )
		{
			continue;
		}
		else if( pSignature[i] == '[' )
		{
			ulDerefOffsetStart = ulSignatureLength;
			bDoDeref = true;
			continue;
		}
		else if( pSignature[i] == '(' )
		{
			ulDerefOffsetStart = ulSignatureLength;
			bDoDeref = true;
			bRelativeCall = true;
			continue;
		}
		else if( pSignature[i] == ']' )
		{
			ulDerefLength = ulSignatureLength - ulDerefOffsetStart;
			continue;
		}
		else if( pSignature[i] == ')' )
		{
			ulDerefLength = ulSignatureLength - ulDerefOffsetStart;
			continue;
		}
		else if( pSignature[i] == '|' )
		{
			ulAddOffset = ulSignatureLength;
			continue;
		}
		else if( pSignature[i] == '?' )
		{
			pSignatureBuffer[ulSignatureLength].m_Byte = 0;
			pSignatureBuffer[ulSignatureLength].m_IsWildcard = true;
			++ulSignatureLength;

			// Skip next char too
			if( pSignature[i + 1] == '?' )
				++i;
		}
		else
		{
			char szCurrentByteStr[3] = { pSignature[i], pSignature[i + 1], 0 };

			pSignatureBuffer[ulSignatureLength].m_Byte = static_cast<BYTE>( strtol( szCurrentByteStr, NULL, 16 ) );
			pSignatureBuffer[ulSignatureLength].m_IsWildcard = false;
			++ulSignatureLength;

			// Skip next char
			++i;
		}
	}

	// Search for it
	BYTE *pStartAddress = reinterpret_cast<BYTE*>( this->m_pModule );
	for( size_t ulCurrentIndex = 0; ulCurrentIndex < this->m_ulModuleSize - ulSignatureLength; ++ulCurrentIndex )
	{
		bool bIsMatched = true;
		for( size_t i = 0; i < ulSignatureLength; i++ )
		{
			if( pSignatureBuffer[i].m_IsWildcard == false && pStartAddress[ulCurrentIndex + i] != pSignatureBuffer[i].m_Byte )
			{
				bIsMatched = false;
				break;
			}
		}

		if( bIsMatched )
		{
			if( bDoDeref && ulDerefLength > 0 && bRelativeCall == false )
			{
				switch( ulDerefLength )
				{
				case 8:
					return static_cast<uintptr_t>( *reinterpret_cast<DWORD64*>( &pStartAddress[ulCurrentIndex + ulDerefOffsetStart] ) );
				case 4:
					return static_cast<uintptr_t>( *reinterpret_cast<DWORD*>( &pStartAddress[ulCurrentIndex + ulDerefOffsetStart] ) );
				case 2:
					return static_cast<uintptr_t>( *reinterpret_cast<WORD*>( &pStartAddress[ulCurrentIndex + ulDerefOffsetStart] ) );
				case 1:
					return static_cast<uintptr_t>( *reinterpret_cast<BYTE*>( &pStartAddress[ulCurrentIndex + ulDerefOffsetStart] ) );
				}
			}
			else if( bDoDeref && ulDerefLength > 0 && bRelativeCall )
			{
				size_t ulInstructionLength = sizeof( uintptr_t ) == 8 ? 7 : 5;

				switch( ulDerefLength )
				{
				case 8:
					return static_cast<uintptr_t>( reinterpret_cast<uintptr_t>( &pStartAddress[ulCurrentIndex] ) + ulInstructionLength + *reinterpret_cast<DWORD64*>( &pStartAddress[ulCurrentIndex + ulDerefOffsetStart] ) );
				case 4:
					return static_cast<uintptr_t>( reinterpret_cast<uintptr_t>( &pStartAddress[ulCurrentIndex] ) + ulInstructionLength + *reinterpret_cast<DWORD*>( &pStartAddress[ulCurrentIndex + ulDerefOffsetStart] ) );
				case 2:
					return static_cast<uintptr_t>( reinterpret_cast<uintptr_t>( &pStartAddress[ulCurrentIndex] ) + ulInstructionLength + *reinterpret_cast<WORD*>( &pStartAddress[ulCurrentIndex + ulDerefOffsetStart] ) );
				case 1:
					return static_cast<uintptr_t>( reinterpret_cast<uintptr_t>( &pStartAddress[ulCurrentIndex] ) + ulInstructionLength + *reinterpret_cast<BYTE*>( &pStartAddress[ulCurrentIndex + ulDerefOffsetStart] ) );
				}
			}
			return reinterpret_cast<uintptr_t>( &pStartAddress[ulCurrentIndex + ulAddOffset] );
		}
	}

	char cb[1024] = { 0 };
	sprintf_s( cb, "Can't find pattern in %I64X with size %I64X\n'%s'", reinterpret_cast<uintptr_t>( this->m_pModule ), this->m_ulModuleSize, pSignature );
	MessageBoxA( 0, cb, "Signature Scan", 0 );

	return NULL;
}