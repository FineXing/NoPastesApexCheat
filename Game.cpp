#include "Game.h"
#include "memory.h"
#include "offsets.h"


extern Memory apex;

bool Entity::isPlayer()
{
	uint64_t entName;
	apex.Read<uint64_t>(ptr + OFFSET_NAME, entName);
	
	if (entName == 125780153691248) {
		return true;
	}
	else
	{
		return false;
	}
}

bool Entity::isItem()
{
	int EntItemID = 0;
	apex.Read<int>(ptr + OFFSET_ITEM_ID, EntItemID);
	if (EntItemID > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Entity::isDummy()
{
	int entTeam;
	apex.Read<int>(ptr + OFFSET_TEAM, entTeam);
	if (entTeam == 97)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Entity::enableGlow(float R, float G, float B)
{
	apex.Write<int>(ptr + OFFSET_GLOW_ENABLE, 1);
	apex.Write<int>(ptr + OFFSET_GLOW_THROUGH_WALLS, 2);
	apex.Write<GlowMode>(ptr + GLOW_TYPE, { 101,102,46,96 });
	apex.Write<float>(ptr + GLOW_COLOR_R, R);
	apex.Write<float>(ptr + GLOW_COLOR_G, G);
	apex.Write<float>(ptr + GLOW_COLOR_B, B);
}


bool Entity::isGlowing()
{
	int glowState;
	apex.Read<int>(ptr + OFFSET_GLOW_ENABLE, glowState);
	if (glowState == 7)
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Entity::getDistance()
{
	return -1.f;
}

Vector Entity::getPosition()
{
	Vector entPos;
	apex.Read<Vector>(ptr + OFFSET_ORIGIN, entPos);
	return entPos;
}


int Item::getItemID()
{
	int EntItemID = 0;
	apex.Read<int>(ptr + OFFSET_ITEM_ID, EntItemID);
	return EntItemID;
}

void Item::enableGlow(float R, float G, float B)
{
	apex.Write<int>(ptr + OFFSET_GLOW_ENABLE, 1);
	apex.Write<int>(ptr + OFFSET_GLOW_THROUGH_WALLS, 2);
	apex.Write<GlowMode>(ptr + GLOW_TYPE, { 101,102,46,96 });
	apex.Write<float>(ptr + GLOW_COLOR_R, R);
	apex.Write<float>(ptr + GLOW_COLOR_G, G);
	apex.Write<float>(ptr + GLOW_COLOR_B, B);
}

bool Item::isGlowing()
{
	int glowState;
	apex.Read<int>(ptr + OFFSET_GLOW_ENABLE, glowState);
	if (glowState == 7)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector Item::getPosition()
{
	Vector entPos;
	apex.Read<Vector>(ptr + OFFSET_ORIGIN, entPos);
	return entPos;
}


Item ptrToItem(uintptr_t ptr)
{
	Item item = Item();
	item.ptr = ptr;
	return item;
}

Entity ptrToEntity(uintptr_t ptr)
{
	Entity ent = Entity();
	ent.ptr = ptr;
	return ent;
}

Player ptrToPlayer(uintptr_t ptr)
{
	Player player = Player();
	player.ptr = ptr;
	return player;
}

void Player::enableGlow(float R, float G, float B)
{
	apex.Write<int>(ptr + OFFSET_GLOW_ENABLE, 1);
	apex.Write<int>(ptr + OFFSET_GLOW_THROUGH_WALLS, 2);
	apex.Write<GlowMode>(ptr + GLOW_TYPE, { 101,102,46,96 });
	apex.Write<float>(ptr + GLOW_COLOR_R, R);
	apex.Write<float>(ptr + GLOW_COLOR_G, G);
	apex.Write<float>(ptr + GLOW_COLOR_B, B);
}

bool Player::isGlowing()
{
	int glowState;
	apex.Read<int>(ptr + OFFSET_GLOW_ENABLE, glowState);
	if (glowState == 7)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Vector Player::getPosition()
{
	Vector entPos;
	apex.Read<Vector>(ptr + OFFSET_ORIGIN, entPos);
	return entPos;
}

void Player::setViewAngles(vec2 angles)
{
	angles.Normalize();
	apex.Write<vec2>(ptr+OFFSET_VIEWANGLES,angles);
}

std::vector Player::getViewAngles()
{
	std::vector<float> angles;
	apex.Read<std::vector>(ptr +OFFSET_VIEWANGLES,angles);
	return angles;
}

