#ifndef MAKE_DETECTOR_H
#define MAKE_DETECTOR_H

#include <string>
#include "Detector.h"

#ifdef OPENCV_DETECT
	#include "ocvface/FaceDetector.h"
	#include "ocvbody/BodyDetector.h"
#elif FWTECH_DETECT
	#include "fwtech/FaceWareDetector.h"
#elif TEST_DETECT
	#include "TestDetector.h"
#endif

std::unique_ptr<Detector> makeDetector(std::string& objectType)
{
	std::unique_ptr<Detector> detector;
#ifdef OPENCV_DETECT
	if (objectType.compare("face") == 0)
	{
		detector.reset(new FaceDetector());
	}	
	else
	{
		detector.reset(new BodyDetector());
	}
#elif FWTECH_DETECT
	detector.reset(new FaceWareDetector());
#elif TEST_DETECT
	detector.reset(new TestDetector());
#endif
	return detector;
}
#endif // MAKE_DETECTOR_H