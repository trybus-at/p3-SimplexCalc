#include <iostream>
#include <cmath>
#include <vector>
using namespace std;


class Simplex
{

private:
    int rows, cols;
    // Stores coefficients of all the variables
    vector<vector<float> > A;
    // Stores constants of constraints
    vector<float> B;
    // Stores the coefficients of the objective function
    vector<float> C;
    int k;

    float maximum;

    bool isUnbounded;

public:
    Simplex(vector<vector<float> > matrix, vector<float> b, vector<float> c, int k_in)
    {
        maximum = 0;
        isUnbounded = false;
        rows = matrix.size();
        cols = matrix[0].size();
        A.resize(rows, vector<float>(cols, 0));
        B.resize(b.size());
        C.resize(c.size());
        k = k_in;

        for (int i = 0; i < rows; i++)
        { // Pass A[][] values to the matrix
            for (int j = 0; j < cols; j++)
            {
                A[i][j] = matrix[i][j];
            }
        }

        for (int i = 0; i < c.size(); i++)
        { // Pass c[] values to the C vector
            C[i] = c[i];
        }
        for (int i = 0; i < b.size(); i++)
        { // Pass b[] values to the B vector
            B[i] = b[i];
        }
    }

    bool simplexAlgorithmCalculataion()
    {
        // Check whether the table is optimal, if optimal no need to process further
        if (checkOptimality())
        {
            return true;
        }

        // Find the column which has the pivot. The least coefficient of the objective function(C array).
        int pivotColumn = findPivotColumn();

        if (isUnbounded)
        {
            cout << "Error unbounded" << endl;
            return true;
        }

        // Find the row with the pivot value. The least value item's row in the B array
        int pivotRow = findPivotRow(pivotColumn);

        // Form the next table according to the pivot value
        doPivotting(pivotRow, pivotColumn);

        return false;
    }

    bool checkOptimality()
    {
        // If the table has further negative constraints,then it is not optimal
        bool isOptimal = false;
        int positveValueCount = 0;

        // Check if the coefficients of the objective function are negative
        for (int i = 0; i < C.size(); i++)
        {
            float value = C[i];
            if (value >= 0)
            {
                positveValueCount++;
            }
        }
        // If all the constraints are positive now,the table is optimal
        if (positveValueCount == C.size())
        {
            isOptimal = true;
            print();
        }
        return isOptimal;
    }

    void doPivotting(int pivotRow, int pivotColumn)
    {

        float pivetValue = A[pivotRow][pivotColumn]; // Gets the pivot value

        float pivotRowVals[cols]; // The column with the pivot

        float pivotColVals[rows]; // The row with the pivot

        float rowNew[cols]; // The row after processing the pivot value

        maximum = maximum - (C[pivotColumn] * (B[pivotRow] / pivetValue)); // Set the maximum step by step
        // Get the row that has the pivot value
        for (int i = 0; i < cols; i++)
        {
            pivotRowVals[i] = A[pivotRow][i];
        }
        // Get the column that has the pivot value
        for (int j = 0; j < rows; j++)
        {
            pivotColVals[j] = A[j][pivotColumn];
        }

        // Set the row values that has the pivot value divided by the pivot value and put into new row
        for (int k = 0; k < cols; k++)
        {
            rowNew[k] = pivotRowVals[k] / pivetValue;
        }

        B[pivotRow] = B[pivotRow] / pivetValue;

        // Process the other coefficients in the A array by subtracting
        for (int m = 0; m < rows; m++)
        {
            // Ignore the pivot row as we already calculated that
            if (m != pivotRow)
            {
                for (int p = 0; p < cols; p++)
                {
                    float multiplyValue = pivotColVals[m];
                    A[m][p] = A[m][p] - (multiplyValue * rowNew[p]);
                }
            }
        }

        // Process the values of the B array
        for (int i = 0; i < B.size(); i++)
        {
            if (i != pivotRow)
            {

                float multiplyValue = pivotColVals[i];
                B[i] = B[i] - (multiplyValue * B[pivotRow]);
            }
        }
        // The least coefficient of the constraints of the objective function
        float multiplyValue = C[pivotColumn];
        // Process the C array
        for (int i = 0; i < C.size(); i++)
        {
            C[i] = C[i] - (multiplyValue * rowNew[i]);
        }

        // Replacing the pivot row in the new calculated A array
        for (int i = 0; i < cols; i++)
        {
            A[pivotRow][i] = rowNew[i];
        }
    }

    // Print the current A array
    void print()
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cout << A[i][j] << " ";
            }
            cout << "" << endl;
        }
        cout << "" << endl;
    }

    // Find smallest constraint coefficients in the objective function's position
    int findPivotColumn()
    {

        int location = 0;
        float minm = C[0];

        for (int i = 1; i < C.size(); i++)
        {
            if (C[i] < minm)
            {
                minm = C[i];
                location = i;
            }
        }

        return location;
    }

    // Find the row with the pivot value.The least value item's row in the B array
    int findPivotRow(int pivotColumn)
    {
        float positiveValues[rows];
        vector<float> result(rows, 0);

        int negativeValueCount = 0;

        for (int i = 0; i < rows; i++)
        {
            if (A[i][pivotColumn] > 0)
            {
                positiveValues[i] = A[i][pivotColumn];
            }
            else
            {
                positiveValues[i] = 0;
                negativeValueCount += 1;
            }
        }
        // Checking the unbound condition if all the values are negative ones
        if (negativeValueCount == rows)
        {
            isUnbounded = true;
        }
        else
        {
            for (int i = 0; i < rows; i++)
            {
                float value = positiveValues[i];
                if (value > 0)
                {
                    result[i] = B[i] / value;
                }
                else
                {
                    result[i] = 0;
                }
            }
        }
        // find the minimum's location of the smallest item of the B array
        float minimum = 99999999;
        int location = 0;
        for (int i = 0; i < sizeof(result) / sizeof(result[0]); i++)
        {
            if (result[i] > 0)
            {
                if (result[i] < minimum)
                {
                    minimum = result[i];

                    location = i;
                }
            }
        }

        return location;
    }

    void CalculateSimplex()
    {
        bool end = false;

        cout << "Initial array (not optimal):" << endl;
        print();

        cout << "Final array (optimal solution)" << endl;

        while (!end)
        {

            bool result = simplexAlgorithmCalculataion();

            if (result)
            {

                end = true;
            }
        }
        cout << "Optimal column strategy:" << endl;
        cout << "[ ";

        for (int i = 0; i < A.size(); i++)
        { // every basic column has the values, get it form B array
            int count0 = 0;
            int index = 0;
            for (int j = 0; j < rows; j++)
            {
                if (A[j][i] == 0.0)
                {
                    count0 += 1;
                }
                else if (A[j][i] == 1)
                {
                    index = j;
                }
            }

            if (count0 == rows - 1)
            {

                cout  << B[index] / maximum << " "; // every basic column has the values, get it form B array
            }
            else
            {
                cout << 0 << " ";
            }
        }

        cout << "]^T" << endl << endl;
        cout << "value of the game: " << -1 * (1 / maximum - static_cast<double>(k)) << endl;
    }
};

int main()
{

    int colSizeA = 6; // set column size in A[][] matrix
    int rowSizeA = 3; // set row size in A[][] matrix

    float C[] = {-1, -1, -1, 0, 0, 1}; // represents the objective funtion
    float B[] = {1, 1, 1, 0};       // represents the right side of constraint inequalities

    float a[3][6] = {// initialize the A[][] array here
                     {3, 2, 8, 1, 0, 0},
                     {0, 10, 4, 0, 1, 0},
                     {0, 5, 5, 0, 0, 1}};

    vector<vector<float> > vec2D(rowSizeA, vector<float>(colSizeA, 0));
    
    int k = 5; // value added to all entries

    vector<float> b(rowSizeA, 0);
    vector<float> c(colSizeA, 0);

    for (int i = 0; i < rowSizeA; i++)
    { // make a vector from given array
        for (int j = 0; j < colSizeA; j++)
        {
            vec2D[i][j] = a[i][j];
        }
    }

    for (int i = 0; i < rowSizeA; i++)
    {
        b[i] = B[i];
    }

    for (int i = 0; i < colSizeA; i++)
    {
        c[i] = C[i];
    }

    // here make the class parameters with A[m][n] vector b[] vector and c[] vector
    Simplex simplex(vec2D, b, c, k);
    simplex.CalculateSimplex();

    return 0;
}