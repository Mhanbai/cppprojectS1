#include "racetrack.h"

RaceTrack::RaceTrack()
{
	for (ModelClass* road : raceTrack) {
		road = 0;
	}
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

	for (int i = 0; i < r_straightNo; i++) {
		result = m_Graphics->AddToPipeline(raceTrack[i], hwnd, "../Engine/data/r_straight.txt", L"../Engine/data/road.dds");
		if (!result) {
			return false;
		}
	}

	for (int i = r_straightNo; i < (r_straightNo + r_curveNo); i++) {
		result = m_Graphics->AddToPipeline(raceTrack[i], hwnd, "../Engine/data/r_longcurveright.txt", L"../Engine/data/road.dds");
		if (!result) {
			return false;
		}
	}

	for (int i = (r_straightNo + r_curveNo); i < (r_straightNo + r_curveNo + r_sharpCurveNo); i++) {
		result = m_Graphics->AddToPipeline(raceTrack[i], hwnd, "../Engine/data/r_sharpcurve.txt", L"../Engine/data/road.dds");
		if (!result) {
			return false;
		}
	}

	for (int i = (r_straightNo + r_curveNo + r_sharpCurveNo); i < (r_straightNo + r_curveNo + r_sharpCurveNo + r_bendNo); i++) {
		result = m_Graphics->AddToPipeline(raceTrack[i], hwnd, "../Engine/data/r_sbendright.txt", L"../Engine/data/road.dds");
		if (!result) {
			return false;
		}
	}

	raceTrack[0]->SetPosition	(0.0f,		4.0f,	0.0f); //Straight
	raceTrack[1]->SetPosition	(0.0f,		4.0f,	60.0f);
	raceTrack[2]->SetPosition	(0.0f,		4.0f,	120.0f);
	raceTrack[3]->SetPosition	(0.0f,		4.0f,	180.0f);
	raceTrack[4]->SetPosition	(0.0f,		4.0f,	240.0f);
	raceTrack[5]->SetPosition	(0.0f,		4.0f,	300.0f);
	raceTrack[6]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[7]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[8]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[9]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[10]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[11]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[12]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[13]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[14]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[15]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[16]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[17]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[18]->SetPosition	(1000.0f,	4.0f,	0.0f); //Curve
	raceTrack[19]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[20]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[21]->SetPosition	(23.17f,	4.0f,	366.82f); raceTrack[21]->SetRotation(270.0f); //Sharp Curve
	raceTrack[22]->SetPosition	(216.5f,    4.0f,   353.3f); raceTrack[22]->SetRotation(90.0f);
	raceTrack[23]->SetPosition	(216.5f, 4.0f, 353.3f); raceTrack[23]->SetRotation(270.0f);
	raceTrack[24]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[25]->SetPosition	(1000.0f,	4.0f,	0.0f);
	raceTrack[26]->SetPosition	(119.9f,	4.0f,	361.0f); raceTrack[26]->SetRotation(270.0f); //Bend
	raceTrack[27]->SetPosition	(1000.0f,	4.0f,	0.0f);


	return true;
}

void RaceTrack::Shutdown()
{
	for (ModelClass* road : raceTrack) {
		road = 0;
	}
}
