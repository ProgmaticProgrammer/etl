#include <unity.h>

#include <iostream>
#include <memory>

#include "M4ContainmentVessel.h"
#include "M4HotWaterSource.h"
#include "M4UserInterface.h"
#include "coffeemaker.h"

namespace CoffeeMakerTest {
using CoffeeMaker::CoffeeMakerAPI;
using std::make_shared;
using namespace M4CoffeeMaker;
///////////////////////////
//     stub for test    //
/////////////////////////
struct CoffeeMakerStub : CoffeeMakerAPI {
  mutable bool buttonPressed = false;
  bool lightOn = false;
  bool boilerOn = false;
  bool valveClosed = true;
  bool plateOn = false;
  bool boilerEmpty = true;
  bool potPresent = true;
  bool potNotEmpty = false;

  WarmerPlateStatus GetWarmerPlateStatus() const override {
    if (!potPresent)
      return WarmerPlateStatus::WARMER_EMPTY;
    else if (potNotEmpty)
      return WarmerPlateStatus::POT_NOT_EMPTY;
    else
      return WarmerPlateStatus::POT_EMPTY;
  }

  BoilerStatus GetBoilerStatus() const override {
    return boilerEmpty ? BoilerStatus::EMPTY : BoilerStatus::NOT_EMPTY;
  }

  BrewButtonStatus GetBrewButtonStatus() const override {
    if (buttonPressed) {
      buttonPressed = false;
      return BrewButtonStatus::PUSHED;
    } else {
      return BrewButtonStatus::NOT_PUSHED;
    }
  }

  void SetBoilerState(BoilerState boilerState) override {
    boilerOn = boilerState == BoilerState::ON;
  }

  void SetWarmerState(WarmerState warmerState) override {
    plateOn = warmerState == WarmerState::ON;
  }

  void SetIndicatorState(IndicatorState indicatorState) override {
    lightOn = indicatorState == IndicatorState::ON;
  }

  void SetReliefValveState(ReliefValveState reliefValveState) override {
    valveClosed = reliefValveState == ReliefValveState::CLOSED;
  }
};
///////////////////////////
//     utilities        //
/////////////////////////
CoffeeMakerStub api;
auto ui = make_shared<M4UserInterface>(api);
auto hws = make_shared<M4HotWaterSource>(api);
auto cv = make_shared<M4ContainmentVessel>(api);

void Poll() {
  hws->Poll();
  cv->Poll();
  ui->Poll();
}
void Reset() {
  api.buttonPressed = false;
  api.lightOn = false;
  api.boilerOn = false;
  api.valveClosed = true;
  api.plateOn = false;
  api.boilerEmpty = true;
  api.potPresent = true;
  api.potNotEmpty = false;
}
void NormalStart() {
  Poll();
  api.boilerEmpty = false;
  api.buttonPressed = true;
  Poll();
}
/////////////////////////
//    test cases       //
/////////////////////////
void test_InitialConditions(void) {
  TEST_ASSERT_FALSE(api.boilerOn);
  TEST_ASSERT_FALSE(api.lightOn);
  TEST_ASSERT_FALSE(api.plateOn);
  TEST_ASSERT_TRUE(api.valveClosed);
}

void test_StartNoPot(void) {
  api.buttonPressed = true;
  api.potPresent = false;
  Poll();
  TEST_ASSERT_FALSE(api.boilerOn);
  TEST_ASSERT_FALSE(api.lightOn);
  TEST_ASSERT_FALSE(api.plateOn);
  TEST_ASSERT_TRUE(api.valveClosed);
}

void test_StartNoWater(void) {
  api.buttonPressed = true;
  api.boilerEmpty = true;
  Poll();
  TEST_ASSERT_FALSE(api.boilerOn);
  TEST_ASSERT_FALSE(api.lightOn);
  TEST_ASSERT_FALSE(api.plateOn);
  TEST_ASSERT_TRUE(api.valveClosed);
}

void test_GoodStart(void) {
  NormalStart();
  TEST_ASSERT_TRUE(api.boilerOn);
  TEST_ASSERT_FALSE(api.lightOn);
  TEST_ASSERT_FALSE(api.plateOn);
  TEST_ASSERT_TRUE(api.valveClosed);
}

void test_StartedPotNotEmpty() {
  NormalStart();
  api.potNotEmpty = true;
  Poll();
  TEST_ASSERT_TRUE(api.boilerOn);
  TEST_ASSERT_FALSE(api.lightOn);
  TEST_ASSERT_TRUE(api.plateOn);
  TEST_ASSERT_TRUE(api.valveClosed);
}
///////////////////////////
// setup and test entry //
/////////////////////////
void suiteSetUp(void) {
  // std::cout << "suiteSetUp" << std::endl;
  ui->Init(hws, cv);
  hws->Init(ui, cv);
  cv->Init(ui, hws);
}
int test_main(int argc, char** argv) {
  suiteSetUp();
  UNITY_BEGIN();
  RUN_TEST(test_InitialConditions);
  RUN_TEST(test_StartNoPot);
  RUN_TEST(test_StartNoWater);
  RUN_TEST(test_GoodStart);
  RUN_TEST(test_StartedPotNotEmpty);
  return UNITY_END();
}
}  // namespace CoffeeMakerTest

using CoffeeMakerTest::cv;
using CoffeeMakerTest::hws;
using CoffeeMakerTest::Poll;
using CoffeeMakerTest::Reset;
using CoffeeMakerTest::test_main;
using CoffeeMakerTest::ui;
// call for each test
void setUp(void) {
  // std::cout << "setUp" << std::endl;
  Poll();
}
// call for each test
void tearDown(void) { Reset(); }

// int suiteTearDown(int num_failures) { return 0; }

int main(int argc, char** argv) { return test_main(argc, argv); }
