solution "eyez"
	project "eyez"
		kind "WindowedApp"
		language "C++"
		configurations "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }
			files { "**.h", "**.cpp" }
			includedirs { "/usr/local/include", "/usr/include/SDL2" }
			libdirs { "/usr/local/lib" }
			links { "SDL2", "SDL2_image", "GLESv2" }

		