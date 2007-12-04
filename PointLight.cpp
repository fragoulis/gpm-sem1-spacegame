#include "PointLight.h"
#include "SimpleMaterial.h"
#include "VisualVertexArraySphere.h"
using tlib::OCSimpleMaterial;
using tlib::OCVisualVertexArraySphere;

PointLight::PointLight()
{
    // Set the light type
    setLightType( POINT );

    // Initialize material component
    OCSimpleMaterial *cMat = new OCSimpleMaterial;
    cMat->setAmbient( Color::black() );
    cMat->setDiffuse( Color::black() );
    cMat->setEmission( Color::yellow() );
    setComponent( cMat );

    // Initialize visual component
    setComponent( new OCVisualVertexArraySphere( 1, 5, 5 ) );
}