#include <windows.h>
#include <gl/gl.h>
#include "Minimap.h"
#include "TextureMgr.h"
#include "Config.h"
using tlib::Config;

void Minimap::init()
{
    Config cfg("config.txt");
    cfg.loadBlock("minimap");

    string sTexture;
    cfg.getString("background", sTexture);

    m_uiBkgTexId = TextureMgr::Instance().getTexture( sTexture.c_str() );
}

void Minimap::render() const
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_uiBkgTexId );
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
    {
        glColor4f( 1.0f, 1.0f, 1.0f, 0.7f );
        glTexCoord2f(0,0); glVertex2f( -m_fSideSize, -m_fSideSize );
        glTexCoord2f(1,0); glVertex2f(  m_fSideSize, -m_fSideSize );
        glTexCoord2f(1,1); glVertex2f(  m_fSideSize,  m_fSideSize );
        glTexCoord2f(0,1); glVertex2f( -m_fSideSize,  m_fSideSize );
    }
    glEnd();
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}