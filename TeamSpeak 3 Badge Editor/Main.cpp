#include "SignatureScan.h"
#include "Plugin.h"

extern struct TS3Functions ts3Functions;

SignatureScan mainModuleSigScan;

uintptr_t ulSendCommand = 0;
uintptr_t ulGetTeamSpeakPacketHandler = 0;
uintptr_t ulPrepareCommandStructForLength = 0;
uintptr_t ulGetCurrentConnectionHandlerId = 0;

class CommandStruct
{
public:

	class CommandData
	{
	public:
		char* m_pCommandString; //0x0000 
		char pad_0x0008[0x40]; //0x0008

	}; //Size=0x0048

	__int64 m_CommandLength; //0x0000 
	CommandData* m_CommandData; //0x0008 
	char pad_0x0010[0x700]; //0x0010

};

uintptr_t GetConnectionHandlerAddressById( uintptr_t ulId )
{
	typedef uintptr_t( __fastcall *GetTeamSpeakPacketHandler_t )( uintptr_t pConnectionId, uintptr_t a2, unsigned __int16 a3, uintptr_t a4 );
	GetTeamSpeakPacketHandler_t GetTeamSpeakPacketHandler = reinterpret_cast<GetTeamSpeakPacketHandler_t>( ulGetTeamSpeakPacketHandler );

	struct UnknownStruct {
		uintptr_t value1;
		uintptr_t value2;
	};

	struct UnknownStruct param1 = { 0 };
	struct UnknownStruct param2 = { 0 };

	uintptr_t ulResult = GetTeamSpeakPacketHandler( ulId, reinterpret_cast<uintptr_t>( &param1 ), ulId, reinterpret_cast<uintptr_t>( &param2 ) );

	return *reinterpret_cast<uintptr_t*>( ulResult );
}

void PrepareCommandStruct( CommandStruct *pStruct, const char *pCommand )
{
	typedef void( __fastcall *PrepareCommandStructForLength_t )( CommandStruct *pCommandStruct, size_t ulLength );
	PrepareCommandStructForLength_t PrepareCommandStructForLength = reinterpret_cast<PrepareCommandStructForLength_t>( ulPrepareCommandStructForLength );

	size_t ulCommandLength = strlen( pCommand );

	PrepareCommandStructForLength( pStruct, ulCommandLength );

	pStruct->m_CommandLength = ulCommandLength;
	memcpy( pStruct->m_CommandData->m_pCommandString, pCommand, pStruct->m_CommandLength );
}

// One could use ts3Functions.getCurrentServerConnectionHandlerID as well, but this way 
// you could inject this dll into the teamspeak process and it would still work, considering you call OnInit from DllMain
uintptr_t GetCurrentConnectionHandlerId( )
{
	typedef __int64 (__cdecl *GetCurrentConnectionHandlerId_t)( );
	GetCurrentConnectionHandlerId_t GetCurrentConnectionHandlerId = reinterpret_cast<GetCurrentConnectionHandlerId_t>( ulGetCurrentConnectionHandlerId );

	return GetCurrentConnectionHandlerId( );
}

// Since the ts3 plugin api does not provide any way to send raw commands to the server, we have to build our own function
uintptr_t SendCommandToServer( const char *pCommand )
{
	typedef __int64( __fastcall *SendCommand_t )( __int64 thisptr, CommandStruct *pCommandStruct, char *a3, BYTE *a4, __int64 a5, char a6 );
	SendCommand_t SendCommand = reinterpret_cast<SendCommand_t>( ulSendCommand );
	
	uintptr_t ulConnectionHandler = GetConnectionHandlerAddressById( GetCurrentConnectionHandlerId( ) );

	CommandStruct s = { 0 };
	PrepareCommandStruct( &s, pCommand );

	return SendCommand( ulConnectionHandler, &s, 0, 0, 0, 0 );
}

void OnConnectedToServer( )
{
	/*

	Maximum of 3 badges at the same time

	http://badges-content.teamspeak.com/list

	1cb07348-34a4-4741-b50f-c41e584370f7 - Creator of TeamSpeak Addons
	50bbdbc8-0f2a-46eb-9808-602225b49627 - Registered during Gamescom 2016
	d95f9901-c42d-4bac-8849-7164fd9e2310 - Registered during Paris Games Week 2016
	62444179-0d99-42ba-a45c-c6b1557d079a - Registered at Gamescom 2014
	d95f9901-c42d-4bac-8849-7164fd9e2310 - Registered at Paris Games Week 2014
	450f81c1-ab41-4211-a338-222fa94ed157 - Creator of at least 1 TeamSpeak Addon
	c9e97536-5a2d-4c8e-a135-af404587a472 - Creator of at least 3 TeamSpeak Addons
	94ec66de-5940-4e38-b002-970df0cf6c94 - Creator of at least 5 TeamSpeak Addons
	534c9582-ab02-4267-aec6-2d94361daa2a - Visited TeamSpeak at Gamescom 2017
	7d9fa2b1-b6fa-47ad-9838-c239a4ddd116 - MIFCOM | Entered Performance
	34dbfa8f-bd27-494c-aa08-a312fc0bb240 - Gaming Hero at Gamescom 2017
	f81ad44d-e931-47d1-a3ef-5fd160217cf8 - 4Netplayers customer
	f22c22f1-8e2d-4d99-8de9-f352dc26ac5b - Rocket Beans TV Community
	64221fd1-706c-4bb2-ba55-996c39effa79 - MyTeamSpeak early adopter
	c3f823eb-5d5c-40f9-9dbd-3437d59a539d - New MyTeamSpeak member
	935e5a2a-954a-44ca-aa7a-55c79285b601 - Discovered at E3 2018
	4eef1ecf-a0ea-423d-bfd0-496543a00305 - Visited TeamSpeak at Gamescom 2018
	24512806-f886-4440-b579-9e26e4219ef6 - Played at Gamescom 2018
	b9c7d6ad-5b99-40fb-988c-1d02ab6cc130 - Met Tim Speak

	*/

	// Creator of at least 5 TeamSpeak Addons, Registered at Gamescom 2014, Discovered at E3 2018
	SendCommandToServer( "clientupdate client_badges=Overwolf=0:badges=94ec66de-5940-4e38-b002-970df0cf6c94,62444179-0d99-42ba-a45c-c6b1557d079a,935e5a2a-954a-44ca-aa7a-55c79285b601" );
}

int OnInit( )
{
	ulSendCommand = mainModuleSigScan.FindSignature( "40 55 53 56 57 41 54 41 56 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 C7 44 24" );
	ulGetTeamSpeakPacketHandler = mainModuleSigScan.FindSignature( "4C 8B DC 49 89 53 ? 49 89 4B ? 56" );
	ulPrepareCommandStructForLength = mainModuleSigScan.FindSignature( "48 89 5C 24 ? 48 89 74 24 ? 41 56 48 83 EC ? 4C 8B 49" );
	ulGetCurrentConnectionHandlerId = mainModuleSigScan.FindSignature( "48 83 EC ? 48 8B 05 ? ? ? ? 48 8D 0D ? ? ? ? 48 8B 10" );

	bool bIsSuccessful = ulSendCommand && ulGetTeamSpeakPacketHandler && ulPrepareCommandStructForLength && ulGetCurrentConnectionHandlerId;

	return bIsSuccessful ? 0 : 1;
}

BOOL WINAPI DllMain( HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved )
{
	return TRUE;
}