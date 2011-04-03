// ======================================================================================
// File         : ConfigParser.cpp
// Author       : Wu Jie 
// Last Change  : 09/26/2009 | 14:45:46 PM | Saturday,September
// Description  : 
// ======================================================================================

///////////////////////////////////////////////////////////////////////////////
// includes 
///////////////////////////////////////////////////////////////////////////////

#include "testbed.h"

///////////////////////////////////////////////////////////////////////////////
// Test cases
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------
// Desc: 
// ------------------------------------------------------------------

TEST( ConfigParser, test, off )
{
    printf ( "Version = %f, WorkingPath = %s \n", ex::core_t::config()->get<float>("Version"), ex::core_t::config()->get<const char*>("cwd") );
    ex::core_t::config()->get<const char*>("NoValue");
}
