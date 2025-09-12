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
    for (int i = 0; i < n; i++)
    {
        patients[i].MAP = calculate_map(patients[i].SBP, patients[i].DBP);
        patients[i].BMI = calculate_bmi(patients[i].weight, patients[i].height);
    }
}

int compare_patients(const void* a, const void* b)
{
    const Patient* p1 = (const Patient*)a;
    const Patient* p2 = (const Patient*)b;

    // compare patients p1 and p2
    //     return negative if patient p1 has higher risk
    //     return positive otherwise
    // seguindo a ordem de prioridade
    if (p1->MAP > p2->MAP) return -1; 
    if (p1->MAP < p2->MAP) return 1;
    if (p1->BMI > p2->BMI) return -1;
    if (p1->BMI < p2->BMI) return 1;
    if (p1->age > p2->age) return -1;
    if (p1->age < p2->age) return 1;
    if (p1->id < p2->id) return -1;
    if (p1->id > p2->id) return 1;

    return 0;
}

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

    for (int i = 0; i < *count; i++)
    {
        scanf("%d %d %f %f %f %f",
              &patients[i].id,
              &patients[i].age,
              &patients[i].weight,
              &patients[i].height,
              &patients[i].SBP,
              &patients[i].DBP);
    }

    return patients;
}

// Function to write patient data
void write_patients(Patient* patients, int n)
{
    // iterate and write patients values (id age MAP.2 BMI.2)
    for (int i = 0; i < n; i++)
    {
        printf("%d %d %.2f %.2f\n", patients[i].id, patients[i].age, patients[i].MAP, patients[i].BMI);
    }
    
}

int main()
{
    int n;
    Patient* patients = read_patients(&n);
    calculate_metrics(patients, n);
    qsort(patients, n, sizeof(Patient), compare_patients);
    write_patients(patients, n);

    // testando se estava lendo corretamente:
    // for (int i = 0; i < n; i++) {
    //     printf("ID=%d Age=%d Weight=%.2f Height=%.2f SBP=%.2f DBP=%.2f MAP=%.2f BMI=%.2f\n",
    //         patients[i].id, patients[i].age,
    //         patients[i].weight, patients[i].height,
    //         patients[i].SBP, patients[i].DBP,
    //         patients[i].MAP, patients[i].BMI);
    // }

    free(patients);
    return 0;
}