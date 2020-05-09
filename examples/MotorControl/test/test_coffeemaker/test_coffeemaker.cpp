#include <unity.h>

#include <memory>

#include "M4ContainmentVessel.h"
#include "M4HotWaterSource.h"
#include "M4UserInterface.h"
#include "coffeemaker.h"
using std::make_shared;
using namespace CoffeeMaker;
using namespace M4CoffeeMaker;
namespace CoffeeMakerTest {

struct CoffeeMakerStub : CoffeeMakerAPI {
  bool buttonPressed;
  bool lightOn;
  bool boilerOn;
  bool valveClosed;
  bool plateOn;
  bool boilerEmpty;
  bool potPresent;
  bool potNotEmpty;

  CoffeeMakerStub() {
    buttonPressed = false;
    lightOn = false;
    boilerOn = false;
    valveClosed = true;
    plateOn = false;
    boilerEmpty = true;
    potPresent = true;
    potNotEmpty = false;
  }

  WarmerPlateStatus GetWarmerPlateStatus() {
    if (!potPresent)
      return WarmerPlateStatus::WARMER_EMPTY;
    else if (potNotEmpty)
      return WarmerPlateStatus::POT_NOT_EMPTY;
    else
      return WarmerPlateStatus::POT_EMPTY;
  }

  BoilerStatus GetBoilerStatus() {
    return boilerEmpty ? BoilerStatus::EMPTY : BoilerStatus::NOT_EMPTY;
  }

  BrewButtonStatus GetBrewButtonStatus() {
    if (buttonPressed) {
      buttonPressed = false;
      return BrewButtonStatus::PUSHED;
    } else {
      return BrewButtonStatus::NOT_PUSHED;
    }
  }

  void SetBoilerState(BoilerState boilerState) {
    boilerOn = boilerState == BoilerState::ON;
  }

  void SetWarmerState(WarmerState warmerState) {
    plateOn = warmerState == WarmerState::ON;
  }

  void SetIndicatorState(IndicatorState indicatorState) {
    lightOn = indicatorState == IndicatorState::ON;
  }

  void SetReliefValveState(ReliefValveState reliefValveState) {
    valveClosed = reliefValveState == ReliefValveState::CLOSED;
  }
};

CoffeeMakerStub api;
auto ui = make_shared<M4UserInterface>(api);
auto hws = make_shared<M4HotWaterSource>(api);
auto cv = make_shared<M4ContainmentVessel>(api);

}  // namespace CoffeeMakerTest
using namespace CoffeeMakerTest;
void setUp(void) {}
void tearDown(void) {}
void suiteSetUp(void) {
  ui->Init(hws, cv);
  hws->Init(ui, cv);
  cv->Init(ui, hws);
}
int suiteTearDown(int num_failures) { return 0; }

void test_WhenInitialized_ThenNotStarted(void) {}

int main(int argc, char** argv) {
  UNITY_BEGIN();
  RUN_TEST(test_WhenInitialized_ThenNotStarted);
  UNITY_END();

  return 0;
}