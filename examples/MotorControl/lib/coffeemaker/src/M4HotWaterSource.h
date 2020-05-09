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
    BoilerStatus status = api_.GetBoilerStatus();
    return status == BoilerStatus::NOT_EMPTY;
  }

  void Start() override {
    api_.SetReliefValveState(ReliefValveState::CLOSED);
    api_.SetBoilerState(BoilerState::ON);
  }

  void StartBrewing() override {
    api_.SetReliefValveState(ReliefValveState::CLOSED);
    api_.SetBoilerState(BoilerState::ON);
  }

  void Poll() {
    BoilerStatus boilerStatus = api_.GetBoilerStatus();
    if (isBrewing_) {
      if (boilerStatus == BoilerStatus::EMPTY) {
        api_.SetBoilerState(BoilerState::OFF);
        api_.SetReliefValveState(ReliefValveState::CLOSED);
        DeclareDone();
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

}  // namespace CoffeeMaker
#endif  //! M4_HOT_WATER_SOURCE_HEADER