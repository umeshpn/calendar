/*                           -----
                             AST.H
    (Header file for astronomical calculations)
     -----------------------------------------
*/

#ifndef AST
#define AST 1
#ifdef IBMPC
#undef IBMPC
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
using std::string;

#define MAKE_FILE  1                 /*   Set this to 1 if you want to
                                          compile separately and link
                                          together
                                     */

#ifdef IBMPC
#  include <dos.h>
#  include <conio.h>
#else
#  define  getch() getchar()
#endif

#define skip_page(fp) fputc(12,fp)

#define   SAME          1
#define   DIFFERENT     0
#define   FULL          1
#define   PART          0
#define   TIME          1
#define   DEGREE        0
#define   MAX_DECL      23.43333
#define   FULL_CIRCLE   360.0
#define   HALF_CIRCLE   180.0
#define   D_IN_Y        365.25       /*  DAYS IN YEAR          */
#define   D_IN_M        30.6001      /*  DAYS IN MONTH         */
#define   D_IN_C        36525.0      /*  DAYS IN CENTURY       */
#define   D_IN_R        57.2957795   /*  DEGREES IN RADIAN     */
#define   H_IN_D        24.0         /*  HOURS IN DAY          */
#define   ASTRO_UNIT    1.496e+08    /*  ASTRONOMICAL UNIT kM  */
#define   EARTH_EQ_RAD  6378.14      /*  EQUITORIAL RADIUS OF
                                         EARTH IN kM       */
#define   ECC_MOON      0.0549       /*  ECCENTRICITY OF MOON  */
#define   INCL_MOON     5.14553      /*  INCLINATION OF MOON   */

#define   SPACE2        "  "

/*   Julian days of different epochs  */
/*   -------------------------------  */
#define   J_DAY_1900_PRE_NOO    2415020.0    /*  1900 Jan  0.5 */
#define   J_DAY_1960_LIB_MID    2437200.5    /*  1960 Sep 23.0 */
#define   J_DAY_2000_1ST_NOO    2451545.0    /*  2000 Jan  1.5 */

#define N_SIGNS   12
#define N_NAKS    27
#define N_PLANETS 14
#define N_PAKSHAS  2
#define N_DAYS     7
#define N_THITHIS 15
#define N_DASAS    9

/*   Definition of structures and enums    */
/*   ----------------------------------    */
struct date
{
   int da_year ;
   int da_mon ;
   int da_day ;
} ;
struct time
{
   int ti_hour ;
   int ti_min ;
   int ti_sec ;
   int ti_hund ;
} ;
typedef struct dms
{
   int degrees ;
   int minutes ;
   double seconds ;
} ST_DMS, *DMS ;

typedef struct date_time
{
   char name[40];

   int year ;
   char month ;
   char day ;

   char hour ;
   char minute ;
   double second ;

   double jul_date ;
   double jul_cent ;
   double decimal_time ;
   double l_time ;
   double g_time ;
   double g_sid_time ;
   double l_sid_time ;

   double latitude ;
   double longitude ;
   double st_long ;

}  ST_DT, *DT ;

typedef struct abs_b_details
{
   double j_date ;
   double latitude ;
   double longitude ;
  double st_long;
} ST_ABD, *ABD ;

typedef struct planet_details
{
   double hel_long ;       /*  Heliocentric Longitude   */
   double hel_dist ;       /*  Heliocentric Distance    */
   double orb_ecc  ;       /*  Orbital Eccentricity     */
   double orb_incl ;       /*  Orbital Inclination      */
   double asc_node_long ;  /*  Longitude of Ascending Node  */

   double geo_long ;       /*  Geocentric Longitude     */
   double geo_lat  ;       /*  Geocentric Latitude      */
   double geo_dist ;       /*  Distance from Earth      */
   int zod_sign    ;       /*  Zodiac Sign              */
   int house_no    ;       /*  House                    */
   char direction  ;       /*  Forward or Retarding     */

} ST_PL_DET, *PL_DET ;

typedef struct house_planet_pos
{
   char hp_type ;
   int hp_no ;
   double degree ;
   struct house_planet_pos *next_hp ;
} ST_HP_POS, *HP_POS ;

typedef struct house_position
{
   int zod_no ;
   double house_degree ;
   double zod_degree ;
} ST_H_POS, *H_POS ;

typedef struct dasa
{
   int lord ;
   double start_j_day ;
   double end_j_day ;
} ST_DASA, *DASA ;


typedef enum
{
   SUN, MOON, MERCURY, VENUS, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO,
   ASC_NODE, DESC_NODE, ASCENDENT,MIDHEAVEN
} PL_ID ;

typedef enum
{
  DASA_KETU, DASA_SUKRA, DASA_RAVI, DASA_CHANDRA, DASA_KUJA, DASA_RAHU, DASA_GURU, DASA_SANI, DASA_BUDHA
} DASAS;

typedef enum
{
   EXALT, FALL, RULER, DETRIMENT
} PL_STAT ;


typedef struct {
   int n_values;
   struct {
      int value;
      double end_j_time;
   } values[3];
} END_VALUES;

typedef struct {
  double beg;
  double end;
} ST_DURATION, *DURATION;



/*     Function Declarations     */
/*     ---------------------     */

#ifdef __cplusplus
extern "C" {
#endif

   char    *asp_det(double,char *);
   void     calculate_a_planet(int,double,double,double,double,double,
                               PL_DET,double [],double [],double [],double [],double []);
   void     calculate_planets(double,ST_PL_DET []);
   void     center_it_pw( FILE *, const char *, int, int );
   int      check_asp(double,double,double);
   double   convert_time(double,double,double);
   double   cubic(double,double,double,double,double);
   char    *date_time_string(double, char *);
   double   d_sin(double);
   double   d_cos(double);
   char    *dms_string(DMS,int,char *);
   char    *dm_string(DMS,int,char *);
   DMS      dms_time (double,DMS);
   double   d_tan(double);
   double   d_time(struct time *);
   double   equiv_time (DMS);
   void     find_birth_info(DT,ABD);
   void     find_hp(double, double, double, double, double, double []);
   double   find_e_and_ayanamsa(double);
   HP_POS   find_hp_pos(ST_PL_DET [],double [],ST_HP_POS []);
   void     find_parameters(double, double *, double *, double *, double *,
                            double *);
   void     full_details(double,ST_PL_DET[]);
   void     get_aspects(ST_PL_DET [],ST_PL_DET [],int,double [][N_PLANETS]);
   double   get_ayanamsa(double j_day);
   double   get_dasas(double,double,ST_DASA []);
   void     get_date_time(double, struct date *, struct time *);
   double   get_e(double j_day);
   double   gr_sidereal_time(double);
   double   hind_long (double);
   void     hindu_details(FILE *,ABD,ST_PL_DET [], double []);
   double   house_degree(double,double,double,double);
   double   j_day_from_local_time(struct date *, DMS, double);
   double   julian_day(struct date *,double);
   double   linear(double,double,double);
   double   local_sidereal_time(double, double);
   double   nakshatra(double);
   double   navamsa(double);
   double   norm_angle(double);
   double   norm_hour(double);
   double   norm_longitude(double);
   double   poly_value(int,double[],double);
   double   poly_val1(int,double[],double);
   void     print_antar_dasa(FILE *,DASA);
   void     print_aspects(FILE *,double [][N_PLANETS], int, int);
   void     print_asp_heading(FILE *, int, int);
   void     print_birth_details(FILE *,DT);
   void     print_hind_details(FILE *,ST_PL_DET [],double);
   void     print_hind_heading(FILE *);
   void     print_house(FILE *,HP_POS,int);
   void     print_hp_heading (FILE *);
   void     print_mutual_aspects(FILE *, double [][N_PLANETS], char *, char *);
   void     print_mutual_asp_heading (FILE *, char *, char *) ;
   void     print_hp_pos(FILE *,HP_POS);
   void     print_planet(FILE *,HP_POS);
   void     print_planet_details(FILE *,ST_PL_DET []);
   void     print_planet_heading(FILE *);
   void     print_self_aspects(FILE *,double [][N_PLANETS]);
   void     print_self_asp_heading(FILE *);
   double   quadratic(double,double,double,double);
   char    *reduced_string(double,char *,int);
   double   sc_d_angle(double,double);
   char    *time_string(double,int,int,char *);
   void     under_line (FILE *,int);
  int      weekday(double, double);

   /*   In HS.C   */

   double   eleventh_from_lat_st_ecl(double,double,double);
   double   first_from_lat_st_ecl(double,double,double);
   void     get_house_degrees(double,ABD,double []);
   double   mh_from_st_ecl(double,double);
   int      quadrant(double);
   double   second_from_lat_st_ecl(double,double,double);
   double   sin_correction(double,double);
   double   tan_correction(double,double);
   double   third_from_lat_st_ecl(double,double,double);
   double   twelfth_from_lat_st_ecl(double,double,double);
  double get_lagnam(double j_day, double latitude, double longitude);

   /*    In DRAW_CH.C   */

   void draw_charts(FILE *, int [][10],int [][10]);
   void draw_chart_data(FILE *, int [][10], const char *);
   void draw_chart(FILE *, int [][10], const char *);
   void draw_chart_tabular(FILE *, int [][10], const char *);
   void draw_top(FILE *);
   void draw_bottom(FILE *);
   void draw_mid1(FILE *);
   void draw_mid2(FILE *, const char *);
   void draw_mid3(FILE *);
   void draw_four(FILE *,int, int, int, int , int [][10]);
   void draw_two (FILE *, int, int, int [][10]);
   char *pl_str(int, char *);

   /*    In LDRAW_CH.C   */

  void ldraw_charts (FILE *fp, int sign_pl[][10], int nav_pl[][10], int lag, int cha, int nlag, int ncha, int print_navamsam); 
  void ldraw_chart (FILE* fp, int pl_arr[][10], const char* title, int asc, int moon);
  void ldraw_chart_tabular(FILE* fp, int pl_arr[][10], const char* title, int asc, int moon);
  void ldraw_chart_data(FILE* fp, int pl_arr[][10], const char* title, int asc, int moon);
  void ldraw_top(FILE *);
  void ldraw_bottom(FILE *);
  void ldraw_mid1(FILE *);
  void ldraw_mid2(FILE *, const char *);
  void ldraw_mid3(FILE *);
  void ldraw_four(FILE *fp,int left, int mid1, int mid2, int right,
                  int pl_arr[][10], int ascendent, int moon);
  void ldraw_two(FILE *fp,int left, int right, int pl_arr[][10], int ascendent, int moon);
   char *lpl_str(int, char *);

   /*     In TITLE.C    */

   void center_it ( FILE *, const char *, int );
   void title_page (FILE *) ;

   /*  In END_VALUES.C   */

   typedef int (*AstFunc)(double);
   typedef double (*AstDFunc)(double);
   int    find_matches(double start_j_day, double end_j_day, END_VALUES* end_values, AstFunc func, int cycle);
   double find_next_match(int val, double start_j_day, double end_j_day, AstFunc func, int cycle);
   int    find_naks_in_day (double j_day, END_VALUES* end_values);
   int    find_naks_in_day_gen (double j_day1, double j_day2, END_VALUES* end_values);
   int    find_thiths_in_day (double j_day, END_VALUES* end_values);
   int    find_thiths_in_day_gen (double j_day1, double j_day2, END_VALUES* end_values);
   double find_zero(double low, double high, AstDFunc func) ;
   int    inakshatram(double j_day);
   int    ithithi(double j_day);
   int    next_sun_transit(double j_day, double* transit_j_day);
   double sunrise_between(double low, double high, double latitude, double longitude);
   double sunset_between(double low, double high, double latitude, double longitude);

   
  double get_sun_altitude(double j_day, double geo_latitude, double geo_longitude);
  int sun_transit_between(double low, double high, double* transit_j_day);

#ifdef __cplusplus
}
#endif

/* In lprint_lib.cxx */
void convert_to_hm(double t, int *h, int *m);
char* get_std_time_string(double j_day, double st_long, int date_and_time, char* ts);
double sunrise_before(double j_day, double latitude, double longitude);
double sunset_after(double j_day, double latitude, double longitude);
char* ast_angle_string(double angle, char* str, char direction, int indian);
void lskip_page(FILE* fp);
void start_latex_table(FILE* fp, const char* format);
void start_latex_tabular(FILE* fp, const char* format);
void end_latex_table(FILE* fp, const char* label, const char* caption);
void end_latex_tabular(FILE* fp);
void print_latex_row(FILE* fp, const char* str);
void print_hline(FILE* fp);
void latex_section(FILE* fp, const char* title);
void latex_subsection(FILE* fp, const char* title);
void lprint_birth_details(FILE *fp,DT b_det);
void lprint_planet_details(FILE *fp,ST_PL_DET pl_details[], int indian);
void lprint_hp_pos (FILE *fp,HP_POS hp_pos);
void lprint_self_aspects (FILE *fp, ST_PL_DET planets[]);
void lprint_antar_dasa(FILE *fp,DASA part_dasa);
void lhindu_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                    double hs[]);
void lmuh_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                  double hs[]);


/*     In SANI.C     */
void print_seven_and_half_sani(FILE* fp, int moon_house, double birth_jday);
void print_kandaka_sani(FILE* fp, int moon_house, double birth_jday);

#if MAKE_FILE
/*  External variables defined in AST_DATA.C     */

extern bool debugging;
extern double ayanamsa ;
extern double au[8] ;
extern double lc[8][4] ;
extern double ec[8][4] ;
extern double ic[8][4] ;
extern double oc[8][4] ;
extern double mc[8][4] ;
extern double dasa_yr[N_DASAS] ;
extern const char *zod_name[N_SIGNS] ;
extern const char *hind_zod_name[N_SIGNS] ;
extern const char *pl_name[N_PLANETS] ;
extern const char *hind_pl_name[N_PLANETS] ;
extern const char *h_dasa_name[N_DASAS] ;
extern const char *paksha_name[N_PAKSHAS] ;
extern const char *thithi_name[N_THITHIS] ;
extern const char *thithi_name_2[N_THITHIS * 2] ;
extern const char *nak_name[N_NAKS] ;
extern const char *nak_name_alt[N_NAKS] ;
extern const char *day_name[N_DAYS] ;
extern const char *sign_dignity[5] ;
extern const char *house_dignity[5] ;
extern const char *mnth_name[12];

extern const char *long_sign_dignity[];
extern const char *long_house_dignity[];
extern const char *l_sign_dignity[];

extern int pl_sign_dignity[13][N_SIGNS] ;

extern const char* lhind_pl_name[];
extern int show_forecast;

#endif

#endif
