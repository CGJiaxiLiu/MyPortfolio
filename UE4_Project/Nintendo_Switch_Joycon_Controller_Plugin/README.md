# Swtich Joycon Controller Plugin
Using Joycon on PC platform hasn't official support, I wrote this plugin to enable use Joycon during game developing.
## How to enable JoyconInputPlugin
I prefer to copy this plugin to your engine directory, thus you can use it in every project, I put it in UnrealEngine\UE_4.XX\Engine\Plugins\Experimental (XX is your engine version).
<br />This plugin only supports PC platform, and contains binaries already compiled. I built it with engine version 4.22.3, If you find it not compatiable, you can put it into your one project file under plugin foleder, and build the whole project, the plugin will be rebuilt to fit your version.
You can enable the plugin by edit-> Plugins, and search Joycon, if you have located it in the engine, you can see be screen blow.
<br /><br />
![JoyconPluginEnable](/UE4_Project/Nintendo_Switch_Joycon_Controller_Plugin/JoyconPluginEnable.png)
Check the enable, and restart the editor, it is available to use.
## How to use JoyconInputPlugin
JoyconPluginDemo is a demo project which shows how to use the plugin. It is pretty easy to use, I have export the controller buttons and axis as bluprint nodes.
* Make sure you have connect you joycon to PC via bluetooth before you open the project.
* Open the JoyconPluginDemo uproject, here yo can see the scene, which has a sphere and three boxes.<br /><br />
![JoyconPluginDemoScene](/UE4_Project/Nintendo_Switch_Joycon_Controller_Plugin/JoyconPluginDemoProjectScene.png)
* Play. Use right joycon stick to control the movement of the sphere. When press and release X button on right joycon, debug message will be printed on the screen.<br /><br />
![recording](/UE4_Project/Nintendo_Switch_Joycon_Controller_Plugin/joycon_uasge_demo.gif)
* Open level blueprint, I control the ball using a simple logic here. The usage of axis value and button envent is exactly same as using mouse position or keyboard event.<br /><br />
![JoyconPluginDemoBP](/UE4_Project/Nintendo_Switch_Joycon_Controller_Plugin/JoyconPluginDemoLevelBP.png)
# Acknowledgement
Thanks to https://github.com/dekuNukem/Nintendo_Switch_Reverse_Engineering/ provides Joycon reverse engineering, parse the signal
and https://github.com/fossephate/JoyCon-Driver/tree/master/joycon-driver demonstrates how to communicate with Joycon. 
