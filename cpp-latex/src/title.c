/*    TITLE.C    */
                 
#include "ast.h"

#define PAGE_WIDTH 80

void title_page(FILE *fp)
{
   skip_page(fp);
   fprintf(fp,"\n\n\n\n\n\n\n\n\n\n");
   center_it(fp,"A S T R O - T R U T H",0);
   center_it(fp,"= = = = = = = = = = =",0);
   center_it(fp,"A Computer Program to do Astronomical & Astrological",0);
   center_it(fp,"Calculations Using the Laws of Physics and without",0);
   center_it(fp,"Almanacs, Ephermeris  or Tables",1);
   center_it(fp,"Copyright : Umesh P.N. (1992)",2);
   center_it(fp,"DISCLAIMER",1);

   center_it(fp,"     This   program  accuarately  calculates   the",0);
   center_it(fp,"planetary positions at a specified time using laws",0);
   center_it(fp,"of  Physics  and Mathematics, and  calculates  the",0);
   center_it(fp,"other  astrological  aspects as  per  the  Western",0);
   center_it(fp,"(Sayana) as well as Indian (Nirayana) methods.    ",1);

   center_it(fp,"     It is  to be borne in mind  the  validity  of",0);
   center_it(fp,"astrology  has not been established  statistically",0);
   center_it(fp,"or  otherwise  so  far, and it is  foolishness  to",0);
   center_it(fp,"think that the planetary positions at the time  of",0);
   center_it(fp,"birth determines one's character and future.      ",1);

   center_it(fp,"     The  author  does not  give  any  predictions",0);
   center_it(fp,"based on these results; nor does he recommend  the",0);
   center_it(fp,"use of these results to assess anybody's future or",0);
   center_it(fp,"to check matching of a would-be-married couple. If",0);
   center_it(fp,"anybody does that, it is their own belief  and  on",0);
   center_it(fp,"their own risk only.                              ",1);

   center_it(fp,"     However,  the author encourages  anybody  who",0);
   center_it(fp,"likes to study Astrology objectively and  conducts",0);
   center_it(fp,"research   in  finding  the  truth   behind   this",0);
   center_it(fp,"(pseudo)science.                                  ",0);
}


void center_it(FILE *fp, const char *any_string, int line_skip)
{
   center_it_pw(fp, any_string, line_skip, PAGE_WIDTH);
}
