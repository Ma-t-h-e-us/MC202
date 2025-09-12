// ==================================================
// Definitions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM_M 100

#define MINIMUM 15
#define MAXIMUM 25

// Struct to hold config data
typedef struct {
    int M;
    int K;
    float Tmin;
    float Tmax;
    char input_file[100];
} Config;

// ==================================================
// Utility Functions

const char* get_file_name(int argc, char *argv[], const char* default_file) {
    if (argc >= 2) {
        return argv[1];  // Return the first argument
    } else {
        return default_file;  // Return default value if no argument
    }
}

// Function to open a file for writing or reading, returns FILE pointer
FILE* open_file(const char* file_name, const char* modes)
{
    FILE* file = fopen(file_name, modes);
    if (!file) {
        perror("Error opening file");
    }
    return file;
}

// Function to close a file
void close_file(FILE* file)
{
    if (file) fclose(file);
}

// Function to prompt user and fill the struct
void read_config_from_std(Config* cfg)
{
    printf("Enter number of iterations (K): ");
    scanf("%d", &cfg->K);

    printf("Enter minimum temperature (Tmin): ");
    scanf("%f", &cfg->Tmin);

    printf("Enter maximum temperature (Tmax): ");
    scanf("%f", &cfg->Tmax);

    printf("Enter input file name (e.g. test_dish): ");
    scanf("%s", cfg->input_file);
}

int read_config_from_file(const char* file_name, Config* cfg)
{
	FILE* file = open_file(file_name, "r");
    if (!file) return 1;

    // Default or undefined value for M (not in file)
    cfg->M = -1;

    char key[50], value[100];
    while (fscanf(file, "%49s = %99[^\n]\n", key, value) == 2) {
        // Trim trailing newline/spaces
        int len = strlen(value);
        while (len > 0 && (value[len - 1] == '\n' || value[len - 1] == ' '))
            value[--len] = '\0';

        if (strcmp(key, "K") == 0)
            cfg->K = atoi(value);
        else if (strcmp(key, "Tmin") == 0)
            cfg->Tmin = atof(value);
        else if (strcmp(key, "Tmax") == 0)
            cfg->Tmax = atof(value);
        else if (strcmp(key, "dish_file") == 0)
            strncpy(cfg->input_file, value, sizeof(cfg->input_file));
    }

	close_file(file);
    return 1;
}

// Function to save struct data to file
int write_config_to_file(const char* file_name, const Config* cfg)
{
	FILE* file = open_file(file_name, "w");
    if (!file) return 1;

    fprintf(file, "K = %d\n", cfg->K);
    fprintf(file, "Tmin = %.2f\n", cfg->Tmin);
    fprintf(file, "Tmax = %.2f\n", cfg->Tmax);
    fprintf(file, "dish_file = %s\n", cfg->input_file);

	close_file(file);
    return 1;
}

// Read input matrices from file
int read_dish_from_file(const char *file_name, int temp[MAXIMUM_M][MAXIMUM_M], int cells[MAXIMUM_M][MAXIMUM_M], int *M)
{
	FILE* file = open_file(file_name, "r");
    if (!file) return 1;

    int i = 0, j;
    char line[256];
    int value;
    int reading_temp = 1;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '#' || line[0] == '\0') {
            if (reading_temp) {
                *M = i;
                i = 0;
                reading_temp = 0;
            }
            continue;
        }

        j = 0;
        char *ptr = line;
        while (sscanf(ptr, "%d", &value) == 1) {
            if (reading_temp) {
                temp[i][j++] = value;
            } else {
                cells[i][j++] = value;
            }

            while (*ptr != ' ' && *ptr != '\0') ptr++;
            while (*ptr == ' ') ptr++;
        }

        i++;
    }

	close_file(file);
    return 1;
}

void write_matrix_to_std(const char *title, int M, int matrix[][100])
{
    printf("%s", title != NULL ? title : "");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void write_dish_to_std(int temp[MAXIMUM_M][MAXIMUM_M], int cells[MAXIMUM_M][MAXIMUM_M], int M)
{
    write_matrix_to_std(NULL, M, temp);
	printf("\n");
    write_matrix_to_std(NULL, M, cells);
}

// Generate aleatory temperatures between MINIMUN and MAXIMUN
void write_random_temperatures_to_file(FILE *f, int M)
{
	int amplitude = MAXIMUM - MINIMUM;
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			int temp = MINIMUM + (rand() % amplitude);
			fprintf(f, "%d ", temp);
		}
		fprintf(f, "\n");
	}
}

// Generae aleatory cells distribution
void write_random_cells_to_file(FILE *f, int M)
{
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			int alive = rand() % 2;
			fprintf(f, "%d ", alive);
		}
		fprintf(f, "\n");
	}
}

int write_random_dish_to_file(const char* file_name, int M)
{
    FILE* f = open_file(file_name, "w");
    if (!f) return 1;

	write_random_temperatures_to_file(f, M);
	fprintf(f, "\n");
	write_random_cells_to_file(f, M);

	close_file(f);
    return 1;
}

// ==================================================
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
            destiny[i][k] = origin[i][k];
        }
    }
}

int main(int argc, char *argv[])
{
    // ATTENTION: Change input read mode back to standard input (keyboard) before submitting!
    // Current mode may be set to read from file or custom source for testing.

    // Read file name from arguments
	const char *file_name = get_file_name(argc, argv, "test_config");

    // Experiment configuration
    Config cfg;

    // MxM matrices (*_next store update)
    int temp[MAXIMUM_M][MAXIMUM_M] = {0};
    int temp_next[MAXIMUM_M][MAXIMUM_M] = {0};

    int cells[MAXIMUM_M][MAXIMUM_M] = {0};
    int cells_next[MAXIMUM_M][MAXIMUM_M] = {0};

    // Read configurations from file
    if (!read_config_from_file(file_name, &cfg)) {
        return 1;
    }

    // Read dish status from file
    if (!read_dish_from_file(cfg.input_file, temp, cells, &cfg.M)) {
        return 1;
    }

    // K iterations
    for (int step = 0; step < cfg.K; step++) {
        // --- 1. Update temperatures ---
        compute_updated_temperatures(cfg.M, temp, temp_next);

        // --- 2. Update Cells (Game of Life + Temperature) ---
        compute_updated_cells(cfg.M, cells, temp_next, cells_next, cfg.Tmin, cfg.Tmax);

        // Update matrices for the next iteration
        updated_matrix(cfg.M, temp_next, temp);
        updated_matrix(cfg.M, cells_next, cells);
    }

    // --- Show final results ---
    write_dish_to_std(temp, cells, cfg.M);

    return 0;
}
