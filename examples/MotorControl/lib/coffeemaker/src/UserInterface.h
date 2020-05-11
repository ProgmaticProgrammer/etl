#ifndef USER_INTERFACE_HEADER
#define USER_INTERFACE_HEADER

#include <memory>

using std::shared_ptr;
using std::weak_ptr;

namespace CoffeeMaker {

class HotWaterSource;
class ContainmentVessel;

class UserInterface {
 private:
  weak_ptr<HotWaterSource> hws_;
  weak_ptr<ContainmentVessel> cv_;

 protected:
  bool isComplete_ = true;
  virtual void CompleteCycle() = 0;
  void StartBrewing();

 public:
  UserInterface() : isComplete_(true) {}

  void Init(const shared_ptr<HotWaterSource>& hws,
            const shared_ptr<ContainmentVessel>& cv) {
    hws_ = hws;
    cv_ = cv;
  }
  // done is when hotwatersource runs out of water
  virtual void Done() = 0;
  // complete is when all coffee in containmentvessel consumed
  void Complete() {
    isComplete_ = true;
    CompleteCycle();
  }
};
}  // namespace CoffeeMaker

#endif  // !USER_INTERFACE_HEADER