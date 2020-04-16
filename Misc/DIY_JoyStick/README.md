# A DIY JoyStick
My friend Zheng Shen and I made a controller with one joystick and two buttons, and communicate with PC through WIFI.

## How it works
Zheng Shen acquired the signal of button and joystick and sent them from single-chip microcomputer to ESP8266 WIFI chip through serial communication.
I config the WIFI chip via LUA script and connect to the WIFI and communicate with the WIFI chip using the Socket package in UE4.
<br /><br />
![pic](/Misc/A_Self-Made_JoyStick/A_Self-Made_JoyStick.jpg)
<br /><br />
![video](/Misc/A_Self-Made_JoyStick/A_Self-Made_JoyStick.mp4)
<br /><br />
And remember to add 'PublicDependencyModuleNames.AddRange(new string[] { "Sockets", "Networking" });' in the project config file to enable Sokect.

# Acknowledgement

The video game we control is 'mountain view', a demo proejct of UE4, the player control glider to travel though the mountains.

