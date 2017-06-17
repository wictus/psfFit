#include "psf.h"

PSF::PSF(std::string inputFile)
{
 std::ifstream file;
 file.open( inputFile.c_str() );
 int z, y;
 double fill;
 while( file >> z >> y >> fill )
 {
   fPoints.push_back( std::make_pair( std::make_pair(z ,-1.0*y ) , fill ) );
 }
 build2DHisto(inputFile);
 filePath = inputFile;
}

void PSF::build2DHisto(std::string& file)
{
  std::string name = "recoHist"+file;
  recoHist = new TH2F(name.c_str(),name.c_str(), 500, 0, 500, 410, 0, 410 );
  recoHist->GetXaxis()->SetTitle("Z [mm]");
  recoHist->GetYaxis()->SetTitle("Y [mm]");
  for( unsigned int i = 0; i < fPoints.size(); i++)
  {
    recoHist->Fill( fPoints[i].first.first, fPoints[i].first.second, fPoints[i].second);
	std::cout<<"Filling: " << fPoints[i].first.first << " " << fPoints[i].first.second << " with " << fPoints[i].second << std::endl;
  }
  TCanvas* c = new TCanvas();
  recoHist->SetMinimum(1);
  recoHist->Draw("colz");
  std::cout<<recoHist->GetEntries()<<std::endl;
  name= name.substr(name.find_last_of("/\\")+1);
  name= name.substr(0, name.find_last_of(".") );
  
  name+=".root";
  name="roots/"+name;
  c->SaveAs(name.c_str());
  name = name.substr(0,name.find_last_of(".") );
  name = name.substr(name.find_last_of("/")+1);
  name+=".png";
  name="pngs/"+name;
  c->SaveAs(name.c_str());
  
  
  delete recoHist;
}

void PSF::findZandY()
{
  double max = 0.0;
  for(unsigned int i = 0; i < fPoints.size(); i++)
  {
    if( fPoints[i].second > max )
    {
      max = fPoints[i].second;
      setZ( fPoints[i].first.first );
      setY( fPoints[i].first.second );
    }
  }
}

void PSF::setZ(int z)
{
  fZ = z;
}

void PSF::setY(int Y)
{
  fY = Y;
}

double PSF::getPSFZ()
{
  return PSFZ;
}

double PSF::getPSFY()
{
  return PSFY;
}


void PSF::fitZ()
{ 
  fillZ();
  TF1* parabola = new TF1("parabola","[0]*(x-[1])**2 + [2]", fZ-1, fZ+1);
  
  parabola->SetParameter(1,fZ);
  parabola->SetParameter(2, fZHisto->GetBinContent( fZHisto->GetMaximumBin() ) );
  fZHisto->Fit("parabola","QIR");
  
  double halfHeight = parabola->GetParameter(2) / 2.0;
  int binLeft= fZHisto->FindFirstBinAbove( halfHeight );
  
  TF1* lineLeft = new TF1("lineLeft","[0]*x + [1]",fZHisto->GetBinCenter( binLeft -1 ),fZHisto->GetBinCenter( binLeft ));
  fZHisto->Fit("lineLeft","QIR");
  
  int binRight = fZHisto->FindLastBinAbove(halfHeight);
  TF1* lineRight = new TF1("lineRight","[0]*x + [1]",fZHisto->GetBinCenter( binRight ), fZHisto->GetBinCenter( binRight+1 ) );
  fZHisto->Fit("lineRight","QIR");
  
  double leftPoint = ( halfHeight - lineLeft->GetParameter(1) ) / lineLeft->GetParameter(0);
  double rightPoint = ( halfHeight - lineRight->GetParameter(1) ) / lineRight->GetParameter(0);
    
  PSFZ = rightPoint - leftPoint;
  
  lineZ = new TLine(leftPoint, halfHeight, rightPoint, halfHeight);
  
}

void PSF::fitY()
{ 
  fillY();
  TF1* parabola = new TF1("parabola","([0]*(x-[1])*(x-[1]))+[2]", fY-1, fY+1);  
  parabola->SetParameter(1,fY);
  parabola->SetParameter(2, fYHisto->GetBinContent( fYHisto->GetMaximumBin() ) );
  fYHisto->Fit("parabola","QIR");
  
  double halfHeight = parabola->GetParameter(2) / 2.0;
  int binLeft= fYHisto->FindFirstBinAbove( halfHeight );
  
  TF1* lineLeft = new TF1("lineLeft","[0]*x + [1]",fYHisto->GetBinCenter( binLeft -1 ),fYHisto->GetBinCenter( binLeft ));
  fYHisto->Fit("lineLeft","QIR");
  
  int binRight = fYHisto->FindLastBinAbove(halfHeight);
  TF1* lineRight = new TF1("lineRight","[0]*x + [1]",fYHisto->GetBinCenter( binRight ), fYHisto->GetBinCenter( binRight+1 ) );
  fYHisto->Fit("lineRight","QIR");
  
  double leftPoint = ( halfHeight - lineLeft->GetParameter(1) ) / lineLeft->GetParameter(0);
  double rightPoint = ( halfHeight - lineRight->GetParameter(1) ) / lineRight->GetParameter(0);
    
  PSFY = rightPoint - leftPoint;
  
  lineY = new TLine(leftPoint, halfHeight, rightPoint, halfHeight);
}

void PSF::fillZ()
{
  fZHisto = new TH1F("zProjection","zProjection",500,0,500);
  fZHisto->GetXaxis()->SetTitle("");
  for(unsigned int i = 0; i < fPoints.size(); i++)
  {
    if( fY == fPoints[i].first.second )
      fZHisto->Fill(fPoints[i].first.first, fPoints[i].second);
  }
}

void PSF::fillY()
{ 
  fYHisto = new TH1F("yProjection","yProjection",420,0,420);
  for(unsigned int i = 0; i < fPoints.size(); i++)
  {
    if( fZ == fPoints[i].first.first)
      fYHisto->Fill(fPoints[i].first.second, fPoints[i].second);
  }
}


void PSF::saveZAsPNG()
{
  TCanvas* canvas = new TCanvas();
  fZHisto->Draw();
  lineZ->Draw();
  std::string name = "zProjection"+filePath;
  
  name= name.substr(name.find_last_of("/\\")+1);
  name= name.substr(0, name.find_last_of(".") );
  
  std::string pngName= "Zprojection/"+name+".png";
  std::string rootName = "Zprojection/"+name+".root";
  
  canvas->SaveAs( pngName.c_str() );
  canvas->SaveAs( rootName.c_str() );
  
  fZHisto->Reset();
  delete fZHisto;
}

void PSF::saveYAsPNG()
{
  TCanvas* canvas = new TCanvas();
  fYHisto->Draw();
  lineY->Draw();
  std::string name = "yProjection"+filePath;
  
  name= name.substr(name.find_last_of("/\\")+1);
  name= name.substr(0, name.find_last_of(".") );
    
  std::string pngName= "Yprojection/"+name+".png";
  std::string rootName = "Yprojection/"+name+".root";
  
  canvas->SaveAs( pngName.c_str() );
  canvas->SaveAs( rootName.c_str() );
  
  fYHisto->Reset();
  delete fYHisto;
}

std::string convertDoubleToStr(const int number)
{
  std::ostringstream strs;
  strs.str("");
  strs.clear();
  strs << number;
  return strs.str();
}
