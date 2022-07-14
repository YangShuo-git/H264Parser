#ifndef EYERH264DEOCDER_NALU_HPP
#define EYERH264DEOCDER_NALU_HPP

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//用来存放读取出来的 NALU
class Nalu {
public:
    Nalu();
    ~Nalu();

    //把NALU的数据拷贝到这个对象中
    int SetBuf(uint8_t * _buf, int _len);

public:
    uint8_t* m_naluBuf = nullptr;  //指向nalu数据
    int m_len = 0;                 //nalu长度
    int m_startCodeLen = 0;        //起始码的长度
};

#endif //EYERH264DEOCDER_NALU_HPP
