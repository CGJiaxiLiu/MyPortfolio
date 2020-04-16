# Using OpenCV in UE4
Researchers can use OpenCV in UE4 to use the scene rendered by UE4 instead of real world image. It can also help produce large data set for tarining in vision related AI without having to manually label the photo.

## How it works
UE4 provide scene capture functionality that can render scene from different view simutaneously, and I read from the render result and pass it to OpenCV.

## How to use
I am not quite familir with OpenCV, I provided a basic demo on depth estimation with stereo camera.
Here is the scene and there are three camera, two capture normal scene, while the middle one capture actual depth data through past processing.
<br /><br />
![scene](/UE4_Project/Using_OpenCV_in_UE4/scene.png)
The three scene capture actor render scene from diferent view to three rendertarget RenderTarget2D_0 to RenderTarget2D_2. And cvDepthEstimator actor needs three figure.
Here are the result. From left to right and top to bottom, right camera, left camera, disparity and actual depth data.
<br /><br />
![four_figure](/UE4_Project/Using_OpenCV_in_UE4/four_figure.png)
