#include "FPSGraph.h"

FPSGraph::FPSGraph()
{}

FPSGraph::~FPSGraph()
{}

void FPSGraph::Draw()
{
	if (!active)
		return;

	ImGui::Begin("FPS Graph", &active);
	ImGui::Text("hi");
	ImGui::End();
}


