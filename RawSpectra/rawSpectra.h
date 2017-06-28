#ifndef RAWSPECTRA
#define RAWSPECTRA

#include "../PSF/psf.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <cmath>
#include "TH2F.h"



class rawSpectra: public PSF
{
public:
  rawSpectra(std::string& inputFile);
  void setR(double radius);
  void setL(double length);
  void saveRawHist(std::string& path);
  void convertRawPoints();
private:
  
  void fillRawHist();
  void calculatePoints();
  void readInputFile(std::ifstream& file);
  TH2F* fRawHist;
  std::vector< std::pair<double, double> > fRawPoints;
  std::vector<double> fZUp, fZDown, fTOF;
  double fR, fL;
};

#endif //RAWSPECTRA
