#ifndef simulation2ActionInitialization_h
#define simulation2ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

namespace simulation2
{

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization() = default;
    ~ActionInitialization() override = default;

    void BuildForMaster() const override;
    void Build() const override;
};

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
