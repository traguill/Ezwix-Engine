#include "Time.h"
#include "SDL\include\SDL_timer.h"

uint64_t Time::frequency = 0;

Time::Time()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();
	started_at = SDL_GetPerformanceCounter();
}

Time::~Time()
{}

double Time::RealTimeSinceStartup()
{
	return (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

void Time::Play()
{
	game_paused = false;
	game_started_at = SDL_GetPerformanceCounter();
	game_started_at = 0;
}

void Time::Stop()
{
	game_paused = false;
	game_started_at = 0;
}

void Time::Pause()
{
	game_paused = true;
	pause_started_at = SDL_GetPerformanceCounter();
}

double Time::TimeSinceGameStartup()
{
	return (double(SDL_GetPerformanceCounter() - game_started_at) / double(frequency));
}




