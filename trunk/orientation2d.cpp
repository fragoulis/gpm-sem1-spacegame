#include "orientation2d.h"
#include "object.h"

namespace tlib 
{
    
    // ----------------------------------------------------------------------------
    void OCOrientation2D::rotate( const float fAngle, const Vector3f &vAxis )
    {   
        Quatf qRotation( fAngle, vAxis );
        qRotation.toRotation();
        
        // rotate that viewing vector
        Quatf qView( 0.0f, m_vView );
        Quatf qResult = qRotation * qView * (~qRotation);

        // update the reference point ...
        m_vView = qResult.xyz();
        m_vView.normalize();

        // rotate the 'up' vector accordingly
        Quatf qUp( 0.0f, m_vUp );
        qResult = qRotation * qUp * (~qRotation);
        
        // update the up vector ...
        m_vUp = qResult.xyz();
        m_vUp.normalize();

    } // end rotate()

    // ----------------------------------------------------------------------------
    void OCOrientation2D::pitch( const float fAngle )
    {
        Vector3f vRight = m_vView.cross( m_vUp );
        rotate( fAngle, vRight );
    }

    // ----------------------------------------------------------------------------
    void OCOrientation2D::yaw( const float fAngle )
    {
        rotate( fAngle, m_vUp );
    }

    // ----------------------------------------------------------------------------
    void OCOrientation2D::roll( const float fAngle )
    {
        rotate( fAngle, m_vView );
    }

    // ----------------------------------------------------------------------------
    void OCOrientation2D::positionAt( const Vector3f &vNewPos )
    {
        const Vector3f& vCenter = getOwner()->getPos() + m_vView;

        // Setup the new viewing vector
        Vector3f vNewView( vCenter - vNewPos );
        vNewView.normalize();

        // Update the up vector
        _updateUpVector( vNewView );

        getOwner()->setPos( vNewPos );
    }

    // ----------------------------------------------------------------------------
    void OCOrientation2D::lookAt( const Vector3f &vNewPos )
    {
        const Vector3f& vEye = getOwner()->getPos();

        // Setup the new viewing vector
        Vector3f vNewView( vNewPos - vEye );
        vNewView.normalize();

        // Update the up vector
        _updateUpVector( vNewView );
    }

    // ----------------------------------------------------------------------------
    void OCOrientation2D::positionAtAndLookAt( const Vector3f &vNewPos, 
                                               const Vector3f &vLookAt )
    {
        // setup the new viewing vector
        Vector3f vNewView( vLookAt - vNewPos );
        vNewView.normalize();

        // udpate the 'up' vector
        _updateUpVector( vNewView );

        // update camera position and target
        getOwner()->setPos( vNewPos );
    }

    // ----------------------------------------------------------------------------
    void OCOrientation2D::_updateUpVector( Vector3f &vNewView )
    {
        m_vView.normalize();

        // The two vectors are in one plane. This gives us the cosine of the angle between
        // these two vectors.
        float dotProd = vNewView.dot( m_vView );
        if( dotProd < -1.0f || dotProd > 1.0f )
            return;

        // By getting the arc cosine of this, we get the angle in radians between the two
        // vectors. But we only get an angle between 0 and pi, that will be a problem.
        float alpha = acosf(dotProd);

        // Here we get the normal to these two vectors, we will need to rotate our front
        // vector around this normal.
        Vector3f normal = vNewView.cross( m_vView );
        normal.normalize();

        m_vUp.selfRotate( alpha, normal );

        // As I said, we have a problem: as we only get an angle between 0 and pi,
        // we don't know if we need to rotate clockwise or counterclockwise ...
        // We solve this problem by just trying it out.
        Vector3f viewTest = m_vView.rotate( alpha, normal );

        // If the dot product is near to one, it means we were right. with our guess
        // rotating counterclockwise.
        // Else, we must rotate clockwise.
        dotProd = viewTest.dot( vNewView );
        if( fabs(dotProd) > 0.999f ) {
                m_vView = viewTest;
                m_vUp = m_vUp.rotate( alpha, normal );
        } else {
                m_vView.selfRotate( -alpha, normal );
                m_vUp = m_vUp.rotate( -alpha, normal );
        }
        m_vView = vNewView; /// <-???????? what is this????

        // If the dot product is near to -1, it means we look the wrong way.
        // To correct this, we just rotate pi radians (a half circle).
        if( dotProd < -0.999f )
            m_vView.selfRotate( (float)M_PI, m_vUp );

    } // end _updateUpVector()

} // end of namespace tlib