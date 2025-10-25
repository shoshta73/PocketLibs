#include "casting.hxx"

#include <print>

class Shape {
  public:
    enum class ShapeKind {
        SK_Parallelogram,
        SK_Rhombus,
        SK_Rectangle,
        SK_Square,
        SK_Ellipse,
        SK_Triangle,
        SK_EquilateralTriangle,
        SK_IsoscelesTriangle,
        SK_ScaleneTriangle,
    };

    constexpr Shape(ShapeKind kind) : kind(kind) {}
    virtual ~Shape() = default;

    auto GetKind() const -> ShapeKind { return kind; }
    virtual auto ComputeArea() const -> double = 0;

  private:
    const ShapeKind kind;
};

class Parallelogram : public Shape {
  protected:
    Parallelogram(ShapeKind kind, double a, double b, double alpha);

  public:
    static auto classof(const Shape *shape) -> bool {
        return shape->GetKind() >= ShapeKind::SK_Parallelogram && shape->GetKind() <= ShapeKind::SK_Square;
    }

    Parallelogram(double a, double b, double alpha);
    ~Parallelogram() override = default;

    auto ComputeArea() const -> double override;
};

class Rhombus : public Parallelogram {
    using Base = Parallelogram;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Rhombus; }

    Rhombus(double a, double b, double alpha);
    ~Rhombus() override = default;

    auto ComputeArea() const -> double override;
};

class Rectangle : public Parallelogram {
  protected:
    Rectangle(ShapeKind kind, double a, double b);

  public:
    static auto classof(const Shape *shape) -> bool {
        return shape->GetKind() >= ShapeKind::SK_Rectangle && shape->GetKind() <= ShapeKind::SK_Square;
    }

    Rectangle(double a, double b);
    ~Rectangle() override = default;

    auto ComputeArea() const -> double override;
};

class Square : public Rectangle {
    using Base = Rectangle;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Square; }

    Square(double a);
    ~Square() override = default;

    auto ComputeArea() const -> double override;
};

class Ellipse : public Shape {
    using Base = Shape;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Ellipse; }

    Ellipse(double a, double b);
    ~Ellipse() override = default;

    auto ComputeArea() const -> double override;
};

class Triangle : public Shape {
  protected:
    Triangle(ShapeKind kind, double a, double b, double c, double alpha, double beta);

  public:
    static auto classof(const Shape *shape) -> bool {
        return shape->GetKind() >= ShapeKind::SK_Triangle && shape->GetKind() <= ShapeKind::SK_ScaleneTriangle;
    }

    Triangle(double a, double b, double c, double alpha, double beta);
    ~Triangle() override = default;

    auto ComputeArea() const -> double override;
};

class EquilateralTriangle : public Triangle {
    using Base = Triangle;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_EquilateralTriangle; }

    EquilateralTriangle(double a);
    ~EquilateralTriangle() override = default;

    auto ComputeArea() const -> double override;
};

class IsoscelesTriangle : public Triangle {
    using Base = Triangle;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_IsoscelesTriangle; }

    IsoscelesTriangle(double a, double b, double alpha);
    ~IsoscelesTriangle() override = default;

    auto ComputeArea() const -> double override;
};

class ScaleneTriangle : public Triangle {
    using Base = Triangle;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_ScaleneTriangle; }

    ScaleneTriangle(double a, double b, double c, double alpha, double beta);
    ~ScaleneTriangle() override = default;

    auto ComputeArea() const -> double override;
};

Parallelogram::Parallelogram(double a, double b, double alpha) : Shape(ShapeKind::SK_Parallelogram) {
    // Implementation of the constructor
}

Parallelogram::Parallelogram(ShapeKind kind, double a, double b, double alpha) : Shape(kind) {
    // Implementation of the constructor
}

auto Parallelogram::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

Rhombus::Rhombus(double a, double b, double alpha) : Parallelogram(ShapeKind::SK_Rhombus, a, b, alpha) {
    // Implementation of the constructor
}

auto Rhombus::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

Rectangle::Rectangle(ShapeKind kind, double a, double b) : Parallelogram(kind, a, b, 90.0) {
    // Implementation of the constructor
}

Rectangle::Rectangle(double a, double b) : Parallelogram(ShapeKind::SK_Rectangle, a, b, 90.0) {
    // Implementation of the constructor
}

auto Rectangle::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

Square::Square(double a) : Rectangle(ShapeKind::SK_Square, a, a) {
    // Implementation of the constructor
}

auto Square::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

Ellipse::Ellipse(double a, double b) : Shape(ShapeKind::SK_Ellipse) {
    // Implementation of the constructor
}

auto Ellipse::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

Triangle::Triangle(ShapeKind kind, double a, double b, double c, double alpha, double beta) : Shape(kind) {
    // Implementation of the constructor
}

Triangle::Triangle(double a, double b, double c, double alpha, double beta) : Shape(ShapeKind::SK_Triangle) {
    // Implementation of the constructor
}

auto Triangle::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

EquilateralTriangle::EquilateralTriangle(double a) : Triangle(ShapeKind::SK_EquilateralTriangle, a, a, a, 60.0, 60.0) {
    // Implementation of the constructor
}

auto EquilateralTriangle::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

IsoscelesTriangle::IsoscelesTriangle(double a, double b, double alpha)
    : Triangle(ShapeKind::SK_IsoscelesTriangle, a, b, a, alpha, alpha) {
    // Implementation of the constructor
}

auto IsoscelesTriangle::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

ScaleneTriangle::ScaleneTriangle(double a, double b, double c, double alpha, double beta)
    : Triangle(ShapeKind::SK_ScaleneTriangle, a, b, c, alpha, beta) {
    // Implementation of the constructor
}

auto ScaleneTriangle::ComputeArea() const -> double {
    // Implementation of the ComputeArea method
    return 0.0;
}

auto GetPoints(const Shape *shape) -> int {
    if (isa<Triangle>(shape)) {
        return 3;
    } else if (isa<Parallelogram>(shape)) {
        return 4;
    } else if (isa<Ellipse>(shape)) {
        return 0;  // Ellipses don't have a fixed number of points
    }
    return 0;
}

auto main() -> int {
    // Create a unique pointer to a Rectangle
    std::unique_ptr<Shape> shape = std::make_unique<Rectangle>(5.0, 10.0);
    std::println("Rectangle:");
    std::println("Number of points: {}", GetPoints(shape.get()));
    std::println("Area: {}", shape->ComputeArea());

    // Cast the shape to a Rectangle and print its area
    if (auto rectangle = dyn_cast<Rectangle>(shape.get())) {
        std::println("Rectangle area (using dyn_cast): {}", rectangle->ComputeArea());
    }

    // Create a unique pointer to an EquilateralTriangle
    shape = std::make_unique<EquilateralTriangle>(5.0);
    std::println("\nEquilateralTriangle:");
    std::println("Number of points: {}", GetPoints(shape.get()));
    std::println("Area: {}", shape->ComputeArea());

    // Cast the shape to an EquilateralTriangle and print its area
    if (auto equilateralTriangle = dyn_cast<EquilateralTriangle>(shape.get())) {
        std::println("EquilateralTriangle area (using dyn_cast): {}", equilateralTriangle->ComputeArea());
    }

    // Create a unique pointer to an Ellipse
    shape = std::make_unique<Ellipse>(5.0, 10.0);
    std::println("\nEllipse:");
    std::println("Number of points: {}", GetPoints(shape.get()));
    std::println("Area: {}", shape->ComputeArea());

    // Cast the shape to an Ellipse and print its area
    if (auto ellipse = dyn_cast<Ellipse>(shape.get())) {
        std::println("Ellipse area (using dyn_cast): {}", ellipse->ComputeArea());
    }

    // Create a unique pointer to a Rhombus
    shape = std::make_unique<Rhombus>(5.0, 10.0, 30.0);
    std::println("\nRhombus:");
    std::println("Number of points: {}", GetPoints(shape.get()));
    std::println("Area: {}", shape->ComputeArea());

    // Cast the shape to a Rhombus and print its area
    if (auto rhombus = dyn_cast<Rhombus>(shape.get())) {
        std::println("Rhombus area (using dyn_cast): {}", rhombus->ComputeArea());
    }

    // Create a unique pointer to a Square
    shape = std::make_unique<Square>(5.0);
    std::println("\nSquare:");
    std::println("Number of points: {}", GetPoints(shape.get()));
    std::println("Area: {}", shape->ComputeArea());

    // Cast the shape to a Square and print its area
    if (auto square = dyn_cast<Square>(shape.get())) {
        std::println("Square area (using dyn_cast): {}", square->ComputeArea());
    }

    // Create a unique pointer to an IsoscelesTriangle
    shape = std::make_unique<IsoscelesTriangle>(5.0, 10.0, 30.0);
    std::println("\nIsoscelesTriangle:");
    std::println("Number of points: {}", GetPoints(shape.get()));
    std::println("Area: {}", shape->ComputeArea());

    // Cast the shape to an IsoscelesTriangle and print its area
    if (auto isoscelesTriangle = dyn_cast<IsoscelesTriangle>(shape.get())) {
        std::println("IsoscelesTriangle area (using dyn_cast): {}", isoscelesTriangle->ComputeArea());
    }

    // Create a unique pointer to a ScaleneTriangle
    shape = std::make_unique<ScaleneTriangle>(5.0, 10.0, 15.0, 30.0, 45.0);
    std::println("\nScaleneTriangle:");
    std::println("Number of points: {}", GetPoints(shape.get()));
    std::println("Area: {}", shape->ComputeArea());

    // Cast the shape to a ScaleneTriangle and print its area
    if (auto scaleneTriangle = dyn_cast<ScaleneTriangle>(shape.get())) {
        std::println("ScaleneTriangle area (using dyn_cast): {}", scaleneTriangle->ComputeArea());
    }

    return 0;
}
