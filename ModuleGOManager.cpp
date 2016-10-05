#include "Application.h"
#include "ModuleGOManager.h"
#include "GameObject.h"
#include "Imgui\imgui.h"

ModuleGOManager::ModuleGOManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root = new GameObject();
}

ModuleGOManager::~ModuleGOManager()
{
	if(root)
		delete root;
}

bool ModuleGOManager::Start()
{

	GameObject* a = CreateGameObject(NULL);
	a->name = "A";

	GameObject* b = CreateGameObject(NULL);
	b->name = "B";

	GameObject* c = CreateGameObject(a);
	c->name = "A";

	GameObject* d = CreateGameObject(a);
	d->name = "D";

	GameObject* e = CreateGameObject(a);
	e->name = "E";

	GameObject* f = CreateGameObject(d);
	f->name = "A";

	GameObject* g = CreateGameObject(b);
	a->name = "A";

	return true;
}

update_status ModuleGOManager::Update(float dt)
{
	
	//Display windows
	HierarchyWindow();

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

void ModuleGOManager::HierarchyWindow()
{
	ImGui::Begin("Hierarchy");

	DisplayGameObjectsChilds(root->GetChilds());

	ImGui::End();
}

void ModuleGOManager::DisplayGameObjectsChilds(const std::vector<GameObject*>* childs)
{
	for (vector<GameObject*>::const_iterator object = (*childs).begin(); object != (*childs).end(); ++object)
	{
		if ((*object)->ChildCount() > 0)
		{
			if (ImGui::TreeNode((*object)->name.data()))
			{
				DisplayGameObjectsChilds((*object)->GetChilds());
				ImGui::TreePop();
			}
		}
		else
		{
			ImGui::Bullet();
			ImGui::Selectable((*object)->name.data());
		}
	}
}
