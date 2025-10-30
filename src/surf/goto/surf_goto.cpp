#include "common.h"
#include "surf_goto.h"
#include "utils/simplecmds.h"
#include "utils/utils.h"

#include "../language/surf_language.h"
#include "../timer/surf_timer.h"

static_global const Vector NULL_VECTOR = Vector(0, 0, 0);

void SurfGotoService::Init() {}

void SurfGotoService::Reset() {}

bool SurfGotoService::GotoPlayer(const char *playerNamePart)
{
	if (!playerNamePart || !V_stricmp("", playerNamePart))
	{
		player->languageService->PrintChat(true, false, "Goto - Command Usage");
		return false;
	}

	if (this->player->timerService->GetTimerRunning())
	{
		this->player->languageService->PrintChat(true, false, "Goto - Error Message (Timer Running)");
		return false;
	}

	for (i32 i = 0; i <= MAXPLAYERS; i++)
	{
		CBasePlayerController *controller = g_pSurfPlayerManager->players[i]->GetController();
		SurfPlayer *otherPlayer = g_pSurfPlayerManager->ToPlayer(i);

		if (!controller || this->player == otherPlayer)
		{
			continue;
		}

		if (V_strstr(V_strlower((char *)otherPlayer->GetName()), V_strlower((char *)playerNamePart)))
		{
			if (otherPlayer->GetController()->GetTeam() == CS_TEAM_SPECTATOR)
			{
				this->player->languageService->PrintChat(true, false, "Goto - Error Message (Player In Spec)", otherPlayer->GetName());
				return false;
			}

			if (this->player->GetController()->GetTeam() == CS_TEAM_SPECTATOR)
			{
				this->player->GetController()->SwitchTeam(CS_TEAM_CT);
				this->player->GetController()->Respawn();
			}

			CCSPlayer_MovementServices *ms = this->player->GetMoveServices();

			if (otherPlayer->GetMoveType() == MOVETYPE_LADDER)
			{
				ms->m_vecLadderNormal(otherPlayer->GetMoveServices()->m_vecLadderNormal());
				this->player->SetMoveType(MOVETYPE_LADDER);
			}
			else
			{
				ms->m_vecLadderNormal(vec3_origin);
			}

			Vector origin;
			QAngle angles;
			otherPlayer->GetOrigin(&origin);
			otherPlayer->GetAngles(&angles);

			this->player->GetPlayerPawn()->Teleport(&origin, &angles, &NULL_VECTOR);
			this->player->languageService->PrintChat(true, false, "Goto - Teleported", otherPlayer->GetName());
			if (this->player->GetPlayerPawn()->m_Collision().m_CollisionGroup() != SURF_COLLISION_GROUP_STANDARD)
			{
				this->player->GetPlayerPawn()->m_Collision().m_CollisionGroup() = SURF_COLLISION_GROUP_STANDARD;
				this->player->GetPlayerPawn()->CollisionRulesChanged();
			}
			return true;
		}
	}

	player->languageService->PrintChat(true, false, "Error Message (Player Not Found)", playerNamePart);
	return false;
}

SCMD(surf_goto, SCFL_PLAYER)
{
	SurfPlayer *player = g_pSurfPlayerManager->ToPlayer(controller);
	const char *targetNamePart = args->ArgS();
	player->gotoService->GotoPlayer(targetNamePart);
	return MRES_SUPERCEDE;
}
