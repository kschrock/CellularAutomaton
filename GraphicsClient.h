#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cstddef>
#include <sys/ioctl.h>
// using namespace std;

class GraphicsClient
{

public:
   std::string hostname;
   int port_num;
   int sockfd;
   char message[256];
   int functionCall = -11; //dummy val
   std::string filePathName;
   int cancelFlag;

   GraphicsClient(std::string hostname, int port_num)
   {
      this->hostname = hostname;
      this->port_num = port_num;
      int sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0)
      {
         fprintf(stderr, "Error creating socket\n");
         exit(-1);
      }

      struct sockaddr_in serv_addr;
      memset(&serv_addr, '0', sizeof(serv_addr));

      serv_addr.sin_family = AF_INET;         //Type of Address
      serv_addr.sin_port = htons(7777);       //set port to connect to
      serv_addr.sin_addr.s_addr = INADDR_ANY; //server address 0.0.0.127
      int connection_status = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
      if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
      {
         fprintf(stderr, "Invalid address/ Address not supported \n");
         exit(-1);
      }

      if (connection_status == -1)
      {
         fprintf(stderr, "Connection Failed \n");
         exit(-1);
      }
      this->sockfd = sockfd;
      char message[100];
      message[0] = 0xFF;            //SYNC
      message[1] = 0x00;            //length of message
      message[2] = 0x00;            //length of message
      message[3] = 0x00;            //length of message
      message[4] = 0x07;            //length of message
      message[5] = 0x02;            //message TYPE, set color
      send(sockfd, message, 12, 0); //12 is the num bits
      message[4] = 0x01;
      message[5] = 0x01;
      send(sockfd, message, 6, 0); //6 is the num of bits
   }
   /*
   send background color message to server
   */
   void setBackgroundColor(int red, int green, int blue)
   {
      char message[12];
      message[0] = 0xFF;
      message[1] = 0x00;                //length of message
      message[2] = 0x00;                //length of message
      message[3] = 0x00;                //length of message
      message[4] = 0x07;                //length of message
      message[5] = 0x02;                //message TYPE, set color
      message[6] = (red >> 4) & 0x0F;   //MSB
      message[7] = red & 0x0F;          //LSB
      message[8] = (green >> 4) & 0x0F; //MSB
      message[9] = green & 0x0F;        //LSB
      message[10] = (blue >> 4) & 0x0F; //MSB
      message[11] = blue & 0x0F;        //LSB
      send(sockfd, message, 12, 0);     //12 is the num of bits
      message[4] = 0x01;                //clear
      message[5] = 0x01;                //clear
      send(sockfd, message, 6, 0);      //6 is the num of bits
      repaint();
   }
   /*
   send drawing color message to server
   */
   void setDrawingColor(int red, int green, int blue)
   {
      char message[12];
      message[0] = 0xFF;
      message[1] = 0x00;                //length of message
      message[2] = 0x00;                //length of message
      message[3] = 0x00;                //length of message
      message[4] = 0x07;                //length of message
      message[5] = 0x06;                //message TYPE, set color
      message[6] = (red >> 4) & 0x0F;   //MSB
      message[7] = red & 0x0F;          //LSB
      message[8] = (green >> 4) & 0x0F; //MSB
      message[9] = green & 0x0F;        //LSB
      message[10] = (blue >> 4) & 0x0F; //MSB
      message[11] = blue & 0x0F;        //LSB
      send(sockfd, message, 12, 0);     //12 is the num of bits
      message[4] = 0x01;                //clear
      message[5] = 0x01;                //clear
      send(sockfd, message, 6, 0);      //6 is the num of bits
      repaint();
   }
   /*
   send clear message to server
   */
   void clear(void)
   {
      char message[6];
      message[0] = 0xFF;
      message[1] = 0x00;
      message[2] = 0x00;
      message[3] = 0x00;
      message[4] = 0x01;
      message[5] = 0x01;
      send(sockfd, message, 6, 0);
   }
   void socketClose(void)
   {
      close(sockfd);
   }
   /*
   send repaint message to server
   */
   void repaint()
   {
      char message[6];
      message[0] = 0xFF;
      message[1] = 0x00;
      message[2] = 0x00;
      message[3] = 0x00;
      message[4] = 0x01;
      message[5] = 0x0C;
      send(sockfd, message, 6, 0);
   }
   /*
   draws line at a certian x and y coordiate message to server
   */
   void drawLine(int x1, int y1, int x2, int y2)
   {
      char message[22];
      message[0] = 0xFF;
      message[1] = 0x00;              //length of message
      message[2] = 0x00;              //length of message
      message[3] = 0x01;              //length of message payload = 17
      message[4] = 0x01;              //length of message
      message[5] = 0x0D;              //message TYPE, set drawLine, payload starts here.
      message[6] = (x1 >> 12) & 0xF;  //x1
      message[7] = (x1 >> 8) & 0xF;   //x1
      message[8] = (x1 >> 4) & 0xF;   //x1
      message[9] = x1 & 0xF;          //x1
      message[10] = (y1 >> 12) & 0xF; //y1
      message[11] = (y1 >> 8) & 0xF;  //y1
      message[12] = (y1 >> 4) & 0xF;  //y1
      message[13] = y1 & 0xF;         //y1
      message[14] = (x2 >> 12) & 0xF; //x2
      message[15] = (x2 >> 8) & 0xF;  //x2
      message[16] = (x2 >> 4) & 0xF;  //x2
      message[17] = x2 & 0xF;         //x2
      message[18] = (y2 >> 12) & 0xF; //y2
      message[19] = (y2 >> 8) & 0xF;  //y2
      message[20] = (y2 >> 4) & 0xF;  //y2
      message[21] = y2 & 0xF;         //y2
      send(sockfd, message, 22, 0);
      repaint();
   }
   /*
   draws reactangle at a certian x and y coordiate message to server
   */
   void drawRectangle(int x, int y, int width, int height)
   {
      char message[22];
      message[0] = 0xFF;
      message[1] = 0x00;                  //length of message
      message[2] = 0x00;                  //length of message
      message[3] = 0x01;                  //length of message payload = 17
      message[4] = 0x01;                  //length of message
      message[5] = 0x07;                  //message TYPE, set drawRectangle, payload starts here.
      message[6] = (x >> 12) & 0xF;       //x
      message[7] = (x >> 8) & 0xF;        //x
      message[8] = (x >> 4) & 0xF;        //x
      message[9] = x & 0xF;               //x
      message[10] = (y >> 12) & 0xF;      //y
      message[11] = (y >> 8) & 0xF;       //y
      message[12] = (y >> 4) & 0xF;       //y
      message[13] = y & 0xF;              //y
      message[14] = (width >> 12) & 0xF;  //width
      message[15] = (width >> 8) & 0xF;   //width
      message[16] = (width >> 4) & 0xF;   //width
      message[17] = width & 0xF;          //width
      message[18] = (height >> 12) & 0xF; //height
      message[19] = (height >> 8) & 0xF;  //height
      message[20] = (height >> 4) & 0xF;  //height
      message[21] = height & 0xF;         //height
      send(sockfd, message, 22, 0);
      repaint();
   }
   /*
   fill reactangle at a certian x and y coordiate message to server
   */
   void fillRectangle(int x, int y, int width, int height)
   {
      char message[22];
      message[0] = 0xFF;
      message[1] = 0x00;                  //length of message
      message[2] = 0x00;                  //length of message
      message[3] = 0x01;                  //length of message payload = 17
      message[4] = 0x01;                  //length of message
      message[5] = 0x08;                  //message TYPE, set drawRectangle, payload starts here.
      message[6] = (x >> 12) & 0xF;       //x
      message[7] = (x >> 8) & 0xF;        //x
      message[8] = (x >> 4) & 0xF;        //x
      message[9] = x & 0xF;               //x
      message[10] = (y >> 12) & 0xF;      //y
      message[11] = (y >> 8) & 0xF;       //y
      message[12] = (y >> 4) & 0xF;       //y
      message[13] = y & 0xF;              //y
      message[14] = (width >> 12) & 0xF;  //width
      message[15] = (width >> 8) & 0xF;   //width
      message[16] = (width >> 4) & 0xF;   //width
      message[17] = width & 0xF;          //width
      message[18] = (height >> 12) & 0xF; //height
      message[19] = (height >> 8) & 0xF;  //height
      message[20] = (height >> 4) & 0xF;  //height
      message[21] = height & 0xF;         //height
      send(sockfd, message, 22, 0);
      repaint();
   }
   /*
   clears reactangle at a certian x and y coordiate message to server
   */
   void clearRectangle(int x, int y, int width, int height)
   {
      char message[22];
      message[0] = 0xFF;
      message[1] = 0x00;                  //length of message
      message[2] = 0x00;                  //length of message
      message[3] = 0x01;                  //length of message payload = 17
      message[4] = 0x01;                  //length of message
      message[5] = 0x09;                  //message TYPE, set clearRectangle, payload starts here.
      message[6] = (x >> 12) & 0xF;       //x
      message[7] = (x >> 8) & 0xF;        //x
      message[8] = (x >> 4) & 0xF;        //x
      message[9] = x & 0xF;               //x
      message[10] = (y >> 12) & 0xF;      //y
      message[11] = (y >> 8) & 0xF;       //y
      message[12] = (y >> 4) & 0xF;       //y
      message[13] = y & 0xF;              //y
      message[14] = (width >> 12) & 0xF;  //width
      message[15] = (width >> 8) & 0xF;   //width
      message[16] = (width >> 4) & 0xF;   //width
      message[17] = width & 0xF;          //width
      message[18] = (height >> 12) & 0xF; //height
      message[19] = (height >> 8) & 0xF;  //height
      message[20] = (height >> 4) & 0xF;  //height
      message[21] = height & 0xF;         //height
      send(sockfd, message, 22, 0);
      repaint();
   }
   /*
   draws an oval at a certian x and y coordiate message to server
   */
   void drawOval(int x, int y, int width, int height)
   {
      char message[22];
      message[0] = 0xFF;
      message[1] = 0x00;                  //length of message
      message[2] = 0x00;                  //length of message
      message[3] = 0x01;                  //length of message payload = 17
      message[4] = 0x01;                  //length of message
      message[5] = 0x0A;                  //message TYPE, set drawOval, payload starts here.
      message[6] = (x >> 12) & 0xF;       //x
      message[7] = (x >> 8) & 0xF;        //x
      message[8] = (x >> 4) & 0xF;        //x
      message[9] = x & 0xF;               //x
      message[10] = (y >> 12) & 0xF;      //y
      message[11] = (y >> 8) & 0xF;       //y
      message[12] = (y >> 4) & 0xF;       //y
      message[13] = y & 0xF;              //y
      message[14] = (width >> 12) & 0xF;  //width
      message[15] = (width >> 8) & 0xF;   //width
      message[16] = (width >> 4) & 0xF;   //width
      message[17] = width & 0xF;          //width
      message[18] = (height >> 12) & 0xF; //height
      message[19] = (height >> 8) & 0xF;  //height
      message[20] = (height >> 4) & 0xF;  //height
      message[21] = height & 0xF;         //height
      send(sockfd, message, 22, 0);
      repaint();
   }
   /*
   fills an oval at a certian x and y coordiate message to server
   */
   void fillOval(int x, int y, int width, int height)
   {
      char message[22];
      message[0] = 0xFF;
      message[1] = 0x00;                  //length of message
      message[2] = 0x00;                  //length of message
      message[3] = 0x01;                  //length of message payload = 17
      message[4] = 0x01;                  //length of message
      message[5] = 0x0B;                  //message TYPE, set fillOval, payload starts here.
      message[6] = (x >> 12) & 0xF;       //x
      message[7] = (x >> 8) & 0xF;        //x
      message[8] = (x >> 4) & 0xF;        //x
      message[9] = x & 0xF;               //x
      message[10] = (y >> 12) & 0xF;      //y
      message[11] = (y >> 8) & 0xF;       //y
      message[12] = (y >> 4) & 0xF;       //y
      message[13] = y & 0xF;              //y
      message[14] = (width >> 12) & 0xF;  //width
      message[15] = (width >> 8) & 0xF;   //width
      message[16] = (width >> 4) & 0xF;   //width
      message[17] = width & 0xF;          //width
      message[18] = (height >> 12) & 0xF; //height
      message[19] = (height >> 8) & 0xF;  //height
      message[20] = (height >> 4) & 0xF;  //height
      message[21] = height & 0xF;         //height
      send(sockfd, message, 22, 0);
      repaint();
   }
   /*
   sets a certian pixel, a specified color message to server
   */
   void setPixel(int x, int y, int red, int green, int blue)
   {
      char message[20];
      message[0] = 0xFF;
      message[1] = 0x00;                 //length of message
      message[2] = 0x00;                 //length of message
      message[3] = 0x00;                 //length of message payload = 15
      message[4] = 0x0F;                 //length of message
      message[5] = 0x03;                 //message TYPE, set setPixel, payload starts here.
      message[6] = (x >> 12) & 0xF;      //x
      message[7] = (x >> 8) & 0xF;       //x
      message[8] = (x >> 4) & 0xF;       //x
      message[9] = x & 0xF;              //x
      message[10] = (y >> 12) & 0xF;     //y
      message[11] = (y >> 8) & 0xF;      //y
      message[12] = (y >> 4) & 0xF;      //y
      message[13] = y & 0xF;             //y
      message[14] = (red >> 4) & 0x0F;   //MSB
      message[15] = red & 0x0F;          //LSB
      message[16] = (green >> 4) & 0x0F; //MSB
      message[17] = green & 0x0F;        //LSB
      message[18] = (blue >> 4) & 0x0F;  //MSB
      message[19] = blue & 0x0F;         //LSB
      send(sockfd, message, 20, 0);
      repaint();
   }
   /*
   Draws a string of chars on a certian x and y coordinate message to server
   */
   void drawstring(int x, int y, std::string stringOfChars)
   {
      if (stringOfChars.size() > 16535)
      {
         exit(-1);
      }
      int length = (stringOfChars.size() * 2) + 14;       // array space
      int messageLength = (stringOfChars.size() * 2) + 9; //payload space
      char message[length];                               //initialize length of array
      message[0] = 0xFF;
      message[1] = (messageLength >> 12) & 0xF; //length of message
      message[2] = (messageLength >> 8) & 0xF;  //length of message
      message[3] = (messageLength >> 4) & 0xF;  //length of message payload = 9 + stringOfChars()*2
      message[4] = messageLength & 0xF;         //length of message
      message[5] = 0x05;                        //message TYPE, set drawstring, payload starts here.
      message[6] = (x >> 12) & 0xF;             //x
      message[7] = (x >> 8) & 0xF;              //x
      message[8] = (x >> 4) & 0xF;              //x
      message[9] = x & 0xF;                     //x
      message[10] = (y >> 12) & 0xF;            //y
      message[11] = (y >> 8) & 0xF;             //y
      message[12] = (y >> 4) & 0xF;             //y
      message[13] = y & 0xF;                    //y
      int currentIndex = 14;
      for (int i = 0; i < stringOfChars.size(); i++)
      {
         int currentChar = (int)(stringOfChars[i]);
         message[currentIndex] = (currentChar >> 4) & 0x0F; //MSB
         message[currentIndex + 1] = currentChar & 0x0F;    //LSB
         currentIndex += 2;
      }
      send(sockfd, message, length, 0);
      repaint();
   }
   /*
   getServerInfo - this gets information from the server dectecting if a message was sent and sends the data to 
   another function called checkMouseClicked.
   */
   void getServerInfo()
   {
      int len = 0;
      char buffer[256];
      ioctl(sockfd, FIONREAD, &len); //get bytes
      if (len > 0)
      {
         len = read(sockfd, buffer, len); //reads the data
      }
      for (int i = 0; i < len; i++)
      {
         message[i] = buffer[i]; //hex decimal
      }
      checkMouseClicked(message, len);
   }
   /*
   checkMouseClicked - this function gets a message from the server and length.
   If the a certian len of the message is returned, this means a clicked happened.
   Once a click happend this function converts the hex to X and Y int coordinates 
   and passes it to another function called buttonLogic to get the correct button call.
   Then updates the current function call..
   */
   void checkMouseClicked(char *messageFromServer, int len)
   {
      if (len == 30)
      {
         char x[4];
         x[0] = messageFromServer[22]; // part of X hex value
         x[1] = messageFromServer[23]; // part of X hex value
         x[2] = messageFromServer[24]; // part of X hex value
         x[3] = messageFromServer[25]; // part of X hex value
         char y[4];
         y[0] = messageFromServer[26];                                       // part of Y hex value
         y[1] = messageFromServer[27];                                       // part of Y hex value
         y[2] = messageFromServer[28];                                       // part of Y hex value
         y[3] = messageFromServer[29];                                       // part of Y hex value
         char xChar = (messageFromServer[22] << 4) | messageFromServer[23];  //MSB, combine 2 nibs
         char xChar2 = (messageFromServer[24] << 4) | messageFromServer[25]; //LSB, combine 2 nibs
         int xValue = (xChar << 8);                                          // get int value of X
         xValue = xValue + (xChar2 & 0xff);
         char yChar = (messageFromServer[26] << 4) | messageFromServer[27];  //MSB, combine 2 nibs
         char yChar2 = (messageFromServer[28] << 4) | messageFromServer[29]; //LSB, combine 2 nibs
         int yValue = (yChar << 8);                                          // get int value of Y
         yValue = yValue + (yChar2 & 0xff);
         int buttonCall = buttonLogic(xValue, yValue); // get button call from X and Y coordinate
         functionCall = buttonCall;                    // update function call
      }
      if (len == 45)
      {
         char x[4];
         x[0] = messageFromServer[22]; // part of X hex value
         x[1] = messageFromServer[23]; // part of X hex value
         x[2] = messageFromServer[24]; // part of X hex value
         x[3] = messageFromServer[25]; // part of X hex value
         char y[4];
         y[0] = messageFromServer[26];                                       // part of Y hex value
         y[1] = messageFromServer[27];                                       // part of Y hex value
         y[2] = messageFromServer[28];                                       // part of Y hex value
         y[3] = messageFromServer[29];                                       // part of Y hex value
         char xChar = (messageFromServer[22] << 4) | messageFromServer[23];  //MSB, combine 2 nibs
         char xChar2 = (messageFromServer[24] << 4) | messageFromServer[25]; //LSB, combine 2 nibs
         int xValue = (xChar << 8);                                          // get int value of X
         xValue = xValue + (xChar2 & 0xff);
         char yChar = (messageFromServer[26] << 4) | messageFromServer[27];  //MSB, combine 2 nibs
         char yChar2 = (messageFromServer[28] << 4) | messageFromServer[29]; //LSB, combine 2 nibs
         int yValue = (yChar << 8);                                          // get int value of Y
         yValue = yValue + (yChar2 & 0xff);
         int buttonCall = buttonLogic(xValue, yValue); // get button call from X and Y coordinate
         functionCall = buttonCall;                    // update function call
      }
   }
   /*
   This function sends message to server to get a file upload. 
   Then it waits for the user to upload a file or cancel. 
   Then sends that information of filename or flag for cancel.
   */
   void requestLoad()
   {
      cancelFlag = -1; //dummyy
      char message[6];
      message[0] = 0xFF;
      message[1] = 0x00;
      message[2] = 0x00;
      message[3] = 0x00;
      message[4] = 0x01;
      message[5] = 0x0E;
      send(sockfd, message, 6, 0); // send request for file upload
      int len = 0;
      char buffer[256];
      char response[256];

      while (1)
      {
         ioctl(sockfd, FIONREAD, &len); //get bytes
         if (len > 0)
         {
            len = read(sockfd, buffer, len); //reads the data
            for (int i = 0; i < len; i++)
            {
               response[i] = buffer[i]; // get the char array messgae from server response
            }
            if (len != 15)
            {
               char xChar = (response[1] << 4) | response[2];  //MSB, combine 2 nibs
               char xChar2 = (response[3] << 4) | response[4]; //LSB, combine 2 nibs
               int payloadLength = (xChar << 8);
               payloadLength = payloadLength + (xChar2 & 0xff); // get pay load length
               char filePathHex[payloadLength - 1];             // create char array
               int count = 0;
               for (int i = 0; i < payloadLength - 1; i++)
               {
                  filePathHex[i] = response[i + 6]; // add the correct hex chars
                  count = count + 1;
               }
               std::string tempFileName;
               for (int i = 0; i < count; i += 2)
               {
                  char currentCharHex = (filePathHex[i] << 4) | filePathHex[i + 1]; // combine 2 nibs
                  int currentCharInt = (int)(currentCharHex);                       //convert hex to int
                  char currentChar = currentCharInt;                                //convert int to char
                                                                                    // printf("%c",currentChar);
                  tempFileName = tempFileName + currentChar;
               }
               cancelFlag = 0; //User did not press cancel.
               filePathName = tempFileName;
            }
            if (len == 15)
            {
               cancelFlag = 1; //User pressed cancel
            }
            break;
         }
      }
      if (cancelFlag != 0)
      {
         cancelFlag = 1; //User pressed cancel
      }
   }
   /*
   This function gets the x and y coordinates and checks the values with the correct button click on the UI.
   The returns a number that represents a certian button.
   */
   int buttonLogic(int x, int y)
   {
      if (x >= 625 && x <= 770)
      {
         //check y values
         if (y >= 15 && y <= 55)
         {
            //Step function
            return 0;
         }
         else if (y >= 75 && y <= 122)
         {
            //Run
            return 1;
         }
         else if (y >= 143 && y <= 178)
         {
            //Pause
            return 2;
         }
         else if (y >= 200 && y <= 243)
         {
            //Reset
            return 3;
         }
         else if (y >= 260 && y <= 300)
         {
            //Random
            return 4;
         }
         else if (y >= 316 && y <= 360)
         {
            //load
            return 5;
         }
         else if (y >= 377 && y <= 419)
         {
            //clear
            return 6;
         }
         else if (y >= 438 && y <= 483)
         {
            //quit
            return 7;
         }
      }
      if (y >= 552 && y <= 592)
      {
         if (x >= 623 && x <= 664)
         {
            //size 1
            return 8;
         }
         else if (x >= 673 && x <= 716)
         {
            //size 2
            return 9;
         }
         else if (x >= 724 && x <= 768)
         {
            //size 3
            return 10;
         }
      }
      return -1;
   }

   GraphicsClient(const GraphicsClient &sam); // Copy constructor
   ~GraphicsClient();                         // Destructor
};

// //Copy constructor
// GraphicsClient::GraphicsClient(const GraphicsClient &sam) {
//    //cout << "\nCopy constructor called." << endl;
//    port_num = sam.port_num;
//    hostname = sam.hostname;

// }

// // Destructor - CHECK ZYBOOKS for more info
// GraphicsClient::~GraphicsClient() {
//   // cout << "Destructor called." << endl;
// }
