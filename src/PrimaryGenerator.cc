#include "PrimaryGenerator.hh"
#include "GeneratorMessenger.hh" // Add this line
#include "RunAction.hh" // Add this line
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <filesystem>
#include "EventAction.hh"
#include "G4RunManager.hh"
namespace SimCalModule
{
    PrimaryGenerator::PrimaryGenerator()
    : fpParticleGun(0),
        fMessenger(new GeneratorMessenger(this)),
        fInputFileName("faser3.10fbInv.1.gfaser.root"),
        fFixedPrimaryVertexPosition(false),
        fPrimaryVertexPosition(G4ThreeVector(0.0,0.0,0.25*cm)),
        ftagNulabel(-1),
        primaryenergy(0),
        fNC(true),
        fCCNue(true),
        fCCNumu(true),
        fCCNutau(true)
    //------------------------------------------------------------------------------
    {
    fpParticleGun = new G4ParticleGun();
    // G4cout << "PrimaryGenerator::PrimaryGenerator()" << G4endl;
    }

    //------------------------------------------------------------------------------
    PrimaryGenerator::~PrimaryGenerator()
    //------------------------------------------------------------------------------
    {
    delete fpParticleGun;
    }

    //------------------------------------------------------------------------------
    void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
    //------------------------------------------------------------------------------
    {
    // fpParticleGun->GeneratePrimaryVertex(anEvent);

    // std::cout<<"Generate Primaries from :" <<fInputFileName << std::endl;
    TFile* file=new TFile(fInputFileName.c_str(),"READ");
    if(!file){
        std::cerr<<"File not found"<<std::endl;
        exit(1);
    }
    TTree* tree=(TTree*)file->Get("gFaser");
    if(!tree){
        std::cerr<<"Tree not found"<<std::endl;
        exit(1);
    }
    std::vector<int>* pdgc = nullptr;
    std::vector<double>* px = nullptr;
    std::vector<double>* py = nullptr;
    std::vector<double>* pz = nullptr;
    std::vector<double>* E = nullptr;
    std::vector<int>* status = nullptr;
    std::vector<int>* firstMother = nullptr;
    double vx;
    double vy;
    double vz;
    tree->SetBranchStatus("*",0);
    tree->SetBranchStatus("pdgc",1);
    tree->SetBranchStatus("px",1);
    tree->SetBranchStatus("py",1);
    tree->SetBranchStatus("pz",1);
    tree->SetBranchStatus("E",1);
    tree->SetBranchStatus("status",1);
    tree->SetBranchStatus("firstMother",1);
    if(fFixedPrimaryVertexPosition==false){
        tree->SetBranchStatus("vx",1);
        tree->SetBranchStatus("vy",1);
        tree->SetBranchStatus("vz",1);
        tree->SetBranchAddress("vx",&vx);
        tree->SetBranchAddress("vy",&vy);
        tree->SetBranchAddress("vz",&vz);
        // std::cout<<"Fix Vertex position : "<<fPrimaryVertexPosition.x()<<" "<<fPrimaryVertexPosition.y()<<" "<<fPrimaryVertexPosition.z()<<std::endl; 
    }
    tree->SetBranchAddress("px",&px);
    tree->SetBranchAddress("py",&py);
    tree->SetBranchAddress("pz",&pz);
    tree->SetBranchAddress("E",&E);
    tree->SetBranchAddress("status",&status);
    tree->SetBranchAddress("pdgc",&pdgc);
    tree->SetBranchAddress("firstMother",&firstMother);
    tree->GetEntry(anEvent->GetEventID());
    G4PrimaryVertex* vertex;
    if(fFixedPrimaryVertexPosition==true) {vertex = new G4PrimaryVertex(fPrimaryVertexPosition,0*ns);}
    else {
        fPrimaryVertexPosition = G4ThreeVector(vx*m,vy*m,vz*m);
        vertex = new G4PrimaryVertex(G4ThreeVector(vx*m,vy*m,vz*m),0*ns);
    }
    G4bool ccnue = false;
    G4bool ccnumu= false;
    G4bool ccnutau= false;
    G4bool nc=  false;
    if(pdgc->size()==0){
        std::cerr<<"No particles found in event"<<std::endl;
        exit(1);
    }
    primaryenergy = E->at(0)*GeV;
    for(size_t j=2;j<pdgc->size();j++){
        if(abs(pdgc->at(0))==12 && status->at(j)==1 && pdgc->at(j)==copysign(11,pdgc->at(0)) && firstMother->at(j)==0){
        ccnue = true;
        break;
        }else if(abs(pdgc->at(0))==14 && status->at(j)==1 && pdgc->at(j)==copysign(13,pdgc->at(0)) && firstMother->at(j)==0){
        ccnumu = true;
        break;
        }else if(abs(pdgc->at(0))==16 && status->at(j)==1 && pdgc->at(j)==copysign(15,pdgc->at(0)) && firstMother->at(j)==0){
        ccnutau = true;
        break;
        }else if(status->at(j)==1 && pdgc->at(j)==pdgc->at(0) && firstMother->at(j)==0){
        nc = true;
        break;
        }
    }
    std::string outputFileName = fInputFileName.substr(0,fInputFileName.find(".root"))+"_interaction.txt";
    std::ofstream outputfile(outputFileName,std::ios_base::app);
    outputfile<<anEvent->GetEventID();
    if(ccnue){
        ftagNulabel = 0;
        outputfile<<" CCNue";
    }
    if(ccnumu){
        outputfile<<" CCNumu";
        ftagNulabel = 1;
    }
    if(ccnutau){
        outputfile<<" CCNutau";
        ftagNulabel = 2;
    }
    if(nc){
        outputfile<<" NC";
        ftagNulabel = 3;
    }
    if(!ccnue && !ccnumu && !ccnutau && !nc){
        outputfile<<" Other";
        ftagNulabel = 4;
    }
    outputfile<<std::endl;
    outputfile.close();
    if(!ccnue && !ccnumu && !ccnutau && !nc){
        std::cerr<<"other neutrino interaction found in event : "<<anEvent->GetEventID()<<std::endl;
    }
    
    if((!fCCNue && ccnue) || (!fCCNumu && ccnumu) || (!fCCNutau && ccnutau) || (!fNC && nc)){
        std::cerr<<"Event not selected : "<<anEvent->GetEventID()<<std::endl;
    }
    for(size_t j=0;j<pdgc->size();j++){
        if(status->at(j)!=1) continue;
        G4PrimaryParticle* particle = new G4PrimaryParticle(pdgc->at(j),px->at(j)*GeV,py->at(j)*GeV,pz->at(j)*GeV);
        vertex->SetPrimary(particle);
    }
    anEvent->AddPrimaryVertex(vertex);
    // if(fRunAction){
    //     fRunAction->AddPrimaryGeneratorData(ftagNulabel,E->at(0),vx*1000,vy*1000,vz*1000);
    // }else{
    //     std::cerr<<"RunAction not found"<<std::endl;
    // }
    file->Close();

    }


    void PrimaryGenerator::SetInputFileName(G4String name)
    {
    fInputFileName = name;
    }

    void PrimaryGenerator::SetFixedPrimaryVertexPosition(G4bool fixed)
    {
    fFixedPrimaryVertexPosition = fixed;
    }

    void PrimaryGenerator::SetPrimaryVertexPosition(G4ThreeVector position)
    {
    fPrimaryVertexPosition = position;
    }

    void PrimaryGenerator::SetCCNue(G4bool ccnue)
    {
    fCCNue = ccnue;
    }

    void PrimaryGenerator::SetCCNumu(G4bool ccnumu)
    {
    fCCNumu = ccnumu;
    }

    void PrimaryGenerator::SetCCNutau(G4bool ccnutau)
    {
    fCCNutau = ccnutau;
    }

    void PrimaryGenerator::SetNC(G4bool nc)
    {
    fNC = nc;
    }

    G4String PrimaryGenerator::GetInputFileName() const
    {
        return fInputFileName;
    }

    G4int PrimaryGenerator::GetftagNulabel() const
    {
        return ftagNulabel;
    }

    G4double PrimaryGenerator::GetPrimaryEnergy() const
    {
        return primaryenergy;
    }

    G4ThreeVector PrimaryGenerator::GetInteractionPlace() const
    {
        return fPrimaryVertexPosition;
    }

    
}
