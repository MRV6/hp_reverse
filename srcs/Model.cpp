#include "../includes/Model.h"
#include "../includes/Memory.h"

uintptr_t Model::listAddress = 0;
int Model::modelDataSize = 0x68;
int Model::modelsCountOffset = 0x1417610;

Model::Model(uintptr_t address) : ptr(reinterpret_cast<GameModel*>(address)) {};

void Model::Init()
{
	Model::listAddress = Memory::GetPointerAddress(0x1417608, { 0x0 });
}

void Model::RenderMenu()
{
	if (ImGui::CollapsingHeader("Models"))
	{
		std::vector<Model> models = Model::GetAll();
		int modelsCount = models.size();

		ImGui::Text("Count: %i", modelsCount);

		for (int i = 0; i < modelsCount; i++)
		{
			Model currentModel = models[i];

			ImGui::PushID(i);

			if (ImGui::TreeNode(currentModel.ptr->name))
			{
				ImGui::Text("Label: %s", currentModel.ptr->label);
				ImGui::Text("Path: %s", currentModel.ptr->path);

				ImGui::TreePop();
			}

			ImGui::PopID();
		}
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