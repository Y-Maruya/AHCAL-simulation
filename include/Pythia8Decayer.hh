#ifndef Pythia8Decayer_H
#define Pythia8Decayer_H

#include "G4VExtDecayer.hh"
#include "globals.hh"
#include "Pythia8/Pythia.h"

class G4Track;
class G4DecayProducts;

class Pythia8Decayer : public G4VExtDecayer
{
public:
    Pythia8Decayer(const std::string s);
    virtual G4DecayProducts* ImportDecayProducts(const G4Track&);

private:
    std::unique_ptr<Pythia8::Pythia> m_decayer;
};

#endif
