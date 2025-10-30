#include "surf.h"

#include "tier0/memdbgon.h"
SurfPlayerManager g_SurfPlayerManager;

SurfPlayerManager *g_pSurfPlayerManager = &g_SurfPlayerManager;
PlayerManager *g_pPlayerManager = dynamic_cast<PlayerManager *>(&g_SurfPlayerManager);

SurfPlayer *SurfPlayerManager::ToPlayer(CPlayerPawnComponent *component)
{
	return static_cast<SurfPlayer *>(MovementPlayerManager::ToPlayer(component));
}

SurfPlayer *SurfPlayerManager::ToPlayer(CBasePlayerController *controller)
{
	return static_cast<SurfPlayer *>(MovementPlayerManager::ToPlayer(controller));
}

SurfPlayer *SurfPlayerManager::ToPlayer(CBasePlayerPawn *pawn)
{
	return static_cast<SurfPlayer *>(MovementPlayerManager::ToPlayer(pawn));
}

SurfPlayer *SurfPlayerManager::ToPlayer(CPlayerSlot slot)
{
	return static_cast<SurfPlayer *>(MovementPlayerManager::ToPlayer(slot));
}

SurfPlayer *SurfPlayerManager::ToPlayer(CEntityIndex entIndex)
{
	return static_cast<SurfPlayer *>(MovementPlayerManager::ToPlayer(entIndex));
}

SurfPlayer *SurfPlayerManager::ToPlayer(CPlayerUserId userID)
{
	return static_cast<SurfPlayer *>(MovementPlayerManager::ToPlayer(userID));
}

SurfPlayer *SurfPlayerManager::ToPlayer(u32 index)
{
	return static_cast<SurfPlayer *>(MovementPlayerManager::players[index]);
}

SurfPlayer *SurfPlayerManager::SteamIdToPlayer(u64 steamID, bool validated)
{
	return static_cast<SurfPlayer *>(PlayerManager::SteamIdToPlayer(steamID, validated));
}
