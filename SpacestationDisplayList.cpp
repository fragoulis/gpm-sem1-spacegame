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
    const float numOfTiles = (float)m_iNumOfTiles/5;
    glBegin(GL_QUADS);
    {
        // back face
        glNormal3f( 0.0f, 0.0f, 1.0f );
        {
            // This is the face of the spaceship with the entrance
            // so it gets a little more complex to draw the 
            // face. We just create four quads instead of a big one
            // in order to leave a hole in the middle
            const float fTileSize = (float)m_iTileSize;
            const float fHalfTiles = numOfTiles * 0.5f;
            glTexCoord2f( 0.0f, 0.0f );
            glVertex3f( -halfFace, -halfFace, halfFace );
            glTexCoord2f( fHalfTiles+1, 0.0f ); 
            glVertex3f( fTileSize, -halfFace, halfFace );
            glTexCoord2f( fHalfTiles+1, fHalfTiles ); 
            glVertex3f( fTileSize, 0.0f, halfFace );
            glTexCoord2f( 0.0f, fHalfTiles ); 
            glVertex3f( -halfFace, 0.0f, halfFace );

            glTexCoord2f( 0.0f, 0.0f );
            glVertex3f( -halfFace, 0.0f, halfFace );
            glTexCoord2f( fHalfTiles, 0.0f ); 
            glVertex3f( 0.0f, 0.0f, halfFace );
            glTexCoord2f( fHalfTiles, fHalfTiles ); 
            glVertex3f( 0.0f, halfFace, halfFace );
            glTexCoord2f( 0.0f, fHalfTiles ); 
            glVertex3f( -halfFace, halfFace, halfFace );

            glTexCoord2f( 0.0f, 0.0f );
            glVertex3f( 0.0f, fTileSize, halfFace );
            glTexCoord2f( fHalfTiles, 0.0f ); 
            glVertex3f( halfFace, fTileSize, halfFace );
            glTexCoord2f( fHalfTiles, fHalfTiles-1 ); 
            glVertex3f( halfFace, halfFace, halfFace );
            glTexCoord2f( 0.0f, fHalfTiles-1 ); 
            glVertex3f( 0.0f, halfFace, halfFace );

            glTexCoord2f( 0.0f, 0.0f );
            glVertex3f( fTileSize, -halfFace, halfFace );
            glTexCoord2f( fHalfTiles-1, 0.0f ); 
            glVertex3f( halfFace, -halfFace, halfFace );
            glTexCoord2f( fHalfTiles-1, fHalfTiles+1 ); 
            glVertex3f( halfFace, fTileSize, halfFace );
            glTexCoord2f( 0.0f, fHalfTiles+1 ); 
            glVertex3f( fTileSize, fTileSize, halfFace );
        }

        // front face
        glNormal3f( 0.0f, 0.0f, -1.0f );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex3f( -halfFace, halfFace, -halfFace );

        glTexCoord2f( numOfTiles, 0.0f ); 
        glVertex3f( halfFace, halfFace, -halfFace );

        glTexCoord2f( numOfTiles, numOfTiles ); 
        glVertex3f( halfFace, -halfFace, -halfFace );

        glTexCoord2f( 0.0f, numOfTiles ); 
        glVertex3f( -halfFace, -halfFace, -halfFace );

        // left face
        glNormal3f( -1.0f, 0.0f, 0.0f );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex3f( -halfFace, -halfFace, -halfFace );

        glTexCoord2f( numOfTiles, 0.0f ); 
        glVertex3f( -halfFace, -halfFace, halfFace );

        glTexCoord2f( numOfTiles, numOfTiles ); 
        glVertex3f( -halfFace, halfFace, halfFace );

        glTexCoord2f( 0.0f, numOfTiles ); 
        glVertex3f( -halfFace, halfFace, -halfFace );

        // right face
        glNormal3f( 1.0f, 0.0f, 0.0f );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex3f( halfFace, -halfFace, halfFace );

        glTexCoord2f( numOfTiles, 0.0f ); 
        glVertex3f( halfFace, -halfFace, -halfFace );

        glTexCoord2f( numOfTiles, numOfTiles ); 
        glVertex3f( halfFace, halfFace, -halfFace );

        glTexCoord2f( 0.0f, numOfTiles ); 
        glVertex3f( halfFace, halfFace, halfFace );

        // top face
        glNormal3f( 0.0f, 1.0f, 0.0f );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex3f( -halfFace, halfFace, halfFace );

        glTexCoord2f( numOfTiles, 0.0f ); 
        glVertex3f( halfFace, halfFace, halfFace );

        glTexCoord2f( numOfTiles, numOfTiles ); 
        glVertex3f( halfFace, halfFace, -halfFace );

        glTexCoord2f( 0.0f, numOfTiles ); 
        glVertex3f( -halfFace, halfFace, -halfFace );

        // bottom face
        glNormal3f( 0.0f, -1.0f, 0.0f );
        glTexCoord2f( 0.0f, 0.0f );
        glVertex3f( -halfFace, -halfFace, -halfFace );

        glTexCoord2f( numOfTiles, 0.0f ); 
        glVertex3f( halfFace, -halfFace, -halfFace );

        glTexCoord2f( numOfTiles, numOfTiles ); 
        glVertex3f( halfFace, -halfFace, halfFace );

        glTexCoord2f( 0.0f, numOfTiles ); 
        glVertex3f( -halfFace, -halfFace, halfFace );
    }
    glEnd();

} // end buildObject()