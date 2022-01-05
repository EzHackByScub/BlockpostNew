#include "Functions.h"
#include "framework/il2cpp-appdata.h"
#include "includes.h"
app::PlayerData* Functions::GetPlayerData(int i)
{
	PlayerDataArray* pdataA = (PlayerDataArray*)(*app::PLH__TypeInfo)->static_fields->player;
	if (!pdataA->Player[i])
		return NULL;
	return pdataA->Player[i];
}
void Functions::nopBytes(uintptr_t address, int size)
{
    DWORD OldProtection;
    VirtualProtect((LPVOID)address, size + 10, 0x40, &OldProtection);
    for (int i = 0; i < size; i++)
    {
        BYTE* Patched = reinterpret_cast<BYTE*>(address + i);
        *Patched = 0x90;
    }
}
