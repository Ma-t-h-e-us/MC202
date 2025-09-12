#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int age;
    float weight;
    float height;
    float SBP; // Systolic Blood Pressure
    float DBP; // Diastolic Blood Pressure
    float MAP; // Mean Arterial Pressure
    float BMI; // Body Mass Index
} Patient;

// Function to calculate MAP
float calculate_map(int SBP, int DBP)
{
    return ((1.0/3)*SBP+(2.0/3)*DBP);
}

// Function to calculate BMI
float calculate_bmi(float weight, float height)
{
    return (weight/(height*height));
}

// Function to calculate MAP and BMI for all patients
void calculate_metrics(Patient* patients, int n)
{
    // iterate and calcule each patient MAP and BMI

    // save the values inside the patient struct 
}

// Comparator patients
int compare_patients(const void* a, const void* b)
{
    const Patient* p1 = (const Patient*)a;
    const Patient* p2 = (const Patient*)b;

    // compare patients p1 and p2
    //     return negative if patient p1 has higher risk
    //     return positive otherwise

    return 0;
}

// Function to sort the list of patient based on compar function
// void qsort(void *base, size_t num, size_t size, int (*compar)(const void *, const void *))
// {
//     sort the list of patient based on compar function
//     num - number of clients
//     size - size in bytes of a client struct
//     compar - function to compare two clients
// }

// Function to read patient data from stdin
Patient* read_patients(int* count)
{
    scanf("%d", count);

    Patient* patients = malloc(*count * sizeof(Patient));

    if (patients == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    // iterate and read patients values (id age ...)

    for (int i = 0; i < count; i++)
    {
        scanf(

        )
    }
    

    return patients;
}

// Function to write patient data
void write_patients(Patient* patients, int n)
{
    // iterate and write patients values (id age MAP.2 BMI.2)
}

int main()
{
    int n;
    
    Patient* patients = read_patients(&n);

    calculate_metrics(patients, n);
    qsort(patients, n, sizeof(Patient), compare_patients);
    
    write_patients(patients, n);
    free(patients);

    return 0;

}