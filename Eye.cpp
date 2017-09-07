#include "Eye.h"
#include "Loader.h"

bool Eye::Init()
{
	if (!InitializeShaders())
	{
		return false;
	}

	ConfigureUniforms();
	ConfigureQuad();
	
	return true;
}

bool Eye::SetMonitorFragmentFile()
{
	fragmentMonitor.reset(new FileMonitor(fragmentPath));
	return fragmentMonitor->Init();
}

bool Eye::Reload()
{
	Destroy();
	return Init();
}

bool Eye::InitializeShaders()
{
	if (!InitShader(vertexShader, vertexPath, GL_VERTEX_SHADER) || 
		!InitShader(fragmentShader, fragmentPath, GL_FRAGMENT_SHADER))
	{
		return false;
	}

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glBindAttribLocation(program, 0, "u_position");

	glLinkProgram(program);
	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		std::cout << "Error failed to link shaders" << std::endl;
		GetLinkerError();
		return false;
	}

	glUseProgram(program);

	return true;
}

bool Eye::InitShader(GLuint& shader, std::string& shaderPath, GLenum shaderType)
{
	std::string shaderSource = LoadShader(shaderPath);
	if (shaderSource.compare("") == 0)
	{
		std::cout << "Error loading " << shaderPath << " shader: " << std::endl;
		return false;
	}
	
	shader = glCreateShader(shaderType);
	if (!CompileShader(shader, shaderSource))
	{
		std::cout << "failed to compile " << shaderPath << " shader" << std::endl;
		return false;
	}
	return true;
}

bool Eye::CompileShader(GLuint shader, std::string& src)
{
	GLint is_compiled;
	const char* c_str = src.c_str();

	glShaderSource(shader, 1, &c_str, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);

	if (is_compiled != 1)
	{
		GetCompileError(shader);
	}
	return is_compiled == 1 ? true : false;
}

void Eye::ConfigureUniforms()
{
	u_time_loc = glGetUniformLocation(program, "u_time");
 
 	u_resolution = glGetUniformLocation(program, "u_resolution");
	glUniform2f(u_resolution, SCREEN_WIDTH, SCREEN_HEIGHT);

	u_position = glGetUniformLocation(program, "u_position");

}

void Eye::ConfigureQuad()
{
	// setup vertex attribs
	GLuint ATTRIB_VERTEX;
	glEnableVertexAttribArray(u_position);
	glVertexAttribPointer(u_position, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    

	glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), vertices);
	// ensure the proper arrays are enabled
	glEnableVertexAttribArray(ATTRIB_VERTEX);
}

void Eye::GetCompileError(GLuint shader)
{
	GLint maxLength = 0;
	
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
	if (maxLength > 1)
	{
		char * errorLog = (char *)malloc(sizeof(char) * maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
		std::cout << errorLog << std::endl;
		free(errorLog);
	}
	return;
}

void Eye::GetLinkerError()
{
	GLint maxLength = 0;
	
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	if(maxLength > 1)
	{
		char* errorLog = (char *)malloc(sizeof(char) * maxLength);
		glGetProgramInfoLog(program, maxLength, NULL, errorLog);
		std::cout << "error linking program: " << errorLog << std::endl;
		free(errorLog);
	}
	return;
}

void Eye::Update(float delta)
{
	if (fragmentMonitor && fragmentMonitor->ShouldReload())
	{
		std::cout << "Reloading..." << std::endl;
		Reload();
	}
	/*
	if (isMoving)
	{
		UpdateMovement(delta);
	}
	else if (delta >= holdDuration)
	{
		StartMovement(delta);
	}
	holdDuration -= delta;
	*/
	// draw the quad and update u_time;
	glUniform1f(u_time_loc, u_time += 1.0f/60.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
}

void Eye::StartMovement(float delta)
{
	destinationX = Random(-150.0f, 150.0f);
	//double n = sqrt(900 - destinationX * destinationX);
	destinationY = Random(-100.0f, 100.0f);
	moveDuration = Random(0.12, 0.35);
	isMoving = true;
	std::cout << " start movement " << destinationX << std::endl;
}

void Eye::UpdateMovement(float delta)
{
	if (delta <= moveDuration)
	{
		movementScale = delta / moveDuration;
		movementScale = (3.0 * movementScale * movementScale) - (2.0 * movementScale * movementScale * movementScale);
		std::cout << "delta " << delta << " scale: " << movementScale << std::endl; 
		currentX = startX + (destinationX - startX) * movementScale;
		currentY = startY + (destinationY - startY) * movementScale;
		moveDuration -= delta;
	}
	else
	{
		std::cout << "delta > move" << std::endl;
		startX = currentX;
		currentX = destinationX;

		startY = destinationY;
		currentY = destinationY;

		holdDuration = Random(0.15f, 1.7f);
		isMoving = false;
	}
}

void Eye::Move() const
{
	//double x = ((SCREEN_WIDTH - eyeTexture->GetWidth()) / 2) + currentX;
	//double y = ((SCREEN_HEIGHT - eyeTexture->GetHeight()) / 2) + currentY;
	std::cout << " X: " << currentX << " Y: " << currentY << std::endl;
	//eyeTexture->Render(x, y, NULL, 0.0F, NULL, SDL_FLIP_NONE);
}

bool Eye::IsMoving() const
{
	return isMoving;
}

double Eye::Random(double min, double max) const
{
	double r = (double)rand() / RAND_MAX;
    return min + r * (max - min);
}

void Eye::Destroy()
{
	if (vertexShader != 0)
	{
		glDeleteShader(vertexShader);
	}

	if (fragmentShader != 0)
	{
		glDeleteShader(fragmentShader);
	}

	if (program != 0)
	{
		glDeleteProgram(program);
	}
}

Eye::~Eye()
{
	Destroy();
}