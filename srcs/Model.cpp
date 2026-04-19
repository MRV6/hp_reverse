#include "../includes/Model.h"
#include "../includes/Memory.h"
#include "../includes/Entity.h"
#include "../includes/Game.h"
#include "../includes/Logs.h"
#include "../includes/World.h"
#include "../includes/Main.h"

#include "../vendor/imgui/misc/cpp/imgui_stdlib.h"

#include <algorithm>
#include <sstream>
#include <unordered_map>

uintptr_t Model::listAddress = 0;
int Model::modelDataSize = 0x68;
int Model::modelsCountOffset = 0x1417610; // Number of models (currently -> 635)
bool Model::showLoadedOnly = false;
bool Model::sortListByIndex = false;

static std::unordered_map<std::string, std::string> searchPerCategory = {};

int lastLoadedModelsCount = 0;

Model::Model(uintptr_t address) : ptr(reinterpret_cast<GameModel*>(address)) {};

void Model::Init()
{
	Model::listAddress = Memory::GetPointerAddress(0x1417608, { 0x0 });
}

char* Model::GetName() const
{
	return this->ptr->name;
}

char* Model::GetLabel() const
{
	return this->ptr->label;
}

char* Model::GetPath() const
{
	return this->ptr->path;
}

unsigned int Model::GetModelIndex() const
{
	return this->ptr->modelIndex;
}

bool Model::IsLoaded() const
{
	World* worldPtr = (World*)Memory::GetPointerAddress(0x141B3A0, { 0x0 });

	if (!worldPtr)
	{
		return false;
	}

	uintptr_t levelPtrParent = Memory::GetPointerAddress(0x1420130, { 0x0 });

	if (!levelPtrParent)
	{
		return false;
	}

	uintptr_t levelPtr = *(uintptr_t*)(levelPtrParent + 0x268);

	if (!levelPtr)
	{
		return false;
	}

	uintptr_t baseAddress = Memory::GetBaseAddress();

	_getLoadedCharDefFile getLoadedCharDefFile = (_getLoadedCharDefFile)(baseAddress + 0x527F0);
	uintptr_t charDefFile = getLoadedCharDefFile(worldPtr, levelPtr, this->GetModelIndex());

	return charDefFile != 0;
}

void Model::Load() const
{
	unsigned int modelIndex = this->GetModelIndex();
	char* modelName = this->GetName();

	Main::RunInGameThread([modelName, modelIndex]() {
		uintptr_t levelPtrParent = Memory::GetPointerAddress(0x1420130, { 0x0 });

		if (!levelPtrParent)
		{
			return;
		}

		uintptr_t levelPtr = *(uintptr_t*)(levelPtrParent + 0x268);

		if (!levelPtr)
		{
			return;
		}

		_loadModel loadModel = (_loadModel)(Memory::GetBaseAddress() + 0x2A8190);

		if (!loadModel((uint16_t)modelIndex, levelPtr, 1, 0, 0))
		{
			Logs::Add("Failed to load model %s !", modelName);
			return;
		}

		Logs::Add("Model %s loaded successfully !", modelName);
	});
}

std::string Model::GetType() const
{
	std::string path = this->GetPath();
	size_t pos = path.find("\\");
	std::string category = path.substr(0, pos);

	return category;
}

static std::string GetModelHeaderText(Model model, bool isLoaded)
{
	std::stringstream headerText;

	headerText << "[" << model.GetModelIndex() << "] " << model.GetName();

	if (isLoaded)
	{
		headerText << " (loaded)";
	}

	headerText << std::endl;

	return headerText.str();
}

static void SpawnForwardEntity(unsigned int modelIndex)
{
	Entity localEntity = Game::GetLocalEntity();
	Vector3 localEntityCoords = localEntity.GetCoords();
	Vector3 spawnCoords = localEntityCoords - localEntity.GetForwardVector();

	Entity::Spawn(modelIndex, spawnCoords);
}

static auto GetSortedModels(size_t modelsCount, std::vector<Model> models, int* currentLoadedModelsCount)
{
	std::unordered_map<std::string, std::vector<Model>> sortedModelsByCategory = {};

	for (size_t i = 0; i < modelsCount; i++)
	{
		Model currentModel = models[i];
		std::string modelType = currentModel.GetType();
		auto& category = sortedModelsByCategory[modelType];

		if (currentModel.IsLoaded())
		{
			(*currentLoadedModelsCount)++;
		}
		else
		{
			if (Model::showLoadedOnly)
			{
				continue;
			}
		}

		category.push_back(currentModel);
	}

	return sortedModelsByCategory;
}

void Model::RenderMenu()
{
	if (ImGui::CollapsingHeader("Models"))
	{
		std::vector<Model> models = Model::GetAll();
		
		std::sort(models.begin(), models.end(), [](const Model& a, const Model& b) {
			if (Model::sortListByIndex)
			{
				return a.GetModelIndex() < b.GetModelIndex();
			}

			return std::strcmp(a.GetName(), b.GetName()) < 0;
		});

		size_t modelsCount = models.size();
		int currentLoadedModelsCount = 0;
		std::unordered_map<std::string, std::vector<Model>> sortedModelsByCategory = GetSortedModels(modelsCount, models, &currentLoadedModelsCount);

		ImGui::Checkbox("Show only loaded models", &Model::showLoadedOnly);
		ImGui::SameLine();
		ImGui::Checkbox("Sort by index", &Model::sortListByIndex);

		ImGui::Text("Total: %i (loaded: %i)", modelsCount, lastLoadedModelsCount);

		for (const auto& [category, models] : sortedModelsByCategory)
		{
			const char* categoryName = category.c_str();

			if (ImGui::TreeNode(categoryName))
			{
				ImGui::PushID(categoryName);
				ImGui::InputText("Search", &searchPerCategory[category]);
				ImGui::PopID();

				for (size_t i = 0; i < models.size(); i++)
				{
					Model currentModel = models[i];
					std::string modelName = currentModel.GetName();

					if (searchPerCategory[category] != "" && modelName.find(searchPerCategory[category]) == std::string::npos)
					{
						continue;
					}

					bool isLoaded = currentModel.IsLoaded();
					unsigned int modelIndex = currentModel.GetModelIndex();

					ImGui::PushID(modelIndex);

					if (ImGui::TreeNodeEx(GetModelHeaderText(currentModel, isLoaded).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						ImGui::Text("Label: %s", currentModel.GetLabel());
						ImGui::Text("Path: %s", currentModel.GetPath());
						ImGui::Text("Address: %p", currentModel.ptr);

						if (!isLoaded)
						{
							if (ImGui::Button("Load"))
							{
								currentModel.Load();
							}
						}
						else
						{
							if (ImGui::Button("Spawn"))
							{
								SpawnForwardEntity(modelIndex);
							}

							if (ImGui::Button("Swap to"))
							{
								if (Game::GetLocalEntity().SetModel(modelIndex))
								{
									Logs::Add("Swapped to %s model !", currentModel.GetName());
								}
								else
								{
									Logs::Add("Could not swap to %s model", currentModel.GetName());
								}
							}
						}

						ImGui::TreePop();
					}

					ImGui::PopID();
				}

				ImGui::TreePop();
			}
		}

		lastLoadedModelsCount = currentLoadedModelsCount;
	}
}

std::vector<Model> Model::GetAll()
{
	std::vector<Model> list;

	uintptr_t modelsList = Model::listAddress;
	int modelsCount = *(int*)(Memory::GetBaseAddress() + Model::modelsCountOffset);

	for (int i = 0; i < modelsCount; i++)
	{
		uintptr_t modelAddr = modelsList + (Model::modelDataSize * i);
		Model model = Model::Model(modelAddr);

		list.push_back(model);
	}

	return list;
}