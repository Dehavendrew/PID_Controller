#include "dot.h"

dot::dot(olc::Pixel p, int x, int y) {
	dot::color = p;
	dot::nTargetPointX = x;
	dot::nTargetPointY = y;
};