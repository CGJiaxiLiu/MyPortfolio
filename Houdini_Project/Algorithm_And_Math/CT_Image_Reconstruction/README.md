# DFT_For_CT_Image_Reconstruction
This proejct demostrate four step of CT scan(computed tomography). The foundation of CT is differet human organs have different X-ray absorptivity, which will influence the X-ray intensitiy after it travel through body, and we can reconstruct the distribution of body with X-ray intensitiy data.
From left to right, the four figures demonstrate:
* Actual organ abosorptivity distribution(the smile face is just for demonstration) on one cross section of body. We cannot directly get this image because we don't want to slice patient's body.
* Data acquired. Numbers of X-ray generator and receiver distributed on a circle around the body. The body keeps rotating and each receiver can get the X-ray intensity after it travel through the body from any direction.
* Project the intensity backward and accumulate.
* Do a high-pass filter via descrete Fourier transform to reconstruct the image and improve the fidelity.

![DFT_For_CT_Image_Reconstruction_JPG](/Houdini_Project/Algorithm_And_Math/CT_Image_Reconstruction/CT_Image_Reconstruction.png)

# Acknowledgement
This algorithm and knowledge comes from my classmates Dekai Zhu, Shukai Zhang and Tong xiao. They build the above mathematical model for CT processing and image construction method in Mathematical Contest in Modeling of Tongji University.
