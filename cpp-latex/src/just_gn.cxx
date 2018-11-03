/*   just_gn.cxx   */
                     
#include "ast.h"

void lprint_birth_details(FILE *fp,DT b_det);
void lprint_planet_details(FILE *fp,ST_PL_DET pl_details[]);
void lprint_hp_pos (FILE *fp,HP_POS hp_pos);
void lprint_self_aspects (FILE *fp, ST_PL_DET pl_details[]);
void lhindu_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                    double hs[]);


bool debugging = true;

int show_forecast = 1;

void lprint_numeric_values(FILE* fp, ST_PL_DET pl_details[])
{
   int pl_no, curr_house , curr_zod ;
   ST_DMS *gl,dummy ;
   double geo_long_dm ;
   char ast_ang_str[50];
   start_latex_tabular(fp, "|l|ll|l|r|l|");
   print_hline(fp);
   print_latex_row(fp, "{\\bf Planet} & \\multicolumn{2}{c|}{{\\bf Position}} & & {\\bf House} & ");
   print_hline(fp);
  for (int pl_no = SUN; pl_no <= DESC_NODE; ++pl_no) {
      geo_long_dm = fmod(pl_details[pl_no].geo_long,30.0);
      gl = dms_time(geo_long_dm,&dummy);
      curr_zod = pl_details[pl_no].zod_sign ;
      curr_house = pl_details[pl_no].house_no ;

      fprintf(fp, "%s & %s & %s & %d & %s \\\\\n", pl_name[pl_no], 
              ast_angle_string(pl_details[pl_no].geo_long, ast_ang_str, pl_details[pl_no].direction, 1),
              long_sign_dignity[pl_sign_dignity[pl_no][curr_zod]],
              curr_house,
              long_house_dignity[pl_sign_dignity[pl_no][curr_house-1]]);
  }
   pl_no = ASCENDENT;
   fprintf(fp, "%s & %s & %s & %s & %s \\\\\n", pl_name[pl_no], 
           ast_angle_string(pl_details[pl_no].geo_long, ast_ang_str, ' ', 1), " ", " ", " ");
 
   print_hline(fp);
   end_latex_tabular(fp);
}

void lprint_concise_details(FILE* fp, const char* name, ABD birth_info, DT det,
                            ST_PL_DET pl_det[], double hs[])
{
  start_latex_tabular(fp, "ll");
  lprint_numeric_values(fp, pl_det);
  int asc = (int)(hind_long(pl_det[ASCENDENT].geo_long) / 30.0);
  int moon =(int)( hind_long(pl_det[MOON].geo_long) / 30.0);
  ldraw_chart_tabular(fp, pl_arr, "\\Grahasthiti", asc, moon)

  end_latex_tabular(fp);
}

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
   char name[100];
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
   fprintf(out_fp, "\\lfoot{\\textbf{\\textsf{AstroTruth\\texttrademark}} Ver. 2.0, \\copyright~Umesh P. Nair, 1994-2006.}\n");
   fprintf(out_fp, "\\cfoot{}\n");
   fprintf(out_fp, "\\rfoot{Page~\\thepage}\n");
   fprintf(out_fp, "\\begin{document}\n");
   while (fscanf(in_fp,"%d,%d/%d/%d,%d:%d,%d:%d,%d:%d,%d:%d,%s",&no,
                 &dd,&mm,&yy,&hh,&mn,&latd,&latm,&longd,&longm,&st_longd,&st_longm, name)
          != EOF)
   {
      {
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
         /* title_page(out_fp); */

         lprint_concise_details(out_fp, &birth_info, &det, pl_det, hs)
         // lprint_birth_details(out_fp,&det);
         // lprint_planet_details(out_fp,pl_det);
         // lprint_hp_pos(out_fp,hp_pos);
         // get_aspects(pl_det,pl_det,SAME,aspects);
         // lprint_self_aspects(out_fp,pl_det);
         // indian = 1;
         // lhindu_details(out_fp,&birth_info,pl_det,hs);
         fprintf(out_fp, "\\end{document}\n");

         /* int moon_rasi = (int)(hind_long(pl_det[MOON].geo_long)+0.5) % 30;*/
         break ;
      }
   }
   fprintf(out_fp,"\n\n");
   fclose(in_fp);
   fclose(out_fp);
   return 0;
}
