#include <ast.h>
#include <string>
#include <vector>
#include<algorithm>
#include <kundali.h>
#include <yogam.h>

string dashapahaaraphalam(int dasha, int apahaaram);

static const char* engMonths[] = {
   "<m>januvari </m>",
   "<m>phebruvari</m>",
   "<m>maaRcc</m>",
   "<m>eepril</m>",
   "<m>mey</m>",
   "<m>juuN</m>",
   "<m>juulai</m>",
   "<m>oogasRR</m>",
   "<m>sepRRaMbaR</m>",
   "<m>okToobaR</m>",
   "<m>navaMbaR</m>",
   "<m>DisambaR</m>"
};

const char* rasi_name[] =
{
   "\\RasiA",
   "\\RasiB",
   "\\RasiC",
   "\\RasiD",
   "\\RasiE",
   "\\RasiF",
   "\\RasiG",
   "\\RasiH",
   "\\RasiI",
   "\\RasiJ",
   "\\RasiK",
   "\\RasiL",
};

const char* lday_name[] = {
   "<m>n~aayaR</m>",
   "<m>tin\"kaL</m>",
   "<m>covva</m>",
   "<m>budhan</m>",
   "<m>vyaazhaM</m>",
   "<m>veLLi</m>",
   "<m>shani</m>"
};

const char* lhind_pl_name[] =
{
   "\\Sun", "\\Moon", "\\Mercury", "\\Venus", "\\Mars", "\\Jupiter", "\\Saturn", "\\Uranus", "\\Neptune", "\\Pluto", "\\Rahu", "\\Ketu", "\\Lagnam", "\\Dashamam"
};

static const char* dasha_name[] =
{
   "<m>keetuRddasha</m>",
   "<m>shukradasha</m>",
   "<m>aadityadasha</m>",
   "<m>candradasha</m>",
   "<m>kujadasha</m>",
   "<m>raahuRddasha</m>",
   "<m>vyaazhadasha</m>",
   "<m>shanidasha</m>",
   "<m>budhadasha</m>"
};

// int indian = 0;

void convert_to_hm(double t, int *h, int *m)
{
   int tm = (int) (t + 0.5);
   *h = tm  / 60;
   *m = tm - *h * 60;
}

char* get_std_time_string(double j_day, double st_long, int date_and_time, char* ts)
{
   // double adj_j_day = convert_time(j_day, 0.0, st_long);
   double adj_j_day = j_day - st_long / 360.0;
   struct date dd;
   struct time tt;
   int h;
   char am_pm = ' ';
   get_date_time(adj_j_day, &dd, &tt);
   if (tt.ti_hour < 12) {
      am_pm = 'A';
      h = tt.ti_hour;
   } else {
      am_pm = 'P';
      h = tt.ti_hour - 12;
   }
   if (h == 0) {
      h = 12;
   }
   if (date_and_time) {
      sprintf(ts, "%02d/%02d/%04d  %02d:%02d %c", dd.da_day,dd.da_mon,
              dd.da_year, h, tt.ti_min, am_pm);
   } else {
      sprintf(ts, "%02d:%02d %c", h, tt.ti_min, am_pm);
   }
   return ts;
}

// Find the sunrise before the time given by the Julian date j_day
double sunrise_before(double j_day, double latitude, double longitude)
{
   double temp1 = j_day;

   // If it is night, find a daytime before this
   double alt = get_sun_altitude(temp1, latitude, longitude);
   while (alt <= 0.0) {
      temp1 -= 0.1;
      alt = get_sun_altitude(temp1, latitude, longitude);
  }
  
   // Now, temp1 is daytime.  Find an interval in which the sunrise occurs  
   double temp2;

   do {
      temp2 = temp1;
      temp1 -= 0.1;
      alt = get_sun_altitude(temp1, latitude, longitude);
   } while (alt > 0);

   // Now, temp1 is night and temp2 is day

   return sunrise_between(temp1, temp2, latitude, longitude);
}

// Find the sunset after the time given by the Julian date j_day
double sunset_after(double j_day, double latitude, double longitude)
{
   double temp1 = j_day;

   // If it is night, find a daytime after this
   double alt = get_sun_altitude(temp1, latitude, longitude);
   while (alt <= 0.0) {
      temp1 += 0.1;
      alt = get_sun_altitude(temp1, latitude, longitude);
  }
  
   // Now, temp1 is daytime.  Find an interval in which the sunset occurs  
   double temp2;
   do {
      temp2 = temp1;
      temp1 += 0.1;
      alt = get_sun_altitude(temp1, latitude, longitude);
   } while (alt > 0);

   // Now, temp1 is day and temp2 is night

   return sunset_between(temp2, temp1, latitude, longitude);
}

static char *ldate_time_string(double j_day,char *any_str, const char* more_formats)
{
   struct date dd ;
   struct time tt ;

   get_date_time(j_day, &dd, &tt);
   fprintf(stderr, "DEBUG: %s = %f (%d/%d/%d)\n", "j_day", j_day, dd.da_year, dd.da_mon-1, dd.da_day);

#if 0
   sprintf(any_str, "{%s %04d} & {%s %s} & {%s %d}", more_formats, dd.da_year, more_formats, engMonths[dd.da_mon-1], more_formats, dd.da_day);
#else
   sprintf(any_str, "{%s %04d/%02d/%02d}", more_formats, dd.da_year, dd.da_mon, dd.da_day);
#endif
   return(any_str);
}

char* ast_angle_string(double angle, char* str, char direction, int indian)
{
   ST_DMS dms;
   int rasi, d, m;
   dms_time(angle, &dms);
   rasi = dms.degrees / 30;
   d = dms.degrees % 30;
   m = dms.minutes;

   if (direction == 'R') {
      sprintf(str, "{\\bf\\em %s} & {\\bf\\em $%d^\\circ~%d'$}", (indian ? rasi_name[rasi] : zod_name[rasi]), d, m);
   } else {
      sprintf(str, "%s & $%d^\\circ~%d'$", (indian ? rasi_name[rasi] : zod_name[rasi]), d, m);
   }
   return str;
}

void lskip_page(FILE* fp)
{
   fprintf(fp, "\\clearpage\n");
}

static char* latitude_string(double latitude, char* str)
{
   char hemisphere;
   // assert (latitude <= 90.0 && latitude >= -90.0);
   if (latitude > 0.0) {
      hemisphere = 'N';
   } else {
      hemisphere = 'S';
      latitude = -latitude;
   }
   ST_DMS dmtime;
   dms_time (latitude,&dmtime);
   sprintf(str, "$%d^{\\circ}~%2d\'$ %c", dmtime.degrees, dmtime.minutes, hemisphere);
   return str;
}

static char* longitude_string(double longitude, char* str)
{
   char hemisphere;
   // assert (longitude <= 180.0 && longitude >= -180.0);
   if (longitude > 0.0) {
      hemisphere = 'W';
   } else {
      hemisphere = 'E';
      longitude = -longitude;
   }
   ST_DMS dmtime;
   dms_time (longitude,&dmtime);
   sprintf(str, "$%d^{\\circ}~%2d\'$ %c", dmtime.degrees, dmtime.minutes, hemisphere);
   return str;
}

void start_latex_table(FILE* fp, const char* format)
{
   fprintf(fp, "\\begin{table}[H]\n"); 
   start_latex_tabular(fp, format);
}

void start_latex_tabular(FILE* fp, const char* format)
{
   fprintf(fp, "\\begin{tabular}{%s}\n", format);
}

void end_latex_table(FILE* fp, const char* label, const char* caption)
{
  end_latex_tabular(fp);
   if (caption) {
      fprintf(fp, "\\caption{%s}\n", caption);
   }
   if (label) {
      fprintf(fp, "\\label{%s}\n", label);
   }
   fprintf(fp, "\\end{table}\n");
}

void end_latex_tabular(FILE* fp)
{
   fprintf(fp, "\\end{tabular}\n");
}

void print_latex_row(FILE* fp, const char* str)
{
   fprintf(fp, "%s \\\\\n", str);
}

void print_hline(FILE* fp)
{
   fprintf(fp, "\\hline\n");
}

void latex_section(FILE* fp, const char* title)
{
   fprintf(fp, "\\section{%s}\n", title);
}

void latex_subsection(FILE* fp, const char* title)
{
   fprintf(fp, "\\subsection{%s}\n", title);
}


/*   -------------------------------
     Function to print birth details
     -------------------------------
*/
void lprint_birth_details(FILE *fp,DT b_det)
{
   char buf[50];

   // lskip_page(fp);
   fprintf(fp, "\\vspace{\\stretch{2}}\n");
   start_latex_table(fp, "rcl");
   print_hline(fp);
   fprintf(fp, "{\\bf Name} & : & %s \\\\\n", b_det -> name);
   fprintf(fp, "{\\bf Date of Birth} & : & %4d/%2d/%2d \\\\\n", b_det -> year, b_det->month, b_det->day);
   fprintf(fp, "{\\bf Time of Birth} & : & %2dh %2dm %04.2fs \\\\\n", b_det->hour,b_det->minute,b_det->second);
   fprintf(fp, "{\\bf Latitude} & : & %s \\\\\n", latitude_string(b_det -> latitude, buf));
   fprintf(fp, "{\\bf Longitude} & : & %s \\\\\n", longitude_string(b_det -> longitude, buf));
   fprintf(fp, "{\\bf Longitude of Standard Time} & : & %s \\\\\n", longitude_string(b_det -> st_long, buf));
   // print_hline(fp);
   // end_latex_table(fp, "tab:BirthDetails", "Birth Details");

   // fprintf(fp, "\\vspace{\\stretch{2}}\n");

   // start_latex_table(fp, "rcl");
   //print_hline(fp);
   fprintf(fp, "&&\\\\\n");

   fprintf(fp,"{\\bf Mean Local Time } & : & %s \\\\\n",
           time_string(b_det->l_time,TIME,FULL,buf));
   fprintf(fp,"{\\bf Greenwich time } & : & %s \\\\\n",
           time_string(b_det->g_time,TIME,FULL,buf));
   fprintf(fp,"{\\bf Greenwich Sidereal time} & : & %s \\\\\n",
           time_string(b_det->g_sid_time,TIME,FULL,buf));
   fprintf(fp,"{\\bf Local Sidereal time} & : & %s \\\\\n",
           time_string(b_det->l_sid_time,TIME,FULL,buf));
   fprintf(fp,"{\\bf Julian Day Number} & : & %f \\\\\n",b_det->jul_date);
   print_hline(fp);
   // end_latex_table(fp, "tab:BirthAdditionalDetails", "Additional Details");
   end_latex_table(fp, "tab:BirthDetails", "Birth Details");
}


void lprint_planet_details(FILE *fp,ST_PL_DET pl_details[], int indian)
{
   int pl_no, curr_house , curr_zod ;
   ST_DMS *gl,dummy ;
   double geo_long_dm ;
   char ast_ang_str[50];

   // lskip_page(fp);
   latex_section(fp, "Western (Sayana) System");

   start_latex_table(fp, "|l|ll|l|r|l|");
   print_hline(fp);
   print_latex_row(fp, "{\\bf Planet} & \\multicolumn{2}{c|}{{\\bf Position}} & & {\\bf House} & ");
   print_hline(fp);
   for (pl_no = SUN ; pl_no <= DESC_NODE ; pl_no++)
   {
      geo_long_dm = fmod(pl_details[pl_no].geo_long,30.0);
      gl = dms_time(geo_long_dm,&dummy);
      curr_zod = pl_details[pl_no].zod_sign ;
      curr_house = pl_details[pl_no].house_no ;

      fprintf(fp, "%s & %s & %s & %d & %s \\\\\n", pl_name[pl_no], 
              ast_angle_string(pl_details[pl_no].geo_long, ast_ang_str, pl_details[pl_no].direction, indian),
              long_sign_dignity[pl_sign_dignity[pl_no][curr_zod]],
              curr_house,
              long_house_dignity[pl_sign_dignity[pl_no][curr_house-1]]);
   }
   pl_no = ASCENDENT;
   fprintf(fp, "%s & %s & %s & %s & %s \\\\\n", pl_name[pl_no], 
           ast_angle_string(pl_details[pl_no].geo_long, ast_ang_str, ' ', indian), " ", " ", " ");
 
   print_hline(fp);
   /*
   fprintf(fp, "  D: Direction, 'R' indicates that the planet is retrograding\n");
   fprintf(fp, "  *:  E=Exalt,       F=Fall,       R=Ruler,       D=Detriment\n");
   fprintf(fp, " **: AE=Acc. Exalt, AF=Acc. Fall, AR=Acc. Ruler, AD=Acc. Detriment\n");
   print_hline(fp);
   */
   end_latex_table(fp, NULL, "Planetary positions");
}

static void lprint_house(FILE *fp,HP_POS hp_pos,int h_no, int indian)
{
   HP_POS next_hp_pos;
   double first_degree, last_degree ;
   char first_string[80],last_string[80],dummy[80];
   int zod_no,first_zod,last_zod ;

   for (next_hp_pos = hp_pos -> next_hp ;  next_hp_pos -> hp_type == 'P' ;
        next_hp_pos = next_hp_pos -> next_hp)
      ;                      /*       NULL loop       */
   first_degree = norm_angle(hp_pos -> degree) ;
   last_degree = norm_angle(next_hp_pos -> degree) ;
   first_zod = (int) (first_degree / 30);
   last_zod = (int) (last_degree / 30 );
   ast_angle_string(first_degree,first_string,'R', indian);
   ast_angle_string(last_degree,last_string,'R', indian);
   print_hline(fp);
   fprintf(fp,"{\\bf\\em %d} & %s & %s \\\\\n",h_no,first_string,
           last_string);
}

static void lprint_planet(FILE *fp,HP_POS hp_pos, int indian)
{
   char dummy[80];
   fprintf(fp," & \\multicolumn{2}{l|}{%s} & %s \\\\\n",
           pl_name[hp_pos->hp_no], ast_angle_string(norm_angle(hp_pos->degree),dummy,' ', indian));
}


void lprint_hp_pos (FILE *fp,HP_POS hp_pos, int indian)
{
   HP_POS temp_hp_pos = hp_pos ;
   int i = 0,h_no = 0;

   start_latex_table(fp, "|r|lr|lr|");
   print_hline(fp);
   print_latex_row(fp, "{\\bf No.} & \\multicolumn{2}{c|}{{\\bf From}} & \\multicolumn{2}{c|}{{\\bf To}}");
   print_hline(fp);
   while (i++ < 24)
   {
      if (temp_hp_pos -> hp_type == 'H')
         h_no++;
      switch (temp_hp_pos -> hp_type)
      {
        case 'H'   : lprint_house(fp,temp_hp_pos,h_no, indian) ; break ;
        case 'P'   : lprint_planet(fp,temp_hp_pos, indian); break ;
      }
      temp_hp_pos = temp_hp_pos -> next_hp;
   }
   print_hline(fp);
   end_latex_table(fp, NULL, "Houses (Placidus system)");
}

#if 1

class AspectDefinition
{
 private:
   std::string name;
   double value;
   double deviation;
 public:
   AspectDefinition(const std::string& nm, double val, double dev) :
      name (nm), value(val), deviation(dev) 
      {}
   bool isAspected(double val)
      {
         val = norm_angle(val);
         if (::fabs(val - this->value) < deviation) {
            return true;
         }
         val = FULL_CIRCLE - val;
         if (::fabs(val - this->value) < deviation) {
            return true;
         }
         return false;
      }

   bool areAspected(double val1, double val2)
      {
         return this->isAspected(val1-val2);
      }
   std::string getName() const { return name; }
};

typedef std::vector<AspectDefinition> AspectDefinitionList;

class Aspect
{
 public:
   std::string planet1;
   std::string planet2;
   double angle1;
   double angle2;
   std::string aspectString;
   Aspect(const std::string& p1, const std::string& p2, double a1, double a2, const std::string& as) 
      : planet1(p1), planet2(p2), angle1(a1), angle2(a2), aspectString(as) 
      {}
   bool operator < (const Aspect& another) const;
};

bool Aspect::operator < (const Aspect& another) const
{
   if (this->aspectString < another.aspectString) {
      return true;
   }
   if (this->aspectString > another.aspectString) {
      return false;
   }
   if (this->planet1 < another.planet1) {
      return true;
   }
   if (this->planet1 > another.planet1) {
      return false;
   }
   if (this->planet2 < another.planet2) {
      return true;
   }
   if (this->planet2 > another.planet2) {
      return false;
   }
   return true;  // Doesn't happen
   
}

class AspectList
{
public:
   typedef std::vector<Aspect>::iterator iterator;
private:
   std::vector<Aspect> aspects;
   AspectDefinitionList adl;
   
public:
   AspectList();
   void checkAspect(const ST_PL_DET planets[], int p1, int p2);
   std::vector<Aspect>::iterator begin()  { return aspects.begin();}
   std::vector<Aspect>::iterator end()  { return aspects.end();}
};

AspectList::AspectList()
{
   adl.push_back(AspectDefinition("CJN & ($0.0 \\pm 7.0$)", 0.0, 7.0));
   adl.push_back(AspectDefinition("OPP & ($180.0 \\pm 7.0$)", 180.0, 7.0));
   adl.push_back(AspectDefinition("SQR & ($90.0 \\pm 7.0$)", 90.0, 7.0));
   adl.push_back(AspectDefinition("TRI & ($120.0 \\pm 7.0$)", 120.0, 7.0));

   adl.push_back(AspectDefinition("SSX & ($30.0 \\pm 5.0$)", 30.0, 5.0));
   adl.push_back(AspectDefinition("SXT & ($60.0 \\pm 5.0$)", 60.0, 5.0));

   adl.push_back(AspectDefinition("SSQ & ($45.0 \\pm 2.0$)", 45.0, 2.0));
   adl.push_back(AspectDefinition("QTL & ($72.0 \\pm 2.0$)", 72.0, 2.0));
   adl.push_back(AspectDefinition("INC & ($150.0 \\pm 2.0$)", 150.0, 2.0));
}
   

void AspectList::checkAspect(const ST_PL_DET planets[], int planet1, int planet2)
{
   double val1 = planets[planet1].geo_long;
   double val2 = planets[planet2].geo_long;
   AspectDefinitionList::iterator it;
   for (it = adl.begin(); it != adl.end() ; ++it) {
      if (it->areAspected(val1, val2)) {
         Aspect aspect (pl_name[planet1], pl_name[planet2], val1, val2, it->getName());
         aspects.push_back(aspect);
         return;  // Cannot have more than one aspect for two planets
      }
   }
}

void lprint_self_aspects (FILE *fp, ST_PL_DET planets[])
{
   char asp_string[10] ;
   int pl_no,other_pl ;

   AspectList aspects;

   for (pl_no = SUN ; pl_no < MIDHEAVEN ; pl_no++) {
      for (other_pl = pl_no+1; other_pl <= MIDHEAVEN ; ++other_pl) {
         aspects.checkAspect(planets, pl_no, other_pl);
      }
   }
   std::sort(aspects.begin(), aspects.end());
   AspectList::iterator it;
   std::string lastAspect("");
   start_latex_table(fp, "{||l|r||l|r||l|r||}");
   fprintf(fp, "\\hline\n");
   fprintf(fp, "{\\bf Aspect} & {\\bf Definition} & {\\bf Planet 1} & {\\bf Longitude} & {\\bf Planet 2} & {\\bf Longitude} \\\\\n");
   for (it = aspects.begin(); it != aspects.end() ; ++it) {
      if (it->aspectString == lastAspect) {
         fprintf(fp, " & & %s & %lf & %s & %lf\\\\\n", 
                 it->planet1.c_str(), it->angle1, it->planet2.c_str(), it->angle2);
      } else {
         fprintf(fp, "\\hline\n");
         fprintf(fp, "%s & %s & %lf & %s & %lf\\\\\n", 
                 it->aspectString.c_str(), it->planet1.c_str(), it->angle1, it->planet2.c_str(), it->angle2);
         lastAspect = it->aspectString;
      }
   }
   fprintf(fp, "\\hline\n");
   end_latex_table(fp, NULL, "Self-aspects");
}
#endif

static const char *lh_dasa_name[] = { "keetu","shukran","ravi","candran","kujan","raahu",
   "guru","shani","budhan" } ;

void lprint_antar_dasa(FILE *fp,DASA part_dasa)
{
   double sjd,ejd,a_sjd,a_ejd,duration,bjd ;
   int lrd ;
   char start_day[60],end_day[60];
   char title[50];

   lrd = part_dasa -> lord ;
   int dasha = lrd;
   sprintf(title, "<m>dashaakaalaM : %s</m>", lh_dasa_name[lrd]);
   sjd = part_dasa -> start_j_day ;
   ejd = part_dasa -> end_j_day ;
   bjd = ejd - dasa_yr[lrd] * D_IN_Y ;
   duration =  ejd - bjd ;

   fprintf(stderr, "bjd = %f\n", bjd);
   fprintf(stderr, "sjd = %f\n", sjd);

   ldate_time_string(sjd,start_day, "");
   ldate_time_string(ejd,end_day, "");
   fprintf(fp, "\\subsubsection{%s (%s <m>mutal</m> %s <m>vare</m>)}\n", dasha_name[dasha], start_day, end_day);

   if (show_forecast) {
     start_latex_table(fp, "|l|l|l|p{3.5in}|");
     print_hline(fp);
     print_latex_row(fp, "{\\bf <m>apahaaraM</m>} & {\\bf <m>tuTakkaM</m>} & {\\bf <m>oTukkaM</m>} & {\\bf <m>phalaM</m>}");
     print_hline(fp);
   } else {
     start_latex_table(fp, "|l|l|l|");
     print_hline(fp);
     print_latex_row(fp, "{\\bf <m>apahaaraM</m>} & {\\bf <m>tuTakkaM</m>} & {\\bf <m>oTukkaM</m>}");
     print_hline(fp);
   }
   if (fabs(sjd - bjd) > 0.00001 )
   {
      while ( bjd < sjd )
      {
         bjd += duration * dasa_yr[lrd] / 120.0 ;
         lrd = (lrd == 8 ? 0 : lrd + 1);
      }
      a_sjd = sjd ;
      a_ejd = bjd ;
      ldate_time_string(a_sjd,start_day, "");
      ldate_time_string(a_ejd,end_day, "");
      lrd = (lrd == 0 ? 8 : lrd -1 );
      if (show_forecast) {
        fprintf(fp,"<m>%s</m> & %s & %s & %s\\\\\n",lh_dasa_name[lrd],start_day,
                end_day, dashapahaaraphalam(dasha, lrd).c_str());
      } else {
        fprintf(fp,"<m>%s</m> & %s & %s \\\\\n",lh_dasa_name[lrd],start_day,
                end_day);
      }
      lrd = (lrd == 8 ? 0 : lrd + 1);
   }
   else
      a_ejd = sjd ;
   do
   {
      a_sjd = a_ejd ;
      a_ejd = a_sjd + duration * dasa_yr[lrd] / 120.0  ;
      if ((a_ejd - ejd) > 0.0001)
         break ;
      ldate_time_string(a_sjd,start_day, "");
      ldate_time_string(a_ejd,end_day, "");
      if (show_forecast) {
        fprintf(fp," <m>%s</m> & %s & %s & %s\\\\\n",lh_dasa_name[lrd],start_day,
                end_day, dashapahaaraphalam(dasha, lrd).c_str());
      } else {
        fprintf(fp," <m>%s</m> & %s & %s \\\\\n",lh_dasa_name[lrd],start_day,
                end_day);
      }
      lrd++;
      if (lrd > 8)
         lrd = 0 ;
   }
   while (lrd != part_dasa -> lord);
   print_hline(fp);
   end_latex_table(fp, NULL, title);
}

static const char* get_nityayogam(int n)
{
   static const char* nityayogams[] =
      {
         "<m>viSkaMbhaM</m>",
         "<m>priiti</m>",
         "<m>aayuSmaan</m>",
         "<m>saubhaagyam</m>",
         "<m>shoobhanaM</m>",
         "<m>atigaNDaM</m>",
         "<m>sukaRmmaa</m>",
         "<m>dh.rti</m>",
         "<m>shuulaM</m>",
         "<m>gaNDa</m>",
         "<m>v.rddhi</m>",
         "<m>dhruvam</m>",
         "<m>vyaaghaataM</m>",
         "<m>haRSaNaM</m>",
         "<m>vajra</m>",
         "<m>siddhi</m>",
         "<m>vyatiipaata</m>",
         "<m>variiyaan</m>",
         "<m>pariigha</m>",
         "<m>shivaH</m>",
         "<m>saaddhyaH</m>",
         "<m>shubhaH</m>",
         "<m>shubhraH</m>",
         "<m>braahmaH</m>",
         "<m>maakhaataH</m>",
         "<m>vaidh.rtaH</m>"
      };
   // assert ( n>=0 && n<= 26);
   return nityayogams[n];
}

static const char* get_karanam(int k) 
{
   static const char* commonKaranams[] =
      {
         "<m>siMhaM</m>",
         "<m>puli</m>",
         "<m>panni</m>",
         "<m>kazhuta</m>",
         "<m>aana</m>",
         "<m>surabhi</m>",
         "<m>viSTi</m>"
      };
   // assert(k >= 0 && k <= 59);
   if (0 == k) {
      return "<m>puzhu</m>";
   } else if (57 == k) {
      return "<m>shakuni</m>";
   } else if (58 == k) {
      return "<m>catuSpaatt</m>";
   } else if (59 == k) {
      return "<m>naagaM</m>";
   } else {
      return commonKaranams[(k-1)%7];
   }
}


void lhindu_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                   double hs[])
{
   int ds_no,nak,paksham,thithi, house;
   ST_DASA ds_det[9];
   double l_moont,sun_moon_diff,j_day,d_thithi,d_nak ;
   char rs[30];
   double lagna = hs[0];
   char ast_angl_str[50];
   int pl_no,hind_sign,nav_sign,sign_no ;
   int next_no[12],next_nav[12],sign_pl[12][10],nav_pl[12][10] ;
   ST_DMS *hind_dm,dummy1 ;
   double h_long,nav ;
   char ay_str[30];

   int indian = 1;
   lskip_page(fp);
   latex_section(fp, "Indian (Nirayana) System");
   fprintf(fp,"  <m>ayanaaMshaM</m> = %s\n\n",time_string(ayanamsa,DEGREE,FULL,ay_str));
   latex_subsection(fp, "<m>grahasphuTan\"n\"aL</m>");
   for ( sign_no = 0 ; sign_no < 12 ; sign_no++)
      next_no[sign_no] = next_nav[sign_no] = 0 ;
   for ( sign_no = 0 ; sign_no < 12 ; sign_no++)
   {
      for ( pl_no = 0 ; pl_no < 10 ; pl_no++)
      {
         sign_pl[sign_no][pl_no] = nav_pl[sign_no][pl_no] = -1 ;
      }
   }
   hind_sign = (int) (hind_long(lagna) / 30.0);
   sign_pl[hind_sign][next_no[hind_sign]] = ASCENDENT ;
   (next_no[hind_sign])++;
   nav_sign = (int) (navamsa(hind_long(lagna)));
   nav_pl[nav_sign][next_nav[nav_sign]] = ASCENDENT ;
   (next_nav[nav_sign])++;

   start_latex_table(fp, "|l|ll|l|l|r|l|");
   print_hline(fp);
   print_latex_row(fp, 
     "{\\bf <m>grahaM</m>} & \\multicolumn{2}{c|}{{\\bf <m>sphuTaM</m>}} & {\\bf <m>navaaMshaM</m>} & {\\bf <m>nakSatraM</m>} & {\\bf <m>bhaavaM</m>} & {\\bf <m>balam</m>} ");
   print_hline(fp);
   // Find Lagnam
   double lag_long = hind_long(pl_details[ASCENDENT].geo_long);
   int lag_sign = (int) (lag_long / 30.0);

   for (pl_no = SUN ; pl_no <= ASCENDENT ; pl_no++)
   {
      int pl_nak, bhavam;
      h_long = hind_long(pl_details[pl_no].geo_long);
      hind_sign = (int) (h_long / 30.0);
      hind_dm = dms_time(h_long - 30.0 * hind_sign,&dummy1);
      nav = navamsa(h_long);
      nav_sign = (int)nav ;
      pl_nak = (int) nakshatra(h_long);
      bhavam = (hind_sign - lag_sign + 12) % 12 + 1;
      // printf("hind_sign = %d, lag_sign = %d, bhavam = %d\n", hind_sign, lag_sign, bhavam);
      fprintf(fp, "%s & %s & %s & \\Star%c & %d & %s\\\\\n",
              lhind_pl_name[pl_no], ast_angle_string(h_long, ast_angl_str, pl_details[pl_no].direction, indian), 
              rasi_name[nav_sign], ((pl_nak < 26) ? 'A' + pl_nak : 'a' + pl_nak-26), bhavam,
              l_sign_dignity[pl_sign_dignity[pl_no][hind_sign]]);

      if ((pl_no < ASCENDENT) && (pl_no <= SATURN || pl_no >= ASC_NODE)) {
         sign_pl[hind_sign][next_no[hind_sign]] = pl_no ;
         (next_no[hind_sign])++;
         nav_pl[nav_sign][next_nav[nav_sign]] = pl_no ;
         (next_nav[nav_sign])++;
      }
   }

   print_hline(fp);
   end_latex_table(fp, NULL, "<m>grahasthaanan\"n\"aL</m>");
   latex_subsection(fp, "<m>bhaavan\"n\"aL</m>");
   start_latex_table(fp, "||l|l r||l|l r||");
   print_hline(fp);
   print_latex_row(fp, "<m>bhaavaM</m> & \\multicolumn{2}{c||}{<m>sphuTam</m>} & <m>bhaavaM</m> & \\multicolumn{2}{c|}{<m>sphuTam</m>}");
   print_hline(fp);
   for ( house = 0 ; house < 6 ; house++)
   {
      fprintf(fp,"%d &  %s &",
              house+1,ast_angle_string(hind_long(hs[house]), rs, ' ', indian));
      fprintf(fp,"%d & %s \\\\\n",
              house+7, ast_angle_string(hind_long(hs[house+6]), rs, ' ', indian));
   }
   print_hline(fp);
   end_latex_table(fp, NULL, "<m>bhaavan\"n\"aL</m>");
   l_moont = hind_long(pl_details[MOON].geo_long);
   j_day = birth_info->j_date  ;
   fprintf(stderr, "l_moont = %f on %s:%d\n", l_moont, __FILE__, __LINE__);
   fprintf(stderr, "j_day = %f on %s:%d\n", j_day, __FILE__, __LINE__);
   fprintf(stderr, "birth_info->st_long = %f on %s:%d\n",birth_info->st_long , __FILE__, __LINE__);
   fprintf(stderr, "birth_info->j_date = %f on %s:%d\n",birth_info->j_date, __FILE__, __LINE__);
   fprintf(stderr, "birth_info->latitude = %f on %s:%d\n",birth_info->latitude, __FILE__, __LINE__);
   fprintf(stderr, "birth_info->longitude = %f on %s:%d\n",birth_info->longitude, __FILE__, __LINE__);
   d_nak = get_dasas(l_moont,j_day - birth_info->st_long/360.0, ds_det);
   nak = (int) d_nak ;
   d_nak = (d_nak - nak) * 100.0 ;
   sun_moon_diff = norm_angle(pl_details[MOON].geo_long -
                              pl_details[SUN].geo_long);
   /*
   paksham = (int) (sun_moon_diff / 180.0);
   d_thithi = (sun_moon_diff - 180.0 * paksham) / 12.0 + 1.0 ;
   if (d_thithi > 15.0 )
      d_thithi -= 15.0 ;
   */
   d_thithi = sun_moon_diff / 12.0;
   thithi = (((int) d_thithi) + 1 ) % 30 ;

   double d_nityayogam = norm_angle(pl_details[MOON].geo_long +  pl_details[SUN].geo_long - 2 * ayanamsa)/360.0 * 27;
   int nityayogam =  (((int) d_nityayogam)) % 27;
   // printf("d_nityayogam = %lf, nityayogam = %d\n",d_nityayogam, nityayogam );

   double d_karanam = sun_moon_diff / 24.0;
   int karanam = (((int) d_karanam) + 1) % 60;

   latex_subsection(fp, "<m>pan~caaMgan\"n\"aL</m>");
   start_latex_table(fp, "lcl");
   fprintf(fp, "\\Thithi & = & \\Thithi%c \\\\\n", ((thithi < 26) ? 'A' + thithi : 'a' + thithi-26));
   fprintf(fp, "\\Varam & = & %s \\\\\n", lday_name[weekday(j_day, birth_info->st_long)]);
   fprintf(fp, "\\Nakshatram & = & \\Star%c{} (<m>paadaM </m>%d) \\\\\n", 
           ((nak < 26) ? 'A' + nak : 'a' + nak-26)
           ,(int)(d_nak / 25.0 + 1));
   
   fprintf(fp, "<m>nityayoogaM</m> & = & %s\\\\\n", get_nityayogam(nityayogam));
   fprintf(fp, "<m>karaNaM</m> & = & %s\\\\\n", get_karanam(karanam));
   end_latex_table(fp, NULL, NULL);
   lskip_page(fp);
   latex_subsection(fp, "<m>grahasthitiyuM navaaMshakavuM</m>");
   int cha_sign = (int) (l_moont / 30.0);
   int lag_nav = (int)(navamsa(hind_long(pl_details[ASCENDENT].geo_long)));
   int cha_nav = (int)(navamsa(l_moont));
   ldraw_charts(fp,sign_pl,nav_pl, lag_sign, cha_sign, lag_nav, cha_nav, 1);
   lskip_page(fp);
   latex_subsection(fp, "<m>dashakaLuM apahaaran\"n\"aLuM</m>");
   for (ds_no = 0 ; ds_no < 9 ; ds_no++)
   {
      lprint_antar_dasa(fp,&(ds_det[ds_no]));
   }

   // Print Yogams
   Kundali kundali(pl_details, ayanamsa);

   vector<Yogam*> yogams;
   yogams.push_back(new RuchakaYogam(&kundali));
   yogams.push_back(new BhadrakaYogam(&kundali));
   yogams.push_back(new HamsaYogam(&kundali));
   yogams.push_back(new MalavyaYogam(&kundali));
   yogams.push_back(new ShashaYogam(&kundali));
   yogams.push_back(new KesariYogam(&kundali));
   yogams.push_back(new SharabhaYogam(&kundali));
   yogams.push_back(new SunabhaaYogam(&kundali));
   yogams.push_back(new AnabhaaYogam(&kundali));
   yogams.push_back(new DhurudhuraaYogam(&kundali));
   yogams.push_back(new LagnaadhiYogam(&kundali));
   yogams.push_back(new ChandraadhiYogam(&kundali));
   yogams.push_back(new KemadrumaYogam(&kundali));
   yogams.push_back(new VesiYogam(&kundali));
   yogams.push_back(new VasiYogam(&kundali));
   yogams.push_back(new UbhayachariYogam(&kundali));
   yogams.push_back(new SushubhaaYogam(&kundali));
   yogams.push_back(new AshubhaaYogam(&kundali));
   yogams.push_back(new KarttariYogam(&kundali));
   yogams.push_back(new LagnaVasumadYogam(&kundali));
   yogams.push_back(new ChandraVasumadYogam(&kundali));
   yogams.push_back(new NipunaYogam(&kundali));
   yogams.push_back(new ShakataYogam(&kundali));
   yogams.push_back(new ShashiMangalaYogam(&kundali));
   yogams.push_back(new VeenaaYogam(&kundali));
   yogams.push_back(new DamaYogam(&kundali));
   yogams.push_back(new PaashaYogam(&kundali));
   yogams.push_back(new KedaaraYogam(&kundali));
   yogams.push_back(new ShoolaYogam(&kundali));
   yogams.push_back(new YugaYogam(&kundali));
   yogams.push_back(new GolaYogam(&kundali));
   vector<Yogam*>::iterator it;

   fprintf(fp, "\\section{<m>yoogan\"n\"aL:</m>}\n\n");
   fprintf(fp, "(<m>apuuR{}NNaM:  31 yoogan\"n\"aL maatramee iviTe parishoodhikkunnuLLuu.</m>)");
   fprintf(fp, "\\begin{enumerate}\n");

   int nYogams = 0;
   for (it = yogams.begin(); it != yogams.end(); ++it) {
      Yogam* yp(*it);
      vector<int> planets;
      if (yp->checkYogam(planets)) {
         yp->printYogam(fp, planets);
         ++nYogams;
      } 
   }

   if (0 == nYogams) {
      fprintf(fp, "\\item{<m>yoogan\"n\"aLonnumilla.</m>}\n");
   }
   fprintf(fp, "\\end{enumerate}\n");
   
   // Testing
   if (false) {
      fprintf(fp, "\\section{Testing}\n");
      vector<int> planets;
      fprintf(fp, "\\begin{enumerate}\n");
      for (it = yogams.begin(); it != yogams.end(); ++it) {
         Yogam* yp(*it);
         yp->printYogam(fp, planets);
      }
      fprintf(fp, "\\end{enumerate}\n");
   }

   // Collect the garbage
   for (it = yogams.begin(); it != yogams.end(); ++it) {
      delete *it;
   }
}

void lmuh_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                   double hs[])
{
   int ds_no,nak,paksham,thithi, house;
   ST_DASA ds_det[9];
   double l_moont,sun_moon_diff,j_day,d_thithi,d_nak ;
   char rs[30];
   double lagna = hs[0];
   char ast_angl_str[50];
   int pl_no,hind_sign,nav_sign,sign_no ;
   int next_no[12],next_nav[12],sign_pl[12][10],nav_pl[12][10] ;
   ST_DMS *hind_dm,dummy1 ;
   double h_long,nav ;
   char ay_str[30];

   int indian = 1;
#if 0
   lskip_page(fp);
   latex_section(fp, "Indian (Nirayana) System");
   fprintf(fp,"  <m>ayanaaMshaM</m> = %s\n\n",time_string(ayanamsa,DEGREE,FULL,ay_str));
   latex_subsection(fp, "<m>grahasphuTan\"n\"aL</m>");
#endif
   for ( sign_no = 0 ; sign_no < 12 ; sign_no++)
      next_no[sign_no] = next_nav[sign_no] = 0 ;
   for ( sign_no = 0 ; sign_no < 12 ; sign_no++)
   {
      for ( pl_no = 0 ; pl_no < 10 ; pl_no++)
      {
         sign_pl[sign_no][pl_no] = nav_pl[sign_no][pl_no] = -1 ;
      }
   }
   hind_sign = (int) (hind_long(lagna) / 30.0);
   sign_pl[hind_sign][next_no[hind_sign]] = ASCENDENT ;
   (next_no[hind_sign])++;
   nav_sign = (int) (navamsa(hind_long(lagna)));
   nav_pl[nav_sign][next_nav[nav_sign]] = ASCENDENT ;
   (next_nav[nav_sign])++;

#if 0
   start_latex_table(fp, "|l|ll|l|l|r|l|");
   print_hline(fp);
   print_latex_row(fp, 
     "{\\bf <m>grahaM</m>} & \\multicolumn{2}{c|}{{\\bf <m>sphuTaM</m>}} & {\\bf <m>navaaMshaM</m>} & {\\bf <m>nakSatraM</m>} & {\\bf <m>bhaavaM</m>} & {\\bf <m>balam</m>} ");
   print_hline(fp);
#endif

   // Find Lagnam
   double lag_long = hind_long(pl_details[ASCENDENT].geo_long);
   int lag_sign = (int) (lag_long / 30.0);

   for (pl_no = SUN ; pl_no <= ASCENDENT ; pl_no++)
   {
      int pl_nak, bhavam;
      h_long = hind_long(pl_details[pl_no].geo_long);
      hind_sign = (int) (h_long / 30.0);
      hind_dm = dms_time(h_long - 30.0 * hind_sign,&dummy1);
      nav = navamsa(h_long);
      nav_sign = (int)nav ;
      pl_nak = (int) nakshatra(h_long);
      bhavam = (hind_sign - lag_sign + 12) % 12 + 1;
      // printf("hind_sign = %d, lag_sign = %d, bhavam = %d\n", hind_sign, lag_sign, bhavam);
#if 0
      fprintf(fp, "%s & %s & %s & \\Star%c & %d & %s\\\\\n",
              lhind_pl_name[pl_no], ast_angle_string(h_long, ast_angl_str, pl_details[pl_no].direction, indian), 
              rasi_name[nav_sign], ((pl_nak < 26) ? 'A' + pl_nak : 'a' + pl_nak-26), bhavam,
              l_sign_dignity[pl_sign_dignity[pl_no][hind_sign]]);
#endif
      if ((pl_no < ASCENDENT) && (pl_no <= SATURN || pl_no >= ASC_NODE)) {
         sign_pl[hind_sign][next_no[hind_sign]] = pl_no ;
         (next_no[hind_sign])++;
         nav_pl[nav_sign][next_nav[nav_sign]] = pl_no ;
         (next_nav[nav_sign])++;
      }
   }

#if 0
   print_hline(fp);
   end_latex_table(fp, NULL, "<m>grahasthaanan\"n\"aL</m>");
   latex_subsection(fp, "<m>bhaavan\"n\"aL</m>");
   start_latex_table(fp, "||l|l r||l|l r||");
   print_hline(fp);
   print_latex_row(fp, "<m>bhaavaM</m> & \\multicolumn{2}{c||}{<m>sphuTam</m>} & <m>bhaavaM</m> & \\multicolumn{2}{c|}{<m>sphuTam</m>}");
   print_hline(fp);
   for ( house = 0 ; house < 6 ; house++)
   {
      fprintf(fp,"%d &  %s &",
              house+1,ast_angle_string(hind_long(hs[house]), rs, ' ', indian));
      fprintf(fp,"%d & %s \\\\\n",
              house+7, ast_angle_string(hind_long(hs[house+6]), rs, ' ', indian));
   }
   print_hline(fp);
   end_latex_table(fp, NULL, "<m>bhaavan\"n\"aL</m>");
#endif
   l_moont = hind_long(pl_details[MOON].geo_long);
   j_day = birth_info->j_date  ;
   d_nak = get_dasas(l_moont,j_day - birth_info->st_long/360.0, ds_det);
   nak = (int) d_nak ;
   d_nak = (d_nak - nak) * 100.0 ;
   sun_moon_diff = norm_angle(pl_details[MOON].geo_long -
                              pl_details[SUN].geo_long);
   /*
   paksham = (int) (sun_moon_diff / 180.0);
   d_thithi = (sun_moon_diff - 180.0 * paksham) / 12.0 + 1.0 ;
   if (d_thithi > 15.0 )
      d_thithi -= 15.0 ;
   */
   d_thithi = sun_moon_diff / 12.0;
   thithi = (((int) d_thithi) + 1 ) % 30 ;

   double d_nityayogam = norm_angle(pl_details[MOON].geo_long +  pl_details[SUN].geo_long - 2 * ayanamsa)/360.0 * 27;
   int nityayogam =  (((int) d_nityayogam)) % 27;
   // printf("d_nityayogam = %lf, nityayogam = %d\n",d_nityayogam, nityayogam );

   double d_karanam = sun_moon_diff / 24.0;
   int karanam = (((int) d_karanam) + 1) % 60;

#if 0
   latex_subsection(fp, "<m>pan~caaMgan\"n\"aL</m>");
#endif

   // Find sunrise and sunset
   double sunrise = sunrise_before(j_day, birth_info->latitude, birth_info->longitude);
   double sunset = sunset_after(j_day, birth_info->latitude, birth_info->longitude);

   printf("%12.2f  %12.2f %12.2f\n", j_day, sunrise, sunset);

   // Find madhyaahnam, abhijit muhoortham etc.
   double day_length = sunset - sunrise;
   double madhyahna_start = sunrise + 0.4 * day_length;
   double madhyahna_end = sunrise + 0.6 * day_length;
   double abhijith_start = sunrise + 0.5 * day_length - 1.0 / 60;
   double abhijith_end = sunrise + 0.5 * day_length + 1.0 / 60;


   // Find udayaalparam
   double up_days = j_day - sunrise; // in days
   double up_minutes = up_days * 24.0 * 60.0;
   double up_vinazhikas = up_days * 60.0 * 60.0;

   int h,m;
   convert_to_hm(up_minutes, &h, &m);

   int n,v;
   convert_to_hm(up_vinazhikas, &n, &v);

  
   lskip_page(fp);
   latex_subsection(fp, "<m>muhuuRttam</m>");
   start_latex_table(fp, "lclp{0.1in}lcl");
   char ts[30], ts1[30];
   fprintf(fp, "<m>samayam</m>& = & {\\tt %s} &&", get_std_time_string(j_day, birth_info->st_long, 1, ts));
   fprintf(fp, "\\Thithi & = & \\Thithi%c \\\\\n", ((thithi < 26) ? 'A' + thithi : 'a' + thithi-26));

   fprintf(fp, "<m>talee udayam</m>& = &{\\tt  %s} &&", get_std_time_string(sunrise, birth_info->st_long, 1, ts));
   fprintf(fp, "\\Varam & = & %s \\\\\n", lday_name[weekday(j_day, birth_info->st_long)]);

   fprintf(fp, "<m>piRRee astamayam</m>& = & {\\tt %s} &&", get_std_time_string(sunset, birth_info->st_long, 1, ts));
   fprintf(fp, "\\Nakshatram & = & \\Star%c{} (<m>paadaM </m>%d) \\\\\n", 
           ((nak < 26) ? 'A' + nak : 'a' + nak-26)
           ,(int)(d_nak / 25.0 + 1));
   
   fprintf(fp, "<m>udayaalpparasamayam</m> &  = & {\\tt %d} <m> maNikkuuR </m> {\\tt %d} <m> miniTT</m> &&", h, m);
   fprintf(fp, "<m>nityayoogaM</m> & = & %s\\\\\n", get_nityayogam(nityayogam));

   fprintf(fp, "  && ({\\tt %d} <m> naazhika </m> {\\tt %d} <m> vinaazhika </m>) &&", n, v);
   fprintf(fp, "<m>karaNaM</m> & = & %s\\\\\n", get_karanam(karanam));

   end_latex_table(fp, NULL, NULL);


   fprintf(fp, "\\vspace{6pt}\n\n");

   if (sunset - sunrise < 1.25) {
      start_latex_table(fp, "lcl");
      fprintf(fp, "<m>madhyaahnam</m>& = & {\\tt %s} -- {\\tt %s} \\\\\n", 
              get_std_time_string(madhyahna_start,  birth_info->st_long, 0, ts),
              get_std_time_string(madhyahna_end,  birth_info->st_long, 0, ts1)
         );
      fprintf(fp, "<m>abhijit</m>& = & {\\tt %s} -- {\\tt %s} \\\\\n", 
              get_std_time_string(abhijith_start, birth_info->st_long, 0, ts),
              get_std_time_string(abhijith_end,birth_info->st_long , 0, ts1)
         );
      end_latex_table(fp, NULL, NULL);
   }

   // Print thse details

#if 0
   start_latex_table(fp, "lcl");
   fprintf(fp, "\\Thithi & = & \\Thithi%c \\\\\n", ((thithi < 26) ? 'A' + thithi : 'a' + thithi-26));
   fprintf(fp, "\\Varam & = & %s \\\\\n", lday_name[weekday(j_day, birth_info->st_long)]);
   fprintf(fp, "\\Nakshatram & = & \\Star%c{} (<m>paadaM </m>%d) \\\\\n", 
           ((nak < 26) ? 'A' + nak : 'a' + nak-26)
           ,(int)(d_nak / 25.0 + 1));
   
   fprintf(fp, "<m>nityayoogaM</m> & = & %s\\\\\n", get_nityayogam(nityayogam));
   fprintf(fp, "<m>karaNaM</m> & = & %s\\\\\n", get_karanam(karanam));
   end_latex_table(fp, NULL, NULL);
#endif

#if 0
   lskip_page(fp);
   latex_subsection(fp, "<m>grahasthitiyuM navaaMshakavuM</m>");
#endif

   int cha_sign = (int) (l_moont / 30.0);
   int lag_nav = (int)(navamsa(hind_long(pl_details[ASCENDENT].geo_long)));
   int cha_nav = (int)(navamsa(l_moont));
   ldraw_charts(fp,sign_pl,nav_pl, lag_sign, cha_sign, lag_nav, cha_nav, 0);
#if 0
   lskip_page(fp);
   latex_subsection(fp, "<m>dashakaLuM apahaaran\"n\"aLuM</m>");
   for (ds_no = 0 ; ds_no < 9 ; ds_no++)
   {
      lprint_antar_dasa(fp,&(ds_det[ds_no]));
   }

   // Print Yogams
   Kundali kundali(pl_details, ayanamsa);

   vector<Yogam*> yogams;
   yogams.push_back(new RuchakaYogam(&kundali));
   yogams.push_back(new BhadrakaYogam(&kundali));
   yogams.push_back(new HamsaYogam(&kundali));
   yogams.push_back(new MalavyaYogam(&kundali));
   yogams.push_back(new ShashaYogam(&kundali));
   yogams.push_back(new KesariYogam(&kundali));
   yogams.push_back(new SharabhaYogam(&kundali));
   yogams.push_back(new SunabhaaYogam(&kundali));
   yogams.push_back(new AnabhaaYogam(&kundali));
   yogams.push_back(new DhurudhuraaYogam(&kundali));
   yogams.push_back(new LagnaadhiYogam(&kundali));
   yogams.push_back(new ChandraadhiYogam(&kundali));
   yogams.push_back(new KemadrumaYogam(&kundali));
   yogams.push_back(new VesiYogam(&kundali));
   yogams.push_back(new VasiYogam(&kundali));
   yogams.push_back(new UbhayachariYogam(&kundali));
   yogams.push_back(new SushubhaaYogam(&kundali));
   yogams.push_back(new AshubhaaYogam(&kundali));
   yogams.push_back(new KarttariYogam(&kundali));
   yogams.push_back(new LagnaVasumadYogam(&kundali));
   yogams.push_back(new ChandraVasumadYogam(&kundali));
   yogams.push_back(new NipunaYogam(&kundali));
   yogams.push_back(new ShakataYogam(&kundali));
   yogams.push_back(new ShashiMangalaYogam(&kundali));
   yogams.push_back(new VeenaaYogam(&kundali));
   yogams.push_back(new DamaYogam(&kundali));
   yogams.push_back(new PaashaYogam(&kundali));
   yogams.push_back(new KedaaraYogam(&kundali));
   yogams.push_back(new ShoolaYogam(&kundali));
   yogams.push_back(new YugaYogam(&kundali));
   yogams.push_back(new GolaYogam(&kundali));
   vector<Yogam*>::iterator it;

   fprintf(fp, "\\section{<m>yoogan\"n\"aL:</m>}\n\n");
   fprintf(fp, "\\begin{enumerate}\n");

   int nYogams = 0;
   for (it = yogams.begin(); it != yogams.end(); ++it) {
      Yogam* yp(*it);
      vector<int> planets;
      if (yp->checkYogam(planets)) {
#if 0
         int mainYogam = yp->getMainYogam();
         if (mainYogam != lastMainYogam) {
            if (mainYogam > (lastMainYogam +1)) {
               for (int i=lastMainYogam+1; i< mainYogam ; ++i) {
                  fprintf(fp, "\\subsection{%s}\n", Yogam::mainYogaName[i]);
                  fprintf(fp, "<m>ii jaatakattil %s onnuMtanne illa.</m>\n", Yogam::mainYogaName[i]);
               }
            }
            fprintf(fp, "\\subsection{%s}\n", Yogam::mainYogaName[mainYogam]);
            mainYogam = lastMainYogaM;
         }
#endif
         yp->printYogam(fp, planets);
         ++nYogams;
      } 
   }

   if (0 == nYogams) {
      fprintf(fp, "\\item{<m>yoogan\"n\"aLonnumilla.</m>}\n");
   }
   fprintf(fp, "\\end{enumerate}\n");
   
   // Testing
   if (false) {
      fprintf(fp, "\\section{Testing}\n");
      vector<int> planets;
      fprintf(fp, "\\begin{enumerate}\n");
      for (it = yogams.begin(); it != yogams.end(); ++it) {
         Yogam* yp(*it);
         yp->printYogam(fp, planets);
      }
      fprintf(fp, "\\end{enumerate}\n");
   }

   // Collect the garbage
   for (it = yogams.begin(); it != yogams.end(); ++it) {
      delete *it;
   }
#endif
}

string dashapahaaraphalam(int dasha, int apaharam)
{
   printf("%d, %d\n", dasha, apaharam);
   switch(dasha) {
      case DASA_KETU :
         switch(apaharam) {
            case DASA_KETU :
               return ("<m>shatrupiiDa, kuTuMbakalahaM, svajanaviroodhaM, apakaTan\"n\"aL, dhananaSTaM, manaHkleeshaM</m>");
            case DASA_SUKRA :
               return ("<m>aTutta bandhattiluLLavaRkk aapatt, svajanaviroodhaM, yaatraakleeshaM, kaaRyataTassaM.</m>");
            case DASA_RAVI :
               return ("<m>pit.rtulyaraayavaRkk aapatt, svajanaviroodhaM, gavaNmenR saMbandhamaayi dooSaM.</m>");
            case DASA_CHANDRA :
               return ("<m>palavidhattil dhanalaabhaM, celavu kuuTutal, santaanan\"n\"aLuTe veeRpaaT, aaroogyakkuRav.</m>");
            case DASA_KUJA :
               return ("<m>svajanaviroodhaM, bhaaRyaaputraadikaLkku roogapiiDayuM duritan\"n\"aLuM, agnibhayaM.</m>");
            case DASA_RAHU :
               return ("<m>matsaran\"n\"aLil eeRppeTeeNTi varika, gavaNmenR saMbandhamaayi dooSaM, kaaRyataTassaM, roogapiiDa, dhananaSTaM, manaHkleeshaM.</m>");
            case DASA_GURU :
               return ("<m>santaanalaabhaM, daivikakaaRyan\"n\"aLil taatpaRyaM, bhuumilaabhaM, gavaNmenRaanukuulyaM.</m>");
            case DASA_SANI :
               return ("<m>bh.rtyajanan\"n\"aLkku duritaM, shatrupiiDa, puuR{}vvikadhanattinRe naashaM, sthaanabhraMshaM.</m>");
            case DASA_BUDHA :
               return ("<m>bandhukkaLumaayi ceeRcca, bhuumilaabhaM, dhanaabhiv.rddhi, shatrupiiDa, vidyaaguNaM, k.rSinaashaM.</m>");
            default:
               return ("");
         }
      case DASA_SUKRA :
         switch(apaharam) {
            case DASA_SUKRA :
               return ("<m>vishiSTavastran\"n\"aL, aabharaNan\"n\"aL, sugandhadravyan\"n\"aL tuTan\"n\"iyava labhikkuka, laiMgikasukhaM, dhanaabhiv.rddhi, kuTumbaabhiv.rddhi, gavaNmenRaanukuulyaM.</m>");
            case DASA_RAVI :
               return ("<m>neetraroogaM, shirooroogan\"n\"aL, pit.rtulyaraayavaRkk aapatt, k.rSinaashaM, dhananaSTaM.</m>");
            case DASA_CHANDRA :
               return ("<m>raktaduuSyaroogan\"n\"aL, dantaroogaM, dhananaashaM, gavaNmenRaanukuulyaM, vyavasaayapravaRttanaM.</m>");
            case DASA_KUJA :
               return ("<m>raktasaMbandhavuM pittasaMbandhavumaaya asukhan\"n\"aL, k.rSinaashaM, bhuumilaabhaM, loohavastukkaL labhikkuka, kalahaM, manaHkleeshaM.</m>");
            case DASA_RAHU :
               return ("<m>apratiikSitamaaya dhanalaabhaM, santaanaguNaM, bandhuguNaM, matsaran\"n\"aLil vijayaM.</m>");
            case DASA_GURU :
               return ("<m>dhaaRmmikakaaRyan\"n\"aLil taatpaRyaM, daivikakaaRyan\"n\"aL ceyyaan iTayaavuka, kuTuMbasukhaM, dhanaabhiv.rddhi, bhuumilaabhaM, vaahanaguNaM.</m>");
            case DASA_SANI :
               return ("<m>uyaRnna padavikaLilluLLavarumaayi sampaRkkaM, bhaaRyakkuM santaanan\"n\"aLkkuM neeTTan\"n\"aL, matsaran\"n\"aLil vijayaM, bhuumi, viiT ennivayuTe laabhaM.</m>");
            case DASA_BUDHA :
               return ("<m>santaanaguNaM, bandhuguNaM, dhanaabhiv.rddhi, aishvaRyaM, vidyaaguNaM, gavaNmenRilninnu sahaayaM, tvagroogan\"n\"aL.</m>");
            case DASA_KETU :
               return ("<m>dhananaSTaM, manaHkleeshaM, apakaTan\"n\"aL, bandhunaashaM, shatrupiiDa, sthaanabhraMshaM.</m>");
            default:
               return ("");
         }
      case DASA_RAVI :
         switch(apaharam) {
            case DASA_RAVI :
               return ("<m>gavaNmenRaanukuulyaM, yaatraakleeshaM, prashasti, padavi, dhanalaabhaM, celavukuuTutal, pitaavinu duritaM, udararoogaM, apakaTaM, manaHkleeshaM.</m>");
            case DASA_CHANDRA :
               return ("<m>shatrunaashaM, santooSaM, dhanalaabhaM, k.rSiguNaM, putiya viiTuNTaakkuka, bandhuguNaM, raktaduuSyaroogan\"n\"aL, manaHkleeshaM, celavu kuuTutal, maataavinu roogaM.</m>");
            case DASA_KUJA :
               return ("<m>roogapiiDa, sthaanamaaRRaM, shatrupiiDa, muRivucatavukaL, dhananaSTaM, gavaNmenRanukuulyaM, sahoodaraguNaM, sahaayikaLkk aapatt, pittaroogaM.</m>");
            case DASA_RAHU :
               return ("<m>shatrupiiDa, manaHkleeshaM, dhananaSTaM, shirooroogaM, sthaanamaaRRaM, bandhunaashaM, yaatra.</m>");
            case DASA_GURU :
               return ("<m>shatrunaashaM, matsaran\"n\"aLil vijayaM, dhanalaabhaM, daivikakaaRyan\"n\"aLil tatpaRyaM, santaanaguNaM, shirooroogaM, raktasaMbandhamaaya asukhaM.</m>");
            case DASA_SANI :
               return ("<m>dhananaashaM, santaanan\"n\"aLuTe veeRpaat, bhaaRyaykku roogapiiDa, celavu kuuTutal, sthaanamaaRRaM, gavaNmenRu saMbandhamaaya buddhimuTTukaL.</m>");
            case DASA_BUDHA :
               return ("<m>raktaduuSyaroogan\"n\"aL, tvagroogaM, neetraroogaM, bhuumilaabhaM, onniTaviTTu sukhaduHkhan\"n\"aL anubhavappeTuka, celavu kuuTutal.</m>");
            case DASA_KETU :
               return ("<m>svajanaviroodhaM, shatrupiiDa, dhananaSTaM, apakaTan\"n\"aL, muRivucatavukaL, pit.rtulyaraayavaRkk aapatt, paadaroogaM.</m>");
            case DASA_SUKRA :
               return ("<m>shirooroogaM, k.rSikkuM bhuusvattinuM naashaM, bhaaRyaykkuM santaanan\"n\"aLkkuM duritan\"n\"aL, ciitta striikaLil taatpaRyaM, jalayaatra, manaHkleeshaM.</m>");
            default:
               return ("");
         }
      case DASA_CHANDRA :
         switch(apaharam) {
            case DASA_CHANDRA :
               return ("<m>bhaaRyaasukhaM, santaanaguNaM, vishiSTavastraabharaNaadilaabhaM, vyavasaayapravaRttanaM, celavu kuuTutal.</m>");
            case DASA_KUJA :
               return ("<m>raktasaMbandhavuM pittasaMbandhavumaaya roogan\"n\"aL, apakaTaM, agnibhayaM, kaLLanmaaruTe upadravaM, bhuumilaabhaM, manaHkleeshaM, yaatraakleeshaM.</m>");
            case DASA_RAHU :
               return ("<m>manaHkleeshaM, raktaduuSyaroogan\"n\"aL, bandhunaashaM, apakaTaM, dhananaSTaM, muRivucatavukaL.</m>");
            case DASA_GURU :
               return ("<m>dooSashaanti, dhaaRmmikakaaRyan\"n\"aLil taatpaRyaM, gavaNmenRaanukuulyaM, vidyaaguNaM, dhanalaabhaM.</m>");
            case DASA_SANI :
               return ("<m>manaHkleeshaM, roogapiiDa, bandhunaashaM, bhaaRyaykkuM maataavinuM kaSTata, bhuumilaabhaM, vaahanaguNaM.</m>");
            case DASA_BUDHA :
               return ("<m>dhanalaabhaM, vaahanaguNaM, vaLaRttujantukkaLuTe anubhavaM, putiya kaaRyan\"n\"aL paThikkuka.</m>");
            case DASA_KETU :
               return ("<m>manaHsvasthatakkuRav, buddhimuTTukaL, bh.rtyanaashaM, udararoogaM, dhananaSTaM, apakaTan\"n\"aL.</m>");
            case DASA_SUKRA :
               return ("<m>jalayaatra, jalaashayaniRmmaaNaM, vaahanaguNaM, striisukhaM, vyaapaaraM muulaM dhanalaabhaM, santaanaguNaM.</m>");
            case DASA_RAVI :
               return ("<m>gavaNmenRaanukuulyaM, shatrunaashaM, aaroogyapuSTi, udararoogan\"n\"aL, duurayaatra.</m>");
            default:
               return ("");
         }
      case DASA_KUJA :
         switch(apaharam) {
            case DASA_KUJA :
               return ("<m>kaaRyataTassaM, pittasaMbandhamaaya asukhan\"n\"aL, muRivucatavukaL, shatrupiiDa, dhananaSTaM.</m>");
            case DASA_RAHU :
               return ("<m>aayudhattil ninnoo agniyil ninnoo upadravaM, raktaduuSyaroogan\"n\"aL, pit.rtulyaraayavaRkku naashaM.</m>");
            case DASA_GURU :
               return ("<m>dooSashaanti, daivikakaaRyan\"n\"aLil taatpaRyaM, pit.rtulyaraayavaruTe aanukuulyaM, dhanaabhiv.rddhi, santaanaguNaM.</m>");
            case DASA_SANI :
               return ("<m>bandhukkaLkk aapatt, santaanaduritaM, gurujanan\"n\"aLkk aapatt, dhananaSTaM, shatrupiiDa.</m>");
            case DASA_BUDHA :
               return ("<m>shatrupiiDa, kaLLanmaarilninn upadravaM, dhananaSTaM, kaccavaTattil ninn dhanalaabhaM, g.rhaniRmmaaNaM.</m>");
            case DASA_KETU :
               return ("<m>agnibhayaM, muRivucatavukaL, anyadeeshayaatra, bhaaRyaykku roogapiiDa, svajanaduritaM.</m>");
            case DASA_SUKRA :
               return ("<m>matsaran\"n\"aLil vijayaM, maanyata, duuradeeshavaasaM, bh.rtyajananaashaM, vishiSTavastulaabhaM.</m>");
            case DASA_RAVI :
               return ("<m>prataapavuM padaviyuM uNTaavuka, bh.rtyajanasahakaraNaM, kaSTappaaT, dhanalaabhaM, vimaRshanan\"n\"aL.</m>");
            case DASA_CHANDRA :
               return ("<m>dhanaabhiv.rddhi, santaanaguNaM, shatrunaashaM, pit.rtulyaraayavaRkku roogapiiDa, udararoogaM.</m>");
            default:
               return ("");
         }
      case DASA_RAHU :
         switch(apaharam) {
            case DASA_RAHU :
               return ("<m>raktaduuSyaroogaM, viSabhayaM, svajanaduritaM, dhananaSTaM, anyadeeshagamanaM, tozhilguNaM.</m>");
            case DASA_GURU :
               return ("<m>kuTuMbasukhaM, bandhuguNaM, matakaaRyan\"n\"aLil taatpaRyaM, gavaNmenRaanukuulyaM, dhanalaabhaM, santaanaguNaM.</m>");
            case DASA_SANI :
               return ("<m>vaataroogaM, svajanaviroodhaM, bhaaRyaykku roogaM, bhuumilaabhaM, tozhilguNaM, bh.rtyajanaanubhavaM.</m>");
            case DASA_BUDHA :
               return ("<m>santaanaguNaM, bandhuguNaM, putiya kaaRyan\"n\"aL paThikkaaniTayaavuka, gavaNmenRaanukuulyaM, tozhilguNaM.</m>");
            case DASA_KETU :
               return ("<m>roogapiiDa, apakaTan\"n\"aL, muRivucatavukaL, shatrupiiDa, svajanaduritaM, manaHkleeshaM, dhananaSTaM.</m>");
            case DASA_SUKRA :
               return ("<m>kuTuMbasukhaM, g.rhoopakaraNalaabhaM, vaahanaguNaM, bhuumilaabhaM, muutraashayaroogaM, yaatraakleeshaM.</m>");
            case DASA_RAVI :
               return ("<m>pit.rtulyaraayavaRkk aapatt, neetraroogaM, yaatraakleeshaM, dhananaSTaM, gavaNmenRaanukuulyaM.</m>");
            case DASA_CHANDRA :
               return ("<m>manaHkleeshaM, kuTuMbakalahaM, k.rSivaRddhanav, vyavasaayapravaRttanan\"n\"aL muulaM dhanalaabhaM.</m>");
            case DASA_KUJA :
               return ("<m>manaHkleeshaM, apakaTan\"n\"aL, muRivucatavukaL, h.rdayasaMbandhavuM udarasaMbandhavumaaya roogaM.</m>");
            default:
               return ("");
         }
      case DASA_GURU :
         switch(apaharam) {
            case DASA_GURU :
               return ("<m>saubhaagyaM, aishvaRyaM, santaanaguNaM, bandhuguNaM, gavaNmenRaanukuulyaM, dhanapuSTi.</m>");
            case DASA_SANI :
               return ("<m>ciitta kuuTTukeTTU muulaM maanahaani, bhuumilaabhaM, g.rhaniRmaaNaM, tozhilguNaM, dhananaSTaM, yaatraakleeshaM.</m>");
            case DASA_BUDHA :
               return ("<m>kaLikaLil taatpaRyaM, putiya kaaRyan\"n\"aL paThikkaan iTayaavuka, vidyaaguNaM, santaanaguNaM, vyaapaaralaabhaM.</m>");
            case DASA_KETU :
               return ("<m>muRivucatavukaL, dhananaSTaM, svajanaduritaM, bandhunaashaM, bh.rtyajanaduritaM, kuTuMbattil dooSaM.</m>");
            case DASA_SUKRA :
               return ("<m>dhanaabhiv.rddhi, k.rSiyilninnuM vyavasaayattil ninnuM prayoojanaM, daivikakaaRyan\"n\"aLil taatpaRyaM.</m>");
            case DASA_RAVI :
               return ("<m>shatrunaashaM, gavaNmenRaanukuulyaM, prashasti, neet.rtvaM, dhanalaabhaM, pitaavinu roogaM.</m>");
            case DASA_CHANDRA :
               return ("<m>vivaahalaabhaM, santaanalaabhaM, kuTuMbasukhaM, k.rSi, vyavasaayaM tuTan\"n\"iyavayil ninnu dhanalaabhaM.</m>");
            case DASA_KUJA :
               return ("<m>bandhuguNaM, shatrukkaLilninnu dhanalaabhaM, bhuumilaabhaM, neetraroogaM, udararoogaM, dhananaSTaM.</m>");
            case DASA_RAHU :
               return ("<m>manaHkleeshaM, shatrupiiDa, yaatraakleeshaM, dhananaSTaM, svajanaduritaM.</m>");
            default:
               return ("");
         }
      case DASA_SANI :
         switch(apaharam) {
            case DASA_SANI :
               return ("<m>k.rSiyilninnu dhanalaabhaM, bh.rtyaguNaM, anyasahaayaM, v.rddhastriikaLe saMrakSikkeeNTi varika.</m>");
            case DASA_BUDHA :
               return ("<m>saubhaagyaM, sampatt, kuTuMbasukhaM, gavaNmenRaanukuulyaM, bandhukkaLumaayi sahakaraNaM.</m>");
            case DASA_KETU :
               return ("<m>apakaTan\"n\"aL, roogapiiDa, shatrupiiDa, svajanaviroodhaM, viSabhayaM, vaahanaapakaTaM.</m>");
            case DASA_SUKRA :
               return ("<m>bhaaRyaasantaanan\"n\"aLil ninnu guNaM, g.rhoopakaraNalaabhaM, palavidhattiluLLa dhanalaabhaM, bhuumilaabhaM.</m>");
            case DASA_RAVI :
               return ("<m>shatrupiiDa, pit.rtulyaraayavaRkku roogapiiDa, bandhunaashaM, neetraroogaM, dhananaSTam, yaatra.</m>");
            case DASA_CHANDRA :
               return ("<m>aTutta bandhattiluLLavaRkk aapatt, roogapiiDa, raktaduuSyaM, jalabhayaM.</m>");
            case DASA_KUJA :
               return ("<m>sthaanabhraMshaM, dhananaSTaM, svajanakalahaM, sahoodaran\"n\"aLkku duritaM, manaHkleeshaM, shatrupiiDa.</m>");
            case DASA_RAHU :
               return ("<m>ciitta kuuTTukeTTu muulaM dhananaSTavuM maanahaaniyuM, udararoogaM, gavaNmenRu saMbandhamaayi dooSaM.</m>");
            case DASA_GURU :
               return ("<m>daivikakaaRyan\"n\"aLil taatpaRyaM, kuTuMbasukhaM, kaaRyasiddhi, dhanalaabhaM, yaatraakleeshaM.</m>");
            default:
               return ("");
         }
      case DASA_BUDHA :
         switch(apaharam) {
            case DASA_BUDHA :
               return ("<m>nalla aaLukaLumaayi aTuppaM, matakaaRyan\"n\"aLil niSTha, dhanaabhiv.rddhi, prashasti, tozhilguNaM.</m>");
            case DASA_KETU :
               return ("<m>manaHkleeshaM, matsaran\"n\"aL, shatrupiiDa, apakaTan\"n\"aL, vaahanasaMbandhamaayi apakaTaM.</m>");
            case DASA_SUKRA :
               return ("<m>daanashiilaM, kuTuMbasukhaM, g.rhoopakaraNalaabhaM, bhaaRyaasantaanan\"n\"aLuTe uyaRcca.</m>");
            case DASA_RAVI :
               return ("<m>vishiSTavastulaabhaM, vaahanaguNaM, tozhilguNaM, sukhabhoogan\"n\"aL, gavaNmenRaanukuulyaM.</m>");
            case DASA_CHANDRA :
               return ("<m>raktaduuSyaroogan\"n\"aL, shirooroogaM, shatrupiiDa, manaHkleeshaM, m.rgan\"n\"aLilninn upadravaM.</m>");
            case DASA_KUJA :
               return ("<m>shatrupiiDa, agnibhayaM, kaLLanmaaruTe upadravaM, manaHkleeshaM, sahoodaraduritaM.</m>");
            case DASA_RAHU :
               return ("<m>manaHkleeshaM, dhananaSTaM, sthaanabhraMshaM, udararoogaM, neetraroogaM, vidyaaguNaM, tozhilguNaM.</m>");
            case DASA_GURU :
               return ("<m>matsaran\"n\"aLil jayaM, gavaNmenRaanukuulyaM, dhanalaabhaM, bandhukkaLkk buddhimuTT.</m>");
            case DASA_SANI :
               return ("<m>dhananaSTaM, kaaRyataTassaM, roogapiiDa, pravaRttanavijayaM, bhuumilaabhaM, duurayaatra.</m>");
            default:
               return ("");
         }
      default:
         return ("");
   }
}
