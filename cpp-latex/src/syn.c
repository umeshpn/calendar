                     /*   SYN.C   */
                     
#include "ast.h"
#if !MAKE_FILE
#  include "ast_data.c"
#  include "gen_lib.c"
#  include "ast_lib.c"
#  include "hs.c"
#  include "draw_ch.c"
#  include "title.c"
#endif

int main(int argc, char *argv[])
{
  char filename[30];
  FILE *in_fp, *out_fp ;
  ST_DT det1, det2 ;
  ST_ABD birth_info1, birth_info2 ;
  ST_PL_DET pl_det1[15], pl_det2[15] ;
  ST_HP_POS hp_pos1[30], hp_pos2[30];
  int no,rec_no1, rec_no2, okay ;
  double aspects[14][14],hs1[12], hs2[12],e1, e2;
  int read_details (FILE *, int, DT) ;

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
/*
           FIRST PERSON'S DETAILS
*/
  printf("Enter the No. of First Person : ");
  scanf("%d",&rec_no1);
  getchar();
  printf("\n\n");
  okay = read_details (in_fp, rec_no1, &det1);
  if (okay)
  {
    find_birth_info(&det1,&birth_info1);
    full_details(birth_info1.j_date,pl_det1);
    e1 = find_e_and_ayanamsa(birth_info1.j_date);
    get_house_degrees(e1,&birth_info1,hs1);
    find_hp_pos(pl_det1,hs1,hp_pos1);
  }
  else
    printf("Record No. %d not found !\n", rec_no1) ;
  if (okay)
  {
/*
           SECOND PERSON'S DETAILS
*/
    printf("Enter the No. of Second Person : ");
    scanf("%d",&rec_no2);
    getchar();
    printf("\n\n");
    okay = read_details (in_fp, rec_no2, &det2);
    if (okay)
    {
      find_birth_info(&det2,&birth_info2);
      full_details(birth_info2.j_date,pl_det2);
      e2 = find_e_and_ayanamsa(birth_info2.j_date);
      get_house_degrees(e2,&birth_info2,hs2);
      find_hp_pos(pl_det2,hs2,hp_pos2);
    }
    else
      printf("Record No. %d not found !\n", rec_no2) ;
  }
  
  fputc(15,out_fp);
  get_aspects(pl_det1,pl_det2,DIFFERENT,aspects);
  print_mutual_aspects(out_fp,aspects, det1.name, det2.name);
  fprintf(out_fp,"\n\n");
  fputc(18,out_fp);
  fclose(in_fp); fclose(out_fp);
  return 0;
}

int read_details (FILE *fp, int rec_no, DT detp)
{
  int no, okay = 0 ;
  int yy,mm,dd,hh,mn,latd,latm,longd,longm,st_longd,st_longm ;
  char name[30] ;
  rewind(fp);
  while (fscanf(fp,"%d,%d/%d/%d,%d:%d,%d:%d,%d:%d,%d:%d",&no,
      &dd,&mm,&yy,&hh,&mn,&latd,&latm,&longd,&longm,&st_longd,&st_longm)
      != EOF)
  {
    printf("%d\n",no);
    if (no == rec_no)
    {
      okay = 1 ;
      printf("Date : %2d/%2d/%4d\n",dd,mm,yy);
      printf("\n\n Name : ");
      gets(name);
      strcpy(detp->name,name) ;
      detp->year = yy ; detp->month = mm ; detp->day = dd ;
      detp->hour = hh ; detp->minute = mn ;
      detp->second = 0.0 ;
      detp->latitude = latd + latm / 60.0 ;
      detp->longitude = longd + longm / 60.0 ;
      detp->st_long = st_longd + st_longm / 60.0 ;
    }
  }
  return (okay);
}
