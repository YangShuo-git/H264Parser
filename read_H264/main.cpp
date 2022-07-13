#include "AnnexBReader.h"

int main(int argc, char const *argv[])
{
    std::string filePath = "./brave.h264";
    AnnexBReader reader(filePath);
    
    int ret = reader.Open();
    if(ret){
        printf("Read Fail"); 
        return -1;
    }

    //FILE* outputFile;
    //outputFile = fopen("./nalu.txt", "w");
    while(1){
        Nalu nalu;
        ret = reader.ReadNalu(nalu);
        if(ret){
            break;  
        }
        printf("=====================\n");
        printf("Start Code Len: %d\n", nalu.m_startCodeLen);
        printf("NALU Buffer Len: %d\n", nalu.m_len);
        printf("%d %d %d %d %d\n", nalu.m_naluBuf[0], nalu.m_naluBuf[1], nalu.m_naluBuf[2], nalu.m_naluBuf[3], nalu.m_naluBuf[4]);

        //fprintf(outputFile, "%d %d %d %d %x\n", nalu.m_naluBuf[0], nalu.m_naluBuf[1], nalu.m_naluBuf[2], nalu.m_naluBuf[3], nalu.m_naluBuf[4]);
    }

    //fclose(outputFile);
    reader.Close();

    return 0;
}