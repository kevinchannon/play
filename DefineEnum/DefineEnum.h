//
// DefineEnum.h
//

#include <cstring>
#include <climits>

#include <boost\preprocessor\tuple\elem.hpp>
#include <boost\preprocessor\seq\for_each.hpp>
#include <boost\preprocessor\stringize.hpp>

// Adapted from http://stackoverflow.com/questions/24309309/how-to-use-boost-preprocessor-to-generate-accessors/24312646#24312646

///////////////////////////////////////////////////////////////////////////////
//
// Define values for each enum symbol
//
///////////////////////////////////////////////////////////////////////////////

#define DECLARE_ENUM_SYMBOL_AND_VALUE( R,_, SYMBOL_AND_VALUE ) \
	BOOST_PP_CAT( e, BOOST_PP_TUPLE_ELEM( 2, 0, SYMBOL_AND_VALUE ) ) = BOOST_PP_TUPLE_ELEM( 2, 1, SYMBOL_AND_VALUE ),

#define DEFINE_ENUM_SYMBOLS_AND_VALUES( SYMBOLS_AND_VALUES ) \
BOOST_PP_SEQ_FOR_EACH( DECLARE_ENUM_SYMBOL_AND_VALUE,_, SYMBOLS_AND_VALUES )

#define DECLARE_ENUM_CASE_FROM_SYMB_AND_VALUE( R,_, SYMBOL_AND_VALUE ) \
	case BOOST_PP_CAT( e, BOOST_PP_TUPLE_ELEM( 2, 0, SYMBOL_AND_VALUE ) ) :

#define DEFINE_ENUM_SYMBOLS_FOR_VALIDITY_FROM_SYMB_AND_VALUE( SYMBOLS_AND_VALUES ) \
	BOOST_PP_SEQ_FOR_EACH( DECLARE_ENUM_CASE_FROM_SYMB_AND_VALUE,_, SYMBOLS_AND_VALUES )

#define DECLARE_ENUM_CASE_AND_TEXT_FROM_SYMB_AND_VAL( R,_, SYMBOL_AND_VALUE ) \
	case BOOST_PP_CAT( e, BOOST_PP_TUPLE_ELEM( 2, 0, SYMBOL_AND_VALUE ) ) : return BOOST_PP_STRINGIZE( BOOST_PP_TUPLE_ELEM( 2, 0, SYMBOL_AND_VALUE ) );

#define DEFINE_ENUM_CASES_AND_TEXTS_FROM_SYMB_AND_VALUE( SYMBOLS_AND_VALUES ) \
	BOOST_PP_SEQ_FOR_EACH( DECLARE_ENUM_CASE_AND_TEXT_FROM_SYMB_AND_VAL,_, SYMBOLS_AND_VALUES ) 

#define DEFINE_ENUM_TEXT_AND_VALUE_CLAUSE_FROM_SYMB_AND_VALUE( R,_, SYMBOL_AND_VALUE ) \
	else if ( strcmp( BOOST_PP_STRINGIZE( BOOST_PP_TUPLE_ELEM( 2, 0, SYMBOL_AND_VALUE ) ), psz ) == 0 ) {\
		m_eVal = BOOST_PP_CAT( e, BOOST_PP_TUPLE_ELEM( 2, 0, SYMBOL_AND_VALUE ) );\
		return;\
	}

#define DEFINE_ENUM_TEXTS_AND_VALUES_CLAUSES_FROM_SYMB_AND_VALUE( SYMBOLS_AND_VALUES ) \
	BOOST_PP_SEQ_FOR_EACH( DEFINE_ENUM_TEXT_AND_VALUE_CLAUSE_FROM_SYMB_AND_VALUE,_ , SYMBOLS_AND_VALUES ) 

#define DEFINE_ENUM_CLASS_WITH_VALUES( ENUM_NAME, SYMBOLS_AND_VALUES ) \
class BOOST_PP_CAT( E, ENUM_NAME ) { \
public :\
	enum EType { \
	BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) ) = LONG_MIN, \
	DEFINE_ENUM_SYMBOLS_AND_VALUES( SYMBOLS_AND_VALUES ) \
		};\
	static const int s_iTypeCount = BOOST_PP_SEQ_SIZE( SYMBOLS_AND_VALUES );\
			BOOST_PP_CAT( E, ENUM_NAME )() : m_eVal( BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) ) ) {}\
			BOOST_PP_CAT( E, ENUM_NAME )( EType e ) : m_eVal( e ) {}\
			explicit BOOST_PP_CAT( E, ENUM_NAME )( int i ) : m_eVal( static_cast< EType >( i ) ) {}\
			const char* Name() const { return BOOST_PP_STRINGIZE( ENUM_NAME ); }\
		bool IsValid() const { \
		switch( m_eVal ){\
		DEFINE_ENUM_SYMBOLS_FOR_VALIDITY_FROM_SYMB_AND_VALUE( SYMBOLS_AND_VALUES )\
			return true;\
		}\
		return false; \
	}\
	const char* ToText() const {\
	switch( m_eVal ) {\
		DEFINE_ENUM_CASES_AND_TEXTS_FROM_SYMB_AND_VALUE( SYMBOLS_AND_VALUES )\
		}\
		return "Unknown"; \
	}\
	void FromText( const char* psz ) {\
		if ( strcmp( psz, "Unknown" ) == 0 ) {\
			m_eVal = BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) );\
			return;\
		}\
		DEFINE_ENUM_TEXTS_AND_VALUES_CLAUSES_FROM_SYMB_AND_VALUE( SYMBOLS_AND_VALUES )\
		else {\
			m_eVal = BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) );\
			return;\
		}\
	}\
	int ToInt() const { return static_cast< int >( m_eVal ); }\
	static BOOST_PP_CAT( E, ENUM_NAME ) Create( const char* psz ) {\
		BOOST_PP_CAT( E, ENUM_NAME ) p;\
		p.FromText( psz );\
		return p;\
	}\
private :\
	EType m_eVal;\
};

///////////////////////////////////////////////////////////////////////////////
//
// Just define the symbols and let them have the default values
//
///////////////////////////////////////////////////////////////////////////////

#define DECLARE_ENUM_SYMBOL( R,_, SYMBOL ) \
	BOOST_PP_CAT( e, SYMBOL ),

#define DEFINE_ENUM_SYMBOLS( SYMBOLS ) \
	BOOST_PP_SEQ_FOR_EACH( DECLARE_ENUM_SYMBOL,_, SYMBOLS)

#define DECLARE_ENUM_CASE_AND_TEXT_FROM_SYMB( R,_, SYMBOL ) \
	case BOOST_PP_CAT( e, SYMBOL ) : return BOOST_PP_STRINGIZE( SYMBOL );

#define DEFINE_ENUM_CASES_AND_TEXTS_FROM_SYMB( SYMBOLS ) \
BOOST_PP_SEQ_FOR_EACH( DECLARE_ENUM_CASE_AND_TEXT_FROM_SYMB,_ , SYMBOLS ) 

#define DEFINE_ENUM_TEXT_AND_VALUE_CLAUSE_FROM_SYMB( R,_, SYMBOL ) \
	else if ( strcmp( BOOST_PP_STRINGIZE( SYMBOL ), psz ) == 0 ) {\
	m_eVal = BOOST_PP_CAT( e, SYMBOL );\
	return;\
}

#define DEFINE_ENUM_TEXTS_AND_VALUES_CLAUSES_FROM_SYMB( SYMBOLS ) \
	BOOST_PP_SEQ_FOR_EACH( DEFINE_ENUM_TEXT_AND_VALUE_CLAUSE_FROM_SYMB,_ , SYMBOLS ) 

#define DEFINE_ENUM_CLASS( ENUM_NAME, SYMBOLS ) \
class BOOST_PP_CAT( E, ENUM_NAME ) { \
public :\
	enum EType { \
	BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) ) = -1, \
	DEFINE_ENUM_SYMBOLS( SYMBOLS ) \
		};\
	static const int s_iTypeCount = BOOST_PP_SEQ_SIZE( SYMBOLS );\
			BOOST_PP_CAT( E, ENUM_NAME )() : m_eVal( BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) ) ) {}\
			BOOST_PP_CAT( E, ENUM_NAME )( EType e ) : m_eVal( e ) {}\
			explicit BOOST_PP_CAT( E, ENUM_NAME )( int i ) : m_eVal( static_cast< EType >( i ) ) {}\
			const char* Name() const { return BOOST_PP_STRINGIZE( ENUM_NAME ); }\
		bool IsValid() const { \
		return m_eVal > BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) ) && m_eVal < s_iTypeCount; \
		} \
		const char* ToText() const {\
		switch( m_eVal ) { \
		DEFINE_ENUM_CASES_AND_TEXTS_FROM_SYMB( SYMBOLS )\
		}\
		return "Unknown"; \
		}\
		void FromText( const char* psz ) {\
		if ( strcmp( psz, "Unknown" ) == 0 ){\
			m_eVal = BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) );\
			return;\
		}\
		DEFINE_ENUM_TEXTS_AND_VALUES_CLAUSES_FROM_SYMB( SYMBOLS )\
		else {\
			m_eVal = BOOST_PP_CAT( e, BOOST_PP_CAT( ENUM_NAME, _Unknown ) );\
			return;\
		}\
		}\
			int ToInt() const { return static_cast< int >( m_eVal ); }\
			static BOOST_PP_CAT( E, ENUM_NAME ) Create( const char* psz ) {\
		BOOST_PP_CAT( E, ENUM_NAME ) p;\
		p.FromText( psz );\
		return p;\
	}\
private :\
		 EType m_eVal;\
};

