/**
* @file graphics.c
* @author Izaan Kamal
* @date 11/4/18
* @brief A graphics library for drawing geometry, for Homework 9 of Georgia Tech
*        CS 2110, Fall 2018.
*/

// Please take a look at the header file below to understand what's required for
// each of the functions.
#include "graphics.h"

// Don't touch this. It's used for sorting for the drawFilledPolygon function.
int int_cmp(const void *a, const void *b)
{
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    return *ia  - *ib;
}

Pixel noFilter(Pixel c) {
    // Don't touch this.
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel greyscaleFilter(Pixel c) {
    int mask = 0x1F;
    int r = c & mask;
    c = c >> 5;
    int g = c & mask;
    c = c >> 5;
    int b = c & mask;

    int n = ((r*77) + (g*151) + (b*28)) >> 8;
    c = (((n << 5) + n) << 5) + n;
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel redOnlyFilter(Pixel c) {
    int mask = 0x1F;
    c = c & mask;
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel brighterFilter(Pixel c) {
    int mask = 0x1F;
    int r = c & mask;
    c = c >> 5;
    int g = c & mask;
    c = c >> 5;
    int b = c & mask;

    int diff = 31 - r;
    r = r + (diff >> 1);

    diff = 31 - g;
    g = g + (diff >> 1);

    diff = 31 - b;
    b = b + (diff >> 1);

    c = (((b << 5) + g) << 5) + r;
    return c;
}


// TODO: Complete according to the prototype in graphics.h
void drawPixel(Screen *screen, Vector coordinates, Pixel pixel) {
    int x = coordinates.x;
    int y = coordinates.y;

    int width = screen -> size.x;
    int height =  screen -> size.y;

    if (x >= 0 && y >= 0 && x < width && y < height) {
        screen -> buffer[(y * width) + x] = pixel;
    }
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledRectangle(Screen *screen, Rectangle *rectangle) {
    int x = rectangle -> top_left.x;
    int y = rectangle -> top_left.y;
    int width = rectangle -> size.x;
    int height = rectangle -> size.y;
    Pixel color = rectangle -> color;

    int row;
    int col;

    for(row = 0; row < height; row++)
    {
        for(col = 0; col < width; col++)
        {
            Vector coordinates;
            coordinates.x = x + col;
            coordinates.y = y + row;
            drawPixel(screen, coordinates, color);
        }
    }

}

// TODO: Complete according to the prototype in graphics.h
void drawLine(Screen *screen, Line *line) {
	int x1 = line -> start.x;
    int y1 = line -> start.y;
    int x2 = line -> end.x;
    int y2 = line -> end.y;
    Pixel color = line -> color;
    Vector coordinates;

    int changed = 0;
    int x = x1;
    int y = y1;
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (dx < 0)
    {
        dx = dx * -1;
    }
    if (dy < 0)
    {
        dy = dy * -1;
    }
    int signx = x2 - x1;
    int signy = y2 - y1;
    signx = (signx > 0) ? 1 : ((signx < 0) ? -1 : 0);
    signy = (signy > 0) ? 1 : ((signy < 0) ? -1 : 0);

    int temp;
    if (dy > dx)
    {
        temp = dy;
        dy = dx;
        dx = temp;
        changed = 1;
    }

    int e = 2 * dy - dx;
    int i;
    for (i = 1; i <= dx; i++)
    {
        coordinates.x = x;
        coordinates.y = y;
        drawPixel(screen, coordinates, color);

        while (e >= 0)
        {
            if (changed)
            {
                x += signx;
            } else {
                y += signy;
            }
            e = e - 2 * dx;
        }
        if (changed)
        {
            y += signy;
        } else {
            x += signx;
        }
        e = e + 2 * dy;
    }
    coordinates.x = x2;
    coordinates.y = y2;
    drawPixel(screen, coordinates, color);
}

// TODO: Complete according to the prototype in graphics.h
void drawPolygon(Screen *screen, Polygon *polygon) {
    int i;
    Line line;
    line.color = polygon -> color;
    Line *ptr_line = &line;
    for (i = 0; i < (polygon -> num_vertices) - 1; i++)
    {
        line.start = polygon -> vertices[i];
        line.end = polygon -> vertices[i+1];
        drawLine(screen, ptr_line);
    }
    line.start = polygon -> vertices[0];
    line.end = polygon -> vertices[(polygon -> num_vertices) - 1];
    drawLine(screen, ptr_line);
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledPolygon(Screen *screen, Polygon *polygon) {
    UNUSED(screen);
	int min_y = INT_MAX;
    int max_y = INT_MIN;

    // -------------------------------------------------------------------------
    // TODO: Here, write some code that will find the minimum and maximum
    // Y values that belong to vertices of the polygon, and store them as
    // min_y and max_y.
    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------
    // END OF TODO
    // -------------------------------------------------------------------------

    // Now we iterate through the rows of our polygon
	for (int row = min_y; row <= max_y; row++) {
        // This variable contains the number of nodes found. We start with 0.
		int nNodes = 0;

        // This array will contain the X coords of the nodes we find.
        // Note that there are at most num_vertices of those. We allocate
        // that much room, but at any time only the first nNodes ints will
        // be our actual data.
        int nodeX[polygon->num_vertices];

        // This loop finds the nodes on this row. It intersects the line
        // segments between consecutive pairs of vertices with the horizontal
        // line corresponding to the row we're on. Don't worry about the
        // details, it just works.
		int j = polygon->num_vertices - 1;
		for (int i = 0; i < polygon->num_vertices; i++) {
			if ((polygon->vertices[i].y < row && polygon->vertices[j].y >= row) ||
				(polygon->vertices[j].y < row && polygon->vertices[i].y >= row)) {
				nodeX[nNodes++] = (polygon->vertices[i].x +
                    (row - polygon->vertices[i].y) *
                    (polygon->vertices[j].x - polygon->vertices[i].x) /
                    (polygon->vertices[j].y - polygon->vertices[i].y));
			}
			j = i;
		}

        // ---------------------------------------------------------------------
        // TODO: Make a call to qsort here to sort the nodeX array we made,
        // from small to large x coordinate. Note that there are nNodes elements
        // that we want to sort, and each is an integer. We give you int_cmp
        // at the top of the file to use as the comparator for the qsort call,
        // so you can just pass it to qsort and not need to write your own
        // comparator.
        // ---------------------------------------------------------------------


        // ---------------------------------------------------------------------
        // END OF TODO
        // ---------------------------------------------------------------------



        // ---------------------------------------------------------------------
        // TODO: Now we fill the x coordinates corresponding to the interior of
        // the polygon. Note that after every node we switch sides on the
        // polygon, that is, if we are on the outside, when we pass a node we
        // end up on the inside, and if are inside, we end up on the outside.
        // As a result, all you need to do is start at the 0th node, iterate
        // through all of the even-indexed nodes, and fill until the next node.
        // For example, you need to fill between nodes 0-1, between nodes 2-3,
        // nodes 4-5 etc. but not between nodes 1-2, or nodes 3-4.
        // ---------------------------------------------------------------------
        UNUSED(nodeX);

        // ---------------------------------------------------------------------
        // END OF TODO
        // ---------------------------------------------------------------------
	}
}

// TODO: Complete according to the prototype in graphics.h
void drawRectangle(Screen *screen, Rectangle *rectangle) {
    UNUSED(screen);
    UNUSED(rectangle);
}

// TODO: Complete according to the prototype in graphics.h
void drawCircle(Screen *screen, Circle *circle) {
    int mx = circle -> center.x;
    int my = circle -> center.y;
    int radius = circle -> radius;

    int x = 0;
    int y = radius;
    int d = 1 - radius;

    Vector coordinates;
    Pixel color = circle -> color;

    while (x <= y)
    {
        coordinates.x = mx + x;
        coordinates.y = my + y;
        drawPixel(screen, coordinates, color);
        coordinates.y = my - y;
        drawPixel(screen, coordinates, color);

        coordinates.x = mx - x;
        drawPixel(screen, coordinates, color);
        coordinates.y = my + y;
        drawPixel(screen, coordinates, color);

        coordinates.x = mx + y;
        coordinates.y = my + x;
        drawPixel(screen, coordinates, color);
        coordinates.y = my - x;
        drawPixel(screen, coordinates, color);

        coordinates.x = mx - y;
        drawPixel(screen, coordinates, color);
        coordinates.y = my + x;
        drawPixel(screen, coordinates, color);

        if (d < 0)
        {
            d = d + 2 * x + 3;
            x += 1;
        } else
        {
            d = d + 2 * (x-y) + 5;
            x += 1;
            y -= 1;
        }
    }

}

// TODO: Complete according to the prototype in graphics.h
void drawFilledCircle(Screen *screen, Circle *circle) {
    int mx = circle -> center.x;
    int my = circle -> center.y;
    int radius = circle -> radius;

    int x = 0;
    int y = radius;
    int d = 1 - radius;

    Line line;
    line.color = circle -> color;
    Line *ptr_line = &line;

    Vector start;
    Vector end;

    while (x <= y)
    {
        start.x = mx + x;
        start.y = my + y;
        end.x = mx + x;
        end.y = (my > y) ? (my - y) : 0;
        line.start = start;
        line.end = end;
        drawLine(screen, ptr_line);

        if (mx >= x)
        {
            start.x = mx - x;
            start.y = my + y;
            end.x = mx - x;
            end.y = (my > y) ? my - y : 0;
            line.start = start;
            line.end = end;
            drawLine(screen, ptr_line);
        }

        start.x = mx + y;
        start.y = my + x;
        end.x = mx + y;
        end.y = (my > x) ? (my - x) : 0;
        line.start = start;
        line.end = end;
        drawLine(screen, ptr_line);

        if (mx >= y)
        {
            start.x = mx - y;
            start.y = my + x;
            end.x = mx - y;
            end.y = (my > x) ? (my - x) : 0;
            line.start = start;
            line.end = end;
            drawLine(screen, ptr_line);
        }

        if (d < 0)
        {
            d = d + 2 * x + 3;
            x += 1;
        } else {
            d = d + 2 * (x-y) + 5;
            x += 1;
            y -= 1;
       }
    }
}

// TODO: Complete according to the prototype in graphics.h
void drawImage(Screen *screen, Image *image, Pixel (*colorFilter)(Pixel)) {
    int x = image -> top_left.x;
    int y = image -> top_left.y;
    int width = image -> size.x;
    int height = image -> size.y;

    int row;
    int col;

    for(row = 0; row < height; row++)
    {
        for(col = 0; col < width; col++)
        {
            Vector coordinates;
            coordinates.x = x + col;
            coordinates.y = y + row;
            int color = colorFilter((image -> buffer)[row * width + col]);
            drawPixel(screen, coordinates, color);
        }
    }
}

// TODO: Complete according to the prototype in graphics.
Image rotateImage(Image *image, int degrees) {
    int width = image -> size.x;
    int height = image -> size.y;
    Pixel *nBuff = malloc(sizeOf(Pixel) * width * height);
    Pixel *oBuff = malloc(sizeOf(Pixel) * width * height);
    int i;
    for (i = 0; i < degrees / 90; i++)
    {
        nBuff[i] = image -> buffer[i];
        oBuff[i] = image -> buffer[i];
    }
    Image nImage = (Image){image -> top_left, image -> size, nBuff};
    while(degrees < 0)
    {
        degrees = degrees + 360;
    }
    for (i = 0; i < degrees / 90; i++)
