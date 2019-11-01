#include <iostream>
#include <vector>

#include "shape.h"


int main()
{
  std::vector<Shape *> shapes;

  Creator creator;

  Shape *rect = creator.createShape("rect", 10, 15);
  shapes.push_back(rect);

  Shape *circle = creator.createShape("circle", 10, 15);
  shapes.push_back(circle);

  for (auto shape : shapes)
  {
    std::cout << shape->getName() << std::endl;
  }

  for (auto shape : shapes)
  {
    delete shape;
  }

  return 0;
}