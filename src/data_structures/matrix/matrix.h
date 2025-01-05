#pragma once

typedef struct {
	float* matrix;
	unsigned int n, m;
} Matrix;

typedef struct {
	float* vector;
	unsigned int n;
} Vector;


Matrix create_matrix(unsigned int rows, unsigned int cols);
void set_matrix_element(Matrix* mat, unsigned int row, unsigned int col, float value);
float get_matrix_element(Matrix* mat, unsigned int row, unsigned int col);
void print_matrix(Matrix* mat);

Matrix add_matrices(Matrix* mat1, Matrix* mat2);
Matrix multiply_matrices(Matrix* mat1, Matrix* mat2);
Matrix transpose_matrix(Matrix* mat);
void delete_matrix(Matrix* mat);


Vector create_vector(unsigned int size);
void set_vector_element(Vector* vec, unsigned int index, float value);
float get_vector_element(Vector* vec, unsigned int index);
void print_vector(Vector* vec);

Vector add_vectors(Vector* vec1, Vector* vec2);
float dot_product_vectors(Vector* vec1, Vector* vec2);
Vector cross_product_vectors(Vector* vec1, Vector* vec2);
Vector normalize_vector(Vector* vec);
void delete_vector(Vector* vec);


Vector multiply_matrix_vector(Matrix* mat, Vector* vec);