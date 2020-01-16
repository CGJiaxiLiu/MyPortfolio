#include "SOP_Joycon.h"
#include "JoyconBase.h"

#include <GU/GU_Detail.h>
#include <OP/OP_Operator.h>
#include <OP/OP_AutoLockInputs.h>
#include <OP/OP_OperatorTable.h>
#include <PRM/PRM_Include.h>
#include <UT/UT_DSOVersion.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_Matrix4.h>
#include <SYS/SYS_Math.h>
#include <stddef.h>

using namespace LJX_HDK_Sample;

void
newSopOperator(OP_OperatorTable *table)
{
    table->addOperator(new OP_Operator(
        "joycon_controller",
        "Joycon",
		SOP_Joycon::myConstructor,
		SOP_Joycon::myTemplateList,
        1,
        1,
        0));
}

static PRM_Name names = {
    PRM_Name("scale", "Scale")
};

PRM_Template
SOP_Joycon::myTemplateList[] = {
    PRM_Template(PRM_STRING, 1, &PRMgroupName, 0, &SOP_Node::pointGroupMenu, 0, 0, SOP_Node::getGroupSelectButton(GA_GROUP_POINT)),
    PRM_Template(PRM_FLT_J,	1, &names, PRMoneDefaults, 0, &PRMscaleRange),
    PRM_Template(),
};


OP_Node *
SOP_Joycon::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
	return new SOP_Joycon(net, name, op);
}

SOP_Joycon::SOP_Joycon(OP_Network *net, const char *name, OP_Operator *op)
    : SOP_Node(net, name, op), myGroup(NULL)
{
    mySopFlags.setManagesDataIDs(true);

	jc = new JoyconBase(SIDE::RIGHT);
	jc->Init();

}

SOP_Joycon::~SOP_Joycon()
{
	if (jc)
	{
		delete jc;
	}
	jc = nullptr;
}

OP_ERROR
SOP_Joycon::cookInputGroups(OP_Context &context, int alone)
{
    // The SOP_Node::cookInputPointGroups() provides a good default
    // implementation for just handling a point selection.
    return cookInputPointGroups(
        context, // This is needed for cooking the group parameter, and cooking the input if alone.
        myGroup, // The group (or NULL) is written to myGroup if not alone.
        alone,   // This is true iff called outside of cookMySop to update handles.
                 // true means the group will be for the input geometry.
                 // false means the group will be for gdp (the working/output geometry).
        true,    // (default) true means to set the selection to the group if not alone and the highlight flag is on.
        0,       // (default) Parameter index of the group field
        -1,      // (default) Parameter index of the group type field (-1 since there isn't one)
        true,    // (default) true means that a pointer to an existing group is okay; false means group is always new.
        false,   // (default) false means new groups should be unordered; true means new groups should be ordered.
        true,    // (default) true means that all new groups should be detached, so not owned by the detail;
                 //           false means that new point and primitive groups on gdp will be owned by gdp.
        0        // (default) Index of the input whose geometry the group will be made for if alone.
    );
}

OP_ERROR
SOP_Joycon::cookMySop(OP_Context &context)
{
    OP_AutoLockInputs inputs(this);
    if (inputs.lock(context) >= UT_ERROR_ABORT)
        return error();

    // Duplicate our incoming geometry with the hint that we only
    // altered points.  Thus, if our input was unchanged, we can
    // easily roll back our changes by copying point values.
    duplicatePointSource(0, context);

    fpreal t = context.getTime();

    float scale = SCALE(t);
	float input_x = 0.0f;
	float input_y = 0.0f;

	if (jc->IsValid())
	{
		// Poll the joycon, fetch input value
		jc->Poll();
		input_x = jc->GetStickX();
		input_y = jc->GetStickY();
	}

    if (error() >= UT_ERROR_ABORT)
        return error();

    if (cookInputGroups(context) >= UT_ERROR_ABORT)
        return error();

    GA_Offset ptoff;
    GA_FOR_ALL_GROUP_PTOFF(gdp, myGroup, ptoff)
    {
        UT_Vector3 p = gdp->getPos3(ptoff);

        p.x() += scale * input_x;
		p.z() += scale * input_y;

        gdp->setPos3(ptoff, p);
    }

    // If we've modified P, and we're managing our own data IDs,
    // we must bump the data ID for P.
    if (!myGroup || !myGroup->isEmpty())
        gdp->getP()->bumpDataId();

    return error();
}
