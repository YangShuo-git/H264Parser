#include "AnnexBReader.h"

AnnexBReader::AnnexBReader(std::string & _filePath)
{
    m_filePath = _filePath;
}

AnnexBReader::~AnnexBReader()
{
    Close();
}

// 用来打开文件
int AnnexBReader::Open()
{
    m_file = fopen(m_filePath.c_str(), "rb");
    if(m_file == nullptr){
        return -1;
    }
    return 0;
}

// 用来关闭文件
int AnnexBReader::Close()
{
    if(m_file != nullptr){
        fclose(m_file);
        m_file = nullptr;
    }
    if(m_buffer != nullptr){
        free(m_buffer);
        m_buffer = nullptr;
    }
    return 0;
}

int AnnexBReader::ReadFromFile()
{
    int tempBufferLen = 1024;   
    uint8_t * tempbuf = (uint8_t *) malloc (tempBufferLen);
    int readedLen = fread(tempbuf, 1, tempBufferLen, m_file);//读取1KB的数据（这里是循环读1024个1字节的数据，即1KB）这里会记忆上次读到的位置吗？

    if(readedLen > 0){
        // 将新读取的 tempbuf 添加到旧的 m_buffer 之后  拼接在一起后，再传给m_buffer
        uint8_t * _buffer = (uint8_t *) malloc (m_bufferLen + tempBufferLen);  
        memcpy(_buffer, m_buffer, m_bufferLen);
        memcpy(_buffer + m_bufferLen, tempbuf, tempBufferLen);

        if(m_buffer != nullptr){
            free(m_buffer);
            m_buffer = nullptr;
        }

        m_buffer = _buffer;
        //m_bufferLen = m_bufferLen + tempBufferLen;
        m_bufferLen += tempBufferLen;
    }
    
    free(tempbuf);   //为什么不释放_buffer?

    return readedLen;
}

//用来检查这个 buffer 的开头是不是 start code
//如果是 start code，那么返回 true，并且会将 startCodeLen 赋值成 start code 的长度 （3 或者 4）
//传入的参数：一个引用，需要获得起始码的值
bool AnnexBReader::CheckStartCode(int & startCodeLen, uint8_t *bufPtr, int bufLen)
{
    if(bufLen <= 2){
        startCodeLen = 0;
        return false;
    }

    if(bufLen >= 4){
        if(bufPtr[0] == 0) {
            if (bufPtr[1] == 0) {
                if (bufPtr[2] == 0) {
                    if (bufPtr[3] == 1) {
                        startCodeLen = 4;  
                        return true;
                    }
                }
                if(bufPtr[2] == 1){
                    startCodeLen = 3;
                    return true;
                }
            }
        }
    }
    
    if(bufLen <= 3){
        if(bufPtr[0] == 0) {
            if (bufPtr[1] == 0) {
                if(bufPtr[2] == 1){
                    startCodeLen = 3;
                    return true;
                }
            }
        }
    }

    startCodeLen = 0;
    return false;
}

// 整个读取 NALU 的过程是这样的：
// 1.先从文件中读取一个 buffer 到内存中
// 2.然后遍历这个 buffer，遇到 startcode，就从这段 buffer 中截断出一个 NALU
// 当内存中的 buffer 用尽后，我们再从文件中继续读取，直到读到文件末尾
int AnnexBReader::ReadNalu(Nalu & nalu)
{
    while(1){
        if(m_bufferLen <= 0){
            int readedLen = ReadFromFile();
            if(readedLen <= 0){
                isEnd = true;
            }
        }

        uint8_t * tmpBuf = m_buffer;

        int startCodeLen = 0;
        // Find Start Code
        bool isStartCode = CheckStartCode(startCodeLen, tmpBuf, m_bufferLen);   // 这里为什么不可以直接用m_buffer?
        if(!isStartCode){
            break;
        }

        nalu.m_startCodeLen = startCodeLen;

        // Find End Code
        int endPos = -1;
        for(int i = 2; i < m_bufferLen; i++){    // i=2的原因：需要跳过第一个起始码，才可以找到下一个起始码
            int startCodeLen = 0;
            bool isStartCode = CheckStartCode(startCodeLen, tmpBuf + i, m_bufferLen - i);
            if(isStartCode){
                endPos = i;
                break;
            }
        }

        // 找到了下一个起始码，把数据复制到NALU类中
        // 并为下一次读取作准备
        if(endPos > 0){
            nalu.SetBuf(m_buffer, endPos);   // 这里的nalu数据包括了起始码

            uint8_t * _buffer = (uint8_t*)malloc(m_bufferLen - endPos);
            memcpy(_buffer, m_buffer + endPos, m_bufferLen - endPos);

            if(m_buffer != nullptr){
                free(m_buffer);
                m_buffer = nullptr;
            }
            m_buffer = _buffer;
            m_bufferLen = m_bufferLen - endPos;

            return 0;
        }
        else{
            if(isEnd == true){
                // 到达文件末尾，取所有 buffer 出来
                nalu.SetBuf(m_buffer, m_bufferLen);
                if(m_buffer != nullptr){
                    free(m_buffer);
                    m_buffer = nullptr;
                }
                m_buffer = nullptr;
                m_bufferLen = 0;

                return 0;
            }
            int readedLen = ReadFromFile();
            if(readedLen <= 0){
                isEnd = true;
            }
        }
    }

    return -1;
}

