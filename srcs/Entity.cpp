#include <sstream>

#include "../includes/Entity.h"
#include "../includes/Memory.h"
#include "../includes/Game.h"
#include "../includes/Matrix.h"
#include "../includes/Logs.h"
#include "../includes/Main.h"
#include "../includes/Offsets.h"

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

	_teleportEntity teleportEntity = (_teleportEntity)(Memory::GetBaseAddress() + Offsets::fnTeleportEntity);

	float newCoords[3] = { coords.y, coords.z, coords.x };
	teleportEntity(this->ptr, &newCoords[0]);
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

unsigned int Entity::GetModel() const
{
	return this->ptr->modelIndex;
}

unsigned int Entity::GetMaxHealth() const
{
	return this->ptr->maxHealth;
}

void Entity::SetMaxHealth(unsigned int maxHealth)
{
	this->ptr->maxHealth = maxHealth;
}

void Entity::ResetHealth()
{
	this->SetHealth(this->GetMaxHealth());
}

Vector3 Entity::GetForwardVector() const
{
	struct Matrix4x4 mat = this->ptr->transformMatrix;

	return Vector3(mat.m[2][2], mat.m[2][0], mat.m[2][1]);
}

bool Entity::SetModel(unsigned int modelIndex)
{
	_setEntityModel setEntityModel = (_setEntityModel)(Memory::GetBaseAddress() + Offsets::fnSetEntityModel);

	return setEntityModel(this->ptr, modelIndex, this->GetModel());
}

std::vector<Entity> Entity::GetAll()
{
	std::vector<Entity> list;

	uintptr_t entityListPtr = Memory::GetPointerAddress(Offsets::entityList, { 0x0 });
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

Entity Entity::Spawn(unsigned int modelIndex, Vector3 coords)
{
	_spawnEntity spawnEntity = (_spawnEntity)(Memory::GetBaseAddress() + Offsets::fnSpawnEntity);
	float coordsArray[3] = { coords.y, coords.z, coords.x };

	GameEntity* res = spawnEntity(modelIndex, &coordsArray[0], 0, 0, 0, 1, 0, 0, 0, 1, 0, 0xFF);

	if (!res)
	{
		Logs::Add("Failed to create entity with model index %d (model not loaded ?)", modelIndex);
		return 0;
	}

	Logs::Add("Spawned entity (address : %p) !", res);

	Entity newEntity((uintptr_t)res);

	return newEntity;
}

void Entity::Kill()
{
	GameEntity* entityPtr = this->ptr;

	Main::RunInGameThread([entityPtr]()
	{
		_killEntity killEntity = (_killEntity)(Memory::GetBaseAddress() + Offsets::fnKillEntity);

		killEntity(entityPtr, 0, 0, 1, 1, 0, 0);
	});
}

void Entity::RenderMenu()
{
	if (ImGui::CollapsingHeader("Entities"))
	{
		std::vector<Entity> entities = Entity::GetAll();
		size_t entitiesCount = entities.size();

		ImGui::Text("Count: %i", entitiesCount);

		if (ImGui::Button("Swap everyone to yourself"))
		{
			unsigned int myModelIndex = Game::GetLocalEntity().GetModel();

			for (size_t i = 0; i < entitiesCount; i++)
			{
				entities[i].SetModel(myModelIndex);
			}
		}

		for (size_t i = 0; i < entitiesCount; i++)
		{
			ImGui::PushID((int)i);

			Entity currentEntity = entities[i];
			std::stringstream nodeText;

			nodeText << "[" << std::hex << currentEntity.GetAddress() << "] " << currentEntity.GetModelName();

			if (ImGui::TreeNode(nodeText.str().c_str()))
			{
				ImGui::Text("Coords: %s", currentEntity.GetCoords().ToString().c_str());
				ImGui::Text("Velocity: %s", currentEntity.GetVelocity().ToString().c_str());
				ImGui::Text("Model index: %d", currentEntity.GetModel());
				ImGui::Text("Char def address: %p", currentEntity.ptr->charDefFilePtr);
				ImGui::Text("Unk flags: %llu", currentEntity.ptr->unkFlags);

				ImGui::Text("Health: %i", currentEntity.GetHealth());

				if (ImGui::Button("Heal"))
				{
					currentEntity.ResetHealth();
				}

				int targetMaxHealth = currentEntity.GetMaxHealth();

				if (ImGui::InputInt("Max health", &targetMaxHealth))
				{
					currentEntity.SetMaxHealth(targetMaxHealth);
				}

				if (ImGui::Button("Teleport to"))
				{
					Game::GetLocalEntity().SetCoords(currentEntity.GetCoords());
				}
				
				ImGui::SameLine();

				if (ImGui::Button("Bring"))
				{
					currentEntity.SetCoords(Game::GetLocalEntity().GetCoords());
				}

				if (ImGui::Button("Kill"))
				{
					currentEntity.Kill();
				}

				ImGui::TreePop();
			}

			ImGui::PopID();
		}
	}
}