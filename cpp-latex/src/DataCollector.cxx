double get_j_day(int year, int month, int day, double st_longitude) {
  struct date d;
  
 d.da_year = year;
 d.da_mon = month;
 d.da_day = day;

 gtime = 0.0 + st_longitude * 24.0 / 360.0; 
 
 return j_day_from_local_time(d, t, st_longitude);
}

class DataCollector 
{
private:
  std::map<int, ST_DURATION> starMap;
  std:map<int, ST_DURATION> thithiMap;

public:
  DataCollector(int year, int month);
  void collect();
}

DataCollector::DataCollector(int year, int month, double latitude, double longitude, double st_longitude) {
 struct date d;
 DMS t;

  double start_j_day = 
  
}
