#ifndef SRC_PARSER_H
#define SRC_PARSER_H

#include <stdio.h>


/**
 * @brief хранит кординаты вершин точек в одномерном массиве в формате x, y, z,
 * ... xn, yn, xn.
 *
 */
typedef struct Vretex {
  unsigned int count;
  double *arg;
} Vertexes;

/**
 * @brief хранит попарно номера вершин для построения линии фасетов в одноменом
 * массиве в формате v1, v2, v,2, v4, v4, v5, v5, vn.
 *
 */
typedef struct Facet {
  unsigned int count;
  unsigned int *arg;
} Facets;

/**
 * @brief функция для парсинга .obj файлов.
 *
 * @param filePath путь и имя файла
 * @param vertexes структура с массивом вершин
 * @param facets структура с массивом фасетов
 * @return int код ошибки
 */
int parser(char *filePath, Vertexes *vertexes, Facets *facets);

/**
 * @brief функция для парсинга .obj файлов.
 *
 * @param f принимает поток файла
 * @param vertexes структура с массивом вершин
 * @param facets структура с массивом фасетов
 * @return int код ошибки
 */
int pre_parser(FILE *f, Vertexes *vertexes, Facets *facets);

/**
 * @brief считает количество пробелов в строке
 *
 * @param count прибавляет к этому счетику количество пробелов
 * @param str строка поиска
 * @return int код ошибки
 */
int count_number_in_string(int *count, char *str);

int getDataVetrtexAndFacet(FILE *f, Vertexes *vertexes, Facets *facets);

void print_vertex(Vertexes vertexes);

void print_facets(Facets facets);

/**
 * @brief
 *
 */
typedef struct Move {
  double dx;
  double dy;
  double dz;
} Move;

/**
 * @brief
 *
 */
typedef struct Scale {
  double dx;
  double dy;
  double dz;
} Scale;

/**
 * @brief
 *
 */
typedef struct Rotate {
  double dx;
  double dy;
  double dz;
} Rotate;

int moveObj(Vertexes *vertex, Move move);

int scaleObj(Vertexes *vertex, double scale);

int rotationObj(Vertexes *vertex, Rotate rotate);

double grad_to_rad(double grad);

int rotateAroundAnAxis(Vertexes *vertex, char c, double angle);

#endif  // SRC_PARSER_H
