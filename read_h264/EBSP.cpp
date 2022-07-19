#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "EBSP.h"

EBSP::EBSP()
{

}

EBSP::~EBSP()
{
    if(buf != nullptr){
        free(buf);
        buf = nullptr;
    }
}

int EBSP::getRBSP(RBSP & rbsp)
{
    rbsp.len = len;
    rbsp.buf = (uint8_t *)malloc(rbsp.len);

    int newIndex = 0;
    for (int i = 0; i < len; i++)
    {
        if (buf[i] == 0x03 )
        {
            // 查看前两个是不是 0x00 0x00
            if (i > 2) //nalu还有一个头，所以从i=3开始
            {
                if(buf[i - 1] == 0x00 && buf[i - 2] == 0x00)
                {
                    // 判断 后一个是不是 0 1 2 3
                    if (i < len - 1)
                    {
                        if (buf[i + 1] == 0x00 || buf[i + 1] == 0x01 
                            || buf[i + 1] == 0x02 || buf[i + 1] == 0x03)
                        {
                            // 满足条件，跳过该位置，不进行拷贝
                            rbsp.len--;
                            continue;
                        }
                        
                    }
                    
                }
            }
            
        }
        rbsp.buf[newIndex] = rbsp.buf[i];
        newIndex++;
    }
    return 0;
}
