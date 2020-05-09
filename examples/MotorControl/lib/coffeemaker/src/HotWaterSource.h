#ifndef HOT_WATER_SOURCE_HEADER
#define HOT_WATER_SOURCE_HEADER
#include <memory>

using std::shared_ptr;
using std::weak_ptr;

namespace CoffeeMaker {

class UserInterface;
class ContainmentVessel;

class HotWaterSource {
 private:
  weak_ptr<UserInterface> ui_;
  weak_ptr<ContainmentVessel> cv_;

 protected:
  bool isBrewing_ = false;

 public:
  void Init(const shared_ptr<UserInterface>& ui,
            const shared_ptr<ContainmentVessel>& cv) {
    ui_ = ui;
    cv_ = cv;
  }

 public:
  virtual bool IsReady() const = 0;
  virtual void StartBrewing() = 0;
  virtual void Pause() = 0;
  virtual void Resume() = 0;

  virtual void Start() {
    isBrewing_ = true;
    StartBrewing();
  }
  void Done() { isBrewing_ = false; }
  void DeclareDone();
};

}  // namespace CoffeeMaker
#endif  // !HOT_WATER_SOURCE_HEADER
