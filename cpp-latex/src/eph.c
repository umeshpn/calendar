                    /*  EPH.C  */
                     
#include "ast.h"
#include "ast_data.c"
#include "gen_lib.c"
#include "ast_lib.c"
#include "draw_ch.c"

void print_eph (FILE *fp, double j_day, int n_days) ;
void print_details(FILE *fp, int curr_day, ST_PL_DET pl_det[]) ;
void print_month_head (FILE *fp, int curr_year, int curr_month) ;
char *conv_to_str(double longitude, char *s) ;
void get_date(double curr_j_date,int *curr_year, int *curr_month, 
         int *curr_day);

void main (int argc, char *argv[] )
{
  FILE *fp ;
  double jul_day ;
  int start_year, start_month, start_day, no_of_days ;
  struct date dd ;

  if (argc != 5 )
  {
    printf("\n Usage : %s Year Month(1-12) Day(1-31) Number_of_days(1-10000)\n", argv[0]);
    exit (1) ;
  }
  start_year = atoi(argv[1]) ;
  start_month = atoi(argv[2]) ;
  start_day = atoi(argv[3]) ;
  no_of_days = atoi(argv[4]) ;
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
  dd.da_year = start_year ;
  dd.da_mon = start_month ;
  dd.da_day = start_day ;
  jul_day = julian_day(&dd, 16.0);
  if ((fp = fopen("XXXX.PRT","w")) == NULL )
  {
    printf("Cannot open XXXX.PRT\n");
    exit (1) ;
  }
  print_eph(fp,jul_day, no_of_days ) ;
  fclose (fp) ;
}
void print_eph (FILE *fp, double j_day, int n_days)
{
  int prev_month = 0 ;
  double curr_j_date = j_day ;
  int curr_year, curr_month, curr_day ;
  ST_DT det ;
    ST_PL_DET pl_det[15] ;

  while (n_days--)
  {
    find_e_and_ayanamsa(curr_j_date) ;
    get_date(curr_j_date,&curr_year, &curr_month, &curr_day);
    if (curr_month != prev_month )
    {
      if (prev_month)
       under_line(fp,128) ;
      print_month_head (fp, curr_year, curr_month);
    }
    full_details(curr_j_date,pl_det);
    print_details(fp, curr_day, pl_det);
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
void print_details(FILE *fp, int curr_day, ST_PL_DET pl_det[])
{
  int i ;
  char z_str[15] ;       
  double hind_long ;

  fprintf(fp, "  %2d  ", curr_day) ;
  for ( i = SUN ; i <= PLUTO ; i++ )
  {  
    hind_long = pl_det[i].geo_long - ayanamsa;
    conv_to_str(hind_long, z_str) ;
    fprintf(fp, "%10s%c", z_str,pl_det[i].direction);
  }
  fprintf(fp,"  \n");
}
void print_month_head (FILE *fp, int curr_year, int curr_month)
{
   char aya_str[20] ;

   time_string(ayanamsa,DEGREE, FULL, aya_str) ;
   fputc(12,fp) ;
   fprintf(fp,"\n\n               EPHERMERIS FOR %s %04d  (at 12 noon EDT), ",
       mnth_name[curr_month - 1 ],curr_year );
   fprintf(fp, "        Ayanamsa = %s\n\n\n", aya_str);
   
   under_line(fp,128);
   fprintf(fp, "  Day     SUN         MOO         MER         VEN     ");
   fprintf(fp, "    MAR         JUP         SAT         URA     ");
   fprintf(fp, "    NEP         PLU     \n");
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
  z = longitude / 30.0 ;
  d = longitude - 30.0 * z ;
  time_string(d, DEGREE, PART, temp_str);
  sprintf(s,"* %c%c%c%s",zod_name[z][0], zod_name[z][1],zod_name[z][2],
         temp_str) ;
  return s ;
}

