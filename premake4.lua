solution "eyez"
	configurations { "Release" }
	includedirs { "/usr/local/include", "/usr/include/SDL2" }
	files { "*.h", "*.cpp", "detectors/*.h" }
	flags { "Optimize" }
	libdirs { "/usr/local/lib" }
	links { "SDL2", "SDL2_image", "GLESv2" }

	project "eyez-test"
		kind "WindowedApp"
		language "C++"
		defines { "TEST_DETECT" }
		files { "detectors/*.cpp" }

	project "eyez-opencv"
		kind "WindowedApp"
		language "C++"
		defines { "OPENCV_DETECT"}
		files { "detectors/ocvface/**.h", "detectors/ocvface/**.cpp", "detectors/ocvbody/**.h", "detectors/ocvbody/**.cpp" }
		links { "pthread", "opencv_core", "opencv_highgui", "opencv_objdetect", "opencv_videoio", "opencv_imgproc" }