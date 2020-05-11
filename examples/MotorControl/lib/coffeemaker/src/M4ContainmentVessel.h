#ifndef M4_CONTAINMENT_VESSEL_HEADER
#define M4_CONTAINMENT_VESSEL_HEADER

#include <iostream>

#include "ContainmentVessel.h"
#include "coffeemaker.h"
namespace M4CoffeeMaker {
using namespace CoffeeMaker;
struct M4ContainmentVessel : ContainmentVessel {
 private:
  CoffeeMakerAPI& api_;
  WarmerPlateStatus lastPotStatus = WarmerPlateStatus::POT_EMPTY;

  void HandleBrewingEvent(WarmerPlateStatus potStatus) {
    switch (potStatus) {
      case WarmerPlateStatus::WARMER_EMPTY:
        ContainerUnavailable();
        api_.SetWarmerState(WarmerState::OFF);
        break;
      case WarmerPlateStatus::POT_NOT_EMPTY:
        ContainerAvailable();
        api_.SetWarmerState(WarmerState::ON);
        break;
      case WarmerPlateStatus::POT_EMPTY:
        ContainerAvailable();
        api_.SetWarmerState(WarmerState::OFF);
        break;
      default:
        break;
    }
  }
  void HandleIncompleteEvent(WarmerPlateStatus potStatus) {
    switch (potStatus) {
      case WarmerPlateStatus::POT_NOT_EMPTY:
        api_.SetWarmerState(WarmerState::ON);
        break;
      case WarmerPlateStatus::WARMER_EMPTY:
        api_.SetWarmerState(WarmerState::OFF);
        break;
      case WarmerPlateStatus::POT_EMPTY:
        api_.SetWarmerState(WarmerState::OFF);
        DeclareComplete();
        break;
      default:
        break;
    }
  }

 public:
  M4ContainmentVessel(CoffeeMakerAPI& api) : api_(api) {}
  bool IsReady() const override {
    return api_.GetWarmerPlateStatus() == WarmerPlateStatus::POT_EMPTY;
  }

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