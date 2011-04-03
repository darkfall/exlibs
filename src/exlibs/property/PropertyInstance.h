// ======================================================================================
// File         : PropertyInstance.h
// Author       : Wu Jie 
// Last Change  : 07/11/2009 | 11:03:13 AM | Saturday,July
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef PROPERTYINSTANCE_H_1250526176
#define PROPERTYINSTANCE_H_1250526176
// #################################################################################

#include "PropertyInstanceBase.h"
#include "PropertyBase.h"

// ######################### 
namespace ex { 
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// class PropertyInstance 
// 
// Purpose: 
// 
///////////////////////////////////////////////////////////////////////////////

template < typename T >
class PropertyInstance : public PropertyInstanceBase
{
    ///////////////////////////////////////////////////////////////////////////////
    // typedefs
    ///////////////////////////////////////////////////////////////////////////////

public:

    typedef PropertyInstanceBase    base_t;
    typedef PropertyInstance<T>     self_t;

    ///////////////////////////////////////////////////////////////////////////////
    // constructor & destructor
    ///////////////////////////////////////////////////////////////////////////////

public:

    // 
    PropertyInstance ( PropertyBase* _pProperty, void* _pObject )
        : base_t ( _pProperty, _pObject )
    {
    }

}; // end class PropertyInstance 

// ######################### 
} // end namespace ex 
// ######################### 

// #################################################################################
#endif // END PROPERTYINSTANCE_H_1250526176
// #################################################################################
