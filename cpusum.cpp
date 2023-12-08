#include <iostream>
#include <chrono>
#include <cmath>

inline double sinsum(double x, int terms)
{
    // sin(x) = x - x^3/3! + x^5/5! + ....
    double term = x; // first term of series
    double sum = term;
    double x2 = x*x;

    for(int n=1; n<terms; n++){
        term *= -x2/(2*n*(2*n+1));
        sum += term;
    }
    return sum;
}

int main(int argc, char*argv[]){
    int steps = (argc>1) ? atoi(argv[1]) : 10000000; // 10 millions
    int terms = (argc>2) ? atoi(argv[2]) : 1000;

    double pi = 3.14159265358979323;
    double step_size = pi/(steps-1); // n-1 steps

    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> dif(0.0);    

    start = std::chrono::system_clock::now();
    
    double cpu_sum = 0.0;
    for(int step=0; step<steps; step++){
        double x = step_size*step;
        cpu_sum += sinsum(x, terms);    // sum of Taylor series
    }
    end = std::chrono::system_clock::now();
    dif = end - start;

    // Trapezoidal Rule correction
    cpu_sum -= 0.5*(sinsum(0.0,terms)+sinsum(pi,terms));
    cpu_sum *= step_size;
    printf("cpu sum = %.10f, steps %d, terms %d, time %.3f s\n",
        cpu_sum, steps, terms, dif.count());

    return 0;
}
