#include "Application.h"
#include "ModuleGOManager.h"
#include "Component.h"
#include "GameObject.h"
#include "Imgui\imgui.h"
#include "ComponentCamera.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = new GameObject();
	root->AddComponent(C_TRANSFORM);
}

ModuleGOManager::~ModuleGOManager()
{
	if(root)
		delete root;

	selected_GO = nullptr;
}

bool ModuleGOManager::Start()
{

	return true;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	if (root)
		PreUpdateGameObjects(root);

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::Update(float dt)
{
	//Update GameObjects
	if(root)
		UpdateGameObjects(dt, root);

	//Display windows
	HierarchyWindow();
	InspectorWindow();

	return UPDATE_CONTINUE;
}

GameObject* ModuleGOManager::CreateGameObject(GameObject* parent)
{
	GameObject* obj_parent = (parent) ? parent : root;

	GameObject* object = new GameObject(obj_parent);

	if (obj_parent->AddChild(object) == false)
	{
		LOG("A child insertion to GameObject %s could not be done", obj_parent->name.data());
	}

	return object;
}

bool ModuleGOManager::RemoveGameObject(GameObject* object)
{
	bool ret = false;

	if (object)
	{
		object->GetParent()->RemoveChild(object);
		object->RemoveAllChilds();
		
		delete object;

		ret = true;
	}

	return ret;
}

void ModuleGOManager::GetAllCameras(std::vector<ComponentCamera*>& list, GameObject* from) const
{
	GameObject* go = (from) ? from : root;
	
	ComponentCamera* cam = (ComponentCamera*)go->GetComponent(C_CAMERA);
	if (cam)
		list.push_back(cam);
	
	const vector<GameObject*>* childs = go->GetChilds();
	for (vector<GameObject*>::const_iterator child = childs->begin(); child != childs->end(); ++child)
		GetAllCameras(list, (*child));
}

void ModuleGOManager::HierarchyWindow()
{
	ImGui::Begin("Hierarchy");

	DisplayGameObjectsChilds(root->GetChilds());

	if (ImGui::IsMouseHoveringWindow())
		if (ImGui::IsMouseClicked(1))
			ImGui::OpenPopup("HierarchyOptions");

	if (ImGui::BeginPopup("HierarchyOptions"))
	{
		if (ImGui::Selectable("Create Empty GameObject"))
		{
			selected_GO = CreateGameObject(NULL);
		}

		if (ImGui::Selectable("Create Empty Child"))
		{
			selected_GO = CreateGameObject(selected_GO);
		}

		if (ImGui::Selectable("Remove selected GameObject"))
		{
			if (selected_GO != nullptr)
			{
				RemoveGameObject(selected_GO);
				selected_GO = nullptr;
			}
		}
		ImGui::EndPopup();
	}

	ImGui::End();
}

void ModuleGOManager::DisplayGameObjectsChilds(const std::vector<GameObject*>* childs)
{
	for (vector<GameObject*>::const_iterator object = (*childs).begin(); object != (*childs).end(); ++object)
	{
		uint flags = 0;
		if ((*object) == selected_GO)
			flags = ImGuiTreeNodeFlags_Selected;
		
		if ((*object)->ChildCount() > 0)
		{
			if (ImGui::TreeNodeEx((*object)->name.data(), flags))
			{
				if (ImGui::IsItemClicked(0))
				{
					selected_GO = (*object);
				}

				DisplayGameObjectsChilds((*object)->GetChilds());
				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::TreeNodeEx((*object)->name.data(), flags | ImGuiTreeNodeFlags_Leaf))
			{
				if (ImGui::IsItemClicked(0))
				{
					selected_GO = (*object);
				}
				ImGui::TreePop();
			}
		}
	}
}

void ModuleGOManager::InspectorWindow()
{
	ImGui::Begin("Inspector");

	if (selected_GO)
	{
		//Active
		bool is_active = selected_GO->IsActive();
		if (ImGui::Checkbox("", &is_active))
		{
			selected_GO->SetActive(is_active);
		}

		//Name
		ImGui::SameLine();
		ImGui::InputText("###goname", selected_GO->name._Myptr(), selected_GO->name.capacity());

		//Components
		const std::vector<Component*>* components = selected_GO->GetComponents();
		for (std::vector<Component*>::const_iterator component = (*components).begin(); component != (*components).end(); ++component)
		{
			(*component)->OnInspector();
		}

		//Options
		if (ImGui::IsMouseHoveringWindow())
			if (ImGui::IsMouseClicked(1))
				ImGui::OpenPopup("InspectorOptions");

		if (ImGui::BeginPopup("InspectorOptions"))
		{
			if (ImGui::Selectable("Add Transform"))
			{
				selected_GO->AddComponent(C_TRANSFORM);
			}

			if (ImGui::Selectable("Add Mesh"))
			{
				selected_GO->AddComponent(C_MESH);
			}

			if (ImGui::Selectable("Add Material"))
			{
				selected_GO->AddComponent(C_MATERIAL);
			}

			if (ImGui::Selectable("Add Camera"))
			{
				selected_GO->AddComponent(C_CAMERA);
			}

			ImGui::EndPopup();
		}
	}

	ImGui::End();
}


void ModuleGOManager::UpdateGameObjects(float dt, GameObject* object)
{
	PROFILE("ModuleGOManager::UpdateGameObjects");

	if(root != object && object->IsActive() == true)
		object->Update(dt);

	std::vector<GameObject*>::const_iterator child = object->GetChilds()->begin();
	for (child; child != object->GetChilds()->end(); ++child)
	{
		UpdateGameObjects(dt, (*child));
	}
	

}

void ModuleGOManager::PreUpdateGameObjects(GameObject * obj)
{
	if (root != obj && obj->IsActive() == true)
		obj->PreUpdate();

	std::vector<GameObject*>::const_iterator child = obj->GetChilds()->begin();
	for (child; child != obj->GetChilds()->end(); ++child)
	{
		PreUpdateGameObjects((*child));
	}
}
