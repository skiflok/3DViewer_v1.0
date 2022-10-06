#include <stdio.h>
#include <string.h>

#include "parser.h"

void print_vertex(Vertexes vertexes) {
  for (size_t i = 3; i < vertexes.count; i++) {
    printf("%.2f ", vertexes.arg[i]);
    if ((i + 1) % 3 == 0) {
      printf("\n");
    }
  }
}

void print_facets(Facets facets) {
  for (size_t i = 0; i < facets.count; i++) {
    printf("%d ", facets.arg[i]);
    if ((i + 1) % 6 == 0) {
      printf("\n");
    }
  }
}
