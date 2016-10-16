#include "Profiler.h"
#include "AutoProfile.h"

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

		if ((*it).second.calls.size() - 1 < id)
		{
			(*it).second.calls.push_back(1);
			(*it).second.total_ms.push_back(time);
		}
		else
		{
			(*it).second.calls.back() += 1;
			(*it).second.total_ms.back() += time;
		}
	}
	else
	{
		ProfilerSample sample;
		sample.id = 0;
		sample.calls.push_back(1);
		sample.total_ms.push_back(time);
		
		samples.insert(pair<const char*, ProfilerSample>(name, sample));
	}
}

//Called every frame
void Profiler::Update()
{
	PROFILE("Profiler::Update");

	if (!is_playing)
		return;
	
	map<const char*, ProfilerSample>::iterator it = samples.begin();

	for (it; it != samples.end(); it++)
	{
		if ((*it).second.id == MAX_TIME_ITEMS - 1) //The queue is full
		{
			(*it).second.calls.pop_front();
			(*it).second.total_ms.pop_front();
		}
		else
		{
			(*it).second.id++;
		}
	}
}

void Profiler::Draw()
{
	PROFILE("Profiler::Draw");

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

			double total_ms;
			int calls;
			float self_time;
			if (current_frame > (*it).second.calls.size() - 1)
			{
				total_ms = 0;
				calls = 0;
				self_time = 0;
			}
			else
			{
				total_ms = (*it).second.total_ms.at(current_frame);
				calls = (*it).second.calls.at(current_frame);
				self_time = total_ms / calls;
			}

			ImGui::Text("%0.5f", total_ms);
			ImGui::NextColumn();

			ImGui::Text("%d", calls);
			ImGui::NextColumn();

			ImGui::Text("%0.5f", self_time);
			ImGui::NextColumn();
		}

		it++;
	}

	ImGui::End();
}
