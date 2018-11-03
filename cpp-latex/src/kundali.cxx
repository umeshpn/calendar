#include "kundali.h"

extern const char* lhind_pl_name[];
extern const char* rasi_name[];

Kundali::Kundali(ST_PL_DET planetDetails[], double ayan)
{
   for (int i=SUN ; i < MIDHEAVEN; ++i) {
      grahams[i].sphutam = norm_angle(planetDetails[i].geo_long - ayan);
      grahams[i].rasi = (int)(grahams[i].sphutam / 30);
      grahams[i].name = lhind_pl_name[i];
   }
   for (int i=SUN ; i < MIDHEAVEN; ++i) {
      grahams[i].house = grahams[i].rasi - grahams[ASCENDENT].rasi;
      if (grahams[i].house < 0) {
         grahams[i].house += 12;
      }
   }
   int seen[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
   this->rasiCount = 0;
   for (int i=SUN; i<= SATURN; ++i) {
      int r = grahams[i].rasi;
      if (seen[r] == 0) {
         ++rasiCount;
         seen[r] = 1;
      }
   }
}

int Kundali::getHouseNumber(int pl) const
{
   // assert (pl >= SUN && pl <= MIDHEAVEN);
   return grahams[pl].house + 1;
}

int Kundali::getRasiNumber(int pl) const
{
   // assert (pl >= SUN && pl <= MIDHEAVEN);
   return grahams[pl].rasi + 1;
}

string Kundali::getRasiName(int pl) const
{
    // assert (pl >= SUN && pl <= MIDHEAVEN);
    return rasi_name[grahams[pl].rasi];
}

string Kundali::getGrahaName(int pl) const
{
    // assert (pl >= SUN && pl <= MIDHEAVEN);
    return grahams[pl].name;
}

int Kundali::getHouseDiff(int pl1, int pl2) const
{
    // assert (pl1 >= SUN && pl1 <= MIDHEAVEN);
    // assert (pl2 >= SUN && pl2 <= MIDHEAVEN);

    int r1 = grahams[pl1].rasi;
    int r2 = grahams[pl2].rasi;

    return ((r1 < r2) ? (r1 + 12 - r2) : (r1 - r2));
}

bool Kundali::isGrahamAtUcham(int pl) const
{
   if (pl < SUN && pl > SATURN) {
      return false;
   }
   return (pl_sign_dignity[pl][grahams[pl].rasi] == 1);
}
bool Kundali::isGrahamAtNeecham(int pl) const
{
   if (pl < SUN && pl > SATURN) {
      return false;
   }
   return (pl_sign_dignity[pl][grahams[pl].rasi] == 2);
}
bool Kundali::isGrahamAtSwasthanam(int pl) const
{
   if (pl < SUN && pl > SATURN) {
      return false;
   }
   return (pl_sign_dignity[pl][grahams[pl].rasi] == 3);
}

bool Kundali::isGrahamAtKendramOfAnother(int pl, int another) const
{
   return (this->getHouseDiff(pl, another) % 3 == 0);
}

bool Kundali::doesGrahamCauseMahayogam(int pl) const
{
   return (this->isGrahamAtKendramOfAnother(pl, ASCENDENT) &&
           (this->isGrahamAtUcham(pl) || this->isGrahamAtSwasthanam(pl)));
}
