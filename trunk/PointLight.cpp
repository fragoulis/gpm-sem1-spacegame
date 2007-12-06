#include "PointLight.h"
#include "SimpleMaterial.h"
#include "VisualVertexArraySphere.h"
#include "Config.h"
using tlib::OCSimpleMaterial;
using tlib::OCVisualVertexArraySphere;
using tlib::Config;

PointLight::PointLight()
{
    // Set the light type
    setLightType( POINT );

    // Read light's attributes
    Config cfg("config.txt");
    cfg.loadBlock("point_light");
    cfg.getFloat("constant_att", &m_fConstantAttenuation);
    cfg.getFloat("linear_att",   &m_fLinearAttenuation);

    // Read material 
    float vfAmbient[4], vfDiffuse[4], vfEmissive[4];
    cfg.getFloat("ambient",   vfAmbient, 4);
    cfg.getFloat("diffuse",   vfDiffuse, 4);
    cfg.getFloat("emissive", vfEmissive, 4);

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color(vfAmbient) );
    cMat->setDiffuse( Color(vfDiffuse) );
    cMat->setEmission( Color(vfEmissive) );
    setComponent( cMat );

    // Read light attributes
    float fRadius;
    int iDetail;
    cfg.getFloat("radius", &fRadius);
    cfg.getInt("detail", &iDetail);

    // Initialize visual component
    setComponent( new OCVisualVertexArraySphere( fRadius, iDetail, iDetail ) );
}