#include "Profiler.h"

Profiler::Profiler()
{}

Profiler::~Profiler()
{
	map<const char*, ProfilerSample>::iterator it = samples.begin();

	for (it; it != samples.end(); it++)
	{
		(*it).second.calls.clear();
		(*it).second.total_ms.clear();
	}

	samples.clear();
}

void Profiler::StoreSample(const char* name, double time)
{
	if (!is_playing)
		return;

	map<const char*, ProfilerSample>::iterator it = samples.find(name);

	if (it != samples.end())
	{
		unsigned int id = (*it).second.id;

		(*it).second.calls[id]++;
		(*it).second.total_ms[id] += time;
	}
	else
	{
		ProfilerSample sample;
		sample.calls.resize(MAX_TIME_ITEMS, 0);
		sample.total_ms.resize(MAX_TIME_ITEMS, 0);
		sample.id = 0;
		sample.calls[0] = 1;
		sample.total_ms[0] = time;
		
		samples.insert(pair<const char*, ProfilerSample>(name, sample));
	}
}

//Called every frame
void Profiler::Update()
{
	if (!is_playing)
		return;

	map<const char*, ProfilerSample>::iterator it = samples.begin();

	for (it; it != samples.end(); it++)
	{
		if ((*it).second.id == MAX_TIME_ITEMS - 1)
		{
			//Erase first and move to the left Reset last
			for(int i = 1; i < MAX_TIME_ITEMS; i++)
			{
				(*it).second.calls[i-1] = (*it).second.calls[i];
				(*it).second.total_ms[i - 1] = (*it).second.total_ms[i];
			}
			(*it).second.calls[MAX_TIME_ITEMS - 1] = 0;
			(*it).second.total_ms[MAX_TIME_ITEMS - 1] = 0;
		}
		else
		{
			(*it).second.id++;
		}
	}
}

void Profiler::Draw()
{
	if (!active)
		return;

	ImGui::Begin("Profiler", &active);

	ImGui::Checkbox("Play", &is_playing);

	ImGui::SliderInt("Frames", &current_frame, 0, MAX_TIME_ITEMS -1);

	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("0 is 600 frames back and 600 is the actual frame.");

	ImGui::Separator();
	static ImGuiTextFilter filter;
	filter.Draw("Search");
	ImGui::Separator();

	ImGui::Columns(4, "Profiler_table");

	ImGui::Text("Name"); ImGui::NextColumn();
	ImGui::Text("Total time(ms)"); ImGui::NextColumn();
	ImGui::Text("Calls"); ImGui::NextColumn();
	ImGui::Text("Self Time"); ImGui::NextColumn();
	ImGui::Separator();

	map<const char*, ProfilerSample>::iterator it = samples.begin();

	while (it != samples.end())
	{
		if (filter.PassFilter((*it).first))
		{
			ImGui::Text((*it).first);
			ImGui::NextColumn();

			ImGui::Text("%0.5f", (*it).second.total_ms[current_frame]);
			ImGui::NextColumn();

			ImGui::Text("%d", (*it).second.calls[current_frame]);
			ImGui::NextColumn();

			float self_time = (*it).second.total_ms[current_frame] / (*it).second.calls[current_frame];

			ImGui::Text("%0.5f", self_time);
			ImGui::NextColumn();
		}

		it++;
	}

	ImGui::End();
}
