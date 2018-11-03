#include "ast.h"
#include<time.h>
int search_star(int star_no, DT now) {
   double start_long, end_long, j_date, hl;
   ST_PL_DET pl_det[15];
   ST_ABD birth_info;
   int i;
	double e;
   start_long = star_no * 360.0 / 27;
   end_long = (star_no+1) * 360.0 / 27;

   find_birth_info(now, &birth_info);
   j_date = birth_info.j_date;
   e = find_e_and_ayanamsa(j_date);
   for (i=0; i<36; ++i) {
      full_details(j_date, pl_det);
      hl = hind_long(pl_det[MOON].geo_long);
		/*
		printf("\nMoon\'s longitude = %10.4f\n", pl_det[MOON].geo_long);
		printf("Moon\'s indian longitude = %10.4f\n", hl);
		printf("Starting longitude of the star = %10.4f\n", start_long);
		printf("Ending longitude of the star = %10.4f\n\n", end_long);
		*/
      if (hl >= start_long && hl < end_long) {
         return i;
      }
      j_date += (1.0/24.0);
   }
   return -1;
}

void list_stars() {
   ST_DT det;
   time_t curr_time;
   int i, j;
   struct tm* gtime;
   time(&curr_time);
   gtime = gmtime(&curr_time);
   strcpy(det.name,"dummy") ;
   det.year = gtime->tm_year + 1900; 
   det.month = gtime->tm_mon + 1 ; 
   det.day = gtime->tm_mday ;
   det.hour = gtime->tm_hour ; 
   det.minute = gtime->tm_min ;
   det.second = 0.0 ;
   det.latitude = 0.0 ;
   det.longitude = 0.0 ;
   det.st_long = 0.0 ;
   for (i=0; i<27; ++i) {
      if ((j = search_star(i, &det)) != -1) {
         if (0 == j) {
            printf("Current star is %s\n", nak_name[i]);
         } else {
            printf("%s will start after %d hours\n", nak_name[i], j);
         }
      }
   }
}

int main(int argc, char *argv[]) {
   ST_DT det;
   time_t curr_time;
   int j;
   struct tm* gtime;
   int star;
   char* sta_to = getenv("STA_TO");
   char* sta_cc = getenv("STA_CC");

   if (argc != 2) {
      return -1;
   }

   if (NULL == sta_to) {
      return -1;
   }
   star = atoi(argv[1]);
   time(&curr_time);
   gtime = gmtime(&curr_time);
   strcpy(det.name,"dummy") ;
   det.year = gtime->tm_year + 1900; 
   det.month = gtime->tm_mon + 1 ; 
   det.day = gtime->tm_mday ;
   det.hour = gtime->tm_hour ; 
   det.minute = gtime->tm_min ;
   det.second = 0.0 ;
   det.latitude = 0.0 ;
   det.longitude = 0.0 ;
   det.st_long = 0.0 ;
   j = search_star(star, &det);

  
   if ( j != -1) {

      printf("To: %s\n", sta_to);
      if (sta_cc) {
         printf("Cc: %s\n", sta_cc);
      }
      if (0 == j) {
         printf("Subject: Current star is %s\n", nak_name[star]);
      } else {
         printf("Subject: %s will start after %d hours\n", nak_name[star], j);
      }
      printf("\n");
      return 0;
   } else {
      return -1;
   }
}
