import json
from hou.session import ATTRIBUTE_TYPE as att_type
from hou.session import ATTRIBUTE_FUNCTION as att_func

node = hou.pwd()
geo = node.geometry()

__frame = hou.frame()
__export_attribute = {'P': [3, att_type.FLOAT_LIST], 'N': [3, att_type.FLOAT_LIST], 'Cd': [3, att_type.FLOAT_LIST],
                      'value': [1, att_type.FLOAT]}
__export_data = dict()

for key in __export_attribute.keys():
    __export_data[key] = list()

for pt in geo.points():
    for key in __export_attribute.keys():
        __export_data[key].append(att_func[__export_attribute[key][1]](pt, key))

index = list()
for prim in geo.prims():
    for vtx in prim.vertices():
        index.append(vtx.point().number())

data = dict()
data['frame'] = __frame
data['attribute'] = __export_attribute
data['data'] = __export_data
data["index"] = index
data["point_count"] = len(geo.points())

jsonStr = json.dumps(data, indent=4)

with open('output_data.json', 'w') as f:
    f.write(jsonStr)
