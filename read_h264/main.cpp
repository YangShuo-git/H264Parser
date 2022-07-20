#include "AnnexBReader.h"

int main(int argc, char const *argv[])
{   
    FILE* outputFile = nullptr;    //将数据输出到该文件句柄中 
    outputFile = fopen("nalu.txt", "w");

    std::string filePath = "./phone.h264";
    AnnexBReader reader(filePath);
    
    int ret = reader.open();
    if(ret){
        printf("Fail to open read file!\n"); 
        return -1;
    }

    while(1){
        Nalu nalu;
        ret = reader.ReadNalu(nalu);
        if(ret){
            break;  
        }
        fprintf(outputFile, "Start Code Len: %d\t NALU Buffer Len: %d\n", nalu.m_startCodeLen, nalu.m_naluLen);
        fprintf(outputFile, "%x %x %x %x %x\n", nalu.m_naluBuf[0], nalu.m_naluBuf[1], nalu.m_naluBuf[2], nalu.m_naluBuf[3], nalu.m_naluBuf[4]);

        EBSP ebsp;
        ret = nalu.getEBSP(ebsp);
        if (ret)
        {
            break;
        }


        RBSP rbsp;
        ret = ebsp.getRBSP(rbsp);
        if (ret)
        {
            break;
        }

        uint8_t naluHeader = rbsp.buf[0];
        int forbidden_bit = (naluHeader >> 7) & 0x01;
        int nal_ref_idc   = (naluHeader >> 5) & 0x03;
        int nal_unit_type = (naluHeader >> 0) & 0x1f;
    }

    reader.close();
    fclose(outputFile);

    return 0;
}