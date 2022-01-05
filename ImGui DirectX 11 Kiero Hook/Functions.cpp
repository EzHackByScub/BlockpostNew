#include "Functions.h"
#include "framework/il2cpp-appdata.h"
app::PlayerData* Functions::GetPlayerData(int i)
{
	PlayerDataArray* pdataA = (PlayerDataArray*)(*app::PLH__TypeInfo)->static_fields->player;
	if (!pdataA->Player[i])
		return NULL;
	return pdataA->Player[i];
}