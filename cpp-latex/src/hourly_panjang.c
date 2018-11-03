/* PANCHANG */

#include "ast.h"

void print_panjang (FILE *fp, double j_day, int n_days, const char *time_zone) ;
void print_details(FILE *fp, double j_day, int curr_day, int curr_hour, int curr_minute, ST_PL_DET pl_det[]) ;
void print_month_head (FILE *fp, int curr_year, int curr_month, const char *time_zone) ;
char *conv_to_str(double longitude, char *s) ;
void get_date(double curr_j_date,int *curr_year, int *curr_month,
              int *curr_day, int *curr_hour, int *curr_minute);

double time_diff;
enum { YES, NO } dls_flag;

int prev_day, prev_nak, prev_thithi, prev_paksham;

int main (int argc, char *argv[] )
{
   FILE *fp ;
   double jul_day ;
   int start_year, start_month, start_day, no_of_days ;
   struct date dd ;
   char *time_zone;

   if (argc != 6 )
   {
      printf("\n Usage : %s Year Month(1-12) Day(1-31) Number_of_days(1-10000) Timezone(PST, IST etc)\n", argv[0]);
      exit (1) ;
   }
   start_year = atoi(argv[1]) ;
   start_month = atoi(argv[2]) ;
   start_day = atoi(argv[3]) ;
   no_of_days = atoi(argv[4]) ;
   time_zone = argv[5];
   if (start_year == 0 )
   {
      printf("\n Error in Start Year\n");
      exit (1) ;
   }
   if (start_month < 1 || start_month > 12 )
   {
      printf("\n Error in Start Month\n");
      exit (1) ;
   }
   if (start_day < 1 || start_month > 31 )
   {
      printf("\n Error in Start day\n");
      exit (1) ;
   }
   if (no_of_days < 1 || no_of_days > 10000 )
   {
      printf("\n Error in number of days\n");
      exit (1) ;
   }
   if (strcmp(time_zone, "IST") == 0) 
   {
      time_diff = 5.5;
      dls_flag = NO;
   } 
   else if  (strcmp(time_zone, "PST") == 0) 
   {
      time_diff = -8.0;
      dls_flag = YES;
   }
   else if  (strcmp(time_zone, "MST") == 0) 
   {
      time_diff = -5.0;
      dls_flag = YES;
   }
   else if  (strcmp(time_zone, "CST") == 0) 
   {
      time_diff = -5.0;
      dls_flag = YES;
   }
   else if  (strcmp(time_zone, "EST") == 0) 
   {
      time_diff = -4.0;
      dls_flag = YES;
   }
   else if  (strcmp(time_zone, "GST") == 0) 
   {
      time_diff = 0.0;
      dls_flag = YES;
   }
   else 
   {
      printf("\n Error in time zone\n");
      exit (1);
   }
   dd.da_year = start_year ;
   dd.da_mon = start_month ;
   dd.da_day = start_day ;
   jul_day = julian_day(&dd, time_diff);
   if ((fp = fopen("PANJANG.PRT","w")) == (FILE *) 0 )
   {
      printf("Cannot open PANJANG.PRT\n");
      exit (1) ;
   }
   print_panjang(fp,jul_day, no_of_days, time_zone ) ;
   fclose (fp) ;
	return 0;
}

void print_panjang (FILE *fp, double j_day, int n_days, const char *time_zone)
{
   int prev_month = 0 ;
   double curr_j_date = j_day + 0.000001;
   int curr_year, curr_month, curr_day, curr_hour, curr_minute ;
   ST_DT det ;
   ST_PL_DET pl_det[15] ;

   int n_hours = 24 * n_days;
   prev_day = prev_nak = prev_thithi = prev_paksham = -1;
   
   while (n_hours--)
   {
      find_e_and_ayanamsa(curr_j_date) ;
      get_date(curr_j_date, &curr_year, &curr_month, &curr_day, &curr_hour, &curr_minute);
      if (curr_month != prev_month )
      {
         if (prev_month)
            under_line(fp,128) ;
         print_month_head (fp, curr_year, curr_month, time_zone);
      }
      full_details(curr_j_date,pl_det);
      print_details(fp, curr_j_date, curr_day, curr_hour, curr_minute, pl_det);
      curr_j_date += 1.0 / 24.0;
      prev_month = curr_month ;
   }
   under_line(fp,128) ;
}

void get_date(double curr_j_date,int *curr_year, int *curr_month,
              int *curr_day, int *curr_hour, int *curr_minute)
{
   struct date dd ;
   struct time tt ;

   get_date_time(curr_j_date + time_diff/24.0, &dd, &tt);
   *curr_year = dd.da_year ;
   *curr_month = dd.da_mon ;
   *curr_day = dd.da_day ;
   *curr_hour = tt.ti_hour;
   *curr_minute = tt.ti_min;
}
void print_details(FILE *fp, double j_day, int curr_day, int curr_hour, int curr_minute, ST_PL_DET pl_det[])
{
   int i ;
   char z_str[15] ;       
   double hind_long, d_nak, d_thithi, sun_moon_diff, moon_long, sun_long ;
   int nak, paksham, thithi ;
   char aya_str[20] ;
   char curr_day_string[3];

   time_string(ayanamsa,DEGREE, FULL, aya_str) ;
   moon_long = norm_angle(pl_det[MOON].geo_long - ayanamsa) ;
   sun_long = norm_angle(pl_det[SUN].geo_long - ayanamsa);
   
   d_nak = nakshatra (moon_long);
   nak = (int) d_nak ;
   d_nak = (d_nak - nak) * 100.0 ;
   sun_moon_diff = norm_angle(moon_long - sun_long);
   paksham = (int) (sun_moon_diff / 180.0);
   d_thithi = (sun_moon_diff - 180.0 * paksham) / 12.0 + 1.0 ;
   if (d_thithi > 15.0 )
      d_thithi -= 15.0 ;
   thithi = (int) d_thithi ;
   d_thithi = (d_thithi - thithi) * 100.0 ;
   if (curr_day == prev_day) {
      strcpy(curr_day_string, "  ");
   } else {
      sprintf(curr_day_string, "%2d", curr_day);
   }
   fprintf (fp, "  %2s  %02d:%02d   %-15s (%6.2f%%)   %-15s%-15s (%6.2f%%)\n",
            curr_day_string, 
            curr_hour, curr_minute,
            (nak == prev_nak) ? " " : nak_name[nak],
            d_nak,
            (paksham == prev_paksham) ? " " : paksha_name[paksham],
            (thithi == prev_thithi) ? " " : thithi_name[thithi], 
            d_thithi);
   prev_day = curr_day;
   prev_nak = nak;
   prev_thithi = thithi;
   prev_paksham = paksham;
}
void print_month_head (FILE *fp, int curr_year, int curr_month, const char *time_zone)
{
      fprintf(fp,"\n\n            PANJANG FOR %s %04d (All times in PST)\n",
              mnth_name[curr_month - 1 ],curr_year);
   
   under_line(fp,82);
   fprintf(fp, "  Day & Time  NAKSHATRAM                  PAKSHAM        THITHI\n");
   under_line(fp,82);
}

char *conv_to_str(double longitude, char *s)
{
   int z ;
   double d ;
   char temp_str[12] ;

   while (longitude >= FULL_CIRCLE)
      longitude -= FULL_CIRCLE ;
   while (longitude < 0.0 )
      longitude += FULL_CIRCLE ;
   z = (int) (longitude / 30.0) ;
   d = longitude - 30.0 * z ;
   time_string(d, DEGREE, PART, temp_str);
   sprintf(s,"* %c%c%c%s",zod_name[z][0], zod_name[z][1],zod_name[z][2],
           temp_str) ;
   return s ;
}



























