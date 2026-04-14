// Online C compiler to run C program online
// chat_gpt_link: https://chatgpt.com/share/68f777d5-6f1c-8000-a425-42bbbd0bb25f
#include <stdio.h>
#include <math.h>

#define PI      3.14159265358979323846f
#define HALF_PI 1.57079632679489661923f

float fast_sine(float x)
{
    // Normalize input range to [-pi, pi]
    if (x >  PI) x -= 2.0f * PI;
    if (x < -PI) x += 2.0f * PI;

    float sign = 1.0f;

    // Sine is odd function
    if (x < 0.0f)
    {
        x = -x;
        sign = -1.0f;
    }

    // Range reduction to [0, pi/2]
    if (x > HALF_PI)
        x = PI - x;  // mirror quadrant 2 to quadrant 1

    // // Polynomial approximation: sin(x) ≈ x - x^3/6 + x^5/120
    // float x2 = x * x;
    // float result = x * (1.0f - x2 *0.1666666666667f + 
    //                                         (x2 * x2) *0.00833333333333f);

    // Minimax 5th order polynomial: sin(x) ≈ x*(a0 + a1*x² + a2*x⁴)
    const float a0 = 0.9999793133f;
    const float a1 = -0.1666240430f;
    const float a2 = 0.0083088221f;

    float x2 = x * x;
    float result = x * (a0 + a1 * x2 + a2 * x2 * x2);

    return sign * result;
}

// corrected robust version
float horner_sine(float x)
{
    //const float PI = 3.14159265358979323846f;
    const float TWO_PI = 2.0f * PI;
    //const float HALF_PI = 0.5f * PI;
    const float QUARTER_PI = 0.25f * PI;

    // Reduce to [-pi, pi]
    x = fmodf(x, TWO_PI);
    if (x <= -PI)
        x += TWO_PI;
    else if (x > PI)
        x -= TWO_PI;

    // Map input from [-pi, pi] to [0, pi/2] using sine symmetry
    float pos;     // position in [0, pi/2]
    int sign = 1;
    if (x < 0.0f) {
        x = -x;
        sign = -1;
    }

    if (x <= HALF_PI) {
        // [0, pi/2]
        pos = x;
    } else {
        // (pi/2, pi]
        pos = PI - x;
    }

    // Now pos in [0, pi/2]. Use sin Taylor for small pos, otherwise compute cos(y) with y = pi/2 - pos
    if (pos <= QUARTER_PI) {
        // sin(pos) via 7th-order Taylor series in Horner form (good for small pos)
        float x2 = pos * pos;
        const float c3 = -1.0f / 6.0f;       // -1/3!
        const float c5 = 1.0f / 120.0f;      // 1/5!
        const float c7 = -1.0f / 5040.0f;    // -1/7!
        float result = pos * (1.0f + x2 * (c3 + x2 * (c5 + x2 * c7)));
        return sign * result;
    } else {
        // For pos near pi/2 compute cos(y) with y = pi/2 - pos (y small), using Horner form
        float y = HALF_PI - pos;
        float y2 = y * y;
        const float c2 = -0.5f;             // -1/2!
        const float c4 = 1.0f / 24.0f;      // 1/4!
        const float c6 = -1.0f / 720.0f;    // -1/6!
        float result = 1.0f + y2 * (c2 + y2 * (c4 + y2 * c6));
        return sign * result;
    }
}

int main() {
    // Write C code here
    for(float i =-3.14159f; i<=3.14159f;i+=0.0174f)
    {
        printf("%.10f\n",horner_sine(i));
    }
    return 0;
}