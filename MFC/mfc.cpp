#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "mfc.h"

MFC::MFC()
{    
}

SSBSIP_MFC_ERROR_CODE MFC::initMFC(int w, int h, int qb)
{
    SSBSIP_MFC_ERROR_CODE ret;

    width=w;
    height=h;
    param = (SSBSIP_MFC_ENC_H264_PARAM*)malloc(sizeof(SSBSIP_MFC_ENC_H264_PARAM));
    memset(param, 0 , sizeof(SSBSIP_MFC_ENC_H264_PARAM));
    param->SourceWidth=width;
    param->SourceHeight=height;
    param->ProfileIDC=1;
    param->LevelIDC=40;
    param->IDRPeriod=3;
    param->NumberReferenceFrames=1;
    param->NumberRefForPframes=2;
    param->SliceMode=0;
    param->SliceArgument=1;
    param->NumberBFrames=0;
    param->LoopFilterDisable=0;
    param->LoopFilterAlphaC0Offset=2;
    param->LoopFilterBetaOffset=1;
    param->SymbolMode=0;
    param->PictureInterlace=0;
    param->Transform8x8Mode=0;
    param->RandomIntraMBRefresh=0;
    param->PadControlOn=0;
    param->LumaPadVal=0;
    param->CbPadVal=0;
    param->CrPadVal=0;
    param->EnableFRMRateControl=0;
    param->EnableMBRateControl=0;
    param->FrameRate=25;
    param->Bitrate=200;
    param->FrameQp=qb;
    param->QSCodeMax=0;
    param->QSCodeMin=0;
    param->CBRPeriodRf=20;
    param->DarkDisable=1;
    param->SmoothDisable=1;
    param->StaticDisable=1;
    param->ActivityDisable=1;

    param->codecType=H264_ENC;

    param->FrameQp_P = param->FrameQp+1;
    param->FrameQp_B = param->FrameQp+3;

    hOpen = SsbSipMfcEncOpen();
    if(hOpen == NULL)
    {
        printf("SsbSipMfcEncOpen Failed\n");
        ret = MFC_RET_FAIL;
        return ret;
    }

    if(SsbSipMfcEncInit(hOpen, param) != MFC_RET_OK)
    {
        printf("SsbSipMfcEncInit Failed\n");
        ret = MFC_RET_FAIL;
        goto out;
    }

    if(SsbSipMfcEncGetInBuf(hOpen, &input_info) != MFC_RET_OK)
    {
        printf("SsbSipMfcEncGetInBuf Failed\n");
        ret = MFC_RET_FAIL;
        goto out;
    }

    ret=SsbSipMfcEncGetOutBuf(hOpen, &output_info);
    if(output_info.headerSize <= 0)
    {
        printf("Header Encoding Failed\n");
        ret = MFC_RET_FAIL;
        goto out;
    }
    headerSize=output_info.headerSize;
    memcpy(header,output_info.StrmVirAddr,headerSize);
    printf("MFC init success:: Yphy(0x%08x) Cphy(0x%08x)\n",
           input_info.YPhyAddr, input_info.CPhyAddr);
    return ret;
out:
    SsbSipMfcEncClose(hOpen);
    return ret;
}

int MFC::getHeader(unsigned char **p)
{
    //memcpy(*p,header,headerSize);
    *p=header;
    return headerSize;
}

void MFC::getInputBuf(void **Y,void **UV)
{
    *Y=input_info.YVirAddr;
    *UV=input_info.CVirAddr;
}

int MFC::encode(void **h264)
{

    if(SsbSipMfcEncExe(hOpen) != MFC_RET_OK){
        printf("Encoding Failed\n");
        return 0;
    }
    SsbSipMfcEncGetOutBuf(hOpen, &output_info);
    if(output_info.StrmVirAddr == NULL)
    {
        printf("SsbSipMfcEncGetOutBuf Failed\n");
        return 0;
    }
    *h264=output_info.StrmVirAddr;
    return output_info.dataSize;
}

void MFC::closeMFC()
{
    SsbSipMfcEncClose(hOpen);
}
