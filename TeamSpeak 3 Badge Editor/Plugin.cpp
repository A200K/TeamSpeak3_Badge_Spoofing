#include "plugin.h"
#include "Main.h"

struct TS3Functions ts3Functions;
static char* pluginID = NULL;

const char* ts3plugin_name( ) {
	return "Badge Editor";
}

const char* ts3plugin_version( ) {
	return "1.0";
}

int ts3plugin_apiVersion( ) {
	return PLUGIN_API_VERSION;
}

const char* ts3plugin_author( ) {
	return "A200K";
}

const char* ts3plugin_description( ) {
	return "Spoofs badges.";
}

const char* ts3plugin_infoTitle( ) {
	return ts3plugin_name();
}

void ts3plugin_setFunctionPointers( const struct TS3Functions funcs ) {
	ts3Functions = funcs;
}

int ts3plugin_init( ) {
	return OnInit( );
}

void ts3plugin_shutdown( ) {
	if( pluginID ) {
		free( pluginID );
		pluginID = NULL;
	}
}

void ts3plugin_onConnectStatusChangeEvent( uint64 serverConnectionHandlerID, int newStatus, unsigned int errorNumber ) {
	if( newStatus == STATUS_CONNECTION_ESTABLISHED )
	{
		OnConnectedToServer( );
	}
}