#include <stdio.h>

#define MAXIMUM_M 100

// Main Functions

// Check if coordinates (x, y) are inside an n x n grid
int in_bounds(int x, int y, int n)
{
    // return true if (x, y) is inside the matrix of size NxN
    if (x < n && y < n) {
        return 0;
    } else {
        return 1;
    }
}

// Count alive neighbors
int count_alive_neighbors(int cells[MAXIMUM_M][MAXIMUM_M], int M, int i, int j) {
    int count = 0;
    int di[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dj[] = {-1,  0,  1,-1, 1,-1, 0, 1};
    for (int d = 0; d < 8; d++) {
        int ni = i + di[d];
        int nj = j + dj[d];
        // Verifica limites
        if (ni < 0 || ni >= M || nj < 0 || nj >= M) {
            continue; // pula vizinho fora da matriz
        }
        // Conta se for célula viva
        if (cells[ni][nj] == 1) {
            count++;
        }
    }
    return count;
}

// Compute updated temperatures
void compute_updated_temperatures(int M, int temp[][100], int temp_next[][100])
{
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            // store updated values in temp_next
            if (i == 0 || j == 0 || i == M-1 || j == M-1) { //verifica se está na borda e mantém a temperatura
                temp_next[i][j] = temp[i][j];
            } else {
                float mediaF = (temp[i-1][j] + temp[i+1][j] + temp[i][j-1] + temp[i][j+1])/4;
                int mediaI = (int)mediaF;
                temp_next[i][j] = mediaI;
            }
        }
    }
}

// Compute updated cells
void compute_updated_cells(int M, int cells[][100], int temp_next[][100], int cells_next[][100], int Tmin, int Tmax)
{
    // iterate and store updated values in cells_next
    int i = 0;
    int j = 0;
    for (i=0; i < M; i++){
        for (j = 0; j < M; j++){
            int vivos = count_alive_neighbors(cells, M, i, j); 
            if (cells[i][j] == 1 && vivos > 3){ //Superpopulaçao
                cells_next[i][j] = 0;
            } else if (cells[i][j] == 1 && vivos < 2){ //solidão
                cells_next[i][j] = 0;
            } else if (cells[i][j] == 1 && 2 <= vivos && vivos <= 3){ //sobrevivencia
                cells_next[i][j] = 1;
            } else if (cells[i][j] == 0 && vivos == 3){ //reprodução
                cells_next[i][j] = 1;
            } else {
                cells_next[i][j] = cells[i][j];
            }
            if (temp_next[i][j] < Tmin || temp_next[i][j] > Tmax) { //regra termica
                cells_next[i][j] = 0;
            }
        }   
    }
}

// Copy updated matrices for next step
void updated_matrix(int M, int origin[][100], int destiny[][100])
{
    // iterate and copy from origin to destiny
    int i = 0;
    int k = 0;
    for (i=0; i < M; i++){
        for (k=0; k < M; k++){
            origin[i][k] = destiny[i][k];
        }
    }
}


int main()
{
    int M, K, Tmin, Tmax;
    int temp[MAXIMUM_M][MAXIMUM_M];
    int temp_next[MAXIMUM_M][MAXIMUM_M];
    int cells[MAXIMUM_M][MAXIMUM_M];
    int cells_next[MAXIMUM_M][MAXIMUM_M];

    // --- 1. Ler os parâmetros ---
    scanf("%d", &M);     // número de linhas/colunas
    scanf("%d", &K);     // número de iterações
    scanf("%d", &Tmin);  // temperatura mínima
    scanf("%d", &Tmax);  // temperatura máxima

    // --- 2. Ler matriz de temperaturas ---
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            scanf("%d", &temp[i][j]);
        }
    }

    // --- 3. Ler matriz de células ---
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            scanf("%d", &cells[i][j]);
        }
    }

    // --- 4. Mostrar para conferir ---
    // printf("M = %d, K = %d, Tmin = %d, Tmax = %d\n", M, K, Tmin, Tmax);
    // printf("\nMatriz de temperaturas:\n");
    // for (int i = 0; i < M; i++) {
    //     for (int j = 0; j < M; j++) {
    //         printf("%d ", temp[i][j]);
    //     }
    //     printf("\n");
    // }
    // printf("\nMatriz de células:\n");
    // for (int i = 0; i < M; i++) {
    //     for (int j = 0; j < M; j++) {
    //         printf("%d ", cells[i][j]);
    //     }
    //     printf("\n");
    // }

    int i = 0;
    for (i = 0; i < K; i++)
    {
        compute_updated_temperatures(M, temp, temp_next);
        compute_updated_cells(M,cells,temp_next,cells_next,Tmin,Tmax);
        updated_matrix(M, temp, temp_next);
        updated_matrix(M, cells, cells_next);
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            printf("%d ", temp[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            printf("%d ", cells[i][j]);
        }
        printf("\n");
    }

    return 0;
}