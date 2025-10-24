# Casting API Reference

## isa

This function checks if the given value is of any of the specified types.

> Template Parameters:
>
> > ```cpp
> > typename... To
> > ```
> >
> > The types to check against.
> >
> > ---
> >
> > ```cpp
> > typename From
> > ```
> >
> > The type of the value.
>
> Parameters:
>
> > ```cpp
> > const From &pVal // (1)
> > From *pVal // (2)
> > const From *pVal // (3)
> > std::unique_ptr<From> &pVal // (4)
> > std::shared_ptr<From> &pVal // (5)
> > const std::optional<From> &pVal // (6)
> > ```
> >
> > The value to check.
>
> Returns:
>
> > ```cpp
> > bool
> > ```
> >
> > True if the value is of any of the specified types, false otherwise.

## cast

This function casts the given value to the specified type.

> [!IMPORTANT]
> In debug mode, this function will assert if the given value is not of the specified type.
>
> In debug mode, if `std::optional` and `optional.has_value()` is false, this function will assert.

> Template Parameters:
>
> > ```cpp
> > typename To
> > ```
> >
> > The type to cast to.
> >
> > ---
> >
> > ```cpp
> > typename From
> > ```
> >
> > The type of the value.
>
> Parameters:
>
> > ```cpp
> > const From &pVal // (1)
> > From *pVal // (2)
> > const From *pVal // (3)
> > std::unique_ptr<From> &&pVal // (4)
> > std::shared_ptr<From> &pVal // (5)
> > const std::optional<From> &pVal // (6)
> > ```
> >
> > The value to cast.
>
> Returns:
>
> > The casted value.

## dyn_cast

This function dynamically casts the given value to the specified type.

> [!WARNING]
>
> ```cpp
> {{#include ../../../casting.h:330:336}}
> ```
>
> This `dyn_cast` overload will consume the owned pointer stored in `std::unique_ptr` if the cast is possible.

> Template Parameters:
>
> > ```cpp
> > typename To
> > ```
> >
> > The type to cast to.
> >
> > ---
> >
> > ```cpp
> > typename From
> > ```
> >
> > The type of the value.
>
> Parameters:
>
> > ```cpp
> > const From &pVal // (1)
> > From *pVal // (2)
> > const From *pVal // (3)
> > std::unique_ptr<From> &&pVal // (4)
> > std::shared_ptr<From> &pVal // (5)
> > const std::optional<From> &pVal // (6)
> > ```
> >
> > The casted value or `nullptr` if the cast is not possible.
