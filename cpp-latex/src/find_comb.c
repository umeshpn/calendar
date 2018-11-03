#include "ast.h"
#include<time.h>

bool debugging = false;

int main(int argc, char *argv[]) {
  double start_long, end_long, j_date, hl;
  ST_PL_DET pl_det[15];
  ST_ABD birth_info;
  ST_DT det;
  det.month = 2;
  det.day = 24;
  det.minute = 0;
  det.second = 0.0;
  det.latitude = 10.0 ;
  det.longitude = -76.5 ;
  det.st_long = -82.5;
	double e;
  int star_no = 11;
  start_long = star_no * 360.0 / 27;
  end_long = (star_no+1) * 360.0 / 27;
  int year;
  for (year = 1950; year < 2010; ++year) {
    det.year = year;

    det.hour = 6.0;
    find_birth_info(&det, &birth_info);
    j_date = birth_info.j_date;
    e = find_e_and_ayanamsa(j_date);
    full_details(j_date, pl_det);
    hl = hind_long(pl_det[MOON].geo_long);
    if (hl >= start_long && hl < end_long) {
      printf("There is a match in %d morning.\n", year);
    }

    j_date += 0.9;
    e = find_e_and_ayanamsa(j_date);
    full_details(j_date, pl_det);
    hl = hind_long(pl_det[MOON].geo_long);
    if (hl >= start_long && hl < end_long) {
      printf("There is a match in %d evening.\n", year);
    }

  }
}
