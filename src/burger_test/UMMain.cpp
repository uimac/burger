/**
 * @file UMMain.cpp
 *
 * @author tori31001 at gmail.com
 *
 * Copyright (C) 2013 Kazuma Hatta
 *
 */
#include <boost/test/included/unit_test.hpp>

#include <iostream>

/**
 * Stream buffer template for debug message routing
 */
template <class CharT, class TraitsT = std::char_traits<CharT> >
class basic_debugbuf : public std::basic_stringbuf<CharT, TraitsT>
{
public:

	virtual ~basic_debugbuf()
	{
		sync();
	}

protected:

	int sync()
	{
		output_debug_string(str().c_str());
		str(std::basic_string<CharT>());	// Clear the string buffer

		return 0;
	}

	void output_debug_string(const CharT *text) {}
};

template<>
void basic_debugbuf<char>::output_debug_string(const char *text)
{
	::OutputDebugStringA(text);
}

template<>
void basic_debugbuf<wchar_t>::output_debug_string(const wchar_t *text)
{
	::OutputDebugStringW(text);
}

template<class CharT, class TraitsT = std::char_traits<CharT> >
class basic_dostream : 
	public std::basic_ostream<CharT, TraitsT>
{
public:
	basic_dostream() : std::basic_ostream<CharT, TraitsT>
				(new basic_debugbuf<CharT, TraitsT>()) {}
	~basic_dostream() 
	{
		delete rdbuf(); 
	}
};

basic_dostream<char> debugout;

boost::unit_test_framework::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	// change log output
	boost::unit_test::unit_test_log_t::instance().set_stream( debugout );
	std::cerr.rdbuf( debugout.rdbuf() );
	return BOOST_TEST_SUITE("burger_test");
}
