#include "coffeemaker.h"

#include <iostream>
using std::cout;
using std::endl;

namespace CoffeeMaker {

/*
 * This function returns the status of the warmer-plate
 * sensor. This sensor detects the presence of the pot
 * and whether it has coffee in it.
 */
WarmerPlateStatus CoffeeMakerAPI::GetWarmerPlateStatus() const {
  cout << "CoffeeMakerAPI::GetWarmerPlateStatus() called!" << endl;
  return WarmerPlateStatus::POT_EMPTY;
}
/*
 * This function returns the status of the boiler switch.
 * The boiler switch is a float switch that detects if
 * there is more than 1/2 cup of water in the boiler.
 */
BoilerStatus CoffeeMakerAPI::GetBoilerStatus() const {
  cout << "CoffeeMakerAPI::GetBoilerStatus() called!" << endl;
  return BoilerStatus::EMPTY;
}
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
BrewButtonStatus CoffeeMakerAPI::GetBrewButtonStatus() const {
  cout << "CoffeeMakerAPI::GetBrewButtonStatus() called!" << endl;
  return BrewButtonStatus::NOT_PUSHED;
}
/*
 * This function turns the heating element in the boiler
 * on or off.
 */
void CoffeeMakerAPI::SetBoilerState(BoilerState s) {
  cout << "CoffeeMakerAPI::SetBoilerState() called!" << endl;
}
/*
 * This function turns the heating element in the warmer
 * plate on or off.
 */
void CoffeeMakerAPI::SetWarmerState(WarmerState s) {
  cout << "CoffeeMakerAPI::SetWarmerState() called!" << endl;
}
/*
 * This function turns the indicator light on or off.
 * The indicator light should be turned on at the end
 * of the brewing cycle. It should be turned off when
 * the user presses the brew button.
 */
void CoffeeMakerAPI::SetIndicatorState(IndicatorState s) {
  cout << "CoffeeMakerAPI::SetIndicatorState() called!" << endl;
}
/*
 * This function opens and closes the pressure-relief
 * valve. When this valve is closed, steam pressure in
 * the boiler will force hot water to spray out over
 * the coffee filter. When the valve is open, the steam
 * in the boiler escapes into the environment, and the
 * water in the boiler will not spray out over the filter.
 */
void CoffeeMakerAPI::SetReliefValveState(ReliefValveState s) {
  cout << "CoffeeMakerAPI::SetReliefValveState() called!" << endl;
}

CoffeeMakerAPI api;

}  // namespace CoffeeMaker