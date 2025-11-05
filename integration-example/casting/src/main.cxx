#include "casting.hxx"

#include <format>
#include <memory>
#include <print>
#include <string>
#include <vector>

// Abstract Syntax Tree for simple mathematical expressions
// Demonstrates LLVM-style RTTI setup for use with casting.hxx

class Expr {
  public:
    enum class ExprKind {
        EK_Literal,
        EK_BinaryOp,
    };

    constexpr Expr(ExprKind kind) : kind(kind) {}
    virtual ~Expr() = default;

    auto GetKind() const -> ExprKind { return kind; }

    // Pure virtual - each expression can be evaluated
    virtual auto Evaluate() const -> double = 0;
    virtual auto ToString() const -> std::string = 0;

  private:
    const ExprKind kind;
};

// Represents a binary operation like +, -, *, /
class BinaryOp : public Expr {
  public:
    enum class OpKind { Add, Subtract, Multiply, Divide };

    BinaryOp(OpKind op, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right)
        : Expr(ExprKind::EK_BinaryOp), op(op), left(std::move(left)), right(std::move(right)) {}

    ~BinaryOp() override = default;

    // LLVM-style RTTI requirement: classof method
    static auto classof(const Expr *expr) -> bool { return expr->GetKind() == ExprKind::EK_BinaryOp; }

    auto GetOp() const -> OpKind { return op; }
    auto GetLeft() const -> const Expr * { return left.get(); }
    auto GetRight() const -> const Expr * { return right.get(); }

    auto GetOpString() const -> std::string {
        switch (op) {
            case OpKind::Add:
                return "+";
            case OpKind::Subtract:
                return "-";
            case OpKind::Multiply:
                return "*";
            case OpKind::Divide:
                return "/";
        }
        return "?";
    }

    auto Evaluate() const -> double override {
        double lhs = left->Evaluate();
        double rhs = right->Evaluate();

        switch (op) {
            case OpKind::Add:
                return lhs + rhs;
            case OpKind::Subtract:
                return lhs - rhs;
            case OpKind::Multiply:
                return lhs * rhs;
            case OpKind::Divide:
                return lhs / rhs;
        }
        return 0.0;
    }

    auto ToString() const -> std::string override {
        return std::format("({} {} {})", left->ToString(), GetOpString(), right->ToString());
    }

  private:
    OpKind op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

// Represents a literal number like 42 or 3.14
class Literal : public Expr {
  public:
    explicit Literal(double value) : Expr(ExprKind::EK_Literal), value(value) {}
    ~Literal() override = default;

    // LLVM-style RTTI requirement: classof method
    static auto classof(const Expr *expr) -> bool { return expr->GetKind() == ExprKind::EK_Literal; }

    auto GetValue() const -> double { return value; }

    auto Evaluate() const -> double override { return value; }

    auto ToString() const -> std::string override { return std::format("{}", value); }

  private:
    double value;
};

// Helper function to convert ExprKind to string
auto ExprKindToString(Expr::ExprKind kind) -> std::string {
    switch (kind) {
        case Expr::ExprKind::EK_Literal:
            return "Literal";
        case Expr::ExprKind::EK_BinaryOp:
            return "BinaryOp";
    }
    return "Unknown";
}

// Demonstrates using isa<>, cast<>, and dyn_cast<> for tree traversal
// Uses ExprEval namespace (defined by CASTING_NAMESPACE macro)
using namespace ExprEval;
void PrintTreeStructure(const Expr *expr, int depth = 0) {
    std::string indent(depth * 2, ' ');

    // Using isa<> for type checking
    if (isa<Literal>(expr)) {
        // Using dyn_cast<> for safe casting
        if (auto *lit = dyn_cast<Literal>(expr)) {
            std::println("{}Literal: {}", indent, lit->GetValue());
        }
    } else if (isa<BinaryOp>(expr)) {
        // Using dyn_cast<> for safe casting
        if (auto *binOp = dyn_cast<BinaryOp>(expr)) {
            std::println("{}Binary Op: {}", indent, binOp->GetOpString());
            PrintTreeStructure(binOp->GetLeft(), depth + 1);
            PrintTreeStructure(binOp->GetRight(), depth + 1);
        }
    }
}

// Count how many operations are in the expression
auto CountOperations(const Expr *expr) -> int {
    // Using isa<> for type checking
    if (isa<BinaryOp>(expr)) {
        // After checking with isa<>, we could use cast<> (which asserts)
        // But dyn_cast<> is safer for demonstration
        if (auto *binOp = dyn_cast<BinaryOp>(expr)) {
            return 1 + CountOperations(binOp->GetLeft()) + CountOperations(binOp->GetRight());
        }
    }
    return 0;
}

auto main() -> int {
    std::println("=== PocketLibs Casting Integration Example ===\n");

    // Build expression: (2 + 3) * 4
    auto expr1 = std::make_unique<BinaryOp>(BinaryOp::OpKind::Multiply,
                                            std::make_unique<BinaryOp>(BinaryOp::OpKind::Add,
                                                                       std::make_unique<Literal>(2.0),
                                                                       std::make_unique<Literal>(3.0)),
                                            std::make_unique<Literal>(4.0));

    std::println("Expression 1: {}", expr1->ToString());
    std::println("Result: {}", expr1->Evaluate());
    std::println("Operations: {}\n", CountOperations(expr1.get()));

    // Build expression: 10 / (5 - 3)
    auto expr2 = std::make_unique<BinaryOp>(BinaryOp::OpKind::Divide, std::make_unique<Literal>(10.0),
                                            std::make_unique<BinaryOp>(BinaryOp::OpKind::Subtract,
                                                                       std::make_unique<Literal>(5.0),
                                                                       std::make_unique<Literal>(3.0)));

    std::println("Expression 2: {}", expr2->ToString());
    std::println("Result: {}", expr2->Evaluate());
    std::println("Operations: {}\n", CountOperations(expr2.get()));

    // Print tree structure
    std::println("Tree structure for Expression 1:");
    PrintTreeStructure(expr1.get());
    std::println("");

    // Demonstrate type checking
    Expr *test = expr1.get();
    std::println("Type checks:");
    std::println("  Is Literal? {}", isa<Literal>(test) ? "yes" : "no");
    std::println("  Is BinaryOp? {}", isa<BinaryOp>(test) ? "yes" : "no");
    std::println("  Actual type: {}\n", ExprKindToString(test->GetKind()));

    std::println("=== Example Complete ===");
    std::println("This example compiled with -fno-rtti!");
    std::println("All type checking is done via PocketLibs casting library.");

    return 0;
}
