typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;

#define UNUSED_PARAM(param) ((void)((param)))

#define REG_DISPCTL *(u16 *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define COLOR(r,g,b) ((r) | (g) << 5 | (b) << 10)
#define RED COLOR(31,0,0)
#define WHITE COLOR(31,31,31)
#define GREEN COLOR(0, 31, 0)
#define BLACK 0

#define WIDTH 240 //this is the width of the GBA emulator

unsigned short *videoBuffer = (u16 *)0x6000000;

void setPixel(int row, int col, u16 color);
void drawRectangle(int row, int col, int width, int height, u16 color);
void drawHollowRectangle(int row, int col, int width, int height, u16 color);

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	// use the functions you wrote to draw a pretty picture here!

	setPixel(10, 10, RED);
	drawRectangle(20, 20, 60, 20, WHITE);
	drawHollowRectangle(40, 100, 40, 20, GREEN);

	while(1);
}

void setPixel(int row, int col, u16 color)
{
	//TODO: set the pixel at the particular row and column
	videoBuffer[(row * WIDTH) + col] = color;

}

void drawRectangle(int row, int col, int width, int height, u16 color)
{
	//TODO: Draws a rectangle of the particular width and height with the left-most pixel being located at (row, col)
	int x;
	int y;
	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			videoBuffer[((row + y) * WIDTH) + col + x] = color;
		}
	}
}

void drawHollowRectangle(int row, int col, int width, int height, u16 color)
{
	//TODO: Draws a hollow rectangle of the particular width and height with the left-most pixel being located at (row, col)
	// MUST BE EFFICIENT - cannot be O(n*n) time
	int x;
	int y;
	for (x = 1; x < width; x++)
	{
		videoBuffer[(row * WIDTH) + col + x] = color;
		videoBuffer[((row + height) * WIDTH) + col + x] = color;
	}
	for (y = 0; y < height + 1; y++)
	{
		videoBuffer[((row + y) * WIDTH) + col] = color;
		videoBuffer[((row + y) * WIDTH) + col + width] = color;
	}
}

int smartDivision(int a, int b, int *q, int *r)
{
	*q = a / b;
	*r = a % b;
	return (*r == 0);
}

#define MULT(a, b, c) ((a) * (b) * (c))

CoordinatePair *ptr = pairs[2];
CoordinatePair pair = *pairs[4];
pairs[3] -> x = pairs[5] -> y;