class Entity
{
public:
	uint64_t ptr;
	bool isPlayer();
	bool isItem();
	void enableGlow(float R, float G, float B);
	bool isGlowing();
};

class Item
{
public:
	uint64_t ptr;
	int getItemID();
	void enableGlow(float R, float G, float B);
	bool isGlowing();
};

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

Item ptrToItem(uintptr_t ptr);
Entity ptrToEntity(uintptr_t ptr);
