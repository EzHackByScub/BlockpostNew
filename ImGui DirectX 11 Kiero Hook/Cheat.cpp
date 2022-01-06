#include "Cheat.h"
#include "Functions.h"
#include "includes.h"
#include <iostream> 
#include "kiero/minhook/include/MinHook.h"
typedef int(__cdecl* send_attackspecialfunc)(app::Client* a1, app::Vector3 a2, app::Vector3 a3, float a4);
typedef void(__fastcall* hookRadarOnGui)(int a1);
send_attackspecialfunc attackspecialaddr = send_attackspecialfunc(reinterpret_cast<std::uintptr_t>(GetModuleHandle(TEXT("GameAssembly.dll"))) + 0x00157D10);
hookRadarOnGui hookRadar;
send_attackspecialfunc origsend_attackspecialfunc;
bool hooked;
int val;
void CrashPlayers()
{
    app::List_1_Player_WeaponInv_* WeaponList = (*app::GUIInv__TypeInfo)->static_fields->wlist;
    if (!Cheat::CrashPlayersactive)
    {
        if (val)
        {
            if ((WeaponList) && (WeaponList->fields._items) && (WeaponList->fields._items->vector))
            {
                app::WeaponInv* WeaponInv = WeaponList->fields._items->vector[val];
                if (!WeaponInv) return;
                app::WeaponInfo* wi = WeaponInv->fields.wi;
                wi->fields.id = 69;
            }
        }
    }
    else
    {
        if ((WeaponList) && (WeaponList->fields._items) && (WeaponList->fields._items->vector))
        {
            for (int index = 0; index < WeaponList->fields._size; index++)
            {
                app::WeaponInv* WeaponInv = WeaponList->fields._items->vector[index];
                if (!WeaponInv) continue;
                app::WeaponInfo* wi = WeaponInv->fields.wi;
                if (!wi) continue;
                if (wi->fields.id == 69)
                {
                    wi->fields.id = 107;
                    val = index;
                    break;
                }
            }
        }
    }   
}
void MaxScoup()
{
    app::List_1_Player_WeaponInv_* WeaponList = (*app::GUIInv__TypeInfo)->static_fields->wlist;
    if ((WeaponList) && (WeaponList->fields._items) && (WeaponList->fields._items->vector))
    {
        for (int index = 0; index < WeaponList->fields._size; index++)
        {
            app::WeaponInv* WeaponInv = WeaponList->fields._items->vector[index];
            if (!WeaponInv) continue;
            WeaponInv->fields.frags = 99999;
        }
    }
    Cheat::maxscoupactive = false;
}
void raycast(int a1)
{
    app::RaycastHit hit;
    app::Camera* currenctcam = (*app::Controll__TypeInfo)->static_fields->csCam;
    if (!currenctcam) return;
    for (int i = 0; i < 40; i++)
    {
        app::PlayerData* enemy = Functions::GetPlayerData(i);
        if (!enemy) continue;
        if (enemy->fields.spawnprotect)
        {
            enemy->fields.leg_limit = 46;
            continue;
        }
        app::Vector3 enemyheadloc = app::Transform_get_position(app::GameObject_get_transform(enemy->fields.goHead, nullptr), nullptr);
        app::Vector3 curcamerapos = app::Transform_get_position(app::Component_1_get_transform((app::Component_1*)currenctcam, nullptr), nullptr);
        if (app::Physics_Linecast_1(enemyheadloc, curcamerapos, &hit, nullptr))
        {
            float cmpPosx = hit.m_Point.x - curcamerapos.x;
            float cmpPosy = hit.m_Point.y - curcamerapos.y;
            float cmpPoz = hit.m_Point.z - curcamerapos.z;
            if (cmpPosx > -1 && cmpPosx < 1 && cmpPosy > -1 && cmpPosy < 1 && cmpPoz > -1 && cmpPoz < 1)
            {
                enemy->fields.leg_limit = 50;
            }
            else
            {
                if (app::Physics_Linecast_1(curcamerapos, enemyheadloc, &hit, nullptr))
                {
                    float cmpPosx = hit.m_Point.x - enemyheadloc.x;
                    float cmpPosy = hit.m_Point.y - enemyheadloc.y;
                    float cmpPoz = hit.m_Point.z - enemyheadloc.z;
                    if (cmpPosx > -1 && cmpPosx < 1 && cmpPosy > -1 && cmpPosy < 1 && cmpPoz > -1 && cmpPoz < 1)
                    {
                        enemy->fields.leg_limit = 50;
                    }
                    else
                    {
                        enemy->fields.leg_limit = 46;
                    }
                }
            }
        }
        else
        {
            enemy->fields.leg_limit = 46;
        }
    }
    return;
}
void Cheat::AimBotStart()
{
    if (!Cheat::AimBotStartactive) return;
    for (int i = 0; i < 40; i++)
    {
        app::PlayerData* enemy = Functions::GetPlayerData(i);
        if (!enemy) continue;
        if (teamcheck)
        {
            if (enemy->fields.team == (*app::Controll__TypeInfo)->static_fields->pl->fields.team) continue;
        }
        while (enemy->fields.bstate != 5)
        {
            if (enemy->fields.leg_limit != 50) break;
            if (!GetAsyncKeyState(VK_LSHIFT)) break;
            if (!enemy) break;
            app::Vector3 enemyheadloc = app::Transform_get_position(app::GameObject_get_transform(enemy->fields.goHead, nullptr), nullptr);
            app::Camera* currenctcam = (*app::Controll__TypeInfo)->static_fields->csCam;
            if (!currenctcam) break;
            app::Vector3 curcamerapos = app::Transform_get_position(app::Component_1_get_transform((app::Component_1*)currenctcam, nullptr), nullptr);

            app::Vector2 angletoTarget = Functions::GetAndAngleToTarget(curcamerapos, enemyheadloc);
            (*app::Controll__TypeInfo)->static_fields->ry = angletoTarget.x;
            (*app::Controll__TypeInfo)->static_fields->rx = angletoTarget.y;

        }
    }
}
void ESP(app::PlayerData* enemy, app::Camera* currenctcam)
{
    app::Vector3 enemyheadloc = app::Transform_get_position(app::GameObject_get_transform(enemy->fields.goHead, nullptr), nullptr);
    app::Vector3 enemyheadscrpos = app::Camera_WorldToScreenPoint(currenctcam, enemyheadloc, app::Camera_MonoOrStereoscopicEye__Enum::Mono, nullptr);
    if (enemyheadscrpos.z <= 1) return;
    enemyheadscrpos.y = Cheat::height - enemyheadscrpos.y;
    app::Vector3 enemyscrpos = app::Camera_WorldToScreenPoint(currenctcam, { enemyheadloc.x,enemyheadloc.y - 2,enemyheadloc.z }, app::Camera_MonoOrStereoscopicEye__Enum::Mono, nullptr);
    if (enemyscrpos.z <= 1) return;
    enemyscrpos.y = Cheat::height - enemyscrpos.y;
    float width = abs(enemyheadscrpos.y - enemyscrpos.y) * 0.3f;
    if (Cheat::spawnprotectcheck)
    {
        if (enemy->fields.spawnprotect)
           ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ enemyheadscrpos.x - width,enemyheadscrpos.y }, ImVec2{ enemyscrpos.x + width,enemyscrpos.y }, ImColor{ Cheat::colorWhspwp[0],Cheat::colorWhspwp[1],Cheat::colorWhspwp[2],Cheat::colorWhspwp[3] }, 0, 15, 3);
        if (!enemy->fields.spawnprotect)
        {
            if (Cheat::visiblecheck)
            {
                if (enemy->fields.leg_limit == 50)
                    ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ enemyheadscrpos.x - width,enemyheadscrpos.y }, ImVec2{ enemyscrpos.x + width,enemyscrpos.y }, ImColor{ Cheat::colorWhvisible[0],Cheat::colorWhvisible[1],Cheat::colorWhvisible[2],Cheat::colorWhvisible[3] }, 0, 15, 3);
                if (enemy->fields.leg_limit == 46)
                    ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ enemyheadscrpos.x - width,enemyheadscrpos.y }, ImVec2{ enemyscrpos.x + width,enemyscrpos.y }, ImColor{ Cheat::colorWh[0],Cheat::colorWh[1],Cheat::colorWh[2],Cheat::colorWh[3] }, 0, 15, 3);
            }
            else
            {
                ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ enemyheadscrpos.x - width,enemyheadscrpos.y }, ImVec2{ enemyscrpos.x + width,enemyscrpos.y }, ImColor{ Cheat::colorWh[0],Cheat::colorWh[1],Cheat::colorWh[2],Cheat::colorWh[3] }, 0, 15, 3);
            }
        }
    }
    else
    {
        if (Cheat::visiblecheck)
        {
            if (enemy->fields.leg_limit == 50)
                ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ enemyheadscrpos.x - width,enemyheadscrpos.y }, ImVec2{ enemyscrpos.x + width,enemyscrpos.y }, ImColor{ Cheat::colorWhvisible[0],Cheat::colorWhvisible[1],Cheat::colorWhvisible[2],Cheat::colorWhvisible[3] }, 0, 15, 3);
            if (enemy->fields.leg_limit == 46)
                ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ enemyheadscrpos.x - width,enemyheadscrpos.y }, ImVec2{ enemyscrpos.x + width,enemyscrpos.y }, ImColor{ Cheat::colorWh[0],Cheat::colorWh[1],Cheat::colorWh[2],Cheat::colorWh[3] }, 0, 15, 3);
        }
        else
        {
            ImGui::GetBackgroundDrawList()->AddRect(ImVec2{ enemyheadscrpos.x - width,enemyheadscrpos.y }, ImVec2{ enemyscrpos.x + width,enemyscrpos.y }, ImColor{ Cheat::colorWh[0],Cheat::colorWh[1],Cheat::colorWh[2],Cheat::colorWh[3] }, 0, 15, 3);
        }

    }
}
int attackspecial(app::Client* a1, app::Vector3 a2, app::Vector3 a3, float a4)
{
    app::Vector3 enemypos;
    app::PlayerData* enemy;
    for (int i = 0; i < 40; i++)
    {
        enemy = Functions::GetPlayerData(i);
        if (!enemy) continue;
        if (enemy->fields.spawnprotect) continue;
        if (enemy->fields.bstate == 5) continue;
        if (Cheat::teamcheck)
            if (enemy->fields.team == (*app::Controll__TypeInfo)->static_fields->pl->fields.team) continue;
        enemypos = enemy->fields.currPos;
        if (enemy)   return origsend_attackspecialfunc(a1, enemypos, { 0,0,0 }, a4);
    }
    if (!enemy)
        return origsend_attackspecialfunc(a1, a2, a3, a4);
}

void FreeCam()
{
    
    uintptr_t transformscam = *(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(reinterpret_cast<uintptr_t>(GetModuleHandle(TEXT("GameAssembly.dll"))) + 0xB35C3C) + 0x5C) + 0x80);
    if (!transformscam) return;
    uintptr_t  nospreedfreecamaddr = (*(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(*(uintptr_t*)(reinterpret_cast<uintptr_t>(GetModuleHandle(TEXT("GameAssembly.dll"))) + 0xB35C3C) + 0x5C) + 0x80) + 0x8) + 0x20) + 0xe8);
    int* nospreed = reinterpret_cast<int*>(nospreedfreecamaddr);
    *nospreed = 0;
    if (GetAsyncKeyState(0x58) & 1) // X key
        (*app::Controll__TypeInfo)->static_fields->freefly = !(*app::Controll__TypeInfo)->static_fields->freefly;
}
void Cheat::FunctionsStart()
{
    CrashPlayers();
    if (!hookRadar)
    {
        MH_Initialize();
        hookRadar = hookRadarOnGui(reinterpret_cast<std::uintptr_t>(GetModuleHandle(TEXT("GameAssembly.dll"))) + 0x287440);
        MH_CreateHook(hookRadar, &raycast, NULL);
        MH_EnableHook(hookRadar);
    }
    if (Cheat::FreeCamactive)
        FreeCam();
    if (attackspecialhook)
    {
        if (!hooked)
        {
            MH_CreateHook(attackspecialaddr, &attackspecial, reinterpret_cast<void**>(&origsend_attackspecialfunc));
            MH_EnableHook(attackspecialaddr);
            hooked = true;
        }
    }
    else
    {
        if (hooked)
        {
            MH_DisableHook(attackspecialaddr);
            hooked = false;
        }
    }
    if (maxscoupactive)
        MaxScoup();
    
}
void Cheat::RenderWallHack()
{

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
            ESP(enemy, currenctcam);

    }
}
