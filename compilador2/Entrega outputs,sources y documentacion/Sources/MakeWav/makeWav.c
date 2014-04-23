#include <stdio.h>
#include <stdlib.h>

/* test_make_wav.c
 * Fri Jun 18 17:13:19 PDT 2010 Kevin Karplus
 * Test program for the write_wav function in make_wav.c
 */

#include <math.h>
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 3\WAV\make_wav.h"
#include "C:\Users\Toto\Projects\Proyecto Nacho Cassol\Proyecto-Parser-C\compilador2\Lote de pruebas\Case 3\WAV\make_wav.c"

#define S_RATE  44100
#define BUF_SIZE 88200 //reemplazo por el valor actual por el parser S_RATE*2 /* 2 second buffer */

int buffer[BUF_SIZE];

int main(int argc, char * argv)
{
    int i;
    float t;
    float amplitude = 32000;
    float freq_Hz = 440;
    float phase=0;

    //float freq_radians_per_sample = freq_Hz*2*M_PI/S_RATE; //comentado por el numero ya calculado
    //hasta arreglar el parseo de los asteriscos
    float freq_radians_per_sample = 1073741824;
    //printf("%d",freq_radians_per_sample);

    //* fill buffer with a sine wave */
    for (i=0; i<BUF_SIZE; i++)
    {
        phase += freq_radians_per_sample;
    buffer[i] = (int)(amplitude * sin(phase));
    }

    write_wav(BUF_SIZE, buffer, S_RATE); //saque "test.wav" xq no lo reconocia

    return 0;
}
