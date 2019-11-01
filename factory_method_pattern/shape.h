#pragma once

#include <string>
#include <iostream>

#include <math.h>

class Shape 
{
public:
  virtual float area() = 0;
  virtual ~Shape() {};

public:
  virtual std::string getName() const
  {
    return "shape";
  }
};


class Rectangle : public Shape 
{
public:
  Rectangle(int width, int height) : m_Width(width), m_Height(height) {}

public:
  virtual float area() { return m_Width * m_Height; };
  virtual ~Rectangle() {};

public:
  virtual std::string getName() const
  {
    return "rect";
  }

protected:
  int m_Width = 0;
  int m_Height = 0;
};


class Circle : public Shape 
{
public:
  Circle(int r) : m_R(r) {}

public:
  virtual float area() { return M_PI * m_R * m_R; };
  virtual ~Circle() {};

public:
  virtual std::string getName() const
  {
    return "circle";
  }

protected:
  int m_R = 0;
};


struct Creator
{
  virtual Shape* createShape(const std::string &type, int a, int b)
  {
    if (type == "rect")
      return new Rectangle(a, b);
    else if(type == "circle")
      return new Circle(a);

    return nullptr;
  }
};
