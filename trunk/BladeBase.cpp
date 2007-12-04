#include "BladeBase.h"
#include "SimpleMaterial.h"
#include "VisualVertexArraySphere.h"
#include "VisualBox.h"
#include "Config.h"
using tlib::OCSimpleMaterial;
using tlib::OCVisualVertexArraySphere;
using tlib::OCVisualBox;
using tlib::Config;
using tlib::Vector3f;

void BladeBase::init()
{
    Config cfg("config.txt");
    cfg.loadBlock("blade_base");

    // Read part's color
    float vfColor[4];
    cfg.getFloat("color", vfColor, 4);

    // Read part's position relative to system center
    float vfOffset[3];
    cfg.getFloat("offset", vfOffset, 3);
    getPos().xyz( vfOffset );

    // Read part's radius
    float fRadius;
    cfg.getFloat("radius", &fRadius);

    // Read part's stacks
    int iStacks;
    cfg.getInt("stacks", &iStacks);

    // Read part's slices
    int iSlices;
    cfg.getInt("slices", &iSlices);

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color( vfColor ) );
    cMat->setDiffuse( Color( vfColor ) );
    setComponent( cMat );

    // Initialize visual component
    setComponent( new OCVisualVertexArraySphere( fRadius, iStacks, iSlices ) );

} // end init()