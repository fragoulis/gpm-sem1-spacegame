#include "CollisionGXModel.h"
#include "VisualGXModel.h"

namespace tlib 
{
    OCCollisionGXModel::OCCollisionGXModel() {
        OCCollisionDynamicBBox::OCCollisionDynamicBBox();
    }

    void OCCollisionGXModel::initBBox()
    {
        // Save the object's bounding box here
        OCGXModel *cVisual = (OCGXModel*)m_oOwner->getComponent("visual");
        m_BBox.xyz( cVisual->getGXModel().GetBoxSize().x * 0.5f,
                    cVisual->getGXModel().GetBoxSize().y * 0.5f,
                    cVisual->getGXModel().GetBoxSize().z * 0.5f );
    }

} // end of namespace tlib