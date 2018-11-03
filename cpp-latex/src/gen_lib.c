/*                            ---------
                              GEN_LIB.C
                              ---------
            ( Routines for General mathematical Calculations )
              ----------------------------------------------
*/

#include "ast.h"

/*        -------------------------------------------------
          Routines to calculate the trigonometric functions
                    of angles expressed in degrees
                    ------------------------------
*/

double d_sin(double x)
{
   return(sin(x/D_IN_R));
}

double d_cos(double x)
{
   return(cos(x/D_IN_R));
}

double d_tan(double x)
{
   return(tan(x/D_IN_R));
}
/*             --------------------------------
               Routines to evaluate polynomials
               --------------------------------
*/
double linear(double a0,double a1,double x)
{
   return ( a0 + x * a1 ) ;
}

double quadratic(double a0,double a1,double a2,double x)
{
   return ( a0 + x * ( a1 + x * a2 )) ;
}

double cubic(double a0, double a1, double a2, double a3, double x)
{
   return ( a0 + x * ( a1 + x * ( a2 + x * a3 ))) ;
}

double poly_value(int n,double coeff[],double x)
{
   int i,j ;
   double value = coeff[n];

   for (i=1 ; i <= n ; i++)
   {
      j = n - i ;
      value *= x ;
      value += coeff[j] ;
   }
   return(value);
}

double poly_val1(int n,double coeff[],double x)
{
   switch (n)
   {
      case 1   : return (linear(coeff[0],coeff[1],x));
      case 2   : return (quadratic(coeff[0],coeff[1],coeff[2],x));
      case 3   : return (cubic(coeff[0],coeff[1],coeff[2],coeff[3],x));
   }
   return ( poly_value(n,coeff,x));
}
/*           -------------------------------
             Routines for time-manipulations
             -------------------------------
*/
double equiv_time (DMS dmtime)
{
   return (dmtime -> degrees 
           + (dmtime -> minutes 
              + dmtime -> seconds/ 60.0
              )/60.0
      );
}

DMS dms_time (double any_time,DMS dmtime)
{
   any_time = any_time * 3600.0 + 0.001 ;
   dmtime -> seconds = fmod(any_time,60.0) ;
   any_time = ( any_time - dmtime->seconds ) / 60.0 + 0.001;
   dmtime -> minutes = (int) ( fmod(any_time,60.0) ) ;
   any_time = ( any_time - dmtime->minutes ) / 60.0 + 0.001 ;
   dmtime -> degrees = (int) (any_time );
  
   return (dmtime);
}

char *dm_string(DMS dmtime,int time_or_deg,char *char_string)
{
   if (time_or_deg == TIME)
      sprintf(char_string,"%04dh %02dm",dmtime -> degrees, dmtime->minutes);
   else
      sprintf(char_string,"%3d:%02d",dmtime -> degrees, dmtime->minutes);
   return(char_string);
}

char *dms_string(DMS dmtime,int time_or_deg,char *char_string)
{
   if (time_or_deg == TIME)
      sprintf(char_string,"%4dh %02dm %05.2fs",dmtime -> degrees, 
              dmtime->minutes,dmtime->seconds);
   else
      sprintf(char_string,"%4d: %02d: %05.2f ",dmtime -> degrees, 
              dmtime->minutes,dmtime->seconds);
   return(char_string);
}

char *time_string(double any_time, int time_or_deg, int full,
                  char *char_string)
{
   ST_DMS *dmtime,dummy ;

   dmtime = dms_time (any_time,&dummy);
   if (full == FULL )
      dms_string (dmtime,time_or_deg,char_string);
   else  
      dm_string (dmtime,time_or_deg,char_string);
   return (char_string);
}

/*  --------------------------------------------------
    Function to find an angle from its sine and cosine
    --------------------------------------------------
*/
double sc_d_angle(double sin_x, double cos_x)
{
   double tan_x,angle ;

   tan_x = sin_x / cos_x ;
   angle = atan(tan_x) * D_IN_R ;   /*   Angle between -90 to +90   */
   if (sin_x >= 0)
   {
      if (cos_x >= 0)                /*   First Quadrant   */
         return (angle);
      else                           /*   Second Quadrant  */
         return (angle + HALF_CIRCLE) ;
   }
   else
   {
      if (cos_x >= 0)                /*   Fourth Quadrant  */
         return (angle + FULL_CIRCLE);
      else                           /*   Third Quadrant  */
         return (angle + HALF_CIRCLE) ;
   }
}

/* ---------------------------------------
   Functions to normalise angles and times
   ---------------------------------------
*/
double norm_angle(double any_angle)
{
   while ( any_angle >= FULL_CIRCLE )
      any_angle -= FULL_CIRCLE ;
   while ( any_angle < 0.0 )
      any_angle += FULL_CIRCLE ;
   return (any_angle);
}

double norm_longitude (double longitude)
{
   while ( longitude > HALF_CIRCLE )
      longitude -= FULL_CIRCLE ;
   while ( longitude <= - HALF_CIRCLE )
      longitude += FULL_CIRCLE ;
   return (longitude);

}

double norm_hour(double any_time)
{
   while ( any_time > H_IN_D )
      any_time -= H_IN_D ;
   while ( any_time < 0.0 )
      any_time += H_IN_D ;
   return (any_time);
}

/*  -------------------------------------------------
    Function to print underline and newline in a file
    -------------------------------------------------
*/
void under_line(FILE *fp,int n)
{
   int i;

   for ( i = 0 ; i < n ; i++)
      fputc('-',fp);
   fputc('\n',fp);
}

/*  ---------------------------------------------
    Function to print a string centered on a line
	 ---------------------------------------------
*/
void center_it_pw(FILE *fp, const char *any_string, int line_skip, int page_width)
{
   int indent,i ;

   indent = (page_width - strlen(any_string))/ 2 ;
   for (i = 0 ; i < indent ; i++)
      fputc(' ',fp);
   fputs(any_string,fp);
   for ( i = 0  ; i <= line_skip ; i++)
      fputc('\n',fp);
}
