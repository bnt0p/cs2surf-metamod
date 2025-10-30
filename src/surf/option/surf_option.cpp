#include "surf_option.h"
#include "surf/db/surf_db.h"
#include "utils/eventlisteners.h"
static_global KeyValues *pServerCfgKeyValues;

IMPLEMENT_CLASS_EVENT_LISTENER(SurfOptionService, SurfOptionServiceEventListener);

void SurfOptionService::LoadDefaultOptions()
{
	char serverCfgPath[1024];
	V_snprintf(serverCfgPath, sizeof(serverCfgPath), "%s%s", g_SMAPI->GetBaseDir(), "/cfg/cs2surf-server-config.txt");

	pServerCfgKeyValues = new KeyValues("ServerConfig");
	pServerCfgKeyValues->LoadFromFile(g_pFullFileSystem, serverCfgPath, nullptr);
}

const char *SurfOptionService::GetOptionStr(const char *optionName, const char *defaultValue)
{
	return pServerCfgKeyValues->GetString(optionName, defaultValue);
}

f64 SurfOptionService::GetOptionFloat(const char *optionName, f64 defaultValue)
{
	return pServerCfgKeyValues->GetFloat(optionName, defaultValue);
}

i64 SurfOptionService::GetOptionInt(const char *optionName, i64 defaultValue)
{
	return pServerCfgKeyValues->GetInt(optionName, defaultValue);
}

KeyValues *SurfOptionService::GetOptionKV(const char *optionName)
{
	return pServerCfgKeyValues->FindKey(optionName);
}

void SurfOptionService::InitOptions()
{
	LoadDefaultOptions();
}

void SurfOptionService::Cleanup()
{
	if (pServerCfgKeyValues)
	{
		delete pServerCfgKeyValues;
	}
}

void SurfOptionService::InitializeLocalPrefs(CUtlString text)
{
	if (this->dataState > LOCAL)
	{
		return;
	}
	if (text.IsEmpty())
	{
		text = "{\n}";
	}
	CUtlString error;
	LoadKV3FromJSON(&this->prefKV, &error, text.Get(), "");
	if (!error.IsEmpty())
	{
		META_CONPRINTF("[Surf::DB] Error fetching local preference: %s\n", error.Get());
		return;
	}
	this->dataState = LOCAL;
	// Calling this before the player is ingame will create unwanted race conditions.
	// We need to make sure the player is both authenticated and ingame.
	if (this->player->IsInGame())
	{
		CALL_FORWARD(eventListeners, OnPlayerPreferencesLoaded, this->player);
		this->currentState = this->dataState;
	}
}

void SurfOptionService::InitializeGlobalPrefs(std::string json)
{
	assert(!json.empty() && "API always sends at least an empty object");

	CUtlString error;
	LoadKV3FromJSON(&this->prefKV, &error, json.c_str(), "");

	if (!error.IsEmpty())
	{
		META_CONPRINTF("[Surf::Options] Error loading global preferences: %s\n", error.Get());
		return;
	}

	this->dataState = GLOBAL;

	META_CONPRINTF("[Surf::Options] Loaded global preferences.\n");

	// Calling this before the player is ingame will create unwanted race conditions.
	// We need to make sure the player is both authenticated and ingame.
	if (this->player->IsInGame())
	{
		CALL_FORWARD(eventListeners, OnPlayerPreferencesLoaded, this->player);
		this->currentState = this->dataState;
	}
}

void SurfOptionService::SaveLocalPrefs()
{
	if (this->player->IsFakeClient() || !this->player->IsAuthenticated())
	{
		return;
	}
	CUtlString error, output;
	SaveKV3AsJSON(&this->prefKV, &error, &output);
	if (!error.IsEmpty())
	{
		META_CONPRINTF("[Surf::DB] Error saving local preference: %s\n", error.Get());
		return;
	}
	this->player->databaseService->SavePrefs(output);
}

void SurfOptionService::OnPlayerActive()
{
	if (this->currentState <= this->dataState)
	{
		CALL_FORWARD(eventListeners, OnPlayerPreferencesLoaded, this->player);
		this->currentState = this->dataState;
	}
}
