/*   STA.C   */
                     
#include "ast.h"

void lprint_birth_details(FILE *fp,DT b_det);
void lprint_planet_details(FILE *fp,ST_PL_DET pl_details[], int indian);
void lprint_hp_pos (FILE *fp,HP_POS hp_pos, int indian);
void lprint_self_aspects (FILE *fp, ST_PL_DET pl_details[]);
void lhindu_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                    double hs[]);

void print_title(FILE* fp);

bool debugging = true;

int show_forecast = 0;

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
         printf("det.st_long = %f\n",det.st_long);
         printf("birth_info.st_long = %f\n",birth_info.st_long);
         printf("birth_info.j_date = %f\n",birth_info.j_date);
         printf("birth_info.latitude = %f\n",birth_info.latitude);
         printf("birth_info.longitude = %f\n",birth_info.longitude);
         full_details(birth_info.j_date,pl_det);
         e = find_e_and_ayanamsa(birth_info.j_date);
         get_house_degrees(e,&birth_info,hs);
         find_hp_pos(pl_det,hs,hp_pos);

         fprintf(out_fp, "\\documentclass{article}\n");
         fprintf(out_fp, "\\usepackage{float}\n");
         fprintf(out_fp, "\\input{panjang_macros}\n");
         fprintf(out_fp, "\\usepackage{fancyhdr}\n");
         fprintf(out_fp, "\\pagestyle{fancy}\n");
         fprintf(out_fp, "\\renewcommand\\headrulewidth{0pt}\n");
         fprintf(out_fp, "\\renewcommand\\plainheadrulewidth{0pt}\n");
         fprintf(out_fp, "\\renewcommand\\footrulewidth{0.4pt}\n");
         fprintf(out_fp, "\\renewcommand\\plainfootrulewidth{0.4pt}\n");
         fprintf(out_fp, "\\lhead{}\n");
         fprintf(out_fp, "\\chead{}\n");
         fprintf(out_fp, "\\rhead{}\n");
         fprintf(out_fp, "\\lfoot{\\textbf{\\textsf{AstroTruth\\texttrademark}} Ver. 2.0, \\copyright~Umesh P. N., 1992-2012.}\n");
         fprintf(out_fp, "\\cfoot{}\n");
         fprintf(out_fp, "\\rfoot{Page~\\thepage}\n");

/*
         fprintf(out_fp, "\\lfoot[\fancyplain{\cconf}{\cconf}]{\fancyplain{\VersionDate}{\VersionDate}}\n");
         fprintf(out_fp, "\\cfoot[\fancyplain{\mypage}{\mypage}]{\fancyplain{\mypage}{\mypage}}\n");
         fprintf(out_fp, "\\rfoot[\fancyplain{\VersionDate}{\VersionDate}]{\fancyplain{\cconf}{\cconf}}\n");
*/         
         fprintf(out_fp, "\\begin{document}\n");
         print_title(out_fp);
         lprint_birth_details(out_fp,&det);
         lprint_planet_details(out_fp,pl_det, 0);
         lprint_hp_pos(out_fp,hp_pos, 0);
         get_aspects(pl_det,pl_det,SAME,aspects);
         lprint_self_aspects(out_fp,pl_det);
         lhindu_details(out_fp,&birth_info,pl_det,hs);
         fprintf(out_fp, "\\end{document}\n");

         /* int moon_rasi = (int)(hind_long(pl_det[MOON].geo_long)+0.5) % 30;*/
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

void print_title(FILE* fp) {
 fprintf(fp, "\\begin{center}\\huge\\textbf{\\textsc{A S T R O - T R U T H}}\\end{center}\n");
 fprintf(fp, "\\begin{center}A Computer Program to do Astronomical and Astrological calculations using the laws of Physics and without ");
 fprintf(fp, "Almanacs, Ephermeris or Tables\n\n");

 fprintf(fp,"Copyright \\copyright : Umesh P. N. (1992)\n\n");
 fprintf(fp,"{\\large \\textsf{DISCLAIMER}}\\end{center}\n\n");

 fprintf(fp,"This  program accuarately calculates  the ");
 fprintf(fp,"planetary positions at a specified time using laws ");
 fprintf(fp,"of Physics and Mathematics, and calculates the ");
 fprintf(fp,"other astrological aspects as per the Western ");
 fprintf(fp,"(Sayana) as well as Indian (Nirayana) methods.\n\n");

 fprintf(fp,"It is to be borne in mind the validity of ");
 fprintf(fp,"astrology has not been established statistically ");
 fprintf(fp,"or otherwise so far, and it is foolishness to ");
 fprintf(fp,"think that the planetary positions at the time of ");
 fprintf(fp,"birth determines one's character and future.\n\n");

 fprintf(fp,"The author does not give any predictions ");
 fprintf(fp,"based on these results; nor does he recommend the ");
 fprintf(fp,"use of these results to assess anybody's future or ");
 fprintf(fp,"to check matching of a would-be-married couple. If ");
 fprintf(fp,"anybody does that, it is their own belief and on ");
 fprintf(fp,"their own risk only.\n\n");

 fprintf(fp,"However, the author encourages anybody who ");
 fprintf(fp,"likes to study Astrology objectively and conducts ");
 fprintf(fp,"research in finding the truth  behind  this ");
 fprintf(fp,"pseudoscience.\n");
 fprintf(fp,"\\clearpage\n");
}
