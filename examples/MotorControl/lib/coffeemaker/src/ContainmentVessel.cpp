#include "ContainmentVessel.h"
namespace CoffeeMaker {

void ContainmentVessel::DeclareComplete() {
  isComplete_ = true;
  auto spui = ui_.lock();
  if (spui) spui->Complete();
}
void ContainmentVessel::ContainerAvailable() {
  auto sphws = hws_.lock();
  if (sphws) sphws->Resume();
}
void ContainmentVessel::ContainerUnavailable() {
  auto sphws = hws_.lock();
  if (sphws) sphws->Pause();
}

}  // namespace CoffeeMaker
