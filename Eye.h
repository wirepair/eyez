#ifndef EYE_H
#define EYE_H

#include <iostream>
#include <cmath>
#include <memory>
#include <vector>

#include "detectors/Detector.h"
#include "detectors/DetectionData.h"
#include "FileMonitor.h"
#include "time.h"
#include "sdl_headers.h"

const float FOCUS_DURATION = 0.2f * FRAMES_PER_SECOND;
const float CALIBRATE_DURATION = 5.0f; 

enum EyeState { START_MOVEMENT, MOVEMENT, END_MOVEMENT, START_DETECTED, DETECTED, END_DETECTED, HOLD_DETECTED };
enum PupilState { FOCUSING, FOCUSED, DIALATING, DIALATED };

class Eye
{
public:
	Eye(std::string& vertexPath, std::string& fragmentPath, std::string& object) : vertexPath(vertexPath), fragmentPath(fragmentPath), object(object) {}

	/** Load & compile shaders and detector **/
	bool Init();

	/** For debugging, will automatically reload fragment shader on change **/
	bool SetMonitorFragmentFile();

	/** Calibrate the camera to eye position, user should align face to the center of the globe/eye. **/
	void Calibrate();

	/** Reloads the shaders **/
	bool Reload();

	/** Called every tick **/
	void Update(float delta);

	/** Tell the pupil to focus **/
	void SetFocus();

	void Focus(float delta);

	inline GLuint GetProgram() { return program; };

	~Eye();

private:
	/** Initializes the vertex and fragment shaders, compiles, and links them. **/
	bool InitializeShaders();

	/** Initializes our detection method. **/
	bool InitDetector();

	/** Calibrates the detector since camera/globe angle can be anywhere **/
	void CalibrateDetector(float delta);

	/** Initializes the shader of shaderType given the shaderPath to the src **/
	bool InitShader(GLuint& shader, std::string& shaderPath, GLenum shaderType);

	/** Compiles an individual shader **/
	bool CompileShader(GLuint shader, std::string& src);

	/** Configures uniforms used in our shaders **/
	void ConfigureUniforms();

	/** Configures the quad where we display our eye. **/
	void ConfigureQuad();
	
	/** Prints GL compilation error information **/
	void GetCompileError(GLuint shader);

	/** Prints GL linker error information **/
	void GetLinkerError();
	
	/** Handle Eye / Pupil State **/
	void UpdateEyeState(float delta);

	/** Returns a random number in between min/max **/
	float Random(float min, float max) const;

	/** Clamps d to min/max **/
	float Clamp(float d, float min, float max) const;

	/** float equality test with tolerance (to reduce eye ball shake on detection) **/
	bool IsSame(float a, float b) const;

	/** Kill it with fire **/
	void Destroy();

private:
	/** object for our eye to detect **/
	std::string& object;

	/** shader data inputs **/
	std::string& vertexPath;
	std::string& fragmentPath;

	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;

	/** shader data **/
	GLuint u_time_loc;
	GLuint u_eye_location;
	GLuint u_resolution;
	GLuint u_pupil_size;
	GLuint u_position = 0;

	
	float u_time = 0.0f;

	/** shader quad vertices for rendering the eye on to **/
	const GLfloat vertices[12] = { -1, -1, 0,  // bottom left corner
                             -1,  1, 0,  // top left corner
                              1,  1, 0,  // top right corner
                              1, -1, 0}; // bottom right corner

	const GLubyte indices[6] = { 0, 2, 1,     // first triangle (bottom left - top left - top right)
	                            0, 3, 2 };



	/** Movement conditionals **/
	bool isMoving;
	EyeState eyeState = START_MOVEMENT;
	float lastTime = 0.0f;
	float moveDuration = 0.0f;
	float holdDuration = 0.0f; 
	float movementScale = 0.0f;

	/** eye position modifiers **/
	float startX = 0.0f;
	float startY = 0.0f;

	float destinationX = 0.0f;
	float destinationY = 0.0f;

	float currentX = 0.0f;
	float currentY = 0.0f;

	// pupil focus //
	PupilState pupilState = DIALATED;
	double focusDuration = FOCUS_DURATION;
	float focusScale = 0.0f;
	float pupilSize = 0.5f;
	float focusSize = 1.5f;
	float currentPupilSize = pupilSize;

	/** Debugging **/
	std::unique_ptr<FileMonitor> fragmentMonitor;

	/** Detector **/
	std::unique_ptr<Detector> detector; 
	float detectedX = 0.0f;
	float detectedY = 0.0f;

	/** Detection Calibration **/
	float calibrateDuration = CALIBRATE_DURATION;
	bool isCalibrating = false;
	float calibratedX = 0.0f;
	float calibratedY = 0.0f;
	std::vector<std::unique_ptr<DetectionData>> detectionData;
	
};

#endif // EYE_H