#include "common.h"
#include "vcan_img2sd.h"
#include "ff.h"
#include "VCAN_camera.h"       //����ͷ��ͷ�ļ�

static FATFS vcanfs;    //�ļ�ϵͳ
static FIL   vcansrc;   //�ļ�


void img_sd_init(void)
{
    int     vcanres;
    char    myfilename[20];
    uint16  imgsize[] = {
                            CAMERA_H,
                            CAMERA_W,
                            //CAMERA_COLOR,
                         };      //�ߡ�������ͷ��ɫ����
    uint32  mybw;
    uint32  Imag_num = 0;

    f_mount(0, &vcanfs);

    do
    {
        Imag_num ++;
        sprintf(myfilename, "0:/vcan%d.sd", Imag_num);
        vcanres = f_open( &vcansrc , myfilename, FA_CREATE_NEW | FA_WRITE);

        if(vcanfs.fs_type == 0)
        {
            vcansrc.fs = 0;
            return;
        }

    }while(vcanres == FR_EXIST);        //����ļ����ڣ�������������1

    if ( vcanres == FR_OK )
    {
        vcanres = f_write(&vcansrc, imgsize, sizeof(imgsize),&mybw);  //��д��ߺͿ�������λ������
    }
    else
    {
        f_close(&vcansrc);
        vcansrc.fs = 0;
    }
}




void img_sd_save(uint8 * imgaddr,uint32 size)
{
#define F_SYNC_TIME   30      //���� n �κ� �� ͬ��

    int   vcanres;
    uint32 mybw;
    static uint8 time = 0;
    //uint32 size = CAMERA_SIZE;

    if(vcansrc.fs != 0)
    {
        time ++;

        vcanres = f_write(&vcansrc, imgaddr, size ,&mybw);

        if(vcanres != FR_OK)
        {
            f_close(&vcansrc);
            vcansrc.fs = 0;
        }

        if(time > F_SYNC_TIME)
        {
            time = 0 ;
            f_sync(&vcansrc);
        }
    }
}

void img_sd_exit(void)
{
    f_close(&vcansrc);
    vcansrc.fs = 0;
}