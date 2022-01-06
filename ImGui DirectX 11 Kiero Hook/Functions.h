#pragma once
#include "framework/il2cpp-appdata.h"

class PlayerDataArray
{
public:
	char pad_0012[0xF]; //0x00c
	app::PlayerData* Player[40]; //0x0010
};
class Functions
{
public:
	static  app::PlayerData* GetPlayerData(int i);
	static void nopBytes(uintptr_t address, int size);
	static app::Vector2 GetAndAngleToTarget(app::Vector3 startPOS, app::Vector3 endPOS);
};