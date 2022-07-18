#ifndef _RBSP_H_
#define _RBSP_H_

#include <stdint.h>

class RBSP 
{
public:
    RBSP();
    ~RBSP();

public:
    uint8_t * buf = nullptr;
    int len = 0;
};

#endif // _RBSP_H_