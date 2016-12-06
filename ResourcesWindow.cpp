#include "ResourcesWindow.h"

ResourcesWindow::ResourcesWindow()
{}

ResourcesWindow::~ResourcesWindow()
{}

void ResourcesWindow::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Resources", &active);



	ImGui::End();
}

