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
  bool isComplete = true;

 public:
  UserInterface() : isComplete(true) {}

  void Init(const shared_ptr<HotWaterSource>& hws,
            const shared_ptr<ContainmentVessel>& cv) {
    hws_ = hws;
    cv_ = cv;
  }

 public:
  virtual void Done() = 0;
  virtual void CompleteCycle() = 0;

 public:
  void Complete() {
    isComplete = true;
    CompleteCycle();
  }

  void StartBrewing();
};
}  // namespace CoffeeMaker

#endif  // !USER_INTERFACE_HEADER