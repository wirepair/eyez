#include <thread>
#include "FaceDetector.h"


bool FaceDetector::Init()
{
	if (!InitCascade())
	{
		return false;
	}

	capture.open(0);
	if (!capture.isOpened())
	{
		std::cout << "Error opening camera for video" << std::endl;
		return false;
	}

	running.store(true);
	detectionThread = std::thread(&FaceDetector::RunDetection, this);

	return true;
}

bool FaceDetector::InitCascade()
{
	if (!face_cascade.load(faceCascadeName))
	{
		std::cout << "Error loading cascade data" << std::endl;
		return false;
	}
	return true;
}

void FaceDetector::RunDetection()
{
	std::cout << "Detection Thread Started" << std::endl;
	Mat frame;
	while(running.load())
	{
		capture.read(frame);
		if (frame.empty())
		{
			std::cout << "Detection Thread Frame Was Empty" << std::endl;
			continue;
		}
		std::vector<Rect> faces;
		Mat frame_gray;

		cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
		equalizeHist( frame_gray, frame_gray );

		//-- Detect faces
		face_cascade.detectMultiScale( frame_gray, faces, 1.2, 2, 0|CASCADE_SCALE_IMAGE, Size(50, 50) );

		if (faces.size() > 0)
		{
			std::cout << faces[0].x << " " << faces[0].width << " " << faces[0].y << " " << faces[0].height << std::endl;
			Point2f center( faces[0].x + faces[0].width/2, faces[0].y + faces[0].height/2 );
			center.y -= 65; // subtract from y axis for camera being below the eye.
			float detectX = (2 * (center.x/frame.rows) - 1);
			float detectY = (2 * (center.y/frame.cols) - 1);
			std::cout << "DETECTED FACE AT: " << "x: " << detectX << " y: " << detectY << std::endl;
			dataLock.lock();
			detectionData.x = detectX;
			detectionData.y = detectY;
			detectionData.detected = true;
			dataLock.unlock();
		}
		else
		{
			dataLock.lock();
			detectionData.detected = false;
			dataLock.unlock();
		}
	}
	capture.release();
	std::cout << "Detection Thread Completed" << std::endl;
}

bool FaceDetector::Detect(float* x, float* y)
{
	dataLock.lock();
	*x = detectionData.x;
	*y = detectionData.y;
	bool ret = detectionData.detected;
	dataLock.unlock();

	return ret;
}


FaceDetector::~FaceDetector()
{
	running.store(false);
	detectionThread.join();
}