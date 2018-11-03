
/*                          ---------
                            AST_LIB.C
                            ---------
          ( Routines for astronomical calculations )
            --------------------------------------
*/

#include "ast.h"

/*     ---------------------------------------
       Function to find and store informations
       ---------------------------------------
*/
void find_birth_info(DT birth_det, ABD birth_info)
{
   struct date birth_date ;
   double birth_time,gr_time,j_day ;

   birth_date.da_year = birth_det -> year ;
   birth_date.da_mon = birth_det -> month ;
   birth_date.da_day = birth_det -> day ;

   birth_time = birth_det->hour +
      + (birth_det->minute + birth_det->second / 60.0
         )/60.0 ;

   birth_det -> decimal_time = birth_time;
   birth_det -> l_time
      = norm_hour(convert_time(birth_time,birth_det->st_long,
                               birth_det->longitude));
   gr_time = convert_time(birth_time,birth_det->st_long,0.0) ;
   j_day = julian_day(&birth_date,gr_time);
   birth_det->g_time = norm_hour(gr_time);
   birth_det -> g_sid_time = gr_sidereal_time(j_day);
   birth_det -> l_sid_time = norm_hour(convert_time(birth_det->g_sid_time,
                                                    0.0,birth_det->longitude));
   birth_det ->jul_date = j_day ;

   birth_info->j_date = j_day ;
   birth_info->latitude = birth_det->latitude ;
   birth_info->longitude = birth_det->longitude;
   birth_info->st_long = birth_det->st_long;
}

/*  -----------------------------------------------------------
    Function to calculate the Julian day number of any instance
    -----------------------------------------------------------
*/

double julian_day (struct date *any_date, double greenwitch_time)
{
   double yy,mm,dd,calc_year,calc_month ;
   double century,quad_century,j_day,j_day1,j_day2 ;

   yy = any_date -> da_year ;
   mm = any_date -> da_mon ;
   dd = any_date -> da_day ;
   if ( mm > 2)
   {
      calc_year = yy ;
      calc_month = mm ;
   }
   else
   {
      calc_year = yy -1 ;
      calc_month = mm + 12 ;
   }
   dd += greenwitch_time / 24.0 ;
   century = floor(calc_year / 100) ;
   quad_century = floor(2 - century + century / 4) ;
   j_day1 = floor(D_IN_Y * calc_year);
   j_day2 = floor(D_IN_M * (calc_month + 1));
   j_day = j_day1 + j_day2 + dd + 1720994.5 + quad_century ;
   return(j_day);
}

/*
double j_day_from_local_time (struct date *l_date, DMS l_time,
                              double longitude)
{
   double decimal_time, gr_time, jul_day ;

   decimal_time = equiv_time(l_time);
   gr_time = convert_time(decimal_time,longitude,0.0);
   jul_day =  julian_day(l_date,gr_time);
   return (jul_day);
}
*/

/*    ---------------------------------------------------------
      Function to convert time according to change in longitude
      ---------------------------------------------------------
*/
double convert_time(double old_time,double old_long,double new_long)
{
   double long_diff,time_diff,new_time ;

   long_diff = new_long - old_long ;
   time_diff = long_diff / 15.0 ;
   new_time = old_time - time_diff ;
   return (new_time);
}

/*     --------------------------------------------------------
       Function to convert the Julian date to the date and time
       --------------------------------------------------------
*/

void get_date_time(double j_day, struct date *dd, struct time *tt)
{
   double int_part, fr_part, a, b,c,d,e,f_day,f_hour,f_minute,f_second ;
   int year,month,day,hour,minute,g ;

   j_day += 0.5 ;
   int_part = floor(j_day);
   fr_part = j_day - int_part ;
   if (int_part > 2299160.0)
   {
      a = floor((int_part - 1867216.25)/ 36524.25);
      b = int_part + 1 + a - floor(a/4.0);
   }
   else 
      b = int_part ;
   c = b + 1524 ;
   d = floor((c - 122.1)/D_IN_Y);
   e = floor(D_IN_Y * d);
   g = (int) ((c - e)/D_IN_M);
   f_day = c - e + fr_part - floor(D_IN_M * g) ;
   month = g <= 13 ? g - 1 : g - 13 ;
   year = (int) (month > 2 ? d - 4716 : d - 4715) ;
   day = (int)(floor(f_day));
   f_hour = ( f_day - day ) * 24.0 ;
   hour = (int)(floor(f_hour));
   f_minute = ( f_hour - hour ) * 60.0 ;
   minute = (int)(floor(f_minute));
   f_second = ( f_minute - minute ) * 60.0 ;
   dd -> da_year = year ;
   dd -> da_mon  = month ;
   dd -> da_day  = day ;
   tt -> ti_hour = hour ;
   tt -> ti_min  = minute ;
   tt -> ti_sec  = (int) f_second ;
   tt -> ti_hund = (int) ((f_second - tt -> ti_sec) * 100.0  ) ;
}

/*   ------------------------------------------------
     Function to get a string the GMT of a julian day
     ------------------------------------------------
*/

char *date_time_string(double j_day,char *any_str)
{
   struct date dd ;
   struct time tt ;

   get_date_time(j_day, &dd, &tt);
   sprintf(any_str,"%02d/%02d/%04d  %02d:%02d:%02d.%02d",dd.da_day,dd.da_mon,
           dd.da_year, tt.ti_hour,tt.ti_min,tt.ti_sec, tt.ti_hund);
   return(any_str);
}

/*   --------------------------------------------------
     Function to calculate the Greenwitch Sidereal Time
     --------------------------------------------------
*/
double gr_sidereal_time(double j_day)
{
   double j_time,j_cent,time_corr,gst,uni_time ;

   j_time = floor(j_day + 0.5) - 0.5  ;
   uni_time = 24.0 * (j_day - j_time) ;
   j_cent = (j_time - J_DAY_2000_1ST_NOO) / D_IN_C ;
   time_corr = norm_hour(quadratic
                         (6.697374558, 2400.051336, 2.5862e-05,j_cent));
   gst = norm_hour((366.25/365.25) * uni_time + time_corr) ;
   return (gst);
}

/*   --------------------------------------------------
     Function to calculate the local sidereal time from
                 Julian day and longitude
                 ------------------------
*/
double local_sidereal_time(double j_day, double longitude)
{
   double gr_sid_time,local_sid_time ;

   gr_sid_time = gr_sidereal_time(j_day);
   local_sid_time = norm_hour(convert_time(gr_sid_time,0.0,longitude));
   return(local_sid_time);
}

/*   ------------------------------------------------------------
     Function to calculate the planetary positions and directions
     ------------------------------------------------------------
*/
void full_details(double j_day,ST_PL_DET pl_details[])
{
   ST_PL_DET dummy_details[N_PLANETS];
   int pl_no;

   calculate_planets(j_day + 0.005,dummy_details);
   calculate_planets(j_day,pl_details);

   for ( pl_no = MERCURY ; pl_no <= PLUTO ; ++pl_no)
   {
      if (dummy_details[pl_no].geo_long > pl_details[pl_no].geo_long)
         pl_details[pl_no].direction = ' ' ;
      else
         pl_details[pl_no].direction = 'R' ;
   }
}

/*        -----------------------------------------
          Function to calculate planetary positions
          -----------------------------------------
*/
void calculate_planets(double j_day,ST_PL_DET pl_details[])
{
   int pl_no,temp_pl_no ;
   double l_sunt, l_moont, l_earth, au_earth,mn_age,theta, j_cent ;
   double upn ;

   j_cent = (j_day - J_DAY_1900_PRE_NOO) / D_IN_C ;
   find_parameters(j_cent,&l_sunt,&l_earth,&au_earth,&l_moont,&theta);
   mn_age = norm_angle (180.0 - l_sunt + l_moont);
   upn = d_sin(theta) ;
   upn = EARTH_EQ_RAD / upn / ASTRO_UNIT ;
   pl_details[MOON].geo_dist = upn ;
   pl_details[MOON].geo_dist = EARTH_EQ_RAD / d_sin(theta) / ASTRO_UNIT ;
   pl_details[MOON].hel_dist = au_earth + pl_details[MOON].geo_dist * d_cos(mn_age);
   pl_details[SUN].geo_dist = au_earth ;

   for (pl_no = MERCURY ; pl_no <= PLUTO ; pl_no++)
   {
      temp_pl_no = pl_no - 2 ;
      calculate_a_planet(pl_no,j_day,l_sunt,l_earth,au_earth,
                         au[temp_pl_no],&(pl_details[pl_no]),lc[temp_pl_no],
                         ec[temp_pl_no],ic[temp_pl_no],oc[temp_pl_no],mc[temp_pl_no]);
   }

   pl_details[ASC_NODE].geo_long = pl_details[MOON].asc_node_long =
      norm_angle(quadratic(259.183275,-1934.142,0.002078,j_cent));
   pl_details[DESC_NODE].geo_long =
      norm_angle(pl_details[ASC_NODE].geo_long + HALF_CIRCLE );
   pl_details[MOON].orb_ecc = ECC_MOON ;
   pl_details[MOON].orb_incl = INCL_MOON ;
   pl_details[SUN].geo_long = l_sunt ;
   pl_details[MOON].geo_long = l_moont ;
   pl_details[SUN].direction = ' ' ;
   pl_details[MOON].direction = ' ' ;
   pl_details[ASC_NODE].direction = 'R' ;
   pl_details[DESC_NODE].direction = 'R' ;
   pl_details[SUN].zod_sign = (int) (l_sunt / 30);
   pl_details[MOON].zod_sign = (int) (l_moont / 30);
   pl_details[ASC_NODE].zod_sign
      = (int) (pl_details[ASC_NODE].geo_long/30);
   pl_details[DESC_NODE].zod_sign
      = (int) (pl_details[DESC_NODE].geo_long/30);
}

/*  -----------------------------------------------------------
    Function to find some parameters for a given Julian Century
    -----------------------------------------------------------
*/
void find_parameters(double j_cent,double *l_sunt, double *l_earth,
                     double *au_earth, double *l_moont, double *theta)
{
   double l_sun,m_sun,c_sun,nu_sun ;
   double l_moon,m_moon,d_moon ;
   double ec_earth,efmn,ffmn ;

   l_sun = norm_angle(quadratic(279.69668,36000.76892,3.025e-04,j_cent));
   m_sun = norm_angle(cubic(358.47583,35999.04975,-1.5e-04,-3.3e-06,j_cent));
   ec_earth = quadratic(1.675104e-02,-4.18e-05,-1.26e-07,j_cent);
   c_sun = quadratic(1.91946,-0.004789,-1.4e-05,j_cent) * d_sin(m_sun)
      + linear(0.020094,-1e-04,j_cent) * d_sin(2.0 * m_sun)
      + 2.93e-04 * d_sin(3.0 * m_sun) ;
   *l_sunt = norm_angle(l_sun + c_sun) ;
   nu_sun = m_sun + c_sun ;
   *au_earth = 1.0000002 * (1.0 - ec_earth * ec_earth)/
      (1.0 + ec_earth * d_cos(nu_sun)) ;
   *l_earth = *l_sunt > 180 ? *l_sunt - 180 : *l_sunt + 180 ;

   l_moon = norm_angle(linear(270.434164,481267.8831,j_cent));
   m_moon = norm_angle(linear(296.104608,477198.8491,j_cent));
   d_moon = norm_angle(linear(350.737486,445267.1142,j_cent));
   *l_moont = l_moon + 6.28875 * d_sin(m_moon)
      + 1.274018 * d_sin(2.0 * d_moon - m_moon)
      + 0.685309 * d_sin(2.0 * d_moon)
      + 0.213616 * d_sin(2.0 * m_moon) ;
   efmn = quadratic(1.0,-0.002495,-7.52e-06,j_cent);
   ffmn = linear(11.250889,483202.0251,j_cent);
   *l_moont = *l_moont - 0.114336 * d_sin(2.0 * ffmn)
      + 0.058793 * d_sin(2.0 * (d_moon - m_moon))
      + 0.05332 * d_sin(2.0 * d_moon + m_moon)
      - 0.034718 * d_sin(d_moon)
      + efmn * ( 0.057212 * d_sin(2.0 * d_moon - nu_sun - m_moon)
                 - 0.185596 * d_sin(nu_sun)
                 + 0.045874 * d_sin(2.0 * d_moon - nu_sun)
                 + 0.041024 * d_sin(m_moon - nu_sun)
                 - 0.030465 * d_sin(m_moon + nu_sun)
         );
   *l_moont = norm_angle (*l_moont);
   *theta =  0.950724 + 0.051818 * d_cos(m_moon)
      + 0.009531 * d_cos(2.0 * d_moon - m_moon)
      + 0.007843 * d_cos (2.0 * d_moon)
      + 0.002824 * d_cos (2.0 * m_moon) ;
}

/*    ----------------------------------------------
      Function to calculate the position of a planet
      ----------------------------------------------
*/
void calculate_a_planet (int pl_no,double j_day,double l_sunt,
                         double l_earth,double au_earth, double au,PL_DET p,
                         double lc[],double ec[],double ic[],double oc[],double mc[])
{
   int i;
   double mult1,mult2,temp_val,c1,c2;
   double nup,r_pp,r_qp,r_vp,r_zp,j_per_l,j_per_m,s_per_l,s_per_m;
   double longitude,magnitude,ellipt_ecc,txx,nu,u,t_sin,t_cos,bbi,r_bb;
   double j_cent,nn,dd,dir_at_sun;

   j_cent = (j_day - J_DAY_1900_PRE_NOO) / D_IN_C;
   p->orb_ecc = poly_val1(3,ec,j_cent);
   nup = 0.2 * j_cent + 0.1 ;
   r_pp = linear(237.47555,3034.9061,j_cent)/D_IN_R ;
   r_qp = linear(265.9165 ,1222.1139,j_cent)/D_IN_R ;
   r_vp = 5 * r_qp - 2 * r_pp ;
   r_zp = r_qp - r_pp ;
   j_per_l = 0.331 * sin(r_vp) - 0.064 * nup * cos(r_vp)
      - 0.034 * cos(r_zp) * sin(r_qp)
      - 0.036 * sin(r_zp) * cos(r_qp);
   j_per_m = j_per_l - ( - 0.02 * cos(r_vp)
                         + 0.034 * cos(r_zp) * sin(r_qp)
                         + 0.037  * sin(r_zp) * cos(r_qp)
      ) / p->orb_ecc ;
   s_per_l = -0.814 * sin(r_vp)
      + 0.161 * nup * cos(r_vp)
      - 0.149 * sin(r_zp)
      - 0.041 * sin(2 * r_zp)
      + 0.081 * cos(r_zp) * sin(r_qp)
      + 0.086 * sin(r_zp) * cos(r_qp) ;
   s_per_m = s_per_l - ( 7.700001e-02 * sin(r_vp)
                         + 4.600001e-02 * cos(r_vp)
                         - 7.600001e-02 * sin(r_zp) * sin(r_qp)
                         - 0.073 * cos(r_qp)
                         - 0.15 * cos(r_zp) * cos(r_qp)
      ) / p->orb_ecc ;
   mult1 = (pl_no == JUPITER) ? 1.0 : 0.0 ;
   mult2 = (pl_no == SATURN ) ? 1.0 : 0.0 ;
   longitude = poly_val1(3,lc,j_cent)
      + mult1 * j_per_l
      + mult2 * s_per_l ;
   if (pl_no == PLUTO)
      longitude += FULL_CIRCLE * ( j_day - J_DAY_1960_LIB_MID )
         / (247.686 * D_IN_Y );
   longitude = norm_angle(longitude);
   p->orb_incl = poly_val1(3,ic,j_cent);
   p->asc_node_long = poly_val1(3,oc,j_cent);
   magnitude = poly_val1(3,mc,j_cent)
      + mult1 * j_per_m + mult2 * s_per_m ;
   magnitude = norm_angle(magnitude);
   if (pl_no == URANUS)
      magnitude = longitude - p->asc_node_long
         - 98.071581 - 0.985765 * j_cent;
   else if (pl_no == NEPTUNE)
      magnitude = longitude - p->asc_node_long
         - 276.045975 - 0.3256394 * j_cent;
   else if (pl_no == PLUTO)
      magnitude = longitude - 224.16024 ;
   magnitude = norm_angle(magnitude);
   ellipt_ecc = 0.0 ;
   for ( i = 0 ; i < 7 ; i++)
      ellipt_ecc = norm_angle(magnitude + p->orb_ecc * d_sin(ellipt_ecc)
                              * D_IN_R) ;

   temp_val = ( (1 + p->orb_ecc) / (1 - p->orb_ecc) )  ;
   txx = sqrt(temp_val) * d_tan(0.5 * ellipt_ecc);
   nu = norm_angle( 2 * atan(txx) * D_IN_R );
   p->hel_dist = au * ( 1 - p->orb_ecc * d_cos(ellipt_ecc));
   u = longitude + nu - magnitude - p->asc_node_long ;
   t_sin = d_cos(p->orb_incl) * d_sin(u);
   t_cos = d_cos(u);
   p->hel_long = norm_angle(sc_d_angle(t_sin,t_cos) + p->asc_node_long);
   bbi = d_sin(u) * d_sin(p->orb_incl);
   temp_val = 1.0 - bbi*bbi ;
   r_bb = atan(bbi/temp_val) ;
   dir_at_sun = norm_angle(p->hel_long - l_sunt);
   c1 = p->hel_dist * cos(r_bb) ;
   nn =  c1 * d_sin(dir_at_sun) ;
   c2 = p->hel_dist * cos(r_bb) ;
   dd = c2 * d_cos(dir_at_sun) + au_earth ;
   p->geo_long = norm_angle( sc_d_angle(nn,dd) + l_sunt);
   p->zod_sign = (int) (p->geo_long / 30);
   temp_val = au_earth * au_earth + p->hel_dist * p->hel_dist
      - 2 * au_earth * p->hel_dist * cos(r_bb)
      * d_cos(p->hel_long - l_earth) ;
   p->geo_dist = sqrt(temp_val) ;
   p->house_no = 0 ;
}

/*  ----------------------------------------------
    Function to calculate the house cusp positions
    ----------------------------------------------
*/
HP_POS find_hp_pos(ST_PL_DET pl_details [], double house_degree [],
                   ST_HP_POS hp_pos [])
{
   int i,h_no ;
   double asc_degree = house_degree[0] ;
   HP_POS hp_pos_ptr = hp_pos, temp_hp_ptr ;
   PL_DET pl_det_ptr = pl_details ;
   int comp_degree(const void *a,const void *b);

   for ( i = 0 ; i < 12 ; i++)
   {
      hp_pos_ptr -> hp_no = (int)(house_degree[i] / 30);
      hp_pos_ptr -> hp_type = 'H' ;
      hp_pos_ptr -> degree = (house_degree[i] < asc_degree) ?
         (house_degree[i] + FULL_CIRCLE) : house_degree[i] ;
      hp_pos_ptr++;
   }
   for ( i = 12 ; i < 24 ; i++)
   {
      hp_pos_ptr -> hp_no = i - 12 ;
      hp_pos_ptr -> hp_type = 'P' ;
      hp_pos_ptr -> degree = pl_det_ptr -> geo_long ;
      if ( hp_pos_ptr -> degree < asc_degree)
         hp_pos_ptr -> degree += FULL_CIRCLE ;
      pl_det_ptr++;
      hp_pos_ptr++;
   }
   pl_details[ASCENDENT].geo_long = house_degree[0] ;
   pl_details[MIDHEAVEN].geo_long = house_degree[9] ;
   qsort((void *)hp_pos,24,sizeof(hp_pos[0]),comp_degree);
   h_no = 0 ;
   for ( i = 0,temp_hp_ptr = hp_pos ; i < 24 ; i++,temp_hp_ptr++)
   {
      switch (temp_hp_ptr -> hp_type)
      {
         case 'H' : h_no++ ;
            break ;
         case 'P' : pl_details[temp_hp_ptr->hp_no].house_no = h_no ;
            break ;
      }
      if ( i == 23)
         temp_hp_ptr -> next_hp = hp_pos ;
      else
         temp_hp_ptr -> next_hp = temp_hp_ptr + 1 ;
   }
   return (hp_pos);
}

/* --------------------------------------------------------------------
   Function to compare two angles and returning the result as -1,0 or 1
           ( Function used by the qsort() library function
                      used in find_hp_pos () )
                      ------------------------
*/
int comp_degree(const void *a,const void *b)
{
   HP_POS first_elem = (HP_POS) a;
   HP_POS second_elem = (HP_POS) b;
   if (first_elem->degree  < second_elem->degree )
      return (-1);
   else if (first_elem->degree > second_elem->degree)
      return (1);
   else return (0);
}

/*  -----------------------------------------------------
    Function to calculate the aspects between two sets of
               planetary details
               -----------------
*/
void get_aspects(ST_PL_DET pl_pos1[],ST_PL_DET pl_pos2[],
                 int same_or_different, double aspect[][N_PLANETS])
{
   int pl_no,oth_pl;

   for (pl_no = SUN ; pl_no <= MIDHEAVEN ; pl_no++)
      for (oth_pl = SUN ; oth_pl <= MIDHEAVEN ; oth_pl++)
         if ((pl_no == oth_pl)  &&  same_or_different == SAME)
            aspect[pl_no][oth_pl] = -90.0 ;
         else
            aspect[pl_no][oth_pl] = norm_angle(pl_pos2[oth_pl].geo_long
                                               - pl_pos1[pl_no].geo_long);
}

/*    ---------------------------------------------------------
      Function returning the aspect name from the aspect degree
      ---------------------------------------------------------
*/
char *asp_det(double aspect,char *asp_string)
{
   if (check_asp(aspect,0.0,7.0))
      strcpy(asp_string,"CJN");
   else if (check_asp(aspect,30.0,5.0))
      strcpy(asp_string,"SSX");
   else if (check_asp(aspect,45.0,2.0))
      strcpy(asp_string,"SSQ");
   else if (check_asp(aspect,60.0,5.0))
      strcpy(asp_string,"SXT");
   else if (check_asp(aspect,72.0,2.0))
      strcpy(asp_string,"QTL");
   else if (check_asp(aspect,90.0,7.0))
      strcpy(asp_string,"SQR");
   else if (check_asp(aspect,120.0,7.0))
      strcpy(asp_string,"TRI");
   else if (check_asp(aspect,135.0,2.0))
      strcpy(asp_string,"SES");
   else if (check_asp(aspect,150.0,2.0))
      strcpy(asp_string,"INC");
   else if (check_asp(aspect,180.0,7.0))
      strcpy(asp_string,"OPP");
   else strcpy(asp_string,"   ");
   return (asp_string);
}

/*    -----------------------------------------
      Function to check for a particular aspect
               ( Returns 1 or 0 )
                 --------------
*/
int check_asp(double aspect,double criteria,double per_diff)
{
   double other_criteria ;
   int ret_value ;

   other_criteria = FULL_CIRCLE - criteria ;
   if (fabs(aspect - criteria) < per_diff)
      ret_value = 1 ;
   else if (fabs(aspect - other_criteria) < per_diff)
      ret_value = 1 ;
   else ret_value = 0 ;
   return (ret_value);
}
/*   -------------------------------
     Function to print birth details
     -------------------------------
*/
void print_birth_details(FILE *fp,DT b_det)
{
   char ts[30];

   skip_page(fp);
   fprintf(fp,"\n\n           BIRTH DETAILS\n");
   fprintf(fp,    "           -------------\n\n");
   fprintf(fp,"Name                       =  %s\n",b_det -> name);
   fprintf(fp,"Date of Birth              =  %2d/%2d/%4d\n",
           b_det->day,b_det->month, b_det->year);
   if (b_det -> latitude > 0.0)
      fprintf(fp,"Latitude                   =  %10s N\n",
              time_string(b_det->latitude,DEGREE,FULL,ts));
   else
      fprintf(fp,"Latitude                   =  %10s S\n",
              time_string(-b_det->latitude,DEGREE,FULL,ts));
   if (b_det->longitude > 0.0)
      fprintf(fp,"Longitude                  =  %10s W\n",
              time_string(b_det->longitude,DEGREE,FULL,ts));
   else
      fprintf(fp,"Longitude                  =  %10s E\n",
              time_string(-b_det->longitude,DEGREE,FULL,ts));
   fprintf(fp,"Time of Birth              =  %2dh %2dm %04.2fs\n",
           b_det->hour,b_det->minute,b_det->second);
   if (b_det->st_long > 0.0)
      fprintf(fp,"Longitude of Standard Time = %10s W\n\n",
              time_string(b_det->st_long,DEGREE,FULL,ts));
   else
      fprintf(fp,"Longitude of Standard Time = %10s E\n\n",
              time_string(-b_det->st_long,DEGREE,FULL,ts));
   fprintf(fp,"Mean Local Time            = %10s\n",
           time_string(b_det->l_time,TIME,FULL,ts));
   fprintf(fp,"Greenwich time             = %10s\n",
           time_string(b_det->g_time,TIME,FULL,ts));
   fprintf(fp,"Greenwich Sidereal time    = %10s\n",
           time_string(b_det->g_sid_time,TIME,FULL,ts));
   fprintf(fp,"Local Sidereal time        = %10s\n",
           time_string(b_det->l_sid_time,TIME,FULL,ts));
   fprintf(fp,"Julian Day Number          = %15.4f\n",b_det->jul_date);
}
/*    ---------------------------------
      Functions to print planet details
      ---------------------------------
*/
void print_planet_heading(FILE *fp)
{
   under_line(fp,67);
   fprintf(fp,"%s%s\n","Planet        Long. D  Sign           dd:mm",
           "   *    House  **");
   under_line(fp,67);
}

void print_planet_details(FILE *fp,ST_PL_DET pl_details[])
{
   int pl_no, curr_house , curr_zod ;
   ST_DMS *gl,dummy ;
   double geo_long_dm ;

   print_planet_heading(fp);
   for (pl_no = SUN ; pl_no <= DESC_NODE ; pl_no++)
   {
      geo_long_dm = fmod(pl_details[pl_no].geo_long,30.0);
      gl = dms_time(geo_long_dm,&dummy);
      curr_zod = pl_details[pl_no].zod_sign ;
      curr_house = pl_details[pl_no].house_no ;

      fprintf(fp,"%-10s%9.3f%2c%2s%-13s%4d:%2d%2s%2s%6d%7s\n",
              pl_name[pl_no],pl_details[pl_no].geo_long,pl_details[pl_no].direction,
              SPACE2,zod_name[curr_zod],gl->degrees,gl->minutes,
              SPACE2,sign_dignity[pl_sign_dignity[pl_no][curr_zod]],
              curr_house,
              house_dignity[pl_sign_dignity[pl_no][curr_house-1]]);
   }
   under_line(fp,67);
   fprintf(fp, "  D: Direction, 'R' indicates that the planet is retrograding\n");
   fprintf(fp, "  *:  E=Exalt,       F=Fall,       R=Ruler,       D=Detriment\n");
   fprintf(fp, " **: AE=Acc. Exalt, AF=Acc. Fall, AR=Acc. Ruler, AD=Acc. Detriment\n");
   under_line(fp,67);
}
/*          --------------------------
            Functions to print aspects
            --------------------------
*/

void print_aspects(FILE *fp,double aspect[][N_PLANETS], int start_planet, int end_planet)
{
   char asp_string[10] ;
   int pl_no,oth_pl ;
   int line_width = (end_planet - start_planet + 1) * 7 + 6;

   print_asp_heading(fp, start_planet, end_planet);

   for (pl_no = SUN ; pl_no <= MIDHEAVEN ; pl_no++)
   {
      fprintf(fp,"%c%c%c  ",pl_name[pl_no][0],pl_name[pl_no][1],
              pl_name[pl_no][2]) ;
      for (oth_pl = start_planet ; oth_pl <= end_planet ; oth_pl++)
      {
         if (aspect[pl_no][oth_pl] < 0.0)
            fprintf(fp,"%7s","XXX");
         else
         {
            strcpy(asp_string,time_string(aspect[pl_no][oth_pl],DEGREE,PART,
                                          asp_string));
            fprintf(fp,"%7s",asp_string);
         }
      }
      fprintf(fp,"\n%5s"," ");
      for (oth_pl = start_planet ; oth_pl <= end_planet ; oth_pl++)
      {
         if (aspect[pl_no][oth_pl] < 0.0)
            fprintf(fp,"%7s","XXX");
         else
         {
            strcpy(asp_string,asp_det(aspect[pl_no][oth_pl],asp_string));
            fprintf(fp,"%7s",asp_string);
         }
      }
      fprintf(fp,"\n\n");
   }
   under_line(fp,line_width);
}

void print_self_aspects (FILE *fp, double aspect[][N_PLANETS])
{
   skip_page(fp);
   print_self_asp_heading (fp);
   print_aspects(fp, aspect, SUN, PLUTO) ;
   skip_page(fp);
   fputc('\n',fp);
   print_aspects(fp, aspect, ASC_NODE, MIDHEAVEN) ;
}

void print_mutual_aspects (FILE *fp, double aspect[][N_PLANETS], 
                           char *name1, char *name2)
{
   skip_page(fp);
   print_mutual_asp_heading (fp, name1, name2);
   print_aspects(fp, aspect, SUN, PLUTO) ;
   skip_page(fp);
   fputc('\n',fp);
   print_aspects(fp, aspect, ASC_NODE, MIDHEAVEN) ;
}

void print_self_asp_heading (FILE *fp)
{
   fprintf(fp,"\n                                       SELF-ASPECTS\n");
   fprintf(fp,"                                       ------------\n\n");
}

void print_mutual_asp_heading (FILE *fp, char *name1, char *name2)
{
   fprintf(fp,"\n                                       MUTUAL-ASPECTS\n");
   fprintf(fp,"                                       --------------\n\n");
   fprintf(fp,"  First  Person        : %-s\n",name1);
   fprintf(fp,"  Second Person        : %-s\n\n\n",name2);
}

void print_asp_heading(FILE *fp, int start_planet, int end_planet)
{
   char short_pl[10];
   int i;
   int line_width = (end_planet - start_planet + 1) * 7 + 6;
   under_line(fp, line_width);
   fprintf(fp, "%5s", " ");
   for ( i = start_planet ; i <= end_planet ; i++)
   {
      strncpy(short_pl,pl_name[i],3);
      short_pl[3] = '\0';
      fprintf(fp,"%7s",short_pl);
   }
   fprintf(fp,"\n");
   under_line(fp, line_width);
   fprintf(fp,"\n");
}

/*        ---------------------------------
          Function to print House positions
          ---------------------------------
*/

void print_hp_pos (FILE *fp,HP_POS hp_pos)
{
   HP_POS temp_hp_pos = hp_pos ;
   int i = 0,h_no = 0;

   skip_page(fp);
   print_hp_heading(fp);
   while (i++ < 24)
   {
      if (temp_hp_pos -> hp_type == 'H')
         h_no++;
      switch (temp_hp_pos -> hp_type)
      {
         case 'H'   : print_house(fp,temp_hp_pos,h_no) ; break ;
         case 'P'   : print_planet(fp,temp_hp_pos); break ;
      }
      temp_hp_pos = temp_hp_pos -> next_hp;
   }
}

void print_hp_heading(FILE *fp)
{
   fprintf(fp,"\n                    DETAILS OF HOUSES\n");
   fprintf(fp,  "                    -----------------\n\n");
}

void print_house(FILE *fp,HP_POS hp_pos,int h_no)
{
   HP_POS next_hp_pos;
   double first_degree, last_degree ;
   char first_string[30],last_string[30],dummy[30];
   int zod_no,first_zod,last_zod ;

   for (next_hp_pos = hp_pos -> next_hp ;  next_hp_pos -> hp_type == 'P' ;
        next_hp_pos = next_hp_pos -> next_hp)
      ;                      /*       NULL loop       */
   first_degree = norm_angle(hp_pos -> degree) ;
   last_degree = norm_angle(next_hp_pos -> degree) ;
   first_zod = (int) (first_degree / 30);
   last_zod = (int) (last_degree / 30 );
   reduced_string(first_degree,first_string,1);
   reduced_string(last_degree,last_string,1);
   fprintf(fp,"\n\n  HOUSE No. %2d :%20s to %20s\n",h_no,first_string,
           last_string);
   for (zod_no = first_zod + 1 ; zod_no <= last_zod ; zod_no++)
      fprintf(fp,"  ( %12s starts at %s )\n",zod_name[zod_no],
              time_string( 30.0 * zod_no ,DEGREE,PART,dummy));
}

/*   ------------------------------------------------------------
     Function to convert an angle to the respective zodiac degree
              e.g.  157.5   ->   VIR 7:30
              ---------------------------
*/

char *reduced_string(double any_degree, char *any_string, int english)
{
   char any1_string[10], any2_string[10],z_string[30];
   int any_zod ;
   double any_zod_degree ;

   any_degree = norm_angle(any_degree);
   any_zod = (int)(any_degree/30);
   any_zod_degree = any_degree - 30.0 * any_zod ;
   time_string(any_degree,DEGREE,PART,any1_string);
   time_string(any_zod_degree,DEGREE,PART,any2_string);
   if (english)
      strcpy(z_string,zod_name[any_zod]);
   else
      strcpy(z_string,hind_zod_name[any_zod]);
   sprintf(any_string,"%7s  ( %c%c%c  %7s )", any1_string, z_string[0],
           z_string[1],z_string[2],any2_string);
   return(any_string);
}

/*   ------------------------------------------------------
     Function to print the planet name and its house degree
     ------------------------------------------------------
*/

void print_planet(FILE *fp,HP_POS hp_pos)
{
   char dummy[30];
   fprintf(fp,"%2s%-15s%25s\n",SPACE2,
           pl_name[hp_pos->hp_no],reduced_string(norm_angle(hp_pos->degree),dummy,1));
}

/*   ------------------------------------------------------------
     Functions to calculate various values in the Hindu Astrology
     ------------------------------------------------------------
*/

double  hind_long(double sayan_long)
{
   return(norm_angle(sayan_long - ayanamsa)) ;
}

double navamsa(double hind_long)
{
   return(0.3 * fmod(hind_long,40.0)) ;
}

double nakshatra(double hind_long)
{
   return( 0.075 * hind_long ) ;
}

/*  -------------------------------------------------------
    Functions to print details according to Hindu Astrology
    -------------------------------------------------------
*/

void print_hind_heading(FILE *fp)
{
   under_line(fp,69);
   fprintf(fp,"%-10s%10s%-15s%13s%7s%-15s\n","Planet","   Long. ",
           "    Sign       ","       dd:mm "," House ","D  Navamsa");
   under_line(fp,69);
}


void hindu_details(FILE *fp,ABD birth_info,ST_PL_DET pl_details[],
                   double hs[])
{
   int ds_no,nak,paksham,thithi, house;
   ST_DASA ds_det[9];
   double l_moont,sun_moon_diff,j_day,d_thithi,d_nak ;
   char rs[30];
   double lagna = hs[0];

   int pl_no,hind_sign,nav_sign,sign_no ;
   int next_no[12],next_nav[12],sign_pl[12][10],nav_pl[12][10] ;
   ST_DMS *hind_dm,dummy1 ;
   double h_long,nav ;
   char ay_str[30];

   skip_page(fp);
   fprintf(fp,"\n                      HINDU (NIRAYANA) SYSTEM\n");
   fprintf(fp,  "                      -----------------------\n\n");
   fprintf(fp,"  Ayanamsa = %s\n",time_string(ayanamsa,DEGREE,FULL,ay_str));
   print_hind_heading(fp);
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

   for (pl_no = SUN ; pl_no <= DESC_NODE ; pl_no++)
   {
      h_long = hind_long(pl_details[pl_no].geo_long);
      hind_sign = (int) (h_long / 30.0);
      hind_dm = dms_time(h_long - 30.0 * hind_sign,&dummy1);
      nav = navamsa(h_long);
      nav_sign = (int)nav ;
      fprintf(fp,"%-10s%10.3f%2s%-15s%7d:%2d%5d%4c%2s%-15s\n",
              hind_pl_name[pl_no],h_long, SPACE2,hind_zod_name[hind_sign],
              hind_dm->degrees,hind_dm->minutes,
              pl_details[pl_no].house_no,pl_details[pl_no].direction,
              SPACE2,hind_zod_name[nav_sign]);
      sign_pl[hind_sign][next_no[hind_sign]] = pl_no ;
      (next_no[hind_sign])++;
      nav_pl[nav_sign][next_nav[nav_sign]] = pl_no ;
      (next_nav[nav_sign])++;
   }
   under_line(fp,69);
   fprintf(fp, "  D: Direction, 'R' indicates that the planet is retrograding (vakra)\n");
   under_line(fp,69);
   fprintf(fp,"\n BHAVAS\n ------\n");
   for ( house = 0 ; house < 12 ; house++)
   {
      fprintf(fp,"    BHAVA %2d = %s\n",
              house+1,reduced_string(hind_long(hs[house]),rs,0));
   }
   l_moont = hind_long(pl_details[MOON].geo_long);
   j_day = birth_info->j_date  ;
   d_nak = get_dasas(l_moont,j_day,ds_det);
   nak = (int) d_nak ;
   d_nak = (d_nak - nak) * 100.0 ;
   sun_moon_diff = norm_angle(pl_details[MOON].geo_long -
                              pl_details[SUN].geo_long);
   paksham = (int) (sun_moon_diff / 180.0);
   d_thithi = (sun_moon_diff - 180.0 * paksham) / 12.0 + 1.0 ;
   if (d_thithi > 15.0 )
      d_thithi -= 15.0 ;
   thithi = (int) d_thithi ;
   d_thithi = (d_thithi - thithi) * 100.0 ;
   fprintf(fp,"\n     Nakshathram = %s (%6.2f %%)\n",nak_name[nak],d_nak);
   fprintf(fp,"\n     Varam       = %s\n",day_name[weekday(j_day, 0.0)]);
   fprintf(fp,"\n     Paksham     = %s\n",paksha_name[paksham]);
   fprintf(fp,"\n     Thithi      = %s (%6.2f %%)\n\n",thithi_name[thithi],
           d_thithi);
   skip_page(fp);
   draw_charts(fp,sign_pl,nav_pl);
   skip_page(fp);
   fprintf(fp,"\n                           DASAS\n");
   fprintf(fp,  "                           -----\n\n");
   fprintf(fp,"   Note : All times are in Greenwitch Mean Time\n\n");
   for (ds_no = 0 ; ds_no < 9 ; ds_no++)
   {
      print_antar_dasa(fp,&(ds_det[ds_no]));
   }
}

double get_dasas(double l_moont,double birth_j_day,ST_DASA ds_det[])
{
   double fns,sjd,ejd ;
   int ns,ds,lrd,ds_no ;

   if (debugging) fprintf(stderr, "DEBUG: jd = %f\n", birth_j_day);

   fns = nakshatra(l_moont);
   ns = (int) fns ;
   ds = ns % 9 ;
   lrd = ds_det[0].lord = ds ;
   ds_det[0].start_j_day = birth_j_day ;
   ds_det[0].end_j_day = birth_j_day + ( ns + 1.0 - fns) *
      dasa_yr[ds] * D_IN_Y ;
//    ds_det[0].end_j_day = birth_j_day + ( fns - ns ) *
//       dasa_yr[ds] * D_IN_Y ;
   for (ds_no = 1 ; ds_no < 9 ; ds_no++)
   {
      if (lrd == 8)
         lrd = 0 ;
      else 
         lrd++;
      sjd = ds_det[ds_no - 1].end_j_day ;
      ejd = sjd + dasa_yr[lrd] * D_IN_Y ;
      ds_det[ds_no].lord = lrd ;
      ds_det[ds_no].start_j_day = sjd ;
      ds_det[ds_no].end_j_day = ejd ;
      if (debugging) fprintf(stderr, "DEBUG: ds_no = %d, lrd = %d, sjd = %f, ejd = %f\n", ds_no, lrd, sjd, ejd);
   }
   return(fns);
}

void print_antar_dasa(FILE *fp,DASA part_dasa)
{
   double sjd,ejd,a_sjd,a_ejd,duration,bjd ;
   int lrd ;
   char start_day[30],end_day[30];

   lrd = part_dasa -> lord ;
   sjd = part_dasa -> start_j_day ;
   ejd = part_dasa -> end_j_day ;
   bjd = ejd - dasa_yr[lrd] * D_IN_Y ;
   duration =  ejd - bjd ;

   date_time_string(sjd,start_day);
   date_time_string(ejd,end_day);
   fprintf(fp,"\nDasa of %3s FROM %25s TO %25s\n\n",h_dasa_name[lrd],
           start_day,end_day);
   fprintf(fp,"   Bhuktis are as follows :-\n\n");
   if (fabs(sjd - bjd) > 0.00001 )
   {
      while ( bjd < sjd )
      {
         bjd += duration * dasa_yr[lrd] / 120.0 ;
         lrd = (lrd == 8 ? 0 : lrd + 1);
      }
      a_sjd = sjd ;
      a_ejd = bjd ;
      date_time_string(a_sjd,start_day);
      date_time_string(a_ejd,end_day);
      lrd = (lrd == 0 ? 8 : lrd -1 );
      fprintf(fp,"     %s   FROM %s TO %s\n",h_dasa_name[lrd],start_day,
              end_day);
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
      date_time_string(a_sjd,start_day);
      date_time_string(a_ejd,end_day);
      fprintf(fp,"     %s   FROM %s TO %s\n",h_dasa_name[lrd],start_day,
              end_day);
      lrd++;
      if (lrd > 8)
         lrd = 0 ;
   }
   while (lrd != part_dasa -> lord);
   under_line(fp,74);
}

int weekday(double j_day, double longitude)
{
   int i_day ;
   double day ;

   day = fmod((j_day+1.0-longitude/360.0),7.0);
   i_day = (int)(day + 0.5);
   if ( i_day > 6)
      i_day = 0 ;
   return(i_day);
}

double find_e_and_ayanamsa(double j_day)
{
   double jc,e ;

   jc = (j_day - J_DAY_2000_1ST_NOO)/D_IN_C ;

   e = get_e(j_day);
   ayanamsa = get_ayanamsa(j_day);
   return (e) ;
}

double get_e(double j_day)
{
   double e;
   double jc = (j_day - J_DAY_2000_1ST_NOO)/D_IN_C ;

   e = norm_angle(cubic(84381.45,-46.815,-0.0006,0.00181,jc)/3600.0);
   return e;
}

double get_ayanamsa(double j_day)
{
   double a;
   double jc = (j_day - J_DAY_2000_1ST_NOO)/D_IN_C ;
   a = (quadratic(85901.4490, 50.2786,2.24e-04,jc*100)/3600.0);
   return a;
}


#if 0
double julian_day_of_previous_samkranthi(double j_day)
{
   double j_cent, l_sunt, l_earth, au_earth, l_moont, theta;
   double day_approx;
   double first, last, mid;
   int i;
   j_cent = (j_day - J_DAY_1900_PRE_NOO) / D_IN_C ;
   find_parameters(j_cent, &l_sunt, &l_earth,
                    &au_earth, &l_moont, &theta);
   day_approx = j_day - fmod(l_sunt * 365.25 / 360.0, 30.0);
   first = day_approx - 3.0;
   last = day_approx + 3.0;

   /* Do a binary search */
   for (i=0; i<10; ++i)    /* Don't go into an infinite loop! */
   {
      double jc, ls;
      mid = 0.5 * (first + last);
      jc = (mid - J_DAY_1900_PRE_NOO) / D_IN_C ;
      find_parameters(jc, &ls, &l_earth,
                      &au_earth, &l_moont, &theta);
      if (fabs(ls) < 0.0000001) 
         break;
      else if (ls < 15.0)
         last = mid;
      else 
         first = mid;
   }
   return mid;
}

int get_ME_day(double j_day, struct date *day)
{
   double j_cent, l_sunt, l_earth, au_earth, l_moont, theta;
   int days_in_ME;
   double prev_samkranthi = julian_day_of_previous_samkranthi(j_day);
   j_cent = (j_day - J_DAY_1900_PRE_NOO) / D_IN_C ;
   find_parameters(j_cent, &l_sunt, &l_earth,
                    &au_earth, &l_moont, &theta);
   days_in_ME = j_day - J_DAY_ME_START;
   day->da_year = days_in_ME / D_IN_Y + 1;
   day->da_month = 
}

#endif
