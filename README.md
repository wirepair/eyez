# Eyez
My attempt at building a creepy eye for the Gakken WorldEye on a raspberry pi 3.

# Building
Set your raspberry pi to expiremental driver (sudo raspi-config)
Install libsdl 2.0.5 from https://www.libsdl.org/download-2.0.php

sudo apt-get premake4 libgles1-mesa libgles2-mesa libegl1-mesa-dev libgles2-mesa-dev libx11-dev

# TODO:
Ensure build guide actually works
Implement movement, focus, connect to OpenCV.