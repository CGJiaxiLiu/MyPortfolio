import c4d
import os
from c4d import storage
from c4d import utils


def main():
    path = c4d.storage.SaveDialog()
    for child in op.GetChildren():
        count = 0
        tempSelectList = list()
        tempSelectName = list()
        for tag in child.GetTags():
            if isinstance(tag, c4d.SelectionTag) and tag.GetName().startswith("Piece"):
                count += 1
            elif isinstance(tag, c4d.SelectionTag) and not tag.GetName().startswith("Piece"):
                tempSelection = c4d.BaseSelect()
                tag.GetBaseSelect().CopyTo(tempSelection)
                tempSelectList.append(tempSelection)
                tempSelectName.append(tag.GetName())
                child.KillTag(c4d.Tpolygonselection, count)
        lastTag = child.GetLastTag()
        for index, selection in enumerate(tempSelectList):
            newTag = child.MakeTag(c4d.Tpolygonselection, lastTag)
            newTag.SetName(tempSelectName[index])
            selection.CopyTo(newTag.GetBaseSelect())

        count = 0
        for tag in child.GetTags():
            if isinstance(tag, c4d.SelectionTag) and tag.GetName().startswith("Piece"):
                newselection = child.GetPolygonS()
                newselection.DeselectAll()
                selection = tag.GetBaseSelect().CopyTo(newselection)

                secList = utils.SendModelingCommand(command=c4d.MCOMMAND_SPLIT, list=[child], mode=c4d.MODELINGCOMMANDMODE_POLYGONSELECTION, doc=doc)
                if secList:
                    sec = secList[0]
                    sec.SetName(tag.GetName())
                    sec.InsertAfter(op)
                    for i in range(count):
                        sec.KillTag(c4d.Tpolygonselection)

                    fbxPath = path + '_' + str(count) + '.fbx'
                    c4dPath = path + '_' + str(count) + '.c4d'
                    print(fbxPath)
                    newDoc = c4d.documents.IsolateObjects(doc, [sec])
                    obj = newDoc.GetFirstObject()
                    newDoc.SetSelection(obj, c4d.SELECTION_NEW)
                    c4d.documents.SaveDocument(newDoc, fbxPath, c4d.SAVEDOCUMENTFLAGS_DONTADDTORECENTLIST, 1026370)
                    c4d.documents.SaveDocument(newDoc, c4dPath, c4d.SAVEDOCUMENTFLAGS_DONTADDTORECENTLIST, c4d.FORMAT_C4DEXPORT)
                    sec.Remove()
                    count += 1