#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <string>
#include <random>
#include <chrono>
#include <iostream>
#include <cfloat>
#include <thread>
#include "offsets.h"
#include "memory.h"
#include "ids.h"
#include "Game.h"

Memory apex;
Memory client;

uint64_t apexBase; // apex base addr
uint64_t clientBase; // client base addr

bool v1 = false;

bool glowItems = true;
float maxDistance = 200.0f * 40.0f;

bool lookingForProcs = true; //read write - controls when cheat starts


static void aimBotThreadFunc()
{
	printf("Started Aimbot Thread");
	while (lookingForProcs ==false)
	{
		uint64_t localPlayerPtr = 0;
		apex.Read<uint64_t>(apexBase + OFFSET_LOCAL_ENT, localPlayerPtr);
		uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
		if (localPlayerPtr = 0)
		{
			Player localPlayer = ptrToPlayer(localPlayerPtr);
			Vector2D vAngles = localPlayer.getViewAngles();
			printf("angles.x: %.1f", vAngles.x);
			printf("angles.y: %.1f", vAngles.y);

		}
		for (int i = 0; i < 10000; i++)
		{
			uint64_t entPtr = 0;
			apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), entPtr);

			Entity ent = ptrToEntity(entPtr);
			if (ent.isDummy())
			{

			}

			Vector entPos = ent.getPosition();
			//Vector localPlayerPos = localPlayer.getPosition();
			//float distance = localPlayer.getPosition().DistTo(entPos);
			if (true)
			{		
				Vector diference;
				//diference.x = entPos.x - localPlayerPos.x;
				//diference.y = entPos.y - localPlayerPos.y;
				//diference.z = entPos.z - localPlayerPos.z;
				//float c = std::sqrt((diference.x * diference.x) + (diference.z * diference.z));
				//float yaw = (atan2(diference.z, diference.x) * 180 / ((float)3.14159265358979323846)) - 90.0f;
				//float pitch = (atan2(diference.y, c) * 180 / ((float)3.14159265358979323846));
				//vec2 vAngles = localPlayer.getViewAngles();
				//vAngles.x = vAngles.x + 1.0f;
				//.y = vAngles.x + 1.0f;
				//localPlayer.setViewAngles(vAngles);
			}
		}
				
	}
}

static void playerGlowThread()
{
	printf("Started Player Glow Thread\n");
	while (lookingForProcs == false&&v1==true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		for (int i = 0; i < 100; i++)
		{
			uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
			uint64_t ent = 0;
			apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), ent);


			uint64_t localPlayer = 0;
			apex.Read<uint64_t>(apexBase + OFFSET_LOCAL_ENT, localPlayer);
			if (localPlayer == 0) continue;

			if (ent == localPlayer)
			{
				continue;
			}

			int entTeam;
			apex.Read<int>(ent + OFFSET_TEAM, entTeam);
			if (entTeam > 0 && entTeam < 60)
			{
				apex.Write<int>(ent + OFFSET_GLOW_ENABLE, 1);
				apex.Write<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2);
				apex.Write<GlowMode>(ent + GLOW_TYPE, { 101,102,46,96 });
				apex.Write<float>(ent + GLOW_COLOR_R, 0.f);
				apex.Write<float>(ent + GLOW_COLOR_G, 122.f);
				apex.Write<float>(ent + GLOW_COLOR_B, 0.f);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
	}

	while (lookingForProcs == false&&v1 == false)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		for (int i = 0; i < 100; i++)
		{
			uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
			uint64_t entPtr = 0;
			apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), entPtr);

			Entity ent = ptrToEntity(entPtr);
			if (ent.isPlayer())
			{
				ent.enableGlow(0,122,0);
			}
		}
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(600));
}

static void itemGlowThread()
{
	printf("Started Item Glow Thread\n");

	while (lookingForProcs == false&&v1 == true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		for (int i = 0; i < 10000; i++)
		{
			uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
			uint64_t ent = 0;
			apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), ent);

			int curentEntItemID;
			apex.Read<int>(ent + OFFSET_ITEM_ID, curentEntItemID);
			if (curentEntItemID == LIGHT_ROUNDS || curentEntItemID == LIGHT_MAGAZINE_LV3 || curentEntItemID == HEAVY_MAGAZINE_LV3 || curentEntItemID == BODY_ARMOR_EVO3 || curentEntItemID == HEAVY_ROUNDS || curentEntItemID == BACKPACK_LV3 || curentEntItemID == HCOG_CLASSIC || curentEntItemID == HCOG_BRUISER || curentEntItemID == BARREL_STABILIZER_LV3)
			{
				apex.Write<int>(ent + OFFSET_GLOW_ENABLE, 1);
				apex.Write<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2);
				apex.Write<GlowMode>(ent + GLOW_TYPE, { 101,102,46,96 });
				apex.Write<float>(ent + GLOW_COLOR_R, 0.f);
				apex.Write<float>(ent + GLOW_COLOR_G, 122.f);
				apex.Write<float>(ent + GLOW_COLOR_B, 122.f);
			}
			else if (curentEntItemID == R99 || curentEntItemID == HEMLOCK || curentEntItemID == GOLD_KRABER || curentEntItemID == BODY_ARMOR_EVO4 || curentEntItemID == KNOCKDOWN_SHIELD_LV4 || curentEntItemID == R301 || curentEntItemID == BACKPACK_LV4)
			{
				apex.Write<int>(ent + OFFSET_GLOW_ENABLE, 1);
				apex.Write<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2);
				apex.Write<GlowMode>(ent + GLOW_TYPE, { 101,102,46,96 });
				apex.Write<float>(ent + GLOW_COLOR_R, 122.f);
				apex.Write<float>(ent + GLOW_COLOR_G, 0.f);
				apex.Write<float>(ent + GLOW_COLOR_B, 0.f);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
	}

	while (lookingForProcs == false && v1 == false)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		for (int i = 0; i < 10000;i++)
		{
			uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
			uint64_t entPtr = 0;
			apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), entPtr);

			Entity ent = ptrToEntity(entPtr);
			if (ent.isItem())
			{
				Item item = ptrToItem(entPtr);
				if (item.isGlowing() == false)
				{
					if (item.getItemID() == LIGHT_ROUNDS || item.getItemID() == LIGHT_MAGAZINE_LV3 || item.getItemID() == HEAVY_MAGAZINE_LV3 || item.getItemID() == BODY_ARMOR_EVO3 || item.getItemID() == HEAVY_ROUNDS || item.getItemID() == BACKPACK_LV3 || item.getItemID() == HCOG_CLASSIC || item.getItemID() == HCOG_BRUISER || item.getItemID() == BARREL_STABILIZER_LV3)
					{
						item.enableGlow(0, 122, 122);
					}
					if (item.getItemID() == R99|| item.getItemID() == R99 || item.getItemID() == HEMLOCK || item.getItemID() == GOLD_KRABER || item.getItemID() == BODY_ARMOR_EVO4 || item.getItemID() == KNOCKDOWN_SHIELD_LV4 || item.getItemID() == R301 || item.getItemID() == BACKPACK_LV4)
					{
						item.enableGlow(122, 0, 0);
					}
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(600));
	}

}

int main(int argc, char* argv[])
{
	//root check
	if (geteuid() != 0)
	{
		printf("Error: %s is not running as root\n", argv[0]);
		return 0;
	}

	const char* clientProc = "client_ap.exe";
	const char* apexProc = "R5Apex.exe";

	bool clientFound = false;
	bool apexFound = false;

	//main cheat loop
	while (lookingForProcs)
	{
		//check if procces is found or not. if found start cheat
		if (apex.get_proc_status() != process_status::FOUND_READY)
		{
			printf("Looking for apex process...\n");
			apex.open_proc(apexProc);

			if (apex.get_proc_status() == process_status::FOUND_READY)
			{
				apexBase = apex.get_proc_baseaddr();
				printf("Apex Proc Found\n");
				printf("Apex Base Addr: %lx\n", apexBase);
				apexFound = true;
			}
		}
		else
		{

			apex.check_proc();
		}
/*
		//check if procces is found or not. if found start cheat
		if (client.get_proc_status() != process_status::FOUND_READY)
		{
			printf("Looking for apex process...\n")
			client.open_proc(clientProc);

			if (client.get_proc_status() == process_status::FOUND_READY)
			{
				clientBase = client.get_proc_baseaddr();
				printf("Client Proc Found\n");
				printf("Client Base Addr: %lx\n", clientBase);
				clientFound = true;
			}
		}
		else
		{
			client.check_proc();
		}
		*/
		if (apexFound)
		{
			lookingForProcs = false;
		}
	}
	printf("Starting Item Glow Thread\n");
	std::thread glowItemThread;
	glowItemThread.~thread();
	glowItemThread = std::thread(itemGlowThread);
	glowItemThread.detach();


	printf("Starting Player Glow Thread\n");
	std::thread glowPlayerThread;
	glowPlayerThread.~thread();
	glowPlayerThread = std::thread(playerGlowThread);
	glowPlayerThread.detach();


	printf("Starting AimBot Thread\n");
	std::thread aimBotThread;
	aimBotThread.~thread();
	aimBotThread = std::thread(aimBotThreadFunc);
	aimBotThread.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	printf("Infinite While Loop Starting\n");
	while (lookingForProcs == false)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		apex.check_proc();
	}
}

