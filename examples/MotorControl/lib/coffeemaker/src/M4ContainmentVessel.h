#ifndef M4_CONTAINMENT_VESSEL_HEADER
#define M4_CONTAINMENT_VESSEL_HEADER

#include "ContainmentVessel.h"
#include "coffeemaker.h"

namespace M4CoffeeMaker {
using namespace CoffeeMaker;
struct M4ContainmentVessel : ContainmentVessel {
 private:
  CoffeeMakerAPI& api_;
  WarmerPlateStatus lastPotStatus = WarmerPlateStatus::POT_EMPTY;
  bool isBrewing_ = false;

  void HandleBrewingEvent(WarmerPlateStatus potStatus) {
    if (potStatus == WarmerPlateStatus::POT_NOT_EMPTY) {
      ContainerAvailable();
      api_.SetWarmerState(WarmerState::ON);
    } else if (potStatus == WarmerPlateStatus::WARMER_EMPTY) {
      ContainerUnavailable();
      api_.SetWarmerState(WarmerState::OFF);
    } else {  // potStatus == POT_EMPTY
      ContainerAvailable();
      api_.SetWarmerState(WarmerState::OFF);
    }
  }
  void HandleIncompleteEvent(WarmerPlateStatus potStatus) {
    if (potStatus == WarmerPlateStatus::POT_NOT_EMPTY) {
      api_.SetWarmerState(WarmerState::ON);
    } else if (potStatus == WarmerPlateStatus::WARMER_EMPTY) {
      api_.SetWarmerState(WarmerState::OFF);
    } else {  // potStatus == POT_EMPTY
      api_.SetWarmerState(WarmerState::OFF);
      DeclareComplete();
    }
  }

 public:
  M4ContainmentVessel(CoffeeMakerAPI& api) : api_(api) {}
  bool IsReady() const override {
    WarmerPlateStatus status = api_.GetWarmerPlateStatus();
    return status == WarmerPlateStatus::POT_EMPTY;
  }
  void Start() override { isBrewing_ = true; }

  void Poll() {
    WarmerPlateStatus potStatus = api_.GetWarmerPlateStatus();
    if (potStatus != lastPotStatus) {
      if (isBrewing_) {
        HandleBrewingEvent(potStatus);
      } else if (isComplete_ == false) {
        HandleIncompleteEvent(potStatus);
      }
      lastPotStatus = potStatus;
    }
  }
};

}  // namespace M4CoffeeMaker

#endif  //! M4_CONTAINMENT_VESSEL_HEADER