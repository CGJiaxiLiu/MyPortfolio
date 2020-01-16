#pragma once
#include <SOP/SOP_Node.h>

class JoyconBase;

namespace LJX_HDK_Sample {
/// Run a sin() wave through geometry by deforming points
/// @see @ref HOM/SOP_HOMWave.C, SOP_HOMWave, SOP_CPPWave
class SOP_Joycon : public SOP_Node
{
public:

	SOP_Joycon(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~SOP_Joycon();

    static PRM_Template myTemplateList[];
    static OP_Node * myConstructor(OP_Network*, const char *, OP_Operator *);

    /// This method is created so that it can be called by handles.  It only
    /// cooks the input group of this SOP.  The geometry in this group is
    /// the only geometry manipulated by this SOP.
    virtual OP_ERROR cookInputGroups(OP_Context &context, int alone = 0);
	

protected:
    /// Method to cook geometry for the SOP
    virtual OP_ERROR cookMySop(OP_Context &context);

private:
    void	getGroups(UT_String &str) { evalString(str, "group", 0, 0); }
    fpreal	SCALE(fpreal t)		{ return evalFloat("scale", 0, t); }

	JoyconBase * jc = nullptr;
	
    /// This is the group of geometry to be manipulated by this SOP and cooked
    /// by the method "cookInputGroups".
    const GA_PointGroup *myGroup;
};
}
