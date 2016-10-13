#include "Component.h"
#include "GameObject.h"
#include "Globals.h"

Component::Component(ComponentType type, GameObject* game_object) : type (type), game_object(game_object)
{}

Component::~Component()
{}

void Component::OnInspector()
{}

ComponentType Component::GetType() const
{
	return type;
}

GameObject * Component::GetGameObject() const
{
	return game_object;
}

bool Component::IsActive()
{
	return active;
}

void Component::SetActive(bool value)
{
	active = value;
}

void Component::Update(float dt)
{
}

