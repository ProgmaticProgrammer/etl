#ifndef M4_HOT_WATER_SOURCE_HEADER
#define M4_HOT_WATER_SOURCE_HEADER

#include "HotWaterSource.h"
#include "coffeemaker.h"

namespace M4CoffeeMaker {
using namespace CoffeeMaker;
struct M4HotWaterSource : HotWaterSource {
 private:
  CoffeeMakerAPI& api_;

 public:
  M4HotWaterSource(CoffeeMakerAPI& api) : api_(api) {}

  bool IsReady() const override {
    return api_.GetBoilerStatus() == BoilerStatus::NOT_EMPTY;
  }

  void StartBrewing() override { Resume(); }

  void Poll() {
    BoilerStatus boilerStatus = api_.GetBoilerStatus();
    if (isBrewing_ && !IsReady()) {
      Pause();
      DeclareDone();
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