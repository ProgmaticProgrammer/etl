#include <unity.h>

#include "motorcontroller.h"

// The states.
Idle idle;
Running running;
WindingDown windingDown;
Locked locked;

etl::ifsm_state* stateList[StateId::NUMBER_OF_STATES] = {&idle, &running,
                                                         &windingDown, &locked};

MotorControl motorControl;
etl::null_message_router nmr;

void setUp(void) {
  motorControl.Initialise(stateList, etl::size(stateList));
  motorControl.reset();
  motorControl.ClearStatistics();
}
void tearDown(void) {}

void test_WhenInitialized_ThenNotStarted(void) {
  TEST_ASSERT_FALSE(motorControl.is_started());
}

void test_WhenStarted_ThenIsIdle(void) {
  motorControl.start(false);
  TEST_ASSERT_TRUE(motorControl.is_started());

  TEST_ASSERT_EQUAL(StateId::IDLE, int(motorControl.get_state_id()));
  TEST_ASSERT_EQUAL(StateId::IDLE,
                    int(motorControl.get_state().get_state_id()));

  TEST_ASSERT_FALSE(motorControl.isLampOn_);
  TEST_ASSERT_EQUAL(0, motorControl.setSpeedCount_);
  TEST_ASSERT_EQUAL(0, motorControl.speed_);
  TEST_ASSERT_EQUAL(0, motorControl.startCount_);
  TEST_ASSERT_EQUAL(0, motorControl.stopCount_);
  TEST_ASSERT_EQUAL(0, motorControl.stoppedCount_);
  TEST_ASSERT_EQUAL(0, motorControl.unknownCount_);
}

int main(int argc, char** argv) {
  UNITY_BEGIN();
  RUN_TEST(test_WhenInitialized_ThenNotStarted);
  RUN_TEST(test_WhenStarted_ThenIsIdle);
  UNITY_END();

  return 0;
}