#include "Math.h"
#include "offsets.h"
#include "memory.h"

class Entity
{
public:
	Entity();
	~Entity();

	uint64 ptr;
	uint64 buffer[0x3ff0];
	bool isItem();

private:

};

Entity::Entity()
{
}

Entity::~Entity()
{
}