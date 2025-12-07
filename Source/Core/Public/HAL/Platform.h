#pragma once
#include <type_traits>

struct FPlatformTypes
{
    //~ Unsigned base types

    // 8-bit unsigned integer
    typedef unsigned char 		uint8;

    // 16-bit unsigned integer
    typedef unsigned short int	uint16;

    // 32-bit unsigned integer
    typedef unsigned int		uint32;

    // 64-bit unsigned integer
    typedef unsigned long long	uint64;

    //~ Signed base types.

    // 8-bit signed integer
    typedef	signed char			int8;

    // 16-bit signed integer
    typedef signed short int	int16;

    // 32-bit signed integer
    typedef signed int	 		int32;

    // 64-bit signed integer
    typedef signed long long	int64;
};

//------------------------------------------------------------------
// Transfer the platform types to global types
//------------------------------------------------------------------

//~ Unsigned base types.
/// An 8-bit unsigned integer.
typedef FPlatformTypes::uint8		uint8;
/// A 16-bit unsigned integer.
typedef FPlatformTypes::uint16		uint16;
/// A 32-bit unsigned integer.
typedef FPlatformTypes::uint32		uint32;
/// A 64-bit unsigned integer.
typedef FPlatformTypes::uint64		uint64;

//~ Signed base types.
/// An 8-bit signed integer.
typedef	FPlatformTypes::int8		int8;
/// A 16-bit signed integer.
typedef FPlatformTypes::int16		int16;
/// A 32-bit signed integer.
typedef FPlatformTypes::int32		int32;
/// A 64-bit signed integer.
typedef FPlatformTypes::int64		int64;
