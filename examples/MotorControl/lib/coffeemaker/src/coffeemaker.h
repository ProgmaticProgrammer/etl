#ifndef COFFEE_MAKER_HEADER
#define COFFEE_MAKER_HEADER

namespace CoffeeMaker {

enum class WarmerPlateStatus { WARMER_EMPTY, POT_EMPTY, POT_NOT_EMPTY };

enum class BoilerStatus { EMPTY, NOT_EMPTY };

enum class BrewButtonStatus { PUSHED, NOT_PUSHED };

enum class BoilerState { ON, OFF };

enum class WarmerState { ON, OFF };

enum class IndicatorState { ON, OFF };

enum class ReliefValveState { OPEN, CLOSED };

class CoffeeMakerAPI {
 public:
  /*
   * This function returns the status of the warmer-plate
   * sensor. This sensor detects the presence of the pot
   * and whether it has coffee in it.
   */
  WarmerPlateStatus GetWarmerPlateStatus();
  /*
   * This function returns the status of the boiler switch.
   * The boiler switch is a float switch that detects if
   * there is more than 1/2 cup of water in the boiler.
   */
  BoilerStatus GetBoilerStatus();
  /*
   * This function returns the status of the brew button.
   * The brew button is a momentary switch that remembers
   * its state. Each call to this function returns the
   * remembered state and then resets that state to
   * NOT_PUSHED.
   *
   * Thus, even if this function is polled at a very slow
   * rate, it will still detect when the brew button is
   * pushed.
   */
  BrewButtonStatus GetBrewButtonStatus();
  /*
   * This function turns the heating element in the boiler
   * on or off.
   */
  void SetBoilerState(BoilerState s);
  /*
   * This function turns the heating element in the warmer
   * plate on or off.
   */
  void SetWarmerState(WarmerState s);
  /*
   * This function turns the indicator light on or off.
   * The indicator light should be turned on at the end
   * of the brewing cycle. It should be turned off when
   * the user presses the brew button.
   */
  void SetIndicatorState(IndicatorState s);
  /*
   * This function opens and closes the pressure-relief
   * valve. When this valve is closed, steam pressure in
   * the boiler will force hot water to spray out over
   * the coffee filter. When the valve is open, the steam
   * in the boiler escapes into the environment, and the
   * water in the boiler will not spray out over the filter.
   */
  void SetReliefValveState(ReliefValveState s);
};
extern CoffeeMakerAPI api;

}  // namespace CoffeeMaker
#endif  // !COFFEE_MAKER_HEADER
