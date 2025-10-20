#include "casting.h"

#include <print>
#include <vector>

namespace {

class Shape {
  public:
    enum class ShapeKind {
        SK_Circle,
        SK_Rectangle,
        SK_Triangle,
    };

    auto GetKind() const -> ShapeKind { return kind; }

  private:
    const ShapeKind kind;

  protected:
    constexpr Shape(ShapeKind kind) : kind(kind) {}
};

class Circle : public Shape {
    using Base = Shape;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Circle; }

    Circle() : Base(ShapeKind::SK_Circle) {}
};

class Rectangle : public Shape {
    using Base = Shape;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Rectangle; }

    Rectangle() : Base(ShapeKind::SK_Rectangle) {}
};

class Triangle : public Shape {
    using Base = Shape;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Triangle; }

    Triangle() : Base(ShapeKind::SK_Triangle) {}
};

auto isa_checks(Shape *shape) -> void {
    if (isa<Circle>(shape)) {
        std::println("isa<Circle>(shape) is true");
    } else {
        std::println("isa<Circle>(shape) is false");
    }

    if (isa<Rectangle>(shape)) {
        std::println("isa<Rectangle>(shape) is true");
    } else {
        std::println("isa<Rectangle>(shape) is false");
    }

    if (isa<Triangle>(shape)) {
        std::println("isa<Triangle>(shape) is true");
    } else {
        std::println("isa<Triangle>(shape) is false");
    }
}

}  // namespace

auto main() -> int {
    std::vector<Shape *> shapes;
    shapes.emplace_back(new Circle());
    shapes.emplace_back(new Rectangle());
    shapes.emplace_back(new Triangle());

    for (auto shape : shapes) {
        isa_checks(shape);
    }
}
