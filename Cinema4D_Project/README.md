# C4D Auto Split
There are cases when yoou build a large model in Cinema4D, but you want to split it into different part and export separately. This script can get rid of the tidious manual work to repeatly split and export.

## How To Use
* Add a python generator
* Copy these code into the code editor.
* For the model you want ot split and export as FBX file, use polygon selection tags to select each part, and the name of these tags should begin with 'Piece'.
* Drag the model under the python generator
* A save file window jump out, choose the directory to save and type in the file name. For example, if you type in 'part', the export file will be part_0.fbx, part_1.fbx... and  part_0.c4d, part_1.c4d...