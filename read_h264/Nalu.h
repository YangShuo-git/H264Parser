#ifndef _NALU_H_
#define _NALU_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "EBSP.h"

//用来存放读取出来的 NALU
class Nalu {
public:
    Nalu();
    ~Nalu();

    //把NALU的数据拷贝到这个对象中
    int setBuf(uint8_t * _buf, int _len);

    //提取EBSP
    int getEBSP(EBSP &ebsp);

public:
    uint8_t* m_naluBuf = nullptr;  //指向nalu数据
    int m_naluLen = 0;             //nalu长度
    int m_startCodeLen = 0;        //起始码的长度
};

#endif // _NALU_H_
