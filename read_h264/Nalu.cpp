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

int Nalu::setBuf(uint8_t * _buf, int _len)
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

int Nalu::getEBSP(EBSP &ebsp)
{
    ebsp.len = m_naluLen - m_startCodeLen;
    ebsp.buf = (uint8_t *)malloc(ebsp.len);
    memcpy(ebsp.buf, m_naluBuf + m_startCodeLen, ebsp.len);  //拷贝之前，记得给buf分配内存

    return 0;
}