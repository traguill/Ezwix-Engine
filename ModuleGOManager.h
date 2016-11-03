#ifndef __MODULE_GO_MANAGER_H__
#define __MODULE_GO_MANAGER_H__

#include "Module.h"
#include <vector>

class GameObject;
class ComponentCamera;

class ModuleGOManager : public Module
{
public:
	ModuleGOManager(const char* name, bool start_enabled = true);
	~ModuleGOManager();

	bool Init(Data& config);
	bool Start(); 
	update_status PreUpdate(float dt);
	update_status Update(float dt);

	GameObject* CreateGameObject(GameObject* parent);
	bool RemoveGameObject(GameObject* object);

	void GetAllCameras(std::vector<ComponentCamera*>& list, GameObject* from = nullptr) const;

	void SaveScene()const;

private:

	void HierarchyWindow();
	void DisplayGameObjectsChilds(const std::vector<GameObject*>* childs);

	void InspectorWindow();

	void UpdateGameObjects(float dt, GameObject* obj);
	void PreUpdateGameObjects(GameObject* obj);
private:
	GameObject* root = nullptr;

	GameObject* selected_GO = nullptr;

	vector<GameObject*> go_to_remove;

};

#endif // !__MODULE_GO_MANAGER_H__

