#include "config.h"
#include <string.h>

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

void read_matrix_from_std(int M, int matrix[MAXIMUM_M][MAXIMUM_M]) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
}

// Read input matrices from user
void read_dish_from_std(int M, int temp[MAXIMUM_M][MAXIMUM_M], int cells[MAXIMUM_M][MAXIMUM_M]) {
    read_matrix_from_std(M, temp);
    read_matrix_from_std(M, cells);
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
