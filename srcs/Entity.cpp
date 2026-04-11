#include "../includes/Entity.h"
#include "../includes/Memory.h"
#include "../includes/Debug.h"

Entity::Entity(uintptr_t address)
{
	this->ptr = 0;

	if (address == 0)
	{
		return;
	}

	this->ptr = reinterpret_cast<GameEntity*>(address);
}

Vector3 Entity::GetCoords() const
{
	if (!this->ptr)
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	return Vector3(this->ptr->xAxis, this->ptr->yAxis, this->ptr->zAxis);
}

void Entity::SetCoords(Vector3 coords)
{
	if (!this->ptr)
	{
		return;
	}

	this->ptr->xAxis = coords.x;
	this->ptr->yAxis = coords.y;
	this->ptr->zAxis = coords.z;
}

Vector3 Entity::GetVelocity() const
{
	if (!this->ptr)
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	return Vector3(this->ptr->xVelocity, this->ptr->yVelocity, this->ptr->zVelocity);
}

void Entity::SetVelocity(Vector3 velocity)
{
	if (!this->ptr)
	{
		return;
	}

	this->ptr->xVelocity = velocity.x;
	this->ptr->yVelocity = velocity.y;
	this->ptr->zVelocity = velocity.z;
}

std::string Entity::GetModelName() const
{
	std::string modelNameAddr = (char*)this->ptr->modelNamePtr;

	return modelNameAddr;
}

uintptr_t Entity::GetAddress() const
{
	return (uintptr_t)this->ptr;
}

std::ostream& operator<<(std::ostream& os, const Entity& ent)
{
	os << "[" << GREEN << std::hex << ent.GetAddress() << std::dec << RESET << "] ";
	os << "Model name: " << YELLOW << ent.GetModelName() << RESET;
	os << " | Coords: " << YELLOW << ent.GetCoords() << RESET;
	os << " | Velocity: " << YELLOW << ent.GetVelocity() << RESET;
	os << std::endl;
	return os;
}

std::vector<Entity> Entity::GetAll()
{
	std::vector<Entity> list;

	uintptr_t entityListPtr = Memory::GetPointerAddress(0x01426D50, { 0x0 });
	uintptr_t entityListAddr = *(uintptr_t*)entityListPtr;

	for (int i = 0; i < 256; i++)
	{
		int offset = ENTITY_MEM_SIZE * i;
		uintptr_t entityAddr = entityListAddr + offset;

		if (*(uintptr_t*)entityAddr == 0)
		{
			break;
		}

		Entity entity = Entity::Entity(entityAddr);

		list.push_back(entity);
	}

	return list;
}

void Entity::DumpAll()
{
	std::vector<Entity> entityList = Entity::GetAll();

	std::cout << std::endl << "========== Entities count : " << GREEN << entityList.size() << RESET << " ==========" << std::endl << std::endl;

	for (int i = 0; i < entityList.size(); i++)
	{
		std::cout << entityList[i] << std::endl;
	}
}