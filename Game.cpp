#include "Game.h"
#include "memory.h"
#include "offsets.h"


extern Memory apex;
extern uint64_t apexBase;
extern float smoothing;



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

uintptr_t getBestEnemey()
{
	uint64_t entityList = apexBase + OFFSET_ENTITYLIST;
	
	for(int i = 0; i <10000;i++)
	{

	}
}

void Player::enableGlow(float R, float G, float B,GlowMode shitty)
{
	apex.Write<int>(ptr + OFFSET_GLOW_ENABLE, 1);
	apex.Write<int>(ptr + OFFSET_GLOW_THROUGH_WALLS, 2);
	apex.Write<GlowMode>(ptr + GLOW_TYPE, shitty);
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


QAngle Player::getViewAngles()
{
	QAngle temp;
	SVector angles(0.f,0.f,0.f);
	apex.Read<SVector>(ptr +OFFSET_VIEWANGLES,angles);
	temp.x = angles.x;
	temp.y = angles.y;
	temp.z = angles.z;
	return temp;
}

void Entity::setViewAngles(QAngle& angles)
{
	setViewAngles(SVector(angles));
}

void Entity::setViewAngles(SVector angles)
{
	apex.Write<SVector>(ptr +OFFSET_VIEWANGLES,angles);
}

QAngle Entity::getViewAngles()
{
	QAngle angles;
	apex.Read<QAngle>(ptr + OFFSET_VIEWANGLES, angles);
	return angles;
}

void Player::setViewAngles(QAngle& angles)
{
	setViewAngles(SVector(angles));
}

void Player::setViewAngles(SVector angles)
{
	apex.Write<SVector>(ptr +OFFSET_VIEWANGLES,angles);
}

Vector Player::getCamPosition()
{
	Vector pos;
	apex.Read<Vector>(ptr + OFFSET_CAMERAPOS,pos);
	return pos;
}

QAngle Player::getRecoilAngles()
{
	QAngle angles;
	apex.Read<QAngle>(ptr + OFFSET_AIMPUNCH,angles);
	return angles;
}

Vector Entity::getCamPosition()
{
	Vector pos;
	apex.Read<Vector>(ptr + OFFSET_CAMERAPOS,pos);
	return pos;
}

float getAngle(float angle)
{
	if(fmod(angle,180)<0)
	{
		return angle;
	}
	else
	{
		return angle + 180.f;
	}
}

//my shit attempt to clamp angles
QAngle clampAngles(QAngle angle)
{
	
	if (angle.x > 89.0f)
	{
		angle.x = 88.8;
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
	return angle;
}

QAngle calcAngles(Player player, Entity target)
{
	QAngle angle = QAngle();
	QAngle oldVAngles = player.getViewAngles();
	Vector shit = target.getPosition() -player.getCamPosition() ;

	
	//find hypo. c = hypo
	float c = sqrt(pow(shit.x, 2) + pow(shit.y, 2));

	float yaw = (atan2(shit.y, shit.x))*(180/M_PI);

	float ptich = (-(atan2(shit.z, c)))*(180/M_PI);

	float diferencePitch = ptich - oldVAngles.x;
	float diferenceYaw = yaw - oldVAngles.y;
	
	float delta = sqrt(pow(diferenceYaw,2.f)+pow( diferencePitch,2.f));

	//float testYaw = getAngle(angle.y);

	//if (diferenceYaw >= 0.f || diferenceYaw <= 180.f)
	//{
	//	angle.y -= diferenceYaw / smoothing;
	//}
	//else if(diferenceYaw >= -0.f || diferenceYaw <= -180.f)
	//{
	//	angle.y += diferenceYaw / smoothing;
	//}

	//angle.y = (3.5f / (1+diferenceYaw)) * sin(((2.f * M_PI_F) / -5.8f) * diferenceYaw);
	angle.y += diferenceYaw + delta/smoothing;
	angle.x += diferencePitch + delta/smoothing;
	angle.z = 0.f;
	return angle;
}

//credit: https://www.unknowncheats.me/forum/apex-legends/410912-apex-legends-external-vis-check.html
float Entity::getVisibleTime()
{
	float lastVisible;
	apex.Read<float>(ptr + OFFSET_VISIBLE_TIME, lastVisible);
	return lastVisible;
}

bool Entity::isVisible()
{
	const auto vis_check = getVisibleTime( );
 
  // If the player was never visible the value is -1
  const auto is_vis = vis_check > last_vis_time_flt || vis_check < 0.f && last_vis_time_flt > 0.f;
 
  last_vis_time_flt = vis_check;
 
  return is_vis;
}

float Player::getVisibleTime()
{
	float lastVisible;
	apex.Read<float>(ptr+OFFSET_VISIBLE_TIME,lastVisible);
	return lastVisible;
}

bool Player::isVisible()
{
	const auto vis_check = getVisibleTime( );
 
  // If the player was never visible the value is -1
  const auto is_vis = vis_check > last_vis_time_flt || vis_check < 0.f && last_vis_time_flt > 0.f;
 
  last_vis_time_flt = vis_check;
 
  return is_vis;
}