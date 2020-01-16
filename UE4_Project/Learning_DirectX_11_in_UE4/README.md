# Learning DirectX 11 in UE4
This is a framework I built after I spent some time learning DirectX. This framework free me from dueling with windows api, message and input handle and camera navigation these stuffs.
## How it works
I link DirectX as a third party library. I control my own Directx device and context, but reading geometry data, texture and input data from UE4.
After rendering each frame, I export the buffer in my render target into an UE4 texture and display the texture on screen.
UE4's has it rendering thread, resource and context, but it requires deep understanding to utilize and modify its build-in rendering pipline, so I start a new rendering context.
The main class design includes:
* The top level manager, RenderManagerActor fetch data from UE4 environment and pass to my rendering pipline. It inherit from UE4's Actor class.
* RenderableActor contains geoemtry and texture that will we will render in our pipline, RenderManagerActor will collect all the Renderable actor at beginning.
* Some DirectX resource initialization manager, they are pure C++ classes, and they works exactly the same as normal DirectX initalization procedures.
## How to use it 
* Open the DXrender project, here is the main scene.
<br /><br />
![scene](/UE4_Project/Learning_DirectX_11_in_UE4/screenshot.png)
The left small viewport is the render result, the right big viewport is UE4's original scene viewport. The camera movement of both two viewport is synchronized.
The shaders locates in DXrender/Shader.
 
