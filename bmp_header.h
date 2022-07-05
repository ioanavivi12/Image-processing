#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Tells the compiler not to add padding for these structs. This may
   be useful when reading/writing to binary files.
   http://stackoverflow.com/questions/3318410/pragma-pack-effect
*/
#pragma pack(1)

typedef struct {
	unsigned char  fileMarker1; /* 'B' */
	unsigned char  fileMarker2; /* 'M' */
	unsigned int   bfSize; /* File's size */
	unsigned short unused1; /* Aplication specific */
	unsigned short unused2; /* Aplication specific */
	unsigned int   imageDataOffset; /* Offset to the start of image data */
} bmp_fileheader;

typedef struct {
	unsigned int   biSize; /* Size of the info header - 40 bytes */
	signed int     width; /* Width of the image */
	signed int     height; /* Height of the image */
	unsigned short planes;
	unsigned short bitPix;
	unsigned int   biCompression; /* Type of compression */
	unsigned int   biSizeImage; /* Size of the image data */
	int            biXPelsPerMeter;
	int            biYPelsPerMeter;
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;
} bmp_infoheader;
typedef struct BGR {
		unsigned char Blue;
		unsigned char Green;
		unsigned char Red;
} BGR;

typedef struct draw {
	BGR color;
	unsigned char line;
} draw;

typedef struct point {
	int x, y;
} point;
#pragma pack()
int compare(BGR x, BGR y) 
{
	if (x.Blue != y.Blue)
		return -1;
	if (x.Green != y.Green)
		return -1;
	if (x.Red != y.Red)
		return -1;
	return 0;
}

int Max(int x, int y) 
{
	if (x > y)
		return x;
	return y;
}

int Min(int x, int y) 
{
	if (x < y)
		return x;
	return y;
}

void quit(BGR **m1, bmp_infoheader hm1) 
{
	int i = 0;
	for (i = 0; i < hm1.height; i++)
		free(m1[i]);
	free(m1);
}

void insert(BGR **m, BGR **m2, bmp_infoheader h1, bmp_infoheader h2, point p) 
{
	int i = 0, j = 0;
	for (i = 0; i < h2.height && p.x + i < h1.height; i++)
		for (j = 0; j < h2.width && p.y + j < h1.width; j++)
			m[p.x + i][p.y + j] = m2[i][j];
}

void save(char *s, bmp_fileheader fheader, bmp_infoheader iheader, BGR **a) 
{
	int i = 0, j = 0, r = 0, aux = 0;
	char zero = 0;
	BGR null;
	null.Blue = null.Green = null.Red = 0;
	FILE *IMAGE = fopen(s, "wb");
	if (!IMAGE) {
		fclose(IMAGE);
		printf("Could not open this file");
	} else {
		fwrite(&fheader, sizeof(bmp_fileheader), 1, IMAGE);
		fwrite(&iheader, sizeof(bmp_infoheader), 1, IMAGE);
		for (i = (int)ftell(IMAGE); i < fheader.imageDataOffset; i++)
			fwrite(&zero, sizeof(char), 1, IMAGE);
		fseek(IMAGE, fheader.imageDataOffset, SEEK_SET);
		for (i = 0; i < iheader.height; i++) {
			fwrite(a[i], sizeof(BGR), iheader.width, IMAGE);
			if (iheader.width % 4 != 0) {
			for (r = 0; r < 4 - (3 * iheader.width) % 4; r++)
					fwrite(&zero, sizeof(char), 1, IMAGE);
			 }
		}
		fclose(IMAGE);
	}
}

void draw_point(point p, draw pen, BGR **bitmap, int w, int h) 
{
	int i = 0, j = 0, lg = 0;
	lg = pen.line / 2;
	for (i = Max(0, p.x - lg); i <= Min(p.x + lg, h - 1); i++)
		for (j = Max(0, p.y - lg); j <= Min(p.y + lg, w - 1); j++)
			bitmap[i][j] = pen.color;
}

void line_OY(point p1, point p2, draw pen, BGR **bitmap, int w, int h) 
{
	point dif, p;
	int i = 0;
	dif.x = p2.x - p1.x;
	dif.y = p2.y - p1.y;
	for (i = Min(p1.y, p2.y); i <= Max(p2.y, p1.y); i++) {
		p.x = ((i * dif.x) - (p1.y * dif.x) + (p1.x * dif.y));
		p.x /= dif.y;
		p.y = i;
		if ((p.x < h && p.x >= 0) && (p.y < w && p.y >= 0))
			draw_point(p, pen, bitmap, w, h);
	}
}

void line_OX(point p1, point p2, draw pen, BGR **bitmap, int w, int h) 
{
	point dif, p;
	int i = 0;
	dif.x = p2.x - p1.x;
	dif.y = p2.y - p1.y;
	for (i = Min(p1.x, p2.x); i <= Max(p2.x, p1.x); i++) {
		p.x = i;
		p.y = ((i * dif.y) - (p1.x * dif.y) + (p1.y * dif.x));
		p.y /= dif.x;
		if ((p.x < h && p.x >= 0) && (p.y < w && p.y >= 0))
			draw_point(p, pen, bitmap, w, h);
	}
}

void draw_rectangle(point p, draw pen, BGR **a, point lg, int w, int h) 
{
	int i = 0;
	for (i = 0; i < lg.y; i++) {
		if (p.y >= 0 && p.y < w && p.x >= 0 && p.x < h)
			draw_point(p, pen, a, w, h);
		p.y++;
	}
	for (i = 0; i < lg.x; i++) {
		if (p.y >= 0 && p.y < w && p.x >= 0 && p.x < h)
			draw_point(p, pen, a, w, h);
		p.x++;
	}
	for (i = 0; i < lg.y; i++) {
		if (p.y >= 0 && p.y < w && p.x >= 0 && p.x < h)
		   draw_point(p, pen, a, w, h);
		p.y--;
	}
	for (i = 0; i < lg.x; i++) {
		if (p.y >= 0 && p.y < w && p.x >= 0 && p.x < h)
			draw_point(p, pen, a, w, h);
		p.x--;
	}
}

void draw_line(point start, point end, draw pen, BGR **a, int w, int h) 
{
	point mx, mn;
	draw_point(start, pen, a, w, h);
	draw_point(end, pen, a, w, h);
	mx.x = Max(start.x, end.x);
	mx.y = Max(start.y, end.y);
	mn.x = Min(start.x, end.x);
	mn.y = Min(start.y, end.y);
	if (mx.x - mn.x >= mx.y - mn.y)
		line_OX(start, end, pen, a, w, h);
	else if (mx.x - mn.x < mx.y - mn.y)
		line_OY(start, end, pen, a, w, h);
}

void draw_triangle(point *pct, draw pen, BGR **bitmap, int w, int h) 
{
	draw_line(pct[0], pct[1], pen, bitmap, w, h);
	draw_line(pct[1], pct[2], pen, bitmap, w, h);
	draw_line(pct[2], pct[0], pen, bitmap, w, h);
}

BGR **build_bitmap(FILE *in, bmp_infoheader x) 
{
	BGR **a = NULL, aux;
	int i = 0, j = 0, ok = 1;
	a = malloc(x.height * sizeof(int *));
	if (!a) {
		free(a);
	}
	for (i = 0; i < x.height; i++) {
		a[i] = malloc(x.width * sizeof(BGR));
		if (!a[i]) {
			for (j = 0; j < i; j++)
				free(a[j]);
			free(a);
			ok = 0;
			printf("Could not allocate");
		}
	}
	if (ok == 1) {
		for (i = 0; i < x.height; i++) {
			for (j = 0; j < x.width; j++) {
				fread(&aux, sizeof(BGR), 1, in);
				a[i][j] = aux;
			}
			if (x.width % 4 != 0)
				fseek(in, (4 -  (3 * x.width) % 4), SEEK_CUR);
		}
		return a;
	}
}

void fill(point p, draw pen, BGR **bitmap, int w, int h, BGR init) 
{
	if ((p.y - 1 >= 0) && compare(bitmap[p.x][p.y - 1], init) == 0) {
		bitmap[p.x][p.y - 1] = pen.color;
		p.y--;
		fill(p, pen, bitmap, w, h, init);
		p.y++;
	}
	if ((p.x + 1 < h) && compare(bitmap[p.x + 1][p.y], init) == 0) {
		bitmap[p.x + 1][p.y] = pen.color;
		p.x++;
		fill(p, pen, bitmap, w, h, init);
		p.x--;
	}
	if ((p.y + 1 < w) && compare(bitmap[p.x][p.y + 1], init) == 0) {
		bitmap[p.x][p.y + 1] = pen.color;
		p.y++;
		fill(p, pen, bitmap, w, h, init);
		p.y--;
	}
	if ((p.x - 1 >= 0) && compare(bitmap[p.x - 1][p.y], init) == 0) {
		bitmap[p.x - 1][p.y] = pen.color;
		p.x--;
		fill(p, pen, bitmap, w, h, init);
		p.x++;
	}
}
