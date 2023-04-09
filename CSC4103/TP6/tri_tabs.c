#include <stdlib.h>
#include <stdio.h>

#define NB_ELEMENTS 7

// fonction compar_short
int compar_short(const void* a, const void* b) {
    short* i = (short*) a;
    short* u = (short*) b;

    if (*i < *u) {
        return -1;
    }
    if (*i == *u) {
        return 0;
    }
    else {
        return 1;
    }
}


// fonction compar_double
int compar_double(const void* a, const void* b) {
    double* i = (double*) a;
    double* u = (double*) b;

    if (*i < *u) {
        return -1;
    }
    if (*i == *u) {
        return 0;
    }
    else {
        return 1;
    }
}

void print_tab_short(short tab[], size_t nmemb) {
  int i;
  for (i = 0 ; i < nmemb ; i++) {
    printf("tab_short[%d] = %d\n", i, tab[i]);
  }
}

void print_tab_double(double tab[], size_t nmemb) {
  int i;
  for (i = 0 ; i < nmemb ; i++) {
    printf("tab_double[%d] = %lf\n", i, tab[i]);
  }
}

int main(){
  short tab_short[NB_ELEMENTS] = {6, 5, 4, 3, 2, 1, 0};
  double tab_double[NB_ELEMENTS] = {7.1, 6.2, 5.3, 4.4, 3.5, 2.6, 1.7};

  printf("tab_short avant tri\n");
  print_tab_short(tab_short, NB_ELEMENTS);
  // TODO : Appelez la fonction qsort
  qsort(tab_short, NB_ELEMENTS, sizeof(short), compar_short);
  printf("tab_short avant tri\n");
  print_tab_short(tab_short, NB_ELEMENTS);

  printf("tab_double avant tri\n");
  print_tab_double(tab_double, NB_ELEMENTS);
  // TODO : Appelez la fonction qsort
    qsort(tab_double, NB_ELEMENTS, sizeof(double), compar_double);
  printf("tab_double avant tri\n");
  print_tab_double(tab_double, NB_ELEMENTS);

  return EXIT_SUCCESS;
}
