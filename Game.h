#include <stdint.h>
#include "vec.h"

class Entity
{
public:
	uint64_t ptr;
	bool isPlayer();
	bool isItem();
	bool isDummy();
	void enableGlow(float R, float G, float B);
	bool isGlowing();
	float getDistance();
	Vector getPosition();
	QAngle getViewAngles();
	void setViewAngles(QAngle& angles);
	void setViewAngles(SVector angles);
	Vector getCamPosition();
	float getVisibleTime();
	bool isVisible();
	float last_vis_time_flt;
};

class Player
{
public:
	uint64_t ptr;
	void enableGlow(float R, float G, float B, GlowMode shitty);
	bool isGlowing();
	float getDistance();
	Vector getPosition();
	QAngle getViewAngles();
	void setViewAngles(QAngle& angles);
	void setViewAngles(SVector angles);
	Vector getCamPosition();
	QAngle getRecoilAngles();
	float getVisibleTime();
	bool isVisible();
	float last_vis_time_flt;
};

class Item
{
public:
	uint64_t ptr;
	int getItemID();
	void enableGlow(float R, float G, float B);
	bool isGlowing();
	float getDistance();
	Vector getPosition();
};

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};


QAngle clampAngles(QAngle angle);
QAngle calcAngles(Player player, Entity target);
float getAngle(float angle);

uintptr_t getBestEnemey();
Item ptrToItem(uintptr_t ptr);
Entity ptrToEntity(uintptr_t ptr);
Player ptrToPlayer(uintptr_t ptr);

