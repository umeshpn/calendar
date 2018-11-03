/* PANCHANG */

#include "ast.h"

void print_panjang (FILE *fp, double j_day, int n_days, const char *time_zone) ;
void print_details(FILE *fp, double j_day, int curr_day, ST_PL_DET pl_det[]) ;
void print_month_head (FILE *fp, int curr_year, int curr_month, const char *time_zone) ;
char *conv_to_str(double longitude, char *s) ;
void get_date(double curr_j_date,int *curr_year, int *curr_month,
              int *curr_day);

double time_diff;
enum { YES, NO } dls_flag;

static char *hm_string(double j_day,char *any_str)
{
   struct date dd ;
   struct time tt ;

   get_date_time(j_day, &dd, &tt);
   sprintf(any_str,"%02d:%02d", tt.ti_hour,tt.ti_min);
   return(any_str);
}

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
      time_diff = -6.0;
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
   /* jul_day = julian_day(&dd, 12.0 - time_diff); */

   jul_day = julian_day(&dd, 0.0 - time_diff);
   if ((fp = fopen("PANJANG.PRT","w")) == (FILE *) 0 )
   {
      printf("Cannot open PANJANG.PRT\n");
      exit (1) ;
   }
   print_panjang(fp,jul_day, no_of_days, time_zone) ;
   fclose (fp) ;
	return 0;
}

void print_panjang (FILE *fp, double j_day, int n_days, const char *time_zone)
{
   int prev_month = 0 ;
   double curr_j_date = j_day ;
   int curr_year, curr_month, curr_day ;
   ST_DT det ;
   ST_PL_DET pl_det[15] ;

   while (n_days--)
   {
      find_e_and_ayanamsa(curr_j_date) ;
      get_date(curr_j_date + time_diff/24.0, &curr_year, &curr_month, &curr_day);
      if (curr_month != prev_month )
      {
         if (prev_month)
            under_line(fp,128) ;
         print_month_head (fp, curr_year, curr_month, time_zone);
      }
      full_details(curr_j_date, pl_det);
      print_details(fp, curr_j_date, curr_day, pl_det);
      curr_j_date += 1.0 ;
      prev_month = curr_month ;
   }
   under_line(fp,128) ;
}

void get_date(double curr_j_date,int *curr_year, int *curr_month,
              int *curr_day)
{
   struct date dd ;
   struct time tt ;

   get_date_time(curr_j_date, &dd, &tt);
   *curr_year = dd.da_year ;
   *curr_month = dd.da_mon ;
   *curr_day = dd.da_day ;
}

static char* get_naks_string(int nak, char* naks_str)
{
   sprintf(naks_str, "%2d ", nak);
   /* strcpy(naks_str, nak_name[nak]);*/
   strcat(naks_str, nak_name[nak]);
   strcat(naks_str, "/");
   strcat(naks_str, nak_name[(nak+1)%27]);
   return naks_str;
}

static char* get_thithis_string(int thi, char* thith_str)
{
   sprintf(thith_str, "%02d ", thi);
   strcat(thith_str, thithi_name[thi]);
   strcat(thith_str, "/");
   strcat(thith_str, thithi_name[(thi+1)%15]);
   return thith_str;
}

void print_details(FILE *fp, double j_day, int curr_day, ST_PL_DET pl_det[])
{
   int i ;
   char z_str[15] ;       
   double hind_long, d_nak, d_thithi, sun_moon_diff, moon_long, sun_long ;
   int nak, paksham, thithi ;
   char aya_str[20] ;
   END_VALUES nak_ev, thi_ev;
   char nak_end[10], nak_end1[10], thi_end[10], thi_end1[10];
   char naks_str[30], naks_str1[30], thithi_str[30], thithi_str1[30];

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

   printf("\n\nday = %d\n", curr_day);
   find_naks_in_day(j_day, &nak_ev);
   strcpy(nak_end, "");
   strcpy(naks_str, "");
   strcpy(thi_end, "");
   strcpy(thithi_str, "");
   strcpy(nak_end1, "");
   strcpy(naks_str1, "");
   strcpy(thi_end1, "");
   strcpy(thithi_str1, "");
   if (nak_ev.n_values > 0) {
      hm_string(nak_ev.values[0].end_j_time + time_diff/24.0, nak_end);
      get_naks_string(nak, naks_str);
      if (nak_ev.n_values > 1) {
         hm_string(nak_ev.values[1].end_j_time + time_diff/24.0, nak_end1);
         get_naks_string(nak_ev.values[1].value, naks_str1);
      } else {
         strcpy(nak_end1, "");
         strcpy(naks_str1, "");
      }
   } else {
      strcpy(nak_end, "");
      strcpy(naks_str, nak_name[nak]);
   }

   find_thiths_in_day(j_day, &thi_ev);
   if (thi_ev.n_values > 0) {
      hm_string(thi_ev.values[0].end_j_time + time_diff/24.0, thi_end);
      get_thithis_string(thithi, thithi_str);
     if (thi_ev.n_values > 1) {
         hm_string(thi_ev.values[1].end_j_time + time_diff/24.0, thi_end1);
         get_thithis_string(thi_ev.values[1].value, thithi_str1);
      } else {
         strcpy(thi_end1, "");
         strcpy(thithi_str1, "");
      }
   } else {
      strcpy(thi_end, "");
      strcpy(thithi_str, thithi_name[thithi]);
   }

   fprintf (fp, "  %2d   %-15s%-28s (%6s)   %-15s%-28s (%6s)%20s\n",
            curr_day, day_name[weekday(j_day + time_diff / 24.0)],
            naks_str, nak_end,
            paksha_name[paksham],
            thithi_str, thi_end, aya_str);

   if ((nak_ev.n_values) > 1 || (thi_ev.n_values) > 1) {
      fprintf (fp, "  %2s   %-15s%-28s (%6s)   %-15s%-28s (%6s)\n",     
               "", "", naks_str1, nak_end1, "", thithi_str1, thi_end1);
   }
}

void print_month_head (FILE *fp, int curr_year, int curr_month, const char *time_zone)
{
   fputc(12,fp) ;
   if (YES == dls_flag) 
      fprintf(fp,"\n\n            PANJANG FOR %s %04d  at 12 noon %s ( 1 PM during Daylight savings time )\n",
              mnth_name[curr_month - 1 ],curr_year, time_zone );
   else
      fprintf(fp,"\n\n                            PANJANG FOR %s %04d  at 12 noon %s\n",
              mnth_name[curr_month - 1 ],curr_year, time_zone );
   
   under_line(fp,128);
   fprintf(fp, "  Day     VARAM        NAKSHATRAM                PAKSHAM         THITHI                           AYANAMSAM\n");
   under_line(fp,128);
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
   z = (int)(longitude / 30.0) ;
   d = longitude - 30.0 * z ;
   time_string(d, DEGREE, PART, temp_str);
   sprintf(s,"* %c%c%c%s",zod_name[z][0], zod_name[z][1],zod_name[z][2],
           temp_str) ;
   return s ;
}



























