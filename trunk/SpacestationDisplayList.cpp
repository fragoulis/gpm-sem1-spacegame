#include <windows.h>
#include <gl/gl.h>
#include "SpacestationDisplayList.h"
#include "Logger.h"
#include "Config.h"

using std::ifstream;
using tlib::Logger;
using tlib::Config;

SpacestationDisplayList::SpacestationDisplayList()
{
    _LOG("Setting up spacestation display list...");

    Config cfg("config.txt");
    cfg.loadBlock("spacestation");

    cfg.getInt("tile_size"  ,&m_iTileSize);
    cfg.getInt("tiles"      ,&m_iNumOfTiles);

    // Generate the object for the display list
    generate();
}

SpacestationDisplayList::~SpacestationDisplayList()
{
}

void SpacestationDisplayList::buildObject() const 
{
    _LOG("Building spacestation object");
    const float halfFace = (float)(m_iTileSize * m_iNumOfTiles) * 0.5f;
    const float fRepeat = 5.0f;
    glBegin(GL_QUADS);
    {
        // back face
        glNormal3f( 0.0f, 0.0f, 1.0f );
        {
            // This is the face of the spaceship with the hole in the middle
            // [the entrance] and that's why it is a bit more difficul to 
            // create it.
            // In next lines we are doing some hacks to texture map this 
            // face nicely.
            const float fTileSize = (float)m_iTileSize;
            // Left part's number of tiles
            const float nTilesRP = 0.5f * fRepeat;
            // Right part's number of tiles
            const float nTilesLP = ( ( m_iNumOfTiles * 0.5f ) - 1.0f ) * fRepeat / m_iNumOfTiles;
            // Strips percent of the texture
            const float nStrip = fRepeat / m_iNumOfTiles;

            // left part
            glTexCoord2f(     0.0f, 0.0f );    glVertex3f( -halfFace, -halfFace, halfFace );
            glTexCoord2f( nTilesLP, 0.0f );    glVertex3f(      0.0f, -halfFace, halfFace );
            glTexCoord2f( nTilesLP, fRepeat ); glVertex3f(      0.0f,  halfFace, halfFace );
            glTexCoord2f(     0.0f, fRepeat ); glVertex3f( -halfFace,  halfFace, halfFace );

            // right part
            glTexCoord2f(     0.0f, 0.0f );    glVertex3f( fTileSize, -halfFace, halfFace );
            glTexCoord2f( nTilesLP, 0.0f );    glVertex3f(  halfFace, -halfFace, halfFace );
            glTexCoord2f( nTilesLP, fRepeat ); glVertex3f(  halfFace,  halfFace, halfFace );
            glTexCoord2f(     0.0f, fRepeat ); glVertex3f( fTileSize,  halfFace, halfFace );

            // low vertical strip 
            glTexCoord2f( 3*nStrip,     0.0f ); glVertex3f(      0.0f, -halfFace, halfFace );
            glTexCoord2f( 4*nStrip,     0.0f ); glVertex3f( fTileSize, -halfFace, halfFace );
            glTexCoord2f( 4*nStrip, nTilesRP ); glVertex3f( fTileSize,      0.0f, halfFace );
            glTexCoord2f( 3*nStrip, nTilesRP ); glVertex3f(      0.0f,      0.0f, halfFace );

            // low vertical strip 
            glTexCoord2f( 3*nStrip, nTilesRP + nStrip ); glVertex3f(      0.0f, fTileSize, halfFace );
            glTexCoord2f( 4*nStrip, nTilesRP + nStrip ); glVertex3f( fTileSize, fTileSize, halfFace );
            glTexCoord2f( 4*nStrip, fRepeat );           glVertex3f( fTileSize,  halfFace, halfFace );
            glTexCoord2f( 3*nStrip, fRepeat );           glVertex3f(      0.0f,  halfFace, halfFace );

        }

        // front face
        glNormal3f( 0.0f, 0.0f, -1.0f );
        glTexCoord2f(    0.0f,    0.0f ); glVertex3f( -halfFace,  halfFace, -halfFace );
        glTexCoord2f( fRepeat,    0.0f ); glVertex3f(  halfFace,  halfFace, -halfFace );
        glTexCoord2f( fRepeat, fRepeat ); glVertex3f(  halfFace, -halfFace, -halfFace );
        glTexCoord2f(    0.0f, fRepeat ); glVertex3f( -halfFace, -halfFace, -halfFace );

        // left face
        glNormal3f( -1.0f, 0.0f, 0.0f );
        glTexCoord2f(    0.0f,    0.0f ); glVertex3f( -halfFace, -halfFace, -halfFace );
        glTexCoord2f( fRepeat,    0.0f ); glVertex3f( -halfFace, -halfFace,  halfFace );
        glTexCoord2f( fRepeat, fRepeat ); glVertex3f( -halfFace,  halfFace,  halfFace );
        glTexCoord2f(    0.0f, fRepeat ); glVertex3f( -halfFace,  halfFace, -halfFace );

        // right face
        glNormal3f( 1.0f, 0.0f, 0.0f );
        glTexCoord2f(    0.0f,    0.0f ); glVertex3f( halfFace, -halfFace,  halfFace );
        glTexCoord2f( fRepeat,    0.0f ); glVertex3f( halfFace, -halfFace, -halfFace );
        glTexCoord2f( fRepeat, fRepeat ); glVertex3f( halfFace,  halfFace, -halfFace );
        glTexCoord2f(    0.0f, fRepeat ); glVertex3f( halfFace,  halfFace,  halfFace );

        // top face
        glNormal3f( 0.0f, 1.0f, 0.0f );
        glTexCoord2f(    0.0f,    0.0f ); glVertex3f( -halfFace, halfFace,  halfFace );
        glTexCoord2f( fRepeat,    0.0f ); glVertex3f(  halfFace, halfFace,  halfFace );
        glTexCoord2f( fRepeat, fRepeat ); glVertex3f(  halfFace, halfFace, -halfFace );
        glTexCoord2f(    0.0f, fRepeat ); glVertex3f( -halfFace, halfFace, -halfFace );

        // bottom face
        glNormal3f( 0.0f, -1.0f, 0.0f );
        glTexCoord2f(    0.0f,    0.0f ); glVertex3f( -halfFace, -halfFace, -halfFace );
        glTexCoord2f( fRepeat,    0.0f ); glVertex3f(  halfFace, -halfFace, -halfFace );
        glTexCoord2f( fRepeat, fRepeat ); glVertex3f(  halfFace, -halfFace,  halfFace );
        glTexCoord2f(    0.0f, fRepeat ); glVertex3f( -halfFace, -halfFace,  halfFace );
    }
    glEnd();

} // end buildObject()