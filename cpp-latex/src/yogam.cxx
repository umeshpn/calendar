#include <yogam.h>
#include <kundali.h>


Yogam::Yogam(const string& yname, const Kundali* kun) : name(yname), kundali(kun)
{}

Yogam::~Yogam()
{}

bool Yogam::checkYogam(vector<int>& planets) const
{
   return false;
}

string Yogam::definition() const 
{
   return "";
}

string Yogam::phalam() const 
{
   return "";
}

void Yogam::printYogam(FILE* fp, vector<int>& planets) const
{
   fprintf(fp, "\\item {\\bf %s}: %s\n\n", this->getName().c_str(), this->definition().c_str());
   if (!planets.empty()) {
      vector<int>::iterator iter;
      fprintf(fp, "\\begin{tabular}{|l|l|r|}\n");
      fprintf(fp, "\\hline\n");
      fprintf(fp, "{\\bf <m>grahaM</m>} & {\\bf <m>raashi</m>} & {\\bf <m>bhaavaM</m>} \\\\\n");
      fprintf(fp, "\\hline\n");
      for (iter = planets.begin(); iter != planets.end(); ++iter) {
         fprintf(fp, "%s & %s & %d \\\\\n", this->kundali->getGrahaName(*iter).c_str(), 
                 this->kundali->getRasiName(*iter).c_str(), 
                 this->kundali->getHouseNumber(*iter));
      }
      fprintf(fp, "\\hline\n");
      fprintf(fp, "\\end{tabular}\n");
   }
   if (show_forecast) {
     fprintf(fp, "\\begin{description}");
     fprintf(fp, "\\item[<m>phalam</m>: ] %s\n\n", this->phalam().c_str());   
     fprintf(fp, "\\end{description}");
   }
}

string ShashaYogam::phalam() const
      {
         return "<m>neet.rtvavaasana, adhikaM taTikkaattatuM uyaramuLLatuM uRaccatuM aaroogyamuLLatumaaya shariiraM, veLuppuniRaM, ellaaM peTTennu ceyyaan taatpaRyaM, veegattiluLLa naTappuM saMsaaravuM, matsaran\"n\"aLil pinmaaRaateyuLLa nilpp, anyadhanalaabhaM, anyag.rhaadhipatyaM, ammayooTu kuuTutal taatpaRyaM, shatrukkaLooTu kSamikkukayuM aaRkkuM kiizhaTan\"n\"ukayuM ceyyaatta svabhaavaM.</m>";
      }


// KesariYogam

bool KesariYogam::checkYogam(vector<int>& planets) const
{
   if (kundali->getHouseDiff(JUPITER, MOON) % 3 == 0) {
      planets.push_back(MOON);
      planets.push_back(JUPITER);
      return true;
   }
   return false;
}

string KesariYogam::definition() const 
{
   return "<m>candraraashiyuTe keendrattil guru nilkkunna yoogaM.</m>";
}

string KesariYogam::phalam() const 
{
   return "<m>dhiirata, saahasikata, paraakramaM enniva uNTaayirikkuM.  prauDhamaayi saMsaarikkuM. maanyatayuM padaviyuM uNTaakuM.  diiRghaayussuM aaroogyavuM uNTaavuM. buddhisaamaRtthyavuM kazhivuM prakaTippikkuM.  yashassu neeTuM.  raajatulyamaaya padaviyil jiivikkuM.</m>";
}


// SharabhaYogam
bool SharabhaYogam::checkYogam(vector<int>& planets) const
{
   if (kundali->getHouseDiff(VENUS, MOON) % 3 == 0) {
      planets.push_back(MOON);
      planets.push_back(VENUS);
      return true;
   }
   return false;
}

string SharabhaYogam::definition() const 
{
   return "<m>candraraashiyuTe keendrattil shukran nilkkunna yoogaM.</m>";
}

string SharabhaYogam::phalam() const 
{
   return "<m>diiRghaayussuM aaroogyavuM dhanavuM uNTaakuM.  sukhajiivitaM nayikkuM.  satsvabhaavavuM sadaacaaraniSThayuM uNTaakuM.  ellaavaruM iSTappeTuM.</m>";
}


// SunabhaaYogam
bool SunabhaaYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, MOON);
   int d2 = kundali->getHouseDiff(MERCURY, MOON);
   int d3 = kundali->getHouseDiff(JUPITER, MOON);
   int d4 = kundali->getHouseDiff(VENUS, MOON);
   int d5 = kundali->getHouseDiff(SATURN, MOON);
   bool s = false;
   bool a = false;
  
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }

   if (s && !a) {
      planets.push_back(MOON);
      if (d1 == 1) {
         planets.push_back(MARS);
      }
      if (d2 == 1) {
         planets.push_back(MERCURY);
      }
      if (d3 == 1) {
         planets.push_back(JUPITER);
      }
      if (d4 == 1) {
         planets.push_back(VENUS);
      }
      if (d5 == 1) {
         planets.push_back(SATURN);
      }
      return true;
   }
   return false;
}

string SunabhaaYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo candraal 2-l nilkkukayuM 12-l nilkkaatirikkukayuM ceyyunna yoogaM.</m>";
}

string SunabhaaYogam::phalam() const 
{
   return "<m>svantaparishramaM koNTu neeTiya dhanaM koNTu sukhajiivitaM nayikkuM.  buddhisaamaRtthyavuM kaRmmasheeSiyuM uNTaakuM.  raajatulyamaaya padaviyil jiivikkuM.  shaastraviSayan\"n\"aLil aRivu neeTuM.  nannaayi saMsaarikkuM.</m>";
}

// AnabhaaYogam
bool AnabhaaYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, MOON);
   int d2 = kundali->getHouseDiff(MERCURY, MOON);
   int d3 = kundali->getHouseDiff(JUPITER, MOON);
   int d4 = kundali->getHouseDiff(VENUS, MOON);
   int d5 = kundali->getHouseDiff(SATURN, MOON);
   bool s = false;
   bool a = false;
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }
   if (!s && a) {
      planets.push_back(MOON);
      if (d1 == 11) {
         planets.push_back(MARS);
      }
      if (d2 == 11) {
         planets.push_back(MERCURY);
      }
      if (d3 == 11) {
         planets.push_back(JUPITER);
      }
      if (d4 == 11) {
         planets.push_back(VENUS);
      }
      if (d5 == 11) {
         planets.push_back(SATURN);
      }
      return true;
   }
   return false;
}

string AnabhaaYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo candraal 12-l nilkkukayuM 2-l nilkkaatirikkukayuM ceyyunna yoogaM.</m>";
}

string AnabhaaYogam::phalam() const 
{
   return "<m>niitiboodhaM, vinayaM, dhiirata, saahasikata enniva uNTaakuM.  pala shaastrashaakhakaLiluM aRivu neeTuM.  striiviSayattil kuuTutal taatpaRyaM kaaNikkuM.</m>";
}

// DhurudhuraaYogam
bool DhurudhuraaYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, MOON);
   int d2 = kundali->getHouseDiff(MERCURY, MOON);
   int d3 = kundali->getHouseDiff(JUPITER, MOON);
   int d4 = kundali->getHouseDiff(VENUS, MOON);
   int d5 = kundali->getHouseDiff(SATURN, MOON);
   bool s = false;
   bool a = false;
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }

   if (s && a) {
      planets.push_back(MOON);
      if (d1 == 1 || d1 == 11) {
         planets.push_back(MARS);
      }
      if (d2 == 1 || d2 == 11) {
         planets.push_back(MERCURY);
      }
      if (d3 == 1 || d3 == 11) {
         planets.push_back(JUPITER);
      }
      if (d4 == 1 || d4 == 11) {
         planets.push_back(VENUS);
      }
      if (d5 == 1 || d5 == 11) {
         planets.push_back(SATURN);
      }
      return true;
   }

   return false;
}

string DhurudhuraaYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo candraal 2-luM 12-luM nilkkunna yoogaM.</m>";
}

string DhurudhuraaYogam::phalam() const 
{
   return "<m>sukhabhoogan\"n\"aLuM dhanavuM aishvaRyavuM uNTaakuM.  nannaayi saMsaarikkuM.  matsaran\"n\"aLil pinmaaRaate nilkkuM.  neet.rtvaM vahikkaanuLLa kazhivuNTaakuM.  eppoozhuM enten\"kiluM ceytukoNTirikkuM.  tyaagashiilamuNTaakuM.  vaLaRttujantukkaL, bhuusvatt, vaahanaM tuTan\"n\"iyava uNTaakuM.</m>";
}

// LagnaadhiYogam
bool LagnaadhiYogam::checkYogam(vector<int>& planets) const
{
   int d;

   d = kundali->getHouseDiff(JUPITER, ASCENDENT);
   if (d < 5 || d > 7) {
      return false;
   }

   d = kundali->getHouseDiff(MERCURY, ASCENDENT);
   if (d < 5 || d > 7) {
      return false;
   }

   d = kundali->getHouseDiff(VENUS, ASCENDENT);
   if (d < 5 || d > 7) {
      return false;
   }

   planets.push_back(JUPITER);
   planets.push_back(MERCURY);
   planets.push_back(VENUS);
   return true;
}

string LagnaadhiYogam::definition() const 
{
   return "<m>lagnaal 6, 7, 8 ennivayil guruvuM budhanuM shukranuM nilkkunna yoogaM.</m>";
}

string LagnaadhiYogam::phalam() const 
{
   return "<m>jiivitattil uyaRccayuNTaakuM.  ellaavaraaluM bahumaanikkappeTuM.  uyaRnna padaviyilettuM.  raajatulyajiivitaM nayikkuM.  diiRghaayussuM aaroogyavuM uNTaakuM.</m>";
}

// ChandraadhiYogam
bool ChandraadhiYogam::checkYogam(vector<int>& planets) const
{
   int d;

   d = kundali->getHouseDiff(JUPITER, ASCENDENT);
   if (d < 5 || d > 7) {
      return false;
   }

   d = kundali->getHouseDiff(MERCURY, ASCENDENT);
   if (d < 5 || d > 7) {
      return false;
   }

   d = kundali->getHouseDiff(VENUS, ASCENDENT);
   if (d < 5 || d > 7) {
      return false;
   }

   planets.push_back(MOON);
   planets.push_back(JUPITER);
   planets.push_back(MERCURY);
   planets.push_back(VENUS);
   return true;
}

string ChandraadhiYogam::definition() const 
{
   return "<m>candraal 6, 7, 8 ennivayil guruvuM budhanuM shukranuM nilkkunna yoogaM.</m>";
}

string ChandraadhiYogam::phalam() const 
{
   return "<m>jiivitattil uyaRccayuNTaakuM.  ellaavaraaluM bahumaanikkappeTuM.  uyaRnna padaviyilettuM.  raajatulyajiivitaM nayikkuM.  diiRghaayussuM aaroogyavuM uNTaakuM.</m>";
}

// KemadrumaYogam
bool KemadrumaYogam::checkYogam(vector<int>& planets) const
{

   for (int i = SUN ; i <= DESC_NODE ; ++i) {
      if (i == MOON) {
         continue;
      }
      int d = kundali->getHouseDiff(i, MOON);
      if ( d == 1 || d == 11) {
         return false;
      }
      if (d % 3 == 0) {
         return false;
      }
      d = kundali->getHouseDiff(i, ASCENDENT);
      if (d % 3 == 0) {
         return false;
      }
   }
   return true;
}

string KemadrumaYogam::definition() const 
{
   return "<m>candran, kujan, budhan, guru, shukran, shani iva niRNNayikkunna oru yoogaM.</m>";
}

string KemadrumaYogam::phalam() const 
{
   return "<m>vaLare ciitta yoogaM.  raajakuTuMbattil janiccaaluM daaridryavuM duHkhavuM uNTaakuM.  yaacikkeeNTi varuM.</m>";
}

// VesiYogam
bool VesiYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, SUN);
   int d2 = kundali->getHouseDiff(MERCURY, SUN);
   int d3 = kundali->getHouseDiff(JUPITER, SUN);
   int d4 = kundali->getHouseDiff(VENUS, SUN);
   int d5 = kundali->getHouseDiff(SATURN, SUN);
   bool s = false;
   bool a = false;
  
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }

   if (s && !a) {
      planets.push_back(SUN);
      if (d1 == 1) {
         planets.push_back(MARS);
      }
      if (d2 == 1) {
         planets.push_back(MERCURY);
      }
      if (d3 == 1) {
         planets.push_back(JUPITER);
      }
      if (d4 == 1) {
         planets.push_back(VENUS);
      }
      if (d5 == 1) {
         planets.push_back(SATURN);
      }
      return true;
   }
   return false;
}

string VesiYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo suuRyaal 2-l nilkkukayuM 12-l nilkkaatirikkukayuM ceyyunna yoogaM.</m>";
}

string VesiYogam::phalam() const 
{
   return "<m>patukkeyuLLa naTattaM, patukkeyuLLa saMsaaraM, saumyasvabhaavaM, bandhukkaLooTu sneehaM, buddhisaamaRtthyaM enniva uNTaakuM.  dhaaraaLaM varavuNTen\"kiluM varavuM celavuM tulyamaayirikkuM.  atinaal valiya sampaadyamoo kaTamoo uNTaavukayilla.</m>";
}

// VasiYogam
bool VasiYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, SUN);
   int d2 = kundali->getHouseDiff(MERCURY, SUN);
   int d3 = kundali->getHouseDiff(JUPITER, SUN);
   int d4 = kundali->getHouseDiff(VENUS, SUN);
   int d5 = kundali->getHouseDiff(SATURN, SUN);
   bool s = false;
   bool a = false;
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }
   if (!s && a) {
      planets.push_back(SUN);
      if (d1 == 11) {
         planets.push_back(MARS);
      }
      if (d2 == 11) {
         planets.push_back(MERCURY);
      }
      if (d3 == 11) {
         planets.push_back(JUPITER);
      }
      if (d4 == 11) {
         planets.push_back(VENUS);
      }
      if (d5 == 11) {
         planets.push_back(SATURN);
      }
      return true;
   }
   return false;
}

string VasiYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo suuRyaal 12-l nilkkukayuM 2-l nilkkaatirikkukayuM ceyyunna yoogaM.</m>";
}

string VasiYogam::phalam() const 
{
   return "<m>manaHkleeshaM, paapacinta, aMgavaikalyaM, alasata, kSiiNaM, amitamaaya celav enniva uNTaakuM.  en\"kiluM 12-l shubhagrahaM ninnaal sukhavuM samaadhaanavuM dhanavuM uNTaakuM.</m>";
}

// UbhayachariYogam
bool UbhayachariYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, SUN);
   int d2 = kundali->getHouseDiff(MERCURY, SUN);
   int d3 = kundali->getHouseDiff(JUPITER, SUN);
   int d4 = kundali->getHouseDiff(VENUS, SUN);
   int d5 = kundali->getHouseDiff(SATURN, SUN);
   bool s = false;
   bool a = false;
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }

   if (s && a) {
      planets.push_back(SUN);
      if (d1 == 1 || d1 == 11) {
         planets.push_back(MARS);
      }
      if (d2 == 1 || d2 == 11) {
         planets.push_back(MERCURY);
      }
      if (d3 == 1 || d3 == 11) {
         planets.push_back(JUPITER);
      }
      if (d4 == 1 || d4 == 11) {
         planets.push_back(VENUS);
      }
      if (d5 == 1 || d5 == 11) {
         planets.push_back(SATURN);
      }
      return true;
   }

   return false;
}

string UbhayachariYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo suuRyaal 2-luM 12-luM nilkkunna yoogaM.</m>";
}

string UbhayachariYogam::phalam() const 
{
   return "<m>yoogattinu kaaraNakkaaraaya grahan\"n\"aL budhan, guru, shukran ennivayaaNen\"kil svajanan\"n\"aLil praadhaanyavuM dhanavuM saRkkaarinRe aMgiikaaravuM uNTaakuM.  kujan, shani ennivayaaNen\"kil dhananaSTaM, manaHkleeshaM tuTan\"n\"iyava uNTaakuM.</m>";
}

// SushubhaaYogam
bool SushubhaaYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, ASCENDENT);
   int d2 = kundali->getHouseDiff(MERCURY, ASCENDENT);
   int d3 = kundali->getHouseDiff(JUPITER, ASCENDENT);
   int d4 = kundali->getHouseDiff(VENUS, ASCENDENT);
   int d5 = kundali->getHouseDiff(SATURN, ASCENDENT);
   bool s = false;
   bool a = false;
  
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }

   if (s && !a) {
      planets.push_back(ASCENDENT);
      if (d1 == 1) {
         planets.push_back(MARS);
      }
      if (d2 == 1) {
         planets.push_back(MERCURY);
      }
      if (d3 == 1) {
         planets.push_back(JUPITER);
      }
      if (d4 == 1) {
         planets.push_back(VENUS);
      }
      if (d5 == 1) {
         planets.push_back(SATURN);
      }
      return true;
   }
   return false;
}

string SushubhaaYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo lagnaal 2-l nilkkukayuM 12-l nilkkaatirikkukayuM ceyyunna yoogaM.</m>";
}

string SushubhaaYogam::phalam() const 
{
   return "<m>dhanaM, striikaLuTe priiti, accaTakkaM, utsaahashiilaM, addhvaanasannaddhata, satsvabhaavaM, sadaacaaraniSTha enniva uNTaakuM.</m>";
}

// AshubhaaYogam
bool AshubhaaYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, ASCENDENT);
   int d2 = kundali->getHouseDiff(MERCURY, ASCENDENT);
   int d3 = kundali->getHouseDiff(JUPITER, ASCENDENT);
   int d4 = kundali->getHouseDiff(VENUS, ASCENDENT);
   int d5 = kundali->getHouseDiff(SATURN, ASCENDENT);
   bool s = false;
   bool a = false;
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }
   if (!s && a) {
      planets.push_back(ASCENDENT);
      if (d1 == 11) {
         planets.push_back(MARS);
      }
      if (d2 == 11) {
         planets.push_back(MERCURY);
      }
      if (d3 == 11) {
         planets.push_back(JUPITER);
      }
      if (d4 == 11) {
         planets.push_back(VENUS);
      }
      if (d5 == 11) {
         planets.push_back(SATURN);
      }
      return true;
   }
   return false;
}

string AshubhaaYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo lagnaal 12-l nilkkukayuM 2-l nilkkaatirikkukayuM ceyyunna yoogaM.</m>";
}

string AshubhaaYogam::phalam() const 
{
   return "<m>tantrashaaliyuM niRbandhabuddhiyuM aayirikkuM.  manaHsvasthata kuRayuM.  iTaykkiTe abhipraayaM maaRuM. celavu kuuTuM.  buddhisaamaRtthyavuM grahaNapaaTavavuM kuRayuM.</m>";
}

// KarttariYogam
bool KarttariYogam::checkYogam(vector<int>& planets) const
{
   int d1 = kundali->getHouseDiff(MARS, ASCENDENT);
   int d2 = kundali->getHouseDiff(MERCURY, ASCENDENT);
   int d3 = kundali->getHouseDiff(JUPITER, ASCENDENT);
   int d4 = kundali->getHouseDiff(VENUS, ASCENDENT);
   int d5 = kundali->getHouseDiff(SATURN, ASCENDENT);
   bool s = false;
   bool a = false;
   if (d1 == 1 || d2 == 1 || d3 == 1 || d4 == 1 || d5 == 1) {
      s = true;
   }
   if (d1 == 11 || d2 == 11 || d3 == 11 || d4 == 11 || d5 == 11) {
      a = true;
   }

   if (s && a) {
      planets.push_back(ASCENDENT);
      if (d1 == 1 || d1 == 11) {
         planets.push_back(MARS);
      }
      if (d2 == 1 || d2 == 11) {
         planets.push_back(MERCURY);
      }
      if (d3 == 1 || d3 == 11) {
         planets.push_back(JUPITER);
      }
      if (d4 == 1 || d4 == 11) {
         planets.push_back(VENUS);
      }
      if (d5 == 1 || d5 == 11) {
         planets.push_back(SATURN);
      }
      return true;
   }

   return false;
}

string KarttariYogam::definition() const 
{
   return "<m>kujan, budhan, guru, shukran, shani ennii grahan\"n\"aLil onnoo atiladhikamoo lagnaal 2-luM 12-luM nilkkunna yoogaM.</m>";
}

string KarttariYogam::phalam() const 
{
   return "<m>yoogattinu kaaraNakkaaraaya grahan\"n\"aL budhan, guru, shukran ennivayaaNen\"kil kuTuMbaadhipatyavuM sukhabhoogan\"n\"aLuM anubhavikkuM.  kujan, shani ennivayaaNen\"kil anyadeeshasan~caaraM, apakaTan\"n\"aL, duHkhaM enniva uNTaakuM.</m>";
}



// LagnaVasumadYogam
bool LagnaVasumadYogam::checkYogam(vector<int>& planets) const
{
   int n;
   int h;

   h= kundali->getHouseNumber(MERCURY);
   if (h == 2 || h == 5 || h == 9 || h == 10) {
      planets.push_back(MERCURY);
      ++n;
   }

   h= kundali->getHouseNumber(JUPITER);
   if (h == 2 || h == 5 || h == 9 || h == 10) {
      planets.push_back(JUPITER);
      ++n;
   }

   h= kundali->getHouseNumber(VENUS);
   if (h == 2 || h == 5 || h == 9 || h == 10) {
      planets.push_back(VENUS);
      ++n;
   }
   return (n>0);
}

string LagnaVasumadYogam::definition() const 
{
   return "<m>lagnaM, budhan, guru, shukran, iva niRNNayikkunna oru yoogaM.</m>";
}

string LagnaVasumadYogam::phalam() const 
{
   return "<m>budhan, guru, shukran ennivayil muunnuM ii yoogattinu kaaraNamaaNen\"kil dhanasam.rddhiyuNTaakuM.  raNTeNNaM mitamaaya dhanalaabhatteyuM oreNNaM alpamaaya dhanalaabhatteyuM suucippikkunnu.  (candravasumadyoogavuM nookkuka.) </m>";
}

// ChandraVasumadYogam
bool ChandraVasumadYogam::checkYogam(vector<int>& planets) const
{
   int n;
   int h;

   h= kundali->getHouseDiff(MERCURY, MOON);
   if (h == 2 || h == 5 || h == 9 || h == 10) {
      planets.push_back(MERCURY);
      ++n;
   }

   h= kundali->getHouseDiff(JUPITER, MOON);
   if (h == 2 || h == 5 || h == 9 || h == 10) {
      planets.push_back(JUPITER);
      ++n;
   }

   h= kundali->getHouseDiff(VENUS, MOON);
   if (h == 2 || h == 5 || h == 9 || h == 10) {
      planets.push_back(VENUS);
      ++n;
   }
   if (n>0) {
       planets.push_back(MOON);
       return true;
   }
   return false;    
}

string ChandraVasumadYogam::definition() const 
{
   return "<m>candran, budhan, guru, shukran, iva niRNNayikkunna oru yoogaM.</m>";
}

string ChandraVasumadYogam::phalam() const 
{
   return "<m>budhan, guru, shukran ennivayil muunnuM ii yoogattinu kaaraNamaaNen\"kil dhanasam.rddhiyuNTaakuM.  raNTeNNaM mitamaaya dhanalaabhatteyuM oreNNaM alpamaaya dhanalaabhatteyuM suucippikkunnu.  (lagnavasumadyoogavuM nookkuka.) </m>";
}


// NipunaYogam
bool NipunaYogam::checkYogam(vector<int>& planets) const
{
   if (kundali->getRasiNumber(SUN) == kundali->getRasiNumber(MERCURY)) { // ADD : Also in 7th
      planets.push_back(SUN);
      planets.push_back(MERCURY);
      return true;
   }
   return false;
}

string NipunaYogam::definition() const 
{
   return "<m>raviyuM budhanuM oree raashiyil varunna yoogaM.</m>";
}

string NipunaYogam::phalam() const 
{
   return "<m>kaaRyapraapti, praayoogikabuddhi, buddhivaibhavaM, pratisandhikaLe neeriTaanuLLa kazhiv, kaRmmakushalata ennivayuNTaakuM.  saMbhaaSaNacaatuRyaM koNT ellaavareyuM vashiikarikkuM.</m>";
}

// ShakataYogam
bool ShakataYogam::checkYogam(vector<int>& planets) const
{
   for (int i=SUN ; i<= DESC_NODE ; ++i) {
      int h = kundali->getHouseNumber(i);
      if (h != 1 && h != 7) {
         return false;
      }
   }
   return true;
}

string ShakataYogam::definition() const 
{
   return "<m>ellaa grahan\"n\"aLuM lagnattiloo 7-loo nilkkunna yoogaM.</m>";
}

string ShakataYogam::phalam() const 
{
   return "<m>daaridryaM, shaariirikaaddhvaanaM koNTu jiivikkeeNTivarika enniva phalaM.</m>";
}


// ShashiMangalaYogam
bool ShashiMangalaYogam::checkYogam(vector<int>& planets) const
{
   if (kundali->getRasiNumber(MOON) == kundali->getRasiNumber(MARS)) {
      planets.push_back(MOON);
      planets.push_back(MARS);
      return true;
   }
   return false;
}

string ShashiMangalaYogam::definition() const 
{
   return "<m>candranuM kujanuM oree raashiyil varunna yoogaM.</m>";
}

string ShashiMangalaYogam::phalam() const 
{
   return "<m>aajiivanaantaM dhanavuM sukhavuM uNTaakuM.</m>";
}

string VeenaaYogam::phalam() const 
{
   return "<m>unnatavidyaabhaasaM, pala vidhattil dhanalaabhaM, sukhabhoogan\"n\"aLuTe anubhavaM, kalaavaasana, utsaahashiilaM, addhvaanasannaddhata enniva uNTaakuM.</m>";
}

string DamaYogam::phalam() const 
{
   return "<m>vaLaRttujantukkaLuM k.rSibhuumiyuM uNTaakuM.  utkkaRSeeccha kuuTuM.  anyaRkku veeNTi addhvaanikkuM.  dayayuM sneehavuM uNTaakuM.</m>";
}

string PaashaYogam::phalam() const 
{
   return "<m>celavu kuuTuM.  atumuulaM kleeshikkuM.  saamaRtthyaM, buddhisaamaRtthyaM, neet.rtvasheeSi, kazhiv enniva uNTaakuM.  ellaavaruM iSTappeTuM.  saRkkaaR jooliyil dhanaM neeTuM.  kuTuMbakaaRyan\"n\"aLil taatpaRyaM kaaNikkuM.</m>";
}

string KedaaraYogam::phalam() const 
{
   return "<m>bandhukkaLkk upakaaraM ceyyuM.  k.rSiyuM bhuusvattuM uNTaakuM.  dhanaM, maanyata, padavi enniva uNTaakuM.  kaRttavyaniSThayuNTaakuM.  sukhajiivitaM nayikkuM.</m>";
}

string ShoolaYogam::phalam() const 
{
   return "<m>buddhisaamaRtthyaM, kSiprakoopaM, amitamaaya celav, saampattikakleeshaM enniva uNTaakuM.  puuR{}vvikadhanaM kaiviTTu pookuM.  potukaaRyan\"n\"aLil taatpaRyaM kaaNikkuM.  santaanan\"n\"aL kuRayuM.</m>";
}

string YugaYogam::phalam() const 
{
   return "<m>aMgavaikalyaM, kaRkkashasvabhaavaM, kaTabaaddhyata, aaroogyakkuRav ennivayuNTaakuM.  bandhubalaM kuRayuM.</m>";
}

string GolaYogam::phalam() const 
{
   return "<m>neetraroogavuM saampattikakleeshavuM duHkhavuM uNTaakuM.  nairaashyavuM aatmahatyaabhilaaSavuM prakaTippikkuM.  manaHsvasthata kuRayuM.</m>";
}

/*

Gurumangala : Jupiter & Mars in the same or 7th houses : Righteous and energetic
Amala : only benefics in 10th from lagna/moon : virtuous and has luxuries
Parvata : kendras have benefics, 7, 8 don't have malefics. : Fortunate charitable, eloquent, easy-going, famous


*/
