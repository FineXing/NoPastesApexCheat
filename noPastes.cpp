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

bool glowItemsEnabled = false;
bool glowPlayersEnabled = false;
bool aimbotEnabled = true;
float maxDistance = 200.0f * 40.0f;
bool rcs = true;
float rcsX = 10.f;
float rcsY = 25.f;
bool lookingForProcs = true; //read write - controls when cheat starts

typedef struct player
{
	uint64_t playerPtr;
	float locX;
	float locY;
	float locZ;
	int team = 0;

}player;


//UNUSED UNTESTED AS OF RN
static void updatePlayerList()
{
	uint64_t entList = apexBase + OFFSET_ENTITYLIST;
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		for(int i = 0; i<=100;i++)
		{
			uint64_t localPlayer = 0;
			apex.Read<uint64_t>(apexBase + OFFSET_LOCAL_ENT, localPlayer);

			uint64_t curEnt = 0;
			apex.Read<uint64_t>(entList + ((uint64_t)i << 5), curEnt);
			if (curEnt == 0)continue;
			if(curEnt == localPlayer)continue;
		}
	}
	
}


static void aimBotThreadFunc()
{
	printf("Started Aimbot Thread\n");
	//previus recoil angle (keeps from jumping down)
	QAngle oldRecoilAngle;
	oldRecoilAngle.x = 0.f;
	oldRecoilAngle.y= 0.f;
	oldRecoilAngle.z =0.f;
	while (lookingForProcs ==false)
	{
		while (aimbotEnabled)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			//getting local player pointer checking it
			uint64_t localPlayerPtr = 0;
			apex.Read<uint64_t>(apexBase + OFFSET_LOCAL_ENT, localPlayerPtr);
			uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
			if (localPlayerPtr == 0) continue;

			//loop though all ents
			for (int i = 0; i < 10000; i++)
			{
			
				Player localPlayer = ptrToPlayer(localPlayerPtr);

				//gets ptr for current ent in loop and transforms to ent class
				uint64_t entPtr = 0;
				apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), entPtr);
				Entity ent = ptrToEntity(entPtr);

				//checks if ent is dummy 
				if (!ent.isDummy())
				{
					continue;
				}

				Vector entPos = ent.getPosition();
				Vector localPlayerPos = localPlayer.getPosition();
				float distance = localPlayer.getPosition().DistTo(entPos);
				printf("dist: %F\n", distance);
				if(true)
				{
					//view angle we are writing to player view angles
					QAngle angle;
					//getting original angles
					vec2 oldVAngles = localPlayer.getViewAngles();

					//recoil control system 
					if (rcs)
					{
						if(distance <143.f)
						{
							//shit = ent pos relitive to localplayer
							Vector shit = entPos - localPlayerPos;

							float c = sqrt(pow(shit.x,2) + pow(shit.z,2));

							float yaw = atan2(shit.z,shit.x) - 90.f;

							float ptich = -1*(atan2(shit.y,c));

							angle.x = yaw;
							angle.y = ptich;

							//my shit attempt to clamp angles probs should make this a funtion
							if (angle.x > 89.0f)
							{
								angle.x -= 180.f;
							}
							if (angle.x < -89.0f) 
							{
								angle.x += 180.f;
							}
							if (angle.y > 180.f) 
							{
								angle.y -= 360.f;
							}
							if (angle.y < -180.f)
							{
								angle.y += 360.f;
							}

						}

						//QAngle recoilAngles = localPlayer.getRecoilAngles();
						//angle.x = oldVAngles.x + (oldRecoilAngle.x - recoilAngles.x)*(rcsX/100.f);
               			//angle.y = oldVAngles.y + (oldRecoilAngle.y - recoilAngles.y)*(rcsY/100.f);

						

						//setting angles
						localPlayer.setViewAngles(angle);
						//oldRecoilAngle = recoilAngles;
					}		
				}
			}
			/*
			if (true)
			{		
				uint64_t Input{ 0x1caa3f0 };

				uint64_t m_pCommands;
				apex.Read<uint64_t>(Input + 0xF8, m_pCommands);

				int first_command_number;
				apex.Read<int>(m_pCommands,first_command_number);
				
				int temp;
				apex.Read<int>(m_pCommands,temp);
				while (first_command_number == temp); //verifies that current usercmd is latest


				int next_cmd_number;
				apex.Read<int>((m_pCommands + 0x218) + 1, next_cmd_number ); //Grab next current usercmd (0x218 is size of UserCmd)
 
				while (true)
				{
					uint64_t current_command{ m_pCommands + 0x218 * (next_cmd_number % 750) };

					bool doingShit = true;

					while (doingShit)
					{
						int curCommandTemp;
						apex.Read<int>(current_command,curCommandTemp);
						if (next_cmd_number == curCommandTemp)
						{
							doingShit = false;
						}
					}

					unsigned int old_command{ m_pCommands + 0x218 * ((next_cmd_number - 1) % 750) };
 
					apex.Write(old_command + 0x38, 1); //Write to buttons
					apex.Write(old_command + 0xC, vAngles); //Write to viewangles
					apex.Read<int>((current_command) + 1, next_cmd_number);
				}
			}
			*/
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
		while (glowPlayersEnabled)
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
			std::this_thread::sleep_for(std::chrono::milliseconds(600));
		}
	}

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
		while (glowItemsEnabled)
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
			std::this_thread::sleep_for(std::chrono::milliseconds(600));
		}
		/* code */
		

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
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		apex.check_proc();
	}
}

