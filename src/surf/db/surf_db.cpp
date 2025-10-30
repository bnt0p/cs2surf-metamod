#include "surf_db.h"
#include "vendor/sql_mm/src/public/sql_mm.h"

using namespace Surf::Database;

Surf::Database::DatabaseType SurfDatabaseService::databaseType;
ISQLConnection *SurfDatabaseService::databaseConnection;

CUtlVector<SurfDatabaseServiceEventListener *> SurfDatabaseService::eventListeners;

bool SurfDatabaseService::RegisterEventListener(SurfDatabaseServiceEventListener *eventListener)
{
	if (eventListeners.Find(eventListener) >= 0)
	{
		return false;
	}
	eventListeners.AddToTail(eventListener);
	return true;
}

bool SurfDatabaseService::UnregisterEventListener(SurfDatabaseServiceEventListener *eventListener)
{
	return eventListeners.FindAndRemove(eventListener);
}

void SurfDatabaseService::Init()
{
	SurfDatabaseService::SetupDatabase();
}

void SurfDatabaseService::Cleanup()
{
	if (databaseConnection)
	{
		databaseConnection->Destroy();
		databaseConnection = NULL;
	}
}
