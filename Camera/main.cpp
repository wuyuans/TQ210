#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "camera.h"

int main(int argc, char ** argv) {
    char *dev_name = "/dev/video0";
    FILE * outf = 0;
    unsigned int image_size;
    outf = fopen("out.yuv", "wb");
    Camera *camera;
    unsigned char image[480*320*3];

    camera=new Camera(dev_name,480,320);
    if(!camera->OpenDevice()){
        return -1;
    }
    image_size=camera->getImageSize();
    for(int i=0;i<50;i++){
        camera->GetBuffer(image);
        fwrite(image,1,image_size,outf);
    }
    camera->CloseDevice();
    fclose(outf);
    return 0;
}
