#include "rawSpectra.h"

using namespace std;
rawSpectra::rawSpectra(std::string& inputFile)
{
  ifstream file;
  file.open(inputFile);
  if(!file.good())
  {
    cerr << "Wrong input file: " << inputFile << endl;
    exit(1);
  }
  
  readInputFile(file);
  file.close();
  
}

void rawSpectra::readInputFile(ifstream& file)
{
  double zUp = 0, zDown = 0, TOF = 0;
  while( file >> zUp >> zDown >> TOF )
  {
    fZUp.push_back(zUp);
    fZDown.push_back(zDown);
    fTOF.push_back(TOF);
  }
  if(fZUp.size() != fZDown.size() && fZDown.size() != fTOF.size() )
  {
    cerr << "Something went wrong while reading data from file\n";
    cerr << "Sizes of arrays: " << fZUp.size() << " " << fZDown.size() << " " << fTOF.size() << endl;
    exit(2);
  }
}

void rawSpectra::calculatePoints()
{
  double Y = 0, Z = 0, tanPhi = 0;

  for(unsigned int i = 0; i < fZUp.size(); i++)
  {
    tanPhi = fZUp[i] - fZDown[i];
    tanPhi*=10;	// cm -> mm
    tanPhi/=2*fR;
    
    double dl = 0;
    double c = 299792458;  //m/s
    dl = fTOF[i] * 0.5 * c; 
    dl *= 1E3; // m-> mm
    dl *= 1E-12; // s->ps
    
    cout << dl << endl;
    
    Y = -1 * dl;
    Y /= sqrt(1 + tanPhi * tanPhi);	
    Z = 0.5*( fZUp[i]*10 + fZDown[i]*10 + 2*Y* tanPhi );
    fPoints.push_back( std::make_pair<> (Z, Y) );
  }
}

void rawSpectra::fillRawHist()
{
  fRawHist = new TH2F("rawHisto", "rawHisto", fL, 0, fL, fR*2, 0 ,fR*2); 
  for(auto point: fPoints)
  {
    fRawHist->Fill( point.first+fL/2, point.second+fR, 1); 
  }
}

void rawSpectra::saveRawHist(std::string& path)
{
  calculatePoints();
  fillRawHist();
  path+= ".root";
  fRawHist->SaveAs( path.c_str() );
}

void rawSpectra::setL(double length)
{
  if( length > 0 )
    fL = length;
  else
  {
    cerr << "Cannot set length of scintillator to 0 or smaller" << endl;
    exit(3);
  }
}

void rawSpectra::setR(double radius)
{
  if( radius > 0 )
    fR = radius;
  else
  {
    cerr << "Cannot set radius barrel to 0 or smaller" << endl;
    exit(3);
  }
}

