#ifndef CONTAINMENT_VESSEL_HEADER
#define CONTAINMENT_VESSEL_HEADER
#include <memory>

#include "HotWaterSource.h"
#include "UserInterface.h"
using std::shared_ptr;
using std::weak_ptr;

namespace CoffeeMaker {

class ContainmentVessel {
 private:
  weak_ptr<UserInterface> ui_;
  weak_ptr<HotWaterSource> hws_;

 protected:
  bool isBrewing_ = false;
  bool isComplete_ = true;

 public:
  void Init(const shared_ptr<UserInterface>& ui,
            const shared_ptr<HotWaterSource>& hws) {
    ui_ = ui;
    hws_ = hws;
  }
  virtual bool IsReady() const = 0;
  virtual void Start() {
    isBrewing_ = true;
    isComplete_ = false;
  }
  void Done() { isBrewing_ = false; }

 protected:
  void DeclareComplete();
  void ContainerAvailable();
  void ContainerUnavailable();
};

}  // namespace CoffeeMaker
#endif  // !CONTAINMENT_VESSEL_HEADER#define