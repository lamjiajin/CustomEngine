/******************************************************************************/
/*!
\file		  FPSController.h
\author 	Liew York Quan Chester
\par    	email: yorkquan.liew\@digipen.edu
\date   	July 22, 2015
\brief    This program contains interface of FrameRateController

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#ifndef FPSCONTROLLER_H_
#define FPSCONTROLLER_H_

#include <ctime>
#include <chrono>
#include <vector>

class FPSController
{
public:

	void Init(double max_fps = 60);
	void Start();
	void End();
	void Reset();

	static    FPSController& GetInstance();
	double    GetFrameTime();
	unsigned  GetFrameCount();

private:
	typedef std::chrono::high_resolution_clock myClock;
	typedef myClock::duration dura;
	typedef myClock::time_point timePoint;
	typedef std::chrono::milliseconds milliDura;
	typedef std::chrono::microseconds microDura;

	FPSController();


	const long double m_time;

	double m_max_fps;
	dura m_min_frame_time;
	unsigned m_frame_count;
	dura m_prev_frame_duration;
	timePoint m_curr_time;
	timePoint m_new_time;
};

#endif


