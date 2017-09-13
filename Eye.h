#ifndef EYE_H
#define EYE_H

#include <iostream>
#include <cmath>
#include <memory>
#include <algorithm>

#include "detectors/Detector.h"
#include "FileMonitor.h"
#include "time.h"
#include "sdl_headers.h"

const float FOCUS_DURATION = 0.2f;

enum PupilState { FOCUSING, FOCUSED, DIALATING, DIALATED};

class Eye
{
public:
	Eye(std::string& vertexPath, std::string& fragmentPath, std::string& object) : vertexPath(vertexPath), fragmentPath(fragmentPath), object(object) {}

	/** Load & compile shaders and detector **/
	bool Init();

	/** For debugging, will automatically reload fragment shader on change **/
	bool SetMonitorFragmentFile();

	/** Reloads the shaders **/
	bool Reload();

	void Update(float delta);

	void Move() const;

	/** Tell the pupil to focus **/
	void SetFocus();

	void Focus(float delta);

	bool IsMoving() const;

	inline GLuint GetProgram() { return program; };

	~Eye();

private:
	/** Initializes the vertex and fragment shaders, compiles, and links them. **/
	bool InitializeShaders();

	/** Initializes our detection method. **/
	bool InitDetector();

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

	
	/** Updates the movement of the eye **/
	void UpdateMovement(float delta);

	/** Starts moving the eye by setting destination location info **/
	void StartMovement(float delta);

	/** Returns a random number in between min/max **/
	float Random(float min, float max) const;

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
};

#endif // EYE_H