#include "windows.h"
#include <vector>
#include <unordered_map>


class DiscretePoint {

  public:

    DiscretePoint(long x, long y);

    long x;
    long y;
};


enum RectCorner {
  TOP_LEFT,
  TOP_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT,
};


enum RectEdge {
  LEFT,
  RIGHT,
  TOP,
  BOTTOM,
};


class DiscreteRect {
  /* Lower bounds are inclusive, upper bounds are exclusive */

  public:

    long left;
    long right;
    long top;
    long bottom;

    DiscreteRect(
      long left,
      long right,
      long top, 
      long bottom
    );

    bool containsPoint(long x, long y) const;
    bool containsPoint(DiscretePoint &point) const;

    long width() const;
    long height() const;

    DiscreteRect cornerRect(RectCorner corner, float xFraction, float yFraction) const;
    DiscreteRect cornerSquareTopLeft(float fraction) const;
    DiscreteRect cornerSquareTopRight(float fraction) const;
    DiscreteRect cornerSquareBottomLeft(float fraction) const;
    DiscreteRect cornerSquareBottomRight(float fraction) const;

    DiscreteRect quadrant(RectCorner corner) const;
    DiscreteRect quadrantTopLeft() const;
    DiscreteRect quadrantTopRight() const;
    DiscreteRect quadrantBottomLeft() const;
    DiscreteRect quadrantBottomRight() const;

    DiscreteRect edgeRect(RectEdge edge, float fraction) const;
    DiscreteRect edgeRectLeft(float fraction) const;
    DiscreteRect edgeRectRight(float fraction) const;
    DiscreteRect edgeRectTop(float fraction) const;
    DiscreteRect edgeRectBottom(float fraction) const;

    DiscreteRect half(RectEdge edge) const;
    DiscreteRect halfLeft() const;
    DiscreteRect halfRight() const;
    DiscreteRect halfTop() const;
    DiscreteRect halfBottom() const;
};


class ScreenArea: public DiscreteRect {

  public:

    ScreenArea(
      long left,
      long right,
      long top, 
      long bottom
    );
};
