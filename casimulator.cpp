#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstddef>
#include "CellularAutomaton.h"

#include <chrono>
#include <thread>
#include <time.h>

int main(void)
{
  CellularAutomaton *obj2 = new CellularAutomaton("blinker.txt", 0); //Normal constructor
  obj2->obj->setBackgroundColor(0, 0, 0);                            // Background color
  obj2->obj->setDrawingColor(127, 255, 212);                         // Drawing color
  obj2->drawGUI();                                                   // draw UI
  obj2->displayCells(obj2->obj, obj2->ptr2);                         // display current CA from file
  printf("Program Successfully Started.\n");
  char ch;
  //infinite loop
  struct timespec remaining, request = {0, 100000000}; // 100ms
  while (1)
  {
    nanosleep(&request, &remaining); // nano sleep 100ms
    obj2->obj->getServerInfo();      // listen for button click
    if (obj2->obj->functionCall == 0)
    {
      // this is for Step button
      obj2->step2DCA(obj2->ptr2);                // Step the current CA once
      obj2->displayCells(obj2->obj, obj2->ptr2); // display the CA
      obj2->obj->functionCall = -7;
    }
    if (obj2->obj->functionCall == 1)
    {
      // this is for Run button
      obj2->obj->functionCall = -7;
      while (1)
      {
        obj2->step2DCA(obj2->ptr2);                // Step through once
        obj2->displayCells(obj2->obj, obj2->ptr2); // display
        nanosleep(&request, &remaining);           // nanosleep 100 ms
        obj2->obj->getServerInfo();                // listen for button click
        if (obj2->obj->functionCall == 2)
        {
          obj2->obj->functionCall = -7;
          // Pause button has been clicked, break while loop
          break;
        }
      }
    }
    if (obj2->obj->functionCall == 2)
    {
      obj2->obj->functionCall = -7;
      // This is for Pause Button
    }
    if (obj2->obj->functionCall == 3)
    {
      // This is for Reset Button
      obj2->initCA(obj2->ptr2, 0);                         // clear the CA, set 0's
      obj2->obj->clear();                                  // clear display
      obj2->drawGUI();                                     // redraw UI
      obj2->resetInitialState(obj2->ptr2, obj2->filename); // reset back to inital filename
      obj2->displayCells(obj2->obj, obj2->ptr2);           // display reset CA
      obj2->obj->functionCall = -7;
    }
    if (obj2->obj->functionCall == 6)
    {
      // This is for Clear Button
      obj2->initCA(obj2->ptr2, 0);               // clear the CA, set 0's
      obj2->obj->clear();                        // clear display
      obj2->drawGUI();                           // redraw UI
      obj2->displayCells(obj2->obj, obj2->ptr2); // display the CA
      obj2->obj->functionCall = -7;
    }
    if (obj2->obj->functionCall == 4)
    {
      // This is for Random Button
      obj2->initCA(obj2->ptr2, -1);              // Randomize the CA, set random
      obj2->displayCells(obj2->obj, obj2->ptr2); // display the CA
      obj2->obj->functionCall = -7;
    }
    if (obj2->obj->functionCall == 5)
    {
      // This is for LOAD file Button
      obj2->obj->functionCall = -7;
      obj2->obj->requestLoad(); //REQUEST Call for file upload
      if (obj2->obj->cancelFlag == 0)
      {
        obj2->filename = obj2->obj->filePathName;            //update filename path
        obj2->initCA(obj2->ptr2, 0);                         // clear the CA, set 0's
        obj2->obj->clear();                                  // clear display
        obj2->drawGUI();                                     // redraw UI
        obj2->displayCells(obj2->obj, obj2->ptr2);           // display the CA
        obj2->resetInitialState(obj2->ptr2, obj2->filename); // reset back to inital filename
        obj2->obj->clear();                                  // clear display
        obj2->drawGUI();                                     // redraw UI
        obj2->displayCells(obj2->obj, obj2->ptr2);           // display the CA
      }
    }
    if (obj2->obj->functionCall == 8)
    {
      // This is for the size 1
      obj2->obj->functionCall = -7;
      obj2->updateGridSize(40, 40);              // update grid size
      obj2->obj->clearRectangle(0, 0, 600, 600); // clear grid with rectangle
      obj2->modifyGridSize(obj2->ptr2, 40, 40);  // modify current CA rows / cols
      obj2->obj->clearRectangle(0, 0, 600, 600); // clear grid with rectangle
      obj2->displayCells(obj2->obj, obj2->ptr2); // display the CA
    }
    if (obj2->obj->functionCall == 9)
    {
      // This is for the size 2
      obj2->obj->functionCall = -7;
      obj2->updateGridSize(150, 150);             // update grid size
      obj2->obj->clearRectangle(0, 0, 600, 600);  // clear grid with rectangle
      obj2->modifyGridSize(obj2->ptr2, 150, 150); // modify current CA rows / cols
      obj2->obj->clearRectangle(0, 0, 600, 600);  // clear grid with rectangle
      obj2->displayCells(obj2->obj, obj2->ptr2);  // display the CA
    }
    if (obj2->obj->functionCall == 10)
    {
      // This is for the size 3
      obj2->obj->functionCall = -7;
      obj2->updateGridSize(600, 600);             // update grid size
      obj2->obj->clearRectangle(0, 0, 600, 600);  // clear grid with rectangle
      obj2->modifyGridSize(obj2->ptr2, 600, 600); // modify current CA rows / cols
      obj2->obj->clearRectangle(0, 0, 600, 600);  // clear grid with rectangle
      obj2->displayCells(obj2->obj, obj2->ptr2);  // display the CA
    }

    if (obj2->obj->functionCall == 7)
    {
      //Quit Program
      obj2->removePointer(obj2->ptr2, obj2->filePointer); // remove the pointers connected to Graphics and CA
      printf("Quit Program Successfully.\n");
      break;
    }
  }
}