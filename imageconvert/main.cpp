/*************************************************************************
	> File Name: main.cpp
	> Author:maobin
	> Mail:
	> Created Time: Wed 13 Dec 2017 08:37:29 PM CST
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <libgen.h>
#include "imageconvert.h"
#include <opencv2/opencv.hpp>
using namespace std;

int main(int argc, char *argv[])
{
  ImageConvert *imageconvert = new ImageConvert;
  if (imageconvert->Init(argc,argv) != 0)
  {
    delete imageconvert;
    return -1;
  }
  imageconvert->Convert();

  delete imageconvert;
  return 0;
}

