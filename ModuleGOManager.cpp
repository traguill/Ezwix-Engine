#include "Application.h"
#include "ModuleGOManager.h"
#include "Component.h"
#include "GameObject.h"
#include "Imgui\imgui.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "RaycastHit.h"
#include <algorithm>

ModuleGOManager::ModuleGOManager(const char* name, bool start_enabled) : Module(name, start_enabled)
{}

ModuleGOManager::~ModuleGOManager()
{
	if (root)
		delete root;

	selected_GO = nullptr;
}

bool ModuleGOManager::Init(Data & config)
{
	//Load last open scene
	const char* path = config.GetString("current_scene_path");
	bool scene_success = false;
	if (path)
	{
		if (strcmp(path, "") != 0)
		{
			current_scene_path = path; //The scene is loaded at start because OpenGL needs to be init at ModuleRender
			scene_success = true;
		}
	}

	if (!scene_success)
	{
		LoadEmptyScene();
	}
	
	return true;
}

bool ModuleGOManager::Start()
{
	//Load last scene 
	if (root == nullptr)
	{
		if (LoadScene(current_scene_path.data()) == false)
		{
			LoadEmptyScene();
		}
	}

	return true;
}

update_status ModuleGOManager::PreUpdate()
{
	//Remove all GameObjects that needs to be erased
	for (vector<GameObject*>::iterator go = go_to_remove.begin(); go != go_to_remove.end(); ++go)
		delete (*go);

	go_to_remove.clear();

	if (root)
		PreUpdateGameObjects(root);

	return UPDATE_CONTINUE;
}

update_status ModuleGOManager::Update()
{
	//Update GameObjects
	if(root)
		UpdateGameObjects(time->DeltaTime(), root);

	//Display windows
	HierarchyWindow();
	InspectorWindow();

	PickObjects();

	//Selected Object shows it's boudning box
	if (selected_GO)
	{
		if (selected_GO->bounding_box)
		{
			g_Debug->AddAABB(*selected_GO->bounding_box, g_Debug->green);
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleGOManager::SaveBeforeClosing(Data& data) const
{
	data.AppendString("current_scene_path", current_scene_path.data());
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
		if(object->GetParent() != nullptr)
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

		root->Save(root_node);

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

bool ModuleGOManager::LoadScene(const char * file_path) 
{
	bool ret = false;
	//TODO: Now the current scene is destroyed. Ask the user if wants to save the changes.

	char* buffer = nullptr;
	uint size = App->file_system->Load(file_path, &buffer);
	if (size == 0)
	{
		LOG("Error while loading Scene: %s", file_path);
		if(buffer)
			delete[] buffer;
		return false;
	}

	Data scene(buffer);
	Data root_objects;
	root_objects = scene.GetArray("GameObjects", 0);

	if (root_objects.IsNull() == false)
	{
		//Remove the current scene
		ClearScene();
		
		for (int i = 0; i < scene.GetArraySize("GameObjects"); i++)
		{
			if(i == 0)
				root = LoadGameObject(scene.GetArray("GameObjects", i)); //First GO is always the root
			else
				LoadGameObject(scene.GetArray("GameObjects", i));
		}	
		current_scene_path = file_path;
		ret = true;
	}
	else
	{
		LOG("The scene %s is not a valid scene file", file_path);
		
	}

	delete[] buffer;

	return ret;
}

void ModuleGOManager::LoadEmptyScene()
{
	ClearScene();

	//Empty scene
	root = new GameObject();
	root->AddComponent(C_TRANSFORM);
	root->name = "Root";
	current_scene_path = "";
}

bool ModuleGOManager::IsRoot(const GameObject * go) const
{
	bool ret = false;

	if (go)
	{
		if (go == root)
			ret = true;
	}

	return ret;
}

void ModuleGOManager::PickObjects()
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP)
	{
		Ray ray = App->camera->GetCurrentCamera()->CastCameraRay(float2(App->input->GetMouseX(), App->input->GetMouseY()));
		selected_GO = Raycast(ray);
	}
}

void ModuleGOManager::SaveSceneBeforeRunning()
{
	Data root_node;
	root_node.AppendArray("GameObjects");

	root->Save(root_node);

	char* buf;
	size_t size = root_node.Serialize(&buf);

	App->file_system->Save("Library/current_scene.json", buf, size); //TODO: Find the right place to save the scene.

	delete[] buf;
}

void ModuleGOManager::LoadSceneBeforeRunning()
{
	LoadScene("Library/current_scene.json");
}

void ModuleGOManager::ClearScene()
{
	RemoveGameObject(root);
	selected_GO = nullptr;
	root = nullptr;
}

GameObject * ModuleGOManager::LoadGameObject(const Data & go_data) const
{
	const char* name = go_data.GetString("name");
	unsigned int uuid = go_data.GetUInt("UUID");
	unsigned int uuid_parent = go_data.GetUInt("parent");
	bool active = go_data.GetBool("active");

	//Find parent GameObject reference
	GameObject* parent = nullptr;
	if (uuid_parent != 0 && root)
	{
		parent = FindGameObjectByUUID(root, uuid_parent);
	}

	//Basic GameObject properties
	GameObject* go = new GameObject(name, uuid, parent, active);

	if(parent)
		parent->AddChild(go);
	
	//Components
	Data component;
	unsigned int comp_size = go_data.GetArraySize("components");
	for (int i = 0; i < comp_size; i++)
	{
		component = go_data.GetArray("components", i);

		int type = component.GetInt("type");

		Component* go_component = go->AddComponent(static_cast<ComponentType>(type));
		go_component->Load(component);
	}

	return go;
}

GameObject * ModuleGOManager::FindGameObjectByUUID(GameObject* start, unsigned int uuid) const
{
	GameObject* ret = nullptr;
	if (start)
	{
		if (start->GetUUID() == uuid)
		{
			ret = start;
		}
		else
		{
			const std::vector<GameObject*>* childs = start->GetChilds();
			for (std::vector<GameObject*>::const_iterator child = childs->begin(); child != childs->end(); ++child)
			{
				ret = FindGameObjectByUUID((*child), uuid);
				if (ret != nullptr)
					break;
			}
		}
	}
	
	return ret;
}

//Sort the AABBs for the distance from the current camera
int  CompareAABB(const void * a, const void * b)
{
	float3 cam_pos = App->camera->GetPosition();
	float a_dst = (cam_pos - ((GameObject*)a)->bounding_box->CenterPoint()).Length();
	float b_dst = (cam_pos - ((GameObject*)b)->bounding_box->CenterPoint()).Length();
	if (a_dst < b_dst) return -1;
	if (a_dst = b_dst) return 0;
	if (a_dst > b_dst) return 1;
}

GameObject * ModuleGOManager::Raycast(const Ray & ray) const
{
	GameObject* ret = nullptr;

	vector<GameObject*> collisions;
	FillIntersectionList(root, ray, collisions); //with AABBs

	std::sort(collisions.begin(), collisions.end(), CompareAABB);

	vector<GameObject*>::iterator it = collisions.begin(); //Test with vertices
	RaycastHit hit;
	while (it != collisions.end())
	{
		if ((*it)->RayCast(ray, hit))
		{
			ret = (*it);
			break;
		}
		++it;
	}

	return ret;
}

void ModuleGOManager::FillIntersectionList(GameObject * obj, const Ray & ray, vector<GameObject*>& list) const
{
	if (obj->bounding_box) 
	{
		if (ray.Intersects(*obj->bounding_box))
		{
			list.push_back(obj);
		}
	}

	const vector<GameObject*>* childs = obj->GetChilds();
	for (vector<GameObject*>::const_iterator child = childs->begin(); child != childs->end(); child++)
	{
		FillIntersectionList(*child, ray, list);
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

		//Static
		ImGui::SameLine();
		ImGui::Text("Static:");
		ImGui::SameLine();
		bool is_static = selected_GO->IsStatic();
		if (ImGui::Checkbox("###static_option", &is_static))
		{
			selected_GO->SetStatic(is_static);
		}

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
