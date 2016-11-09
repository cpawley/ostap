#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
# $Id$
# =============================================================================
## @file core.py
#  Core objects for Ostap 
#
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2011-06-07
#  
# =============================================================================
"""Core objects for Ostap 
"""
# =============================================================================
__version__ = "$Revision$"
__author__  = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__    = "2011-06-07"
__all__     = (
    'cpp'              ,  ## global C++ namespace 
    'ROOTCWD'          ,  ## context manager to keep/preserve ROOT current directory
    'rootID'           ,  ## global identifier for ROOT objects
    'funcID'           ,  ## global identifier for ROOT functions 
    'funID'            ,  ## global identifier for ROOT functions 
    'fID'              ,  ## global identifier for ROOT functions 
    'histoID'          ,  ## global identifier for ROOT histograms 
    'hID'              ,  ## global identifier for ROOT histograms 
    'dsID'             ,  ## global identifier for ROOT/RooFit datasets
    ##
    'VE'               ,  ## shortcut for Gaudi::Math::ValuewithError
    'SE'               ,  ## shortcut for StatEntity
    'WSE'              ,  ## shortcut for Gaudi::Math::WStatEntity 
    ##
    'binomEff'         ,  ## binomial efficiency  
    'binomEff2'        ,  ## binomial efficiency
    'zechEff'          ,  ## binomial efficiency: Zech's recipe 
    'wilsonEff'        ,  ## binomial efficiency: Wilson 
    'agrestiCoullEff'  ,  ## binomial efficiency: Agresti-Coull
    ##
    'iszero'           ,  ## comparison with zero  for doubles  
    'isequal'          ,  ## comparison for doubles 
    'isint'            ,  ## Is float value actually int  ? 
    'islong'           ,  ## Is float value actually long ?
    'inrange'          ,  ## Is float walue in range ?  
    ##
    'natural_entry'    ,  ## natual entry?   @see Gaudi::Math::natural_entry 
    'natural_number'   ,  ## natual numnber? @see Gaudi::Math::natural_number 
    )
# =============================================================================
import ROOT, cppyy, math, sys
cpp = cppyy.gbl 
# =============================================================================
# logging 
# =============================================================================
from ostap.logger.logger import getLogger 
if '__main__' ==  __name__ : logger = getLogger( 'ostap.core' )
else                       : logger = getLogger( __name__     )
# =============================================================================
logger.debug ( 'Core objects/classes/functions for Ostap')
# =============================================================================
from ostap.math.base      import ( cpp      , Ostap   ,
                                   iszero   , isequal ,
                                   isint    , islong  ,
                                   inrange  ,
                                   natural_number     ,
                                   natural_entry      )   
from ostap.math.ve        import VE
from ostap.stats.counters import SE,WSE 
#
binomEff        = Ostap.Math.binomEff
binomEff2       = Ostap.Math.binomEff2
zechEff         = Ostap.Math.zechEff
wilsonEff       = Ostap.Math.wilsonEff
agrestiCoullEff = Ostap.Math.agrestiCoullEff

# =============================================================================
## @class ROOTCWD
#  context manager to preserve current directory (rather confusing stuff in ROOT)
#  @code
#  print ROOT.gROOT.CurrentDirectory() 
#  with ROOTCWD() :
#     print ROOT.gROOT.CurrentDirectory() 
#     rfile = ROOT.TFile( 'test.root' , 'recreate' )
#     print ROOT.gROOT.CurrentDirectory() 
#  print ROOT.gROOT.CurrentDirectory() 
#  @endcode 
#  @author Vanya BELYAEV Ivan.Belyaev@iep.ru
#  @date 2015-07-30
class ROOTCWD(object) :
    """Context manager to preserve current directory
    (rather confusing stuff in ROOT) 
    >>> print ROOT.gROOT.CurrentDirectory() 
    >>> with ROOTCWD() :
    ...     print ROOT.gROOT.CurrentDirectory() 
    ...     rfile = ROOT.TFile( 'test.root' , 'recreate' )
    ...     print ROOT.gROOT.CurrentDirectory() 
    ... print ROOT.gROOT.CurrentDirectory() 
    """
    ## context manager ENTER 
    def __enter__ ( self ) :
        "Save current working directory"
        self._dir = ROOT.gROOT.CurrentDirectory() 
        
    ## context manager EXIT 
    def __exit__  ( self , *_ ) :
        "Make the previous directory current again"
        if self._dir :
            if   not hasattr ( self._dir , 'IsOpen' ) : self._dir.cd() 
            elif     self._dir.IsOpen()               : self._dir.cd()
            
## =============================================================================
#  global identifier for ROOT objects 
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2011-06-07
def rootID ( prefix = 'o_' ) :
    """ Construct the unique ROOT-id 
    """
    _fun = lambda i : prefix + '%d'% i
    
    _root_ID = 1000
    ## 
    with ROOTCWD() : ## keep the current working directory:
        
        _id  = _fun ( _root_ID )
        grd  = ROOT.gROOT
        cwd  = grd.CurrentDirectory()
        while grd.FindObject    ( _id ) or \
              grd.FindObjectAny ( _id ) or \
              cwd.FindObject    ( _id ) or \
              cwd.FindObjectAny ( _id ) : 
                
            _root_ID += 10 
            _id       = _fun ( _root_ID ) 
            
    return _id                 ## RETURN
# =============================================================================
## global ROOT identified for function objects 
def funcID  () : return rootID  ( 'f_' )
## global ROOT identified for function objects 
def funID   () : return funcID  ( )
## global ROOT identified for function objects 
def fID     () : return funcID  ( )
## global ROOT identified for histogram objects 
def histoID () : return rootID  ( 'h_' )
## global ROOT identified for histogram objects 
def histID  () : return histoID ( )
## global ROOT identified for histogram objects 
def hID     () : return histoID ( )
## global ROOT identified for dataset objects 
def dsID    () : return rootID  ( 'ds_' )

# ==================================================================================
## get current directory in ROOT
#  @code
#  d = cwd()
#  print d
#  @endcode 
def cwd() :
    """ Get current directory in ROOT
    >>> d = cdw() 
    """
    return ROOT.gROOT.CurrentDirectory()

# =================================== ===============================================
## get current directory in ROOT
#  @code
#  print pwd() 
#  @endcode 
def pwd() :
    """ Get current directory in ROOT
    >>> print pwd() 
    """
    return ROOT.gROOT.CurrentDirectory().GetPath() 


# =============================================================================
if '__main__' == __name__ :
    
        
    from ostap.line.line import line 
    logger.info ( __file__  + '\n' + line  ) 
    logger.info ( 80*'*'   )
    logger.info ( __doc__  )
    logger.info ( 80*'*' )
    logger.info ( ' Author  : %s' %         __author__    ) 
    logger.info ( ' Version : %s' %         __version__   ) 
    logger.info ( ' Date    : %s' %         __date__      )
    logger.info ( ' Symbols : %s' %  list ( __all__     ) )
    logger.info ( 80*'*' ) 
    
# =============================================================================
# The END 
# =============================================================================
