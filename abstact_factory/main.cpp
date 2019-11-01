#include "factory.hpp"

int main() {


  GUIFactory* factory = new MacGUIFactory();

  Button* button = factory->createButton();
  button->paint();
  CheckBox* checkbox = factory->createCheckBox();
  checkbox->paint();

  delete factory;
  delete button;
  delete checkbox;

  return 0;
}