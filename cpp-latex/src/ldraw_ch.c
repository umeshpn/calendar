
/*  DRAW_CH.C */

#include "ast.h"

typedef enum { ARI, TAU, GEM, CAN, LEO, VIR, LIB, SCO, SAG,
               CAP, AQU, PIS } SIGN_SHORT_ID ;

void ldraw_charts (FILE *fp, int sign_pl[][10], int nav_pl[][10], int lag, int cha, int nlag, int ncha, int print_navamsam) 
{
   fputs("\n\n", fp);
   ldraw_chart(fp, sign_pl, "\\Grahasthiti", lag, cha);
   if (print_navamsam) {
      fputs("\n\n", fp);
      ldraw_chart(fp, nav_pl, "<m>navaaMshakaM</m>", nlag, ncha);
   }
}

void ldraw_chart(FILE* fp, int pl_arr[][10], const char* title, int asc, int moon)
{
   fprintf(fp, "{ \\setlength{\\arrayrulewidth}{1pt}\n");
   start_latex_table(fp, 
       "||p{0.4in}p{0.4in}p{0.1in}|p{0.4in}p{0.4in}p{0.1in}|p{0.4in}p{0.4in}p{0.1in}|p{0.4in}p{0.4in}p{0.1in}||");
   ldraw_chart_data(fp, pl_arr, title, asc, moon);
   end_latex_table(fp, NULL, title);
   fprintf(fp, "}\n");
}

void ldraw_chart_tabular(FILE* fp, int pl_arr[][10], const char* title, int asc, int moon)
{
   fprintf(fp, "{ \\setlength{\\arrayrulewidth}{1pt}\n");
   start_latex_tabular(fp,
       "||p{0.4in}p{0.4in}p{0.1in}|p{0.4in}p{0.4in}p{0.1in}|p{0.4in}p{0.4in}p{0.1in}|p{0.4in}p{0.4in}p{0.1in}||");
   ldraw_chart_data(fp, pl_arr, title, asc, moon);
   end_latex_tabular(fp);
   fprintf(fp, "}\n");
}

void ldraw_chart_data(FILE* fp, int pl_arr[][10], const char* title, int asc, int moon)
{
   ldraw_top(fp);
   ldraw_four(fp,PIS,ARI,TAU,GEM,pl_arr, asc, moon);
   ldraw_mid1(fp);
   ldraw_two(fp,AQU,CAN,pl_arr, asc, moon);
   ldraw_mid2(fp, title);
   ldraw_two(fp,CAP,LEO,pl_arr, asc, moon);
   ldraw_mid3(fp);
   ldraw_four(fp,SAG,SCO,LIB,VIR,pl_arr, asc, moon);
   ldraw_bottom(fp);
}

void ldraw_top (FILE *fp)
{
   print_hline(fp);
   print_hline(fp);
}

void ldraw_bottom (FILE *fp)
{
   print_hline(fp);
   print_hline(fp);
}

void ldraw_mid1(FILE *fp)
{
   print_hline(fp);
}
void ldraw_mid2(FILE *fp, const char* title)
{
#if 0
   int i,either_space ;	
  
   fputs(MARGIN,fp);
   fputc(LM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(RM_SS,fp);
   either_space = (2 * COL_WIDTH + 1 - strlen(title) ) / 2  ;
   for ( i = 0 ; i < either_space ; i++)
      fputc(SPACE,fp);
   fputs(title,fp);
   for ( i = 0 ; i < either_space ; i++)
      fputc(SPACE,fp);
   fputc(LM_SS, fp);
   for ( i = 0; i < COL_WIDTH; i++)
      fputc(LR_SS, fp);
   fputc(RM_DS, fp);
   fputc('\n',fp);
#endif
   fprintf(fp, "\\cline{1-3}\\cline{10-12}\n");
}

void ldraw_mid3(FILE *fp)
{
   print_hline(fp);
}

int house_from_asc(int house_no, int asc)
{
   return ((house_no < asc) ? (house_no + 13 - asc) : (house_no +1 - asc));
}

void ldraw_four(FILE *fp,int left, int mid1, int mid2, int right,
               int pl_arr[][10], int ascendent, int moon)
{
   int i ;
   char l_first[40],l_second[40],m1_first[40],m1_second[40],m2_first[40],
      m2_second[40],r_first[40],r_second[40];
   for ( i = 0 ; i < 4 ; i++)
   {
      lpl_str(pl_arr[left][i],l_first) ;
      lpl_str(pl_arr[left][i+5],l_second);
      lpl_str(pl_arr[mid1][i],m1_first) ;
      lpl_str(pl_arr[mid1][i+5],m1_second);
      lpl_str(pl_arr[mid2][i],m2_first) ;
      lpl_str(pl_arr[mid2][i+5],m2_second);
      lpl_str(pl_arr[right][i],r_first) ;
      lpl_str(pl_arr[right][i+5],r_second);
      if (i == 0) {
         int h1 = house_from_asc(left, ascendent);
         int h2 = house_from_asc(mid1, ascendent);
         int h3 = house_from_asc(mid2, ascendent);
         int h4 = house_from_asc(right, ascendent);
        
/*          fprintf(fp, " \\fbox{%d} & %s & %s & \\fbox{%d} & %s & %s & \\fbox{%d} & %s & %s & \\fbox{%d} & %s & %s \\\\\n", */
/*                  h1, l_first,l_second, h2, m1_first,m1_second, h3, m2_first, */
/*                  m2_second, h4, r_first,r_second); */
         fprintf(fp, " %s & %s & {\\bf %d} & %s & %s & {\\bf %d} & %s & %s & {\\bf %d} & %s & %s & {\\bf %d}\\\\\n",
                 l_first,l_second, h1, m1_first,m1_second, h2, m2_first,
                 m2_second, h3, r_first,r_second, h4);
      } else if (i == 3) {
         int h1 = house_from_asc(left, moon);
         int h2 = house_from_asc(mid1, moon);
         int h3 = house_from_asc(mid2, moon);
         int h4 = house_from_asc(right, moon);
        
/*          fprintf(fp, " \\fbox{%d} & %s & %s & \\fbox{%d} & %s & %s & \\fbox{%d} & %s & %s & \\fbox{%d} & %s & %s \\\\\n", */
/*                  h1, l_first,l_second, h2, m1_first,m1_second, h3, m2_first, */
/*                  m2_second, h4, r_first,r_second); */
         fprintf(fp, " %s & %s & %d & %s & %s & %d & %s & %s & %d & %s & %s & %d\\\\\n",
                 l_first,l_second, h1, m1_first,m1_second, h2, m2_first,
                 m2_second, h3, r_first,r_second, h4);
      } else {
/*          fprintf(fp, "& %s & %s & & %s & %s & & %s & %s & & %s & %s \\\\\n", */
/*                  l_first,l_second, m1_first,m1_second, m2_first, */
/*                  m2_second, r_first,r_second); */
         fprintf(fp, "%s & %s & & %s & %s & & %s & %s & & %s & %s & \\\\\n",
                 l_first,l_second, m1_first,m1_second, m2_first,
                 m2_second, r_first,r_second);
      }
   }
}


void ldraw_two(FILE *fp,int left, int right, int pl_arr[][10], int ascendent, int moon)
{
   int i ;
   char l_first[40],l_second[40], r_first[40],r_second[40];

   for ( i = 0 ; i < 4 ; i++)
   {
      lpl_str(pl_arr[left][i],l_first) ;
      lpl_str(pl_arr[left][i+5],l_second);
      lpl_str(pl_arr[right][i],r_first) ;
      lpl_str(pl_arr[right][i+5],r_second);
      if (i == 0) {
         int h1 =  house_from_asc(left, ascendent);
         int h2 =  house_from_asc(right, ascendent);
/*          fprintf(fp, "\\fbox{%d} & %s & %s &  \\multicolumn{6}{c|}{} & \\fbox{%d} & %s & %s   \\\\\n", */
/*                  h1, l_first,l_second, h2, r_first,r_second); */
         fprintf(fp, "%s & %s & {\\bf %d} & \\multicolumn{6}{c|}{} &  %s & %s & {\\bf %d}  \\\\\n",
                 l_first,l_second, h1, r_first,r_second, h2);
      } else if (i == 3) {
         int h1 =  house_from_asc(left, moon);
         int h2 =  house_from_asc(right, moon);
/*          fprintf(fp, "\\fbox{%d} & %s & %s &  \\multicolumn{6}{c|}{} & \\fbox{%d} & %s & %s   \\\\\n", */
/*                  h1, l_first,l_second, h2, r_first,r_second); */
         fprintf(fp, "%s & %s & %d & \\multicolumn{6}{c|}{} & %s & %s & %d  \\\\\n",
                 l_first,l_second, h1, r_first,r_second, h2);
      } else {
         fprintf(fp, " %s & %s & & \\multicolumn{6}{c|}{} & %s & %s & \\\\\n",
                 l_first,l_second, r_first,r_second);
      }
   }
}
char *lpl_str(int pl_no, char *pl_short_name)
{
   if (pl_no == 8) {
      /* Shorten the name of Neptune*/
      sprintf(pl_short_name,"<m>nep.</m> ");      
   }
   else if (pl_no >= 0 && pl_no < 14)
   {
      strcpy(pl_short_name, lhind_pl_name[pl_no]);
   }
   else
      sprintf(pl_short_name," ");
   return (pl_short_name);
}
