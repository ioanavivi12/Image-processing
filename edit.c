#include "bmp_header.h"
// DABELEA IOANA-VIVIANA 313CBb
int main() 
{
	char *command = NULL;
	const int len = 200;
	int i = 0, j = 0, gr = 0, rd = 0, bl = 0, ln = 0, ok = 1;
	bmp_fileheader fheader, newfheader;
	bmp_infoheader iheader, newiheader;
	BGR **bitmap = NULL, **newbitmap = NULL, init;
	draw pen;
	point p1, p2, *pct = NULL, lg, p;
	pct = (point *)malloc((len / 4) * sizeof(point));
	if (!pct) {
		free(pct);
		return -1;
	}
	command = malloc(len * sizeof(char));
	if (!command) {
		free(command);
		free(pct);
		printf("Could not allocate!");
		return -1;
	}
	while (scanf("%s", command) == 1) {
		if (strcmp(command, "edit") == 0) {
			scanf("%s", command);
			FILE *IMAGE = fopen(command, "rb");
			if (!IMAGE) {
				fclose(IMAGE);
				printf("Could not open this file");
			} else {
				fread(&fheader, sizeof(bmp_fileheader), 1, IMAGE);
				fread(&iheader, sizeof(bmp_infoheader), 1, IMAGE);
				fseek(IMAGE, fheader.imageDataOffset, SEEK_SET);
				bitmap = build_bitmap(IMAGE, iheader);
				fclose(IMAGE);
			}
		} else if (strcmp(command, "save") == 0) {
			scanf("%s", command);
			save(command, fheader, iheader, bitmap);
		} else if (strcmp(command, "quit") == 0) {
			quit(bitmap, iheader);
		} else if (strcmp(command, "insert") == 0) {
			scanf("%s", command);
			scanf("%d %d", &p.y, &p.x);
			FILE *IMAGE = fopen(command, "rb");
			if (!IMAGE) {
				fclose(IMAGE);
				printf("Could not open this file");
			} else {
				fread(&newfheader, sizeof(bmp_fileheader), 1, IMAGE);
				fread(&newiheader, sizeof(bmp_infoheader), 1, IMAGE);
				newbitmap = build_bitmap(IMAGE, newiheader);
				fclose(IMAGE);
				insert(bitmap, newbitmap, iheader, newiheader, p);
				quit(newbitmap, newiheader);
			}
		} else if (strcmp(command, "set") == 0) {
			scanf("%s ", command);
			if (strcmp(command, "draw_color") == 0) {
				scanf("%d %d %d", &rd, &gr, &bl);
				pen.color.Red = rd;
				pen.color.Blue = bl;
				pen.color.Green = gr;
			}  else if (strcmp(command, "line_width") == 0) {
				scanf("%d", &ln);
				pen.line = ln;
			}
		} else if (strcmp(command, "draw") == 0) {
			scanf("%s", command);
			if (strcmp(command, "line") == 0) {
				scanf("%d %d %d %d", &p1.y, &p1.x, &p2.y, &p2.x);
				draw_line(p1, p2, pen, bitmap, iheader.width, iheader.height);
			}  else if (strcmp(command, "rectangle") == 0) {
				scanf("%d %d %d %d", &p1.y, &p1.x, &lg.y, &lg.x);
				i = iheader.height;
				j = iheader.width;
				draw_rectangle(p1, pen, bitmap, lg, j, i);
			} else if (strcmp(command, "triangle") == 0) {
				for (i = 0; i < 3; i++)
					scanf("%d %d", &pct[i].y, &pct[i].x);
				draw_triangle(pct, pen, bitmap, iheader.width, iheader.height);
			}
		} else if (strcmp(command, "fill") == 0) {
			scanf("%d %d", &p.y, &p.x);
			if (compare(bitmap[p.x][p.y], pen.color) != 0) {
				init = bitmap[p.x][p.y];
				fill(p, pen, bitmap, iheader.width, iheader.height, init);
				bitmap[p.x][p.y] = pen.color;
			}
		}
	}
	free(command);
	free(pct);
	return 0;
}
