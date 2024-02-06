#include "precompiled.h"

CTraceUtil gTraceUtil;

cvar_t* CTraceUtil::CvarRegister(const char* Name, const char* Value)
{
	cvar_t* CvarPointer = g_engfuncs.pfnCVarGetPointer(Name);

	if (CvarPointer == nullptr)
	{
		this->m_CvarData[Name].name = Name;

		this->m_CvarData[Name].string = (char*)(Value);

		this->m_CvarData[Name].flags = (FCVAR_SERVER | FCVAR_SPONLY);

		g_engfuncs.pfnCVarRegister(&this->m_CvarData[Name]);

		CvarPointer = g_engfuncs.pfnCVarGetPointer(this->m_CvarData[Name].name);

		if (CvarPointer != nullptr)
		{
			g_engfuncs.pfnCvar_DirectSet(CvarPointer, Value);
		}
	}

	return CvarPointer;
}

float CTraceUtil::GetUserAiming(edict_t * pEdict, int* cpId, int* cpBody, float distance)
{
	float Result = 0.0f;

	if (!FNullEnt(pEdict))
	{
		auto Entityindex = ENTINDEX(pEdict);

		if (Entityindex > 0 && Entityindex <= gpGlobals->maxClients)
		{
			Vector v_forward;

			Vector v_src = pEdict->v.origin + pEdict->v.view_ofs;

			g_engfuncs.pfnAngleVectors(pEdict->v.v_angle, v_forward, NULL, NULL);

			TraceResult trEnd;

			Vector v_dest = v_src + v_forward * distance;

			g_engfuncs.pfnTraceLine(v_src, v_dest, 0, pEdict, &trEnd);

			*cpId = FNullEnt(trEnd.pHit) ? 0 : ENTINDEX(trEnd.pHit);

			*cpBody = trEnd.iHitgroup;

			if (trEnd.flFraction < 1.0f)
			{
				Result = (trEnd.vecEndPos - v_src).Length();
			}

			return Result;
		}
	}

	*cpId = 0;

	*cpBody = 0;

	return Result;
}