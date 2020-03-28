#include "windows.h"
#include "rect.hh"
#include <iostream>
#include <exception>


DiscretePoint::DiscretePoint(long x, long y): x(x), y(y) {}

DiscreteRect::DiscreteRect(
  long left,
  long right,
  long top, 
  long bottom
): left(left), right(right), top(top), bottom(bottom) {}

long DiscreteRect::width() const {
  return this->right - this->left;
}

long DiscreteRect::height() const {
  return this->bottom - this->top;
}

bool DiscreteRect::containsPoint(long x, long y) const {

  return (
    x >= this->left && x < this->right 
    && y >= this->top && y < this->bottom
  );
}

bool DiscreteRect::containsPoint(DiscretePoint &point) const {
  /* Tests if a point lies within the rect */

  return this->containsPoint(point.x, point.y);
}

DiscreteRect DiscreteRect::cornerRect(RectCorner corner, float xFraction, float yFraction) const {

  if (xFraction < 0 || yFraction < 0) {
    throw std::runtime_error("Fraction was negative");
  }

  long left = this->left;
  long right = this->right;
  long top = this->top;
  long bottom = this->bottom;

  long width = this->width();
  long height = this->height();

  switch (corner) {

    case TOP_LEFT:
      right = left + long(width * xFraction);
      bottom = top + long(height * yFraction);
      break;

    case TOP_RIGHT:
      left = right - long(width * xFraction);
      bottom = top + long(height * yFraction);
      break;

    case BOTTOM_LEFT:
      right = left + long(width * xFraction);
      top = bottom - long(height * yFraction);
      break;

    case BOTTOM_RIGHT:
      left = right - long(width * xFraction);
      top = bottom - long(height * yFraction);
      break;
  }

  return DiscreteRect(left, right, top, bottom);
}

DiscreteRect DiscreteRect::quadrant(RectCorner corner) const {

  return this->cornerRect(corner, 0.5, 0.5);
}

DiscreteRect DiscreteRect::cornerSquareTopLeft(float fraction) const {

  return this->cornerRect(TOP_LEFT, fraction, fraction);
};

DiscreteRect DiscreteRect::cornerSquareTopRight(float fraction) const {

  return this->cornerRect(TOP_RIGHT, fraction, fraction);
};

DiscreteRect DiscreteRect::cornerSquareBottomLeft(float fraction) const {

  return this->cornerRect(BOTTOM_LEFT, fraction, fraction);
};

DiscreteRect DiscreteRect::cornerSquareBottomRight(float fraction) const {

  return this->cornerRect(BOTTOM_RIGHT, fraction, fraction);
};

DiscreteRect DiscreteRect::quadrantTopLeft() const {

  return this->quadrant(TOP_LEFT);
}

DiscreteRect DiscreteRect::quadrantTopRight() const {

  return this->quadrant(TOP_RIGHT);
}

DiscreteRect DiscreteRect::quadrantBottomLeft() const {

  return this->quadrant(BOTTOM_LEFT);
}

DiscreteRect DiscreteRect::quadrantBottomRight() const {

  return this->quadrant(BOTTOM_RIGHT);
}

DiscreteRect DiscreteRect::edgeRect(RectEdge edge, float fraction) const {

  if (fraction < 0) {
    throw std::runtime_error("Fraction was negative");
  }

  long left = this->left;
  long right = this->right;
  long top = this->top;
  long bottom = this->bottom;

  long width = this->width();
  long height = this->height();

  switch (edge) {

    case LEFT:
      right = left + long(width * fraction);
      break;

    case RIGHT:
      left = right - long(width * fraction);
      break;

    case BOTTOM:
      top = bottom - long(height * fraction);
      break;

    case TOP:
      bottom = top + long(height * fraction);
      break;
  }

  return DiscreteRect(left, right, top, bottom);
}

DiscreteRect DiscreteRect::edgeRectLeft(float fraction) const {
  return this->edgeRect(LEFT, fraction);
};
DiscreteRect DiscreteRect::edgeRectRight(float fraction) const {
  return this->edgeRect(RIGHT, fraction);
};
DiscreteRect DiscreteRect::edgeRectTop(float fraction) const {
  return this->edgeRect(TOP, fraction);
};
DiscreteRect DiscreteRect::edgeRectBottom(float fraction) const {
  return this->edgeRect(BOTTOM, fraction);
};

DiscreteRect DiscreteRect::half(RectEdge edge) const {
  return this->edgeRect(edge, 0.5);
}

DiscreteRect DiscreteRect::halfLeft() const {
  return this->half(LEFT);
}

DiscreteRect DiscreteRect::halfRight() const {
  return this->half(RIGHT);
}

DiscreteRect DiscreteRect::halfTop() const {
  return this->half(TOP);
}

DiscreteRect DiscreteRect::halfBottom() const {
  return this->half(BOTTOM);
}

ScreenArea::ScreenArea(
  long left,
  long right,
  long top, 
  long bottom
): DiscreteRect(left, right, top, bottom) {}
