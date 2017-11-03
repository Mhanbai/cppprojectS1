#include "racetrack.h"

RaceTrack::RaceTrack()
{
	raceTrack = 0;
}

RaceTrack::RaceTrack(const RaceTrack &)
{
}

RaceTrack::~RaceTrack()
{
}

bool RaceTrack::Initialize(GraphicsClass *& graphics, HWND & hwnd)
{
	bool result;
	m_Graphics = graphics;

	result = m_Graphics->AddToPipeline(raceTrack, hwnd, "../Engine/data/racetrack.txt", L"../Engine/data/road.dds");
	if (!result) {
		return false;
	}

	raceTrack->SetPosition	(0.0f, 4.0f, 0.0f);

	return true;
}

void RaceTrack::Shutdown()
{
	raceTrack = 0;
}
