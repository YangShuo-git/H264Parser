#ifndef _EBSP_H_
#define _EBSP_H_

#include <stdint.h>
#include "RBSP.h"

class EBSP {
public:
    EBSP();
    ~EBSP();

    int getRBSP(RBSP & rbsp);

public:
    //表示 EBSP 的数据和长度
    uint8_t * buf = nullptr;
    int len = 0;
};


#endif // _EBSP_H_