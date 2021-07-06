#include <stdio.h>
#include <stdlib.h>
// Sample CSV : Ultimax01A_Data.csv

// Function Declarations
double dividedDifference(int i, int j, double *flowRatesArray, double *diffHeadsArray);
void newtonInterpolation(int pointsCount, double *flowRatesArray, double *diffHeadsArray);

int main()
{
    char filename[1024], flow_rate[50], diff_head[50];
    double flowrates, diffheads, *flowRatesArray, *diffHeadsArray;
    int pointsCount = 1;
    FILE *fptr;

    printf("Input filename of CSV file containing the data points: ");
    scanf("%s", filename);

    fptr = fopen(filename, "r");
    fscanf(fptr, "%49s,%49s,", flow_rate, diff_head);

    flowRatesArray = (double *)calloc(pointsCount, pointsCount * sizeof(double *));
    diffHeadsArray = (double *)calloc(pointsCount, pointsCount * sizeof(double *));

    // reading the points
    while (!feof(fptr))
    {
        flowRatesArray = (double *)realloc(flowRatesArray, pointsCount * sizeof(double));
        diffHeadsArray = (double *)realloc(diffHeadsArray, pointsCount * sizeof(double));
        fscanf(fptr, "%lf,%lf,", flowRatesArray + (pointsCount - 1), diffHeadsArray + (pointsCount - 1));
        ++pointsCount;
    }
    // Extra Headin Count added so Subtracted..
    --pointsCount;

    fclose(fptr);

    printf("Program read %d data points from %s.\n", pointsCount, filename);
    printf("Interpolation polynomial to be derived is of degree %d.\n\n", pointsCount - 1);

    newtonInterpolation(pointsCount, flowRatesArray, diffHeadsArray);

    // freeing the memory allocated dynamically
    free(flowRatesArray);
    free(diffHeadsArray);
}

// Recursive Function to Calculate the Divided difference in Range i to j.
// Recursively calculates the dividedDiff == f[x(i) ,x(i + 1), ..... , x(j + 1)]
// 0 based indexing for 'i' and 'j'.
double dividedDifference(int i, int j, double *flowRatesArray, double *diffHeadsArray)
{
    // base condition
    if (i == j)
    {
        return diffHeadsArray[i];
    }

    // Calling Recursively -> f(i + 1,j) - f(i , j - 1) / (xj - xi);
    return ((dividedDifference(i + 1, j, flowRatesArray, diffHeadsArray) - dividedDifference(i, j - 1, flowRatesArray, diffHeadsArray)) / (flowRatesArray[j] - flowRatesArray[i]));
}

// Iterative Version of Newton Divider Difference InterPolation...
void newtonInterpolation(int pointsCount, double *flowRatesArray, double *diffHeadsArray)
{

    double valueTobeInterpolated; // Value to be interpolated -> valueTobeInterpolated
    double result = 0;            // InterPolated Value -> F(valueTobeInterpolated)
    int i, j;                     // iterators

    // Takes the Input Of the Value to be Interpolated.
    printf("Input the design flow rate Q (L/s): ");
    scanf("%lf", &valueTobeInterpolated);

    printf("h(Q) = ");

    for (i = pointsCount - 1; i >= 0; i--)
    {
        // currentTerm = dividedDifference[0,i]*(Q - x0)*(Q - x1)*....*(Q - x(i - 1));
        double currentTerm = 1;

        // Calls the Divided Difference Function to get it in range [0 , i].
        double dividedDiffTillHere = dividedDifference(0, i, flowRatesArray, diffHeadsArray);
        currentTerm *= dividedDiffTillHere;

        printf("%.4lf", dividedDiffTillHere);

        for (j = 0; j < i; j++)
        {
            printf("(Q - %.4lf)", flowRatesArray[j]);
            currentTerm *= (valueTobeInterpolated - flowRatesArray[j]);
        }

        // currentTerm added to Result
        result += currentTerm;

        if (i > 0)
            printf(" + ");
    }

    printf("\n\nEstimated differential head across the pump when Q = %.4lf L/s is h = %.4lf m.\n", valueTobeInterpolated, result);
}
