#include "HotWaterSource.h"

#include "ContainmentVessel.h"
namespace CoffeeMaker {

void HotWaterSource::DeclareDone() {
  auto spui = ui_.lock();
  if (spui) spui->Done();
  auto spcv = cv_.lock();
  if (spcv) spcv->Done();
  isBrewing_ = false;
}
}  // namespace CoffeeMaker
