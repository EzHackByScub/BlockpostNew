#pragma once
class Cheat
{
public:
	static void RenderWallHack();
	static void FunctionsStart();
	static void AimBotStart();
public:
	inline static bool teamcheck;
	inline static bool wallhackactive;
	inline static bool spawnprotectcheck;
	inline static bool attackspecialhook;
	inline static bool AimBotStartactive;
	inline static bool FreeCamactive;
	inline static bool visiblecheck;
	inline static bool maxscoupactive;
	inline static bool CrashPlayersactive;
	inline static int width;
	inline static int height;
	inline static float colorWh[4] = { 1, 0, 0,1 };
	inline static float colorWhspwp[4] = { 0, 0, 1,1 };
	inline static float colorWhvisible[4] = { 0, 1, 0,1};
};