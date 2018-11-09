#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
## @file ostap/fitting/variables.py
#  Module with decoration of some RooFit variables for efficient usage in python
#  @see RooAbsReal
#  @see RooRealVar
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2011-06-07
# =============================================================================
"""Module with decoration of some RooFit variables for efficient usage in python
- see RooAbsReal
- see RooRealVar
"""
# =============================================================================
__version__ = "$Revision$"
__author__  = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__    = "2011-06-07"
__all__     = (
    'SETVAR'  , ## context manager to preserve the current value for RooRealVar
    ) 
# =============================================================================
import ROOT, random
from   ostap.core.core import VE
# =============================================================================
# logging 
# =============================================================================
from ostap.logger.logger import getLogger , allright,  attention
if '__main__' ==  __name__ : logger = getLogger( 'ostap.fitting.variables' )
else                       : logger = getLogger( __name__ )
# =============================================================================
logger.debug( 'Some useful decorations for RooFit variables')
# =============================================================================
_new_methods_ = []

# =============================================================================
## fix parameter at some value
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2012-09-20
def _fix_par_ ( var , value  = None ) :
    """Fix parameter at some value :

    >>> par = ...
    >>> par.fix ( 10 )     
    """
    #
    if None is value :
        if var.isConstant() : return var.ve() 
        var.setConstant( True )
        return var.ve()
    
    if hasattr ( value , 'value' ) : value = value.value()
    #
    var.setVal      ( value )
    if not var.isConstant() : var.setConstant ( True  )
    #
    return var.ve() 

# =============================================================================
## release the parameter
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2012-09-20
def _rel_par_ ( var )  :
    """Release the parameters

    >>> par = ...
    >>> par.release ()     
    """
    if var.isConstant() : var.setConstant ( False )
    #
    return var.ve()

# ==============================================================================
## Convert RooRealVar into ValueWithError 
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2014-06-23
def _rrv_ve_ ( var ) :
    """Convert RooRealVar into ValueWithError
    
    >>> par = ...
    >>> ve  = par.ve()    
    """
    v  =      var.getVal()
    e2 = 0 if var.isConstant() else var.getError()**2
    #
    return VE ( v , e2 )

# ==============================================================================
## check if the given value is in the range of RooRealVar
#  @code 
#  mass_range = ...
#  if v in mass_range : ...
#  @endcode 
def _rrv_contains_ ( var , value ) :
    """check if the given value is in the range of RooRealVar
    >>> mass_range = ...
    >>> if v in mass_range : ... 
    """
    if var.hasMin() and value < var.getMin() : return False 
    if var.hasMax() and value > var.getMax() : return False
    return True 
    
# =============================================================================
## decorate RooRealVar:
ROOT.RooRealVar     . as_VE           = _rrv_ve_ 
ROOT.RooRealVar     . asVE            = _rrv_ve_ 
ROOT.RooRealVar     . ve              = _rrv_ve_
ROOT.RooRealVar     . fix             = _fix_par_
ROOT.RooRealVar     . Fix             = _fix_par_
ROOT.RooRealVar     . release         = _rel_par_
ROOT.RooRealVar     . Release         = _rel_par_
## convert to float 
ROOT.RooRealVar     . __float__       = lambda s : s.getVal()
## print it in more suitable form 
ROOT.RooRealVar     . __repr__        = lambda s : "'%s' : %s " % ( s.GetName() , s.ve() )
ROOT.RooRealVar     . __str__         = lambda s : "'%s' : %s " % ( s.GetName() , s.ve() )


ROOT.RooConstVar    . as_VE          = lambda s : VE ( s.getVal() , 0 )
ROOT.RooFormulaVar  . as_VE          = lambda s : VE ( s.getVal() , 0 )
ROOT.RooConstVar    . asVE           = lambda s : VE ( s.getVal() , 0 )
ROOT.RooFormulaVar  . asVE           = lambda s : VE ( s.getVal() , 0 )


ROOT.RooAbsReal       .__contains__ = lambda s,v : False ## ??? do we need it???
ROOT.RooAbsRealLValue .__contains__ = _rrv_contains_ 

# =====================================================================
ROOT.RooAbsReal. minmax  = lambda s : ()
ROOT.RooAbsReal.xminmax  = lambda s : ()
ROOT.RooAbsRealLValue  . xmin            = lambda s : s.getMin()
ROOT.RooAbsRealLValue  . xmax            = lambda s : s.getMax()
ROOT.RooAbsRealLValue  . minmax          = lambda s : ( s.xmin() , s.xmax() ) 
ROOT.RooAbsRealLValue  .xminmax          = lambda s : ( s.xmin() , s.xmax() ) 


_new_methods_ += [
    ROOT.RooRealVar   . as_VE     ,
    ROOT.RooRealVar   . asVE      ,
    ROOT.RooRealVar   . ve        ,
    ROOT.RooRealVar   . fix       ,
    ROOT.RooRealVar   . Fix       ,
    ROOT.RooRealVar   . release   ,
    ROOT.RooRealVar   . Release   ,
    ## convert to float 
    ROOT.RooRealVar   . __float__ ,
    ## print it in more suitable form 
    ROOT.RooRealVar   . __repr__  ,
    #
    ROOT.RooAbsRealLValue .__contains__ , 
    ROOT.RooRealVar   . xmin      ,
    ROOT.RooRealVar   . xmax      ,
    ROOT.RooRealVar   . minmax    ,
    #
    ROOT.RooConstVar    .as_VE    ,
    ROOT.RooFormulaVar  .as_VE    ,
    ROOT.RooConstVar    .asVE     ,
    ROOT.RooFormulaVar  .asVE     ,
    #
    ]


# =============================================================================
## Prepare ``soft'' gaussian constraint for the given variable
#  @code 
#    >>> var     = ...                            ## the variable 
#    >>> extcntr = var.constaint( VE(1,0.1**2 ) ) ## create constrains 
#    >>> model.fitTo ( ... , extcntr )            ## use it in the fit 
#  @endcode 
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2014-06-23
def _rar_make_constraint_ ( v , value ) :
    """Prepare ``soft'' gaussian constraint for the variable
    
    >>> var     = ...                            ## the variable 
    >>> extcntr = var.constaint( VE(1,0.1**2 ) ) ## create constrains 
    >>> model.fitTo ( ... , extcntr )            ## use it in the fit 
    """
    #
    #
    ## create gaussian constrains
    #
    vn       = 'Constr(%s)' % v.GetName()
    vt       = 'Gauissian constraint(%s) at %s' % ( v.GetName() , value )
    #
    v._cvv   = ROOT.RooFit.RooConst ( value.value () )  ## NB! 
    v._cve   = ROOT.RooFit.RooConst ( value.error () )  ## NB! 
    v._cntr  = ROOT.RooGaussian     ( vn , vt , v , v._cvv , v._cve )
    #
    ## keep it 
    v._cntrs = ROOT.RooArgSet       ( v._cntr )
    #
    return ROOT.RooFit.ExternalConstraints ( v._cntrs ) 

ROOT.RooAbsReal. constraint = _rar_make_constraint_

_new_methods_ += [
    ROOT.RooAbsReal. constraint 
    ]


# ============================================================================
## make a histogram for RooRealVar
#  @see RooRealVar
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2013-07-14
def _rrv_as_H1_ ( v , bins = 100 , double = True ) :
    """Make TH1 histogram from RooRealVar
    
    >>> variable = ...
    >>> histo = variable.histo ( 100 )    
    """
    _hT = ROOT.TH1D if double else ROOT.TH1F
    _h  = _hT ( hID() , v.GetTitle() , bins , v.getMin()  , v.getMax() )
    _h.Sumw2()
    
    return _h 

ROOT.RooRealVar   . histo = _rrv_as_H1_
ROOT.RooRealVar   . asH1  = _rrv_as_H1_

_RRV_ = ROOT.RooRealVar

_new_methods_ += [
    ROOT.RooRealVar.histo , 
    ROOT.RooRealVar.asH1  
    ]

# =============================================================================
## Math operations 
# =============================================================================

# ============================================================================
## Addition of RooRealVar and ``number''
def _rrv_add_ ( s , o ) :
    """Addition of RooRealVar and ``number''

    >>> var = ...
    >>> num = ...
    >>> res = var + num
    
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return v + o

# ============================================================================
## Subtraction  of RooRealVar and ``number''
def _rrv_sub_ ( s , o ) :
    """Subtraction of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = var - num
    
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return v - o

# ============================================================================
## Multiplication of RooRealVar and ``number''
def _rrv_mul_ ( s , o ) :
    """Multiplication  of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = var * num
    
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return v * o

# ============================================================================
## Division of RooRealVar and ``number''
def _rrv_div_ ( s , o ) :
    """Division of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = var / num
    
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return v / o

# ============================================================================
## (right) Addition of RooRealVar and ``number''
def _rrv_radd_ ( s , o ) :
    """(right) Addition of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = num + var 
    
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return o + v 

# ============================================================================
## (right) subtraction  of RooRealVar and ``number''
def _rrv_rsub_ ( s , o ) :
    """(right) subtraction of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = num - var 
    
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return o - v 

# ============================================================================
## (right) multiplication of RooRealVar and ``number''
def _rrv_rmul_ ( s , o ) :
    """(right) Multiplication  of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = num * var     
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return o * v 

# ============================================================================
## (right) Division of RooRealVar and ``number''
def _rrv_rdiv_ ( s , o ) :
    """(right) Division of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = num / var     
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return o / v 

# ============================================================================
## pow of RooRealVar and ``number''
def _rrv_pow_ ( s , o ) :
    """pow of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = var ** num     
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return v**o  

# ============================================================================
## (right) pow of RooRealVar and ``number''
def _rrv_rpow_ ( s , o ) :
    """pow of RooRealVar and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> res = num ** var 
    
    """
    if   isinstance ( o , _RRV_    ) and not o.isConstant() : o = o.ve     () 
    elif hasattr    ( o , 'getVal' )                        : o = o.getVal ()
    #
    v = s.getVal() if s.isConstant() else s.ve()
    #
    return o**v   


# ============================================================================
ROOT.RooRealVar . __add__   = _rrv_add_
ROOT.RooRealVar . __sub__   = _rrv_sub_
ROOT.RooRealVar . __div__   = _rrv_div_
ROOT.RooRealVar . __mul__   = _rrv_mul_
ROOT.RooRealVar . __pow__   = _rrv_pow_

ROOT.RooRealVar . __radd__  = _rrv_radd_
ROOT.RooRealVar . __rsub__  = _rrv_rsub_
ROOT.RooRealVar . __rdiv__  = _rrv_rdiv_
ROOT.RooRealVar . __rmul__  = _rrv_rmul_
ROOT.RooRealVar . __rpow__  = _rrv_rpow_


_new_methods_ += [
    ROOT.RooRealVar.__add__  , 
    ROOT.RooRealVar.__sub__  , 
    ROOT.RooRealVar.__div__  , 
    ROOT.RooRealVar.__mul__  , 
    ROOT.RooRealVar.__pow__  , 
    ROOT.RooRealVar.__radd__ , 
    ROOT.RooRealVar.__rsub__ , 
    ROOT.RooRealVar.__rdiv__ , 
    ROOT.RooRealVar.__rmul__ , 
    ROOT.RooRealVar.__rpow__ , 
    ]

# =============================================================================
## (compare RooRealVar and "number"
def _rrv_le_ ( s , o ) :
    """compare RooRealVal and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> iv var <= num : print ' ok! '
    """
    return o >= s.getVal()

# ============================================================================
## (compare RooRealVar and "number"
def _rrv_lt_ ( s , o ) :
    """compare RooRealVal and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> iv var < num : print ' ok! '
    """
    return o > s.getVal()

# ============================================================================
## (compare RooRealVar and "number"
def _rrv_ge_ ( s , o ) :
    """compare RooRealVal and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> iv var >= num : print ' ok! '
    """
    return o <= s.getVal()

# ============================================================================
## (compare RooRealVar and "number"
def _rrv_gt_ ( s , o ) :
    """compare RooRealVal and ``number''
    
    >>> var = ...
    >>> num = ...
    >>> iv var > num : print ' ok! '
    """
    return o < s.getVal()

# ============================================================================
ROOT.RooRealVar . __lt__   = _rrv_lt_
ROOT.RooRealVar . __gt__   = _rrv_gt_
ROOT.RooRealVar . __le__   = _rrv_le_
ROOT.RooRealVar . __ge__   = _rrv_ge_

_new_methods_ += [
    ROOT.RooRealVar.__lt__  ,
    ROOT.RooRealVar.__gt__  ,
    ROOT.RooRealVar.__le__  ,
    ROOT.RooRealVar.__ge__  ,
    ]




# =============================================================================
## get min/max in one go 
#  @see RooRealVar
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2013-07-14
def _rrv_minmax_ ( s ) :
    """Get min/max in one go

    >>> var = ...
    >>> mn,mx = var.minmax()
    """
    return s.getMin(),s.getMax()

ROOT.RooRealVar   . minmax  = _rrv_minmax_

_new_methods_ += [
    ROOT.RooRealVar.minmax  ,
    ]


# =============================================================================
## Properties 
# =============================================================================

# =============================================================================
def _rav_getval_  ( self ) :
    """Get the value, associated with the variable
    >>> var = ...
    >>> print var.value 
    """
    return self.getVal()

# =============================================================================
def _rav_getvale_ ( self ) :
    """Get the value(and the error), associated with the variable
    >>> var = ...
    >>> print  var.value 
    """
    v = self.getVal()
    e = self.getError() 
    return VE ( v , e*e ) if e>0 else v

# =============================================================================
def _rav_setval_  ( self , value ) :
    """Assign the valeu for the variable 
    >>> var = ...
    >>> var.value = 10 
    """
    value = float ( value )
    self.setVal ( value ) 
    return self.getVal()

# =============================================================================
def _rav_setvalc_  ( self , value ) :
    """Assign the valeu for the variable 
    >>> var = ...
    >>> var.value = 10 
    """
    value = float ( value )
    mn,mx  = self.getMin(), self.getMax() 
    if not mn <= value <= mx :
        logger.warning('Value %s is out the range [%s,%s]' %  ( value  , mn , mx ) ) 
    self.setVal ( value ) 
    return self.getVal()

# =============================================================================
def _rav_geterr_  ( self ) :
    """Get the error
    >>> var = ...
    >>> print(var.error)
    """
    return self.getError()

# =============================================================================
def _rav_seterr_  ( self , value ) :
    """Set the error
    >>> var = ...
    >>> var.error = 10 
    """
    value = float ( value )
    if not 0<= value :
        logger.warning('Error %s is not non-negative' % value  ) 
    self.setError ( value )
    return self.getError()

# =============================================================================
## decorate classes 
for t in ( ROOT.RooAbsReal       , 
           ROOT.RooAbsLValue     , 
           ROOT.RooAbsRealLValue , 
           ROOT.RooRealVar       ) :

    _getter_ = None
    _setter_ = None

    if hasattr  ( t , 'getVal' ) and hasattr ( t , 'getError' ) :
        _getter_ = _rav_getvale_
    elif hasattr  ( t , 'getVal' ) :
        _getter_ = _rav_getval_

    if hasattr  ( t , 'setVal' ) and hasattr ( t , 'getMin' ) and hasattr ( t , 'getMax' ) :
        _setter_ = _rav_setvalc_
    elif hasattr  ( t , 'setVal' ) :
        _setter_ = _rav_setval_

    doc1 = """The current value, associated with the variable,
    
    >>> var = ...
    
    get value:
    =========
    
    >>> print (var.value) ## getter
    
    """
    doc2 = """The current value, associated with the variable,
    
    >>> var = ...
    
    get value:
    =========
    
    >>> print (var.value) ## getter
    
    Set value:
    ==========

    >>> var.value = 15 
    
    """
    if   _setter_  : t.value = property ( _getter_ , _setter_ , None  , doc2 )
    elif _getter_  : t.value = property ( _getter_ , _setter_ , None  , doc1 )


    doce1 = """The current error, associated with the variable,
    
    >>> var = ...
    
    Get error:
    =========
    
    >>> print (var.error) ## getter
    
    """
    doce2 = """The current error, associated with the variable,
    
    >>> var = ...
    
    Get error:
    =========
    
    >>> print (var.error) ## getter
    
    Set error:
    ==========

    >>> var.error = 15 
    
    """
    
    _gettere_ = None
    _settere_ = None

    if hasattr  ( t , 'getError' ) and hasattr ( t , 'setError' ) :
        _gettere_ = _rav_geterr_
        _settere_ = _rav_seterr_
    elif hasattr  ( t , 'getError' ) :
        _gettere_ = _rav_geterr_

    if   _settere_  : t.error = property ( _gettere_ , _settere_ , None  , doce2 )
    elif _gettere_  : t.error = property ( _gettere_ , _settere_ , None  , doce1 )

    if hasattr ( t , 'getVal' ) and not hasattr ( t , '__float__' ) :
        t.__float__ = lambda s : s.getVal()

# =============================================================================
## @class SETVAR
#  Simple context manager to preserve current value for RooAbsVar
#  @code
#  var = ...
#  var.setVal(1) 
#  print '1) value %s ' % var.getVal() 
#  with SETVAR(var) :
#        print '2) value %s ' % var.getVal() 
#        var.setVal(10)
#        print '3) value %s ' % var.getVal() 
#  print '4) value %s ' % var.getVal() 
#  @endcode
class SETVAR(object):
    """ Simple context manager to preserve current value for RooAbsVar
    >>> var = ...
    >>> var.setVal(1) 
    >>> print '1) value %s ' % var.getVal() 
    >>> with SETVAR(var) :
    ...    print '2) value %s ' % var.getVal() 
    ...    var.setVal(10)
    ...    print '3) value %s ' % var.getVal() 
    >>> print '4) value %s ' % var.getVal() 
    """
    def __init__  ( self , xvar ) :
        self.xvar = xvar
    def __enter__ ( self        ) :
        self._old = float ( self.xvar.getVal() ) 
        return self 
    def __exit__  ( self , *_   ) :
        self.xvar.setVal  ( self._old ) 


# =============================================================================
_decorated_classes_ = (
    ROOT.RooRealVar       ,
    ROOT.RooConstVar      ,
    ROOT.RooFormulaVar    ,
    ROOT.RooAbsReal       ,
    ROOT.RooAbsRealLValue
)

_new_methods_ = tuple ( _new_methods_ ) 

# =============================================================================
if '__main__' == __name__ :
    
    from ostap.utils.docme import docme
    docme ( __name__ , logger = logger )
    
# =============================================================================
# The END 
# =============================================================================