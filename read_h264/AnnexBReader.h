#ifndef _ANNEXBREADER_H_
#define _ANNEXBREADER_H_


#include <stdint.h>
#include <string>

#include "Nalu.h"


//设计一个读取工具类
//在这个类中，我们可以通过构造函数设置 AnnexB 的文件路径
//然后循环调用 ReadNalu 函数，不断地读出 NALU，直到 ReadNalu 函数返回读取结束
class AnnexBReader
{
public:
    AnnexBReader(std::string & _filePath);
    ~AnnexBReader();

    // 打开文件
    int Open();

    // 关闭文件
    int Close();

    // 读取一个Nalu
    int ReadNalu(Nalu &nalu);

private:
    bool CheckStartCode(int &startCodeLen, uint8_t * bufPtr, int bufLen);
    int ReadFromFile();

    std::string m_filePath;
    FILE * m_file = nullptr;

    bool isEnd = false;
    uint8_t *m_buffer = nullptr;   //从file中读取的数据缓冲区
    int m_bufferLen = 0;           //从file中读取的数据长度
};

#endif // _ANNEXBREADER_H_