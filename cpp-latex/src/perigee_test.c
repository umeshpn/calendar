
/* perigee_test.c  */

#include <stdio.h>
#include "ast.h"

void find_perigees(int, int);
double get_perigee(int);

int
main() {
   find_perigees(1900, 2001);
   return 0;
}

void 
find_perigees(int first_year, int last_year) {
   char dts[50];
   double jde, j_cent, phase_of_moon, perigee_value;
   double l_sunt, l_moont, l_earth, au_earth, theta;
   int k, first_k, last_k;
   first_k = (int)((first_year - 1999.97) * 13.2555 + 0.5);
   last_k = (int)((last_year - 1999.97) * 13.2555 + 0.5);
   for (k = first_k ; k <= last_k; ++k) {
      jde = get_perigee(k);
      j_cent = (jde - J_DAY_1900_PRE_NOO) / D_IN_C ;
      find_parameters(j_cent, &l_sunt, &l_earth, &au_earth, &l_moont, &theta);
      phase_of_moon = norm_angle(l_moont - l_sunt);
      printf("%s %f %f\n", 
             date_time_string(jde, dts), 
             perigee_value, 
             phase_of_moon);
   }
}

double
get_perigee(int k) {
   double t = (double)k / 1325.55;
   double coeff_array[5];
   coeff_array[0] = 2451534.6698 + 27.55454988 * k;
   coeff_array[1] = 0.0;
   coeff_array[2] = -0.0006886;
   coeff_array[3] = -0.000001098;
   coeff_array[4] = 0.0000000052;

   return poly_value(4, coeff_array, t);
}















