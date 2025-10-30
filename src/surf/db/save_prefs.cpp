#include "surf_db.h"
#include "surf/option/surf_option.h"

#include "vendor/sql_mm/src/public/sql_mm.h"

#include "queries/players.h"

void SurfDatabaseService::SavePrefs(CUtlString prefs)
{
	if (!SurfDatabaseService::IsReady() || !this->IsSetup())
	{
		return;
	}
	u64 steamID64 = this->player->GetSteamId64();
	std::string cleanedPrefs = SurfDatabaseService::GetDatabaseConnection()->Escape(prefs);

	Transaction txn;

	CUtlString query;
	query.Format(sql_players_set_prefs, prefs.Get(), steamID64);

	txn.queries.push_back(query.Get());

	SurfDatabaseService::GetDatabaseConnection()->ExecuteTransaction(txn, OnGenericTxnSuccess, OnGenericTxnFailure);
}
