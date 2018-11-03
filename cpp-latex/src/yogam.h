
#ifndef STA_YOGAM_H
#define STA_YOGAM_H

#include <ast.h>
#include <string>
#include <vector>
#include <kundali.h>

class Yogam
{
public:
#if 0
   enum MainYogas
   {
      NO_MAIN_YOGAM = 0
      PANCHA_MAHAPURUSHA,
      PARIVARTANA,
      KARTARI,
      OTHER
      
   };
#endif
protected:
   const Kundali* kundali;
   string name;

public:
   Yogam(const string& yname, const Kundali* kun);
   virtual ~Yogam();

   virtual bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

public:
   string getName() const { return name;}
   void printYogam(FILE* fp, vector<int>& planets) const;

};

// Mahapurushayogams
class RuchakaYogam : public Yogam
{
public:
   RuchakaYogam(const Kundali* kun) : Yogam("<m>rucakayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const
      {
         if (kundali->doesGrahamCauseMahayogam(MARS)) {
            planets.push_back(MARS);
            return true;
         }
         return false;
      }
   virtual string definition() const
      {
         return "<m>mahaapuruSayoogan\"n\"aLilonn.  lagnakeendrattil uccaaraashiyiloo svakSeetrattiloo kujan nilkkunna yoogaM.</m>";
      }
   virtual string phalam() const
      {
         return "";
      }
};

class BhadrakaYogam : public Yogam
{
public:
   BhadrakaYogam(const Kundali* kun) : Yogam("<m>bhaadrakayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const
      {
         if (kundali->doesGrahamCauseMahayogam(MERCURY)) {
            planets.push_back(MERCURY);
            return true;
         }
         return false;
      }
   virtual string definition() const
      {
         return "<m>mahaapuruSayoogan\"n\"aLilonn.  lagnakeendrattil uccaaraashiyiloo svakSeetrattiloo budhan nilkkunna yoogaM.</m>";
      }
   virtual string phalam() const
      {
         return "";
      }
};

class HamsaYogam : public Yogam
{
public:
   HamsaYogam(const Kundali* kun) : Yogam("<m>haMsayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const
      {
         if (kundali->doesGrahamCauseMahayogam(JUPITER)) {
            planets.push_back(JUPITER);
            return true;
         }
         return false;
      }
   virtual string definition() const
      {
         return "<m>mahaapuruSayoogan\"n\"aLilonn.  lagnakeendrattil uccaaraashiyiloo svakSeetrattiloo guru nilkkunna yoogaM.</m>";
      }
   virtual string phalam() const
      {
         return "";
      }
};

class MalavyaYogam : public Yogam
{
public:
   MalavyaYogam(const Kundali* kun) : Yogam("<m>maaLavyayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const
      {
         if (kundali->doesGrahamCauseMahayogam(VENUS)) {
            planets.push_back(VENUS);
            return true;
         }
         return false;
      }
   virtual string definition() const
      {
         return "<m>mahaapuruSayoogan\"n\"aLilonn.  lagnakeendrattil uccaaraashiyiloo svakSeetrattiloo shukran nilkkunna yoogaM.</m>";
      }
   virtual string phalam() const
      {
         return "";
      }
};

class ShashaYogam : public Yogam
{
public:
   ShashaYogam(const Kundali* kun) : Yogam("<m>shashayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const
      {
         if (kundali->doesGrahamCauseMahayogam(SATURN)) {
            planets.push_back(SATURN);
            return true;
         }
         return false;
      }
   virtual string definition() const
      {
         return "<m>mahaapuruSayoogan\"n\"aLilonn.  lagnakeendrattil uccaraashiyiloo svakSeetrattiloo shani nilkkunna yoogaM.</m>";
      }
   virtual string phalam() const;
};


class KesariYogam : public Yogam
{
public:
   KesariYogam(const Kundali* kun) : Yogam("<m>keesariyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class SharabhaYogam : public Yogam
{
public:
   SharabhaYogam(const Kundali* kun) : Yogam("<m>sharabhayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};


class SunabhaaYogam : public Yogam
{
public:
   SunabhaaYogam(const Kundali* kun) : Yogam("<m>sunabhaayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class AnabhaaYogam : public Yogam
{
public:
   AnabhaaYogam(const Kundali* kun) : Yogam("<m>anabhaayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class DhurudhuraaYogam : public Yogam
{
public:
   DhurudhuraaYogam(const Kundali* kun) : Yogam("<m>dhurudhuraayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class LagnaadhiYogam : public Yogam
{
public:
   LagnaadhiYogam(const Kundali* kun) : Yogam("<m>lagnaadhiyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class ChandraadhiYogam : public Yogam
{
public:
   ChandraadhiYogam(const Kundali* kun) : Yogam("<m>candraadhiyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class KemadrumaYogam : public Yogam
{
public:
   KemadrumaYogam(const Kundali* kun) : Yogam("<m>keemadrumayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};


class VasiYogam : public Yogam
{
public:
   VasiYogam(const Kundali* kun) : Yogam("<m>vaasiyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class VesiYogam : public Yogam
{
public:
   VesiYogam(const Kundali* kun) : Yogam("<m>veesiyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};


class UbhayachariYogam : public Yogam
{
public:
   UbhayachariYogam(const Kundali* kun) : Yogam("<m>ubhayacariyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class SushubhaaYogam : public Yogam
{
public:
   SushubhaaYogam(const Kundali* kun) : Yogam("<m>sushubhaayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class AshubhaaYogam : public Yogam
{
public:
   AshubhaaYogam(const Kundali* kun) : Yogam("<m>ashubhaayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class KarttariYogam : public Yogam
{
public:
   KarttariYogam(const Kundali* kun) : Yogam("<m>kaRttariyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};


class LagnaVasumadYogam : public Yogam
{
public:
   LagnaVasumadYogam(const Kundali* kun) : Yogam("<m>lagnavasumadyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};


class ChandraVasumadYogam : public Yogam
{
public:
   ChandraVasumadYogam(const Kundali* kun) : Yogam("<m>candravasumadyoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class NipunaYogam : public Yogam
{
public:
   NipunaYogam(const Kundali* kun) : Yogam("<m>nipuNayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class ShakataYogam : public Yogam
{
public:
   ShakataYogam(const Kundali* kun) : Yogam("<m>shakaTayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class ShashiMangalaYogam : public Yogam
{
public:
   ShashiMangalaYogam(const Kundali* kun) : Yogam("<m>shashimaMgalayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const;
   virtual string definition() const;
   virtual string phalam() const;

};

class VeenaaYogam : public Yogam
{
public:
   VeenaaYogam(const Kundali* kun) : Yogam("<m>viiNaayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const {
      return (kundali->getRasiCount() == 7);
   }
   virtual string definition() const {
      return "<m>suuRyaadisaptagrahan\"n\"aL 7 raashikaLil nilkkunna yoogaM.</m>";
   }
   virtual string phalam() const;

};

class DamaYogam : public Yogam
{
public:
   DamaYogam(const Kundali* kun) : Yogam("<m>daamayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const {
      return (kundali->getRasiCount() == 6);
   }
   virtual string definition() const {
      return "<m>suuRyaadisaptagrahan\"n\"aL 6 raashikaLil nilkkunna yoogaM.</m>";
   }
   virtual string phalam() const;

};

class PaashaYogam : public Yogam
{
public:
   PaashaYogam(const Kundali* kun) : Yogam("<m>paashayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const {
      return (kundali->getRasiCount() == 5);
   }
   virtual string definition() const {
      return "<m>suuRyaadisaptagrahan\"n\"aL 5 raashikaLil nilkkunna yoogaM.</m>";
   }
   virtual string phalam() const;

};

class KedaaraYogam : public Yogam
{
public:
   KedaaraYogam(const Kundali* kun) : Yogam("<m>keedaarayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const {
      return (kundali->getRasiCount() == 4);
   }
   virtual string definition() const {
      return "<m>suuRyaadisaptagrahan\"n\"aL 4 raashikaLil nilkkunna yoogaM.</m>";
   }
   virtual string phalam() const;

};

class ShoolaYogam : public Yogam
{
public:
   ShoolaYogam(const Kundali* kun) : Yogam("<m>shuulayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const {
      return (kundali->getRasiCount() == 3);
   }
   virtual string definition() const {
      return "<m>suuRyaadisaptagrahan\"n\"aL 3 raashikaLil nilkkunna yoogaM.</m>";
   }
   virtual string phalam() const;

};

class YugaYogam : public Yogam
{
public:
   YugaYogam(const Kundali* kun) : Yogam("<m>yugayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const {
      return (kundali->getRasiCount() == 2);
   }
   virtual string definition() const {
      return "<m>suuRyaadisaptagrahan\"n\"aL 2 raashikaLil nilkkunna yoogaM.</m>";
   }
   virtual string phalam() const;

};

class GolaYogam : public Yogam
{
public:
   GolaYogam(const Kundali* kun) : Yogam("<m>gooLayoogaM</m>", kun)
      {}
   bool checkYogam(vector<int>& planets) const {
      return (kundali->getRasiCount() == 1);
   }
   virtual string definition() const {
      return "<m>suuRyaadisaptagrahan\"n\"aL 1 raashiyil nilkkunna yoogaM.</m>";
   }
   virtual string phalam() const;

};




#endif

