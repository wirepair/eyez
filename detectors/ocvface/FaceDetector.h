#ifndef FACE_DETECTOR_H
#define FACE_DETECTOR_H

#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

#include "opencv2/objdetect.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include "../Detector.h"
#include "../DetectionData.h"

using namespace cv;

class FaceDetector : public Detector
{
public:
	bool Init() override;

	void RunDetection();

	bool Detect(float* x, float* y) override;

	~FaceDetector();

private:
	bool InitCascade();

	std::thread detectionThread;
	std::mutex dataLock;
	DetectionData detectionData;
	std::atomic_bool running;

	const std::string faceCascadeName = "res/lbpcascade_frontalface.xml"; //"res/haarcascade_frontalface_alt.xml";
	CascadeClassifier face_cascade;
	VideoCapture capture;
	
};

#endif // FACE_DETECTOR_H