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
	inline static int width;
	inline static int height;
	inline static float colorWh[4] = { 0.780f, 0.031f, 0.756f,1.0f };
	inline static float colorWhspwp[4] = { 0.780f, 0.031f, 0.756f,1.0f };
};