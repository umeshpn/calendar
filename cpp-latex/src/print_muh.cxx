//
// print_muh.cxx
// (Prints details of muhoorththams for given input)
//


// Input file looks like this:

//    <date>  <time> <latitude> <longitude>   <std_longitude> 
// 21/12/2005   9:0    40:15      120:17        120:0

// Output is in Malayalam LaTeX , giving the following details:

// Date, time
// Time of sunrise and sunset, madhyaahnam and abhijith
// udayaalppara naazhika of the time.
// thithhi, vaaram, nakshathram, yOgam karaNam
// grahanila

#include "ast.h"

void lprint_birth_details(FILE *fp,DT b_det);
void lprint_planet_details(FILE *fp,ST_PL_DET pl_details[]);
void lprint_hp_pos (FILE *fp,HP_POS hp_pos);
void lprint_self_aspects (FILE *fp, ST_PL_DET pl_details[]);
void lhindu_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                    double hs[]);
void lmuh_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                  double hs[]);

extern int indian;


int print_muh_for_line(FILE *in_fp, FILE* out_fp)
{
   double hs[12],e;
   int yy,mm,dd,hh,mn,latd,latm,longd,longm,st_longd,st_longm ;
   int st = fscanf(in_fp,"%d/%d/%d,%d:%d,%d:%d,%d:%d,%d:%d",
                   &dd,&mm,&yy,&hh,&mn,&latd,&latm,&longd,&longm,&st_longd,&st_longm);
   printf("%d\n", st);
   if (st == EOF) {
      return st;
   }
   if (st != 11) {
      return 0;
   }

   ST_DT det ;
   ST_ABD birth_info ;
   ST_PL_DET pl_det[15];
   ST_HP_POS hp_pos[30];
   det.year = yy ; det.month = mm ; det.day = dd ;
   det.hour = hh ; det.minute = mn ;
   det.second = 0.0 ;
   det.latitude = latd + latm / 60.0 ;
   det.longitude = longd + longm / 60.0 ;
   det.st_long = st_longd + st_longm / 60.0 ;
   find_birth_info(&det,&birth_info);
   full_details(birth_info.j_date,pl_det);
   e = find_e_and_ayanamsa(birth_info.j_date);
   get_house_degrees(e,&birth_info,hs);
   find_hp_pos(pl_det,hs,hp_pos);
   indian = 1;
   lmuh_details(out_fp,&birth_info,pl_det,hs);

   return st;
}

int main(int argc, char *argv[])
{
   char filename[30];
   FILE *in_fp, *out_fp ;
   if (argc < 2)
      exit(1);
   strcpy(filename,argv[1]);
   if ((in_fp = fopen(filename,"r")) == NULL)
   {
      printf("\n cannot open %s\n",filename);
      exit(1);
   }
   if ( argc == 2)
      out_fp = stdout ;
   else
   {
      strcpy(filename,argv[2]);
      if (( out_fp = fopen(filename,"w")) == NULL)
      {
         printf("\n Cannot open %s\n",filename);
         exit(0);
      }
   }

   while (print_muh_for_line(in_fp, out_fp) != EOF) 
      ;


   return 0;
}
