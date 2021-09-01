#define OLC_PGE_APPLICATION
#include <iostream>
#include "C:\Users\Drew\Desktop\olcPixelGameEngine-master\olcPixelGameEngine.h"
#include "dot.h"

class Example : public olc::PixelGameEngine
{

	int nHeight = 0;
	int nWidth = 0;
	int nGraphHeight = 0;
	int npixelW = 10;

	std::vector<dot> dots;
	std::vector<olc::Pixel> colors = {
		olc::Pixel(255, 0, 0),
		olc::Pixel(0, 255, 0),
		olc::Pixel(0, 0, 255),
		olc::Pixel(255, 255, 0)
	};
	int nNumDots = 4;
	int nSelectedDot = 0;

	int nStep = 0;
	int clk = 0;
	int nClockDiv = 4;

	olc::Pixel pixel;
	olc::Pixel blank;
	int nTextBoundary = 0;

	int nPrevError = 0;
	int nPrevErrorY = 0;

	bool bDotGrabbed = false;
	

public:

	Example()
	{
		sAppName = "Driver Environment";
	}

	void Reset() {
		FillRect(0, 0, nWidth, ScreenHeight(), olc::Pixel(0,0,0));

		for (int i = 0; i < nNumDots; ++i) {
			dots[i].nDotPositionX = 0;
			dots[i].nDotPositionY = 0;
			dots[i].nDotDirectionX = 1;
			dots[i].nDotDirectionY = 1;
			dots[i].nDotVelocityX = 0;
			dots[i].nDotVelocityY = 0;
			dots[i].fTotalGainX = 0.0;
			dots[i].fTotalGainY = 0.0;

			DrawLine(olc::vi2d(dots[i].nTargetPointX, 0), olc::vi2d(dots[i].nTargetPointX, nHeight), olc::Pixel(100, 100, 100));
			DrawLine(olc::vi2d(0, dots[i].nTargetPointY), olc::vi2d(nWidth, dots[i].nTargetPointY), olc::Pixel(100, 100, 100));
		}


		clk = 0;
		nStep = 0;

		ClearPlot();
		DrawLine(olc::vi2d(0, ScreenHeight() - nGraphHeight / 2), olc::vi2d(nWidth, ScreenHeight() - nGraphHeight / 2), olc::Pixel(100, 100, 100));
		FillRect(0, nHeight, nWidth, npixelW / 2, olc::Pixel(255, 255, 255));
		UpdateText();
	}

	void ClearText() {
		DrawString(nTextBoundary, 10, "Game Delay: " + std::to_string(nClockDiv), olc::BLACK);
		DrawString(nTextBoundary, 20, "Sample Delay: " + std::to_string(dots[nSelectedDot].nSampleRate), olc::BLACK);
		DrawString(nTextBoundary, 30, "P Gain: " + std::to_string(dots[nSelectedDot].fPGain), olc::BLACK);
		DrawString(nTextBoundary, 40, "I Gain: " + std::to_string(dots[nSelectedDot].fIGain), olc::BLACK);
		DrawString(nTextBoundary, 50, "D Gain: " + std::to_string(dots[nSelectedDot].fDGain), olc::BLACK);
	}

	void UpdateText() {
		DrawString(nTextBoundary, 10, "Game Delay: " + std::to_string(nClockDiv), olc::WHITE);
		DrawString(nTextBoundary, 20, "Sample Delay: " + std::to_string(dots[nSelectedDot].nSampleRate), olc::WHITE);
		DrawString(nTextBoundary, 30, "P Gain: " + std::to_string(dots[nSelectedDot].fPGain), olc::WHITE);
		DrawString(nTextBoundary, 40, "I Gain: " + std::to_string(dots[nSelectedDot].fIGain), olc::WHITE);
		DrawString(nTextBoundary, 50, "D Gain: " + std::to_string(dots[nSelectedDot].fDGain), olc::WHITE);
	}

	void ClearPlot() {
		FillRect(0, nHeight + npixelW, nWidth, nGraphHeight, olc::Pixel(0, 0, 0));
		DrawLine(olc::vi2d(0, ScreenHeight() - nGraphHeight / 2), olc::vi2d(nWidth, ScreenHeight() - nGraphHeight / 2), olc::Pixel(100, 100, 100));
		FillRect(0, nHeight, nWidth, npixelW / 2, olc::Pixel(255, 255, 255));
	}

	void PlotError() {
		int nXError = dots[nSelectedDot].nTargetPointX - dots[nSelectedDot].nDotPositionX;
		int nYError = dots[nSelectedDot].nTargetPointY - dots[nSelectedDot].nDotPositionY;
		int maxError = 160;
		int minError = -1 * maxError;

		if (nXError >= maxError) {
			nXError = maxError;
		}
		if (nYError >= maxError) {
			nYError = maxError;
		}
		if (nXError <= minError) {
			nXError = minError;
		}
		if (nYError <= minError) {
			nYError = minError;
		}

		int xPlotPoint1 = nHeight + (nGraphHeight / 2) + ((nGraphHeight * nXError / maxError)) / 2 - 1;
		int xPlotPoint2 = nHeight + (nGraphHeight / 2) + ((nGraphHeight * nPrevError / maxError)) / 2 - 1;

		int yPlotPoint1 = nHeight + (nGraphHeight / 2) + ((nGraphHeight * nYError / maxError)) / 2 - 1;
		int yPlotPoint2 = nHeight + (nGraphHeight / 2) + ((nGraphHeight * nPrevErrorY / maxError)) / 2 - 1;


		if (clk % nWidth == 0) {
			ClearPlot();
			Draw(clk % nWidth, xPlotPoint1, olc::Pixel(0, 0, 255));
			Draw(clk % nWidth, yPlotPoint1, olc::Pixel(255, 255, 0));
			nPrevError = nXError;
			nPrevErrorY = nYError;

		}
		else {
			DrawLine(olc::vi2d((clk % nWidth) - 1,  xPlotPoint2), olc::vi2d(clk % nWidth, xPlotPoint1), olc::Pixel(0, 0, 255));
			DrawLine(olc::vi2d((clk % nWidth) - 1,  yPlotPoint2), olc::vi2d(clk % nWidth, yPlotPoint1), olc::Pixel(255,255,0));
			nPrevError = nXError;
			nPrevErrorY = nYError;
		}
	}


	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		nGraphHeight = ScreenHeight() / 4;
		nHeight = ScreenHeight() - nGraphHeight;
		nWidth = ScreenWidth();

		blank = olc::Pixel(0, 0, 0);

		for (int i = 0; i < nNumDots; ++i) {
			dot d1 = dot(colors[i], (0.1 * nWidth / nNumDots) + i * (nWidth / nNumDots), nHeight / 2);
			dots.push_back(d1);

			DrawLine(olc::vi2d(0, dots[i].nTargetPointY), olc::vi2d(nWidth, dots[i].nTargetPointY), olc::Pixel(100, 100, 100));
			DrawLine(olc::vi2d(dots[i].nTargetPointX, 0), olc::vi2d(dots[i].nTargetPointX, nHeight), olc::Pixel(100, 100, 100));
		}
		DrawLine(olc::vi2d(0, ScreenHeight() - nGraphHeight / 2), olc::vi2d(nWidth, ScreenHeight() - nGraphHeight / 2), olc::Pixel(100, 100, 100));
		FillRect(0, nHeight, nWidth, npixelW / 2, olc::Pixel(255, 255, 255));

		nTextBoundary = nWidth - 150;
		UpdateText();

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override{

		//drag and drop commands

		if (GetMouse(0).bPressed) {
			int xMouse = GetMouseX();
			int yMouse = GetMouseY();

			for (int i = 0; i < nNumDots; ++i) {
				if (xMouse >= dots[i].nDotPositionX && xMouse <= dots[i].nDotPositionX + npixelW && yMouse >= dots[i].nDotPositionY && yMouse <= dots[i].nDotPositionY + npixelW) {
					ClearText();
					bDotGrabbed = true;
					nSelectedDot = i;
					UpdateText();
				}
			}
		}

		if (GetMouse(0).bReleased) {
			bDotGrabbed = false;
		}

		if (GetMouse(0).bHeld) {

			if (bDotGrabbed) {

				FillRect(dots[nSelectedDot].nDotPositionX, dots[nSelectedDot].nDotPositionY, npixelW, npixelW, blank);
				dots[nSelectedDot].nDotPositionX = GetMouseX();
				dots[nSelectedDot].nDotPositionY = GetMouseY();
				FillRect(dots[nSelectedDot].nDotPositionX, dots[nSelectedDot].nDotPositionY, npixelW, npixelW, dots[nSelectedDot].color);

			}
			
		}


		//User Controls
		if (GetKey(olc::Key::UP).bHeld && GetKey(olc::Key::S).bPressed) {
			ClearText();
			++dots[nSelectedDot].nSampleRate;
			UpdateText();
		}
		if (GetKey(olc::Key::DOWN).bHeld && GetKey(olc::Key::S).bPressed) {
			ClearText();
			if (dots[nSelectedDot].nSampleRate > 1) --dots[nSelectedDot].nSampleRate;
			UpdateText();
		}
		if (GetKey(olc::Key::UP).bHeld && GetKey(olc::Key::C).bPressed) {
			ClearText();
			++nClockDiv;
			UpdateText();
		}
		if (GetKey(olc::Key::DOWN).bHeld && GetKey(olc::Key::C).bPressed) {
			ClearText();
			if(nClockDiv > 1) --nClockDiv;
			UpdateText();
		}
		if (GetKey(olc::Key::UP).bHeld && GetKey(olc::Key::P).bPressed) {
			ClearText();
			dots[nSelectedDot].fPGain = dots[nSelectedDot].fPGain + 0.01;
			UpdateText();
		}
		if (GetKey(olc::Key::DOWN).bHeld && GetKey(olc::Key::P).bPressed) {
			ClearText();
			dots[nSelectedDot].fPGain = dots[nSelectedDot].fPGain - 0.01;
			UpdateText();
		}
		if (GetKey(olc::Key::UP).bHeld && GetKey(olc::Key::I).bPressed) {
			ClearText();
			dots[nSelectedDot].fIGain = dots[nSelectedDot].fIGain + 0.01;
			UpdateText();
		}
		if (GetKey(olc::Key::DOWN).bHeld && GetKey(olc::Key::I).bPressed) {
			ClearText();
			dots[nSelectedDot].fIGain = dots[nSelectedDot].fIGain - 0.01;
			UpdateText();
		}
		if (GetKey(olc::Key::UP).bHeld && GetKey(olc::Key::D).bPressed) {
			ClearText();
			dots[nSelectedDot].fDGain = dots[nSelectedDot].fDGain + 0.01;
			UpdateText();
		}
		if (GetKey(olc::Key::DOWN).bHeld && GetKey(olc::Key::D).bPressed) {
			ClearText();
			dots[nSelectedDot].fDGain = dots[nSelectedDot].fDGain - 0.01;
			UpdateText();
		}
		if (GetKey(olc::Key::UP).bHeld && GetKey(olc::Key::X).bHeld){
			DrawLine(olc::vi2d(dots[nSelectedDot].nTargetPointX, 0), olc::vi2d(dots[nSelectedDot].nTargetPointX, nHeight), blank);
			++dots[nSelectedDot].nTargetPointX;
			DrawLine(olc::vi2d(dots[nSelectedDot].nTargetPointX, 0), olc::vi2d(dots[nSelectedDot].nTargetPointX, nHeight), olc::Pixel(100, 100, 100));
		}
		if (GetKey(olc::Key::DOWN).bHeld && GetKey(olc::Key::X).bHeld) {
			DrawLine(olc::vi2d(dots[nSelectedDot].nTargetPointX, 0), olc::vi2d(dots[nSelectedDot].nTargetPointX, nHeight), blank);
			--dots[nSelectedDot].nTargetPointX;
			DrawLine(olc::vi2d(dots[nSelectedDot].nTargetPointX, 0), olc::vi2d(dots[nSelectedDot].nTargetPointX, nHeight), olc::Pixel(100, 100, 100));
		}
		if (GetKey(olc::Key::UP).bHeld && GetKey(olc::Key::Y).bHeld) {
			DrawLine(olc::vi2d(0, dots[nSelectedDot].nTargetPointY), olc::vi2d(nWidth, dots[nSelectedDot].nTargetPointY), blank);
			++dots[nSelectedDot].nTargetPointY;
			DrawLine(olc::vi2d(0, dots[nSelectedDot].nTargetPointY), olc::vi2d(nWidth, dots[nSelectedDot].nTargetPointY), olc::Pixel(100, 100, 100));
		}
		if (GetKey(olc::Key::DOWN).bHeld && GetKey(olc::Key::Y).bHeld) {
			DrawLine(olc::vi2d(0, dots[nSelectedDot].nTargetPointY), olc::vi2d(nWidth, dots[nSelectedDot].nTargetPointY), blank);
			--dots[nSelectedDot].nTargetPointY;
			DrawLine(olc::vi2d(0, dots[nSelectedDot].nTargetPointY), olc::vi2d(nWidth, dots[nSelectedDot].nTargetPointY), olc::Pixel(100, 100, 100));
		}
		if (GetKey(olc::Key::R).bPressed) {
			Reset();
		}


		//Statistics
		PlotError();

		//Update Logic
		if(nStep % nClockDiv == 0){

			//Keep middle line drawn
			for (int i = 0; i < nNumDots; ++i) {
				DrawLine(olc::vi2d(dots[i].nTargetPointX, 0), olc::vi2d(dots[i].nTargetPointX, nHeight), olc::Pixel(100, 100, 100));
				DrawLine(olc::vi2d(0, dots[i].nTargetPointY), olc::vi2d(nWidth, dots[i].nTargetPointY), olc::Pixel(100, 100, 100));



				if (clk % dots[i].nSampleRate == 0 && !bDotGrabbed) {

					//Feedback Contrller

					int errorX = dots[i].nTargetPointX - dots[i].nDotPositionX;
					int errorY = dots[i].nTargetPointY - dots[i].nDotPositionY;

					dots[i].nIErrorX = dots[i].nIErrorX + errorX;
					dots[i].nIErrorY = dots[i].nIErrorY + errorY;

					float diffX = (errorX + dots[i].nDPrevErrorX) / 2 - (dots[i].nDPrevPrevErrorX + dots[i].nDPrevErrorX) / 2;
					float diffY = (errorY + dots[i].nDPrevErrorY) / 2 - (dots[i].nDPrevPrevErrorY + dots[i].nDPrevErrorY) / 2;

					DrawString(nTextBoundary, 60, "Gain X: " + std::to_string(dots[nSelectedDot].fTotalGainX), olc::BLACK);
					DrawString(nTextBoundary, 70, "Gain Y: " + std::to_string(dots[nSelectedDot].fTotalGainY), olc::BLACK);

					dots[i].fTotalGainX = dots[i].fPGain * errorX + dots[i].fIGain * dots[i].nIErrorX + dots[i].fDGain * diffX;
					dots[i].fTotalGainY = dots[i].fPGain * errorY + dots[i].fIGain * dots[i].nIErrorY + dots[i].fDGain * diffY;

					DrawString(nTextBoundary, 60, "Gain X: " + std::to_string(dots[nSelectedDot].fTotalGainX), olc::WHITE);
					DrawString(nTextBoundary, 70, "Gain Y: " + std::to_string(dots[nSelectedDot].fTotalGainY), olc::WHITE);

					dots[i].nDPrevErrorX = errorX;
					dots[i].nDPrevErrorY = errorY;

					dots[i].nDPrevPrevErrorX = dots[i].nDPrevErrorX;
					dots[i].nDPrevPrevErrorY = dots[i].nDPrevErrorY;

				}

				//Undraw Last Pixel
				//Draw(nDotPosition, nHeight / 2, blank);

				FillRect(dots[i].nDotPositionX, dots[i].nDotPositionY, npixelW, npixelW, blank);

				//Transfer function
				//nDotVelocityX = nDotVelocityX + fTotalGainX;
				//nDotVelocityY = nDotVelocityY + fTotalGainY;


				//State Update
				dots[i].nDotPositionX = dots[i].nDotPositionX + dots[i].fTotalGainX;
				dots[i].nDotPositionY = dots[i].nDotPositionY + dots[i].fTotalGainY;

				FillRect(dots[i].nDotPositionX, dots[i].nDotPositionY, npixelW, npixelW, dots[i].color);

			}

			++clk;
		}
		
		++nStep;
	
		return true;
	}

};

void show() {
	Example demo;

	if (demo.Construct(1200, 600, 1, 1))
		demo.Start();
}

int main() {

	show();

	return 0;
}

