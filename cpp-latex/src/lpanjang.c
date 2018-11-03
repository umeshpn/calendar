/* PANCHANG */

#include "ast.h"

void print_panjang (FILE *fp, double j_day, int n_days, const char *time_zone, double latitude, double longitude) ;
void print_details(FILE *fp, double j_day, int curr_day, ST_PL_DET pl_det[], double latitude, double longitude) ;
char *conv_to_str(double longitude, char *s) ;
void get_date(double curr_j_date,int *curr_year, int *curr_month, int *curr_day);

double time_diff;
enum { YES, NO } dls_flag;

static char* lday_name[] = {
   "<m>n~aayaR</m>",
   "<m>tin\"kaL</m>",
   "<m>covva</m>",
   "<m>budhan</m>",
   "<m>vyaazhaM</m>",
   "<m>veLLi</m>",
   "<m>shani</m>"
};

static char* mmonths[] = {
   "<m>meeTa</m>",
   "<m>iTava</m>",
   "<m>mithuna</m>",
   "<m>kaRkkaTaka</m>",
   "<m>cin\"n\"a</m>",
   "<m>kanni</m>",
   "<m>tulaa</m>",
   "<m>v.rshcika</m>",
   "<m>dhanu</m>",
   "<m>makara</m>",
   "<m>kuMbha</m>",
   "<m>miina</m>",
};

static char* mmmonths[] = {
   "<m>meeTaM</m>",
   "<m>iTavaM</m>",
   "<m>mithunaM</m>",
   "<m>kaRkkaTakaM</m>",
   "<m>cin\"n\"aM</m>",
   "<m>kanni</m>",
   "<m>tulaaM</m>",
   "<m>v.rshcikaM</m>",
   "<m>dhanu</m>",
   "<m>makaraM</m>",
   "<m>kuMbhaM</m>",
   "<m>miinaM</m>",
};

static char *disp_time_string(double j_day, char *any_str)
{
   struct date dd ;
   struct time tt ;
   char am_pm;

   get_date_time(j_day, &dd, &tt);

   am_pm = (tt.ti_hour > 11 ? 'P' : 'A');
   if (tt.ti_hour > 12) {
      tt.ti_hour -= 12;
   } else {
      if (tt.ti_hour == 0){
         tt.ti_hour = 12;
      }
   }
   
   sprintf(any_str,"%04d/%02d/%02d  %02d:%02d %cM",dd.da_year,dd.da_mon, dd.da_day,
            tt.ti_hour,tt.ti_min, am_pm);
   return(any_str);
}

static char *hm_string(double j_day,char *any_str)
{
   struct date dd ;
   struct time tt ;
   char am_pm;
   int h, m;

   get_date_time(j_day, &dd, &tt);

   h = tt.ti_hour;
   m = tt.ti_min;

   if ( h < 12 ) {
      am_pm = 'A';
      if (0 == h) {
         h = 12;
      }
      sprintf(any_str,"{\\tt %2d:%02d A}", h, m, am_pm);
   } else {
      am_pm = 'P';
      if (h > 12) {
         h -= 12;
      }
      sprintf(any_str,"{\\tt %2d:%02d P}", h, m, am_pm);
   }

   return(any_str);
}

#if 0
int main (int argc, char *argv[] )
{
   FILE *fp ;
   double jul_day ;
   int start_year, start_month, start_day, no_of_days ;
   struct date dd ;
   char *time_zone;
   double latitude, longitude;

   if (argc != 8 )
   {
      printf("\n Usage : %s Year Month(1-12) Day(1-31) Number_of_days(1-10000) Timezone(PST, IST etc) Latitude Longitude\n", argv[0]);
      exit (1) ;
   }
   start_year = atoi(argv[1]) ;
   start_month = atoi(argv[2]) ;
   start_day = atoi(argv[3]) ;
   no_of_days = atoi(argv[4]) ;
   time_zone = argv[5];
   latitude = atof(argv[6]);
   longitude = atof(argv[7]);
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
      time_diff = -7.0;
      dls_flag = YES;
   }
   else if  (strcmp(time_zone, "CST") == 0) 
   {
      time_diff = -6.0;
      dls_flag = YES;
   }
   else if  (strcmp(time_zone, "EST") == 0) 
   {
      time_diff = -5.0;
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
   if ((fp = fopen("p.mm","w")) == (FILE *) 0 )
   {
      printf("Cannot open p.mm\n");
      exit (1) ;
   }
   print_panjang(fp,jul_day, no_of_days, time_zone, latitude, longitude) ;
   fclose (fp) ;
	return 0;
}
#endif

static void print_month_head (FILE *fp, int curr_year, int curr_month, const char *time_zone, double j_day)
{
   int mal_year, mal_month1, mal_month2;
   double transit_j_day;
   char t_str[50];
   fprintf(fp, "\\clearpage\n");
   fprintf(fp, "\\centering{\\huge %s %04d (%s) \\normalsize}\\par\n", mnth_name[curr_month - 1], curr_year, time_zone);
   if (curr_month < 8) {
      mal_year = curr_year - 825;
   } else {
      mal_year = curr_year - 824;
   }
   mal_month1 = (curr_month + 7) % 12;
   mal_month2 = (mal_month1 + 1 ) % 12;
   if (mal_month2 == 4) {
      fprintf(fp, "\\centering{%d %s - %d %s}\\par\n", mal_year-1, mmmonths[mal_month1], 
              mal_year, mmmonths[mal_month2]);
   } else {
      fprintf(fp, "\\centering{%d %s - %s}\\par\n", mal_year, mmmonths[mal_month1], 
              mmmonths[mal_month2]);
   }
   next_sun_transit(j_day, &transit_j_day);
   disp_time_string(transit_j_day + time_diff/24.0, t_str);
   fprintf(fp,  "\\centering{%s%s : %s}\\par\n", mmonths[mal_month2], "<m>saMkramaM</m>", t_str);
   /* fprintf(fp, "\\begin{table}\n"); */
   fprintf(fp, "\\begin{tabular}{|l|l r l|l r l|c|c|}\n");
   fprintf(fp, "\\hline\n");
   fprintf(fp, "\\Day & \\Nakshatram  && & \\Thithi &&& \\Udayaastamayam \\\\\n");
   fprintf(fp, "\\hline\n");

}

static void print_month_tail(FILE *fp, int curr_year, int curr_month, const char *time_zone)
{ 
   fprintf(fp, "\\hline\n");
   fprintf(fp, "\\end{tabular}\n");
   /*
   if (YES == dls_flag) 
      fprintf(fp, "\\caption{PANJANG FOR %s %04d  at 12 noon %s ( 1 PM during Daylight savings time )}\n",
              mnth_name[curr_month - 1 ],curr_year, time_zone );
   else
      fprintf(fp,"\\caption{PANJANG FOR %s %04d  at 12 noon %s}\n",
              mnth_name[curr_month - 1 ],curr_year, time_zone );

   */
   /*fprintf(fp, "\\end{table}\n");*/
}

void print_panjang (FILE *fp, double j_day, int n_days, const char *time_zone, double latitude, double longitude)
{
   int prev_month = 0 ;
   double curr_j_date = j_day ;
   int curr_year, curr_month, curr_day ;
   ST_DT det ;
   ST_PL_DET pl_det[15] ;

   fprintf(fp, "\\documentclass{article}\n");
   fprintf(fp, "\\include{panjang_macros}\n");
   fprintf(fp, "\\begin{document}\n");
   fprintf(fp, "\\usepackage{auto-pst-pdf}\n");
   fprintf(fp, "\\include{panjang_mukh}\n");   
   while (n_days--)
   {
      printf("%d\n", n_days);
      find_e_and_ayanamsa(curr_j_date) ;
      get_date(curr_j_date + time_diff/24.0, &curr_year, &curr_month, &curr_day);
      if (curr_month != prev_month )
      {
         if (prev_month)
            print_month_tail (fp, curr_year, curr_month, time_zone);
         print_month_head (fp, curr_year, curr_month, time_zone, curr_j_date);
      }
      full_details(curr_j_date, pl_det);
      print_details(fp, curr_j_date, curr_day, pl_det, latitude, longitude);
      curr_j_date += 1.0 ;
      prev_month = curr_month ;
   }
   print_month_tail (fp, curr_year, curr_month, time_zone);
   fprintf(fp, "\\include{panjang_app}\n");   
   fprintf(fp, "\\end{document}\n");
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

static char* get_naks_string(int nak, double j_day, int left, char* naks_str)
{
   char str[20];
   char na_name[15];
   int nak1;
   // sprintf(na_name, "\\Star%c", ((nak < 26) ? 'A' + nak : 'a' + nak-26));
   // strcpy(naks_str, na_name);
   if (nak < 0) {
      strcpy(naks_str, " & & ");
   } else if (j_day < 0.0) {
      sprintf(na_name, "\\Star%c", ((nak < 26) ? 'A' + nak : 'a' + nak-26));
      strcpy(naks_str, "{\\bf ");
      strcat(naks_str, na_name);
      strcat(naks_str, "}");
      strcat(naks_str, "& &");
   } else {
      sprintf(na_name, "\\Star%c", ((nak < 26) ? 'A' + nak : 'a' + nak-26));
      if (left) {
         strcpy(naks_str, "{\\bf ");
      }
      strcat(naks_str, na_name);
      if (left) {
         strcat(naks_str, "}");
      }
      strcat(naks_str, " & ");
      hm_string(j_day, str);
      strcat(naks_str, str);
      strcat(naks_str, " & ");
      nak1 = (nak+1)%27;
      sprintf(na_name, "\\Star%c", ((nak1 < 26) ? 'A' + nak1 : 'a' + nak1-26) );
      if (!left) {
         strcat(naks_str, "{\\bf ");
      }
      strcat(naks_str, na_name);
      if (!left) {
         strcat(naks_str, "}");
      }
   }
   return naks_str;
}

static char* get_thithis_string(int thi, double j_day, int left, char* thith_str)
{
   char str[20];
   char th_name[15];
   int thi1;
   if (thi < 0) {
      strcpy(thith_str, " & & ");
   } else if (j_day < 0.0) {
      sprintf(th_name, "\\Thithi%c", ((thi < 26) ? 'A' + thi : 'a' + thi-26));
      strcpy(thith_str, "{\\bf ");
      strcat(thith_str, th_name);
      strcat(thith_str, "}");
      strcat(thith_str, "& &");
   } else {
      sprintf(th_name, "\\Thithi%c", ((thi < 26) ? 'A' + thi : 'a' + thi-26));
      if (left) {
         strcpy(thith_str, "{\\bf ");
      }
      strcat(thith_str, th_name);
      if (left) {
         strcat(thith_str, "}");
      }
      strcat(thith_str, " & ");
      hm_string(j_day, str);
      strcat(thith_str, str);
      strcat(thith_str, " & ");
      thi1 = (thi+1)%30;
      sprintf(th_name, "\\Thithi%c", ((thi1 < 26) ? 'A' + thi1 : 'a' + thi1-26));
      if (!left) {
         strcat(thith_str, "{\\bf ");
      }
      strcat(thith_str, th_name);
      if (!left) {
         strcat(thith_str, "}");
      }
   }
   return thith_str;
}


void print_details(FILE *fp, double j_day, int curr_day, ST_PL_DET pl_det[], double latitude, double longitude)
{
   int i ;
   char z_str[15] ;       
   double hind_long, d_nak, d_thithi, sun_moon_diff, moon_long, sun_long ;
   int nak, paksham, thithi ;
   char aya_str[20] ;
   END_VALUES nak_ev, thi_ev;
   char naks_str[50], naks_str1[50], thithi_str[50], thithi_str1[50];
   char udayam_str[20], astamayam_str[20];
   double udayam, astamayam;

   time_string(ayanamsa,DEGREE, FULL, aya_str) ;
   moon_long = norm_angle(pl_det[MOON].geo_long - ayanamsa) ;
   sun_long = norm_angle(pl_det[SUN].geo_long - ayanamsa);
   
   d_nak = nakshatra (moon_long);
   nak = (int) d_nak ;
   d_nak = (d_nak - nak) * 100.0 ;
   sun_moon_diff = norm_angle(moon_long - sun_long);

   /*
   paksham = (int) (sun_moon_diff / 180.0);
   d_thithi = (sun_moon_diff - 180.0 * paksham) / 12.0 + 1.0 ;
   if (d_thithi > 15.0 )
      d_thithi -= 15.0 ;
   thithi = (int) d_thithi ;
   d_thithi = (d_thithi - thithi) * 100.0 ;
   */

   d_thithi = sun_moon_diff / 12.0;
   thithi = (((int) d_thithi) + 1 ) % 30 ;
   // printf("\n\nday = %d\n", curr_day);

   strcpy(naks_str, "");
   strcpy(thithi_str, "");
   strcpy(naks_str1, "");
   strcpy(thithi_str1, "");

   udayam = sunrise_between(j_day, j_day + 0.5, latitude, longitude);
   astamayam = sunset_between(j_day + 0.5, j_day + 1.0, latitude, longitude);
   find_naks_in_day(j_day, &nak_ev);
   
   if (nak_ev.n_values > 0) {
      int left = (udayam < nak_ev.values[0].end_j_time ? 1 : 0);
      get_naks_string(nak_ev.values[0].value, nak_ev.values[0].end_j_time + time_diff/24.0, left, naks_str);

      if (nak_ev.n_values > 1) {
         get_naks_string(nak_ev.values[1].value, nak_ev.values[1].end_j_time + time_diff/24.0, 0, naks_str1);
      } else {
         get_naks_string(-1, -1.0, 0, naks_str1);
      }
   } else {
      get_naks_string(nak, -1.0, 1, naks_str);
   }

   find_thiths_in_day(j_day, &thi_ev);
   if (thi_ev.n_values > 0) {
      int left = (udayam < thi_ev.values[0].end_j_time ? 1 : 0);
      get_thithis_string(thi_ev.values[0].value, thi_ev.values[0].end_j_time + time_diff/24.0, left, thithi_str);
     if (thi_ev.n_values > 1) {
         get_thithis_string(thi_ev.values[1].value, thi_ev.values[1].end_j_time + time_diff/24.0, 0, thithi_str1);
      } else {
         get_thithis_string(-1, -1.0, 0, thithi_str1);
      }
   } else {
      get_thithis_string(thithi, -1.0, 1, thithi_str);
   }


   hm_string(udayam + time_diff / 24.0, udayam_str);
   hm_string(astamayam + time_diff / 24.0, astamayam_str);

   fprintf (fp, "%2d/%s & %s &  %s & %s/%s\\\\\n",
            curr_day, lday_name[weekday(j_day + time_diff / 24.0)],
            naks_str, thithi_str, udayam_str, astamayam_str);



   if ((nak_ev.n_values) > 1 || (thi_ev.n_values) > 1) {
      fprintf (fp, "%s & %s  &  %s &\\\\\n",
               "", naks_str1, thithi_str1);
   }
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

