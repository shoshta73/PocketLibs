# LLVM-style RTTI

## Official tutorial

The official tutorial can be found [on this page](https://llvm.org/docs/HowToSetUpLLVMStyleRTTI.html).

## Starting point

Let's assume we have a class called `Shape` and two derived classes called
`Parallelogram` and `Ellipse`. The inheritance tree might look like this:

![inheritance-tree-starting-point.svg](../assets/guides/llvm-style-rtti/inheritance-tree-starting-point.svg)

So your shape class might look like this:

```cpp
class Shape {
  public:
    virtual ~Shape() = default;

    virtual double ComputeArea() const = 0;
};
```

With your parallelogram and ellipse classes looking like this:

```cpp
class Parallelogram : public Shape {
    using Base = Shape;

  public:
    Parallelogram(double a, double b, double alpha);
    ~Parallelogram() override = default;

    double ComputeArea() const override;
};

class Ellipse : public Shape {
    using Base = Shape;

  public:
    Ellipse(double a, double b);
    ~Ellipse() override = default;

    double ComputeArea() const override;
};
```

Now, you get a `Shape *` from somewhere and you want to perform some type
checking on it when compiling with `-fno-rtti`. Since `dynamic_cast` and `typeid`
won't work, the following code:

```cpp
if (dynamic_cast<Parallelogram *>(pShape)) {
    // Do something with the parallelogram
} else if (dynamic_cast<Ellipse *>(pShape)) {
    // Do something with the ellipse
}
```

will not work.

To work around this, you would need to implement something mimicking the `dynamic_cast`
behavior yourself. This is where LLVM-style RTTI comes in.

LLVM-style RTTI specific implementations RTTI which bases on type `disciminator`,
and `classof` method to perform runtime type checking.

## Type discriminator

First, you need to define a type discriminator for your classes, and since we
have a Shape class from which every other shape inherits, we will put our type
discriminator in the Shape class like this:

```cpp
class Shape {
  public:
    enum class ShapeKind{};
};
```

then we fill in the `ShapeKind` enum with the values we want to use for our
classes, so in our case we will have:

```cpp
enum class ShapeKind {
    SK_Parallelogram,
    SK_Ellipse,
};
```

We need to store a value for the discriminator in the `Shape` class. Therefore,
we will add a `ShapeKind` member to the `Shape` class:

```cpp
class Shape {
  private:
    const ShapeKind kind;
};
```

We will also need trivial accessors for the discriminator. To provide one, we will
implement the `GetKind` method as such:

```cpp
class Shape {
  public:
    auto GetKind() const -> ShapeKind { return kind; }
};
```

Now, since the C++ standard mandates that each constant member must be initialized
in the `Shape` class, we will add a constructor that accepts the discriminator
as a parameter:

```cpp
class Shape {
  public:
    constexpr Shape(ShapeKind kind) : kind(kind) {}
};
```

To facilitate the above changes, we will modify the `Parallelogram` and `Ellipse` constructors to provide `ShapeKind` to the `Shape` constructor:

```cpp

Parallelogram::Parallelogram(double a, double b, double alpha) : Shape(ShapeKind::SK_Parallelogram){
  /* ... */
}

Ellipse::Ellipse(double a, double b) : Shape(ShapeKind::SK_Ellipse){
  /* ... */
}

```

## Classof method

We need to implement the `classof` method, which will be used to perform runtime type checking in the `isa` function. To do so, we will add a public static method to each derived class as follows:

```cpp
class Parallelogram : public Shape {
  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Parallelogram; }
};

class Ellipse : public Shape {
  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Ellipse; }
};
```

The contract of this function should not exceed checking the discriminator value
alone.

## Adding types

### No new inheritance subtree

Let's assume you want to add the `Triangle` class to your hierarchy tree. First,
you would add the `ShapeKind` discriminator to facilitate the `Triangle` class:

```cpp
class Shape {
  public:
    enum class ShapeKind {
        SK_Parallelogram,
        SK_Ellipse,
        SK_Triangle,
    };
};
```

Then, you would add the `Triangle` class and its constructor:

```cpp
class Triangle : public Shape {
    using Base = Shape;

  public:
    Triangle(double a, double b, double c, double alpha, double beta);
    ~Triangle() override = default;

    auto ComputeArea() const -> double override;
};
```

Then, we will add the static `classof` method to the `Triangle` class:

```cpp
class Triangle : public Shape {
  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Triangle; }
};
```

Finally, your new inheritance tree would look like this:

![inheritance-tree-adding-types.svg](../assets/guides/llvm-style-rtti/inheritance-tree-adding-types-no-subtree.svg)

### New inheritance subtree

> [!IMPORTANT]
> Before reading this section, please refer to the [No new inheritance subtree](#no-new-inheritance-subtree) section.

Now that you have added the `Triangle` class, if you want to add three base
mathematical cases for triangles, which are `Equilateral`, `Isosceles`, and
`Scalene`, you would first modify the `ShapeKind` enum to include the new values:

```cpp
enum class ShapeKind {
    SK_Parallelogram,
    SK_Ellipse,
    SK_Triangle,
    SK_EquilateralTriangle,
    SK_IsoscelesTriangle,
    SK_ScaleneTriangle,
};
```

Then, you would add a protected constructor to the `Triangle` class:

```cpp
class Triangle : public Shape {
  protected:
    Triangle(ShapeKind kind, double a, double b, double c, double alpha, double beta);
};
```

Then, you implement the `EquilateralTriangle`, `IsoscelesTriangle`, and
`ScaleneTriangle` classes:

```cpp
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

    IsoscelesTriangle(double a, double b, double alpha );
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
```

To facilitate the above changes, we will modify the `classof` method in the
`Triangle` class as follows:

```cpp
class Triangle : public Shape {
  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() >= ShapeKind::SK_Triangle && shape->GetKind() <= ShapeKind::SK_ScaleneTriangle; }
};
```

Finally, your new inheritance tree would look like this:

![inheritance-tree-adding-types.svg](../assets/guides/llvm-style-rtti/inheritance-tree-adding-types-new-subtree.svg)

#### Exercise

> [!IMPORTANT]
> Before reading this section, please refer to the [New inheritance subtree](#new-inheritance-subtree)
> section and the [No new inheritance subtree](#no-new-inheritance-subtree) section.

As an exercise, we will be adding the `Rhombus`, `Rectangle`, and `Square`
classes to the inheritance tree. At the end of this exercise, you should have
the following inheritance tree:

![inheritance-tree-adding-types.svg](../assets/guides/llvm-style-rtti/inheritance-tree-adding-types-new-subtree-exercise.svg)

> [!NOTE]

First, we will add the `Rhombus`, then the `Rectangle`, and finally the `Square` class.

##### Rhombus

First, we will modify the `ShapeKind` enum to include the new value:

```cpp
enum class ShapeKind {
    SK_Parallelogram,
    SK_Rhombus,
    SK_Ellipse,
    SK_Triangle,
    SK_EquilateralTriangle,
    SK_IsoscelesTriangle,
    SK_ScaleneTriangle,
};
```

Then, we will add the `Rhombus` class and its constructor:

```cpp
class Rhombus : public Shape {
    using Base = Shape;

  public:
    Rhombus(double a, double b, double alpha);
    ~Rhombus() override = default;

    auto ComputeArea() const -> double override;
};
```

To facilitate the above changes, we will add a protected constructor to the
`Parallelogram` class as follows:

```cpp
class Parallelogram : public Shape {
  protected:
    Parallelogram(ShapeKind kind, double a, double b, double alpha);
};
```

Then, we will modify the `classof` method in the `Parallelogram` class as follows:

```cpp
class Parallelogram : public Shape {
  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() >= ShapeKind::SK_Parallelogram && shape->GetKind() <= ShapeKind::SK_Rhombus; }
};
```

Finally, we will add the `Rhombus` class and its constructor:

```cpp
class Rhombus : public Parallelogram {
    using Base = Parallelogram;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Rhombus; }

    Rhombus(double a, double b, double alpha);
    ~Rhombus() override = default;

    auto ComputeArea() const -> double override;
};
```

##### Rectangle

First, we will modify the `ShapeKind` enum to include the new value:

```cpp
enum class ShapeKind {
    SK_Parallelogram,
    SK_Rhombus,
    SK_Rectangle,
    SK_Ellipse,
    SK_Triangle,
    SK_EquilateralTriangle,
    SK_IsoscelesTriangle,
    SK_ScaleneTriangle,
};
```

Since we have modified the `Parallelogram` constructor in the [rhombus](#rhombus)
section, we will only need to modify the `classof` method in the `Parallelogram`
class as follows:

```cpp
class Parallelogram : public Shape {
  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() >= ShapeKind::SK_Parallelogram && shape->GetKind() <= ShapeKind::SK_Rectangle; }
};
```

Then, we will add the `Rectangle` class and its constructor:

```cpp
class Rectangle : public Parallelogram {
    using Base = Parallelogram;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Rectangle; }

    Rectangle(double a, double b);
    ~Rectangle() override = default;

    auto ComputeArea() const -> double override;
};
```

##### Square

First, we will modify the `ShapeKind` enum to include the new value:

```cpp
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
```

First, we will add a protected constructor to the `Rectangle` class:

```cpp
class Rectangle : public Parallelogram {
  protected:
    Rectangle(ShapeKind kind, double a, double b);
};
```

Then, we will modify the `classof` method in the `Rectangle` class as follows:

```cpp
class Rectangle : public Parallelogram {
  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() >= ShapeKind::SK_Rectangle && shape->GetKind() <= ShapeKind::SK_Square; }
};
```

Then, we will add the `Square` class and its constructor:

```cpp
class Square : public Rectangle {
    using Base = Rectangle;

  public:
    static auto classof(const Shape *shape) -> bool { return shape->GetKind() == ShapeKind::SK_Square; }

    Square(double a);
    ~Square() override = default;

    auto ComputeArea() const -> double override;
};
```

## Using `classof` method

The `classof` method is a static method that returns `true` if the given value is
of the specified type, and `false` otherwise. It is not meant to be ussed directly
by the user, but rather calling it should be done indirectly in the `isa` function.

So the users of your library do not have to worry about the including `casting.hxx`
and worry about implementing type checking themselves you should include `casting.hxx`
in your header where you have defined the `Shape` class like so:

```cpp
#ifndef SHAPE_H
#define SHAPE_H

#include "casting.hxx"

/* ... */

#endif  // SHAPE_H
```

Internal api of `isa` function boils down to something like this:

```cpp
auto isa<Rectangle>(Shape *shape) {return true;} // if value is of Rectangle type
auto isa<Rectangle>(Shape *shape) {return false;} // if value is not of Rectangle type
```

It does that by first checking if shape is base any of base classes of
`Rectangle` class, and if it is, it returns `true`. If it is not, it checks if
if Rectangle has a `classof` method, and if it does, it calls it with the shape
as a parameter, and if checks done by `classof` method returns pass it returns
`true`, otherwise it returns `false`.

## Using `isa` function

Say you have function called `GetPoints` that returns a number of points based on
the shape of the given shape.

When compiling with `rtti` enabled, the following code will work:

```cpp
if (auto triangle = dynamic_cast<Triangle *>(shape)) {
  return 3;
} else if (auto rectangle = dynamic_cast<Rectangle *>(shape)) {
  return 4;
}
```

However, when compiling with `rtti` disabled, the following code will not work:

```cpp
if (auto triangle = dynamic_cast<Triangle *>(shape)) {
  return 3;
} else if (auto parallelogram = dynamic_cast<Parallelogram *>(shape)) {
  return 4;
}
```

so you would use the `isa` function instead:

```cpp
if (isa<Triangle>(shape)) {
  return 3;
} else if (isa<Parallelogram>(shape)) {
  return 4;
}
```
