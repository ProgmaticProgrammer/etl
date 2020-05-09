
#include "UserInterface.h"

#include <memory>

#include "ContainmentVessel.h"
#include "HotWaterSource.h"

namespace CoffeeMaker {

void UserInterface::StartBrewing() {
  auto sphws = hws_.lock();
  auto spcv = cv_.lock();
  if (sphws && sphws->IsReady() && spcv && spcv->IsReady()) {
    isComplete = false;
    sphws->Start();
    spcv->Start();
  }
}
}  // namespace CoffeeMaker
