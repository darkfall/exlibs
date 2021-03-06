// ======================================================================================
// File         : Line.h
// Author       : Wu Jie 
// Last Change  : 11/05/2009 | 15:17:37 PM | Thursday,November
// Description  : 
// ======================================================================================

// #################################################################################
#ifndef LINE_H_1257405360
#define LINE_H_1257405360
// #################################################################################

///////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////

// ######################### 
namespace ex { 
// ######################### 

///////////////////////////////////////////////////////////////////////////////
// class Line2D 
// 
// Purpose: 
// 
///////////////////////////////////////////////////////////////////////////////

template < typename T >
class Line2D
{
    ///////////////////////////////////////////////////////////////////////////////
    // typedefs
    ///////////////////////////////////////////////////////////////////////////////

public:

    typedef Vector2<T> vector2_t;

    ///////////////////////////////////////////////////////////////////////////////
    // public member functions
    ///////////////////////////////////////////////////////////////////////////////

public:

    //
    Line2D ( const vector2_t& _start = vector2_t(0,0), const vector2_t& _end = vector2_t(1,0) )
    {
        this->set (_start, _end);
    }

    //
    void set ( const vector2_t& _start, const vector2_t& _end )
    {
        m_start = _start;
        m_end = _end;
    }

    //
    vector2_t& start() { return m_start; }
    const vector2_t& start() const { return m_start; }

    vector2_t& end() { return m_end; }
    const vector2_t& end() const { return m_end; }

    //
    float length() const { return vector2_t(m_end - m_start).to_vec2f_t().length() ; }
    vector2_t dir() const { vector2_t res; vector2_t(m_end - m_start).get_normalize(&res); return res; }

    ///////////////////////////////////////////////////////////////////////////////
    // protected member data
    ///////////////////////////////////////////////////////////////////////////////

protected:

    vector2_t m_start;
    vector2_t m_end;

}; // end class Line2D

// ######################### 
} // end namespace ex 
// ######################### 

// #################################################################################
#endif // END LINE_H_1257405360
// #################################################################################

