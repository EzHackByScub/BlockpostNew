#include "Cheat.h"
#include "Functions.h"
#include "includes.h"

void ESP(app::PlayerData* enemy, app::Camera* currenctcam)
{
    app::Vector3 enemyscrpos = app::Camera_WorldToScreenPoint(currenctcam, enemy->fields.currPos, app::Camera_MonoOrStereoscopicEye__Enum::Mono, nullptr);
    if (enemyscrpos.z <= 1) return;
    enemyscrpos.y = Cheat::height - enemyscrpos.y;
    ImGui::GetBackgroundDrawList()->AddCircle(ImVec2{ enemyscrpos.x,enemyscrpos.y }, 10, ImColor{255,0,0,255},12,2);
}
void Cheat::Start()
{
    if (!height)
        height = app::Screen_get_height(nullptr);
    if (!width)
        width = app::Screen_get_width(nullptr);
    app::Camera* currenctcam = (*app::Controll__TypeInfo)->static_fields->csCam;
    if (!currenctcam) return;
    for (int i = 0; i < 40; i++)
    {
        app::PlayerData* enemy = Functions::GetPlayerData(i);
        if (!enemy) continue;
        if (teamcheck)
        {
            if (enemy->fields.team == (*app::Controll__TypeInfo)->static_fields->pl->fields.team) continue;
        }
        if (enemy->fields.currweapon == nullptr) continue;
        if (enemy->fields.bstate == 5) continue;
        if (wallhackactive)
            ESP(enemy,currenctcam);
        
    }
}