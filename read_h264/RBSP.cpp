#include <stdlib.h>
#include "RBSP.h"

RBSP::RBSP()
{

}

RBSP::~RBSP()
{
    if(buf != nullptr){
        free(buf);
        buf = nullptr;
    }
}