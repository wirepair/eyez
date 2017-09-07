#ifndef EYE_H
#define EYE_H

#include <iostream>
#include <cmath>
#include <memory>

#include "FileMonitor.h"
#include "time.h"
#include "sdl_headers.h"

class Eye
{

public:
	Eye(std::string& vertexPath, std::string& fragmentPath) : vertexPath(vertexPath), fragmentPath(fragmentPath) {}

	/** Load & compile shaders **/
	bool Init();

	/** For debugging, will automatically reload fragment shader on change **/
	bool SetMonitorFragmentFile();

	/** Reloads the shaders **/
	bool Reload();

	void Update(float delta);

	void Move() const;

	bool IsMoving() const;

	inline GLuint GetProgram() { return program; };

	~Eye();

private:
	/** Initializes the vertex and fragment shaders, compiles, and links them. **/
	bool InitializeShaders();

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
	double Random(double min, double max) const;

	void Destroy();

private:
	/** shader data inputs **/
	std::string& vertexPath;
	std::string& fragmentPath;

	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;

	/** shader data **/
	GLuint u_time_loc;
	GLuint u_resolution;
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
	double lastTime = 0.0f;
	double moveDuration = 0.0f;
	double holdDuration = 0.0f; 
	double movementScale = 0.0f;

	/** eye position modifiers **/
	double startX = 0.0f;
	double startY = 0.0f;

	double destinationX = 0.0f;
	double destinationY = 0.0f;

	double currentX = 0.0f;
	double currentY = 0.0f;

	/** Debugging **/
	std::unique_ptr<FileMonitor> fragmentMonitor;
};

#endif // EYE_H