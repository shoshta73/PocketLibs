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
> > const From &pVal
> > From *pVal
> > const From *pVal
> > std::unique_ptr<From> &pVal
> > std::shared_ptr<From> &pVal
> > const std::optional<From> &pVal
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
> > const From &pVal
> > From *pVal
> > const From *pVal
> > std::unique_ptr<From> &&pVal
> > std::shared_ptr<From> &pVal
> > const std::optional<From> &pVal
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
> {{#include ../../../casting.h:326:332}}
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
> > const From &pVal
> > From *pVal
> > const From *pVal
> > std::unique_ptr<From> &&pVal
> > std::shared_ptr<From> &pVal
> > const std::optional<From> &pVal
> > ```
> >
> > The casted value or `nullptr` if the cast is not possible.
