#pragma once
#include "C:\Users\Drew\Desktop\olcPixelGameEngine-master\olcPixelGameEngine.h"

class dot
{
public:
	olc::Pixel color;
	int nTargetPointX = 0;
	int nTargetPointY = 0;
	int nSampleRate = 10;

	int nDotDirectionX = 1;
	int nDotDirectionY = 1;
	float nDotPositionX = 0;
	float nDotPositionY = 0;
	float nDotVelocityX = 0;
	float nDotVelocityY = 0;

	float fTotalGainX = 0;
	float fTotalGainY = 0;
	float fPGain = 0.03;
	float fIGain = 0.01;
	float fDGain = 0.01;

	int nIErrorX = 0;
	int nIErrorY = 0;
	int nDPrevErrorX = 0;
	int nDPrevErrorY = 0;
	int nDPrevPrevErrorX = 0;
	int nDPrevPrevErrorY = 0;

	dot(olc::Pixel p, int x, int y);
};