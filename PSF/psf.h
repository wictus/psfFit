#ifndef PSF_H
#define PSF_H

#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <sstream>
#include <iostream>
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TLine.h"

class PSF{
public:
  PSF();
  PSF(std::string& inputFile);
  void setZ(int Z);
  void setY(int Y);
  void findZandY();
  void fitZ();
  void fitY();
  void saveZAsPNG();
  void saveYAsPNG();
  double getPSFZ();
  double getPSFY();
protected:
  TH2F* recoHist;
  void build2DHisto(std::string& name);
  void fillZ();
  void fillY();
  std::vector< std::pair< std::pair<int, int>, double > > fPoints;
  TH1F* fZHisto, *fYHisto;
  int fZ, fY;
  TLine* lineZ, *lineY;
  double PSFZ, PSFY;
  std::string filePath;
};

std::string convertDoubleToStr(const int number);

#endif
