#include "Functions.h"
#include "framework/il2cpp-appdata.h"
#include "includes.h"
#include <iostream>
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
app::Vector2 Functions::GetAndAngleToTarget(app::Vector3 startPOS, app::Vector3 endPOS)
{
    float deltaX = endPOS.x - startPOS.x;
    float deltaY = endPOS.y - startPOS.y - 0.1f;
    float deltaZ = endPOS.z - startPOS.z;

    float dist = sqrt(
        pow((endPOS.x - startPOS.x), 2.0) +
        pow((endPOS.y - startPOS.y), 2.0) +
        pow((endPOS.z - startPOS.z), 2.0));
    if (dist < 0)
    {
        dist = dist * -1;
    }

    float xzlength = sqrt((deltaX * deltaX) + (deltaZ * deltaZ));
    float angleX = atan2(deltaY, xzlength) * (-57.2957795);
    float angleY = atan2(deltaX, deltaZ) * (57.2957795);
    app::Vector2 angle = { angleX,angleY };
    return  angle;
}