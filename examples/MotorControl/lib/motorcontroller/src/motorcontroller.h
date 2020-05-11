#ifndef MOTOR_CONTROLLER_HEADER
#define MOTOR_CONTROLLER_HEADER

#include "etl/container.h"
#include "etl/enum_type.h"
#include "etl/fsm.h"
#include "etl/packet.h"
#include "etl/queue.h"

//***************************************************************************
// Events
struct EventId {
  enum enum_type { START, STOP, STOPPED, SET_SPEED, RECURSIVE, UNSUPPORTED };

  ETL_DECLARE_ENUM_TYPE(EventId, etl::message_id_t)
  ETL_ENUM_TYPE(START, "Start")
  ETL_ENUM_TYPE(STOP, "Stop")
  ETL_ENUM_TYPE(STOPPED, "Stopped")
  ETL_ENUM_TYPE(SET_SPEED, "Set Speed")
  ETL_ENUM_TYPE(RECURSIVE, "Recursive")
  ETL_ENUM_TYPE(UNSUPPORTED, "Unsupported")
  ETL_END_ENUM_TYPE
};

class Start : public etl::message<EventId::START> {};

class Stop : public etl::message<EventId::STOP> {
 public:
  Stop() : isEmergencyStop(false) {}
  Stop(bool emergency) : isEmergencyStop(emergency) {}

  const bool isEmergencyStop;
};

class SetSpeed : public etl::message<EventId::SET_SPEED> {
 public:
  SetSpeed(int speed_) : speed(speed_) {}

  const int speed;
};

class Stopped : public etl::message<EventId::STOPPED> {};

class Recursive : public etl::message<EventId::RECURSIVE> {};

class Unsupported : public etl::message<EventId::UNSUPPORTED> {};
// Events - end
//***************************************************************************
// States
struct StateId {
  enum enum_type { IDLE, RUNNING, WINDING_DOWN, LOCKED, NUMBER_OF_STATES };

  ETL_DECLARE_ENUM_TYPE(StateId, etl::fsm_state_id_t)
  ETL_ENUM_TYPE(IDLE, "Idle")
  ETL_ENUM_TYPE(RUNNING, "Running")
  ETL_ENUM_TYPE(WINDING_DOWN, "Winding Down")
  ETL_ENUM_TYPE(LOCKED, "Locked")
  ETL_END_ENUM_TYPE
};

//***********************************
// The motor control FSM.
//***********************************

const etl::message_router_id_t MOTOR_CONTROL = 0;

class MotorControl : public etl::fsm {
 public:
  MotorControl() : fsm(MOTOR_CONTROL) {}


  void Initialise(etl::ifsm_state** p_states, size_t size) {
    set_states(p_states, size);
    ClearStatistics();
  }


  void ClearStatistics() {
    startCount_ = 0;
    stopCount_ = 0;
    setSpeedCount_ = 0;
    unknownCount_ = 0;
    stoppedCount_ = 0;
    isLampOn_ = false;
    speed_ = 0;
  }


  void SetSpeedValue(int speed) { speed_ = speed; }


  void TurnRunningLampOn() { isLampOn_ = true; }


  void TurnRunningLampOff() { isLampOn_ = false; }


  template <typename T>
  void queue_recursive_message(const T& message) {
    messageQueue.emplace(message);
  }

  typedef etl::largest<Start, Stop, SetSpeed, Stopped, Recursive> Largest_t;

  typedef etl::packet<etl::imessage, Largest_t::size, Largest_t::alignment>
      Packet_t;

  etl::queue<Packet_t, 2> messageQueue;

  int startCount_ = 0;
  int stopCount_ = 0;
  int setSpeedCount_ = 0;
  int unknownCount_ = 0;
  int stoppedCount_ = 0;
  bool isLampOn_ = false;
  int speed_ = 0;
};

//***********************************
// The idle state.
//***********************************
class Idle : public etl::fsm_state<MotorControl, Idle, StateId::IDLE, Start,
                                   Recursive> {
 public:

  etl::fsm_state_id_t on_event(etl::imessage_router&, const Start&) {
    ++get_fsm_context().startCount_;
    return StateId::RUNNING;
  }


  etl::fsm_state_id_t on_event(etl::imessage_router&, const Recursive&) {
    get_fsm_context().queue_recursive_message(Start());
    return StateId::IDLE;
  }


  etl::fsm_state_id_t on_event_unknown(etl::imessage_router&,
                                       const etl::imessage&) {
    ++get_fsm_context().unknownCount_;
    return STATE_ID;
  }


  etl::fsm_state_id_t on_enter_state() {
    get_fsm_context().TurnRunningLampOff();
    return StateId::LOCKED;
  }
};

//***********************************
// The running state.
//***********************************
class Running : public etl::fsm_state<MotorControl, Running, StateId::RUNNING,
                                      Stop, SetSpeed> {
 public:
  etl::fsm_state_id_t on_event(etl::imessage_router&, const Stop& event) {
    ++get_fsm_context().stopCount_;

    if (event.isEmergencyStop) {
      return StateId::IDLE;
    } else {
      return StateId::WINDING_DOWN;
    }
  }

  etl::fsm_state_id_t on_event(etl::imessage_router&, const SetSpeed& event) {
    ++get_fsm_context().setSpeedCount_;
    get_fsm_context().SetSpeedValue(event.speed);
    return STATE_ID;
  }

  etl::fsm_state_id_t on_event_unknown(etl::imessage_router&,
                                       const etl::imessage&) {
    ++get_fsm_context().unknownCount_;
    return STATE_ID;
  }

  etl::fsm_state_id_t on_enter_state() {
    get_fsm_context().TurnRunningLampOn();

    return STATE_ID;
  }
};

//***********************************
// The winding down state.
//***********************************
class WindingDown : public etl::fsm_state<MotorControl, WindingDown,
                                          StateId::WINDING_DOWN, Stopped> {
 public:

  etl::fsm_state_id_t on_event(etl::imessage_router&, const Stopped&) {
    ++get_fsm_context().stoppedCount_;
    return StateId::IDLE;
  }


  etl::fsm_state_id_t on_event_unknown(etl::imessage_router&,
                                       const etl::imessage&) {
    ++get_fsm_context().unknownCount_;
    return STATE_ID;
  }
};

//***********************************
// The locked state.
//***********************************
class Locked : public etl::fsm_state<MotorControl, Locked, StateId::LOCKED> {
 public:

  etl::fsm_state_id_t on_event_unknown(etl::imessage_router&,
                                       const etl::imessage&) {
    ++get_fsm_context().unknownCount_;
    return STATE_ID;
  }
};

#endif  // !MOTOR_CONTROLLER_HEADER