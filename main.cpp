#include <iostream>
#include "PSF/psf.h"
#include "RawSpectra/rawSpectra.h"
#include "TStyle.h"

using namespace std;

void workWithPSF(std::string& prePath)
{

  ofstream psfZ, psfY;
  psfZ.open((prePath+"psfZ.txt").c_str());
  psfY.open((prePath+"psfY.txt").c_str());

  for(unsigned int i =99; i < 201; i++)
  {
    std::string path = prePath;
    if(i < 10)
      path+="00";
    else if(i < 100)
      path+="0";
    path+= convertDoubleToStr(i);
    path+=".txt";
    PSF kiko( path );
/*    kiko.findZandY();
    kiko.fitZ();
    kiko.saveZAsPNG();
    kiko.fitY();
    kiko.saveYAsPNG();
  
    psfY << i << "  "<< kiko.getPSFY() << std::endl;
    psfZ << i << "  "<< kiko.getPSFZ() << std::endl;
*/
  }
  
  psfY.close();
  psfZ.close();    
  /*
  for(unsigned int i = 0; i < 420; i+=10)
  {
    kiko.setY(i);
    kiko.fitX();
    kiko.saveXAsPNG();
  } 
   
  for(unsigned int i = 0; i < 300; i+=10)
  {
    kiko.setX(i);
    kiko.fitY();
    kiko.saveYAsPNG();
  }
  */
}

int main(int argc, char **argv) {
      
  gStyle->SetOptFit(1);
  std::string prePath = argv[1];

  //workWithPSF( prePath );
  rawSpectra spectra(prePath);
  spectra.setL(300);
  spectra.setR(410/2);
  spectra.saveRawHist(prePath); 
  
  return 0;
}
