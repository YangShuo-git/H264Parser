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
    int readFileLen = 1024;   
    uint8_t * readFileBuf = (uint8_t *) malloc (readFileLen);
    int readedLen = fread(readFileBuf, 1, readFileLen, m_file);//读取1KB的数据（这里是循环读1024个1字节的数据，即1KB）这里会记忆上次读到的位置吗？
    if(readedLen > 0){
        // 将 新读取的readFileBuf 和 旧的m_buffer 拼接在一起 （借助一个tmpBuf）
        uint8_t * tmpbuf = (uint8_t *) malloc (m_bufferLen + readFileLen);  
        memcpy(tmpbuf, m_buffer, m_bufferLen);
        memcpy(tmpbuf + m_bufferLen, readFileBuf, readFileLen);

        if(m_buffer != nullptr){
            free(m_buffer);
            m_buffer = nullptr;
        }

        m_buffer = tmpbuf;   //tmpbuf与m_buffer指向同一块内存，之后会释放m_buffer
        m_bufferLen += readFileLen;
    }
    
    free(readFileBuf);   
    readFileBuf = nullptr;
    
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

        uint8_t * tmpBuf = m_buffer;   // 这里为什么要用一个临时buffer？

        int startCodeLen = 0;
        // Find Start Code
        bool isStartCode = CheckStartCode(startCodeLen, tmpBuf, m_bufferLen);  
        if(!isStartCode){
            break;    // 从这里结束读取
        }
        nalu.m_startCodeLen = startCodeLen;

        // Find End Code  也就是下一个起始码
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

            uint8_t * leftBuf = (uint8_t*)malloc(m_bufferLen - endPos);
            memcpy(leftBuf, m_buffer + endPos, m_bufferLen - endPos);

            if(m_buffer != nullptr){
                free(m_buffer);
                m_buffer = nullptr;
            }
            m_buffer = leftBuf;
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

