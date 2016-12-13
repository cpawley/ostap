#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
## @file
#  Few utilities to simplify linear algebra manipulations 
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
# =============================================================================
"""Few utilities to simplify linear algebra manipulations 
"""
# =============================================================================
__author__  = "Vanya BELYAEV Ivan.Belyaev@nikhef.nl"
__date__    = "2009-09-12"
__version__ = ""
# =============================================================================
__all__     = () ## nothing to be imported !
# =============================================================================
import ROOT, cppyy 
# =============================================================================
# logging 
# =============================================================================
from ostap.logger.logger import getLogger
if '__main__' ==  __name__ : logger = getLogger ( 'ostap.math.linalg' )
else                       : logger = getLogger ( __name__            )
# =============================================================================

cpp   = cppyy.gbl

## C++ namespace std
std   = cpp.std

## C++ namespace Ostap 
Ostap = cpp.Ostap

## ROOT::Math namespace
_RM = ROOT.ROOT.Math


a = Ostap.Math.ValueWithError()

# =============================================================================
## try to pickup the vector
@staticmethod
def _vector_ ( i , typ = 'double' ) :
    """Pick up the vector of corresponding size
    >>> V3   = Ostap.Math.Vector(3)
    >>> vct  = V3 ()
    """
    v = _RM.SVector ( typ , i )
    return deco_vector ( v ) 

# =============================================================================
## try to pickup the matrix
@staticmethod
def _matrix_ ( i , j , typ = 'double' ) :
    """Pick up the matrix of corresponding size
    >>> M3x4   = Ostap.Math.Matrix(3,4)
    >>> matrix = M3x4 ()    
    """
    m = _RM.SMatrix ( "%s,%d,%d" % ( typ , i , j ) )
    return deco_matrix( m )  

# =============================================================================
## try to pickup the symmeric matrix
@staticmethod
def _sym_matrix_ ( i , typ = 'double' ) :
    """Pick up the symmetric matrix of corresponding size
    >>> SymM3  = Ostap.Math.SymMatrix(3)
    >>> matrix = SymM3 ()
    """
    m = _RM.SMatrix('%s,%d,%d,ROOT::Math::MatRepSym<%s,%d>' %  ( typ , i , i , typ , i ) )
    return deco_symmatrix  ( m ) 

Ostap.Vector         =     _vector_
Ostap.Math.Vector    =     _vector_
Ostap.Matrix         =     _matrix_
Ostap.Math.Matrix    =     _matrix_
Ostap.SymMatrix      = _sym_matrix_
Ostap.Math.SymMatrix = _sym_matrix_

# =============================================================================
## self-printout of S-vectors
#  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
#  @date 2009-09-12
def _v_str_ ( self , fmt = ' %g' ) :
    """Self-printout of SVectors: (...)
    """
    index  = 0
    result = ''
    while index < self.kSize :
        if 0 != index : result += ', '
        result += fmt % self.At( index )
        index  += 1
    return "( " + result + ' )'

# =============================================================================
## iterator for SVector
#  @code
#  vct = ...
#  for i in vct : print i 
#  @endcode
def _v_iter_ ( self ) :
    """Iterator for SVector
    >>> vct = ...
    >>> for i in vct : print i 
    """
    for i in range(self.kSize) :
        yield self(i)
        
# =============================================================================
## iterator for SVector
#  @code
#  vct = ...
#  for i,v in vct.iteritems() : print i,v 
#  @endcode
def _v_iteritems_ ( self ) :
    """Iterator for SVector
    >>> vct = ...
    >>> for i,v in vct.iteritems() : print i,v 
    """
    for i in range(self.kSize) :
        yield i,self(i)

# =============================================================================
## helper function for Linear Algebra multiplications 
def _linalg_mul_ ( a  , b ) :    
    if isinstance ( b , ( int , long , float ) ) :
        b  = float( b )
        v  = a.__class__( a )
        v *= b
        return v 
    _ops_  = Ostap.Math.MultiplyOp ( a.__class__ , b.__class__ )
    return _ops_.multiply ( a , b )

# =============================================================================
## helper function for "right" multiplication (Linear Algebra)
def _linalg_rmul_ ( a , b ) :
    if isinstance ( b , ( int , long , float ) ) :
        b  = float( b )
        v  = a.__class__( a )
        v *= b
        return v
    return NotImplemented 

# =============================================================================
## helper function for Linear Algebra divisions 
def _linalg_div_ ( a  , b ) :
    if isinstance ( b , ( int , long , float ) ) :
        b  = float( b )
        v  = a.__class__( a )
        v /= b
        return v
    return NotImplemented

# =============================================================================
## 
def _vector_cross_ ( a, b ) :
    try  : 
        _ops_  = Ostap.Math.MultiplyOp ( a.__class__ , b.__class__ )
        return _ops_.cross ( a , b )
    except TypeError :
        return NotImplemented 

# =============================================================================
## equality of vectors 
def _vector_eq_ ( a , b ) :
    """Equality for vectors
    """
    if         a    is      b   : return True 
    elif len ( a ) != len ( b ) : return False
    #
    try  : 
        _ops_  = Ostap.Math.Equal_To ( a.__class__ )() 
        return _ops_ ( a , b )
    except TypeError :
        for i in len(a) :
            if a[i] != b[i]   : return false
        return True
    
# =============================================================================
## equality of matrices 
def _matrix_eq_ ( a , b ) :
    """Equalty for matrices
    """
    if  a is b : return True
    
    try :
        if   a.kRows != b.kRows : return False
        elif a.kCols != b.kCols : return False
    except :
        pass
        
    try  : 
        _ops_  = Ostap.Math.Equal_To ( a.__class__ )() 
        return _ops_ ( a , b )
    except TypeError :
        return NotImplemented


# =============================================================================
## decorate vector 
def deco_vector ( t ) :

    if not hasattr ( t , '_decorated' ) :

        t ._old_str_    = t.__str__
        t ._old_repr_   = t.__repr__
        
        t ._old_add_    = t.__add__
        t ._old_radd_   = t.__radd__
        t ._old_mul_    = t.__mul__
        t ._old_rmul_   = t.__rmul__
        t ._old_sub_    = t.__sub__
        t ._old_rsub_   = t.__rsub__
        t ._old_div_    = t.__div__

        _operations   = Ostap.Math.VctrOps( t )
        
        t.__add__       = lambda a,b : _operations.add  ( a , b )
        t.__sub__       = lambda a,b : _operations.sub  ( a , b )
        t.__radd__      = lambda a,b : _operations.add  ( a , b )
        t.__rsub__      = lambda a,b : _operations.rsub ( a , b )
        
        t.__mul__       = lambda a,b : _linalg_mul_     ( a , b )
        t.__rmul__      = lambda a,b : _linalg_rmul_    ( a , b )
        t.__div__       = lambda a,b : _linalg_div_     ( a , b )
        
        t.__eq__        = lambda a,b : _vector_eq_      ( a , b )
        t.__neq__       = lambda a,b : not ( a == b ) 

        t.cross         = lambda a,b : _vector_cross_   ( a , b )
        
        t.__rdiv__      = lambda s,*a :  NotImplemented 

        t. _new_str_    = _v_str_
        t. __str__      = _v_str_
        t. __repr__     = _v_str_

        t. __len__      = lambda s : s.kSize 
        t. __contains__ = lambda s, i : 0<=i<s.kSize

        t. __iter__     = _v_iter_        
        t. iteritems    = _v_iteritems_
        
        t. _decorated = True
        
    return t


Ostap.Vector2             = Ostap.Vector(2)
Ostap.Vector3             = Ostap.Vector(3)
Ostap.Vector4             = Ostap.Vector(4)
Ostap.Vector5             = Ostap.Vector(5)
Ostap.Vector6             = Ostap.Vector(6)
Ostap.Vector8             = Ostap.Vector(8)

Ostap.Math.Vector2        = Ostap.Vector2
Ostap.Math.Vector3        = Ostap.Vector3
Ostap.Math.Vector4        = Ostap.Vector4
Ostap.Math.Vector5        = Ostap.Vector5
Ostap.Math.Vector6        = Ostap.Vector6
Ostap.Math.Vector8        = Ostap.Vector8

## vectors of vectors
Ostap.Vectors2            = std.vector ( Ostap.Vector2 )
Ostap.Vectors3            = std.vector ( Ostap.Vector3 )
Ostap.Vectors4            = std.vector ( Ostap.Vector4 )
Ostap.Math.Vectors2       = Ostap.Vectors2
Ostap.Math.Vectors3       = Ostap.Vectors3
Ostap.Math.Vectors4       = Ostap.Vectors4

# ============================================================================
## self-printout of matrices
def _mg_str_ ( self , fmt = ' %+11.4g') :
    """Self-printout of matrices
    >>> matrix = ...
    >>> print matrix 
    """
    _rows = self.kRows
    _cols = self.kCols
    _line = ''
    for _irow in range ( 0 , _rows ) :
        _line += ' |'
        for _icol in range ( 0 , _cols ) :
            _line += fmt % self( _irow , _icol )
        _line += ' |'
        if ( _rows - 1 )  != _irow : _line += '\n'
    return _line

## self-printout of symmetrical matrices
def _ms_str_ ( self , fmt = ' %+11.4g' , width = 12 ) :
    """Self-printout of symmetrical matrices
    >>> matrix = ...
    >>> print matrix 
    """
    _rows = self.kRows
    _cols = self.kCols
    _line = ''
    for _irow in range ( 0 , _rows ) :
        _line += ' |'
        for _icol in range ( 0 , _cols  ) :
            if _icol < _irow : _line += width*' '
            else             : _line += fmt % self( _irow , _icol )
        _line += ' |'
        if ( _rows - 1 )  != _irow : _line += '\n'
    return _line

# =============================================================================
## get the correlation matrix
def _m_corr_ ( self ) :
    """Get the correlation matrix
    >>> mtrx = ...
    >>> corr = mtrx.correlations()
    """
    from math import sqrt

    _t = type ( self )
    _c = _t   ()
    _rows = self.kRows
    for i in range ( 0 , _rows ) :
        _dI = sqrt ( self ( i , i ) )
        for j in range ( i + 1 , _rows ) :
            _dJ = sqrt ( self ( j , j ) )
            _c [ i , j ] = self ( i , j ) /  (  _dI * _dJ )
        _c[ i , i ] = 1.0

    return _c

# =============================================================================
## "getter"
def _m_get_ ( o , i , j ) :

    try :
        return o ( i , j )
    except :
        pass
    
    try :
        return o [ i , j ]
    except :
        pass

    return o [ i ][ j ]
        
# =============================================================================
## add some matrix-like object to the matrix
#  @code
#  m = ...
#  o = ...
#  m.add_to  ( o ) 
#  @endcode
def _mg_increment_ ( m , o ) : 
    """ Add some ``matrix-like'' object to the matrix
    >>> m = ...
    >>> o = ...
    >>> m.increment  ( o ) 
    """
    for i in range ( m.kRows ) :
        for j in range ( m.kCols ) :
            m[i,j] = m(i,j) + _m_get_ ( o , i , j )
                    
    return m


# =============================================================================
## add some matrix-like object to the matrix
#  @code
#  m = ...
#  o = ...
#  m.add_to  ( o ) 
#  @endcode
def _ms_increment_ ( m , o ) : 
    """ Add some ``matrix-like'' object to the matrix
    >>> m = ...
    >>> o = ...
    >>> m.increment  ( o ) 
    """
    for i in range ( m.kRows ) :
        for j in range ( i , m.kCols ) :
            m[i,j] = m(i,j) + 0.5 * ( _m_get_ ( o , i , j ) + _m_get_ ( o , j , i ) )  
                    
    return m


# =============================================================================
## iterator for SMatrix
#  @code
#  matrix = ...
#  for i in matrix : print i 
#  @endcode
def _m_iter_ ( self ) :
    """Iterator for SMatrix
    >>> matrix = ...
    >>> for i in matrix : print i 
    """
    for i in range(self.kRows) :
        for j in range(self.kCols) :
            yield self(i,j)

# =============================================================================
## iterator for SMatrix
#  @code
#  matrix = ...
#  for i,j,v in matrix.iteritems() : print i,j,v 
#  @endcode
def _m_iteritems_ ( self ) :
    """Iterator for SMatrix
    >>> matrix = ...
    >>> for i,j,v in matrix.iteritems() : print i,j,v
    """
    for i in range(self.kRows) :
        for j in range(self.kCols) :
            yield i,j,self(i,j)

        
# =============================================================================
## construct ``similarity'' with ``vector-like'' object
#  @code
#  m = ...
#  v = ...
#  m.sim ( v )
def  _ms_sim_ ( m , v ) :
    """ construct ``similarity'' with ``vector-like'' object
    >>> m = ...
    >>> v = ...
    >>> m.sim ( v )
    """
    sim = 0.0
    for i in range(m.kRows) :
        for j in range(m.kCols) :
            sim += v[i]*m(i,j)*v[j] 
    return sim 


# =============================================================================
## get the eigenvalues for symmetric matrices :
def _eigen_1_ ( self , sorted = True ) :
    """Get the eigenvalues for symmetric matrices :
    >>> mtrx = ...
    >>> values = mtrx.eigenValues ( sorted = True )
    """
    return Ostap.Math.EigenSystems.eigenValues ( self , sorted )

# =============================================================================
## get the eigevectors for symmetric matrices :
def _eigen_2_ ( self , sorted = True ) :
    """Get the eigevectors for symmetric matrices :
    >>> mtrx = ...
    >>> values, vectors = mtrx.eigenVectors( sorted = True )
    """
    if   2 == self.kCols :
        _values  = Ostap.Vector2  ()
        _vectors = Ostap.Vectors2 ()
    elif 3 == self.kCols :
        _values  = Ostap.Vector3  ()
        _vectors = Ostap.Vectors3 ()
    elif 4 == self.kCols :
        _values  = Ostap.Vector4  ()
        _vectors = Ostap.Vectors4 ()
    else :
        raise AttributeError, "Not implemented for dimention: %s" % self.kCols

    st = Ostap.Math.EigenSystems.eigenVectors ( self , _values , _vectors , sorted )
    if st.isFailure () :
        print 'EigenVectors: Failure from EigenSystems' , st

    return ( _values , _vectors )


_eigen_1_ .__doc__ += '\n' +  Ostap.Math.EigenSystems.eigenValues  . __doc__
_eigen_2_ .__doc__ += '\n' +  Ostap.Math.EigenSystems.eigenVectors . __doc__

            
# =============================================================================
##  decorate the matrix  type 
def deco_matrix ( m  ) :
    
    if not hasattr ( m , '_decorated' ) :

        ##  save 'old method'
        m. _old_str_   = m . __str__
        m. _old_repr_  = m . __repr__

        m. _new_str_   = _mg_str_
        m. __repr__    = _mg_str_
        m. __str__     = _mg_str_
        
        m ._old_add_    = m.__add__
        m ._old_radd_   = m.__radd__
        m ._old_mul_    = m.__mul__
        m ._old_rmul_   = m.__rmul__
        m ._old_sub_    = m.__sub__
        m ._old_rsub_   = m.__rsub__
        m ._old_div_    = m.__div__

        _operations     = Ostap.Math.MtrxOps( m )
        
        m.__add__       = lambda a,b : _operations.add  ( a , b )
        m.__sub__       = lambda a,b : _operations.sub  ( a , b )
        m.__radd__      = lambda a,b : _operations.add  ( a , b )
        m.__rsub__      = lambda a,b : _operations.rsub ( a , b )
        
        m.__mul__       = lambda a,b : _linalg_mul_     ( a , b )
        m.__rmul__      = lambda a,b : _linalg_rmul_    ( a , b )
        m.__div__       = lambda a,b : _linalg_div_     ( a , b )
        
        m.__rdiv__      = lambda s,*a :  NotImplemented 

        m.__eq__        = lambda a,b : _matrix_eq_      ( a , b )
        m.__neq__       = lambda a,b : not ( a == b ) 

        
        m._increment_  = _mg_increment_
        m. increment   = _mg_increment_

        m.__iter__     = _m_iter_
        m.iteritems    = _m_iteritems_
        m.__contains__ = lambda s,ij : 0<=ij[0]<s.kRows and 0<=ij[1]<s.kCols
        
        m._decorated   = True
        
    return m

# =============================================================================
##  decorate the symmetrix matrix  type 
def deco_symmatrix ( m ) :

    if not hasattr ( m , '_decorated' ) :

        ##  save 'old method'
        m. _old_str_   = m . __str__
        m. _old_repr_  = m . __repr__

        m.correlations = _m_corr_
        m. _new_str_   = _ms_str_
        m. __repr__    = _ms_str_
        m. __str__     = _ms_str_

        m ._old_add_    = m.__add__
        m ._old_radd_   = m.__radd__
        m ._old_mul_    = m.__mul__
        m ._old_rmul_   = m.__rmul__
        m ._old_sub_    = m.__sub__
        m ._old_rsub_   = m.__rsub__
        m ._old_div_    = m.__div__

        _operations     = Ostap.Math.MtrxOps( m )
        
        m.__add__       = lambda a,b : _operations.add  ( a , b )
        m.__sub__       = lambda a,b : _operations.sub  ( a , b )
        m.__radd__      = lambda a,b : _operations.add  ( a , b )
        m.__rsub__      = lambda a,b : _operations.rsub ( a , b )
        
        m.__mul__       = lambda a,b : _linalg_mul_     ( a , b )
        m.__rmul__      = lambda a,b : _linalg_rmul_    ( a , b )
        m.__div__       = lambda a,b : _linalg_div_     ( a , b )
        
        m.__rdiv__      = lambda s,*a :  NotImplemented 

        m.__eq__        = lambda a,b : _matrix_eq_      ( a , b )
        m.__neq__       = lambda a,b : not ( a == b ) 

        m._increment_  = _ms_increment_
        m.increment    = _ms_increment_

        m.__iter__     = _m_iter_
        m.iteritems    = _m_iteritems_
        m.__contains__ = lambda s,ij : 0<=ij[0]<s.kRows and 0<=ij[1]<s.kCols
        
        m._sim_        = _ms_sim_
        m.sim          = _ms_sim_

        if m.kCols < 4 : 
            m.eigenValues  = _eigen_1_
            m.eigenVectors = _eigen_2_
        
        m._decorated   = True
        
    return m

Ostap.SymMatrix2x2        = Ostap.SymMatrix(2)
Ostap.SymMatrix3x3        = Ostap.SymMatrix(3)
Ostap.SymMatrix4x4        = Ostap.SymMatrix(4)
Ostap.SymMatrix5x5        = Ostap.SymMatrix(5)
Ostap.SymMatrix6x6        = Ostap.SymMatrix(6)
Ostap.SymMatrix7x7        = Ostap.SymMatrix(7)
Ostap.SymMatrix8x8        = Ostap.SymMatrix(8)
Ostap.SymMatrix9x9        = Ostap.SymMatrix(9)


Ostap.Math.SymMatrix2x2   = Ostap.SymMatrix2x2
Ostap.Math.SymMatrix3x3   = Ostap.SymMatrix3x3
Ostap.Math.SymMatrix4x4   = Ostap.SymMatrix4x4
Ostap.Math.SymMatrix5x5   = Ostap.SymMatrix5x5
Ostap.Math.SymMatrix6x6   = Ostap.SymMatrix6x6
Ostap.Math.SymMatrix7x7   = Ostap.SymMatrix7x7
Ostap.Math.SymMatrix8x8   = Ostap.SymMatrix8x8
Ostap.Math.SymMatrix9x9   = Ostap.SymMatrix9x9

for i in range(11) :
    
    t0 = Ostap.Vector ( i )
    deco_vector    ( t0 )
    
    t1 = Ostap.SymMatrix(i)
    deco_symmatrix ( t1 )
    
    for j in range(11) : 
        t2 = Ostap.Matrix(i,j)
        deco_matrix ( t2 ) 

# =============================================================================
if '__main__' == __name__ :
        
    from ostap.utils.docme import docme
    docme ( __name__ , logger = logger )
    
    logger.info('TEST vectors: ')
    
    LA3 = Ostap.Math.Vector3
    l1  = LA3(0,1,2)
    l2  = LA3(3,4,5)
    
    logger.info ( 'l1 , l2 : %s %s '  % ( l1 , l2  ) )
    logger.info ( 'l1 + l2 : %s    '  % ( l1 + l2  ) )
    logger.info ( 'l1 - l2 : %s    '  % ( l1 - l2  ) )
    logger.info ( 'l1 * l2 : %s    '  % ( l1 * l2  ) )
    logger.info ( 'l1 *  2 : %s    '  % ( l1 *  2  ) )
    logger.info ( ' 2 * l2 : %s    '  % ( 2  * l2  ) )
    logger.info ( 'l1 +  2 : %s    '  % ( l1 +  2  ) )
    logger.info ( ' 2 + l2 : %s    '  % ( 2  + l2  ) )
    logger.info ( 'l1 -  2 : %s    '  % ( l1 -  2  ) )
    logger.info ( ' 2 - l2 : %s    '  % ( 2  - l2  ) )
    logger.info ( 'l1 /  2 : %s    '  % ( l1 /  2  ) )
    
    l1 /= 2 
    logger.info ( 'l1 /= 2 : %s    '  % l1 )
    l1 *= 2 
    logger.info ( 'l1 *= 2 : %s    '  % l1 )
    l1 += 2 
    logger.info ( 'l1 += 2 : %s    '  % l1 )
    l1 -= 2 
    logger.info ( 'l1 -= 2 : %s    '  % l1 )

    logger.info('TEST matrices: ')
    
    m22 = Ostap.Math.Matrix(2,2) ()
    m23 = Ostap.Math.Matrix(2,3) ()
    s22 = Ostap.Math.SymMatrix(2)()
    
    l2  = Ostap.Math.Vector(2)()
    l3  = Ostap.Math.Vector(3)()
    
    l2[0]    = 1
    l2[1]    = 2
    
    l3[0]    = 1
    l3[1]    = 2
    l3[1]    = 3
    
    logger.info ( 'l2 , l3 : %s %s '  % ( l2 , l3  ) )
    
    m22[0,0] = 1
    m22[0,1] = 1
    m22[1,1] = 1
    
    m23[0,0] = 1
    m23[1,1] = 1
    m23[0,2] = 1
    
    s22[0,0] = 2
    s22[1,0] = 1
    s22[1,1] = 3
    
    logger.info ( 'm22\n%s'   % m22     ) 
    logger.info ( 's22\n%s'   % s22     ) 
    logger.info ( 'm23\n%s'   % m23     ) 
    logger.info ( 'm22/3\n%s' % (m22/3) ) 
    logger.info ( 'm23*3\n%s' % (m23*3) ) 

    logger.info ( 'm22 * m23 :\n%s' % ( m22 * m23 ) ) 
    logger.info ( 'm22 *  l2 : %s ' % ( m22 * l2  ) ) 
    logger.info ( 'l2  * m22 : %s ' % ( l2  * m22 ) ) 
    logger.info ( 'm23 *  l3 : %s ' % ( m23 * l3  ) ) 
    logger.info ( 'l2  * m23 : %s ' % ( l2  * m23 ) )
    
    logger.info ( 'm22 * s22 + 2 * m22 :\n%s ' %  ( m22*s22 + 2*m22  ) )
    logger.info ( 'm22 == m22*1.0 : %s ' % (  m22 == m22 * 1.0 ) )
    logger.info ( 'm22 != m22*1.1 : %s ' % (  m22 != m22 * 1.1 ) )
    logger.info ( 'm23 == m23*1.0 : %s ' % (  m23 == m23 * 1.0 ) )
    logger.info ( 'm23 != m23*1.1 : %s ' % (  m23 != m23 * 1.1 ) )
    logger.info ( 'l1  == l1 *1.0 : %s ' % (  l1  == l1  * 1.0 ) )
    logger.info ( 'l1  != l1 *1.1 : %s ' % (  l1  != l1  * 1.1 ) )
    logger.info ( 's22 == s22*1.0 : %s ' % (  s22 == s22 * 1.0 ) )
    logger.info ( 's22 != s22*1.1 : %s ' % (  s22 != s22 * 1.1 ) )

# =============================================================================
# The END 
# =============================================================================
