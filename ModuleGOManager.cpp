#include "Application.h"
#include "ModuleGOManager.h"
#include "Component.h"
#include "GameObject.h"
#include "Imgui\imgui.h"
#include "ComponentCamera.h"

ModuleGOManager::ModuleGOManager(const char* name, bool start_enabled) : Module(name, start_enabled)
{}

ModuleGOManager::~ModuleGOManager()
{
	if(root)
		delete root;

	selected_GO = nullptr;
}

bool ModuleGOManager::Init(Data & config)
{
	root = new GameObject();
	root->AddComponent(C_TRANSFORM);
	return true;
}

bool ModuleGOManager::Start()
{

	return true;
}

update_status ModuleGOManager::PreUpdate(float dt)
{
	//Remove all GameObjects that needs to be erased
	for (vector<GameObject*>::iterator go = go_to_remove.begin(); go != go_to_remove.end(); ++go)
		delete (*go);

	go_to_remove.clear();

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
		
		go_to_remove.push_back(object);

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

void ModuleGOManager::SaveScene()const
{
	//Prompt Save Window  TODO: put all this code inside a Windows class or a method.
	char file_name_final[512];
	OPENFILENAME sfn;

	ZeroMemory(&sfn, sizeof(sfn));
	sfn.lStructSize = sizeof(sfn);
	sfn.hwndOwner = NULL;
	sfn.lpstrFile = file_name_final;
	sfn.lpstrFile[0] = '\0';
	sfn.nMaxFile = sizeof(file_name_final);
	sfn.lpstrFilter = "(*.json)";
	sfn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK;

	char* base_path = SDL_GetBasePath();
	std::string base = base_path;
	//Remove /Debug/ and replace it for/Game/
	#if _DEBUG
	base.erase(base.length() - 7, base.length());
	base.append("\\Game\\");
	#endif
	base.append("Assets\\");
	sfn.lpstrInitialDir = base.data();
	SDL_free(base_path);

	if (GetSaveFileName(&sfn))
	{
		string name_to_save = sfn.lpstrFile;
		if (name_to_save.find(base, 0) != string::npos) //User selected a directory inside the assets folder
		{
			name_to_save.erase(0, base.length()); //Remove the directory from C://User//etc... Name_to_save starts from Assets folder
		}

		//Save the scene ---------------------------------------------------------------------------------------------------
		Data root_node;
		root_node.AppendArray("GameObjects");

		const vector<GameObject*>* childs = root->GetChilds();
		for (vector<GameObject*>::const_iterator gameobjects = childs->begin(); gameobjects != childs->end(); ++gameobjects)
			(*gameobjects)->Save(root_node);

		char* buf;
		size_t size = root_node.Serialize(&buf);

		//Add extension if doesn't have it yet
		if(name_to_save.find(".json", name_to_save.length() - 6) == string::npos)
			name_to_save += ".json";

		App->file_system->Save(name_to_save.data(), buf, size);

		delete[] buf;
		//------------------------------------------------------------------------------------------------------------------
	}
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
