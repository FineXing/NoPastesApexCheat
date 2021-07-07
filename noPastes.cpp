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
#include "Game.h"
#include "offsets.h"
#include "memory.h"
#include "ids.h"


Memory apex;
Memory client;

uint64_t apexBase; // apex base addr
uint64_t clientBase; // client base addr

bool glowItems = true;

bool lookingForProcs = true; //read write - controls when cheat starts

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

static void aimBotThread()
{
	while (lookingForProcs ==false)
	{
		uint64_t localPlayer = 0;
		apex.Read<uint64_t>(apexBase + OFFSET_LOCAL_ENT, localPlayer);
		if (localPlayer = 0)continue;


	}
}

static void playerGlowThread()
{
	while (lookingForProcs == false)
	{
		for (int i = 0; i < 10000; i++)
		{
			uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
			uint64_t ent = 0;
			apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), ent);

			uint64_t localPlayer = 0;
			apex.Read<uint64_t>(g_Base + OFFSET_LOCAL_ENT, localPlayer);
			if (localPlayer == 0) continue;
			
			if (ent == localPlayer)
			{
				continue;
			}

			int playerTeamNum;
			apex.Read<uint64_t>(ent + OFFSET_TEAM, playerTeamNum);
			if (playerTeamNum < 0 || playerTeamNum > 50)
			{
				continue;
			}

			apex.Write<int>(ent + OFFSET_GLOW_ENABLE, 1);
			apex.Write<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2);
			apex.Write<GlowMode>(ent + GLOW_TYPE, { 101,102,46,96 });
			apex.Write<float>(ent + GLOW_COLOR_R, 0.f);
			apex.Write<float>(ent + GLOW_COLOR_G, 122.f);
			apex.Write<float>(ent + GLOW_COLOR_B, 0.f);
			}

		}

	}
}

static void itemGlowThread()
{
	printf("Started Glow Thread\n");

	while (lookingForProcs == false)
	{
		for (int i = 0; i < 10000; i++)
		{
			uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
			uint64_t ent = 0;
			apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), ent);

			int curentEntItemID;
			apex.Read<int>(ent + OFFSET_ITEM_ID, curentEntItemID);
			if (curentEntItemID == LIGHT_ROUNDS || curentEntItemID == LIGHT_MAGAZINE_LV3 || curentEntItemID == HEAVY_MAGAZINE_LV3 || curentEntItemID == HEAVY_ROUNDS || curentEntItemID == BACKPACK_LV3 || curentEntItemID == HCOG_CLASSIC || curentEntItemID == HCOG_BRUISER || curentEntItemID == BARREL_STABILIZER_LV3)
			{
				apex.Write<int>(ent + OFFSET_GLOW_ENABLE, 1);
				apex.Write<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2);
				apex.Write<GlowMode>(ent + GLOW_TYPE, { 101,102,46,96 });
				apex.Write<float>(ent + GLOW_COLOR_R, 0.f);
				apex.Write<float>(ent + GLOW_COLOR_G, 122.f);
				apex.Write<float>(ent + GLOW_COLOR_B, 122.f);
			}
			else if (curentEntItemID == R99 || curentEntItemID == HEMLOCK || curentEntItemID == GOLD_KRABER || curentEntItemID == BODY_ARMOR_EVO3)
			{
				apex.Write<int>(ent + OFFSET_GLOW_ENABLE, 1);
				apex.Write<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2);
				apex.Write<GlowMode>(ent + GLOW_TYPE, { 101,102,46,96 });
				apex.Write<float>(ent + GLOW_COLOR_R, 122.f);
				apex.Write<float>(ent + GLOW_COLOR_G, 0.f);
				apex.Write<float>(ent + GLOW_COLOR_B, 0.f);
			}
		}

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


	printf("Starting Player Glow Thread\n")
	std::thread glowPlayerThread;
	glowPlayerThread.~thread();
	glowPlayerThread = std::thread(playerGlowThread);
	glowPlayerThread.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	printf("Infinite While Loop Starting\n");
	while (lookingForProcs == false)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		apex.check_proc();
	}
}

