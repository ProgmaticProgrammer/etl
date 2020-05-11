
#include "UserInterface.h"
// #include <iostream>
#include <memory>

#include "ContainmentVessel.h"
#include "HotWaterSource.h"

namespace CoffeeMaker {

void UserInterface::StartBrewing() {
  auto sphws = hws_.lock();
  auto spcv = cv_.lock();
  
  if (sphws && sphws->IsReady() && spcv && spcv->IsReady()) {
    isComplete_ = false;
    sphws->Start();
    spcv->Start();
    // std::cout << "ready ready go" << std::endl;
  }
}
}  // namespace CoffeeMaker