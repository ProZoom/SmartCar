#include "common.h"
#include "MK60_port.h"
#include "MK60_gpio.h"
#include "MK60_spi.h"

#include "VCAN_LCD.h"
#include "Vcan_touch.h"


#define XPT2046_INT_IN    PTX_T(TOUCH_INT_PTXn,IN)

#define USE_TSLIB       1

#if (USE_TSLIB )
#define  TOUCH_COUNT    5

typedef struct {
    //x[5]/y[5] �Ǵ���ֵ��xfb[5]/yfb[5] ��LCD���ꡣa[7]��У������
    int x[TOUCH_COUNT], xfb[TOUCH_COUNT];
    int y[TOUCH_COUNT], yfb[TOUCH_COUNT];
    int a[7];
} calibration;

#else
#define  TOUCH_COUNT    4

typedef struct {
    //x[5]/y[5] �Ǵ���ֵ��xfb[5]/yfb[5] ��LCD���ꡣa[7]��У������
    int x[TOUCH_COUNT], xfb[TOUCH_COUNT];
    int y[TOUCH_COUNT], yfb[TOUCH_COUNT];
    float a[7];
} calibration;

#endif


static uint32 touch_baud;
static uint32 touch_delay;

static calibration cal;


static int perform_calibration(calibration *cal);


/*
 * ����ģ��SPI IO �� �ж� IO ��ʼ��
 */
void touch_init(void)
{
    touch_baud = spi_init(TOUCH_SPI, TOUCH_CS, MASTER,150*1000);               //��ʼ��SPI,����ģʽ

    port_init(TOUCH_INT_PTXn,ALT1  | PULLUP | IRQ_FALLING);         //�������½��ش����ж�

    touch_delay = 8*1000*1000 / touch_baud;

    //gpio_init(TOUCH_CS_PTXn,GPO,0);
}

/******************************************************
* ��������Touchl_Calibrate
* ����  ��������У������
* ����  : ��
* ���  ��1 --- У���ɹ�
                    0   --- У��ʧ��
* ����  ����
* ע��  ����
*********************************************************/
int touch_calibrate(void)
{
    uint16 lcd_h = LCD_H;
    uint16 lcd_w = LCD_W;
    uint8 i;
    Site_t site;

    //���ô�����У����λ��
    cal.xfb[0] = 40;
    cal.yfb[0] = 40;

    cal.xfb[1] = 40;
    cal.yfb[1] = lcd_h - 40;

    cal.xfb[2] = lcd_w - 40;
    cal.yfb[2] = lcd_h - 40;

    cal.xfb[3] = lcd_w -40;
    cal.yfb[3] = 40;

#if (USE_TSLIB )
    cal.xfb[4] = lcd_w/2;
    cal.yfb[4] = lcd_h/2;
#endif

    //ѭ����ʾ5������У��㣬����ȡ����ֵ
    for(i = 0; i< TOUCH_COUNT;i++)
    {
        LCD_clear(BCOLOUR);                     //����

        DELAY_MS(500);                          //��ʱ�����򰴵�̫�죬���´���ʧ��

        site.x = cal.xfb[i];            //LCD��ʾ������ʮ��ͼ��
        site.y = cal.yfb[i];
        LCD_cross(site,10,FCOLOUR);

        while(!xpt2046_read(&site));            //�ȴ���ȡ������λ��
        cal.x[i] = site.x;
        cal.y[i] = site.y;
    }

    LCD_clear(BCOLOUR);                     //����


    //���д���У��
    if( perform_calibration(&cal) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int touch_get_point(Site_t *site)
{
    Site_t sitetmp;

    if(xpt2046_read(&sitetmp))
    {
#if (USE_TSLIB )
        site->x =( cal.a[0]
                 + cal.a[1] * sitetmp.x
                 + cal.a[2] * sitetmp.y)/cal.a[6];
        site->y =( cal.a[3]
                 + cal.a[4] * sitetmp.x
                 + cal.a[5] * sitetmp.y)/cal.a[6];
#else
        site->x =( cal.a[0] * sitetmp.x
                 + cal.a[1] * sitetmp.y
                 + cal.a[2]             )/cal.a[6];
        site->y =( cal.a[3] * sitetmp.x
                 + cal.a[4] * sitetmp.y
                 + cal.a[5]             )/cal.a[6];

#endif
        if((int16)site->x < 0)site->x = 0;
        else if(site->x >= LCD_W ) site->x = LCD_W-1;
        if((int16)site->y < 0)site->y = 0;
        else if(site->y >= LCD_H ) site->y = LCD_H-1;
        return 1;
    }
    else
    {
        return 0;
    }

}

int touch_load(touch_call_back_func func)
{
    return func((uint8 *)&(cal.a[0]),sizeof(int)*7);
}

int touch_save(touch_call_back_func func)
{
    return func((uint8 *)&(cal.a[0]),sizeof(int)*7);
}


//tslib (touchscreen library)
int perform_calibration(calibration *cal) {
#if (USE_TSLIB )

    int j;
    float n, x, y, x2, y2, xy, z, zx, zy;
    float det, a, b, c, e, f, i;
    float scaling = 65536.0;

// Get sums for matrix
    n = x = y = x2 = y2 = xy = 0;
    for(j=0;j<5;j++) {
        n += 1.0;
        x += (float)cal->x[j];
        y += (float)cal->y[j];
        x2 += (float)(cal->x[j]*cal->x[j]);
        y2 += (float)(cal->y[j]*cal->y[j]);
        xy += (float)(cal->x[j]*cal->y[j]);
    }

// Get determinant of matrix -- check if determinant is too small
    det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
    if(det < 0.1 && det > -0.1) {
        //printf("ts_calibrate: determinant is too small -- %f\n",det);
        return 0;
    }

// Get elements of inverse matrix
    a = (x2*y2 - xy*xy)/det;
    b = (xy*y - x*y2)/det;
    c = (x*xy - y*x2)/det;
    e = (n*y2 - y*y)/det;
    f = (x*y - n*xy)/det;
    i = (n*x2 - x*x)/det;

// Get sums for x calibration
    z = zx = zy = 0;
    for(j=0;j<5;j++) {
        z += (float)cal->xfb[j];
        zx += (float)(cal->xfb[j]*cal->x[j]);
        zy += (float)(cal->xfb[j]*cal->y[j]);
    }

// Now multiply out to get the calibration for framebuffer x coord
    cal->a[0] = (int)((a*z + b*zx + c*zy)*(scaling));
    cal->a[1] = (int)((b*z + e*zx + f*zy)*(scaling));
    cal->a[2] = (int)((c*z + f*zx + i*zy)*(scaling));

//    printf("%f %f %f\n",(a*z + b*zx + c*zy),
//                (b*z + e*zx + f*zy),
//                (c*z + f*zx + i*zy));

// Get sums for y calibration
    z = zx = zy = 0;
    for(j=0;j<5;j++) {
        z += (float)cal->yfb[j];
        zx += (float)(cal->yfb[j]*cal->x[j]);
        zy += (float)(cal->yfb[j]*cal->y[j]);
    }

// Now multiply out to get the calibration for framebuffer y coord
    cal->a[3] = (int)((a*z + b*zx + c*zy)*(scaling));
    cal->a[4] = (int)((b*z + e*zx + f*zy)*(scaling));
    cal->a[5] = (int)((c*z + f*zx + i*zy)*(scaling));

//    printf("%f %f %f\n",(a*z + b*zx + c*zy),
//                (b*z + e*zx + f*zy),
//                (c*z + f*zx + i*zy));

// If we got here, we're OK, so assign scaling to a[6] and return
    cal->a[6] = (int)scaling;
    return 1;
/*
// This code was here originally to just insert default values
    for(j=0;j<7;j++) {
        c->a[j]=0;
    }
    c->a[1] = c->a[5] = c->a[6] = 1;
    return 1;
*/

#else
//#define  TOUXCH_DIV    (65536.0)

    uint16 test_x=0, test_y=0;
    uint16 gap_x=0, gap_y=0;


    /* K��(X0��X2) (Y1��Y2)��(X1��X2) (Y0��Y2) */
    cal->a[6] = ((cal->x[0] - cal->x[2]) * (cal->y[1] - cal->y[2])) -
                    ((cal->x[1] - cal->x[2]) * (cal->y[0] - cal->y[2])) ;

    if( cal->a[6]  == 0 )
    {
        return 0;
    }
    else
    {
        /* A��((XD0��XD2) (Y1��Y2)��(XD1��XD2) (Y0��Y2))��K */
        cal->a[0] = (((cal->xfb[0] - cal->xfb[2]) * (cal->y[1] - cal->y[2])) -
                   ((cal->xfb[1] - cal->xfb[2]) * (cal->y[0] - cal->y[2])));

        /* B��((X0��X2) (XD1��XD2)��(XD0��XD2) (X1��X2))��K */
        cal->a[1] = (((cal->x[0] - cal->x[2]) * (cal->xfb[1] - cal->xfb[2])) -
                   ((cal->xfb[0] - cal->xfb[2]) * (cal->x[1] - cal->x[2])));

        /* C��(Y0(X2XD1��X1XD2)+Y1(X0XD2��X2XD0)+Y2(X1XD0��X0XD1))��K */
        cal->a[2] = ((cal->x[2] * cal->xfb[1] - cal->x[1] * cal->xfb[2]) * cal->y[0] +
                   (cal->x[0] * cal->xfb[2] - cal->x[2] * cal->xfb[0]) * cal->y[1] +
                   (cal->x[1] * cal->xfb[0] - cal->x[0] * cal->xfb[1]) * cal->y[2]);

        /* D��((YD0��YD2) (Y1��Y2)��(YD1��YD2) (Y0��Y2))��K */
        cal->a[3] = (((cal->yfb[0] - cal->yfb[2]) * (cal->y[1] - cal->y[2])) -
                   ((cal->yfb[1] - cal->yfb[2]) * (cal->y[0] - cal->y[2]))) ;

        /* E��((X0��X2) (YD1��YD2)��(YD0��YD2) (X1��X2))��K */
        cal->a[4] = (((cal->x[0] - cal->x[2]) * (cal->yfb[1] - cal->yfb[2])) -
                   ((cal->yfb[0] - cal->yfb[2]) * (cal->x[1] - cal->x[2]))) ;


        /* F��(Y0(X2YD1��X1YD2)+Y1(X0YD2��X2YD0)+Y2(X1YD0��X0YD1))��K */
        cal->a[5] = ((cal->x[2] * cal->yfb[1] - cal->x[1] * cal->yfb[2]) * cal->y[0] +
                   (cal->x[0] * cal->yfb[2] - cal->x[2] * cal->yfb[0]) * cal->y[1] +
                   (cal->x[1] * cal->yfb[0] - cal->x[0] * cal->yfb[1]) * cal->y[2]);

        //У���4����
            /*ȡһ�������Xֵ*/
        test_x = (( (cal->a[0] * cal->x[3]) +
                   (cal->a[1] * cal->y[3]) +
                    cal->a[2]
                 )) / cal->a[6] ;

            /*ȡһ�������Yֵ*/
        test_y = (( (cal->a[3] * cal->x[3]) +
                   (cal->a[4] * cal->y[3]) +
                   cal->a[5]
                 )) / cal->a[6] ;

        /* ʵ���������������Ĳ� */
        gap_x = abs(test_x - cal->xfb[3]);
        if(gap_x > 10)return 0;
        gap_y = abs(test_y - cal->yfb[3]);
        if(gap_y > 10)return 0;

    }
    return 1 ;

#endif

}


/*
*********************************************************************************************************
*   �� �� ��: TSC2046_ReadAdc
*   ����˵��: ѡ��һ��ģ��ͨ��������ADC��������ADC�������
*   ��    �Σ�_ucCh = 0x90 ��ʾYͨ���� 0xd0 ��ʾXͨ��
*   �� �� ֵ: 12λADCֵ
*********************************************************************************************************
*/
uint16 xpt2046_read_ch(uint8_t _ucCh)
{
    uint16 tmp;
    uint8 buff[2]={0,0};
    spi_mosi_cmd(TOUCH_SPI,TOUCH_CS, &_ucCh ,NULL ,buff, buff, 1 , 2); //SPI���ͽ��պ���
                                                                                //�������� _ucCh ����2������

#if 1
    //if(_ucCh == 0x90)
    {
        //tmp =  (buff[0]<<5);
        //tmp |= (buff[1]>>3);
        //tmp >>= 4;
        tmp = buff[0] <<8;
        tmp |= buff[1];
        tmp >>= 3;

        return tmp ;
    }
//    else
//    {
//        return (buff[0]<<4) | (buff[1]>>4);
//    }
#else

#endif


}


/*
 * ��ȡTP x y ��ADֵ(12bit�������4096)
 */
void xpt2046_get_xy(Site_t * site)
{
    DELAY_US(touch_delay);

    site->x= xpt2046_read_ch(TOUCH_XCh_12bit);

    DELAY_US(touch_delay);

    site->y = xpt2046_read_ch(TOUCH_YCh_12bit);
}


/******************************************************
* ��������Read_2046
* ����  ���õ����˲�֮���X Y
* ����  ��Coordinate�ṹ���ַ
* ���  ��1�ɹ���0ʧ��
* ����  ����
* ע��  ��������Ӧ��ʵ��"ר��,���Ǻܾ�׼�������ٶȱȽϿ�
*********************************************************/

uint8 xpt2046_read(Site_t * site )
{
#define READ_COUNT      3
#define XPT2046_ERROR   40
    //static Site_t screen;
    uint8 count=0,i;
    Site_t  sitexy;

    uint32 sumx,sumy;

    int buffer[2][READ_COUNT]={{0},{0}};  /*����X��Y���ж�β���*/

    /* ѭ������ READ_COUNT �� */
    do{
        xpt2046_get_xy(&sitexy);
        if((sitexy.x == 0) || (sitexy.y == 0xFFF))return 0;//����
        buffer[0][count]=sitexy.x;
        buffer[1][count]=sitexy.y;
        count++;
    }while(count<READ_COUNT); //�û����������ʱ��TP_INT_IN�ź�Ϊ�� ���� count<10

    if(count == READ_COUNT)
    {
        //ȥ����Сֵ �� ���ֵ ,����ƽ��ֵ
        sumx = buffer[0][0];
        sumy = buffer[1][0];
        for(i=1;i<READ_COUNT;i++)
        {
            sumx += buffer[0][i];
            sumy += buffer[1][i];
        }
        site->x = sumx /READ_COUNT;
        site->y = sumy /READ_COUNT;


        //У�顣�ж���û�������㳬�� ���ֵ
        for(i=1;i<READ_COUNT;i++)
        {
            if(abs(buffer[0][i] -site->x ) > XPT2046_ERROR)
            {
                return 0;
            }
            if(abs(buffer[1][i] -site->y ) > XPT2046_ERROR)
            {
                return 0;
            }

        }

        return 1;

        //
    }
    return 0;

}









