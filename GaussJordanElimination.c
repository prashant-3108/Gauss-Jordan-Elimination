#include <stdio.h>
#include <stdlib.h>

double **inputData(int rows, int cols);
void display(int rows, int cols, double **A);
int GaussElim(int N, double **coeff, double **constants, double **solution);
void freeMatrix(int rows, double **matrix);

int main()
{
    double **coeff;
    double **constants;
    double **solution;

    int N, X = 1, i;
    printf("Number of linear equations in the system: ");
    scanf("%d", &N);

    printf("Input coefficient matrix: \n");
    coeff = inputData(N, N);
    printf("Input constants matrix: \n");
    constants = inputData(N, X);

    puts("\nCoeff matrix: ");
    display(N, N, coeff);
    puts("\nConstants matrix: ");
    display(N, X, constants);

    //Initialising the Solution Array
    solution = (double **)malloc(sizeof(double *) * N);
    for (i = 0; i < N; i++)
    {
        solution[i] = (double *)malloc(sizeof(double *) * 1);
    }

    // Calculating the Gaussian Elimination
    int solve = GaussElim(N, coeff, constants, solution);
    if (solve == 0)
    {
        puts("\nSystem of Linear Equations have Infinitely Many Solutions.");
    }
    else if (solve == -1)
    {
        puts("\nSystem of Linear Equations have No Solutions.");
    }
    else
    {
        puts("\nSystem of Linear Equations have a Unique Solution.");
        display(N, X, solution);
    }

    // Freeing the Memory of the Matrices Used
    freeMatrix(N, solution);
    freeMatrix(N, coeff);
    freeMatrix(N, constants);

    return 0;
}

//input the Matrix Elements..
double **inputData(int rows, int cols)
{
    double **matrix;
    matrix = calloc(rows, rows * sizeof(double *));

    for (int i = 0; i < rows; i++)
        matrix[i] = calloc(rows, cols * sizeof(double));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("\tInput the value of the matrix at [%d][%d]: ", i, j);
            scanf("%lf", &matrix[i][j]);
        }
    }
    return matrix;
}

// Function to calculate if Solution exists or not and if exists updates the Solution Matrix of N x 1.
int GaussElim(int N, double **coeff, double **constants, double **solution)
{
    int rows = N, cols = N + 1, i, j, k, currRow = 0, currCol = 0;
    int returnVal = 1;
    double **augmented;

    // initialising the augmented[N][N+1] matrix;
    augmented = (double **)malloc(sizeof(double *) * rows);
    for (i = 0; i < N; i++)
    {
        augmented[i] = (double *)malloc(sizeof(double *) * cols);
    }

    // Assigning values to Augmented Matrix...
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            if (j < cols - 1)
            {
                augmented[i][j] = coeff[i][j];
            }
            else
            {
                augmented[i][j] = constants[i][0];
            }
        }
    }

    puts("\nAugmented matrix before Forward Elimination: ");
    display(rows, cols, augmented);

    // currRow and CurrCol are the indices deciding pivots
    while (currRow < rows && currCol < cols - 1)
    {
        // BONUS part : swap from bottom as Case of Division by 0 can occur here.
        // Pivot needs to to be Swapped with non-zero Value
        if (augmented[currRow][currCol] == 0)
        {
            int foundRowSwap = 0;
            for (j = currRow + 1; j < rows; j++)
            {
                if (augmented[j][currCol] != 0)
                {
                    foundRowSwap = 1;
                    break;
                }
            }
            // if Found then Swap Row
            if (foundRowSwap)
            {
                //swapping
                double temp;
                for (k = 0; k < cols; k++)
                {
                    temp = augmented[currRow][k];
                    augmented[currRow][k] = augmented[j][k];
                    augmented[j][k] = temp;
                }
            }
            // Traverse to the next col in Same Row to Find Pivot of this Row
            else
            {
                currCol++;
                continue;
            }
        }

        // Found the Pivot
        double division = augmented[currRow][currCol];

        // if 0 then break
        if (division == 0)
        {
            break;
        }

        // make Pivot = 1
        for (k = 0; k < cols; k++)
        {
            //dividing row elements by Pivot
            augmented[currRow][k] /= division;
        }

        // Reducing Each row to make all elements in other rows 0 except pivot  Element (=1).
        for (j = 0; j < rows; j++)
        {
            if (j != currRow)
            {
                double toBeMultiplied = augmented[j][currCol];

                for (k = 0; k < cols; k++)
                {
                    augmented[j][k] -= toBeMultiplied * augmented[currRow][k];
                }
            }
        }
        // onto next pivot
        currRow++;
        currCol++;
    }

    puts("\nAugmented matrix after Forward Elimination: ");
    display(rows, cols, augmented);

    // checking for Result Status

    int zeroCount = 0;
    for (i = 0; i < cols; i++)
    {
        if (!augmented[rows - 1][i])
        {
            zeroCount++;
        }
    }

    // Infinite Solutions as 0 == 0 for last row..
    if (zeroCount == cols)
    {
        returnVal = 0;
    }

    // No Solutions as 0 != val for last row..
    if (zeroCount == cols - 1)
    {
        returnVal = -1;
    }

    // Back Substitution - Unique Solutions
    // if solution exists then calculates
    if (returnVal == 1)
    {
        for (i = rows - 1; i >= 0; i--)
        {
            solution[i][0] = augmented[i][cols - 1];

            // reduces the calculayted xi's greater than current xi
            for (int j = i + 1; j < rows; j++)
            {
                solution[i][0] -= augmented[i][j] * solution[j][0];
            }

            // solution[i][0] /= augmented[i][i];
        }
    }

    freeMatrix(N, augmented);
    return returnVal;
}

// display the matrix
void display(int rows, int cols, double **A)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            printf("%12.4lf ", A[i][j]);
        puts("");
    }
}

// free the memory of dynamically allocated matrix
void freeMatrix(int rows, double **matrix)
{
    int i = 0;
    for (i = 0; i < rows; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
}