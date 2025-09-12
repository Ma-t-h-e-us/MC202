#ifndef CONFIG_H
#define CONFIG_H

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

const char* get_file_name(int argc, char *argv[], const char* default_file);

FILE* open_file(const char* file_name, const char* modes);
void close_file(FILE* f);

void read_config_from_std(Config* cfg);
int read_config_from_file(const char* file_name, Config* cfg);
int write_config_to_file(const char* file_name, const Config* cfg);

void read_dish_from_std(int M, int temp[MAXIMUM_M][MAXIMUM_M], int cells[MAXIMUM_M][MAXIMUM_M]);
int read_dish_from_file(const char *file_name, int temp[MAXIMUM_M][MAXIMUM_M], int cells[MAXIMUM_M][MAXIMUM_M], int *M);
void write_dish_to_std(int temp[MAXIMUM_M][MAXIMUM_M], int cells[MAXIMUM_M][MAXIMUM_M], int M);

int write_random_dish_to_file(const char* file_name, int M);

#endif // CONFIG_H