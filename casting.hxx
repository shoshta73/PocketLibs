/**
 * @file casting.hxx
 * @brief Provides type casting and type checking utilities compatible with LLVM-style RTTI.
 *
 * This header file defines a set of utilities for type casting and type checking
 * that are designed to be used with LLVM-style RTTI (Run-Time Type Information).
 * It includes functions for checking if a value is of a specific type (`isa`),
 * casting a value to a specific type (`cast`), and dynamically casting a value
 * to a specific type (`dyn_cast`).
 */
/**
 * @defgroup casting Casting Functions
 * @brief Functions for type casting and type checking.
 */

#ifndef CASTING_HXX
#define CASTING_HXX

#ifndef CASTING_NAMESPACE
#define CASTING_NAMESPACE
#endif  // CASTING_NAMESPACE

#ifndef __cplusplus
#error "C++ is required to use this library"
#endif

#if __cplusplus < 202002L
#error "C++20 is required to use this library"
#endif

#include <cassert>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>

namespace CASTING_NAMESPACE {

namespace detail {

template <typename T, typename From>
concept ClassofCallable = requires(const From *p) {
    { T::classof(p) } -> std::convertible_to<bool>;
};

template <typename To, typename From>
auto isa_impl(const From &pVal) -> bool {
    if constexpr (std::is_base_of_v<To, From>) {
        return true;
    } else if constexpr (ClassofCallable<To, From>) {
        return To::classof(&pVal);
    } else {
        return false;
    }
}

template <typename T>
struct is_optional : std::false_type {};
template <typename U>
struct is_optional<std::optional<U>> : std::true_type {};
template <typename T>
inline constexpr bool is_optional_v = is_optional<T>::value;

template <typename T>
struct is_shared_ptr : std::false_type {};
template <typename U>
struct is_shared_ptr<std::shared_ptr<U>> : std::true_type {};
template <typename T>
inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

template <typename T>
struct is_unique_ptr : std::false_type {};
template <typename U>
struct is_unique_ptr<std::unique_ptr<U>> : std::true_type {};
template <typename T>
inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

}  // namespace detail

/**
 * @brief Checks if the given value is of any of the specified types.
 * @ingroup casting
 *
 * @tparam To Types to check against.
 * @tparam From Type of the value.
 * @param pVal The value to check.
 * @return True if the value is of any of the specified types, false otherwise.
 */
template <typename... To, typename From>
    requires(!std::is_pointer_v<From> && !detail::is_optional_v<From> && !detail::is_shared_ptr_v<From> &&
             !detail::is_unique_ptr_v<From>)
[[nodiscard]] auto isa(const From &pVal) -> bool {
    return (detail::isa_impl<To>(pVal) || ...);
}

/**
 * @brief Checks if the given pointer is of any of the specified types.
 * @ingroup casting
 *
 * @tparam To Types to check against.
 * @tparam From Type of the pointer.
 * @param pVal The pointer to check.
 * @return True if the pointer is of any of the specified types, false otherwise.
 */
template <typename... To, typename From>
[[nodiscard]] auto isa(From *pVal) -> bool {
    assert(pVal && "isa<> used on null pointer");
    return (detail::isa_impl<To>(*pVal) || ...);
}

/**
 * @brief Checks if the given const pointer is of any of the specified types.
 * @ingroup casting
 *
 * @tparam To Types to check against.
 * @tparam From Type of the const pointer.
 * @param pVal The const pointer to check.
 * @return True if the const pointer is of any of the specified types, false otherwise.
 */
template <typename... To, typename From>
[[nodiscard]] auto isa(const From *pVal) -> bool {
    assert(pVal && "isa<> used on null pointer");
    return (detail::isa_impl<To>(*pVal) || ...);
}

/**
 * @brief Checks if the given unique pointer is of any of the specified types.
 * @ingroup casting
 *
 * @tparam To Types to check against.
 * @tparam From Type of the unique pointer.
 * @param pVal The unique pointer to check.
 * @return True if the unique pointer is of any of the specified types, false otherwise.
 */
template <typename... To, typename From>
[[nodiscard]] auto isa(const std::unique_ptr<From> &pVal) -> bool {
    assert(pVal && "isa<> used on null pointer");
    return (detail::isa_impl<To>(*pVal) || ...);
}

/**
 * @brief Checks if the given shared pointer is of any of the specified types.
 * @ingroup casting
 *
 * @tparam To Types to check against.
 * @tparam From Type of the shared pointer.
 * @param pVal The shared pointer to check.
 * @return True if the shared pointer is of any of the specified types, false otherwise.
 */
template <typename... To, typename From>
[[nodiscard]] auto isa(const std::shared_ptr<From> &pVal) -> bool {
    assert(pVal && "isa<> used on null pointer");
    return (detail::isa_impl<To>(*pVal) || ...);
}

/**
 * @brief Checks if the given optional is of any of the specified types.
 * @ingroup casting
 *
 * @tparam To Types to check against.
 * @tparam From Type of the optional.
 * @param pVal The optional to check.
 * @return True if the optional is of any of the specified types, false otherwise.
 */
template <typename... To, typename From>
[[nodiscard]] auto isa(const std::optional<From> &pVal) -> bool {
    assert(pVal.has_value() && "isa<> used on empty optional");
    return (detail::isa_impl<To>(*pVal) || ...);
}

/**
 * @brief Casts the given value to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the value.
 * @param pVal The value to cast.
 * @return The casted value.
 */
template <typename To, typename From>
    requires(!std::is_pointer_v<From> && !detail::is_optional_v<From> && !detail::is_shared_ptr_v<From> &&
             !detail::is_unique_ptr_v<From>)
auto cast(From &pVal) -> To & {
    assert(isa<To>(pVal) && "cast<> argument of incompatible type!");
    return static_cast<To &>(pVal);
}

/**
 * @brief Casts the given const value to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the const value.
 * @param pVal The const value to cast.
 * @return The casted const value.
 */
template <typename To, typename From>
    requires(!std::is_pointer_v<From> && !detail::is_optional_v<From> && !detail::is_shared_ptr_v<From> &&
             !detail::is_unique_ptr_v<From>)
auto cast(const From &pVal) -> const To & {
    assert(isa<To>(pVal) && "cast<> argument of incompatible type!");
    return static_cast<const To &>(pVal);
}

/**
 * @brief Casts the given pointer to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the pointer.
 * @param pVal The pointer to cast.
 * @return The casted pointer.
 */
template <typename To, typename From>
auto cast(From *pVal) -> To * {
    assert(isa<To>(pVal) && "cast<> argument of incompatible type!");
    return static_cast<To *>(pVal);
}

/**
 * @brief Casts the given const pointer to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the const pointer.
 * @param pVal The const pointer to cast.
 * @return The casted const pointer.
 */
template <typename To, typename From>
auto cast(const From *pVal) -> const To * {
    assert(isa<To>(pVal) && "cast<> argument of incompatible type!");
    return static_cast<const To *>(pVal);
}

/**
 * @brief Casts the given unique pointer to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the unique pointer.
 * @param pVal The unique pointer to cast.
 * @return The casted unique pointer.
 */
template <typename To, typename From>
auto cast(std::unique_ptr<From> &&pVal) -> std::unique_ptr<To> {
    assert(pVal && "cast<> used on null pointer");
    assert(isa<To>(pVal) && "cast<> argument of incompatible type!");
    // casting consumes the owned pointer stored in std::unique_ptr
    return std::unique_ptr<To>(static_cast<To *>(pVal.release()));
}

/**
 * @brief Casts the given shared pointer to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the shared pointer.
 * @param pVal The shared pointer to cast.
 * @return The casted shared pointer.
 */
template <typename To, typename From>
auto cast(const std::shared_ptr<From> &pVal) -> std::shared_ptr<To> {
    assert(pVal && "cast<> used on null pointer");
    assert(isa<To>(pVal) && "cast<> argument of incompatible type!");
    return std::static_pointer_cast<To>(pVal);
}

/**
 * @brief Casts the given optional to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the optional.
 * @param pVal The optional to cast.
 * @return The casted optional.
 */
template <typename To, typename From>
auto cast(const std::optional<From> &pVal) -> std::optional<To> {
    if (!pVal.has_value()) return std::nullopt;
    assert(isa<To>(*pVal) && "cast<> argument of incompatible type!");
    return std::optional<To>(cast<To>(*pVal));
}

/**
 * @brief Dynamically casts the given value to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the value.
 * @param pVal The value to cast.
 * @return The casted value, or nullptr if the cast fails.
 */
template <typename To, typename From>
    requires(!std::is_pointer_v<From> && !detail::is_optional_v<From> && !detail::is_shared_ptr_v<From> &&
             !detail::is_unique_ptr_v<From>)
[[nodiscard]] auto dyn_cast(From &pVal) -> To * {
    return isa<To>(pVal) ? cast<To>(&pVal) : nullptr;
}

/**
 * @brief Dynamically casts the given const value to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the const value.
 * @param pVal The const value to cast.
 * @return The casted const value, or nullptr if the cast fails.
 */
template <typename To, typename From>
    requires(!std::is_pointer_v<From> && !detail::is_optional_v<From> && !detail::is_shared_ptr_v<From> &&
             !detail::is_unique_ptr_v<From>)
[[nodiscard]] auto dyn_cast(const From &pVal) -> const To * {
    return isa<To>(pVal) ? cast<To>(&pVal) : nullptr;
}

/**
 * @brief Dynamically casts the given pointer to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the pointer.
 * @param pVal The pointer to cast.
 * @return The casted pointer, or nullptr if the cast fails.
 */
template <typename To, typename From>
[[nodiscard]] auto dyn_cast(From *pVal) -> To * {
    return (pVal && isa<To>(pVal)) ? cast<To>(pVal) : nullptr;
}

/**
 * @brief Dynamically casts the given const pointer to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the const pointer.
 * @param pVal The const pointer to cast.
 * @return The casted const pointer, or nullptr if the cast fails.
 */
template <typename To, typename From>
[[nodiscard]] auto dyn_cast(const From *pVal) -> const To * {
    return (pVal && isa<To>(pVal)) ? cast<To>(pVal) : nullptr;
}

/**
 * @brief Dynamically casts the given unique pointer to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the unique pointer.
 * @param pVal The unique pointer to cast.
 * @return The casted unique pointer, or nullptr if the cast fails.
 */
template <typename To, typename From>
[[nodiscard]] auto dyn_cast(std::unique_ptr<From> &&pVal) -> std::unique_ptr<To> {
    if (!pVal || !isa<To>(pVal)) {
        return nullptr;
    }
    return cast<To>(std::move(pVal));
}

/**
 * @brief Dynamically casts the given shared pointer to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the shared pointer.
 * @param pVal The shared pointer to cast.
 * @return The casted shared pointer, or nullptr if the cast fails.
 */
template <typename To, typename From>
[[nodiscard]] auto dyn_cast(const std::shared_ptr<From> &pVal) -> std::shared_ptr<To> {
    if (!pVal || !isa<To>(pVal)) {
        return nullptr;
    }
    return cast<To>(pVal);
}

/**
 * @brief Dynamically casts the given optional to the specified type.
 * @ingroup casting
 *
 * @tparam To Type to cast to.
 * @tparam From Type of the optional.
 * @param pVal The optional to cast.
 * @return The casted optional, or std::nullopt if the cast fails.
 */
template <typename To, typename From>
[[nodiscard]] auto dyn_cast(const std::optional<From> &pVal) -> std::optional<To> {
    if (!pVal.has_value() || !isa<To>(pVal)) {
        return std::nullopt;
    }
    return cast<To>(pVal);
}

}  // namespace CASTING_NAMESPACE

#ifdef CASTING_NAMESPACE
#undef CASTING_NAMESPACE  // Dont leak this macro outside of this file
#endif

#endif  // CASTING_HXX
