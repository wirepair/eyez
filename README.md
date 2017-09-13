# Eyez
My attempt at building a creepy eye for the Gakken WorldEye on a raspberry pi 3.

# Components
- Gakken WorldEye Globe Projector - [Japan](https://www.amazon.co.jp/%E5%AD%A6%E7%A0%94-83000-Gakken%E3%83%AF%E3%83%BC%E3%83%AB%E3%83%89%E3%83%BB%E3%82%A2%E3%82%A4/dp/B00DE2VX5M) (8000円) [US](https://www.amazon.com/World-Eye-Digital-Globe-Aqualium/dp/B00DE2VX5M) (212$)
- Raspberry Pi 3 (~60$)
- Raspberry Pi Camera (Built with: [Kuman](http://www.kumantech.com/kuman-5mp-1080p-hd-camera-module-for-raspberry-pi-for-raspberry-pi-3-model-b-b-a-rpi-2-1-sc15_p0063.html))

# Building
Set your raspberry pi to expiremental driver (sudo raspi-config)
- sudo apt-get update
- sudo apt-get upgrade
- Install libsdl 2.0.5 from [https://www.libsdl.org/download-2.0.php](https://www.libsdl.org/download-2.0.php)
- Install opencv 3.0.1 from [http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/](http://www.pyimagesearch.com/2016/04/18/install-guide-raspberry-pi-3-raspbian-jessie-opencv-3/)

# TODO
- Ensure build guide actually works
- Implement movement, focus, connect to OpenCV.