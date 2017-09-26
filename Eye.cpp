#include <algorithm>
#include <iomanip>
#include <cfloat>

#include "Eye.h"
#include "Loader.h"
#include "detectors/Detector.h"
#include "detectors/MakeDetector.h"

bool Eye::Init()
{
    if (!InitializeShaders())
    {
    	return false;
    }

    ConfigureUniforms();
    ConfigureQuad();

    if (!InitDetector())
    {
    	return false;
    }
    return true;
}

bool Eye::InitDetector()
{
    if (detector)
    {
        return true;
    }
    detector = makeDetector(object);
    return detector->Init();
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

    u_pupil_size = glGetUniformLocation(program, "u_pupil_size");
    glUniform1f(u_pupil_size, pupilSize);

    u_eye_location = glGetUniformLocation(program, "u_eye_location");
    glUniform2f(u_eye_location, currentX, currentY);
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

void Eye::Calibrate()
{
    isCalibrating = true;
    calibratedX = 0.0f;
    calibratedY = 0.0f;
    std::cout << "Calibration Started" << std::endl;
}

void Eye::CalibrateDetector(float delta)
{
    if (delta <= calibrateDuration)
    {
        std::unique_ptr<DetectionData> data(new DetectionData());
        data->detected = detector->Detect(&data->x, &data->y);
        if (data->detected)
        {
            detectionData.push_back(std::move(data));
        }
        calibrateDuration -= delta;
    }
    else
    {
        calibrateDuration = CALIBRATE_DURATION;
        size_t numElements = detectionData.size();
        for (const auto &data : detectionData)
        {
            calibratedX += data->x;
            calibratedY += data->y;
        }
        calibratedX /= numElements;
        calibratedY /= numElements;
        detectionData.clear();
        isCalibrating = false;
        std::cout << "Calibration Complete: CalibratedX: " << std::setprecision(5) << calibratedX << " CalibratedY: " << calibratedY << std::endl;
    }
}

void Eye::Update(float delta)
{
    if (fragmentMonitor && fragmentMonitor->ShouldReload())
    {
        std::cout << "Reloading..." << std::endl;
        Reload();
    }

    if (isCalibrating)
    {
        CalibrateDetector(delta);
        return;
    }

    UpdateEyeState(delta);

    // move eye to new currentX & currentY location
    glUniform2f(u_eye_location, currentX, currentY);

    // draw the quad and update u_time;
    glUniform1f(u_time_loc, u_time += 1.0f/60.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);
}

void Eye::UpdateEyeState(float delta)
{
    float x = detectedX;
    float y = detectedY;
    bool detected = detector->Detect(&x, &y);
    if (detected)
    {
        detectedX = (IsSame(detectedX, (x - calibratedX))) ? detectedX : x - calibratedX;
        detectedY = (IsSame(detectedY, (y - calibratedY))) ? detectedY : y - calibratedY;
        if (eyeState < START_DETECTED)
        {
            eyeState = START_DETECTED;
        }
    }
    else if (eyeState == END_DETECTED || eyeState == HOLD_DETECTED)
    {
        eyeState = START_MOVEMENT;
    }

    std::cout << "eyeState: " << eyeState << std::endl;

    switch(eyeState)
    {
        case START_MOVEMENT:
            destinationX = Random(-0.40f, 0.40f);
            destinationY = Random(-0.30f, 0.30f);
            moveDuration = Random(0.72f, 1.05f);
            eyeState = MOVEMENT;
            break;
        case MOVEMENT:
            if (delta <= moveDuration)
            {
                movementScale = delta / moveDuration;
                movementScale = (3.0f * movementScale * movementScale) - (2.0f * movementScale * movementScale * movementScale);
                currentX = startX + (destinationX - startX) * movementScale;
                currentY = startY + (destinationY - startY) * movementScale;
                moveDuration -= delta;
            }
            else
            {
            startX = currentX;
            currentX = destinationX;
            startY = destinationY;
            currentY = destinationY;
            moveDuration = Random(0.42f, 0.95);
            holdDuration = Random(0.35f, 1.2f);
            eyeState = END_MOVEMENT;
            }
            break;
        case END_MOVEMENT:
            if (delta >= holdDuration)
            {
                eyeState = START_MOVEMENT;
            }
            else
            {
                holdDuration -= delta;
            }
            break;
        case START_DETECTED:
            SetFocus();
            destinationX = detectedX;
            destinationY = detectedY;
            moveDuration = Random(0.28f, 0.42f);
            eyeState = DETECTED;
            break;
        case DETECTED:
            if (delta <= moveDuration)
            {
                movementScale = delta / moveDuration;
                movementScale = (3.0f * movementScale * movementScale) - (2.0f * movementScale * movementScale * movementScale);
                currentX = startX + (destinationX - startX) * movementScale;
                currentY = startY + (destinationY - startY) * movementScale;
                moveDuration -= delta;
            }
            else
            {
                startX = currentX;
                currentX = destinationX;

                startY = destinationY;
                currentY = destinationY;
                eyeState = END_DETECTED;
                SetFocus();
                Focus(delta);
                moveDuration = Random(0.28f, 0.42f);
            }
            break;
        case END_DETECTED:
            SetFocus();
            Focus(delta);
            currentX = detectedX;
            currentY = detectedY;
            break;
        case HOLD_DETECTED:
            break;
    }
}

void Eye::SetFocus()
{
    switch(pupilState)
    {
        case DIALATED:
            pupilState = FOCUSING;
            break;
        case FOCUSED:
            pupilState = DIALATING;
            break;
    }
}

void Eye::Focus(float delta)
{
    switch (pupilState)
    {
        case FOCUSING:
        {
            std::cout << "focusing: " <<  delta << " " << focusDuration << std::endl;
            if (delta <= focusDuration)
            {
            focusScale = delta / focusDuration;
            focusScale = (3.0f * focusScale * focusScale) - (2.0f * focusScale * focusScale * focusScale);
            currentPupilSize = pupilSize + (focusSize - pupilSize) * focusScale;
            std::cout << "focusing currentSize: " << currentPupilSize << std::endl;
            glUniform1f(u_pupil_size, currentPupilSize);
            focusDuration -= delta;
            }
            else
            {
            currentPupilSize = focusSize;
            glUniform1f(u_pupil_size, currentPupilSize);
            focusDuration = FOCUS_DURATION;
            pupilState = FOCUSED;
            }
            break;
        }
        case FOCUSED: // fall through
        case DIALATED:
            break;
        case DIALATING:
            {
            std::cout << "dialating: " <<  delta << " " << focusDuration << std::endl;
            if (delta <= focusDuration)
            {
            focusScale = delta / focusDuration;
            focusScale = (3.0f * focusScale * focusScale) - (2.0f * focusScale * focusScale * focusScale);
            currentPupilSize = focusSize + (pupilSize - focusSize) * focusScale;
            std::cout << "unfocusing currentSize: " << currentPupilSize << std::endl;
            glUniform1f(u_pupil_size, currentPupilSize);
            focusDuration -= delta;
            }
            else
            {
            focusDuration = FOCUS_DURATION;
            currentPupilSize = pupilSize;
            glUniform1f(u_pupil_size, currentPupilSize);
            pupilState = DIALATED;
            }
            break;	
        }
    }
}

/** Give a bit of tolerance so we don't have the eye shaking **/
bool Eye::IsSame(float a, float b) const
{
    return fabs(a - b) < 0.015;
}

float Eye::Clamp(float d, float min, float max) const
{
    const float t = d < min ? min : d;
    return t > max ? max : t;
}

float Eye::Random(float min, float max) const
{
    float r = (float)rand() / RAND_MAX;
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