#ifndef __MODULE_GO_MANAGER_H__
#define __MODULE_GO_MANAGER_H__

#include "Module.h"
#include <vector>
#include <list>
#include <string>
#include "Octree.h"

class GameObject;
class ComponentCamera;

#define OCTREE_SIZE 50

class ModuleGOManager : public Module
{
public:
	ModuleGOManager(const char* name, bool start_enabled = true);
	~ModuleGOManager();

	bool Init(Data& config);
	bool Start(); 
	update_status PreUpdate();
	update_status Update();
	void SaveBeforeClosing(Data& data)const;

	GameObject* CreateGameObject(GameObject* parent);
	bool RemoveGameObject(GameObject* object);

	void GetAllCameras(std::vector<ComponentCamera*>& list, GameObject* from = nullptr) const;

	void SaveScene()const;
	bool LoadScene(const char* file_path); //Removes the current scene
	void LoadEmptyScene();
	void LoadMeshGameObject(const Data& go_data);

	bool IsRoot(const GameObject* go)const;

	void PickObjects();

	void SaveSceneBeforeRunning();//Saves the scene before running the game
	void LoadSceneBeforeRunning();

	//Handles the insertion / remove of the octree and dynamic gameobjects list. TODO: Rename the methods. Look confusing.
	bool InsertGameObjectInOctree(GameObject* go);
	bool RemoveGameObjectOfOctree(GameObject* go);

private:

	void HierarchyWindow();
	void DisplayGameObjectsChilds(const std::vector<GameObject*>* childs);

	void InspectorWindow();

	void UpdateGameObjects(float dt, GameObject* obj);
	void PreUpdateGameObjects(GameObject* obj);

	void ClearScene(); //Removes the current scene

	GameObject* LoadGameObject(const Data& go_data);

	GameObject* FindGameObjectByUUID(GameObject* start, unsigned int uuid)const; //Should be a public method?

	GameObject* Raycast(const Ray& ray)const;
	void FillIntersectionList(GameObject* obj, const Ray& ray, vector<GameObject*>& list)const;

private:
	GameObject* root = nullptr;

	GameObject* selected_GO = nullptr;
	vector<GameObject*> go_to_remove;

	string current_scene_path = "";

public: 
	//GameObjects TODO:Add functionallity to make it private
	Octree<GameObject*> octree; //Static
	list<GameObject*> dynamic_gameobjects;

};

#endif // !__MODULE_GO_MANAGER_H__

