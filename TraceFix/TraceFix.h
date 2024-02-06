#pragma once

class CTraceFix
{
public:
	void ServerActivate();
	void TraceLine(const float* vStart, const float* vEnd, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr);

public:
	std::map<int, cvar_t*> m_tf_distance;
};

extern CTraceFix gTraceFix;
