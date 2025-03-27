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
#include <TgFaserTree.h>
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
        ClearData();
        fpParticleGun = new G4ParticleGun();
        // G4cout << "PrimaryGenerator::PrimaryGenerator()" << G4endl;
        gFaserFile = new TFile(fInputFileName.c_str(),"READ");
        if(!gFaserFile){
            std::cerr<<"File not found"<<std::endl;
            exit(1);
        }
        gFasergFaserTree = (TgFaserTree*)gFaserFile->Get("gFaser");
        if(!gFasergFaserTree){
            std::cerr<<"gFaserTree not found"<<std::endl;
            exit(1);
        }
        gFaserTree->SetBranchStatus("*",0);
        gFaserTree->SetBranchStatus("pdgc",1);
        gFaserTree->SetBranchStatus("px",1);
        gFaserTree->SetBranchStatus("py",1);
        gFaserTree->SetBranchStatus("pz",1);
        gFaserTree->SetBranchStatus("E",1);
        gFaserTree->SetBranchStatus("status",1);
        gFaserTree->SetBranchStatus("firstMother",1);
        gFaserTree->SetBranchStatus("vx",1);
        gFaserTree->SetBranchStatus("vy",1);
        gFaserTree->SetBranchStatus("vz",1);
        gFaserTree->SetBranchAddress("vx",&vx);
        gFaserTree->SetBranchAddress("vy",&vy);
        gFaserTree->SetBranchAddress("vz",&vz);
        gFaserTree->SetBranchAddress("px",&px);
        gFaserTree->SetBranchAddress("py",&py);
        gFaserTree->SetBranchAddress("pz",&pz);
        gFaserTree->SetBranchAddress("E",&E);
        gFaserTree->SetBranchAddress("status",&status);
        gFaserTree->SetBranchAddress("pdgc",&pdgc);
        gFaserTree->SetBranchAddress("firstMother",&firstMother);
    }

    //------------------------------------------------------------------------------
    PrimaryGenerator::~PrimaryGenerator()
    //------------------------------------------------------------------------------
    {
        delete fpParticleGun;
        // delete fMessenger;
        gFaserFile->Close();

    }

    //------------------------------------------------------------------------------
    void PrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
    //------------------------------------------------------------------------------
    {
    ClearData();
    gFaserTree->GetEntry(anEvent->GetEventID());
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
        secondarypdgid = pdgc->at(j);
        secondarymomentum_px = px->at(j);
        secondarymomentum_py = py->at(j);
        secondarymomentum_pz = pz->at(j);
        secondaryenergy = E->at(j);
        break;
        }else if(abs(pdgc->at(0))==14 && status->at(j)==1 && pdgc->at(j)==copysign(13,pdgc->at(0)) && firstMother->at(j)==0){
        ccnumu = true;
        secondarypdgid = pdgc->at(j);
        secondarymomentum_px = px->at(j);
        secondarymomentum_py = py->at(j);
        secondarymomentum_pz = pz->at(j);
        secondaryenergy = E->at(j);
        break;
        }else if(abs(pdgc->at(0))==16 && status->at(j)==1 && pdgc->at(j)==copysign(15,pdgc->at(0)) && firstMother->at(j)==0){
        ccnutau = true;
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
    for(size_t j=0;j<pdgc->size();j++){
        if(status->at(j)!=1) continue;
        G4PrimaryParticle* particle = new G4PrimaryParticle(pdgc->at(j),px->at(j)*GeV,py->at(j)*GeV,pz->at(j)*GeV);
        vertex->SetPrimary(particle);
    }
    for (size_t j = 0; j < vertex->GetNumberOfParticle(); j++){
        G4PrimaryParticle* particle = vertex->GetPrimary(j);
        if (abs(particle->GetPDGcode()) == 13 && abs(particle->GetPx()-secondarymomentum_px*GeV) < 0.001*GeV && abs(particle->GetPy()-secondarymomentum_py*GeV) < 0.001*GeV && abs(particle->GetPz()-secondarymomentum_pz*GeV) < 1*GeV){
            fPrimary_trackid = j+1;
            // if (ccnumu) G4cout<<"prima "<< fPrimary_trackid<<G4endl;
            break;
        }
    }
    if (ccnumu){
        for (size_t j = 0; j < vertex->GetNumberOfParticle(); j++){
            G4PrimaryParticle* particle = vertex->GetPrimary(j);
            if ((particle->GetPDGcode()/100 % 10 == 4 || particle->GetPDGcode()/1000 % 10 == 4)&& particle->GetPDGcode() < 1e6){
                fD_id = j+1;
                fNumuCClabel = 1;
                break;
            }
        }
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

    void PrimaryGenerator::ClearData()
    {
        ftagNulabel = -1;
        primaryenergy = 0;
        secondarypdgid = 0;
        secondaryenergy = -1;
        secondarymomentum_px = -1;
        secondarymomentum_py = -1;
        secondarymomentum_pz = -1;
        fNumuCClabel = -1;
        fD_id = -1;
        fPrimary_trackid = -1;
        pdgc->clear();
        px->clear();
        py->clear();
        pz->clear();
        E->clear();
        status->clear();
        firstMother->clear();
        vx = 0;
        vy = 0;
        vz = 0;
    }
}
