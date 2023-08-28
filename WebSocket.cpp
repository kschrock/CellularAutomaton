#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstddef>
#include "GraphicsClient.h"


#include <chrono>
#include <thread>

int main(void)
{
    GraphicsClient *obj1 = new GraphicsClient("127.0.0.1", 7777); //Normal constructor
    
    // obj1->clear();
    obj1->setBackgroundColor(0,0,128);
    // obj1->setDrawingColor(127,255,212);
    obj1->drawLine(600,0,600,600);
    obj1->drawRectangle(625,450,100,50);//void drawRectangle(int x, int y, int width, int height)
    obj1->drawstring(660,480,"Quit");
    obj1->drawRectangle(625,380,100,50);//void drawRectangle(int x, int y, int width, int height)
    obj1->drawstring(660,410,"Load");
    obj1->drawRectangle(625,310,100,50);//void drawRectangle(int x, int y, int width, int height)
    obj1->drawstring(650,340,"Random");
    obj1->drawRectangle(625,240,100,50);//void drawRectangle(int x, int y, int width, int height)
    obj1->drawstring(660,270,"Reset");
    obj1->drawRectangle(625,170,100,50);//void drawRectangle(int x, int y, int width, int height)
    obj1->drawstring(660,200,"Pause");
    obj1->drawRectangle(625,100,100,50);//void drawRectangle(int x, int y, int width, int height)
    obj1->drawstring(660,130,"Run");
    obj1->drawRectangle(625,30,100,50);//void drawRectangle(int x, int y, int width, int height)
    obj1->drawstring(660,60,"Step");

    obj1->drawstring(640,540,"SELECT SIZE");
    obj1->drawRectangle(620,550,40,40);
    obj1->drawstring(635,575,"1");
    obj1->drawRectangle(670,550,40,40);
    obj1->drawstring(685,575,"2");
    obj1->drawRectangle(720,550,40,40);
    obj1->drawstring(735,575,"3");
    // obj1->fillRectangle(0,0,1000,1000);
    // obj1->clear();
    // obj1->fillOval(0,0,200,100);
    // obj1->setPixel(10,10,255,192,203);
    // obj1->setBackgroundColor(0,0,0);
    // obj1->drawstring(100,100,"Hello, World!!!! This will be a very fun game. just checking if this works");
    obj1->socketClose();
}
