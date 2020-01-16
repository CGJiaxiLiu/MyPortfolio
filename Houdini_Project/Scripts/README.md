# Export Geometry as Json

In some circumstances when our program needs 3D model data, including their position and normal, etc. But we may not want to use commercial model parse library, such as FBX SDK. We can build model in Houdini, then use this script to export the data as json file, and correspondingly parse the json in our program. This script can export attribute on point, with different data type, including int, int list, float, flaot list, string, string list. Also includes index buffer, count of point and current frame.
## How to use
* Copy code in hou.session.py to windows->python source editor. These are attribute type enum, and to help figure out to use which function to fetch data.
* Copy code in hou.export_json.py to a python node in OBJ network, connect it to the geometry to export, and add the name of the attribute need to export, its size(size of int, float and string is 1, and the length of list for list attributes) and data type to '__export_attribute' dictionary type variable.
