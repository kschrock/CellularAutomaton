#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <algorithm>
#include "GraphicsClient.h"
#include <ctype.h>
#include <string>
#include <unistd.h>

unsigned int microseconds;

class CellularAutomaton
{
public:
    GraphicsClient *obj;
    std::string filename;
    int quiescentState;
    int cellSize;
    int cellGap;
    int cellOffset;
    struct ca_data
    {
        int length;                      //size of array
        int cellValue;                   //initial state
        unsigned char *main_data_malloc; //pointer of unsigned char, this is current state
        int initial_state_first_index;
        int initial_state_last_index;
        int currentFlag;    //flag for edge case
        int numberOfStates; //track number of States per cell, only 2 states are currently valid 0 and 1
        GraphicsClient *obj7;
        unsigned char **cadata;     //initial array
        unsigned char **cadataCOPY; //initial array
        // cadata' for both, i would make the 1d array case just be a 1xWidth 2d array (or you could make a WidthxHeight 1d array for the 2d array)
        int width;               // is an int type that gives the width of the CA
        int height;              // is an int type that gives the height of the CA if it is two-dimensional
        unsigned char dimension; // is an unsigned char flag that contains 1 if the CA is 1 dimensions and if it is 2 dimensions.
    };

    struct ca_data *ptr2;
    FILE *filePointer;

    CellularAutomaton(std::string fileName, int quiescentState)
    {
        filename = fileName;
        FILE *fp;
        char fileText[100];
        char *c = new char[fileName.length() + 1];
        strcpy(c, fileName.c_str());
        fp = fopen(c, "r");
        if (fp == NULL)
        {
            printf("\n%s\" File NOT FOUND!", fileName.c_str());
            exit(EXIT_FAILURE);
        }
        fgets(fileText, 100, fp);
        int row, column;
        sscanf(fileText, "%d %d", &row, &column); //scan the first 2 nums correctly
        this->quiescentState = quiescentState;
        struct ca_data *ptr = create2DCA(column, row, 0);
        ptr->dimension = 2; //update flag
                            // printf("\n");
                            // displayCA(ptr);
        int tempRow = 0;    // in the set method i subtract 1 for correct index
        int tempCol = 0;    // in the set method i subtract 1 for correct index
        while (fgets(fileText, 100, fp))
        {

            const char *pszChar = fileText; //point to the current line of text
            int t[100];                     // array of nums per line
            char *token;                    // initialize token pointer
            token = strtok(fileText, " ");
            int tokens = 0;
            while (1)
            {
                while (token != NULL)
                {
                    t[tokens] = atoi(token);
                    token = strtok(NULL, " ");
                    tokens++; // increase number of total tokens
                }
                for (int k = 0; k < tokens; k++)
                {
                    if (tokens == 1)
                    {
                        // this is to catch last empty line in file
                        break;
                    }
                    set2DCACell(ptr, k, tempCol, t[k]);
                    //set the value for the current position in the 2-D array
                }
                break;
            }
            tempCol++;
            //update current col/height of the 2d array
        }
        int m = std::max(column, row);
        if (m > 200 && m <= 600)
        {
            setCellSize(1);
            setGapSize(0);
            setOffsetSize(1, 0);
        }
        else if (m > 100 && m <= 200)
        {
            setCellSize(2);
            setGapSize(1);
            setOffsetSize(2, 1);
        }
        else if (m > 50 && m <= 100)
        {
            setCellSize(4);
            setGapSize(1);
            setOffsetSize(4, 1);
        }
        else if (m > 1 && m <= 50)
        {
            setCellSize(10);
            setGapSize(2);
            setOffsetSize(10, 2);
        }
        GraphicsClient *obj7 = new GraphicsClient("127.0.0.1", 7777); //Normal constructor
        obj = obj7;                                                   // update GraphicsClient pointer object
        ptr2 = ptr;                                                   // update CA pointer object
        filePointer = fp;                                             // update pointer file object
    }
    /*
    removePointer - frees the pointers that at the CA data and current file
    */
    void removePointer(struct ca_data *ptr, FILE *fp)
    {
        free(ptr);
        fclose(fp);
    }
    /*
    updateGridSize - updates the Cell Size, Cell Gap, and Cell Offset with  the given paramaters
    */
    void updateGridSize(int column, int row)
    {
        int m = std::max(column, row);
        if (m > 200 && m <= 600)
        {
            setCellSize(1);
            setGapSize(0);
            setOffsetSize(1, 0);
        }
        else if (m > 100 && m <= 200)
        {
            setCellSize(2);
            setGapSize(1);
            setOffsetSize(2, 1);
        }
        else if (m > 50 && m <= 100)
        {
            setCellSize(4);
            setGapSize(1);
            setOffsetSize(4, 1);
        }
        else if (m > 1 && m <= 50)
        {
            setCellSize(10);
            setGapSize(2);
            setOffsetSize(10, 2);
        }
    }
    /*
    drawGUI - draws the UI of the Buttons and the line seperating the CA display
    */
    void drawGUI()
    {
        obj->drawLine(600, 0, 600, 600);
        obj->drawRectangle(625, 435, 150, 45);
        obj->drawstring(660, 465, "Quit");
        obj->drawRectangle(625, 375, 150, 45);
        obj->drawstring(660, 405, "Clear");
        obj->drawRectangle(625, 315, 150, 45);
        obj->drawstring(660, 345, "Load");
        obj->drawRectangle(625, 255, 150, 45);
        obj->drawstring(650, 285, "Random");
        obj->drawRectangle(625, 195, 150, 45);
        obj->drawstring(660, 225, "Reset");
        obj->drawRectangle(625, 135, 150, 45);
        obj->drawstring(660, 165, "Pause");
        obj->drawRectangle(625, 75, 150, 45);
        obj->drawstring(660, 105, "Run");
        obj->drawRectangle(625, 10, 150, 45);
        obj->drawstring(660, 40, "Step");

        obj->drawstring(640, 540, "SELECT SIZE");
        obj->drawRectangle(625, 550, 40, 40);
        obj->drawstring(635, 575, "1");
        obj->drawRectangle(675, 550, 40, 40);
        obj->drawstring(685, 575, "2");
        obj->drawRectangle(725, 550, 40, 40);
        obj->drawstring(735, 575, "3");
    }
    /*
    This function takes a Graphics Client object and ca_data stuct object
    This prints the Rectangle of the 1's in the 2d ca_data array.
    */
    void displayCells(GraphicsClient *obj, struct ca_data *ca)
    {
        //obj->setDrawingColor(127, 255, 212);
        obj->clearRectangle(0, 0, 600, 600);
        //obj->setDrawingColor(127, 255, 212);
        for (int i = 0; i < ca->height; i++)
        {
            for (int j = 0; j < ca->width; j++)
            {
                if (ca->cadata[i][j] == 1)
                {
                    int y = i * cellOffset;
                    int x = j * cellOffset;
                    obj->fillRectangle(x, y, cellSize, cellSize); //(int x, int y, int width, int height)
                }
            }
        }
        // drawGUI();
    }
    /*
    This function takes a Graphics Client object and ca_data stuct object
    This CLEARS the Rectangle of the 1's in the 2d ca_data array.
    */
    void clearOldDisplay(GraphicsClient *obj, struct ca_data *ca)
    {
        obj->setDrawingColor(255, 255, 255);
        for (int i = 0; i < ca->height; i++)
        {
            for (int j = 0; j < ca->width; j++)
            {
                if (ca->cadataCOPY[i][j] == 1)
                {
                    int y = i * cellOffset;
                    int x = j * cellOffset;
                    obj->fillRectangle(x, y, cellSize, cellSize); //(int x, int y, int width, int height)
                    obj->clear();
                    obj->repaint();
                }
            }
        }
    }
    /*
    resetInitialState - reset the current CA data with the fileName/upload
    */
    void resetInitialState(struct ca_data *ca, std::string fileName)
    {
        FILE *fp;
        char fileText[100];
        char *c = new char[fileName.length() + 1];
        strcpy(c, fileName.c_str());
        fp = fopen(c, "r");
        if (fp == NULL)
        {
            printf("\n%s\" File NOT FOUND!", fileName.c_str());
            exit(EXIT_FAILURE);
        }
        fgets(fileText, 100, fp);
        int row, column;
        sscanf(fileText, "%d %d", &row, &column); //scan the first 2 nums correctly
        int tempRow = 0;                          // in the set method i subtract 1 for correct index
        int tempCol = 0;                          // in the set method i subtract 1 for correct index
        while (fgets(fileText, 100, fp))
        {

            const char *pszChar = fileText; //point to the current line of text
            int t[100];                     // array of nums per line
            char *token;                    // initialize token pointer
            token = strtok(fileText, " ");
            int tokens = 0;
            while (1)
            {
                while (token != NULL)
                {
                    t[tokens] = atoi(token);
                    token = strtok(NULL, " ");
                    tokens++; // increase number of total tokens
                }
                for (int k = 0; k < tokens; k++)
                {
                    if (tokens == 1)
                    {
                        // this is to catch last empty line in file
                        break;
                    }
                    set2DCACell(ca, k, tempCol, t[k]);
                    //set the value for the current position in the 2-D array
                }
                break;
            }
            tempCol++;
            //update current col/height of the 2d array
        }
        int m = std::max(column, row);
        if (m > 200 && m <= 600)
        {
            setCellSize(1);
            setGapSize(0);
            setOffsetSize(1, 0);
        }
        else if (m > 100 && m <= 200)
        {
            setCellSize(2);
            setGapSize(1);
            setOffsetSize(2, 1);
        }
        else if (m > 50 && m <= 100)
        {
            setCellSize(4);
            setGapSize(1);
            setOffsetSize(4, 1);
        }
        else if (m > 1 && m <= 50)
        {
            setCellSize(10);
            setGapSize(2);
            setOffsetSize(10, 2);
        }
        filePointer = fp;
    }
    void modifyGridSize(struct ca_data *ca, int w, int h)
    {
        ca->cadata = new unsigned char *[h];
        for (int i = 0; i < h; ++i)
        {
            ca->cadata[i] = new unsigned char[w];
        }
        ca->width = w;  //update width of array
        ca->height = h; //update height of array
        ca->dimension = 2;

        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                ca->cadata[i][j] = 0;
            }
        }
        ca->cadataCOPY = new unsigned char *[h];
        for (int i = 0; i < h; ++i)
        {
            ca->cadataCOPY[i] = new unsigned char[w];
        }
        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                ca->cadataCOPY[i][j] = 0;
            }
        }
    }
    void displayCA(struct ca_data *ca);
    void initCA(struct ca_data *ca, int state);
    int set2DCACell(struct ca_data *ca, unsigned int x, unsigned int y, unsigned char state);
    void step2DCA(struct ca_data *ca);
    struct ca_data *create2DCA(int w, int h, unsigned char qstate);
    unsigned char rule(struct ca_data *ptr, int i, int j);
    void setNumberOfStates(struct ca_data *ptr, int numStates);
    void setCellSize(int size);
    void setGapSize(int size);
    void setOffsetSize(int cell, int gap);
};

/*
create2DCA creates a 2d unsigned char array and gives the inital value the qstate
returns ca_data struct 
*/
CellularAutomaton::ca_data *CellularAutomaton::create2DCA(int w, int h, unsigned char qstate)
{
    struct ca_data *ptr = (struct ca_data *)malloc(sizeof(struct ca_data)); //allocate memory for struct pointer
    ptr->cadata = new unsigned char *[h];
    ptr->cadataCOPY = new unsigned char *[h];
    for (int i = 0; i < h; ++i)
    {
        ptr->cadata[i] = new unsigned char[w];
        ptr->cadataCOPY[i] = new unsigned char[w];
    }
    ptr->width = w;  //update width of array
    ptr->height = h; //update height of array
    ptr->dimension = 2;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            ptr->cadata[i][j] = qstate;
            ptr->cadataCOPY[i][j] = qstate;
        }
    }
    return ptr;
}

/*
set2DCACell, sets the x,and y coordinate the given value to the given ca_data stuct.
*/
int CellularAutomaton::set2DCACell(struct ca_data *ca, unsigned int x, unsigned int y, unsigned char state)
{

    if (ca == NULL)
    {
        printf("ERROR, POINTER IS NULL");
    }
    if (x < 0 || y < 0)
    {
        printf("ERROR, index is out of bounds."); // error checking
        exit(-1);
    }
    // printf("x=%d y=%d ", x, y);
    // printf("w=%d h=%d \n", ca->width-1, ca->height-1);
    if (x > ca->width - 1 || y > ca->height - 1)
    {
        printf("x=%d y=%d ", x, y);
        printf("w=%d h=%d value=%d \n", ca->width - 1, ca->height - 1, state);
        printf("ERROR, index is out of bounds."); // error checking
        exit(-1);
    }
    ca->cadata[y][x] = state;
    return 0;
}

/*
This function sets the number of states per cell for the given struct
*/
void CellularAutomaton::setNumberOfStates(struct ca_data *ptr, int numStates)
{
    ptr->numberOfStates = numStates;
}
/*
The 2DCA is stored as an array of unsigned char type 
and the parameters to the function are an int type that is the 
number of cells to initialize (first parameter) 
a pointer to the start of the array (second parameter), 
// note...int* p = a; // equivalent to &a[0]; create a pointer to the first element
*/
void CellularAutomaton::initCA(struct ca_data *ca, int state)
{
    if (ca == NULL)
    {
        printf("ERROR, POINTER IS NULL");
        exit(-1);
    }

    if (state == -1)
    {
        srand(time(NULL)); // Initialization of random generato
        for (int i = 0; i < ca->height; i++)
        {
            for (int j = 0; j < ca->width; j++)
            {
                int randomCell = rand() % 2;   /* random int between 0 and 1 */
                ca->cellValue = randomCell;    //update initialState cell to random 0 or 1
                ca->cadata[i][j] = randomCell; //set to given state
            }
        }
    }
    else
    {
        ca->cellValue = state; //update initialState cell
        for (int i = 0; i < ca->height; i++)
        {
            for (int j = 0; j < ca->width; j++)
            {
                ca->cellValue = state;    //update initialState cell to random 0 or 1
                ca->cadata[i][j] = state; //set to given state
            }
        }
    }
}

/*
Create a function name “display2DCA” that outputs the current state of the 2DCA. The
function takes two parameters. The first parameter is a pointer to the start of the 2DCA
array, and the second parameter is the number of cells in the 2DCA. Each cell state is
separated by a space and terminated with an end of line character (\n). The left must
printed cell is at index 0. Place this function in the file “dca1d.c”
*/
void CellularAutomaton::displayCA(struct ca_data *ca)
{

    for (int i = 0; i < ca->height; i++)
    {
        for (int j = 0; j < ca->width; j++)
        {
            printf("%d ", ca->cadata[i][j]);
        }
        printf("\n");
    }
    printf("\n"); //terminated new line
}

/*
The wrap property should also be stored in this structure, and thus the rule may use it to 
compute the next state.  For a 2DCA the bottom wraps back to the top and the right wraps 
to the left.  With this definition, you should not need to allocate buffer space around the 
CA, and depend on the rule to do the right thing.  (If this is not clear, I will go over this in 
lecture to make sure everyone is doing the easy version of this.) 
*/
void CellularAutomaton::step2DCA(struct ca_data *ca)
{
    if (ca == NULL)
    {
        printf("ERROR, POINTER IS NULL");
        exit(-1);
    }

    struct ca_data *copyOfPtr = (struct ca_data *)malloc(sizeof(struct ca_data));
    copyOfPtr->cadata = new unsigned char *[ca->height];
    for (int i = 0; i < ca->height; ++i)
    {
        copyOfPtr->cadata[i] = new unsigned char[ca->width];
    }
    copyOfPtr->width = ca->width;   //update width of array
    copyOfPtr->height = ca->height; //update height of array

    for (int i = 0; i < ca->height; i++)
    {
        for (int j = 0; j < ca->width; j++)
        {
            copyOfPtr->cadata[i][j] = ca->cadata[i][j]; //copy values into new memory location
        }
    }

    copyOfPtr->currentFlag = ca->currentFlag; //update flag
    //clearOldDisplay(ca->cada);
    for (int i = 0; i < ca->height; i++)
    {
        for (int j = 0; j < ca->width; j++)
        {
            unsigned char value = rule(copyOfPtr, i, j); // function call, with pointer function
            ca->cadata[i][j] = value;                    //update value on 2d
        }
    }

    free(copyOfPtr); //release memory of copy pointer
    copyOfPtr = NULL;
}

/*
rule function, applies the given ruleset to the given 2DCA, with the i and j index.
after it applies the ruleset, it returns a unsigned char which represents the next step value.
*/
unsigned char CellularAutomaton::rule(struct ca_data *ptr, int i, int j)
{
    int num_neighbors = 0;

    if (j + 1 < ptr->width)
    {
        if (ptr->cadata[i][j + 1] == 1)
            num_neighbors++;
    }

    if (i + 1 < ptr->height && j - 1 >= 0)
    {
        if (ptr->cadata[i + 1][j - 1] == 1)
            num_neighbors++;
    }

    if (i + 1 < ptr->height)
    {
        if (ptr->cadata[i + 1][j] == 1)
            num_neighbors++;
    }

    if (i + 1 < ptr->height && j + 1 < ptr->width)
    {
        if (ptr->cadata[i + 1][j + 1] == 1)
            num_neighbors++;
    }

    if (i - 1 >= 0 && j - 1 >= 0)
    {
        if (ptr->cadata[i - 1][j - 1] == 1)
            num_neighbors++;
    }

    if (i - 1 >= 0)
    {
        if (ptr->cadata[i - 1][j] == 1)
            num_neighbors++;
    }

    if (i - 1 >= 0 && j + 1 < ptr->width)
    {
        if (ptr->cadata[i - 1][j + 1] == 1)
            num_neighbors++;
    }

    if (j - 1 >= 0)
    {
        if (ptr->cadata[i][j - 1] == 1)
            num_neighbors++;
    }

    if (ptr->cadata[i][j] == 1)
    {
        if (num_neighbors <= 1 || num_neighbors >= 4)
        {
            //Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            //Any live cell with more than three live neighbours dies, as if by overpopulation.
            return 0;
        }
        if (num_neighbors == 2 || num_neighbors == 3)
        {
            return 1;
        }
    }
    else if (ptr->cadata[i][j] == 0 && num_neighbors == 3)
    {
        //Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
        return 1;
    }
    else
    {
        //All other live cells die in the next generation. Similarly, all other dead cells stay dead.
        return 0;
    }
}

/*
setCellSize, sets the size to the Cell size
*/
void CellularAutomaton::setCellSize(int size)
{
    cellSize = size;
}
/*
setGapSize, sets the size to the gap size
*/
void CellularAutomaton::setGapSize(int size)
{
    cellGap = size;
}
/*
setOffsetSize, sets the offset to the cells
*/
void CellularAutomaton::setOffsetSize(int cell, int gap)
{
    cellOffset = cell + gap;
}