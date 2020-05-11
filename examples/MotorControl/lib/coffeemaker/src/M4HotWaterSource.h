#ifndef M4_HOT_WATER_SOURCE_HEADER
#define M4_HOT_WATER_SOURCE_HEADER
#include <iostream>

#include "HotWaterSource.h"
#include "coffeemaker.h"

namespace M4CoffeeMaker {
using namespace CoffeeMaker;
struct M4HotWaterSource : HotWaterSource {
 private:
  CoffeeMakerAPI& api_;
  BoilerStatus lastBoilerStatus = BoilerStatus::EMPTY;

 public:
  M4HotWaterSource(CoffeeMakerAPI& api) : api_(api) {}

  bool IsReady() const override {
    return api_.GetBoilerStatus() == BoilerStatus::NOT_EMPTY;
  }

  void StartBrewing() override { Resume(); }

  void Poll() {
    // std::cout << "M4HotWaterSource.Poll" << std::endl;
    // if (isBrewing_)
    //   std::cout << "isBrewing_" << std::endl;
    // else
    //   std::cout << "notBrewing_" << std::endl;
    // if (IsReady())
    //   std::cout << "IsReady" << std::endl;
    // else
    //   std::cout << "notsReady" << std::endl;

    if (isBrewing_) {  // brewing
      BoilerStatus boilerStatus = api_.GetBoilerStatus();
      if (boilerStatus != lastBoilerStatus) {
        if (lastBoilerStatus == BoilerStatus::NOT_EMPTY &&
            boilerStatus == BoilerStatus::EMPTY) {  // hot water all gone
          api_.SetBoilerState(BoilerState::OFF);
          api_.SetReliefValveState(ReliefValveState::CLOSED);
          DeclareDone();
        }
        lastBoilerStatus = boilerStatus;
      }
    }
  }

  void Pause() override {
    api_.SetBoilerState(BoilerState::OFF);
    api_.SetReliefValveState(ReliefValveState::OPEN);
  }

  void Resume() override {
    api_.SetBoilerState(BoilerState::ON);
    api_.SetReliefValveState(ReliefValveState::CLOSED);
  }
};

}  // namespace M4CoffeeMaker
#endif  //! M4_HOT_WATER_SOURCE_HEADER