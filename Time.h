#ifndef __TIME_H__
#define __TIME_H__

#include <cstdint>

class Time
{
public:
	Time();
	~Time();

	double RealTimeSinceStartup(); //In seconds
	void Play();
	void Stop();
	void Pause();
	double TimeSinceGameStartup();

private:
	//Real time clock
	uint64_t	started_at = 0;
	static uint64_t frequency;

	//Game time clock
	uint64_t game_started_at = 0;
	bool game_paused = false;
	uint64_t pause_started_at = 0;

};
#endif
