#include "AnnexBReader.h"

int main(int argc, char const *argv[])
{
    std::string filePath = "./phone.h264";
    AnnexBReader reader(filePath);
    
    int ret = reader.Open();
    if(ret){
        printf("Read Fail\n"); 
        return -1;
    }

    // FILE* outputFile;
    // outputFile = fopen("nalu.txt", "w");
    while(1){
        Nalu nalu;
        ret = reader.ReadNalu(nalu);
        if(ret){
            break;  
        }
        // printf("=====================\n");
        // printf("Start Code Len: %d\n", nalu.m_startCodeLen);
        // printf("NALU Buffer Len: %d\n", nalu.m_naluLen);
        // printf("%x %x %x %x %x\n", nalu.m_naluBuf[0], nalu.m_naluBuf[1], nalu.m_naluBuf[2], nalu.m_naluBuf[3], nalu.m_naluBuf[4]);
        // fprintf(outputFile, "%x %x %x %x %x\n", nalu.m_naluBuf[0], nalu.m_naluBuf[1], nalu.m_naluBuf[2], nalu.m_naluBuf[3], nalu.m_naluBuf[4]);
    
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

    //fclose(outputFile);
    reader.Close();

    return 0;
}