class BaseComponent;

// Интерфейс Посредника
// Предоставляет метод, используемый компонентами для уведомления посредника о различных событиях.
class Mediator {
 public:
  virtual void widgetChanged(Widget *sender, std::string event) const = 0;
};

// Базовый Компонент обеспечивает базовую функциональность хранения экземпляра
//  посредника внутри объектов компонентов.

class Widget {
 protected:
  Mediator *mediator_;
 
 public:
  Widget(Mediator *mediator = nullptr) : mediator_(mediator) {}
  virtual void changed(const std::string &mes) {mediator_->widgetChanged(this, mes)}
};

class ListBox : public Widget {
  void setList(std::vector<std::string> &list)
  {
  	changed("set_new_list")
  }
}
