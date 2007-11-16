#pragma once
#include "Object.h"
using tlib::Object;
using tlib::Quatf;

class Blade : public Object
{
private:
    // The rotation delta 
    // The object will rotate that much every frame
    Quatf m_qRotFactor;
    float m_fRotFactor;

public:
    /** 
     * Initializes the rotating blade system part
     */
    void init();

    /** 
     * Getter/Setter for rotation factor
     */
    const Quatf& getRotFactor() const {
        return m_qRotFactor;
    }

    void setRotFactor( const Quatf& qRotFactor ) {
        m_qRotFactor = qRotFactor;
    }

    bool isRotFactor() const {
        return (m_fRotFactor<1e-6f) ? true : false;
    }

    /**
     * 
     */
    void slowDown();

    /**
     * Updates the blades rotation
     */
    void update();
};