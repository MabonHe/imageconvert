/*************************************************************************
	> File Name: imageconvert.cpp
	> Author:maobin
	> Mail:
	> Created Time: Wed 13 Dec 2017 04:47:58 PM CST
 ************************************************************************/

#include "imageconvert.h"
#define ULONG unsigned long
ImageConvert::ImageConvert():m_width(0),m_height(0),m_count(0),m_oneimagesize(0),m_dataLength(0),m_frame(0)
{
    memset(m_filename,0,sizeof(m_filename));
    memset(m_format,0,sizeof(m_format));
    cout << "ImageConvert:" << __FUNCTION__ << __LINE__ <<endl;
}

ImageConvert::~ImageConvert()
{

}

void ImageConvert::Usage()
{
	cout <<"Usage*******************************************************************"<<endl;
	cout <<"--width=" << endl;
	cout <<"--height=" << endl;
	cout <<"--format=" << endl;
	cout <<"--filename=" << endl;
    cout <<"--count=" << endl;
    cout <<"--frame=" <<"   which frame need to convert" <<endl;
    cout <<"example: " << endl;
    cout <<"          imageconvert --width=1920 --height=1080 --format=nv12 --filename=test.nv12" << endl;
    cout <<"          imageconvert --width=1920 --height=1080 --format=nv12 --count=100 --filename=test.raw" << endl;
    cout <<"usage********************************************************************" << endl;
}
int ImageConvert::Init(int argc,char *argv[])
{
  cout << "ImageConvert:" << __FUNCTION__ << __LINE__ <<endl;
  char key[256], value[256];

  for(int i = 0; i < argc ; i++)
  {
  	   memset(key,0,sizeof(key));
  	   memset(value,0,sizeof(value));
       Spilt_parameters(argv[i],key,value);
       if ((!strcmp("--help", key)) || (!strcmp("-h", key)) || argc <= 1 )
        {
            Usage();
            return -1;
        }
       else if (0 == strcmp("--width", key))
       {
           m_width = atoi(value);
       }
       else if (0 == strcmp("--height", key))
       {
       	    m_height = atoi(value);
       }
       else if (0 == strcmp("--count", key))
       {
            m_count = atoi(value);
       }
      else if (0 == strcmp("--frame", key))
       {
            m_frame = atoi(value);
       }
       else if (0 == strcmp("--format", key))
       {
       	    strncpy(m_format,value,16);
       }
       else if (0 == strcmp("--filename",key))
       {
       	    strncpy(m_filename,value,256);
       }


  }

  PrintInfo();
  if ( -1 == checkParameters())
  {
    return -1;
  }

  return 0;
}

int ImageConvert::Spilt_parameters(const char *arg, char *key, char *value)
{
    char buffer[256];
    strcpy(buffer, arg);
    char *p = strtok(buffer, "=");
    if (!p)
        return -1;
    strcpy(key, p);
    p = strtok(NULL, "=");
    if (!p)
        return -1;
    strcpy(value, p);
    return 0;
}

void ImageConvert::PrintInfo()
{
  cout <<"image info ************************************************************************" << endl;
    cout <<"width:" << m_width << endl;
    cout <<"height:" << m_height << endl;
    cout <<"format:" << m_format << endl;
    cout <<"filename:" << m_filename << endl;
    cout <<"one_img_size:" << m_oneimagesize << endl;
    cout <<"datalength:" << m_dataLength << endl;
    cout << "count:" << m_count  << endl;
  cout << "image info ************************************************************************" << endl;
}

int ImageConvert::Convert()
{
    cout << "function:" << __FUNCTION__ << " line:"<< __LINE__ <<endl;
    if (m_count == 0)
    {
      m_count = 1;
      m_frame = 0;
    }
    else
    {
      if (m_frame == 0 )
      {
          m_frame=5;
      }

    }
    char outputjpg[256], outputbmp[256], outputpng[256];
    memset(outputjpg,0,sizeof(outputjpg));
    memset(outputbmp,0,sizeof(outputbmp));
    memset(outputpng,0,sizeof(outputpng));
    sprintf(outputjpg,"%s.jpg",m_filename);
    sprintf(outputbmp,"%s.bmp",m_filename);
    sprintf(outputpng,"%s.png",m_filename);
    Mat image;
    bool ret = true;
    uchar *pRawData = NULL;
    m_dataLength = DataLength(m_filename);
    m_oneimagesize = m_dataLength/m_count;
    cout <<"datalength:" << m_dataLength << endl;
    cout << "one_img_size:" << m_oneimagesize << endl;
    ULONG nDataLen = ReadRawData(m_filename, &pRawData);
    ULONG one_img_size = 0;
    int stride = 0;
    MEDIA_FORMAT foramt = GetFormatFromString(m_format);

    func_convert_format func = NULL;
    switch (foramt)
    {
    case MEDIA_FORMAT_NV12:
        one_img_size = m_width * m_height * 3 / 2;
        func=convert_nv122bgr;
        stride = m_width;
        break;
    case MEDIA_FORMAT_NV16:
        one_img_size = m_width * m_height * 2;
        func = convert_nv162bgr;
        stride = m_width;
        break;
    case MEDIA_FORMAT_UYVY:
        one_img_size = m_width * m_height * 2;
        func = convert_uyvy2bgr;
        stride = 2*m_width;
        break;
    case MEDIA_FORMAT_YUY2:
        one_img_size = m_width * m_height * 2;
        func = convert_yuy22bgr;
        stride = 2*m_width;
        break;
    case MEDIA_FORMAT_XRGB:
        one_img_size = m_width * m_height * 4;
        func = convert_xrgb2bgr;
        stride = 4*m_width;
        break;
    case MEDIA_FORMAT_XBGR:
        one_img_size = m_width * m_height * 4;
        func = convert_xbgr2bgr;
        stride = 4*m_width;
        break;
    // Vaapipostproc output format
    case MEDIA_FORMAT_I420:
        one_img_size = m_width * m_height * 3 / 2;
        func = convert_i4202bgr;
        stride = m_width;
        break;
    case MEDIA_FORMAT_ARGB:
        one_img_size = m_width * m_height * 4;
        func = convert_argb2bgr;
        stride = 4*m_width;
        break;
    case MEDIA_FORMAT_BGRA:
        one_img_size = m_width * m_height * 4;
        func = convert_bgra2bgr;
        stride = 4*m_width;
        break;
    case MEDIA_FORMAT_BGR24:
        one_img_size = m_width * m_height * 3;
        func = convert_bgr2bgr;
        stride = 3*m_width;
        break;
    case MEDIA_FORMAT_RGB565:
        one_img_size = m_width * m_height * 2;
        func = convert_rgb162bgr;
        stride = 2*m_width;
        break;
    default:
        free(pRawData);
        cout << "unknown format" << endl;
        return -1;
        break;

    }
     if (stride % 64 != 0)
         {
            stride += 64 - stride % 64;
         }
     cout << __LINE__ << "start convert" << endl;
     func(pRawData, stride, m_width, m_height, image);
     cout << __LINE__ << "end convert" << endl;
     imwrite(outputbmp,image);
     imwrite(outputjpg,image);
     imwrite(outputpng,image);
     free(pRawData);

}

void ImageConvert::convert_nv122bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);
    uchar *pY = NULL;
    uchar *pCbCr = NULL;
    uchar *pBGR = NULL;

    BYTE Y = 0, Cr = 0, Cb = 0;
    BYTE R = 0, G = 0, B = 0;

    // NV12 to BGR
    for (int y = 0; y < h; y++)
    {
        pY = buff + y * stride;
        pCbCr = buff + (h + y / 2) * stride;
        pBGR = dst.data + y * dst.step;

        for (int x = 0, x3 = 0; x < w; x++, x3 += 3)
        {
            Y  = pY[x];
            Cr = pCbCr[x / 2 * 2 + 1];
            Cb = pCbCr[x / 2 * 2];

            ConvertYCrCbToRGB(Y, Cr, Cb, R, G, B);

            pBGR[x3] = B;
            pBGR[x3 + 1] = G;
            pBGR[x3 + 2] = R;
        }
    }
}

void ImageConvert::convert_nv162bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);
    uchar *pY = NULL;
    uchar *pCbCr = NULL;
    uchar *pBGR = NULL;

    BYTE Y = 0, Cr = 0, Cb = 0;
    BYTE R = 0, G = 0, B = 0;

    // NV16 to BGR
    for (int y = 0; y < h; y++)
    {
        pY = buff + y * stride;
        pCbCr = buff + h*stride + y*stride;
        pBGR = dst.data + y * dst.step;

        for (int x = 0, x6 = 0; x < w; x += 2, x6 += 6)
        {
            Y  = pY[x];
            Cr = pCbCr[x+1];
            Cb = pCbCr[x];

            ConvertYCrCbToRGB(Y, Cr, Cb, R, G, B);

            pBGR[x6] = B;
            pBGR[x6 + 1] = G;
            pBGR[x6 + 2] = R;

            Y  = pY[x + 1];
            ConvertYCrCbToRGB(Y, Cr, Cb, R, G, B);
            pBGR[x6 + 3] = B;
            pBGR[x6 + 4] = G;
            pBGR[x6 + 5] = R;
        }
    }
}

void ImageConvert::convert_yuy22bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pBGR = NULL;
    uchar *pYUY2 = NULL;
    uchar *pCbCr = NULL;

    int yuy2step = w * 2;

    BYTE Y = 0, Cr = 0, Cb = 0;
    BYTE R = 0, G = 0, B = 0;

    // YUY2 to BGR
    for (int y = 0; y < h; y++)
    {
        pBGR = dst.data + y * dst.step;
        pYUY2 = buff + y * stride;

        for (int x4 = 0, x6 = 0; x4 < yuy2step; x4 += 4, x6 += 6)
        {
            Cb = pYUY2[x4 + 1];
            Cr = pYUY2[x4 + 3];

            Y = pYUY2[x4];

            ConvertYCrCbToRGB(Y, Cr, Cb, R, G, B);

            pBGR[x6] = B;
            pBGR[x6 + 1] = G;
            pBGR[x6 + 2] = R;

            Y = pYUY2[x4 + 2];

            ConvertYCrCbToRGB(Y, Cr, Cb, R, G, B);

            pBGR[x6 + 3] = B;
            pBGR[x6 + 4] = G;
            pBGR[x6 + 5] = R;
        }
    }
}

void ImageConvert::convert_uyvy2bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pBGR = NULL;
    uchar *pYUY2 = NULL;
    uchar *pCbCr = NULL;

    int yuy2step = w * 2;

    BYTE Y = 0, Cr = 0, Cb = 0;
    BYTE R = 0, G = 0, B = 0;

    // UYVY to BGR
    for (int y = 0; y < h; y++)
    {
        pBGR = dst.data + y * dst.step;
        pYUY2 = buff + y * stride;

        for (int x4 = 0, x6 = 0; x4 < yuy2step; x4 += 4, x6 += 6)
        {
            Cb = pYUY2[x4];
            Y = pYUY2[x4 + 1];
            Cr = pYUY2[x4 + 2];

            ConvertYCrCbToRGB(Y, Cr, Cb, R, G, B);

            pBGR[x6] = B;
            pBGR[x6 + 1] = G;
            pBGR[x6 + 2] = R;

            Y = pYUY2[x4 + 3];

            ConvertYCrCbToRGB(Y, Cr, Cb, R, G, B);

            pBGR[x6 + 3] = B;
            pBGR[x6 + 4] = G;
            pBGR[x6 + 5] = R;
        }
    }
}

void ImageConvert::convert_i4202bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pBGR0 = NULL;
    uchar *pBGR1 = NULL;
    uchar *pYUV0 = NULL;
    uchar *pYUV1 = NULL;
    uchar *pCbCr = NULL;
    uchar *pV = NULL;
    uchar *pU = NULL;
    uchar *Vorg = NULL;
    uchar *Uorg = NULL;

    BYTE Y00 = 0, Y01 = 0, Y10 = 0, Y11 = 0, Cr = 0, Cb = 0;
    BYTE R = 0, G = 0, B = 0;

    int yuvstep = w;

    Vorg = buff + h * stride;
    Uorg = buff + h * stride + h * stride / 4;

    // I420 to BGR
    for (int y2 = 0; y2 < h; y2 += 2)
    {
        pBGR0 = dst.data + y2 * dst.step;
        pBGR1 = dst.data + (y2 + 1) * dst.step;

        pYUV0 = buff + y2 * stride;
        pYUV1 = buff + (y2 + 1) * stride;

        pV = Vorg + (y2 / 2) * (stride / 2);
        pU = Uorg + (y2 / 2) * (stride / 2);

        for (int x = 0, x2 = 0, x6 = 0; x2 < yuvstep; x++, x2 += 2, x6 += 6)
        {
            Y00 = pYUV0[x2];
            Y01 = pYUV0[x2+1];
            Y10 = pYUV1[x2];
            Y11 = pYUV1[x2+1];

            Cr = pU[x];
            Cb = pV[x];

            ConvertYCrCbToRGB(Y00, Cr, Cb, R, G, B);

            pBGR0[x6] = B;
            pBGR0[x6 + 1] = G;
            pBGR0[x6 + 2] = R;

            ConvertYCrCbToRGB(Y01, Cr, Cb, R, G, B);

            pBGR0[x6 + 3] = B;
            pBGR0[x6 + 4] = G;
            pBGR0[x6 + 5] = R;

            ConvertYCrCbToRGB(Y10, Cr, Cb, R, G, B);

            pBGR1[x6] = B;
            pBGR1[x6 + 1] = G;
            pBGR1[x6 + 2] = R;

            ConvertYCrCbToRGB(Y11, Cr, Cb, R, G, B);

            pBGR1[x6 + 3] = B;
            pBGR1[x6 + 4] = G;
            pBGR1[x6 + 5] = R;
        }
    }
}

void ImageConvert::convert_xbgr2bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pBGR = NULL;
    uchar *pXBGR = NULL;

    int xbgrstep = w * 4;

    // XBGR to BGR
    for (int y = 0; y < h; y++)
    {
        pBGR = dst.data + y * dst.step;
        pXBGR = buff + y * stride;

        for (int x4 = 0, x3 = 0; x4 < xbgrstep; x4 += 4, x3 += 3)
        {
            pBGR[x3] = pXBGR[x4+0];
            pBGR[x3 + 1] = pXBGR[x4+1];
            pBGR[x3 + 2] = pXBGR[x4+2];
        }
    }
}

void ImageConvert::convert_xrgb2bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pBGR = NULL;
    uchar *pXRGB = NULL;

    int xbgrstep = w * 4;

    // XRGB to BGR
    for (int y = 0; y < h; y++)
    {
        pBGR = dst.data + y * dst.step;
        pXRGB = buff + y * stride;

        for (int x4 = 0, x3 = 0; x4 < xbgrstep; x4 += 4, x3 += 3)
        {
            pBGR[x3] = pXRGB[x4+0] << 3;
            pBGR[x3 + 1] = pXRGB[x4+1] << 2;
            pBGR[x3 + 2] = pXRGB[x4+2] << 3;
        }
    }
}

void ImageConvert::convert_argb2bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pBGR = NULL;
    uchar *pXRGB = NULL;

    int xbgrstep = w * 4;

    // ARGB to BGR
    for (int y = 0; y < h; y++)
    {
        pBGR = dst.data + y * dst.step;
        pXRGB = buff + y * stride;

        for (int x4 = 0, x3 = 0; x4 < xbgrstep; x4 += 4, x3 += 3)
        {
            pBGR[x3] = pXRGB[x4+0]; // B
            pBGR[x3 + 1] = pXRGB[x4+1]; // G
            pBGR[x3 + 2] = pXRGB[x4+2]; // R
        }
    }
}

void ImageConvert::convert_bgra2bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pBGR = NULL;
    uchar *pBGRA = NULL;

    int xbgrstep = w * 4;

    for (int y = 0; y < h; y++)
    {
        pBGR = dst.data + y * dst.step;
        pBGRA = buff + y * stride;

        for (int x4 = 0, x3 = 0; x4 < xbgrstep; x4 += 4, x3 += 3)
        {
            pBGR[x3] = pBGRA[x4]; // B
            pBGR[x3 + 1] = pBGRA[x4+1]; // G
            pBGR[x3 + 2] = pBGRA[x4+2]; // R
        }
    }
}

void ImageConvert::convert_bgr2bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pSrcBGR = NULL;
    uchar *pDstRGB = NULL;

    int srcstep = w * 3;

    // BGR to BGR
    for (int y = 0; y < h; y++)
    {
        pDstRGB = dst.data + y * dst.step;
        pSrcBGR = buff + y * stride;

        for (int x3 = 0; x3 < srcstep; x3 += 3)
        {
            pDstRGB[x3] = pSrcBGR[x3]; // B
            pDstRGB[x3 + 1] = pSrcBGR[x3+1]; // G
            pDstRGB[x3 + 2] = pSrcBGR[x3+2]; // R
        }
    }
}

void ImageConvert::convert_rgb162bgr(uchar *buff, int stride, int w, int h, Mat &dst)
{
    dst.create(Size(w, h), CV_8UC3);

    uchar *pSrcBGR = NULL;
    uchar *pDstRGB = NULL;

    int srcstep = w * 2;

    // BGR to BGR
    for (int y = 0; y < h; y++)
    {
        pDstRGB = dst.data + y * dst.step;
        pSrcBGR = buff + y * stride;

        for (int x2 = 0, x3 = 0; x2 < srcstep; x2 += 2, x3 += 3)
        {
            pDstRGB[x3] = (pSrcBGR[x2] & 0x1F) << 3; // B
            pDstRGB[x3 + 1] = ((pSrcBGR[x2] >> 5) + (pSrcBGR[x2+1]&0x7) << 3) << 2; // G
            pDstRGB[x3 + 2] = (pSrcBGR[x2+1]&0xF8); // R
        }
    }
}

inline BYTE ImageConvert::Clip(int clr)
{
    return (BYTE) (clr < 0 ? 0 : (clr > 255 ? 255 : clr));
}

inline void ImageConvert::ConvertYCrCbToRGB(
    BYTE y,
    BYTE cr,
    BYTE cb,
    BYTE &r,
    BYTE &g,
    BYTE &b
    )
{
    int c = (int)y - 16;
    int d = (int) cb - 128;
    int e = (int) cr - 128;

    r = Clip((298 * c + 409 * e + 128) >> 8);
    g = Clip((298 * c - 100 * d - 208 * e + 128) >> 8);
    b = Clip((298 * c + 516 * d + 128) >> 8);
}

ULONG ImageConvert::ReadRawData(const char *filename, uchar **pnv12)
{
    FILE *fp = NULL;

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        cout << "fail to open file " << endl;
        return -1;
    }



    fseek(fp, m_oneimagesize * m_frame, SEEK_SET);
    ULONG datalen = ftell(fp);
    cout << "datalen:" << datalen << endl;
    *pnv12 = (uchar *)malloc((size_t) m_oneimagesize);

    size_t readlen = fread(*pnv12, 1, m_oneimagesize, fp);

    fclose(fp);

    if (readlen != m_oneimagesize)
    {
        cout << "readlength:"<< readlen <<" != imagesize" << m_oneimagesize << endl;
        return -2;
    }

    return readlen;
}

ULONG ImageConvert::DataLength(const char *filename)
{
    FILE *fp = NULL;

    fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        cout << "fail to open file " << endl;
        return -1;
    }

    fseek(fp, 0L, SEEK_END);
    ULONG datalen = ftell(fp);
    cout << "datalen:" << datalen << endl;
    fseek(fp, 0L, SEEK_SET);
    fclose(fp);
    return datalen;
}
MEDIA_FORMAT ImageConvert:: GetFormatFromString(char *strRawDumpFormat)
{
    MEDIA_FORMAT RawDumpFormat = MEDIA_FORMAT_UNKNOWN;

    if (strcmp(strRawDumpFormat, "UYVY") == 0 || strcmp(strRawDumpFormat, "uyvy") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_UYVY;
    }
    else if (strcmp(strRawDumpFormat, "YUY2") == 0 || strcmp(strRawDumpFormat, "yuy2") == 0 || strcmp(strRawDumpFormat, "YUYV") == 0 || strcmp(strRawDumpFormat, "yuyv") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_YUY2;
    }
    else if (strcmp(strRawDumpFormat, "NV12") == 0 || strcmp(strRawDumpFormat, "nv12") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_NV12;
    }
    else if (strcmp(strRawDumpFormat, "NV16") == 0 || strcmp(strRawDumpFormat, "nv16") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_NV16;
    }
    else if (strcmp(strRawDumpFormat, "XRGB") == 0 || strcmp(strRawDumpFormat, "RGBx") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_XRGB;
    }
    else if (strcmp(strRawDumpFormat, "XBGR") == 0 || strcmp(strRawDumpFormat, "BGRx") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_XBGR;
    }
    else if (strcmp(strRawDumpFormat, "I420") == 0 || strcmp(strRawDumpFormat, "i420") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_I420;
    }
    else if (strcmp(strRawDumpFormat, "ARGB") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_ARGB;
    }
    else if (strcmp(strRawDumpFormat, "BGRA") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_BGRA;
    }
    else if (strcmp(strRawDumpFormat, "BGR") == 0 ||
             strcmp(strRawDumpFormat, "BGR888") == 0 ||
             strcmp(strRawDumpFormat, "BGR24") == 0)
    {
        RawDumpFormat = MEDIA_FORMAT_BGR24;
    }
    else if (strcmp(strRawDumpFormat, "RGB16") == 0 ||
             strcmp(strRawDumpFormat, "RGB565") == 0 )
    {
        RawDumpFormat = MEDIA_FORMAT_RGB565;
    }
    return RawDumpFormat;
}

void ImageConvert::debug(char *msg,DebugLevel level)
{

    if (DEBUG_LEVEL == 1 )
    {
      cout << "function:" <<__FUNCTION__<<"line:" << __LINE__<< level << ":" <<"msg:"<< msg << endl;
    }

}
int ImageConvert::checkParameters()
  {
     if (m_filename[0] == 0)
     {
       cout << "please type filename...." << endl;
       return -1;
     }

     if (m_format[0] == 0)
     {
       cout << "please type format ..." << endl;
       return -1;
     }
     if (m_width == 0)
     {
      cout <<"please type width..." << endl;
      return -1;
     }
     if (m_height == 0)
     {
      cout <<"pleae type height .." << endl;
      return -1;
     }

     return 0;
  }
