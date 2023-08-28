# coms327P2, Part A
Submitted by: **Kordell Schrock**

## Application Features

### Core Features
- [X] **GraphicsClient( std::string, int )** A constructor with two parameters for GraphicsClient.
- [X] **setBackgroundColor( int , int, int )** sets the background color of the associated display. The parameters are the red, green and then blue values respectively.
- [X] **setDrawingColor( int, int, int )** set the color that objects will be drawn at until
another setDrawingColor call is made. The parameters are again, red, green and
blue.
- [X] **clear()** clears the display to the background color 
- [X] **setPixel( int, int, int, int, int )** sets the pixel at the location given by the first two
parameters to the color given by the last three parameters. The first two
parameter are the location given by the x and then y coordinate. The last three
parameters are the color given by red, green, and blue in that order.
- [X] **drawRectangle( int, int, int, int )** draws a rectangle at the specified coordinates
given by the first two parameters of the specified size given by the last two
parameters. The first two parameters are again the x and y coordinate in that
order, and the last two parameters are the width and height, also given in that
order.
- [X] **fillRectangle( int, int, int, int )** draws a filled rectangle at the position and size
given by the parameters. The parameters are the same as the drawRectangle
parameters.
- [X] **clearRectangle( int, int, int, int )** clears (sets the pixels to the background color) at
the location and size specified by the parameters. These parameters are the same
as the drawRectangle parameters
array
- [X] **drawOval( int, int, int, int )**  draws an oval at the specified location inscribed in a
rectangle of the specified size. The parameters are the same as given in
drawRectangle.
- [X] **fillOval( int, int, int, int )** is the same as the drawOval method except the oval is
filled.
- [X] **drawLine( int, int, int, int )**  draws a string of characters on the display given by
the last parameter at the position given by the first two parameters, x, y, in that
order.
- [X] **drawstring( int, int, string )**  draws an oval at the specified location inscribed in a
rectangle of the specified size. The parameters are the same as given in
drawRectangle.
- [X] **repaint()**  send the redraw (repaint) signal to the attached graphics server.
- [X] **CellularAutomaton** 
* 1) Write a class definition (header file CellularAutomaton.h) that represents a 2D cellular
automaton named “CellularAutomaton” The ideas behind this class is that it represents
a 2DCA and therefore should have methods associated with loading and simulating a
2DCA. The class contains the following definitions.
* 2) A constructor that takes two arguments. First, a C++ std::string argument that is a
name of a file. Second, it gives an int parameter that is the quiescent state of the
CA. It then reads a 2DCA in the same format as Project 1C.
* 3) Proper copy constructor, assignment = , and destructors. The first two must
perform deep copies.
* 4) A step function that takes a single argument that is the rule function and performs
one step of the 2DCA.
* 5) A method that takes a single reference parameter to a GraphicsClient object and
display the 2DCA on the attached graphics window associated with the
GraphicsClient object. The size of the cell displayed on the window is dependent
on the size of the 2DCA width and height according to the following table. Let m
be the maximum value of the width and height of the 2DCA.
* 6)
m Cell size in pixels Cell gap in pixels
200 < m <= 600
1 0
100 < m <= 200 
2 1
50 < m <= 100 
4 1
1 < m <= 50 
10 2
* 7)
a. STEP.  Exceutes one step of the CA and displays the result. 
b. RUN. Continulously runs (steps) the CA at a rate of approximately 1 step every 
100 ms.  (You may add a speed control for variable speed control if you wish.) 
c. PAUSE.  If the CA is in the run mode, the CA will stop running. 
d. QUIT. Terminites the and exits the program. 
e. RESET. Sets the state of the CA back to the initial state when it was loaded 
f. LOAD.  Uses a “file browser” to select a file to load.  (see additional messages in 
revised message format document) 
g. CLEAR.  Sets all the cells to state 0. 
h. RANDOMIZE.  Sets the cells in the CA to random initial states.

- [X] **create makefile** Make sure an updated make file works correctly, and that the executable file created is called “gol” 
- [X] **documentation** 10% points come from documentation and following the syllabus requirements for
projects.


### main file has default blinker.txt. Just change filename for different file in the main.
### the main logic is in the default constructor, did not have time to move over to main. 
### working video of simulation called "Working Simulator" in this folder.

### You need 2 terminals to run this, one for Server and one for compiling makeFile

### Step to run local Server
1. java -jar GraphicsServerV2.jar 7777

### Steps to run makeFile
1. make gol 
2. ./gol 
3. make clean

