#pragma once

class CTraceUtil
{
public:
	cvar_t* CvarRegister(const char* Name, const char* Value);
	float GetUserAiming(edict_t* pEdict, int* cpId, int* cpBody, float distance);

private:
	std::map<std::string, cvar_t> m_CvarData;
};

extern CTraceUtil gTraceUtil;