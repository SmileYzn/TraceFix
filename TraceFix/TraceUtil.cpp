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

const char* CTraceUtil::GetPath()
{
	if (this->m_Path.empty())
	{
		std::string GameDir = gpMetaUtilFuncs->pfnGetGameInfo(&Plugin_info, GINFO_GAMEDIR);

		if (!GameDir.empty())
		{
			this->m_Path = gpMetaUtilFuncs->pfnGetPluginPath(&Plugin_info);

			if (!this->m_Path.empty())
			{
				this->m_Path.erase(0, GameDir.length() + 1U);

				std::replace(this->m_Path.begin(), this->m_Path.end(), (char)(92), (char)(47));

				this->m_Path.erase(this->m_Path.find_last_of((char)(47)), this->m_Path.length());

				while (std::count(this->m_Path.begin(), this->m_Path.end(), (char)(47)) > 1)
				{
					this->m_Path.erase(this->m_Path.find_last_of((char)(47)), this->m_Path.length());
				}
			}
		}
	}

	return this->m_Path.c_str();
}

void CTraceUtil::ServerCommand(const char* Format, ...)
{
	char Command[255] = { 0 };

	va_list	argptr;

	va_start(argptr, Format);

	vsnprintf(Command, sizeof(Command), Format, argptr);

	va_end(argptr);

	Q_strncat(Command, "\n", 1);

	g_engfuncs.pfnServerCommand(Command);
}

TraceResult CTraceUtil::GetUserAiming(edict_t* pEntity, float DistanceLimit)
{
	TraceResult Result = { };

	if (!FNullEnt(pEntity))
	{
		auto EntityIndex = g_engfuncs.pfnIndexOfEdict(pEntity);

		if (EntityIndex > 0 && EntityIndex <= gpGlobals->maxClients)
		{
			Vector v_forward;

			Vector v_src = pEntity->v.origin + pEntity->v.view_ofs;

			g_engfuncs.pfnAngleVectors(pEntity->v.v_angle, v_forward, NULL, NULL);

			Vector v_dest = v_src + v_forward * DistanceLimit;

			g_engfuncs.pfnTraceLine(v_src, v_dest, 0, pEntity, &Result);
		}
	}

	return Result;
}

//float CTraceUtil::GetUserAiming(edict_t* pEntity, int* cpId, int* cpBody, float distance)
//{
//	float Result = 0.0f;
//
//	if (!FNullEnt(pEntity))
//	{
//		auto Entityindex = ENTINDEX(pEntity);
//
//		if (Entityindex > 0 && Entityindex <= gpGlobals->maxClients)
//		{
//			Vector v_forward = Vector(0.0f, 0.0f, 0.0f);
//
//			Vector v_src = pEntity->v.origin + pEntity->v.view_ofs;
//
//			g_engfuncs.pfnAngleVectors(pEntity->v.v_angle, v_forward, NULL, NULL);
//
//			TraceResult trEnd;
//
//			Vector v_dest = v_src + v_forward * distance;
//
//			g_engfuncs.pfnTraceLine(v_src, v_dest, 0, pEntity, &trEnd);
//
//			*cpId = FNullEnt(trEnd.pHit) ? 0 : ENTINDEX(trEnd.pHit);
//
//			*cpBody = trEnd.iHitgroup;
//
//			if (trEnd.flFraction < 1.0f)
//			{
//				Result = (trEnd.vecEndPos - v_src).Length();
//			}
//
//			return Result;
//		}
//	}
//
//	*cpId = 0;
//
//	*cpBody = 0;
//
//	return Result;
//}
