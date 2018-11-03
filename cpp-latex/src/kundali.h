#ifndef STA_KUNDALI_H
#define STA_KUNDALI_H

#include "ast.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class Graham
{
   friend class Kundali;
private:
   string name;
   double sphutam;
   int rasi;
   int house;
public:
   Graham() : name(""), sphutam(-1.0), rasi(-1), house(-1) {}
};


class Kundali
{
private:
   Graham grahams[14];
   int rasiCount;

public:
   Kundali(ST_PL_DET planetDetails[], double ayan);
   
   int getHouseNumber(int pl) const ;
   int getRasiNumber(int pl) const;
   string getRasiName(int pl) const;
   string getGrahaName(int pl) const;
   int getHouseDiff(int pl1, int pl2) const;
   int getRasiCount() const { return rasiCount;}

   bool isGrahamAtUcham(int pl) const;
   bool isGrahamAtNeecham(int pl) const;
   bool isGrahamAtSwasthanam(int pl) const;

   bool isGrahamAtKendramOfAnother(int pl, int another) const;
   bool doesGrahamCauseMahayogam(int pl) const;
  
};

#endif
