#include <stdio.h>
#include <stdlib.h>

/////////////////////////////////////////////////////////////
///// Sort functions
/////////////////////////////////////////////////////////////

int sort_get_index(const double tab[], int top, double val) {
    int indice = top;

    for (int i=0; i<top; i++) {
        if (tab[i] > val) {
            indice=i;
            break;
        }
    }
    return indice;
}

void sort_insert_at(double tab[], int i, int top, double val) {
    int j = top-1;
    while (j >= i) {
        tab[j + 1] = tab[j];
        j = j - 1;
    }
    tab[j + 1] = val;

}

void sort_insert(double tab[], int top, double val) {
    int i = sort_get_index(tab, top, val);
    sort_insert_at(tab, i, top, val);
}

/////////////////////////////////////////////////////////////
///// Grid
/////////////////////////////////////////////////////////////

#define NB_COLUMNS 32
#define NB_LINES 16

void grid_init(char grid[], char pixel) {

    for (int i=0; i < NB_LINES*NB_COLUMNS; i++) {
        grid[i] = pixel;
    }
}

void grid_display(const char grid[]) {
    for (int i=0; i < NB_LINES; i++) {
        for (int j = 0; j < NB_COLUMNS; ++j) {
            printf("%c", grid[i*NB_COLUMNS + j]);
        }
        printf("\n");
    }
}

void plot_point(char grid[], int x, int y, char pixel) {
    if (NB_COLUMNS*(NB_LINES - y - 1) < 0 || x > NB_COLUMNS) {
        exit(EXIT_FAILURE);
    }
    else {
        grid[NB_COLUMNS*(NB_LINES - y - 1) + x] = pixel;
    }
}

void plot_vline(char grid[], int x, double fy0, double fy1, char pixel) {
    fy0 = fy0 + 0.5; // correct round up
    fy1 = fy1 + 0.5; // correct round up
    int y0 = (int) fy0;
    int y1 = (int) fy1;

    for (int j = y0; j <= y1; ++j) {
        grid[NB_COLUMNS*(NB_LINES - j - 1)+ x] = pixel;
    }

}


int main() {
    printf("TP2 !\n");

    char grid[NB_COLUMNS*NB_LINES];

    grid_init(grid, '*');
    plot_vline(grid, 1, 1.2, 3.8, '|');
    grid_display(grid);

    return 0;
}
