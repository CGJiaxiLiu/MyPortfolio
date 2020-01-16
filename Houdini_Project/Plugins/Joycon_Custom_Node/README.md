# A Custom Joycon Controller Node
This custom SOP node which can communicate with Nintendo Switch Joycon and get its input value and modify the geometry according to the input.

## How it works
This custom node is based on [HDK(Houdini Development Kit)](https://www.sidefx.com/docs/hdk/). The node first connects with Joycon during initialization and poll the joycon when it needs to cook geometry.

## How to use
I am using CMake to generate the project and my Houdini version is h17.5, if your houdini version isn't same as mine, you need to rebuild the binary. 
The preparation for using HDK and build the project including 

* Add Houdinix.y\toolkit\cmake directory to system path.
* Include the Houidni package, the header JoyconBase.h and link the static library JoyconProject.lib in the CMakeLists.
* Generate the project and build it, the binaries file and library will output to user_name\Documents\houdini17.5\dso.
* Copy the hidapi.dll and JoyconProject.dll dynamic library to dso directory.
* Create an Houdini project and use node 'Joycon' in OBJ level. This is a sample node that will translate the geometry horizontally according to the joystick input value of right Joycon.

A sample project and binaries have been provided, you need first copy the files in [dso](/Houdini_Project/Plugins/Joycon_Custom_Node/dso) to your dso directory, and then run the [joycon.hipnc](/Houdini_Project/Plugins/Joycon_Custom_Node/joycon.hipnc)

I am using my joycon to control the movement of the red line.<br /><br />
![joycon_plugin](/Houdini_Project/Plugins/Joycon_Custom_Node/joycon_plugin.gif)
<br />
This node may not be useful, but it indicates you can create a custom node wtih any C++ library you need, and it has much better performance than writing python and [VEX](https://www.sidefx.com/docs/houdini/vex/index.html) code (Houdini built-in script language, which resembles C, both on grammar and efficiency). It comes more handy when you need to write complex algorithm, for it is hard to write complex code in a single python or VEX node, especially when you need OOP and third-party libraries.


# Acknowledgement
Thanks to https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/ provides Joycon reverse engineering, parse the signal
and https://github.com/fossephate/JoyCon-Driver/tree/master/joycon-driver demonstrates how to communicate with Joycon. 
