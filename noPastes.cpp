#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <random>
#include <chrono>
#include <iostream>
#include <cfloat>
#include <thread>
#include "Game.h"
#include "offsets.h"
#include "memory.h"


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

static void itemGlowThread()
{
	printf("Started Glow Thread");
	while (glowItems == true)
	{

		if (!lookingForProcs)
		{
			for (int i = 0; i >= 10000;i++)
			{
				uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
				uint64_t ent = 0;

				apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), ent);
				//uint64_t BaseEnt;
				//apex.Read<uint64_t>(entityList,BaseEnt)
				if (ent == 0) continue;

				int curentEntItemID = apex<int>(apexBase + ent + OFFSET_ITEM_ID);
				if (curentEntItemID == 47)
				{
					apex.Write<int>(ent + OFFSET_GLOW_ENABLE, 1);
					apex.Write<int>(ent + OFFSET_GLOW_THROUGH_WALLS, 2);
					apex.Write<GlowMode>(ent + GLOW_TYPE, { 101,102,46,96 });
					apex.Write<float>(ent + GLOW_COLOR_R, 0.f);
					apex.Write<float>(ent + GLOW_COLOR_G, 122.f);
					apex.Write<float>(ent + GLOW_COLOR_B, 122.f);
				}
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
			printf("Starting Glow Thread");
			std::thread glowThread;
			glowThread.~thread();
			glowThread = std::thread(itemGlowThread);
			glowThread.detach();
		}
	}
	printf("Infinite While Loop Starting");
	while (lookingForProcs== false)
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));
		apex.check_proc();
	}
}
