#ifndef __MODULE_GO_MANAGER_H__
#define __MODULE_GO_MANAGER_H__

#include "Module.h"
#include <vector>
#include <string>

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
	void SaveBeforeClosing(Data& data)const;

	GameObject* CreateGameObject(GameObject* parent);
	bool RemoveGameObject(GameObject* object);

	void GetAllCameras(std::vector<ComponentCamera*>& list, GameObject* from = nullptr) const;

	void SaveScene()const;
	bool LoadScene(const char* file_path); //Removes the current scene
	void LoadEmptyScene();

	bool IsRoot(const GameObject* go)const;

	void PickObjects();

private:

	void HierarchyWindow();
	void DisplayGameObjectsChilds(const std::vector<GameObject*>* childs);

	void InspectorWindow();

	void UpdateGameObjects(float dt, GameObject* obj);
	void PreUpdateGameObjects(GameObject* obj);

	void ClearScene(); //Removes the current scene

	GameObject* LoadGameObject(const Data& go_data)const;

	GameObject* FindGameObjectByUUID(GameObject* start, unsigned int uuid)const; //Should be a public method?

	GameObject* Raycast(const Ray& ray)const;
	void FillIntersectionList(GameObject* obj, const Ray& ray, vector<GameObject*>& list)const;

private:
	GameObject* root = nullptr;

	GameObject* selected_GO = nullptr;
	vector<GameObject*> go_to_remove;

	string current_scene_path = "";

};

#endif // !__MODULE_GO_MANAGER_H__

