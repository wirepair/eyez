#ifndef TEST_DETECTOR_H
#define TEST_DETECTOR_H

#include "Detector.h"

class TestDetector : public Detector
{
	inline bool Init() { return true; }

	bool Detect(float* x, float* y);
};

#endif // TEST_DETECTOR_H