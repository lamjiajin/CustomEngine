/******************************************************************************/
/*!
\file		  FPSController.cpp
\author 	Liew York Quan Chester
\par    	email: yorkquan.liew\@digipen.edu
\date   	July 22, 2015
\brief    This program contains implementation of FrameRateController

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#include "FPSController.h"

#define SECOND_TO_MILLI 1000.0
#define SECOND_TO_MICRO 1000000
/******************************************************************************/
/*!
\brief
COnstructor
\return
Return nothing
*/
/******************************************************************************/
FPSController::FPSController() :
	m_max_fps(0),
	m_min_frame_time(0),
	m_prev_frame_duration(0),
	m_frame_count(0),
	m_time(0)
{
	//intentionally empty
}

/******************************************************************************/
/*!
\brief Initialise the FPSController

\param max_fps
frame rate limit

\return
Return nothing
*/
/******************************************************************************/
void FPSController::Init(double max_fps)
{
	m_max_fps = max_fps;
	m_min_frame_time = std::chrono::microseconds(unsigned(std::round(SECOND_TO_MICRO / max_fps)));

}
/******************************************************************************/
/*!
\brief
start tracking frame time
\return
Return nothing
*/
/******************************************************************************/
void FPSController::Start()
{
	m_curr_time = myClock::now();
}
/******************************************************************************/
/*!
\brief
delay time if frame ends early
\return
Return nothing
*/
/******************************************************************************/
void FPSController::End()
{
	do
	{
		m_new_time = myClock::now();
		m_prev_frame_duration = m_new_time - m_curr_time;
	} while (m_prev_frame_duration < m_min_frame_time);

	++m_frame_count;

}
/******************************************************************************/
/*!
\brief
resets values
\return
Return nothing
*/
/******************************************************************************/
void FPSController::Reset()
{
	m_frame_count = 0;
}
/******************************************************************************/
/*!
\brief
Get current Instance
\return
FPSController&
*/
/******************************************************************************/
FPSController& FPSController::GetInstance()
{
	static FPSController m_instance;
	return m_instance;
}
/******************************************************************************/
/*!
\brief
Get prev frame duration
\return
prev frame time
*/
/******************************************************************************/
double FPSController::GetFrameTime()
{
	// Getting in milliseconds
	return (double)std::chrono::duration_cast<microDura>(m_prev_frame_duration).count() / SECOND_TO_MILLI;// / SECOND_TO_MILLI;
}
/******************************************************************************/
/*!
\brief
get number of frames past
\return
number of frame
*/
/******************************************************************************/
unsigned FPSController::GetFrameCount()
{
	return m_frame_count;
}

