/*                     ----------
                       AST_DATA.C
           ( Data for astronomical calculations)
         ----------------------------------

            Global constants
            ----------------
*/

#include "ast.h"

double ayanamsa ;
double au[8] = { 0.3870986,  0.7233316, 1.5236883,  5.202561,
         9.554746,  19.21814,  30.10957,   39.43871    };
double lc[8][4] =
{
     { 178.179078,  149474.07078,  3.011e-04, 0.0       },
     { 342.767053,   58519.21191,  3.097e-04, 0.0       },
     { 293.737334,   19141.69551,  3.107e-04, 0.0       },
     { 238.049257,   3036.301986,  3.347e-04, -1.65e-06 },
     { 266.564377,   1223.509884,  3.245e-04, -5.80e-06 },
     { 243.49747 ,    429.863546,  3.16e-04, -6.00e-07  },
     {  84.957994,    219.885914,  3.205e-04,  6.00e-07 },
     { 181.64632 ,      0.0     ,  0.0      ,  0.0      }
};
double ec[8][4] =
{
     { 0.20561421,    2.0463e-05,    -3.0e-08,        0.0     },
     { 6.82069e-03,  -4.774e-05,      9.099999e-08,   0.0     },
     { 9.331289e-02,  9.206399e-05,  -7.7e-08,        0.0     },
     { 4.833475e-02,  1.6418e-04,    -4.676e-07,     -1.7e-09 },
     { 5.589232e-02, -3.455e-04,     -7.28e-07,       7.4e-10 },
     { 0.0463444,    -2.658e-05,      7.7e-08,        0.0     },
     { 8.97704e-03,   6.33e-06,      -2.0e-09,        0.0     },
     { 0.250236,      0.0,            0.0,            0.0     }
};
double ic[8][4] =
{
     { 7.002881,   0.0018608,    -0.0000183,  0.0     },
     { 3.393631,   0.0010058,    -0.000001,   0.0     },
     { 1.850333,  -0.000675,      0.0000126,  0.0     },
     { 1.308736,  -0.0056961,     0.0000039,  0.0     },
     { 2.492519,  -0.0039189,    -1.549e-05,  4.0e-08 },
     { 0.772464,   0.0006253,     0.0000395,  0.0     },
     { 1.779242,  -9.543599e-03,  0.0000091,  0.0     },
     { 17.1699,    0.0,           0.0,        0.0     }
};
double oc[8][4] =
{
     { 47.145944,   1.1852083,  0.0001739,     0.0       },
     { 75.779647,   0.8998499,  0.00041,       0.0       },
     { 48.786442,   0.7709917, -0.0000014,    -5.33e-06  },
     { 99.443414,   1.01053,    3.5222e-04,   -8.51e-06  },
     { 112.790414,  0.8731951, -1.5218e-04,   -5.31e-06  },
     { 73.477111,   0.4986678,  0.0013117,     0.0       },
     { 130.681389,  1.098935,   2.4987e-04,   -4.718e-06 },
     { 109.88562,   0.0,        0.0,           0.0       }
};
double mc[8][4] =
{
     { 102.27938, 149472.51529, 0.000007     ,   0.0 },
     { 212.60322, 58517.80387,  0.001286     ,   0.0 },
     { 319.51913, 19139.85475,  0.000181     ,   0.0 },
     { 223.32833, 3034.69202,  -7.220001e-04 ,   0.0 },
     { 175.46622, 1221.55147,   0.0          ,   0.0 },
     { 0.0,          0.0,       0.0          ,   0.0 }
};

double dasa_yr[] = { 7,20,6,10,7,18,16,19,17 };

  /*     Global Character Strings          */



const char *mnth_name[] = { "JANUARY", "FEBRUARY", "MARCH", "APRIL", 
                    "MAY", "JUNE", "JULY", "AUGUST", 
                    "SEPTEMBER", "OCTOBER", "NOVEMBER", "DECEMBER"
                  } ;

const char *zod_name[] = {  "Aries","Taurus","Gemini","Cancer","Leo",
  "Virgo","Libra","Scorpius","Sagittarius","Capriconus","Aquarius",
  "Pisces" } ;

const char *hind_zod_name[] = { "MESHA","VRUSHABHA","MIDHUNA","KATAKA",
  "SIMHA","KANYA","THULA","VRISCHIKA","DHANU","MAKARA","KUMBHA","MEENA"};

const char *pl_name[] = { "Sun","Moon","Mercury","Venus","Mars",
  "Jupiter","Saturn","Uranus","Neptune","Pluto","Drhead","Drtail",
  "Ascendent","Midheaven" } ;

const char *hind_pl_name[] = { "RAVI","CHANDRA","BUDHA","SUKRA",
  "KUJA","GURU","SANI","HERSHEL","NEPTUNE","PLUTO","RAHU","KETU",
  "LAGNA","DASAMA" } ;

const char *h_dasa_name[] = { "KET","SUK","RAV","CHA","KUJ","RAH",
   "GUR","SAN","BUD" } ;

const char *paksha_name[] = { "SUKLA","KRISHNA" } ;

const char *thithi_name[] = { "VASYA","PRADIPADAM","DWITHEEYA","THRUTHEEYA",
   "CHATHURTHI","PANCHAMI","SHASHTI","SAPTAMI","ASHTAMI","NAVAMI",
   "DASAMI","EKADASI","DWADASI","THRAYODASI","CHATURDASI" };

// const char *thithi_name_2[] = { 
//   "  AMAVASI",
//   "S PRADIPADAM",
//   "S DWITHEEYA",
//   "S THRUTHEEYA",
//   "S CHATHURTHI",
//   "S PANCHAMI",
//   "S SHASHTI",
//   "S SAPTAMI",
//   "S ASHTAMI",
//   "S NAVAMI",
//   "S DASAMI",
//   "S EKADASI",
//   "S DWADASI",
//   "S THRAYODASI",
//   "S CHATURDASI",
//   "  POURNAMI",
//   "K PRADIPADAM",
//   "K DWITHEEYA",
//   "K THRUTHEEYA",
//   "K CHATHURTHI",
//   "K PANCHAMI",
//   "K SHASHTI",
//   "K SAPTAMI",
//   "K ASHTAMI",
//   "K NAVAMI",
//   "K DASAMI",
//   "K EKADASI",
//   "K DWADASI",
//   "K THRAYODASI",
//   "K CHATURDASI"
// };

const char *thithi_name_2[] = { 
  "Amavasi",
  "Prathama",
  "Dwitheeya",
  "Thritheeya",
  "Chathurthi",
  "Panchami",
  "Shashti",
  "Sapthami",
  "Ashtami",
  "Navami",
  "Dhashami",
  "Ekadhashi",
  "Dwadhashi",
  "Threyodhashi",
  "Chathurdhashi",
  "Pournami",
  "Prathama",
  "Dwitheeya",
  "Thritheeya",
  "Chathurthi",
  "Panchami",
  "Shashti",
  "Sapthami",
  "Ashtami",
  "Navami",
  "Dhashami",
  "Ekadhashi",
  "Dwadhashi",
  "Threyodhashi",
  "Chathurdhashi",
};

 const char *nak_name[] =
 {
  "ASWATHI",      "BHARANI",      "KARTHIKA",    "ROHINI",
  "MAKAYIRAM",    "ATHIRA",       "PUNARTAM",    "POOYAM",
  "AYILYAM",      "MAKAM",        "POORAM",      "UTHRAM",
  "ATTHAM",       "CHITHRA",      "SWATHI",      "VISAKHAM",
  "ANIZHAM",      "THRIKKETA",    "MOOLAM",      "POORADAM",
  "UTHRADAM",     "THIRUVONAM",   "AVITTAM",     "CHATHAYAM",
  "POORUTTATHI",  "UTHRUTTATHI",  "REVATHI"
};

const char *nak_name_alt[] =
{
  "Ashwathi",      "Bharani",      "Karthika",    "Rohini",
  "Makayiram",    "Thiruvathira",   "Punartham",    "Pooyam",
  "Aayillyam",      "Makam",        "Pooram",      "Uthram",
  "Atham",       "Chithira",      "Chothi",      "Vishakham",
  "Anizham",      "Thrikketta",    "Moolam",      "Pooradam",
  "Uthradam",     "Thiruvonam",   "Avittam",     "Chathayam",
  "Pooruruttathi",  "Uthruttathi",  "Revathi"
};

const char *day_name[] =
{
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
  "Friday", "Saturday"
} ;

int pl_sign_dignity[13][12] =
{
/* Signs    ARI TAU GEM CAN LEO VIR LIB SCO SAG CAP AQU PIS   */
/* --------------------------------------------------------   */
/* SUN */ {  1 , 0 , 0 , 0 , 3 , 0 , 2 , 0 , 0 , 0 , 4 , 0  },
/* MOO */ {  0 , 1 , 0 , 3 , 0 , 0 , 0 , 2 , 0 , 4 , 0 , 0  },
/* MER */ {  0 , 0 , 3 , 0 , 2 , 3 , 0 , 0 , 4 , 0 , 1 , 4  },
/* VEN */ {  4 , 3 , 0 , 0 , 0 , 2 , 3 , 4 , 0 , 0 , 0 , 1  },
/* MAR */ {  3 , 4 , 0 , 2 , 0 , 0 , 4 , 3 , 0 , 1 , 0 , 0  },
/* JUP */ {  0 , 0 , 4 , 1 , 0 , 4 , 0 , 0 , 3 , 2 , 0 , 3  },
/* SAT */ {  2 , 0 , 0 , 4 , 4 , 0 , 1 , 0 , 0 , 3 , 3 , 0  },
/* URA */ {  0 , 2 , 0 , 0 , 4 , 0 , 0 , 1 , 0 , 0 , 3 , 0  },
/* NEP */ {  0 , 0 , 4 , 1 , 0 , 4 , 0 , 0 , 3 , 2 , 0 , 3  },
/* PLU */ {  3 , 4 , 0 , 0 , 1 , 0 , 4 , 3 , 0 , 0 , 2 , 0  },
/* RAH */ {  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
/* KET */ {  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
/* LAG */ {  0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  }
/* --------------------------------------------------------   */
/* Houses    1   2   3   4   5   6   7   8   9  10  11  12    */
} ;

const char *sign_dignity[]  = {
                                 " ", 
                                 "E",
                                 "F",
                                 "R",
                                 "D"
                        };

const char *house_dignity[]  = {
                                 "  ",
                                 "AE",
                                 "AF",
                                 "AR",
                                 "AD"
                         };

const char *long_sign_dignity[]  = {
                                 " ", 
                                 "Exalt",
                                 "Fall",
                                 "Ruler",
                                 "Detriment"
                        };

const char *long_house_dignity[]  = {
                                 "  ",
                                 "Acc. Exalt",
                                 "Acc. Fall",
                                 "Acc. Ruler",
                                 "Acc. Detriment"
                         };

const char *l_sign_dignity[]  = {
                                 " ", 
                                 "<m>uccaM</m>",
                                 "<m>niicaM</m>",
                                 "<m>svasthaanaM</m>",
                                 ""
                        };
