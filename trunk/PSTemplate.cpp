#include "PSTemplate.h"
#include "TextureMgr.h"
#include "PSCommon.h"
#include "Config.h"
#include "Logger.h"
#include <string>
using std::string;
using tlib::Config;
using tlib::Logger;

// ----------------------------------------------------------------------------
PSTemplate::PSTemplate( const char *block )
{
    _LOG("Initializing particle template of type "+ string(block));

    // Open configuration file
    Config cfg("config.txt");
    cfg.loadBlock( string(block) );

    // Read release time and count
    cfg.getDouble("release_time", &m_dReleaseTime);
    cfg.getInt("release_count", &m_iReleaseCount);

    // Read particles' velocity, size and lifespan limits
    cfg.getFloat("size", m_fSize, 2);
    cfg.getInt("lifespan", m_iLifeSpan, 2);
    cfg.getFloat("velocity", m_fVelocity, 2);

    // Read particles' start and end color
    float vfEndColor[4];
    cfg.getFloat("start_color", m_vfColor, 4);
    cfg.getFloat("end_color", vfEndColor, 4);

    for( int i=0; i<4; ++i )
        m_vfColorFactor[i] = vfEndColor[i] - m_vfColor[i];

    // Read system's lifespan
    cfg.getDouble("sys_lifespan", &m_dSysDuration);

    // Read particle number
    cfg.getInt("max_particles", &m_iNumOfParticles);

    // Read particle direction cone
    cfg.getFloat("dir_x", m_vfDirection[0], 2);
    cfg.getFloat("dir_y", m_vfDirection[1], 2);
    cfg.getFloat("dir_z", m_vfDirection[2], 2);

    // Read texture
    string sTexture;
    cfg.getString("texture", sTexture);
    // Set the texture format to alpha
    TextureMgr::Instance().setFormat( Image::ALPHA );
    // Generate the texture using the texture manager and save the id
    m_uiTexId = TextureMgr::Instance().getTexture( sTexture.c_str() );
}

// ----------------------------------------------------------------------------
void PSTemplate::copyTo( PSCommon &ps ) const
{
    // memcpy's maybe faster?
    ps.m_fSize[0] = m_fSize[0];
    ps.m_fSize[1] = m_fSize[1];

    ps.m_fVelocity[0] = m_fVelocity[0];
    ps.m_fVelocity[1] = m_fVelocity[1];

    ps.m_vfColor[0] = m_vfColor[0];
    ps.m_vfColor[1] = m_vfColor[1];
    ps.m_vfColor[2] = m_vfColor[2];
    ps.m_vfColor[3] = m_vfColor[3];

    ps.m_vfColorFactor[0] = m_vfColorFactor[0];
    ps.m_vfColorFactor[1] = m_vfColorFactor[1];
    ps.m_vfColorFactor[2] = m_vfColorFactor[2];
    ps.m_vfColorFactor[3] = m_vfColorFactor[3];

    ps.m_vfDirection[0][0] = m_vfDirection[0][0];
    ps.m_vfDirection[0][1] = m_vfDirection[0][1];
    ps.m_vfDirection[1][0] = m_vfDirection[1][0];
    ps.m_vfDirection[1][1] = m_vfDirection[1][1];
    ps.m_vfDirection[2][0] = m_vfDirection[2][0];
    ps.m_vfDirection[2][1] = m_vfDirection[2][1];

    ps.m_iReleaseCount = m_iReleaseCount;

    ps.m_dReleaseTime = m_dReleaseTime;
    ps.m_dSysDuration = m_dSysDuration;

    ps.m_iLifeSpan[0] = m_iLifeSpan[0];
    ps.m_iLifeSpan[1] = m_iLifeSpan[1];

    ps.m_iNumOfParticles = m_iNumOfParticles;

    ps.m_uiTexId = m_uiTexId;
}