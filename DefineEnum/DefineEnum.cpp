//
// DefineEnum.cpp
//

#include "DefineEnum.h"

#include <iostream>

/// An enum that uses the default values
DEFINE_ENUM_CLASS( PersonType, \
	(Alice)\
	(Bob)\
	(Eve)\
	)

/// An enum that has values for each symbol
DEFINE_ENUM_CLASS_WITH_VALUES( SomeType, \
	((A,1))\
	((B,10))\
	((C,100))\
	)

template< typename T >
void Print( const T& p )
{
	std::cout << p.ToText() << " (" << p.ToInt() << ") " << ( p.IsValid() ? "is" : "is not" ) << " a valid " << p.Name() << std::endl;
	std::cout << p.Name() << " has " << T::s_iTypeCount << " values" << std::endl;

	std::cout << std::endl;
}

int main(int argc, char* argv[])
{
	EPersonType alice( EPersonType::eAlice );
	
	Print( alice );

	Print( EPersonType( static_cast< EPersonType::EType >( 17 ) ) );

	Print( EPersonType::Create( "Bob" ) );

	auto b = ESomeType::Create( "B" );

	Print( b );

	ESomeType t( ESomeType::eA );

	Print( t );

	return 0;
}

