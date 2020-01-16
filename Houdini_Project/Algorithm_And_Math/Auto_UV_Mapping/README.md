# Auto UV Mapping Based On Conformal Mapping

This algorithm is based on conformal mapping, which mapps a open 3D mesh to a circle plane and can be applied to automatically UV coordinates assign. However, due to the nature of comformal mapping, it preserves angle on the surface but can cause severe area distortion, a large region of the surface is mapped to a small area on the plane, which will requires to increase texture resolution.
<br />
<br />![Auto_UV_Mapping_GIF](/Houdini_Project/Algorithm_And_Math/Auto_UV_Mapping/Auto_UV_Mapping.gif)

# Acknowledgement
This algorithm implemantation is based on the pseudocode provided in the book 'Computational Conformal Geometry', author Xianfeng David Gu and Shing-Tung Yau.
<br />
<br />![reference_book](/Houdini_Project/Algorithm_And_Math/Auto_UV_Mapping/reference-book.jpg)
