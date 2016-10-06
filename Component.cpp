#include "Component.h"
#include "GameObject.h"
#include "Globals.h"

Component::Component(ComponentType type) : type (type)
{}

Component::~Component()
{}

void Component::OnInspector()
{}

ComponentType Component::GetType()
{
	return type;
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