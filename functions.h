//
// Created by caiotavares on 25/04/23.
//
#include "dataStructure.h"
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <sstream> // stringstream
#include <cmath>

using namespace std;

#ifndef CONTEUDO_03_FUNCTIONS_H
#define CONTEUDO_03_FUNCTIONS_H

void write_pgm(int **array, int numrows, int numcols, char *filename);
void write_pgm_matrix(int **array, int numrows, int numcols, char *filename);
Image read_image(char *filename);
Image darken(Image image, int p);
Image negative(Image image);
Image whiten(Image image, int p);
Image turn_plus_90(Image image);
Image turn_minus_90(Image image);
Image turn_plus_180(Image image);
Image binarizing(Image image, int k);
Image horizontal_mirror_right(Image image);
Image variables_binarize_4_factors(Image image, int a, int b, int s_sup, int s_inf);
Image variables_binarize_3_factors(Image image, int a, int b, int s_sup);
Image subtraction(Image image, float c, float a);
float proportion_to_0_1(int pixel);
int proportion_to_0_255(float pixel);
Image resolution_x4(Image image);
Image poor_quality(Image image);
Image histogram_equalization(Image image);
Image local_histogram_equalization(Image image);
Image blurring(Image image);
Image laplace2(Image image);
Image laplace(Image image);
Image median_filter(Image image);
int findMedian(int pixel[3][3]);
Image highBoost_filter(Image image, double boostFactor);

// High-Boost filter
Image highBoost_filter(Image image, double boostFactor) {
    int filterSize = 3;
    Image filtered_image;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.array = (int **)malloc(filtered_image.numrows * sizeof(int *));
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (int *)malloc(filtered_image.numcols * sizeof(int));
    }

    // Cria uma imagem borrada
    Image blurred_image = blurring(image);

    // Cria uma máscara subtraindo a imagem borrada da original
    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            int mask_value = image.array[row][col] - blurred_image.array[row][col];
            int filtered_value = (int)(image.array[row][col] + boostFactor * mask_value);
            filtered_image.array[row][col] = clamp(filtered_value, 0, 255);
        }
    }

    return filtered_image;
}


int findMedian(int pixel[3][3]) {
    int values[9];
    int k = 0;

    // Extrai os valores da matriz 3x3 e os armazena em um array
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            values[k++] = pixel[i][j];
        }
    }

    // Ordena os valores em ordem crescente
    for (int i = 0; i < 9 - 1; i++) {
        for (int j = 0; j < 9 - i - 1; j++) {
            if (values[j] > values[j + 1]) {
                int temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
            }
        }
    }

    // Retorna o valor mediano, que está no meio do array ordenado
    return values[4];
}

// Filtro da mediana
Image median_filter(Image image) {
    int filterSize = 3;
    Image filtered_image;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.array = (int **)malloc(filtered_image.numrows * sizeof(int *));
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (int *)malloc(filtered_image.numcols * sizeof(int));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {

            int pixel[3][3] = {0};
            for (int i = 0; i < filterSize; i++) {
                for (int j = 0; j < filterSize; j++) {
                    int neighbor_row = row - filterSize/2 + i;
                    int neighbor_col = col - filterSize/2 + j;
                    if (neighbor_col >= 0 && neighbor_col < image.numcols &&
                        neighbor_row >= 0 && neighbor_row < image.numrows) {
                        pixel[i][j] = image.array[neighbor_row][neighbor_col];
                    }
                }
            }

            filtered_image.array[row][col] = findMedian(pixel);
        }
    }

    return filtered_image;
}

// Filtro Laplaciano de máscara diagonal
Image laplace(Image image) {
    int filterSize = 3;
    int laplacianFilter[3][3] = {
            {0, -1, 0},
            {-1,  4, -1},
            {0, -1, 0}
    };

    Image filtered_image;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.array = (int **)malloc(filtered_image.numrows * sizeof(int *));
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (int *)malloc(filtered_image.numcols * sizeof(int));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            int sum = 0;
            for (int i = 0; i < filterSize; i++) {
                for (int j = 0; j < filterSize; j++) {
                    int neighbor_row = row - filterSize/2 + i;
                    int neighbor_col = col - filterSize/2 + j;
                    if (neighbor_row >= 0 && neighbor_row < image.numrows &&
                        neighbor_col >= 0 && neighbor_col < image.numcols) {
                        sum += image.array[neighbor_row][neighbor_col] * laplacianFilter[i][j];
                    }
                }
            }
            filtered_image.array[row][col] = clamp(sum, 0, 255);
        }
    }

    return filtered_image;
}

// Filtro Laplaciano de máscara de vizinhança-8
Image laplace2(Image image) {
    int filterSize = 3;
    int laplacianFilter[3][3] = {
            {-1, -1, -1},
            {-1,  8, -1},
            {-1, -1, -1}
    };

    Image filtered_image;
    filtered_image.numrows = image.numrows;
    filtered_image.numcols = image.numcols;
    filtered_image.array = (int **)malloc(filtered_image.numrows * sizeof(int *));
    for (int row = 0; row < filtered_image.numrows; ++row) {
        filtered_image.array[row] = (int *)malloc(filtered_image.numcols * sizeof(int));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            int sum = 0;
            for (int i = 0; i < filterSize; i++) {
                for (int j = 0; j < filterSize; j++) {
                    int neighbor_row = row - filterSize/2 + i;
                    int neighbor_col = col - filterSize/2 + j;
                    if (neighbor_row >= 0 && neighbor_row < image.numrows &&
                        neighbor_col >= 0 && neighbor_col < image.numcols) {
                        sum += image.array[neighbor_row][neighbor_col] * laplacianFilter[i][j];
                    }
                }
            }
            filtered_image.array[row][col] = clamp(sum, 0, 255);
        }
    }

    return filtered_image;
}

// Efeiro de bluring (borramento)
Image blurring(Image image) {
    int filterSize = 9;
    Image blurred_image;
    blurred_image.numrows = image.numrows;
    blurred_image.numcols = image.numcols;
    blurred_image.array = (int **)malloc(blurred_image.numrows * sizeof(int *));
    for (int row = 0; row < blurred_image.numrows; ++row) {
        blurred_image.array[row] = (int *)malloc(blurred_image.numcols * sizeof(int));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            int sum = 0;
            int count = 0;
            for (int i = -filterSize/2; i <= filterSize/2; i++) {
                for (int j = -filterSize/2; j <= filterSize/2; j++) {
                    int neighbor_row = row + i;
                    int neighbor_col = col + j;
                    if (neighbor_row >= 0 && neighbor_row < image.numrows &&
                        neighbor_col >= 0 && neighbor_col < image.numcols) {
                        sum += image.array[neighbor_row][neighbor_col];
                        count++;
                    }
                }
            }
            blurred_image.array[row][col] = sum / count;
        }
    }

    return blurred_image;
}

// Equalização local de histograma
Image local_histogram_equalization(Image image) {
    Image equalized_image;
    equalized_image.numrows = image.numrows;
    equalized_image.numcols = image.numcols;
    equalized_image.array = (int **) malloc(equalized_image.numrows * sizeof(int *));
    for (int row = 0; row < equalized_image.numrows; ++row) {
        equalized_image.array[row] = (int *) malloc(equalized_image.numcols * sizeof(int));
    }

    int hist[256] = {0};
    int cdf[256] = {0};
    float alpha = 255.0 / 9.0;  // Valor de alpha para o espaço 3x3

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            // Zerar o histograma
            for (int i = 0; i < 256; i++) {
                hist[i] = 0;
            }

            // Calcular o histograma para o espaço 3x3
            for (int i = row - 1; i <= row + 1; i++) {
                for (int j = col - 1; j <= col + 1; j++) {
                    if (i >= 0 && i < image.numrows && j >= 0 && j < image.numcols) {
                        hist[image.array[i][j]]++;
                    }
                }
            }

            // Calcular a função de distribuição cumulativa
            int sum = 0;
            for (int i = 0; i < 256; i++) {
                sum += hist[i];
                cdf[i] = round(alpha * sum);
            }

            // Aplicar a equalização para o pixel atual
            equalized_image.array[row][col] = cdf[image.array[row][col]];
        }
    }

    return equalized_image;
}


// Equalização global
Image histogram_equalization(Image image) {
    Image down_image;
    down_image.numrows = image.numrows;
    down_image.numcols = image.numcols;
    down_image.array = (int **) malloc(down_image.numrows * sizeof(int *));
    for (int row = 0; row < down_image.numrows; ++row) {
        down_image.array[row] = (int *) malloc(down_image.numcols * sizeof(int));
    }

    int hist[256] = {0};
    int cdf[256] = {0};
    int pixels = image.numrows * image.numcols;
    float alpha = 255.0 / pixels;

    // Calculate the histogram
    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            hist[image.array[row][col]]++;
        }
    }

    // Calculate the cumulative distribution function
    int sum = 0;
    for (int i = 0; i < 256; i++) {
        sum += hist[i];
        cdf[i] = round(alpha * sum);
    }

    // Apply the equalization to each pixel
    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            down_image.array[row][col] = cdf[image.array[row][col]];
        }
    }

    return down_image;
}

Image poor_quality(Image image) {
    int factor = 2;
    int new_rows = image.numrows / factor;
    int new_cols = image.numcols / factor;

    Image down_image;
    down_image.numrows = new_rows;
    down_image.numcols = new_cols;
    down_image.array = (int **) malloc(down_image.numrows * sizeof(int *));
    for (int row = 0; row < down_image.numrows; ++row) {
        down_image.array[row] = (int *) malloc(down_image.numcols * sizeof(int));
    }

    for (int row = 0; row < new_rows; ++row) {
        for (int col = 0; col < new_cols; ++col) {
            int sum = 0;
            for (int i = 0; i < factor; ++i) {
                for (int j = 0; j < factor; ++j) {
                    sum += image.array[row * factor + i][col * factor + j];
                }
            }
            down_image.array[row][col] = sum / (factor * factor);
        }
    }

    return down_image;
}

Image resolution_x4(Image image) {
    Image aux_image;
    aux_image.numrows = image.numrows * 2;
    aux_image.numcols = image.numcols * 2;
    aux_image.array = (int **) malloc(aux_image.numrows * sizeof(int *));
    for (int row = 0; row < aux_image.numrows; ++row) {
        aux_image.array[row] = (int *) malloc(aux_image.numcols * sizeof(int));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            int pixel = image.array[row][col];

            // Preenche os quatro pixels na imagem auxiliar
            aux_image.array[2*row][2*col] = pixel;
            aux_image.array[2*row+1][2*col] = pixel;
            aux_image.array[2*row][2*col+1] = pixel;
            aux_image.array[2*row+1][2*col+1] = pixel;
        }
    }
    return aux_image;
}

Image subtraction(Image image, float c, float a) {
    Image sup_image;
    sup_image.numrows = image.numrows;
    sup_image.numcols = image.numcols;
    sup_image.array = (int **) malloc(sup_image.numrows * sizeof(int *));
    for (int row = 0; row < sup_image.numrows; ++row) {
        sup_image.array[row] = (int *) malloc(sup_image.numrows * sizeof(int));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            float aux = proportion_to_0_1(image.array[row][col]);
//            std::cout << aux << " aux:  \n";
            float pixel = c * pow(aux, a);
//            std::cout << pixel << " pixel depois do calculo\n";
            pixel = proportion_to_0_255(pixel);
//            std::cout << pixel << " pixel passado para 255\n";
            sup_image.array[row][col] = pixel;
        }
    }
    return sup_image;
}

float proportion_to_0_1(int pixel) {
    return pixel / 255.0;
}

int proportion_to_0_255(float pixel) {
    return (pixel * 255);
}

Image variables_binarize_3_factors(Image image, int a, int b, int s_sup) {
    //Alocate space for the sup image
    Image sup_image;
    sup_image.numrows = image.numrows;
    sup_image.numcols = image.numcols;
    sup_image.array = (int **)malloc(sup_image.numrows * sizeof(int *));
    for (int row = 0; row < sup_image.numrows; row++) {
        sup_image.array[row] = (int *) malloc(sup_image.numrows * sizeof(int));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            if(image.array[row][col] > a && image.array[row][col] < b) {
                sup_image.array[row][col] = s_sup;
            } else {
                sup_image.array[row][col] = image.array[row][col];
            }
        }
    }
    return sup_image;
}

Image variables_binarize_4_factors(Image image, int a, int b, int s_sup, int s_inf) {
    //Alocate space for the sup image
    Image sup_image;
    sup_image.numrows = image.numrows;
    sup_image.numcols = image.numcols;
    sup_image.array = (int **)malloc(sup_image.numrows * sizeof(int *));
    for (int row = 0; row < sup_image.numrows; row++) {
        sup_image.array[row] = (int *) malloc(sup_image.numrows * sizeof(int));
    }

    for (int row = 0; row < image.numrows; row++) {
        for (int col = 0; col < image.numcols; col++) {
            if(image.array[row][col] > a && image.array[row][col] < b) {
                sup_image.array[row][col] = s_sup;
            } else {
                sup_image.array[row][col] = s_inf;
            }
        }
    }
    return sup_image;
}

Image horizontal_mirror_right(Image image) {
    Image sup_image;
    sup_image.numrows = image.numcols; // Swap the rows and columns for the new image
    sup_image.numcols = image.numrows;

    // Allocate memory for the new image
    sup_image.array = (int**) malloc(sup_image.numrows * sizeof(int*));
    for (int row = 0; row < sup_image.numrows; ++row) {
        sup_image.array[row] = (int*) malloc(sup_image.numcols * sizeof(int));
    }

    // Populate the new image
    for (int row = 0; row < sup_image.numrows; ++row) {
        for (int col = 0; col < sup_image.numcols; ++col) {
            sup_image.array[col][row] = image.array[col*image.numrows][row];
        }
    }

    return sup_image;
}

void write_pgm_matrix(int **array, int numrows, int numcols, char *filename) {
    unsigned char byte;
    FILE *arq = fopen(filename, "wb");

    fprintf(arq, "P2\n");
    fprintf(arq, "# CREATOR: GIMP PNM Filter Version 1.1\n");
    fprintf(arq, "%d ", numcols);
    fprintf(arq, "%d\n", numrows);

    for (int row = 0; row < numrows; row++) {
        for (int col = 0; col < numcols; col++) {
            if (row == 0 && col == 0) {
                fprintf(arq, "255\n");
            } else {
                int pixel = array[row][col];
                fprintf(arq, "%d ", pixel);
            }
        }
        fprintf(arq, "\n");
    }
    fclose(arq);
}

Image binarizing(Image image, int k) {
    for (int row = 0; row != image.numrows; row++) {
        for (int col = 0; col != image.numcols; col++) {
            if (image.array[row][col] <= k) image.array[row][col] = 0;
            else image.array[row][col] = 255;
        }
    }

    return image;
}

void write_pgm(int **array, int numrows, int numcols, char *filename) {
    unsigned char byte;
    FILE *arq = fopen(filename, "wb");

    fprintf(arq, "P2\n");
    fprintf(arq, "# CREATOR: GIMP PNM Filter Version 1.1\n");
    fprintf(arq, "%d ", numcols);
    fprintf(arq, "%d\n", numrows);

    for (int row = 0; row < numrows; row++) {
        for (int col = 0; col < numcols; col++) {
            int pixel = array[row][col];
            if (row == 0 && col == 0) {
                fprintf(arq, "255\n");
            } else {
                fprintf(arq, "%d", pixel);
                fprintf(arq, "\n");
            }
        }
    }
    fclose(arq);
}

Image read_image(char *filename) {
    int row = 0, col = 0, numrows = 0, numcols = 0, grey_scale = 0;
    ifstream infile(filename);
    stringstream ss;
    string inputLine = "";

    // First line : version
    getline(infile,inputLine);
    if(inputLine.compare("P2") != 0) cerr << "Version error" << endl;
    else cout << "Version : " << inputLine << endl;

    // Second line : comment
    getline(infile,inputLine);
    cout << "Comment : " << inputLine << endl;

    // Continue with a stringstream
    ss << infile.rdbuf();
    // Third line : size
    ss >> numcols >> numrows;
    cout << numcols << " columns and " << numrows << " rows" << endl;

//    int array[numrows][numcols];

    int **array = new int*[numrows];
    // Following lines : data
    for(row = 0; row < numrows; ++row) {
        array[row] = new int[numcols];
        for (col = 0; col < numcols; ++col) ss >> array[row][col];
    }


    Image image;
    image.array = array;
    image.numrows = numrows;
    image.numcols = numcols;

    return image;
}

Image darken(Image image, int p) {

    // darken the image
    for(int row = 0; row < image.numrows; ++row) {
        for(int col = 0; col < image.numcols; ++col) {
            image.array[row][col] = image.array[row][col] - p;
            if (image.array[row][col] < 0) image.array[row][col] = 0;
        }
    }
    return image;
}

Image negative(Image image) {

    // darken the image
    for(int row = 0; row < image.numrows; ++row) {
        for(int col = 0; col < image.numcols; ++col) {
            image.array[row][col] = 255 - image.array[row][col];
        }
    }
    image.array[0][0] = 255;
    return image;
}

Image whiten(Image image, int p) {

    // darken the image
    for(int row = 0; row < image.numrows; ++row) {
        for(int col = 0; col < image.numcols; ++col) {
            image.array[row][col] = image.array[row][col] + p;
            if (image.array[row][col] > 255) image.array[row][col] = 255;
        }
    }
    return image;
}

Image turn_plus_90(Image image) {
    Image sup_image;
    sup_image.numrows = image.numcols; // Swap the rows and columns for the new image
    sup_image.numcols = image.numrows;

    // Allocate memory for the new image
    sup_image.array = (int**) malloc(sup_image.numrows * sizeof(int*));
    for (int row = 0; row < sup_image.numrows; ++row) {
        sup_image.array[row] = (int*) malloc(sup_image.numcols * sizeof(int));
    }

    // Populate the new image
    for (int row = 0; row < sup_image.numcols; ++row) {
        for (int col = 0; col < sup_image.numrows; ++col) {
            sup_image.array[col][row] = image.array[image.numrows-1-row][col];
        }
    }

    return sup_image;
}

Image turn_minus_90(Image image) {
    Image sup_image;
    sup_image.numrows = image.numcols; // Swap the rows and columns for the new image
    sup_image.numcols = image.numrows;

    // Allocate memory for the new image
    sup_image.array = (int**) malloc(sup_image.numrows * sizeof(int*));
    for (int row = 0; row < sup_image.numrows; ++row) {
        sup_image.array[row] = (int*) malloc(sup_image.numcols * sizeof(int));
    }

    // Populate the new image
    for (int row = 0; row < sup_image.numcols; ++row) {
        for (int col = 0; col < sup_image.numrows; ++col) {
            sup_image.array[col][row] = image.array[row][image.numcols-1-col];
        }
    }

    return sup_image;
}

Image turn_plus_180(Image image) {
    Image sup_image;
    sup_image.numrows = image.numrows;
    sup_image.numcols = image.numcols;

    // Allocate memory for the new image
    sup_image.array = (int **) malloc(sup_image.numrows * sizeof(int *));
    for (int row = 0; row < sup_image.numrows; ++row) {
        sup_image.array[row] = (int *) malloc(sup_image.numcols * sizeof(int));
    }

    // Populate the new image
    for (int row = 0; row < sup_image.numrows; ++row) {
        for (int col = 0; col < sup_image.numcols; ++col) {
            sup_image.array[row][col] = image.array[image.numrows - 1 - row][image.numcols - 1 - col];
        }
    }

    return sup_image;
}

#endif //CONTEUDO_03_FUNCTIONS_H
