#pragma once
#include "Visual.h"

namespace tlib
{

    /**
     * Code for this class and the sphere is taken from the tutorials
     */
    class OCVisualVertexArray : public IOCVisual
    {
    protected:    
        // The object's vertices
	    float (*m_VertexArray)[3];
        
        // The vertices' normals
	    float (*m_NormalArray)[3];

        // The object's texture coordinates
	    float (*m_TexArray)[2];

        // The object's index array that defines the polygon
        unsigned int *m_IndexArray;

    public:
        /**
         * Destructor
         */
        virtual ~OCVisualVertexArray();

        /**
         * Returns the unique component ID
         */
        const string componentID() const { 
            return string("vertexarray"); 
        }

        /**
         * Creates the polygon
         */
        virtual bool create() = 0;

    }; // end of OCVisualVertexArray class

} // end of namespace tlib