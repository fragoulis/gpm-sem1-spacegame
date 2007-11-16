#include "VisualVertexArray.h"

namespace tlib
{

    OCVisualVertexArray::~OCVisualVertexArray() {
	    delete [] m_VertexArray;
	    delete [] m_NormalArray;
	    delete [] m_TexArray;
	    delete [] m_IndexArray;
    }

} // end of namespace tlib