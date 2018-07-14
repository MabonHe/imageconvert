/*************************************************************************
	> File Name: imageconvert.h
	> Author:maobin
	> Mail:
	> Created Time: Wed 13 Dec 2017 08:39:14 PM CST
 ************************************************************************/

#ifndef _IMAGECONVERT_H
#define _IMAGECONVERT_H
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <libgen.h>
#define BYTE unsigned char
#define ULONG unsigned long
using namespace std;
using namespace cv;

enum MEDIA_FORMAT
{
    MEDIA_FORMAT_UNKNOWN,
    MEDIA_FORMAT_NV12,
    MEDIA_FORMAT_NV16,
    MEDIA_FORMAT_UYVY,
    MEDIA_FORMAT_YUY2,
    MEDIA_FORMAT_XRGB,
    MEDIA_FORMAT_XBGR,
    MEDIA_FORMAT_I420,
    MEDIA_FORMAT_ARGB,
    MEDIA_FORMAT_BGRA,
    MEDIA_FORMAT_BGR24,
    MEDIA_FORMAT_RGB565
};


#define DEBUG_LEVEL 1
enum DebugLevel
{
   INFO,
   ERROR,
   RESULT,
   DEBUG
};
class ImageConvert
{

public:
    ImageConvert();
    ~ImageConvert();
    void Usage();
    void PrintInfo();
    int Init(int argc, char *argv[]);
    int Spilt_parameters(const char *arg, char *key, char *value);
    int Convert();
private:
    typedef void (*func_convert_format)(uchar *buff, int stride, int w, int h, Mat &dst);
    static inline BYTE Clip(int clr);
    static inline void ConvertYCrCbToRGB(
                                   BYTE y,
                                   BYTE cr,
                                   BYTE cb,
                                   BYTE &r,
                                   BYTE &g,
                                   BYTE &b
                                 );
    static void convert_nv122bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_nv162bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_yuy22bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_uyvy2bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_i4202bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_xbgr2bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_xrgb2bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_argb2bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_bgra2bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_bgr2bgr(uchar *buff, int stride, int w, int h, Mat &dst);
    static void convert_rgb162bgr(uchar *buff, int stride, int w, int h, Mat &dst);
private:
    ULONG ReadRawData(const char *filename, uchar **pnv12);
    ULONG DataLength(const char *filename);
    MEDIA_FORMAT GetFormatFromString(char *strRawDumpFormat);
private:
    int checkParameters();
    void debug(char *msg,DebugLevel level);

private:
    int m_width;
    int m_height;
    int m_count;
    int m_frame;
    int m_oneimagesize;
    ULONG m_dataLength;
    char m_format[16];
    char m_filename[256];


};
#endif
