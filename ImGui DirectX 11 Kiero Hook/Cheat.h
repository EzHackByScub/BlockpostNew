#pragma once
class Cheat
{
public:
	static void Start();
public:
	inline static bool teamcheck;
	inline static bool wallhackactive;
	inline static bool spawnprotectcheck;
	inline static bool attackspecialhook;
	inline static int width;
	inline static int height;
	inline static float colorWh[4] = { 0.780f, 0.031f, 0.756f,1.0f };
	inline static float colorWhspwp[4] = { 0.780f, 0.031f, 0.756f,1.0f };
};