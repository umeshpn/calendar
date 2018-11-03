/*  HS.C  */


#include "ast.h"

int quadrant(double theta)
{
   return ((int)(norm_angle(theta)/90)) ;
}

double sin_correction(double theta, double mult_fact)
{
   double new_theta ;

   new_theta = asin(d_sin(theta) * mult_fact) * D_IN_R ;
   return (new_theta);
}
double tan_correction(double theta, double mult_fact)
{
   double new_theta ;

   if (fabs(theta - 90) < 0.00001)
      new_theta = 90.0 ;
   else if (fabs(theta-270) < 0.00001)
      new_theta = 270.0 ;
   else
   {
      new_theta = norm_angle(atan(d_tan(theta) * mult_fact) * D_IN_R) ;
      if (quadrant(theta) != quadrant(new_theta))
         new_theta = norm_angle( HALF_CIRCLE + new_theta) ;
   }
   return(new_theta) ;
}

double mh_from_st_ecl(double st, double ecl)
{
   return(tan_correction( 15.0 * st, 1.0 / d_cos(ecl)));
}

double first_from_lat_st_ecl(double lat, double st, double ecl)
{
   int i ;
   double d_st, sda = 90.0, cst, lng, dcl, ad ;

   d_st = 15.0 * st ;
   for (i = 0 ; i < 5 ; i++)
   {
      cst = d_st + sda ;
      lng = tan_correction(cst,1.0/d_cos(ecl));
      dcl = sin_correction(lng,d_sin(ecl));  
      ad = D_IN_R * asin(d_tan(lat) * d_tan(dcl)) ;
      sda = 90.0 + ad ;
   }
   return(lng);
}

double second_from_lat_st_ecl(double lat, double st, double ecl)
{
   int i ;
   double d_st, sda = 90.0, cst, lng, dcl, ad  ;

   d_st = 15.0 * st ;
   for (i = 0 ; i < 5 ; i++)
   {
      cst = d_st + 2.0 / 3.0 * sda + 60.0;
      lng = tan_correction(cst,1.0/d_cos(ecl));
      dcl = sin_correction(lng,d_sin(ecl));  
      ad = D_IN_R * asin(d_tan(lat) * d_tan(dcl)) ;
      sda = 90.0 + ad ;
   }
   return(lng);
}

double third_from_lat_st_ecl(double lat, double st, double ecl)
{
   int i ;
   double d_st, sda = 90.0 , cst, lng, dcl, ad  ;

   d_st = 15.0 * st ;
   for (i = 0 ; i < 5 ; i++)
   {
      cst = d_st + sda / 3.0 + 120.0;
      lng = tan_correction(cst,1.0/d_cos(ecl));
      dcl = sin_correction(lng,d_sin(ecl));
      ad = D_IN_R * asin(d_tan(lat) * d_tan(dcl)) ;
      sda = 90.0 + ad ;
   }
   return(lng);
}

double eleventh_from_lat_st_ecl(double lat, double st, double ecl)
{
   int i ;
   double d_st, sda = 90.0, cst, lng, dcl, ad  ;

   d_st = 15.0 * st ;
   for (i = 0 ; i < 5 ; i++)
   {
      cst = d_st + sda / 3.0 ;
      lng = tan_correction(cst,1.0/d_cos(ecl));
      dcl = sin_correction(lng,d_sin(ecl));  
      ad = D_IN_R * asin(d_tan(lat) * d_tan(dcl)) ;
      sda = 90.0 + ad ;
   }
   return(lng);
}

double twelfth_from_lat_st_ecl(double lat, double st, double ecl)
{
   int i ;
   double d_st, sda = 90.0, cst, lng, dcl, ad ;

   d_st = 15.0 * st ;
   for (i = 0 ; i < 5 ; i++)
   {
      cst = d_st + (2.0/3.0) * sda ;
      lng = tan_correction(cst,1.0/d_cos(ecl));
      dcl = sin_correction(lng,d_sin(ecl));  
      ad = D_IN_R * asin(d_tan(lat) * d_tan(dcl)) ;
      sda = 90.0 + ad ;
   }
   return(lng);
}

void get_house_degrees(double ecl, ABD b_info, double hs[])
{
   double lat,st ;

   lat = b_info -> latitude ;
   st = local_sidereal_time(b_info->j_date,b_info->longitude);
   hs[9] = mh_from_st_ecl(st,ecl) ;
   hs[0] = first_from_lat_st_ecl(lat,st,ecl);
   hs[1] = second_from_lat_st_ecl(lat,st,ecl);
   hs[2] = third_from_lat_st_ecl(lat,st,ecl);
   hs[10] = eleventh_from_lat_st_ecl(lat,st,ecl);
   hs[11] = twelfth_from_lat_st_ecl(lat,st,ecl);
   hs[3] = norm_angle(hs[9] + HALF_CIRCLE);
   hs[4] = norm_angle(hs[10] + HALF_CIRCLE);
   hs[5] = norm_angle(hs[11] + HALF_CIRCLE);
   hs[6] = norm_angle(hs[0] + HALF_CIRCLE);
   hs[7] = norm_angle(hs[1] + HALF_CIRCLE);
   hs[8] = norm_angle(hs[2] + HALF_CIRCLE);
}

double get_lagnam(double j_day, double latitude, double longitude)
{
   double st = local_sidereal_time(j_day, longitude);
   double lag = norm_angle(first_from_lat_st_ecl(latitude, st, get_e(j_day)) - get_ayanamsa(j_day));
   return lag;
}

