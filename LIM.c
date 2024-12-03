// Last modified on: 4/20/21
// Modified by: Russell512
// Description: This is a C program to implement the Lagrange Interpolation Method
// To run this file, compile using: gcc LIM.c -o LIM, and then execute ./LIM

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to calculate the greatest common divisor (GCD)
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Structure to represent a fraction
typedef struct {
    long long num; // Numerator
    long long den; // Denominator
} Fraction;

// Normalize the fraction to its simplest form
void normalize(Fraction *frac) {
    if (frac->den < 0) {
        frac->num = -frac->num;
        frac->den = -frac->den;
    }
    long long g = gcd(llabs(frac->num), llabs(frac->den));
    frac->num /= g;
    frac->den /= g;
}

// Fraction addition
Fraction fraction_add(Fraction a, Fraction b) {
    Fraction result;
    result.num = a.num * b.den + b.num * a.den;
    result.den = a.den * b.den;
    normalize(&result);
    return result;
}

// Fraction multiplication
Fraction fraction_mul(Fraction a, Fraction b) {
    Fraction result;
    result.num = a.num * b.num;
    result.den = a.den * b.den;
    normalize(&result);
    return result;
}

// Polynomial multiplication
Fraction* multiplyPolynomials(Fraction* a, int deg_a, Fraction* b, int deg_b, int *deg_result) {
    int deg_r = deg_a + deg_b;
    Fraction *result = (Fraction*)calloc(deg_r + 1, sizeof(Fraction));
    // Initialize coefficients of the result polynomial to zero
    for (int i = 0; i <= deg_r; i++) {
        result[i].num = 0;
        result[i].den = 1;
    }
    for (int i = 0; i <= deg_a; i++) {
        for (int j = 0; j <= deg_b; j++) {
            Fraction prod = fraction_mul(a[i], b[j]);
            result[i + j] = fraction_add(result[i + j], prod);
        }
    }
    *deg_result = deg_r;
    return result;
}

// Polynomial addition
Fraction* addPolynomials(Fraction* a, int deg_a, Fraction* b, int deg_b, int *deg_result) {
    int deg_r = (deg_a > deg_b) ? deg_a : deg_b;
    Fraction *result = (Fraction*)malloc((deg_r + 1) * sizeof(Fraction));
    for (int i = 0; i <= deg_r; i++) {
        Fraction term_a = {0, 1};
        Fraction term_b = {0, 1};
        if (i <= deg_a) term_a = a[i];
        if (i <= deg_b) term_b = b[i];
        result[i] = fraction_add(term_a, term_b);
    }
    *deg_result = deg_r;
    return result;
}

int main() {
    int N;
    long long *x = NULL;
    long long *y = NULL;

    printf("Enter the number of data points N and N pairs of data points (Xi Yi), this is done in C:\n");
    // Input data
    scanf("%d", &N);

    x = (long long*)malloc(N * sizeof(long long));
    y = (long long*)malloc(N * sizeof(long long));

    for (int i = 0; i < N; i++) {
        scanf("%lld %lld", &x[i], &y[i]);
    }

    if (N == 0) {
        fprintf(stderr, "No data points entered.\n");
        return 1;
    }

    // Initialize the polynomial to zero
    Fraction *polynomial = NULL;
    int deg_poly = 0; // Highest degree of the polynomial
    polynomial = (Fraction*)malloc(sizeof(Fraction));
    polynomial[0].num = 0;
    polynomial[0].den = 1;

    // Lagrange Interpolation Method
    for (int i = 0; i < N; i++) {
        // Initialize numerator to 1
        Fraction *numerator = NULL;
        int deg_num = 0;
        numerator = (Fraction*)malloc(sizeof(Fraction));
        numerator[0].num = 1;
        numerator[0].den = 1;

        long long denominator = 1;

        for (int j = 0; j < N; j++) {
            if (i != j) {
                // Construct the term (x - x_j)
                Fraction term[2];
                term[0].num = -x[j];
                term[0].den = 1;
                term[1].num = 1;
                term[1].den = 1;
                int deg_term = 1;

                // Multiply the numerator by the current term
                int deg_new_num;
                Fraction *new_num = multiplyPolynomials(numerator, deg_num, term, deg_term, &deg_new_num);
                // Free the old numerator
                free(numerator);
                numerator = new_num;
                deg_num = deg_new_num;

                // Update the denominator
                denominator *= (x[i] - x[j]);
            }
        }

        // Multiply the numerator by y[i] / denominator
        for (int k = 0; k <= deg_num; k++) {
            numerator[k].num *= y[i];
            numerator[k].den *= denominator;
            normalize(&numerator[k]);
        }

        // Add the current term to the polynomial
        int deg_new_poly;
        Fraction *new_poly = addPolynomials(polynomial, deg_poly, numerator, deg_num, &deg_new_poly);
        // Free the old polynomial and numerator
        free(polynomial);
        free(numerator);
        polynomial = new_poly;
        deg_poly = deg_new_poly;
    }

    // Output the polynomial
    printf("f(x) = ");
    int first = 1;
    for (int i = deg_poly; i >= 0; i--) {
        if (polynomial[i].num != 0) {
            if (!first && polynomial[i].num > 0)
                printf("+");
            if (i == 0 || llabs(polynomial[i].num) != polynomial[i].den)
                if (polynomial[i].den == 1)
                    printf("%lld", polynomial[i].num);
                else
                    printf("(%lld/%lld)", polynomial[i].num, polynomial[i].den);
            if (i >= 1)
                printf("x");
            if (i >= 2)
                printf("^%d", i);
            first = 0;
        }
    }
    printf("\n");

    // Free allocated memory
    free(x);
    free(y);
    free(polynomial);

    return 0;
}
