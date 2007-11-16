#pragma once
#include "VisualVertexArray.h"

namespace tlib
{

    /**
     * Code for this class and the sphere is taken from the tutorials
     */
    class OCVisualVertexArraySphere : public OCVisualVertexArray
    {
    private:           
        // Sphere's radius
        float m_fRadius;

        // Sphere attributes
	    int m_iStacks, m_iSlices;

    public:
        /**
         * Constructor
         */
        OCVisualVertexArraySphere( float fRadius,
                                   int iStacks, int iSlices );

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("vertexarraysphere"); 
        }

        /**
         * Renders the sphere
         */
        void render() const;

        /**
         * Creates the sphere
         */
        bool create();

    }; // end of OCVisualVertexArraySphere class

} // end of namespace tlib