/*   STA.C   */
                     
#include "ast.h"
#if !MAKE_FILE
#  include "ast_data.c"
#  include "gen_lib.c"
#  include "ast_lib.c"
#  include "hs.c"
#  include "draw_ch.c"
#  include "title.c"
#endif

static int get_details(ST_DT *det);

int main(int argc, char *argv[])
{
   char filename[30],name[30];
   FILE *in_fp, *out_fp ;
   ST_DT det ;
   ST_ABD birth_info ;
   ST_PL_DET pl_det[15];
   ST_HP_POS hp_pos[30];
   int okay,yy,mm,dd,hh,mn,latd,latm,longd,longm,st_longd,st_longm ;
   int no,rec_no ;
   double aspects[14][14],hs[12],e;

   get_details(&det);
   printf("Output file : ");
   scanf("%s", filename);
   if (( out_fp = fopen(filename,"w")) == NULL)
   {
      printf("\n Cannot open %s\n",filename);
      exit(0);
   }
   find_birth_info(&det,&birth_info);
   full_details(birth_info.j_date,pl_det);
   e = find_e_and_ayanamsa(birth_info.j_date);
   get_house_degrees(e,&birth_info,hs);
   find_hp_pos(pl_det,hs,hp_pos);
   title_page(out_fp);
   print_birth_details(out_fp,&det);
   print_planet_details(out_fp,pl_det);
   print_hp_pos(out_fp,hp_pos);
   get_aspects(pl_det,pl_det,SAME,aspects);
   print_self_aspects(out_fp,aspects);
   hindu_details(out_fp,&birth_info,pl_det,hs);

   fprintf(out_fp,"\n\n");
   fclose(out_fp);
}

static int get_details(ST_DT *det)
{
   int deg, min;
	float time_zone;
	char north_or_south, east_or_west;
   printf("\n\n Name : ");
   gets(det->name);
   printf("Birthday: \n");
   printf("\tYear: ");
   scanf("%d", &det->year);
   printf("\tMonth: ");
   scanf("%d", &det->month);
   printf("\tDay: ");
   scanf("%d", &det->day);
   printf("\tHour: ");
   scanf("%d", &det->hour);
   printf("\tMinute: ");
   scanf("%d", &det->minute);
   det->second = 0.0 ;

   printf("\nPlace:\n");

   printf("\tLatitude:\n");
   printf("\t\tDegrees: ");
   scanf("%d", &deg);
   printf("\t\tminute: ");
   scanf("%d", &min);
   north_or_south = ' ';
   while (north_or_south != 'E' && east_or_west != 'W') {
      printf("\t\tNorth or South (N/S) : "); 
      scanf("%c", &north_or_south);
      north_or_south = toupper(north_or_south);
   }
   det->latitude = deg + min / 60.0 ;
   if (north_or_south == 'S') {
      det->latitude *= -1;
   }

   printf("\tLongitude:\n");
   printf("\t\tDegrees: ");
   scanf("%d", &deg);
   printf("\t\tminute: ");
   scanf("%d", &min);
   east_or_west = ' ';
   while (east_or_west != 'E' && east_or_west != 'W') {
      printf("\t\t East or West (E/W) : ");
      scanf("%c", &east_or_west);
      east_or_west = toupper(east_or_west);
   }
   det->longitude = deg + min / 60.0 ;
   if (east_or_west == 'E') {
      det->longitude *= -1;
   }
	 
   printf("Timezone(-12 to +12) : ");
   scanf("%f", &time_zone);
   det->st_long = time_zone * 15.0;

   return 0;
}
