#ifndef RAWSPECTRA
#define RAWSPECTRA

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <cmath>
#include "TH2F.h"



class rawSpectra
{
public:
  rawSpectra(std::string& inputFile);
  void setR(double radius);
  void setL(double length);
  void saveRawHist(std::string& path);
private:
  void fillRawHist();
  void calculatePoints();
  void readInputFile(std::ifstream& file);
  TH2F* fRawHist;
  std::vector< std::pair<double, double> > fPoints;
  std::vector<double> fZUp, fZDown, fTOF;
  double fR, fL;
};

#endif //RAWSPECTRA
