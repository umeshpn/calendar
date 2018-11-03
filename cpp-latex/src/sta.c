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

bool debugging = false;

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
   char dummy[30];

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
   printf("Enter the No. : ");
   scanf("%d",&rec_no);
   getchar();
   printf("\n\n");
   okay = 0 ;
   while (fscanf(in_fp,"%d,%d/%d/%d,%d:%d,%d:%d,%d:%d,%d:%d",&no,
                 &dd,&mm,&yy,&hh,&mn,&latd,&latm,&longd,&longm,&st_longd,&st_longm)
          != EOF)
   {
      printf("%d\n",no);
      if ( no > rec_no )
         break ;
      if (no == rec_no)
      {
         okay = 1 ;
         printf("Date : %2d/%2d/%4d\n",dd,mm,yy);
         printf("\n\n Name : ");
         gets(name);
         char* last_char = name + strlen(name);
         if (*last_char == '\n') {
            *last_char = '\0';
         }
         strcpy(det.name,name) ;
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
         title_page(out_fp);
         print_birth_details(out_fp,&det);
         print_planet_details(out_fp,pl_det);
         print_hp_pos(out_fp,hp_pos);
         get_aspects(pl_det,pl_det,SAME,aspects);
         print_self_aspects(out_fp,aspects);
         hindu_details(out_fp,&birth_info,pl_det,hs);
         int moon_rasi = (int)(hind_long(pl_det[MOON].geo_long)+0.5) % 30;
         /*
         print_seven_and_half_sani(out_fp, moon_rasi, birth_info.j_date);
         print_kandaka_sani(out_fp, moon_rasi, birth_info.j_date);
         */
         break ;
      }
   }
   fprintf(out_fp,"\n\n");
   fclose(in_fp);
   fclose(out_fp);
   if (!okay)
      printf("\n Record not found !\n");
   return 0;
}
