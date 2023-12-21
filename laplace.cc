#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <chrono>

#define SIZE 400

int main()
{
    int i,j,n,ht,niter;
    double *g1, *g2, *p;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> dif(0.0);

    FILE *img;

    ht = SIZE+2;
    niter = 40000;

    g1 = (double*) malloc(SIZE*ht*sizeof(double));
    g2 = (double*) malloc(SIZE*ht*sizeof(double));

    for(i=0; i<SIZE*ht; i++)
        g1[i] = 0.0;                            // Zero whole grid

    for(i=SIZE/8; i<7*SIZE/8; i++)
        g1[i] = 1.0;                            // Boundary Condition

    // Copy top row to second grid
    for(i=0; i<SIZE; i++)
        g2[i] = g1[i];
    // Copy bottom row to second grid
    for(i=0; i<SIZE; i++)
        g2[i+(ht-1)*SIZE] = g1[i+(ht-1)*SIZE];

    // iteration loop
    start = std::chrono::system_clock::now();
    for(n=0; n<niter; n++)
    {
        for(j=1; j<ht-1; j++)
        {
            // Left
            g2[j*SIZE] = (g1[j*SIZE+1] + g1[(j-1)*SIZE] + g1[(j+1)*SIZE])/3.0;
            // Body
            for(i=1; i<SIZE-1; i++)
            {
                g2[j*SIZE+i] = 0.25*(g1[j*SIZE+i-1]+g1[j*SIZE+i+1]+g1[(j-1)*SIZE+i]+g1[(j+1)*SIZE+i]);
            }
            // Right
            g2[j*SIZE+SIZE-1] = (g1[j*SIZE+SIZE-2]+g1[(j-1)*SIZE+SIZE-1]+g1[(j+1)*SIZE+SIZE-1])/3.0;
        }
        p=g1;
        g1=g2;
        g2=p;
    }
    end = std::chrono::system_clock::now();
    dif = end - start; 
    std::cout<<"Total Iteration Time: "<<dif.count()<<"s\n";
    
    img = fopen("laplace.pam","w");
    fprintf(img,"P2\n %d %d 255\n",SIZE, SIZE);
    for(i=SIZE; i<(SIZE+1)*SIZE; i++)
        fprintf(img,"%d\n",(int)(255*g1[i]));
    fclose(img);

    return 0;
}