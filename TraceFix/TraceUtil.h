#pragma once

class CTraceUtil
{
public:
	cvar_t* CvarRegister(const char* Name, const char* Value);
	void ServerCommand(const char* Format, ...);
	const char* GetPath();
	float GetUserAiming(edict_t* pEdict, int* cpId, int* cpBody, float distance);

private:
	std::map<std::string, cvar_t> m_CvarData;
	std::string m_Path;
};

extern CTraceUtil gTraceUtil;