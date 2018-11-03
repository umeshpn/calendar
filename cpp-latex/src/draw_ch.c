
/*  DRAW_CH.C */

#include "ast.h"
#define SPACE 32
#ifdef GRCHAR
#  define LT_DD 201
#  define RT_DD 187
#  define LB_DD 200
#  define RB_DD 188
#  define LR_DD 205
#  define TB_DD 186
#  define TM_DS 209
#  define BM_DS 207
#  define LM_DS 199
#  define RM_DS 182
#  define TM_SS 194
#  define BM_SS 193
#  define LM_SS 195
#  define RM_SS 180
#  define MM_SS 197
#  define LR_SS 196
#  define TB_SS 179
#else
#  define LT_DD '#'
#  define RT_DD '#'
#  define LB_DD '#'
#  define RB_DD '#'
#  define LR_DD '#'
#  define TB_DD '#'
#  define TM_DS '#'
#  define BM_DS '#'
#  define LM_DS '#'
#  define RM_DS '#'
#  define TM_SS '*'
#  define BM_SS '*'
#  define LM_SS '*'
#  define RM_SS '*'
#  define MM_SS '*'
#  define LR_SS '*'
#  define TB_SS '*'
#endif
#define COL_WIDTH 10
#define MARGIN "          "

typedef enum { ARI, TAU, GEM, CAN, LEO, VIR, LIB, SCO, SAG,
               CAP, AQU, PIS } SIGN_SHORT_ID ;

void draw_charts (FILE *fp, int sign_pl[][10], int nav_pl[][10]) 
{
   fputs("\n\n", fp);
   draw_chart(fp, sign_pl, "G R A H A S T H I T I");
   fputs("\n\n", fp);
   draw_chart(fp, nav_pl, "N A V A M S A");
}

void draw_chart (FILE* fp, int pl_arr[][10], const char* title)
{
   draw_top(fp);
   draw_four(fp,PIS,ARI,TAU,GEM,pl_arr);
   draw_mid1(fp);
   draw_two(fp,AQU,CAN,pl_arr);
   draw_mid2(fp, title);
   draw_two(fp,CAP,LEO,pl_arr);
   draw_mid3(fp);
   draw_four(fp,SAG,SCO,LIB,VIR,pl_arr);
   draw_bottom(fp);
}
void draw_top (FILE *fp)
{
   int i;

   fputs(MARGIN,fp);
   fputc(LT_DD,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_DD,fp);
   fputc(TM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_DD,fp);
   fputc(TM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_DD,fp);
   fputc(TM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_DD,fp);
   fputc(RT_DD,fp);
   fputc('\n',fp);
}
void draw_bottom (FILE *fp)
{
   int i;

   fputs(MARGIN,fp);
   fputc(LB_DD,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_DD,fp);
   fputc(BM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_DD,fp);
   fputc(BM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_DD,fp);
   fputc(BM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_DD,fp);
   fputc(RB_DD,fp);
   fputc('\n',fp);
}
void draw_mid1(FILE *fp)
{
   int i;

   fputs(MARGIN,fp);
   fputc(LM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(MM_SS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(BM_SS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(MM_SS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(RM_DS,fp);
   fputc('\n',fp);
}
void draw_mid2(FILE *fp, const char* title)
{
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
}

void draw_mid3(FILE *fp)
{
   int i ;

   fputs(MARGIN,fp);
   fputc(LM_DS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(MM_SS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(TM_SS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(MM_SS,fp);
   for ( i = 0 ; i < COL_WIDTH ; i++)
      fputc(LR_SS,fp);
   fputc(RM_DS,fp);
   fputc('\n',fp);
}
void draw_four(FILE *fp,int left, int mid1, int mid2, int right,
               int pl_arr[][10])
{
   int i ;
   char l_first[4],l_second[4],m1_first[4],m1_second[4],m2_first[4],
      m2_second[4],r_first[4],r_second[4];

   for ( i = 0 ; i < 5 ; i++)
   {
      fputs(MARGIN,fp);
      pl_str(pl_arr[left][i],l_first) ;
      pl_str(pl_arr[left][i+5],l_second);
      pl_str(pl_arr[mid1][i],m1_first) ;
      pl_str(pl_arr[mid1][i+5],m1_second);
      pl_str(pl_arr[mid2][i],m2_first) ;
      pl_str(pl_arr[mid2][i+5],m2_second);
      pl_str(pl_arr[right][i],r_first) ;
      pl_str(pl_arr[right][i+5],r_second);
      fprintf(fp,
              "%c %-3s  %-3s %c %-3s  %-3s %c %-3s  %-3s %c %-3s  %-3s %c\n",
              TB_DD,l_first,l_second,TB_SS,m1_first,m1_second,TB_SS,m2_first,
              m2_second,TB_SS,r_first,r_second,TB_DD);
   }
}

void draw_two(FILE *fp,int left, int right, int pl_arr[][10])
{
   int i ;
   char l_first[4],l_second[4],m1_first[4],m1_second[4],m2_first[4],
      m2_second[4],r_first[4],r_second[4];

   for ( i = 0 ; i < 5 ; i++)
   {
      fputs(MARGIN,fp);
      pl_str(pl_arr[left][i],l_first) ;
      pl_str(pl_arr[left][i+5],l_second);
      strcpy(m1_first,"   ");
      strcpy(m1_second,"   ");
      strcpy(m2_first,"   ");
      strcpy(m2_second,"   ");
      pl_str(pl_arr[right][i],r_first) ;
      pl_str(pl_arr[right][i+5],r_second);
      fprintf(fp,
              "%c %-3s  %-3s %c %-3s  %-3s %c %-3s  %-3s %c %-3s  %-3s %c\n",
              TB_DD,l_first,l_second,TB_SS,m1_first,m1_second,SPACE,m2_first,
              m2_second,TB_SS,r_first,r_second,TB_DD);
   }
}
char *pl_str(int pl_no, char *pl_short_name)
{
   if (pl_no >= 0 && pl_no < 14)
   {
      strncpy(pl_short_name,hind_pl_name[pl_no],3);
      pl_short_name[3] = '\0';
   }
   else
      sprintf(pl_short_name,"   ");
   return (pl_short_name);
}
