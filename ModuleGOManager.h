#ifndef __MODULE_GO_MANAGER_H__
#define __MODULE_GO_MANAGER_H__

#include "Module.h"
#include <vector>

class GameObject;

class ModuleGOManager : public Module
{
public:
	ModuleGOManager(Application* app, bool start_enabled = true);
	~ModuleGOManager();

	bool Start(); //Testing
	update_status Update(float dt);

	GameObject* CreateGameObject(GameObject* parent);
	bool RemoveGameObject(GameObject* object);

private:

	void HierarchyWindow();
	void DisplayGameObjectsChilds(const std::vector<GameObject*>* childs);

	void InspectorWindow();

	void UpdateGameObjects(float dt, GameObject* obj);
private:
	GameObject* root = nullptr;

	GameObject* selected_GO = nullptr;

};

#endif // !__MODULE_GO_MANAGER_H__

