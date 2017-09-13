#ifndef FACE_DETECTOR_H
#define FACE_DETECTOR_H

#include "../Detector.h"

class FaceDetector : public Detector
{
public:
	inline bool Init() { return true; }

	bool Detect(float* x, float* y);
};

#endif // FACE_DETECTOR_H