#include <stdio.h>

int main()
{
    char function[256];
    double x0;

    fgets(function, sizeof(function), stdin);
    scanf("%lf", &x0);

    return 0;
}
