#include "TRandom3.h"
#include "TVector3.h"
#include "TFile.h"
#include "TVector3.h"
#include <iostream>
#include <math.h>
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TTree.h"

using namespace std;

main(int argc, char* argv[])
{
  bool verbose = false;
  
  // Simulate N events
  int Nevents = 3;

  for(int i = 1; i < argc; i++)
  {
    if(string(argv[i]) == "-v")
    {
      verbose = true;
      cout << "Verbose mode enabled." << endl;
    }
    else if(string(argv[i]) == "-n")
    {
      Nevents = atoi(argv[i + 1]);
      cout << "Simulating " << Nevents << " events." << endl;
    }
  }

  TRandom3 *vertexGenerator = new TRandom3();
  vertexGenerator->SetSeed(1234);
  TRandom3 *modeGenerator = new TRandom3();
  vertexGenerator->SetSeed(2341);
  TRandom3 *energyGenerator = new TRandom3();
  energyGenerator->SetSeed(3412);

  TFile F("testfile.root","RECREATE");


  //Define simulation objects
  Int_t Event = 0;
  TVector3 intVertex;
  Double_t Energy = 0;
  Int_t Nmodes = 0;
  TVector3 modeVector;
  TVector3 faceIntersect;
  int   faceID = 0;
  float distance = 0;
  float fD = 100;
  float dD = 0; //dummy variable 
  float numerator = 0;
  float denominator = 0;
  float time = 0;
  float speed = 0;

  //Create Tree and Define Branches
  TTree* tPhonon = new TTree("tPhonon","Root tree storing phonon simulation information"); 
  tPhonon->Branch("Event",&Event);
  tPhonon->Branch("intVertex",&intVertex);
  tPhonon->Branch("Energy",&Energy);
  tPhonon->Branch("Nmodes",&Nmodes);
  tPhonon->Branch("faceIntersect",&faceIntersect);
  tPhonon->Branch("faceID",&faceID);
  tPhonon->Branch("modeVector",&modeVector);
  tPhonon->Branch("distance",&distance);
  tPhonon->Branch("time",&time);
  tPhonon->Branch("fD",&fD);

  //vertex histogram
  TH3F* hVer = new TH3F("hVer","Phonon Simulation Vertex Positions",35,-25,25,35,-25,25,35,-25,25);

  // Define Phonon axes
  if(verbose) cout << "Phonon axes :" << endl;
  TVector3 a(1,1,0);
  a = a.Unit();
  if(verbose) a.Print();
  TVector3 b(1,0,0);
  if(verbose) b.Print();
  TVector3 c(0,0,1);
  if(verbose) c.Print();
  if(verbose) cout << endl << endl;

  // Define Face Axes/Points
  if(verbose) cout << "Face Axes :" << endl;
  TVector3 n1v(0,-1,0);
  TVector3 f1p = n1v*25;
  if(verbose) n1v.Print();
  TVector3 n2v(0,0,1);
  TVector3 f2p = n2v*25;
  if(verbose) n2v.Print();
  TVector3 n3v(1,0,0);
  TVector3 f3p = n3v*25;
  if(verbose) n3v.Print();
  TVector3 n4v(0,0,-1);
  TVector3 f4p = n4v*25;
  if(verbose) n4v.Print();
  TVector3 n5v(0,1,0);
  TVector3 f5p = n5v*25;
  if(verbose) n5v.Print();
  TVector3 n6v(-1,0,0);
  TVector3 f6p = n6v*25;
  if(verbose) n6v.Print();
  if(verbose) cout << endl << endl;

  if(verbose) cout << "Energy" << '\t' << " x " << '\t' << " y " << '\t' << " z " << endl;
  if(verbose) cout << "______" << '\t' << "___" << '\t' << "___" << '\t' << "___" << endl;
  
  for( Event = 0; Event < Nevents; Event++ )
  {
    Double_t x = 0;
    Double_t y = 0;
    Double_t z = 0;
    Int_t mode = 0;

    //Generate Vertex
    x = vertexGenerator->Uniform(-25.0,25.0);
    y = vertexGenerator->Uniform(-25.0,25.0);
    z = vertexGenerator->Uniform(-25.0,25.0);
    intVertex.SetXYZ(x,y,z);

    hVer->Fill(x,y,z);

    //Generate a degraded alpha-like (point-like) energy from a flat spectrum (Kadel, 2010)
    Energy = energyGenerator->Uniform(1,3);
    //Approximate number of modes from non-equilibrium excitation
    Nmodes = 300;

    //Generate (Energy dependant) N phonons with modes randomly distributed
    for(int j =0 ; j < Nmodes ; j++)
    {
      
      mode = modeGenerator->Integer(3);

      if (mode < 1){
        modeVector = a;
        speed = 1;
      }
      else if ( mode < 2){
          modeVector = b;
          speed = 7;
      }
      else{
          modeVector = c;
          speed = 1;
      }
    
      // calculate distance to intersection with plane
      // 3 options:
      // parallel no intersection -> numerator != 0 & denominator = 0
      // parallel and in plane -> numerator = 0 & denominator = 0
      // point intersection ->
      for( int f = 1 ; f < 7; f++)
      {
        TVector3 faceNormal;
        TVector3 facePoint;
        TVector3 displacementVector;

        if (f==1)
        { 
          faceNormal = n1v;
          facePoint = f1p;
          fD = TMath::Abs(faceNormal*intVertex + 25);
          if(verbose) cout << Energy << '\t' << x << '\t' << y << '\t' << z << endl;
          if(verbose) cout << "mode: " << mode << " -> Mode Vector: ";
          if(verbose) modeVector.Print();
          if(verbose) cout << endl;
        }
        else if (f==2) 
        {
          faceNormal = n2v;
          facePoint = f2p;
          dD = TMath::Abs(faceNormal*intVertex + 25);
          if(dD < fD) fD = dD;

        }
        else if (f==3) 
        {
          faceNormal = n3v;
          facePoint = f3p;
          dD = TMath::Abs(faceNormal*intVertex + 25);
          if(dD < fD) fD = dD;
        }
        else if (f==4) 
        {
          faceNormal = n4v;
          facePoint = f4p;
          dD = TMath::Abs(faceNormal*intVertex + 25);
          if(dD < fD) fD = dD;
        }
        else if (f==5) 
        {
          faceNormal = n5v;
          facePoint = f5p;
          dD = TMath::Abs(faceNormal*intVertex + 25);
          if(dD < fD) fD = dD;
        }
        else if (f==6) 
        {
          faceNormal = n6v;
          facePoint = f6p;
          dD = TMath::Abs(faceNormal*intVertex + 25);
          if(dD < fD) fD = dD;
        }

        numerator = (facePoint - intVertex)*faceNormal;
        denominator = modeVector*faceNormal;

        //if(numerator == 0 ) continue;
        //if(denominator == 0 ) continue;
        
        distance = numerator/denominator;
        displacementVector.SetMagThetaPhi(distance,modeVector.Theta(),modeVector.Phi());
        if( distance < 0 ) displacementVector= -1*displacementVector;
        faceIntersect = intVertex + displacementVector;
        time = distance/speed;

        if( TMath::Abs(TMath::Nint(faceIntersect.X())) > 25) continue;
        if( TMath::Abs(TMath::Nint(faceIntersect.Y())) > 25) continue;
        if( TMath::Abs(TMath::Nint(faceIntersect.Z())) > 25) continue;

        //print results
        if(verbose) cout << "face: " << f << "\tnumerator: " << '\t' << numerator << "\tdenominator: " << '\t' << denominator << "\tdistance: " << '\t' << distance << endl; 
        if(verbose) cout << "intersection point: ";
        if(verbose) faceIntersect.Print();
        if(verbose) cout << endl;
        if( (distance > 0) && !(isinf(distance)) ) tPhonon->Fill();
      }
        

    }
    
    
    if(verbose) cout << endl << endl;
  }

  hVer->Write();
  tPhonon->Write();
  F.Close();

  return 0;
}
