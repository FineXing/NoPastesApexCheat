#include "offsets.h"
#define FPS		                144
#define BONE_GRAVITY_FOV		2.5f
#define FOV_PER_FRAME			0.08f
static float lastLowestFov = BONE_GRAVITY_FOV;

void AimAssist2(std::vector<UINT> players) {

	int localTeam;
	apex.Read<int>(OFFSET_LOCAL_ENT + OFFSET_TEAM, localTeam);

	float lowestFov = FLT_MAX;
	Vec3 cameraPos, viewAngles;
	apex.Read<Vec3>(OFFSET_LOCAL_ENT + OFFSET_CAMERAPOS, cameraPos);
	apex.Read<Vec3>(OFFSET_LOCAL_ENT + OFFSET_VIEWANGLES, viewAngles);

	uint64_t target = NULL;
	Vec3 targetCenter;

	for (int i = 0; i < 100; i++)
	{
		uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
		uint64_t ent = 0;
		apex.Read<uint64_t>(entityList + ((uint64_t)i << 5), ent);
		if (ent == 0)continue;


		int playerTeam;
		apex.Read<int>(ent + OFFSET_TEAM, playerTeam);
		if (localTeam == playerTeam)continue;

		bool aliveState;
		apex.Read<int>(ent + OFFSET_LIFE_STATE, aliveState);
		if (aliveState != 0)continue;


	}

	for (auto const& val : players)
	{
		int playerTeam;
		apex.Read<_>( + OFFSET_TEAM, playerTeam);
		if (playerTeam == localTeam)
			continue;

		if (!IsAlive(ents[val].pEntity))
			continue;

		Vec3 potentialTargetCenter = GetBone(ents[val].pEntity, 2);
		float currentFOV = GetFOV(cameraPos, viewAngles, potentialTargetCenter);

		if (currentFOV < lowestFov) {
			lowestFov = currentFOV;
			target = ents[val].pEntity;
			targetCenter = potentialTargetCenter;
		}
	}

	if (!target)
		return;

	Vec3 targetBone;
	lowestFov = BONE_GRAVITY_FOV;
	UINT bones[] = { 2, 3, 5, 7, 8, 11, 12 };
	for (auto const& val : bones) {
		Vec3 potentialTargetBone = GetBone(target, val);
		float currentFOV = GetFOV(cameraPos, viewAngles, potentialTargetBone);

		if (currentFOV < lowestFov) {
			lowestFov = currentFOV;
			targetBone = potentialTargetBone;
		}
	}
	if (lowestFov < BONE_GRAVITY_FOV && lowestFov > lastLowestFov) {
		Vec3 newViewAngles = viewAngles -
			(viewAngles - AimAt(cameraPos, targetBone)).normal * FOV_PER_FRAME;
		d.Write(ents[localId].pEntity + OFFSET_VIEWANGLES, newViewAngles);
	}
	lastLowestFov = lowestFov;
}


unsigned int Input{ 0x1C74380 };
auto m_pCommands{ apex.read<unsigned int>(Input + 0xF8, true) };
auto first_command_number{ apex.read<int>(m_pCommands) };
while (first_command_number == apex.read<int>(m_pCommands)); //verifies that current usercmd is latest
auto next_cmd_number{ apex.read<int>(m_pCommands + 0x218) + 1 }; //Grab next current usercmd (0x218 is size of UserCmd)
 
while (true) {
	unsigned int current_command{ m_pCommands + 0x218 * (next_cmd_number % 750) };
	while (next_cmd_number != apex.read<int>(current_command)); //Wait for usercmd to be accessed
	unsigned int old_command{ m_pCommands + 0x218 * ((next_cmd_number - 1) % 750) };
 
	Memory::Structs::Vector2 view_angles{ 0.0f, 0.0f };
	apex.write(old_command + 0x38, 1); //Write to buttons
	apex.write(old_command + 0xC, view_angles); //Write to viewangles
 
	next_cmd_number = apex.read<int>(current_command) + 1;
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
								apex.Write(old_command + 0xC, angle); //Write to viewangles
								apex.Read<int>((current_command) + 1, next_cmd_number);
						}
						*/
					