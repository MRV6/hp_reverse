#include "../includes/Entity.h"
#include "../includes/Memory.h"
#include "../includes/Debug.h"
#include "../includes/Game.h"

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

unsigned int Entity::GetHealth() const
{
	return this->ptr->health;
}

void Entity::SetHealth(unsigned int health)
{
	this->ptr->health = health;
}

std::ostream& operator<<(std::ostream& os, const Entity& ent)
{
	os << "[" << GREEN << std::hex << ent.GetAddress() << std::dec << RESET << "] ";
	os << "Model name: " << YELLOW << ent.GetModelName() << RESET;
	os << " | Coords: " << YELLOW << ent.GetCoords() << RESET;
	os << " | Velocity: " << YELLOW << ent.GetVelocity() << RESET;
	os << " | Health: " << YELLOW << ent.GetHealth() << RESET;
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

void Entity::RenderMenu()
{
	if (ImGui::CollapsingHeader("Entities"))
	{
		std::vector<Entity> entities = Entity::GetAll();

		for (int i = 0; i < entities.size(); i++)
		{
			ImGui::PushID(i);

			Entity currentEntity = entities[i];
			std::string modelName = currentEntity.GetModelName();

			if (ImGui::TreeNode(modelName.c_str()))
			{
				ImGui::Text("Address: %x", currentEntity.GetAddress());
				ImGui::Text("Health: %i", currentEntity.GetHealth());
				ImGui::Text("Coords: %s", currentEntity.GetCoords().ToString().c_str());
				ImGui::Text("Velocity: %s", currentEntity.GetVelocity().ToString().c_str());

				ImGui::TreePop();
			}

			ImGui::PopID();
		}
	}
}