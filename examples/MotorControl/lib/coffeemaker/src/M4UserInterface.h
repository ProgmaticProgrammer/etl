#ifndef M4_USER_INTERFACE_HEADER
#define M4_USER_INTERFACE_HEADER
#include <iostream>

#include "UserInterface.h"
#include "coffeemaker.h"
namespace M4CoffeeMaker {
using namespace CoffeeMaker;
struct M4UserInterface : UserInterface {
 private:
  CoffeeMakerAPI& api_;

  void CheckButton() {
    if (api_.GetBrewButtonStatus() == BrewButtonStatus::PUSHED) {
      StartBrewing();
    }
  }

 public:
  M4UserInterface(CoffeeMakerAPI& api) : api_(api) {}
  void Poll() { CheckButton(); }

 public:
  virtual void Done() override {
    api_.SetIndicatorState(IndicatorState::ON);
    std::cout << "SetIndicatorState::on" << std::endl;
  }

 protected:
  virtual void CompleteCycle() override {
    api_.SetIndicatorState(IndicatorState::OFF);
    std::cout << "SetIndicatorState::off" << std::endl;
  }
};

}  // namespace M4CoffeeMaker
#endif  // !M4_USER_INTERFACE_HEADER