// ================================================================================
#ifndef OSTAP_BERNSTEIN3D_H 
#define OSTAP_BERNSTEIN3D_H 1
// ============================================================================
// Include files
// ============================================================================
// Ostap
// ============================================================================
#include "Ostap/Math.h"
#include "Ostap/Bernstein.h"
#include "Ostap/NSphere.h"
// ============================================================================
/** @file Ostap/Bernstein3D.h
 *  Collection of files related to 3D-moodels, based on Bernstein polynomials 
 *  @author Vanya Belyaev
 *  @date   2017-11-18
 */
// ============================================================================
namespace Ostap
{
  // ==========================================================================
  namespace Math
  {
    // ========================================================================
    class Bernstein3D   ;
    class Bernstein3DSym;    
    class Bernstein3DMix;    
    // ========================================================================
    /** @class Bernstein3D
     *  Generic 3D-polynomial of order defined as 
     *  \f[ P(x,y,z) = \sum_{i,j,k} a_{ijk}B^{n_x}_i(x) B^{n_y}_j(y) B^{n_z}_k(z)\f] 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-11-14
     */
    class Bernstein3D
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Bernstein3D ( const unsigned short       nX    =  1 ,
                    const unsigned short       nY    =  1 ,
                    const unsigned short       nZ    =  1 ,
                    const double               xmin  =  0 ,
                    const double               xmax  =  1 ,
                    const double               ymin  =  0 ,
                    const double               ymax  =  1 ,
                    const double               zmin  =  0 ,
                    const double               zmax  =  1 ) ;
      // ======================================================================
      /// copy constructor 
      Bernstein3D ( const Bernstein3D&    right ) = default ;   
      /// move constructor 
      Bernstein3D (       Bernstein3D&&   right ) ;
      // ======================================================================
      /// from symmetric variant 
      explicit Bernstein3D ( const Bernstein3DSym& right ) ;
      /// from symmetric variant 
      explicit Bernstein3D ( const Bernstein3DMix& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double evaluate    ( const double x ,
                           const double y , 
                           const double z ) const ;
      // ======================================================================
      /// get the value
      double operator () ( const double x ,
                           const double y , 
                           const double z ) const 
      { return evaluate ( x ,   y , z ) ; }
      // ======================================================================
    public: // setters
      // ======================================================================
      /// set k-parameter
      bool setPar       ( const unsigned int   k     ,
                          const double         value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int   k     ,
                          const double         value )
      { return ( k < m_pars.size() ) && setPar ( k , value ) ; }
      /// set (l,m)-parameter
      bool setPar       ( const unsigned short l     ,
                          const unsigned short m     ,
                          const unsigned short n     ,
                          const double         value )
      {
        const unsigned int k = index ( l , m , n ) ;
        return  ( k < m_pars.size() ) && setPar ( k , value )  ;
      }
      /// set (l,m)-parameter
      bool setParameter ( const unsigned short l     ,
                          const unsigned short m     ,
                          const unsigned short n     ,
                          const double         value )
      { return setPar   ( l , m  , n , value ) ; }
      // ======================================================================
    public: // getters
      // ======================================================================
      /// get (l,m,n)-parameter
      double  par       ( const unsigned short l ,
                          const unsigned short m ,
                          const unsigned short n ) const 
      {  return par ( index ( l , m , n ) ) ; }
      /// get (l,m,n)-parameter
      double  parameter ( const unsigned short l ,
                          const unsigned short m ,
                          const unsigned short n ) const { return par (  l , m , n ) ; }
      /// get k-parameter
      double  par       ( const unsigned int k ) const
      { return k < m_pars.size() ? m_pars[k] : 0.0 ; }
      /// get k-parameter
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      /// get all parameters at once
      const std::vector<double>& pars() const { return m_pars ; }
      // ======================================================================
    public: // convert (l,m,n) into single index k
      // ======================================================================
      /// convert (l,m,n)-index into single index k  
      unsigned int index ( const unsigned short l , 
                           const unsigned short m , 
                           const unsigned short n ) const 
      {
        return 
          ( l > m_nx || m > m_ny || n > m_nz ) ? -1  :  // NB!
          1u * ( m_nz + 1 ) * ( m_ny + 1 ) * l +
          1u * ( m_nz + 1 )                * m + 
          n ;
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get the actual number of parameters
      std::size_t npars () const { return m_pars.size() ; }
      /// get lower edge
      double xmin () const { return m_xmin ; }
      /// get upper edge
      double xmax () const { return m_xmax ; }
      /// get lower edge
      double ymin () const { return m_ymin ; }
      /// get upper edge
      double ymax () const { return m_ymax ; }
      /// get lower edge
      double zmin () const { return m_zmin ; }
      /// get upper edge
      double zmax () const { return m_zmax ; }
      /// get the polynomial order (X)
      unsigned short nX () const { return m_nx ; }
      /// get the polynomial order (Y)
      unsigned short nY () const { return m_ny ; }
      /// get the polynomial order (Y)
      unsigned short nZ () const { return m_nz ; }
      // ======================================================================
    public:  // transformations
      // ======================================================================
      double x  ( const double tx ) const
      { return xmin ()  + ( xmax () - xmin () ) * tx ; }
      double y  ( const double ty ) const
      { return ymin ()  + ( ymax () - ymin () ) * ty ; }
      double z  ( const double tz ) const
      { return zmin ()  + ( zmax () - zmin () ) * tz ; }
      double tx ( const double x ) const
      { return  ( x - xmin () ) / ( xmax () - xmin () )      ; }
      double ty ( const double y ) const
      { return  ( y - ymin () ) / ( ymax () - ymin () )      ; }
      double tz ( const double z ) const
      { return  ( z - zmin () ) / ( zmax () - zmin () )      ; }
      // ======================================================================
    public:
      // ======================================================================
      /// simple  manipulations with polynoms: shift it!
      Bernstein3D& operator += ( const double a ) ;
      /// simple  manipulations with polynoms: shift it!
      Bernstein3D& operator -= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein3D& operator *= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein3D& operator /= ( const double a ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// negate it!
      Bernstein3D  operator-() const ;
      // ======================================================================
    public:
      // ======================================================================
      /// Sum of Bernstein polynomial and a constant
      Bernstein3D __add__   ( const double value ) const ;
      /// Sum of Bernstein polynomial and a constant
      Bernstein3D __radd__  ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein3D __mul__   ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein3D __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      Bernstein3D __sub__   ( const double value ) const ;
      /// Constant minus Bernstein polynomial
      Bernstein3D __rsub__  ( const double value ) const ;
      /// Divide Benrstein polynomial by a constant
      Bernstein3D __div__   ( const double value ) const ;
      /// Negate Bernstein polynomial
      Bernstein3D __neg__   () const ;
      // ======================================================================
    public: // general integration
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{low}}^{x_{high}} 
       *      \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}}
       *      \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param x     variable
       *  @param z     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double z    ,                          
                          const double xlow , const double xhigh ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param y     variable
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double z    ,
                          const double ylow , const double yhigh ) const ;
      /** integral over z-dimension
       *  \f[ \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integrateZ ( const double x    ,
                          const double y    ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateXY ( const double z    ,                          
                           const double xlow , const double xhigh ,
                           const double ylow , const double yhigh ) const ;
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateXZ ( const double y    ,                          
                           const double xlow , const double xhigh ,
                           const double zlow , const double zhigh ) const ;      
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateYZ ( const double x    ,                          
                           const double ylow , const double yhigh ,
                           const double zlow , const double zhigh ) const ;
      // ======================================================================      
    public: // special cases
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[  x_{min} < x < x_{max}, 
       *       y_{min} < y < y_{max},
       *       z_{min} < z < z_{max}\f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       */
      double integrateX ( const double y , 
                          const double z ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       */
      double integrateY ( const double x , 
                          const double z ) const ;
      /** integral over z-dimension
       *  \f[ \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       */
      double integrateZ ( const double x , 
                          const double y ) const ;

      // ======================================================================
    public: // special cases
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{min}}^{x_{max}}
       *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       */
      double integrateXY ( const double z    ) const ;
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{min}}^{x_{min}}
       *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       */
      double integrateXZ ( const double y    ) const ;
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{min}}^{y_{max}}
       *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       */
      double integrateYZ ( const double x    ) const ;
      // ======================================================================
    public: // few helper functions to expose internals
      // ======================================================================
      /// evaluate the basic polynomials
      double basicX ( const unsigned short i , const double         x ) const
      { return ( i > m_nx || x < m_xmin || x < m_xmax ) ? 0.0 : m_bx[i](x) ; }
      /// evaluate the basic polynomials
      double basicY ( const unsigned short i , const double         y ) const
      { return ( i > m_ny || y < m_ymin || y < m_ymax ) ? 0.0 : m_by[i](y) ; }
      /// evaluate the basic polynomials
      double basicZ ( const unsigned short i , const double         z ) const
      { return ( i > m_nz || z < m_zmin || z < m_zmax ) ? 0.0 : m_bz[i](z) ; }
      /// expose some internals
      const Bernstein& basicX ( const unsigned short i ) const { return m_bx[i] ; }
      /// expose some internals
      const Bernstein& basicY ( const unsigned short i ) const { return m_by[i] ; }
      /// expose some internals
      const Bernstein& basicZ ( const unsigned short i ) const { return m_bz[i] ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap two Bernstein polynomials   
      void swap ( Bernstein3D& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the tag value 
      std::size_t tag () const ; // get the tag value 
      // ======================================================================
    private: // helper functions to make calculations
      // ======================================================================
      /// helper function to make calculations
      double calculate ( const std::vector<double>& fx , 
                         const std::vector<double>& fy , 
                         const std::vector<double>& fz ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // polynom order in x-dimension
      unsigned short m_nx ; // polynom order in x-dimension
      // polynom order in y-dimension
      unsigned short m_ny ; // polynom order in y-dimension
      // polynom order in z-dimension
      unsigned short m_nz ; // polynom order in z-dimension
      /// the list of parameters
      std::vector<double>  m_pars ;                // the list of parameters
      /// the left edge of interval
      double m_xmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_xmax  ;                             // the right edge of interval
      /// the left edge of interval
      double m_ymin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_ymax  ;                             // the right edge of interval
      /// the left edge of interval
      double m_zmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_zmax  ;                             // the right edge of interval
      // ======================================================================
    private:
      // ======================================================================
      ///  vectors of basic  Bernstein polynomials
      typedef std::vector<Bernstein>  VB ;
      ///  vector  of basic  Bernstein polynomials
      VB m_bx ; //  vector  of basic  Bernstein polynomials
      ///  vector  of basic  Bernstein polynomials
      VB m_by ; //  vector  of basic  Bernstein polynomials
      ///  vector  of basic  Bernstein polynomials
      VB m_bz ; //  vector  of basic  Bernstein polynomials
      // ======================================================================
    } ;
    // ========================================================================
    ///  Bernstein plus      constant
    inline Bernstein3D operator+( const Bernstein3D& p , const double v )
    { return Bernstein3D ( p ) += v ; } //  Bernstein plus constant
    ///  Bernstein multiply  constant
    inline Bernstein3D operator*( const Bernstein3D& p , const double v )
    { return Bernstein3D ( p ) *= v ; } //  Bernstein plus constant
    ///  Bernstein minus constant
    inline Bernstein3D operator-( const Bernstein3D& p , const double v )
    { return Bernstein3D ( p ) -= v ; } //  Bernstein plus constant
    ///  Bernstein divide constant
    inline Bernstein3D operator/( const Bernstein3D& p , const double v )
    { return Bernstein3D ( p ) /= v ; } //  Bernstein plus constant
    ///  Constant plus  Bernstein
    inline Bernstein3D operator+( const double v , const Bernstein3D& p ) { return p +   v  ; }
    ///  Constant times Bernstein
    inline Bernstein3D operator*( const double v , const Bernstein3D& p ) { return p *   v  ; }
    ///  Constant minus Bernstein
    inline Bernstein3D operator-( const double v , const Bernstein3D& p ) { return v + (-p) ; }
     // ========================================================================
    /// swap two Bernstein polynomials   
    inline  void swap ( Bernstein3D& a , Bernstein3D& b ) { a.swap ( b ) ;  }
    // ========================================================================
    /** @class Bernstein3DSym
     *  Generic 3D-polynomial of order N*N*N defined as 
     *  \f[ P(x,y,z) = \sum_{i,j,k} a_{ijk}B^{n}_i(x) B^{n}_j(y) B^{n}_k(z)\f] 
     *  where \f[ P(x,y,z) = P(y,x,z) = P(x,z,y)\f] 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-11-14
     */
    class Bernstein3DSym
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Bernstein3DSym ( const unsigned short       N     =  1 ,
                       const double               xmin  =  0 ,
                       const double               xmax  =  1 ) ;
      // ======================================================================
      /// copy constructor 
      Bernstein3DSym ( const Bernstein3DSym&  right ) = default ;   
      /// move constructor 
      Bernstein3DSym (       Bernstein3DSym&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double evaluate    ( const double x ,
                           const double y , 
                           const double z ) const ;
      // ======================================================================
      /// get the value
      double operator () ( const double x ,
                           const double y , 
                           const double z ) const 
      { return evaluate ( x ,   y , z ) ; }
      // ======================================================================
    public: // setters
      // ======================================================================
      /// set k-parameter
      bool setPar       ( const unsigned int   k     ,
                          const double         value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int   k     ,
                          const double         value )
      { return ( k < m_pars.size() ) && setPar ( k , value ) ; }
      /// set (l,m)-parameter
      bool setPar       ( const unsigned short l     ,
                          const unsigned short m     ,
                          const unsigned short n     ,
                          const double         value ) 
      {
        const unsigned int k = index ( l , m , n ) ;
        return  ( k < m_pars.size() ) && setPar ( k , value )  ;
      }
      /// set (l,m)-parameter
      bool setParameter ( const unsigned short l     ,
                          const unsigned short m     ,
                          const unsigned short n     ,
                          const double         value )
      { return setPar   ( l , m  , n , value ) ; }
      // ======================================================================
    public: // getters
      // ======================================================================
      /// get (l,m,n)-parameter
      double  par       ( const unsigned short l ,
                          const unsigned short m ,
                          const unsigned short n ) const 
      { return par ( index ( l , m , n ) ) ; }
      /// get (l,m,n)-parameter
      double  parameter ( const unsigned short l ,
                          const unsigned short m ,
                          const unsigned short n ) const 
      { return par (  l , m , n ) ; }
      /// get k-parameter
      double  par       ( const unsigned int k ) const
      { return k < m_pars.size() ? m_pars[k] : 0.0 ; }
      /// get k-parameter
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      /// get all parameters at once
      const std::vector<double>& pars() const { return m_pars ; }
      // ======================================================================
    public : // convert (i,j,k) into single index 
      // ======================================================================
      /// convert (l,m,n)-index into single index k  
      unsigned int index ( const unsigned short l , 
                           const unsigned short m , 
                           const unsigned short n ) const 
      {
        return 
          m  > l   ?  index ( m , l , n )    :
          n  > m   ?  index ( l , n , m )    :
          l  > m_n ? -1                      : // NB!
          1u * l * ( l + 1 ) * ( l + 2 ) / 6 +
          1u * m * ( m + 1 )             / 2 + 
          n ;
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get the actual number of parameters
      std::size_t npars () const { return m_pars.size() ; }
      /// get lower edge
      double xmin () const { return m_xmin  ; }
      /// get upper edge
      double xmax () const { return m_xmax  ; }
      /// get lower edge
      double ymin () const { return xmin () ; }
      /// get upper edge
      double ymax () const { return xmax () ; }
      /// get lower edge
      double zmin () const { return xmin () ; }
      /// get upper edge
      double zmax () const { return xmax () ; }
      /// get the polynomial order (X)
      unsigned short nX () const { return m_n  ; }
      /// get the polynomial order (Y)
      unsigned short nY () const { return nX() ; }
      /// get the polynomial order (Y)
      unsigned short nZ () const { return nY() ; }
      // ======================================================================
    public:  // transformations
      // ======================================================================
      double x  ( const double tx ) const
      { return xmin ()  + ( xmax () - xmin () ) * tx ; }
      double y  ( const double ty ) const 
      { return ymin ()  + ( ymax () - ymin () ) * ty ; }
      double z  ( const double tz ) const 
      { return zmin ()  + ( zmax () - zmin () ) * tz ; }
      double tx ( const double x ) const
      { return  ( x - xmin () ) / ( xmax () - xmin () ) ; }
      double ty ( const double y ) const
      { return  ( y - ymin () ) / ( ymax () - ymin () ) ; }
      double tz ( const double z ) const
      { return  ( z - zmin () ) / ( zmax () - zmin () ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// simple  manipulations with polynoms: shift it!
      Bernstein3DSym& operator += ( const double a ) ;
      /// simple  manipulations with polynoms: shift it!
      Bernstein3DSym& operator -= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein3DSym& operator *= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein3DSym& operator /= ( const double a ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// negate it!
      Bernstein3DSym  operator-() const ;
      // ======================================================================
    public:
      // ======================================================================
      /// Sum of Bernstein polynomial and a constant
      Bernstein3DSym __add__   ( const double value ) const ;
      /// Sum of Bernstein polynomial and a constant
      Bernstein3DSym __radd__  ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein3DSym __mul__   ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein3DSym __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      Bernstein3DSym __sub__   ( const double value ) const ;
      /// Constant minus Bernstein polynomial
      Bernstein3DSym __rsub__  ( const double value ) const ;
      /// Divide Benrstein polynomial by a constant
      Bernstein3DSym __div__   ( const double value ) const ;
      /// Negate Bernstein polynomial
      Bernstein3DSym __neg__   () const ;
      // ======================================================================
    public: // general integration
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{low}}^{x_{high}} 
       *      \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}}
       *      \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param x     variable
       *  @param z     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double z    ,                          
                          const double xlow , const double xhigh ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param y     variable
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double z    ,
                          const double ylow , const double yhigh ) const 
      { return integrateX ( x , z , ylow , yhigh ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integrateZ ( const double x    ,
                          const double y    ,
                          const double zlow , const double zhigh ) const 
      { return integrateX ( x , y , zlow , zhigh ) ; }
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateXY ( const double z    ,                          
                           const double xlow , const double xhigh ,
                           const double ylow , const double yhigh ) const ;
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateXZ ( const double y    ,                          
                           const double xlow , const double xhigh ,
                           const double zlow , const double zhigh ) const 
      { return integrateXY (  y , xlow , xhigh , zlow , zhigh ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateYZ ( const double x    ,                          
                           const double ylow , const double yhigh ,
                           const double zlow , const double zhigh ) const 
      { return integrateXY (  x , ylow , yhigh , zlow , zhigh ) ; }
      // ======================================================================      
    public: // special cases
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[  x_{min} < x < x_{max}, 
       *       y_{min} < y < y_{max},
       *       z_{min} < z < z_{max}\f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       */
      double integrateX ( const double y , const double z ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       */
      double integrateY ( const double x , const double z ) const 
      { return integrateX ( x , z ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       */
      double integrateZ ( const double x , const double y ) const 
      { return integrateX ( x , y ) ; }
      // ======================================================================
    public: // special cases
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{min}}^{x_{max}}
       *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       */
      double integrateXY ( const double z    ) const ;
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{min}}^{x_{min}}
       *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       */
      double integrateXZ ( const double y    ) const { return integrateXY ( y ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{min}}^{y_{max}}
       *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       */
      double integrateYZ ( const double x    ) const { return integrateXY ( x ) ; }
      // ======================================================================
    public: // few helper functions to expose internals
      // ======================================================================
      /// evaluate the basic polynomials
      double basicX ( const unsigned short i , const double         x ) const
      { return ( i > nX() || x < xmin () || x < xmax () ) ? 0.0 : m_b [i](x) ; }
      /// evaluate the basic polynomials
      double basicY ( const unsigned short i , const double         y ) const
      { return ( i > nY() || y < ymin () || y < ymax () ) ? 0.0 : m_b [i](y) ; }
      /// evaluate the basic polynomials
      double basicZ ( const unsigned short i , const double         z ) const
      { return ( i > nZ() || z < zmin () || z < zmax () ) ? 0.0 : m_b [i](z) ; }
      /// expose some internals
      const Bernstein& basicX ( const unsigned short i ) const { return m_b [i] ; }
      /// expose some internals
      const Bernstein& basicY ( const unsigned short i ) const { return m_b [i] ; }
      /// expose some internals
      const Bernstein& basicZ ( const unsigned short i ) const { return m_b [i] ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap two Bernstein polynomials   
      void swap ( Bernstein3DSym& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the tag value 
      std::size_t tag () const ; // get the tag value 
      // ======================================================================
    private: // helper functions to make calculations
      // ======================================================================
      /// helper function to make calculations
      double calculate ( const std::vector<double>& fx , 
                         const std::vector<double>& fy , 
                         const std::vector<double>& fz ) const ;
      // ======================================================================
    private:
      // ======================================================================
      // polynom order in x-dimension
      unsigned short m_n  ; // polynom order in x-dimension
      /// the list of parameters
      std::vector<double>  m_pars ;                // the list of parameters
      /// the left edge of interval
      double m_xmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_xmax  ;                             // the right edge of interval
      // ======================================================================
    private:
      // ======================================================================
      ///  vectors of basic  Bernstein polynomials
      typedef std::vector<Bernstein>  VB ;
      ///  vector  of basic  Bernstein polynomials
      VB m_b ; //  vector  of basic  Bernstein polynomials
      // ======================================================================
    } ;
    // ========================================================================
    ///  Bernstein plus      constant
    inline Bernstein3DSym operator+( const Bernstein3DSym& p , const double v )
    { return Bernstein3DSym ( p ) += v ; } //  Bernstein plus constant
    ///  Bernstein multiply  constant
    inline Bernstein3DSym operator*( const Bernstein3DSym& p , const double v )
    { return Bernstein3DSym ( p ) *= v ; } //  Bernstein plus constant
    ///  Bernstein minus constant
    inline Bernstein3DSym operator-( const Bernstein3DSym& p , const double v )
    { return Bernstein3DSym ( p ) -= v ; } //  Bernstein plus constant
    ///  Bernstein divide constant
    inline Bernstein3DSym operator/( const Bernstein3DSym& p , const double v )
    { return Bernstein3DSym ( p ) /= v ; } //  Bernstein plus constant
    ///  Constant plus  Bernstein
    inline Bernstein3DSym operator+( const double v , const Bernstein3DSym& p ) { return p +   v  ; }
    ///  Constant times Bernstein
    inline Bernstein3DSym operator*( const double v , const Bernstein3DSym& p ) { return p *   v  ; }
    ///  Constant minus Bernstein
    inline Bernstein3DSym operator-( const double v , const Bernstein3DSym& p ) { return v + (-p) ; }
     // ========================================================================
    /// swap two Bernstein polynomials   
    inline  void swap ( Bernstein3DSym& a , Bernstein3DSym& b ) { a.swap ( b ) ;  }
    // ========================================================================
    /** @class Bernstein3DMix
     *  Generic "partially  symmetrized" 
     *  3D-polynomial of order N*N*Nz  defined as 
     *  \f[ P(x,y,z) = \sum_{i,j,k} a_{ijk}B^{n}_i(x) B^{n}_j(y) B^{n_z}_k(z)\f] 
     *  where \f[ P(x,y,z) = P(y,x,z)\f] 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-11-14
     */
    class Bernstein3DMix
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Bernstein3DMix ( const unsigned short       N     =  1 ,
                       const unsigned short       Nz    =  1 ,
                       const double               xmin  =  0 ,
                       const double               xmax  =  1 ,
                       const double               zmin  =  0 ,
                       const double               zmax  =  1 ) ;
      // ======================================================================
      /// copy constructor 
      Bernstein3DMix ( const Bernstein3DMix&  right ) = default ;   
      /// move constructor 
      Bernstein3DMix (       Bernstein3DMix&& right ) ;
      // ======================================================================
      /// from symmetric variant 
      explicit Bernstein3DMix ( const Bernstein3DSym&  right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double evaluate    ( const double x ,
                           const double y , 
                           const double z ) const ;
      // ======================================================================
      /// get the value
      double operator () ( const double x ,
                           const double y , 
                           const double z ) const 
      { return evaluate ( x ,   y , z ) ; }
      // ======================================================================
    public: // setters
      // ======================================================================
      /// set k-parameter
      bool setPar       ( const unsigned int   k     ,
                          const double         value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int   k     ,
                          const double         value )
      { return ( k < m_pars.size() ) && setPar ( k , value ) ; }
      /// set (l,m)-parameter
      bool setPar       ( const unsigned short l     ,
                          const unsigned short m     ,
                          const unsigned short n     ,
                          const double         value ) ;
      /// set (l,m)-parameter
      bool setParameter ( const unsigned short l     ,
                          const unsigned short m     ,
                          const unsigned short n     ,
                          const double         value )
      { return setPar   ( l , m  , n , value ) ; }
      // ======================================================================
    public: // getters
      // ======================================================================
      /// get (l,m,n)-parameter
      double  par       ( const unsigned short l ,
                          const unsigned short m ,
                          const unsigned short n ) const 
      { return par ( index ( l , m , n ) ) ; }
      /// get (l,m,n)-parameter
      double  parameter ( const unsigned short l ,
                          const unsigned short m ,
                          const unsigned short n ) const 
      { return par (  l , m , n ) ; }
      /// get k-parameter
      double  par       ( const unsigned int k ) const
      { return k < m_pars.size() ? m_pars[k] : 0.0 ; }
      /// get k-parameter
      double  parameter ( const unsigned int k ) const 
      { return par ( k ) ; }
      /// get all parameters at once
      const std::vector<double>& pars() const { return m_pars ; }
      // ======================================================================
    public:  // convert (i,j,k) into single index 
      // ======================================================================
      /// convert (l,m,n)-index into single index k  
      unsigned int index ( const unsigned short l , 
                           const unsigned short m , 
                           const unsigned short n ) const 
      {
        return 
          m > l    ?   index ( m , l , n )    :
          l > m_n  ?  -1                      : // NB!
          n > m_nz ?  -1                      : // NB!
          ( 1u * l * ( l + 1 ) / 2 + m ) * ( m_nz + 1 ) + n ;
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get the actual number of parameters
      std::size_t npars () const { return m_pars.size() ; }
      /// get lower edge
      double xmin () const { return m_xmin  ; }
      /// get upper edge
      double xmax () const { return m_xmax  ; }
      /// get lower edge
      double ymin () const { return xmin () ; }
      /// get upper edge
      double ymax () const { return xmax () ; }
      /// get lower edge
      double zmin () const { return m_zmin  ; }
      /// get upper edge
      double zmax () const { return m_zmax ; }
      /// get the polynomial order (X)
      unsigned short nX () const { return m_n  ; }
      /// get the polynomial order (Y)
      unsigned short nY () const { return nX() ; }
      /// get the polynomial order (Y)
      unsigned short nZ () const { return m_nz ; }
      // ======================================================================
    public:  // transformations
      // ======================================================================
      double x  ( const double tx ) const
      { return xmin ()  + ( xmax () - xmin () ) * tx ; }
      double y  ( const double ty ) const 
      { return ymin ()  + ( ymax () - ymin () ) * ty ; }
      double z  ( const double tz ) const
      { return zmin ()  + ( zmax () - zmin () ) * tz ; }
      double tx ( const double x ) const
      { return  ( x - xmin () ) / ( xmax () - xmin () ) ; }
      double ty ( const double y ) const 
      { return  ( y - ymin () ) / ( ymax () - ymin () ) ; }
      double tz ( const double z ) const 
      { return  ( z - zmin () ) / ( zmax () - zmin () ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// simple  manipulations with polynoms: shift it!
      Bernstein3DMix& operator += ( const double a ) ;
      /// simple  manipulations with polynoms: shift it!
      Bernstein3DMix& operator -= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein3DMix& operator *= ( const double a ) ;
      /// simple  manipulations with polynoms: scale it!
      Bernstein3DMix& operator /= ( const double a ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// negate it!
      Bernstein3DMix  operator-() const ;
      // ======================================================================
    public:
      // ======================================================================
      /// Sum of Bernstein polynomial and a constant
      Bernstein3DMix __add__   ( const double value ) const ;
      /// Sum of Bernstein polynomial and a constant
      Bernstein3DMix __radd__  ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein3DMix __mul__   ( const double value ) const ;
      /// Product of Bernstein polynomial and a constant
      Bernstein3DMix __rmul__  ( const double value ) const ;
      /// Subtract a constant from Benrstein polynomial
      Bernstein3DMix __sub__   ( const double value ) const ;
      /// Constant minus Bernstein polynomial
      Bernstein3DMix __rsub__  ( const double value ) const ;
      /// Divide Benrstein polynomial by a constant
      Bernstein3DMix __div__   ( const double value ) const ;
      /// Negate Bernstein polynomial
      Bernstein3DMix __neg__   () const ;
      // ======================================================================
    public: // general integration
      // ======================================================================
      /** get the integral over 2D-region
       *  \f[ \int_{x_{low}}^{x_{high}} 
       *      \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}}
       *      \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param x     variable
       *  @param z     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double z    ,                          
                          const double xlow , const double xhigh ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param y     variable
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double z    ,
                          const double ylow , const double yhigh ) const 
      { return integrateX ( x , z , ylow  , yhigh ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integrateZ ( const double x    ,
                          const double y    ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateXY ( const double z    ,                          
                           const double xlow , const double xhigh ,
                           const double ylow , const double yhigh ) const ;
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateXZ ( const double y    ,                          
                           const double xlow , const double xhigh ,
                           const double zlow , const double zhigh ) const ;      
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateYZ ( const double x    ,                          
                           const double ylow , const double yhigh ,
                           const double zlow , const double zhigh ) const 
      { return integrateXZ ( x , ylow , yhigh , zlow , zhigh ) ; }
      // ======================================================================      
    public: // special cases
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[  x_{min} < x < x_{max}, 
       *       y_{min} < y < y_{max},
       *       z_{min} < z < z_{max}\f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       */
      double integrateX ( const double y , const double z ) const ;
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       */
      double integrateY ( const double x , const double z ) const 
      { return integrateX ( x , z ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       */
      double integrateZ ( const double x , const double y ) const ;
      // ======================================================================
    public: // special cases
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{min}}^{x_{max}}
       *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       */
      double integrateXY ( const double z    ) const ;
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{min}}^{x_{min}}
       *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       */
      double integrateXZ ( const double y    ) const ;
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{min}}^{y_{max}}
       *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       */
      double integrateYZ ( const double x    ) const ;
      // ======================================================================
    public: // few helper functions to expose internals
      // ======================================================================
      /// evaluate the basic polynomials
      double basicX ( const unsigned short i , const double         x ) const
      { return ( i > nX() || x < xmin () || x < xmax () ) ? 0.0 : m_b [i](x) ; }
      /// evaluate the basic polynomials
      double basicY ( const unsigned short i , const double         y ) const
      { return ( i > nY() || y < ymin () || y < ymax () ) ? 0.0 : m_b [i](y) ; }
      /// evaluate the basic polynomials
      double basicZ ( const unsigned short i , const double         z ) const
      { return ( i > m_nz || z < m_zmin || z < m_zmax ) ? 0.0 : m_bz[i](z) ; }
      /// expose some internals
      const Bernstein& basicX ( const unsigned short i ) const { return m_b [i] ; }
      /// expose some internals
      const Bernstein& basicY ( const unsigned short i ) const { return m_b [i] ; }
      /// expose some internals
      const Bernstein& basicZ ( const unsigned short i ) const { return m_bz[i] ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap two Bernstein polynomials   
      void swap ( Bernstein3DMix& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the tag value 
      std::size_t tag () const ; // get the tag value 
      // ======================================================================
    private: // helper functions to make calculations
      // ======================================================================
      /// helper function to make calculations
      double calculate ( const std::vector<double>& fx , 
                         const std::vector<double>& fy , 
                         const std::vector<double>& fz ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// polynom order in x,y-dimensions
      unsigned short m_n  ; // polynom order in x,y-dimensions
      /// polynom order in z-dimension
      unsigned short m_nz ; // polynom order in z-dimension
      /// the list of parameters
      std::vector<double>  m_pars ;                // the list of parameters
      /// the left edge of interval
      double m_xmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_xmax  ;                             // the right edge of interval
      /// the left edge of interval
      double m_zmin  ;                             // the left edge of interval
      /// the right edge of interval
      double m_zmax  ;                             // the right edge of interval
      // ======================================================================
    private:
      // ======================================================================
      ///  vectors of basic  Bernstein polynomials
      typedef std::vector<Bernstein>  VB ;
      ///  vector  of basic  Bernstein polynomials
      VB m_b  ; //  vector  of basic  Bernstein polynomials
      ///  vector  of basic  Bernstein polynomials
      VB m_bz ; //  vector  of basic  Bernstein polynomials
      // ======================================================================
    } ;
    // ========================================================================
    ///  Bernstein plus      constant
    inline Bernstein3DMix operator+( const Bernstein3DMix& p , const double v )
    { return Bernstein3DMix ( p ) += v ; } //  Bernstein plus constant
    ///  Bernstein multiply  constant
    inline Bernstein3DMix operator*( const Bernstein3DMix& p , const double v )
    { return Bernstein3DMix ( p ) *= v ; } //  Bernstein plus constant
    ///  Bernstein minus constant
    inline Bernstein3DMix operator-( const Bernstein3DMix& p , const double v )
    { return Bernstein3DMix ( p ) -= v ; } //  Bernstein plus constant
    ///  Bernstein divide constant
    inline Bernstein3DMix operator/( const Bernstein3DMix& p , const double v )
    { return Bernstein3DMix ( p ) /= v ; } //  Bernstein plus constant
    ///  Constant plus  Bernstein
    inline Bernstein3DMix operator+( const double v , const Bernstein3DMix& p ) { return p +   v  ; }
    ///  Constant times Bernstein
    inline Bernstein3DMix operator*( const double v , const Bernstein3DMix& p ) { return p *   v  ; }
    ///  Constant minus Bernstein
    inline Bernstein3DMix operator-( const double v , const Bernstein3DMix& p ) { return v + (-p) ; }
     // ========================================================================
    /// swap two Bernstein polynomials   
    inline  void swap ( Bernstein3DMix& a , Bernstein3DMix& b ) { a.swap ( b ) ;  }
    // ========================================================================
    /** @class Positive3D
     *  The 3D-polynomial of order Nx*Ny*Nz, that is constrained 
     *  to be non-negative over the  defined range      
     *  \f[  P(x,y,z) = \sum_{i,j,k} a_{ijk}B^{n_x}_i(x) B^{n_y}_j(y) B^{n_z}_k(z)\f] 
     *  where all coefficients \f$a_{ijk}\f$ are non-negative and 
     *  \f$ \sum_{i,j,k} a_{ijk}=1 \f$ 
     *  @author Vanya BELYAEV Ivan.Belayev@itep.ru
     *  @date 2017-11-14
     */
    class Positive3D 
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Positive3D ( const unsigned short       Nx    =  1 ,
                   const unsigned short       Ny    =  1 ,
                   const unsigned short       Nz    =  1 ,
                   const double               xmin  =  0 ,
                   const double               xmax  =  1 ,
                   const double               ymin  =  0 ,
                   const double               ymax  =  1 ,
                   const double               zmin  =  0 ,
                   const double               zmax  =  1 ) ;
      // ======================================================================
      /// copy constructor 
      Positive3D ( const Positive3D&  right ) = default ;
      /// move constructor 
      Positive3D (       Positive3D&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double evaluate    ( const double x , 
                           const double y , 
                           const double z ) const
      { return m_bernstein ( x , y , z ) ; }
      // ======================================================================
      /// get the value
      double operator () ( const double x , 
                           const double y , 
                           const double z ) const
      { return evaluate  ( x , y , z ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value )
      { return setPar   ( k , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const 
      { return m_sphere.phase ( k ) ; }        
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      // ======================================================================
      /// get all parameters (phases on sphere)
      const std::vector<double>& pars  () const { return m_sphere   .pars () ; }
      /// get bernstein coefficients
      const std::vector<double>& bpars () const { return m_bernstein.pars () ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin () const { return m_bernstein.xmin () ; }
      double         xmax () const { return m_bernstein.xmax () ; }
      double         ymin () const { return m_bernstein.ymin () ; }
      double         ymax () const { return m_bernstein.ymax () ; }
      double         zmin () const { return m_bernstein.zmin () ; }
      double         zmax () const { return m_bernstein.zmax () ; }
      // polynom order
      unsigned short nX   () const { return m_bernstein.nX   () ; }
      unsigned short nY   () const { return m_bernstein.nY   () ; }
      unsigned short nZ   () const { return m_bernstein.nZ   () ; }
      // ======================================================================
    public:
      // ======================================================================
      // transform variables
      double tx ( const double  x ) const { return m_bernstein.tx (  x ) ; }
      double ty ( const double  y ) const { return m_bernstein.ty (  y ) ; }
      double tz ( const double  z ) const { return m_bernstein.tz (  z ) ; }
      double  x ( const double tx ) const { return m_bernstein. x ( tx ) ; }
      double  y ( const double ty ) const { return m_bernstein. y ( ty ) ; }
      double  z ( const double tz ) const { return m_bernstein. z ( tz ) ; }
      // ======================================================================
    public: // integrals 
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[ \int_{x_{low}}^{x_{high}} 
       *      \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}}
       *      \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
    public: //  partial integrals 
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param x     variable
       *  @param z     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double z    ,                          
                          const double xlow , const double xhigh ) const 
      { return m_bernstein.integrateX ( y ,  z , xlow , xhigh ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param y     variable
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double z    ,
                          const double ylow , const double yhigh ) const 
      { return m_bernstein.integrateY ( x ,  z , ylow , yhigh ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integrateZ ( const double x    ,
                          const double y    ,
                          const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateZ ( x ,  y , zlow , zhigh ) ; }
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateXY ( const double z    ,                          
                           const double xlow , const double xhigh ,
                           const double ylow , const double yhigh ) const 
      { return m_bernstein.integrateXY ( z , xlow , xhigh , ylow , yhigh ) ; }
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateXZ ( const double y    ,                          
                           const double xlow , const double xhigh ,
                           const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateXZ ( y , xlow , xhigh , zlow , zhigh ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateYZ ( const double x    ,                          
                           const double ylow , const double yhigh ,
                           const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateYZ ( x , ylow , yhigh , zlow , zhigh ) ; }
      // ======================================================================      
    public: // Integrals: special cases
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[  x_{min} < x < x_{max}, 
       *       y_{min} < y < y_{max},
       *       z_{min} < z < z_{max} \f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       */
      double integrateX ( const double y , 
                          const double z ) const 
      { return m_bernstein.integrateX ( y , z ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       */
      double integrateY ( const double x , 
                          const double z ) const 
      { return m_bernstein.integrateY ( x , z ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       */
      double integrateZ ( const double x , 
                          const double y ) const 
      { return m_bernstein.integrateZ ( x , y ) ; }
      // ======================================================================
    public: // Integrals: special cases
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{min}}^{x_{max}}
       *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       */
      double integrateXY ( const double z    ) const 
      { return m_bernstein.integrateXY ( z ) ; }
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{min}}^{x_{min}}
       *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       */
      double integrateXZ ( const double y    ) const 
      { return m_bernstein.integrateXZ ( y ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{min}}^{y_{max}}
       *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       */
      double integrateYZ ( const double x    ) const 
      { return m_bernstein.integrateYZ ( x ) ; }
      // ======================================================================
    public: // ingeredients
      // =====================================================================
      // get the bernstein polinomial in 2D
      const  Ostap::Math::Bernstein3D& bernstein () const
      { return m_bernstein ; }
      /// get the parameter sphere
      const  Ostap::Math::NSphere&     sphere    () const
      { return m_sphere ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap two Bernstein polynomials   
      void swap ( Positive3D& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the tag value 
      std::size_t tag () const { return m_bernstein.tag () ; }
      // ======================================================================
    private:
      // ======================================================================
      /// update bernstein coefficients
      bool updateBernstein () ;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual bernstein polynomial
      Ostap::Math::Bernstein3D m_bernstein ; // the actual bernstein polynomial
      /// the external parameter sphere
      Ostap::Math::NSphere     m_sphere    ;
      // ======================================================================
    } ;
    // ========================================================================
    /// swap two Bernstein polynomials   
    inline  void swap ( Positive3D& a , Positive3D& b ) { a.swap ( b ) ;  }
    // ========================================================================
    /** @class Positive3DSym
     *  The 3D-polynomial of order N*N*N, that is constrained 
     *  to be non-negative ans symmetric over the  defined range      
     *  \f[  P(x,y,z) = \sum_{i,j,k} a_{ijk}B^{n}_i(x) B^{n}_j(y) B^{n}_k(z)\f] 
     *  where all coefficients \f$a_{ijk}\f$ are:
     * - non-negative: \f$ a_{ijk}\ge0 \f$
     * - symmetric: \f$ a_{ijk}=a_{jik}=a_{ikj}\f$
     * - constrainted: \f$ \sum_{i,j,k} a_{ijk}=1 \f$ 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-11-14
     */
    class Positive3DSym
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Positive3DSym ( const unsigned short       N     =  1 ,
                      const double               xmin  =  0 ,
                      const double               xmax  =  1 );
      // ======================================================================
      /// copy constructor 
      Positive3DSym ( const Positive3DSym&  right ) = default ;
      /// move constructor 
      Positive3DSym (       Positive3DSym&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double evaluate    ( const double x , 
                           const double y , 
                           const double z ) const
      { return m_bernstein ( x , y , z ) ; }
      // ======================================================================
      /// get the value
      double operator () ( const double x , 
                           const double y , 
                           const double z ) const
      { return evaluate  ( x , y , z ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value )
      { return setPar   ( k , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const 
      { return m_sphere.phase ( k ) ; }        
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      // ======================================================================
      /// get all parameters (phases on sphere)
      const std::vector<double>& pars  () const { return m_sphere   .pars () ; }
      /// get bernstein coefficients
      const std::vector<double>& bpars () const { return m_bernstein.pars () ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin () const { return m_bernstein.xmin () ; }
      double         xmax () const { return m_bernstein.xmax () ; }
      double         ymin () const { return m_bernstein.ymin () ; }
      double         ymax () const { return m_bernstein.ymax () ; }
      double         zmin () const { return m_bernstein.zmin () ; }
      double         zmax () const { return m_bernstein.zmax () ; }
      // polynom order
      unsigned short nX   () const { return m_bernstein.nX   () ; }
      unsigned short nY   () const { return m_bernstein.nY   () ; }
      unsigned short nZ   () const { return m_bernstein.nZ   () ; }
      // ======================================================================
    public:
      // ======================================================================
      // transform variables
      double tx ( const double  x ) const { return m_bernstein.tx (  x ) ; }
      double ty ( const double  y ) const { return m_bernstein.ty (  y ) ; }
      double tz ( const double  z ) const { return m_bernstein.tz (  z ) ; }
      double  x ( const double tx ) const { return m_bernstein. x ( tx ) ; }
      double  y ( const double ty ) const { return m_bernstein. y ( ty ) ; }
      double  z ( const double tz ) const { return m_bernstein. z ( tz ) ; }
      // ======================================================================
    public: // integrals 
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[ \int_{x_{low}}^{x_{high}} 
       *      \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}}
       *      \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
    public: //  partial integrals 
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateX ( const double y    ,
                          const double z    ,                          
                          const double xlow , const double xhigh ) const 
      { return m_bernstein.integrateX ( y ,  z , xlow , xhigh ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateY ( const double x    ,
                          const double z    ,
                          const double ylow , const double yhigh ) const 
      { return m_bernstein.integrateY ( x ,  z , ylow , yhigh ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integrateZ ( const double x    ,
                          const double y    ,
                          const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateZ ( x ,  y , zlow , zhigh ) ; }
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateXY ( const double z    ,                          
                           const double xlow , const double xhigh ,
                           const double ylow , const double yhigh ) const 
      { return m_bernstein.integrateXY ( z , xlow , xhigh , ylow , yhigh ) ; }
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateXZ ( const double y    ,                          
                           const double xlow , const double xhigh ,
                           const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateXZ ( y , xlow , xhigh , zlow , zhigh ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateYZ ( const double x    ,                          
                           const double ylow , const double yhigh ,
                           const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateYZ ( x , ylow , yhigh , zlow , zhigh ) ; }
      // ======================================================================      
    public: // Integrals: special cases
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[  x_{min} < x < x_{max}, 
       *       y_{min} < y < y_{max},
       *       z_{min} < z < z_{max} \f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       */
      double integrateX ( const double y , 
                          const double z ) const 
      { return m_bernstein.integrateX ( y , z ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       */
      double integrateY ( const double x , 
                          const double z ) const 
      { return m_bernstein.integrateY ( x , z ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       */
      double integrateZ ( const double x , 
                          const double y ) const 
      { return m_bernstein.integrateZ ( x , y ) ; }
      // ======================================================================
    public: // Integrals: special cases
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{min}}^{x_{max}}
       *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       */
      double integrateXY ( const double z    ) const 
      { return m_bernstein.integrateXY ( z ) ; }
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{min}}^{x_{min}}
       *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       */
      double integrateXZ ( const double y    ) const 
      { return m_bernstein.integrateXZ ( y ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{min}}^{y_{max}}
       *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       */
      double integrateYZ ( const double x    ) const 
      { return m_bernstein.integrateYZ ( x ) ; }
      // ======================================================================
    public: // ingeredients
      // =====================================================================
      // get the bernstein polinomial in 2D
      const  Ostap::Math::Bernstein3DSym& bernstein () const 
      { return m_bernstein ; }
      /// get the parameter sphere
      const  Ostap::Math::NSphere&        sphere    () const
      { return m_sphere ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap two Bernstein polynomials   
      void swap ( Positive3DSym& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the tag value 
      std::size_t tag () const { return m_bernstein.tag () ; }
      // ======================================================================
    private:
      // ======================================================================
      /// update bernstein coefficients
      bool updateBernstein () ;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual bernstein polynomial
      Ostap::Math::Bernstein3DSym m_bernstein ; // the actual bernstein polynomial
      /// the external parameter sphere
      Ostap::Math::NSphere        m_sphere    ;
      // ======================================================================
    } ;
    // ========================================================================
    /// swap two Bernstein polynomials   
    inline  void swap ( Positive3DSym& a , Positive3DSym& b ) { a.swap ( b ) ;  }
    // ========================================================================
    /** @class Positive3DMix
     *  The 3D-polynomial of order N*N*Nz, that is constrained 
     *  to be non-negative and symmetric for \f$ x \leftrightarrow y\f$ interchange 
     *  over the  defined range      
     *  \f[  P(x,y,z) = \sum_{i,j,k} a_{ijk}B^{n}_i(x) B^{n}_j(y) B^{n_z}_k(z)\f] 
     *  where all coefficients \f$ a_{ijk} \f$ are:
     * - non-negative: \f$ a_{ijk}\ge0 \f$
     * - symmetric for \f$ x \leftrightarrow y\f$ interchange: \f$ a_{ijk}=a_{jik}\f$
     * - constrainted: \f$ \sum_{i,j,k} a_{ijk}=1 \f$ 
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2017-11-14
     */
    class Positive3DMix
    {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the order
      Positive3DMix ( const unsigned short       N     =  1 ,
                      const unsigned short       Nz    =  1 ,
                      const double               xmin  =  0 ,
                      const double               xmax  =  1 ,
                      const double               zmin  =  0 ,
                      const double               zmax  =  1 );
      // ======================================================================
      /// copy constructor 
      Positive3DMix ( const Positive3DMix&  right ) = default ;
      /// move constructor 
      Positive3DMix (       Positive3DMix&& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the value
      double evaluate    ( const double x , 
                           const double y , 
                           const double z ) const
      { return m_bernstein ( x , y , z ) ; }
      // ======================================================================
      /// get the value
      double operator () ( const double x , 
                           const double y , 
                           const double z ) const
      { return evaluate  ( x , y , z ) ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get number of parameters
      std::size_t npars () const { return m_sphere.nPhi () ; }
      /// set k-parameter
      bool setPar       ( const unsigned int k , const double value ) ;
      /// set k-parameter
      bool setParameter ( const unsigned int k , const double value )
      { return setPar   ( k , value ) ; }
      /// get the parameter value
      double  par       ( const unsigned int k ) const 
      { return m_sphere.phase ( k ) ; }        
      /// get the parameter value
      double  parameter ( const unsigned int k ) const { return par ( k ) ; }
      // ======================================================================
      /// get all parameters (phases on sphere)
      const std::vector<double>& pars  () const { return m_sphere   .pars () ; }
      /// get bernstein coefficients
      const std::vector<double>& bpars () const { return m_bernstein.pars () ; }
      // ======================================================================
    public:
      // ======================================================================
      /// get lower/upper edges
      double         xmin () const { return m_bernstein.xmin () ; }
      double         xmax () const { return m_bernstein.xmax () ; }
      double         ymin () const { return m_bernstein.ymin () ; }
      double         ymax () const { return m_bernstein.ymax () ; }
      double         zmin () const { return m_bernstein.zmin () ; }
      double         zmax () const { return m_bernstein.zmax () ; }
      // polynom order
      unsigned short nX   () const { return m_bernstein.nX   () ; }
      unsigned short nY   () const { return m_bernstein.nY   () ; }
      unsigned short nZ   () const { return m_bernstein.nZ   () ; }
      // ======================================================================
    public:
      // ======================================================================
      // transform variables
      double tx ( const double  x ) const { return m_bernstein.tx (  x ) ; }
      double ty ( const double  y ) const { return m_bernstein.ty (  y ) ; }
      double tz ( const double  z ) const { return m_bernstein.tz (  z ) ; }
      double  x ( const double tx ) const { return m_bernstein. x ( tx ) ; }
      double  y ( const double ty ) const { return m_bernstein. y ( ty ) ; }
      double  z ( const double tz ) const { return m_bernstein. z ( tz ) ; }
      // ======================================================================
    public: // integrals 
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[ \int_{x_{low}}^{x_{high}} 
       *      \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}}
       *      \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\mathrm{d}z\f]
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integral   ( const double xlow , const double xhigh ,
                          const double ylow , const double yhigh ,
                          const double zlow , const double zhigh ) const ;
      // ======================================================================
    public: //  partial integrals 
      // ======================================================================
      /** integral over x-dimension
       *  \f[ \int_{x_{low}}^{x_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param x     variable
       *  @param z     variable
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateX ( const double y    ,
                          const double z    ,                          
                          const double xlow , const double xhigh ) const 
      { return m_bernstein.integrateX ( y ,  z , xlow , xhigh ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param y     variable
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       */
      double integrateY ( const double x    ,
                          const double z    ,
                          const double ylow , const double yhigh ) const 
      { return m_bernstein.integrateY ( x ,  z , ylow , yhigh ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       *  @param zlow  low  edge in z
       *  @param zhigh high edge in z
       */
      double integrateZ ( const double x    ,
                          const double y    ,
                          const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateZ ( x ,  y , zlow , zhigh ) ; }
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{y_{low}}^{y_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param ylow  low  edge in y
       *  @param yhigh high edge in y
       */
      double integrateXY ( const double z    ,                          
                           const double xlow , const double xhigh ,
                           const double ylow , const double yhigh ) const 
      { return m_bernstein.integrateXY ( z , xlow , xhigh , ylow , yhigh ) ; }
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{low}}^{x_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       *  @param xlow  low  edge in x
       *  @param xhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateXZ ( const double y    ,                          
                           const double xlow , const double xhigh ,
                           const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateXZ ( y , xlow , xhigh , zlow , zhigh ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{low}}^{y_{high}}
       *      \int_{z_{low}}^{z_{high}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       *  @param ylow  low  edge in x
       *  @param yhigh high edge in x
       *  @param zlow  low  edge in y
       *  @param zhigh high edge in y
       */
      double integrateYZ ( const double x    ,                          
                           const double ylow , const double yhigh ,
                           const double zlow , const double zhigh ) const 
      { return m_bernstein.integrateYZ ( x , ylow , yhigh , zlow , zhigh ) ; }
      // ======================================================================      
    public: // Integrals: special cases
      // ======================================================================
      /** get the integral over 3D-region
       *  \f[  x_{min} < x < x_{max}, 
       *       y_{min} < y < y_{max},
       *       z_{min} < z < z_{max} \f]
       */
      double integral   () const ;
      /** integral over x-dimension
       *  \f[ \int_{x_{min}}^{x_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\f]
       *  @param y     variable
       *  @param z     variable
       */
      double integrateX ( const double y , 
                          const double z ) const 
      { return m_bernstein.integrateX ( y , z ) ; }
      /** integral over y-dimension
       *  \f[ \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\f]
       *  @param x     variable
       *  @param z     variable
       */
      double integrateY ( const double x , 
                          const double z ) const 
      { return m_bernstein.integrateY ( x , z ) ; }
      /** integral over z-dimension
       *  \f[ \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}z\f]
       *  @param x     variable
       *  @param y     variable
       */
      double integrateZ ( const double x , 
                          const double y ) const 
      { return m_bernstein.integrateZ ( x , y ) ; }
      // ======================================================================
    public: // Integrals: special cases
      // ======================================================================
      /** integral over x&y-dimensions
       *  \f[ \int_{x_{min}}^{x_{max}}
       *      \int_{y_{min}}^{y_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}y\f]
       *  @param z     variable
       */
      double integrateXY ( const double z    ) const 
      { return m_bernstein.integrateXY ( z ) ; }
      /** integral over x&z-dimensions
       *  \f[ \int_{x_{min}}^{x_{min}}
       *      \int_{z_{max}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}x\mathrm{d}z\f]
       *  @param y     variable
       */
      double integrateXZ ( const double y    ) const 
      { return m_bernstein.integrateXZ ( y ) ; }
      /** integral over y&z-dimensions
       *  \f[ \int_{y_{min}}^{y_{max}}
       *      \int_{z_{min}}^{z_{max}} \mathcal{B}(x,y,z) \mathrm{d}y\mathrm{d}z\f]
       *  @param x     variable
       */
      double integrateYZ ( const double x    ) const 
      { return m_bernstein.integrateYZ ( x ) ; }
      // ======================================================================
    public: // ingeredients
      // =====================================================================
      // get the bernstein polinomial in 2D
      const  Ostap::Math::Bernstein3DMix& bernstein () const 
      { return m_bernstein ; }
      /// get the parameter sphere
      const  Ostap::Math::NSphere&        sphere    () const
      { return m_sphere ; }
      // ======================================================================
    public:
      // ======================================================================
      /// swap two Bernstein polynomials   
      void swap ( Positive3DMix& right ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the tag value 
      std::size_t tag () const { return m_bernstein.tag () ; }
      // ======================================================================
    private:
      // ======================================================================
      /// update bernstein coefficients
      bool updateBernstein () ;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual bernstein polynomial
      Ostap::Math::Bernstein3DMix m_bernstein ; // the actual bernstein polynomial
      /// the external parameter sphere
      Ostap::Math::NSphere        m_sphere    ;
      // ======================================================================
    } ;
    // ========================================================================
    /// swap two Bernstein polynomials   
    inline  void swap ( Positive3DMix& a , Positive3DMix& b ) { a.swap ( b ) ;  }
    // ========================================================================
  } //                                         The end of namespace Ostap::Math
  // ==========================================================================
} //                                                The end of namespace  Gaudi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // OSTAP_BERNSTEIN3D_H
// ============================================================================
