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
	vec2 getViewAngles();
	void setViewAngles(QAngle& angles);
	void setViewAngles(SVector angles);
};

class Player
{
public:
	uint64_t ptr;
	void enableGlow(float R, float G, float B);
	bool isGlowing();
	float getDistance();
	Vector getPosition();
	vec2 getViewAngles();
	void setViewAngles(QAngle& angles);
	void setViewAngles(SVector angles);
	Vector getCamPosition();
	QAngle getRecoilAngles();

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


uintptr_t getBestEnemey();
Item ptrToItem(uintptr_t ptr);
Entity ptrToEntity(uintptr_t ptr);
Player ptrToPlayer(uintptr_t ptr);
