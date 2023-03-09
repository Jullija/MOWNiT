#include <stdio.h>
#include <gsl/gsl_ieee_utils.h>
#include <gsl/gsl_math.h>

int main(){
    gsl_ieee_env_setup();
    float num = 1.0/3.0;
    gsl_ieee_printf_float(&num);
    printf("\n");
    
    int i = 0;
    while (i < 150){
        num = num/2;
        gsl_ieee_printf_float(&num);
        printf("\n");
        i += 1;
    }
    return 0;
}