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
        fCCNutau(true),
        secondarypdgid(0),
        secondaryenergy(-1),
        secondarymomentum_px(-1),
        secondarymomentum_py(-1),
        secondarymomentum_pz(-1),
        fNumuCClabel(-1),
        fD_id(-1),
        fPrimary_trackid(-1)
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
    fNumuCClabel = -1;
    fD_id = -1;
    fPrimary_trackid = -1;
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
    int secondaryindex = -1;
    for(size_t j=2;j<pdgc->size();j++){
        if(abs(pdgc->at(0))==12 && status->at(j)==1 && pdgc->at(j)==copysign(11,pdgc->at(0)) && firstMother->at(j)==0){
        ccnue = true;
        secondaryindex = j;
        secondarypdgid = pdgc->at(j);
        secondarymomentum_px = px->at(j);
        secondarymomentum_py = py->at(j);
        secondarymomentum_pz = pz->at(j);
        secondaryenergy = E->at(j);
        break;
        }else if(abs(pdgc->at(0))==14 && status->at(j)==1 && pdgc->at(j)==copysign(13,pdgc->at(0)) && firstMother->at(j)==0){
        ccnumu = true;
        secondaryindex = j;
        secondarypdgid = pdgc->at(j);
        secondarymomentum_px = px->at(j);
        secondarymomentum_py = py->at(j);
        secondarymomentum_pz = pz->at(j);
        secondaryenergy = E->at(j);
        break;
        }else if(abs(pdgc->at(0))==16 && status->at(j)==1 && pdgc->at(j)==copysign(15,pdgc->at(0)) && firstMother->at(j)==0){
        ccnutau = true;
        secondaryindex = j;
        secondarypdgid = pdgc->at(j);
        secondarymomentum_px = px->at(j);
        secondarymomentum_py = py->at(j);
        secondarymomentum_pz = pz->at(j);
        secondaryenergy = E->at(j);
        break;
        }else if(status->at(j)==1 && pdgc->at(j)==pdgc->at(0) && firstMother->at(j)==0){
        nc = true;
        break;
        }
    }
    // std::string outputFileName = fInputFileName.substr(0,fInputFileName.find(".root"))+"_interaction.txt";
    // std::ofstream outputfile(outputFileName,std::ios_base::app);
    // outputfile<<anEvent->GetEventID();
    if(ccnue){
        ftagNulabel = 0;
        // outputfile<<" CCNue";
    }
    if(ccnumu){
        // outputfile<<" CCNumu";
        ftagNulabel = 1;
        fNumuCClabel = 0;
    }
    if(ccnutau){
        // outputfile<<" CCNutau";
        ftagNulabel = 2;
    }
    if(nc){
        // outputfile<<" NC";
        ftagNulabel = 3;
    }
    if(!ccnue && !ccnumu && !ccnutau && !nc){
        // outputfile<<" Other";
        ftagNulabel = 4;
    }
    // outputfile<<std::endl;
    // outputfile.close();
    if(!ccnue && !ccnumu && !ccnutau && !nc){
        std::cerr<<"other neutrino interaction found in event : "<<anEvent->GetEventID()<<std::endl;
    }
    
    if((!fCCNue && ccnue) || (!fCCNumu && ccnumu) || (!fCCNutau && ccnutau) || (!fNC && nc)){
        std::cerr<<"Event not selected : "<<anEvent->GetEventID()<<std::endl;
    }
    int num_added = 0;
    for(size_t j=0;j<pdgc->size();j++){
        if(status->at(j)!=1) continue;
        G4PrimaryParticle* particle = new G4PrimaryParticle(pdgc->at(j),px->at(j)*GeV,py->at(j)*GeV,pz->at(j)*GeV);
        vertex->SetPrimary(particle);
        num_added++;
        if (j ==secondaryindex){
            fPrimary_trackid = num_added; // Set the primary track ID
        }
    }
    if (ftagNulabel<3 && fPrimary_trackid == -1) {
        std::cerr << "Primary track ID not found for event: " << anEvent->GetEventID() << std::endl;
        fPrimary_trackid = 1;
    }
    // if (ccnumu){
        for (size_t j = 0; j < vertex->GetNumberOfParticle(); j++){
            G4PrimaryParticle* particle = vertex->GetPrimary(j);
            if ((abs(particle->GetPDGcode())/100 % 10 == 4 || abs(particle->GetPDGcode())/1000 % 10 == 4)&& abs(particle->GetPDGcode()) < 1e6){
                fD_id = j+1;
                // fNumuCClabel = 1;
                break;
            }
        }
    // }
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

    G4double PrimaryGenerator::GetSecondaryEnergy() const
    {
        return secondaryenergy;
    }

    G4int PrimaryGenerator::GetSecondarypdgid() const
    {
        return secondarypdgid;
    }

    G4ThreeVector PrimaryGenerator::GetSecondaryMomentum() const
    {
        G4ThreeVector Momentum(secondarymomentum_px,secondarymomentum_py,secondarymomentum_pz);
        return Momentum;
    }

    G4int PrimaryGenerator::GetNumuCClabel() const
    {
        return fNumuCClabel;
    }

    G4int PrimaryGenerator::GetD_id() const
    {
        return fD_id;
    }

    G4int PrimaryGenerator::GetPrimary_trackid() const
    {
        return fPrimary_trackid;
    }

}
