## Application Features
 source files and what they contain, and a discussion of each class in the source code.

### “GraphicsClient.h file  
This class encapsulates drawing methods that can be made to a
graphics server program via network messages. (described in another document) The
idea is that when you create GraphicsClient object, it links the object to a specific
window that is opened by the graphics server. If you create two of these obejcts, then
two windows are created by the graphics server program. When an object is destroyed,
then the network connection should be terminated and the appropriate actions will be
taken on the graphics server side. Note that the server program may be given connections 
to the same address and port, each connection gets a different socket to write data. If
;you need more information on how this works, please ask in class and look at the
example code that is posted. 
Two types of messages may be sent back to the client: Mouse message and file selection message. These
have the same general format as the message that may be sent to the graphics server, and are detailed
below. In the mouse messages below, the BUTTON NUMBER is 1 for left button, 2 for middle button,
and 3 for right button. The x and y coordinates are the x and y coordinates of where the mouse is when
the button action occurs. A click is only sent if there is a press and release action within a certain amount
of time. (operating system dependent)

### CellularAutomaton.h file 
The ideas behind this class is that it represents a 2DCA and therefore should have methods associated with loading and simulating a 2DCA. This reads a filename and then creates a 2DCA from that file, then runs the rule on each step that is applied to the  2DCA. Then sends the data to the Graphics Client to show the alive cells. 
