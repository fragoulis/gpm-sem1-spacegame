#pragma once

class Minimap
{
private:
    // The minimap's background texture id
    unsigned m_uiBkgTexId;

    // Minimap size
    float m_fSideSize;

public:
    /**
     * Loads minimap's background texture
     */
    void init();

    /**
     * Mutator
     */
    void setMinimapSize( float fSide ) {
        m_fSideSize = fSide;
    }

    /**
     * Renders the interface
     */
    void render() const;
};