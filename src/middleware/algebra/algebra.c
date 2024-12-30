#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//========================MATRICEX========================
Matrix create_matrix(unsigned int rows, unsigned int cols) {
    Matrix mat;
    mat.n = rows;
    mat.m = cols;
    mat.matrix = (float*)malloc(rows * cols * sizeof(float));
    return mat;
}

// Function to set a value in the matrix
void set_matrix_element(Matrix* mat, unsigned int row, unsigned int col, float value) {
    if (row < mat->n && col < mat->m) {
        mat->matrix[row * mat->m + col] = value;
    }
}

// Function to get a value from the matrix
float get_matrix_element(Matrix* mat, unsigned int row, unsigned int col) {
    if (row < mat->n && col < mat->m) {
        return mat->matrix[row * mat->m + col];
    }
    return 0.0f; // Return 0 if out of bounds
}

// Function to print the matrix
void print_matrix(Matrix* mat) {
    for (unsigned int i = 0; i < mat->n; i++) {
        for (unsigned int j = 0; j < mat->m; j++) {
            printf("%f ", get_matrix_element(mat, i, j));
        }
        printf("\n");
    }
}

// Function to add two matrices
Matrix add_matrices(Matrix* mat1, Matrix* mat2) {
    if (mat1->n != mat2->n || mat1->m != mat2->m) {
        fprintf(stderr, "Matrices dimensions do not match for addition.\n");
        exit(EXIT_FAILURE);
    }
    Matrix result = create_matrix(mat1->n, mat1->m);
    for (unsigned int i = 0; i < mat1->n; i++) {
        for (unsigned int j = 0; j < mat1->m; j++) {
            float sum = get_matrix_element(mat1, i, j) + get_matrix_element(mat2, i, j);
            set_matrix_element(&result, i, j, sum);
        }
    }
    return result;
}

// Function to multiply two matrices
Matrix multiply_matrices(Matrix* mat1, Matrix* mat2) {
    if (mat1->m != mat2->n) {
        fprintf(stderr, "Matrices dimensions do not match for multiplication.\n");
        exit(EXIT_FAILURE);
    }
    Matrix result = create_matrix(mat1->n, mat2->m);
    for (unsigned int i = 0; i < mat1->n; i++) {
        for (unsigned int j = 0; j < mat2->m; j++) {
            float sum = 0.0f;
            for (unsigned int k = 0; k < mat1->m; k++) {
                sum += get_matrix_element(mat1, i, k) * get_matrix_element(mat2, k, j);
            }
            set_matrix_element(&result, i, j, sum);
        }
    }
    return result;
}

// Function to transpose a matrix
Matrix transpose_matrix(Matrix* mat) {
    Matrix result = create_matrix(mat->m, mat->n);
    for (unsigned int i = 0; i < mat->n; i++) {
        for (unsigned int j = 0; j < mat->m; j++) {
            set_matrix_element(&result, j, i, get_matrix_element(mat, i, j));
        }
    }
    return result;
}

// Function to delete a matrix
void delete_matrix(Matrix* mat) {
    free(mat->matrix);
    mat->matrix = NULL;
    mat->n = 0;
    mat->m = 0;
}


//========================VECTORS========================
Vector create_vector(unsigned int size) {
    Vector vec;
    vec.n = size;
    vec.vector = (float*)malloc(size * sizeof(float));
    return vec;
}

void set_vector_element(Vector* vec, unsigned int index, float value) {
    if (index < vec->n) {
        vec->vector[index] = value;
    }
}

float get_vector_element(Vector* vec, unsigned int index) {
    if (index < vec->n) {
        return vec->vector[index];
    }
    return 0.0f; // Return 0 if out of bounds
}

void print_vector(Vector* vec) {
    for (unsigned int i = 0; i < vec->n; i++) {
        printf("%f ", get_vector_element(vec, i));
    }
    printf("\n");
}

Vector add_vectors(Vector* vec1, Vector* vec2) {
    if (vec1->n != vec2->n) {
        fprintf(stderr, "Vectors dimensions do not match for addition.\n");
        exit(EXIT_FAILURE);
    }
    Vector result = create_vector(vec1->n);
    for (unsigned int i = 0; i < vec1->n; i++) {
        float sum = get_vector_element(vec1, i) + get_vector_element(vec2, i);
        set_vector_element(&result, i, sum);
    }
    return result;
}

float dot_product_vectors(Vector* vec1, Vector* vec2) {
    if (vec1->n != vec2->n) {
        fprintf(stderr, "Vectors dimensions do not match for multiplication.\n");
        exit(EXIT_FAILURE);
    }
    float dotProduct = 0.0f;
    for (unsigned int i = 0; i < vec1->n; i++) {
        dotProduct += get_vector_element(vec1, i) * get_vector_element(vec2, i);
    }
    return dotProduct;
}

Vector cross_product_vectors(Vector* vec1, Vector* vec2) {
    if (vec1->n != 3 || vec2->n != 3) {
        fprintf(stderr, "Cross product is only defined for 3D vectors.\n");
        exit(EXIT_FAILURE);
    }
    Vector result = create_vector(3);
    set_vector_element(&result, 0, get_vector_element(vec1, 1) * get_vector_element(vec2, 2) - get_vector_element(vec1, 2) * get_vector_element(vec2, 1));
    set_vector_element(&result, 1, get_vector_element(vec1, 2) * get_vector_element(vec2, 0) - get_vector_element(vec1, 0) * get_vector_element(vec2, 2));
    set_vector_element(&result, 2, get_vector_element(vec1, 0) * get_vector_element(vec2, 1) - get_vector_element(vec1, 1) * get_vector_element(vec2, 0));
    return result;
}

Vector normalize_vector(Vector* vec) {
    float magnitude = 0.0f;
    for (unsigned int i = 0; i < vec->n; i++) {
        magnitude += get_vector_element(vec, i) * get_vector_element(vec, i);
    }
    magnitude = sqrt(magnitude);

    if (magnitude == 0.0f) {
        fprintf(stderr, "Cannot normalize a zero vector.\n");
        exit(EXIT_FAILURE);
    }

    Vector result = create_vector(vec->n);
    for (unsigned int i = 0; i < vec->n; i++) {
        set_vector_element(&result, i, get_vector_element(vec, i) / magnitude);
    }
    return result;
}

void delete_vector(Vector* vec) {
    free(vec->vector);
    vec->vector = NULL;
    vec->n = 0;
}


//========================COMMON========================
Vector multiply_matrix_vector(Matrix* mat, Vector* vec) {
    if (mat->m != vec->n) {
        fprintf(stderr, "Matrix columns must match vector size for multiplication.\n");
        exit(EXIT_FAILURE);
    }
    Vector result = create_vector(mat->n);
    for (unsigned int i = 0; i < mat->n; i++) {
        float sum = 0.0f;
        for (unsigned int j = 0; j < mat->m; j++) {
            sum += get_matrix_element(mat, i, j) * get_vector_element(vec, j);
        }
        set_vector_element(&result, i, sum);
    }
    return result;
}
