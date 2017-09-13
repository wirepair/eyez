#ifndef BODY_DETECTOR_H
#define BODY_DETECTOR_H

#include "../Detector.h"

class BodyDetector : public Detector
{
public:
	inline bool Init() { return true; }

	bool Detect(float* x, float* y);
};

#endif // BODY_DETECTOR_H