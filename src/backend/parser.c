#include "parser.h"

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int parser(char *filePath, Vertexes *vertexes, Facets *facets) {
  int error = 0;
  //************************
  FILE *f = fopen(filePath, "r");

  if (!f) error = 1;

  if (!error) error = pre_parser(f, vertexes, facets);

  if (!error) error = getDataVetrtexAndFacet(f, vertexes, facets);

  if (f) fclose(f);

  return error;
}

int pre_parser(FILE *f, Vertexes *vertexes, Facets *facets) {
  int error = 0;
  //************************
  char *temp_string = NULL;
  size_t len = 0;
  ssize_t lineSize = 0;
  int space_count = 0;
  //************************

  // первый проход с подсчетом строк v f
  while ((int)(lineSize = getline(&temp_string, &len, f)) != EOF) {
    if (temp_string[0] == 'v' && temp_string[1] == ' ') {
      vertexes->count++;
    }

    if (temp_string[0] == 'f' && temp_string[1] == ' ') {
      count_number_in_string(&space_count, temp_string);
    }
  }

  vertexes->count = vertexes->count * 3 + 3;
  facets->count = space_count * 2;

  free(temp_string);

  return error;
}

int count_number_in_string(int *count, char *str) {
  int error = 0;
  int temp_count = 0;
  char *tmp = str;
  while (*tmp) {
    // printf("%c", *tmp);
    if (*tmp == ' ' && isdigit(*(tmp + 1))) {
      temp_count++;
    }
    tmp++;
  }
  *count += temp_count;
  return error;
}

int getDataVetrtexAndFacet(FILE *f, Vertexes *vertexes, Facets *facets) {
  int error = 0;

  rewind(f);

  vertexes->arg = (double *)malloc(vertexes->count * sizeof(double));
  facets->arg = (unsigned int *)malloc(facets->count * sizeof(unsigned int));

  if (!vertexes->arg || !facets->arg) error = 2;

  // printf("vertexes->arg_v %p\n", vertexes->arg);
  // printf("vertexes->arg_v %p\n", facets->arg);

  if (!error) {
    vertexes->arg[0] = 0;
    vertexes->arg[1] = 0;
    vertexes->arg[2] = 0;
    facets->arg[0] = 0;

    char *temp_string = NULL;
    size_t len = 0;
    ssize_t lineSize = 0;
    char seps[] = " ";
    char *token = NULL;
    unsigned int arg_v_index = 3;
    unsigned int arg_f_index = 0;
    char temp[32];
    int countTokes = 0;
    int numberOfTokens = 0;

    // vertex
    while ((int)(lineSize = getline(&temp_string, &len, f)) != EOF) {
      if (temp_string[0] == 'v' && temp_string[1] == ' ') {
        token = strtok(temp_string, seps);

        while (token != NULL) {
          token = strtok(NULL, seps);
          if (token == NULL || *token == '\n') break;
          vertexes->arg[arg_v_index] = atof(token);
          arg_v_index++;
        }
      }

      // facets
      if (temp_string[0] == 'f' && temp_string[1] == ' ') {
        numberOfTokens = 1;
        countTokes = 0;
        count_number_in_string(&numberOfTokens, temp_string);
        token = strtok(temp_string, seps);
        countTokes++;

        while (token != NULL) {
          token = strtok(NULL, seps);
          countTokes++;
          if (countTokes == 2) {
            memset(temp, 0, 32);
            strcpy(temp, token);
          }
          if (token == NULL || *token == '\n') break;
          if (countTokes == 2) {
            facets->arg[arg_f_index] = atoi(token);
            arg_f_index++;
          } else {
            facets->arg[arg_f_index] = atoi(token);
            arg_f_index++;
            facets->arg[arg_f_index] = atoi(token);
            arg_f_index++;
          }
          if (numberOfTokens == countTokes) {
            facets->arg[arg_f_index] = atoi(temp);
            arg_f_index++;
          }
        }
      }
    }
  }

  return error;
}

int moveObj(Vertexes *vertex, Move move) {
  int error = 0;

  for (unsigned int i = 3; i < vertex->count; i += 3) {
    vertex->arg[i] += move.dx;
    vertex->arg[i + 1] += move.dy;
    vertex->arg[i + 2] += move.dz;
  }

  return error;
}

int rotationObj(Vertexes *vertex, Rotate rotate) {
  int error = 0;

  if (rotate.dx) {
    error = rotateAroundAnAxis(vertex, 'x', rotate.dx);
  }

  if (rotate.dy) {
    error = rotateAroundAnAxis(vertex, 'y', rotate.dy);
  }

  if (rotate.dz) {
    error = rotateAroundAnAxis(vertex, 'z', rotate.dz);
  }

  return error;
}

int scaleObj(Vertexes *vertex, double scale) {
  int error = 0;

  for (unsigned int i = 3; i < vertex->count; i += 3) {
    vertex->arg[i] *= scale;
    vertex->arg[i + 1] *= scale;
    vertex->arg[i + 2] *= scale;
  }

  return error;
}

double grad_to_rad(double grad) { return grad * M_PI / 180; }

int rotateAroundAnAxis(Vertexes *vertex, char c, double angle) {
  int error = 0;

  int a = 0;
  int b = 0;

  switch (c) {
    case 'x':
      a = 1;
      b = 2;
      break;

    case 'y':
      a = 0;
      b = 2;
      break;

    case 'z':
      a = 0;
      b = 1;
      break;

    default:
      error = 3;
  }

  if (!error) {
    angle = grad_to_rad(angle);
    for (unsigned int i = 3; i < vertex->count; i += 3) {
      double a1 = vertex->arg[i + a];
      double a2 = vertex->arg[i + b];
      vertex->arg[i + a] = cos(angle) * a1 - sin(angle) * a2;
      vertex->arg[i + b] = sin(angle) * a1 + cos(angle) * a2;
    }
  }

  return error;
}
