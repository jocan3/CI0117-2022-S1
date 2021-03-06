// Copyright 2022 Jose Andres Mena <jose.menaarias@ucr.ac.cr>

#include "reader.h"

void** create_matrix_value(size_t row_count,
            size_t col_count, size_t element_size);
void free_matrix_value(const size_t row_count, void** matrix);

matrix_t* read_matrix(FILE* fptr) {
    matrix_t* matrix = malloc(sizeof(matrix_t));

    if (fscanf(fptr, "%i", &matrix->num_rows) == 0 || matrix->num_rows <= 0) {
        free(matrix);
        return NULL;
    }

    if (fscanf(fptr, "%i", &matrix->num_cols) == 0 || matrix->num_cols <= 0) {
        free(matrix);
        return NULL;
    }

    matrix->value = (char**)create_matrix_value(matrix->num_rows,
                        matrix->num_cols + 1, sizeof(char));

    if (!matrix->value) {
        free(matrix);
        return NULL;
    }

    for (int i = 0; i < matrix->num_rows; ++i) {
        fscanf(fptr, "%s", matrix->value[i]);
    }

    return matrix;
}

void destroy_matrix(matrix_t* matrix) {
    free_matrix_value(matrix->num_rows, (void**)matrix->value);
    free(matrix);
}

void** create_matrix_value(size_t row_count,
            size_t col_count, size_t element_size) {
  void** matrix = (void**) calloc(row_count, sizeof(void*));
  if ( matrix == NULL ) {
    return NULL;
  }

  for (size_t row = 0; row < row_count; ++row) {
    if ( (matrix[row] = calloc(col_count, element_size) ) == NULL ) {
      free_matrix_value(row_count, matrix);
      return NULL;
    }
  }

  return matrix;
}

void free_matrix_value(const size_t row_count, void** matrix) {
  if (matrix) {
    for (size_t row = 0; row < row_count; ++row) {
      free(matrix[row]);
    }
  }

  free(matrix);
}

