// ======================================================================================
// File         : QuadTreeItem.hpp
// Author       : Wu Jie 
// Last Change  : 12/29/2009 | 14:22:31 PM | Tuesday,December
// Description  : 
// ======================================================================================

// ######################### 
namespace ex { 
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// macro defines
///////////////////////////////////////////////////////////////////////////////

#define qtree_t typename TQuadTreeItem<T>::qtree_t
#define node_t typename TQuadTreeItem<T>::node_t
#define item_t typename TQuadTreeItem<T>::item_t
#define base_t typename TQuadTreeItem<T>::base_t

///////////////////////////////////////////////////////////////////////////////
// defines
///////////////////////////////////////////////////////////////////////////////

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

template < typename T >
TQuadTreeItem<T>::TQuadTreeItem ( const T& _value, const qtree_t& _qtree )
    : TQuadTreeItemBase<T> ( _value, _qtree.MapFromWorld(_value.AABRect().toRect()) )
{
}

// ------------------------------------------------------------------ 
// Desc: 
// ------------------------------------------------------------------ 

template < typename T >
TQuadTreeItem<T>::~TQuadTreeItem ()
{
}

///////////////////////////////////////////////////////////////////////////////
// undefs
///////////////////////////////////////////////////////////////////////////////

#undef qtree_t
#undef node_t
#undef item_t
#undef base_t

// ######################### 
} // end namespace ex 
// ######################### 

