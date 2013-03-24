#ifndef MFC_H
#define MFC_H

#include "SsbSipMfcApi.h"

class MFC
{
public:
    MFC();
    SSBSIP_MFC_ERROR_CODE initMFC(int w, int h, int qb);
    int getHeader(unsigned char** p);
    void getInputBuf(void **Y,void **UV);
    int encode(void **h264);
    void closeMFC();
private:
    void *hOpen;
    unsigned char header[100];
    int headerSize;
    SSBSIP_MFC_ENC_H264_PARAM *param;
    int width, height;
    SSBSIP_MFC_CODEC_TYPE codec_type;
    SSBSIP_MFC_ENC_INPUT_INFO input_info;
    SSBSIP_MFC_ENC_OUTPUT_INFO output_info;
};

#endif // MFC_H
