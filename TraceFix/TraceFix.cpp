#include "precompiled.h"

CTraceFix gTraceFix;

void CTraceFix::ServerActivate()
{
	for (int WeaponID = WEAPON_P228; WeaponID <= WEAPON_P90; WeaponID++)
	{
		auto SlotInfo = g_ReGameAPI_Api->GetWeaponSlot((WeaponIdType)WeaponID);

		if (SlotInfo)
		{
			if ((SlotInfo->slot == PRIMARY_WEAPON_SLOT) || (SlotInfo->slot == PISTOL_SLOT))
			{
				if (SlotInfo->weaponName)
				{
					if (SlotInfo->weaponName[0u] != '\0')
					{
						if (this->m_tf_distance.find(WeaponID) == this->m_tf_distance.end())
						{
							std::string cvarName = "tf_";

							cvarName.append(SlotInfo->weaponName);

							this->m_tf_distance[WeaponID] = gTraceUtil.CvarRegister(cvarName.c_str(), "8192.0");
						}
					}
				}
			}
		}
	}

	gTraceUtil.ServerCommand("exec %s/tracefix.cfg", gTraceUtil.GetPath());
}

void CTraceFix::TraceLine(const float* vStart, const float* vEnd, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr)
{
	if (fNoMonsters == dont_ignore_monsters)
	{
		if (!FNullEnt(pentToSkip))
		{
			auto EntityIndex = g_engfuncs.pfnIndexOfEdict(pentToSkip);

			if (EntityIndex > 0 && EntityIndex <= gpGlobals->maxClients)
			{
				auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

				if (Player)
				{
					if (Player->IsAlive())
					{
						if (Player->m_pActiveItem)
						{
							if ((Player->m_pActiveItem->iItemSlot() == PRIMARY_WEAPON_SLOT) || (Player->m_pActiveItem->iItemSlot() == PISTOL_SLOT))
							{
								auto DistanceLimit = this->m_tf_distance[Player->m_pActiveItem->m_iId]->value;

								if (DistanceLimit > 0.0f)
								{
									auto trResult = gTraceUtil.GetUserAiming(pentToSkip, DistanceLimit);

									if (!FNullEnt(trResult.pHit))
									{
										auto TargetIndex = ENTINDEX(trResult.pHit);

										if (TargetIndex > 0 && TargetIndex <= gpGlobals->maxClients)
										{
											g_engfuncs.pfnMakeVectors(pentToSkip->v.v_angle);

											auto vEndRes = Vector
											(
												(vStart[0] + (gpGlobals->v_forward[0] * 9999.0f)),
												(vStart[1] + (gpGlobals->v_forward[1] * 9999.0f)),
												(vStart[2] + (gpGlobals->v_forward[2] * 9999.0f))
											);

											g_engfuncs.pfnTraceLine(vStart, vEndRes, fNoMonsters, pentToSkip, ptr);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
