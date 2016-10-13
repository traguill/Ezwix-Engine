#ifndef __COMPONENT_H__
#define __COMPONENT_H__

enum ComponentType
{
	C_TRANSFORM,
	C_MESH,
	C_MATERIAL
};

class GameObject;

class Component
{
public:
	
	Component(ComponentType type, GameObject* game_object);
	virtual ~Component();

	virtual void Update(float dt);

	bool IsActive();
	void SetActive(bool value);
	virtual void OnInspector();

	ComponentType GetType()const;
	GameObject* GetGameObject()const;

private:
	bool active = true;
	ComponentType type;
	GameObject* game_object = nullptr;

};
#endif // !__COMPONENT_H__