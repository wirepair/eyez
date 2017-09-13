solution "eyez"
	configurations { "Release" }
	includedirs { "/usr/local/include", "/usr/include/SDL2" }
	files { "*.h", "*.cpp", "detectors/*.h" }
	flags { "Optimize" }
	libdirs { "/usr/local/lib" }
	links { "SDL2", "SDL2_image" }

	project "eyez-test"
		kind "WindowedApp"
		language "C++"
		defines { "TEST_DETECT" }
		files { "detectors/*.cpp" }
		links { "GLESv2" }
		
	project "eyez-opencv"
		kind "WindowedApp"
		language "C++"
		defines { "OPENCV_DETECT"}
		files { "detectors/ocvface/**.h", "detectors/ocvface/**.cpp", "detectors/ocvbody/**.h", "detectors/ocvbody/**.cpp" }
		links { "GLESv2" }

	
		