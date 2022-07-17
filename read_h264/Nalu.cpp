#include "Nalu.h"

Nalu::Nalu()
{

}

Nalu::~Nalu()
{
    if(m_naluBuf != nullptr){
        free(m_naluBuf);
        m_naluBuf = nullptr;
    }
}

int Nalu::SetBuf(uint8_t * _buf, int _len)
{
    if(m_naluBuf != nullptr){
        free(m_naluBuf);
        m_naluBuf = nullptr;
    }
    m_naluLen = _len;

    m_naluBuf = (uint8_t *)malloc(m_naluLen);
    memcpy(m_naluBuf, _buf, m_naluLen);

    return 0;
}
