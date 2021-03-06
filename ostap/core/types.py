#!/usr/bin/env python
# -*- coding: utf-8 -*-
# =============================================================================
## @file ostap/core/types.py
#  core types for ostap 
#  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
#  @date   2011-06-07
# =============================================================================
"""Core types for ostap 
"""
# =============================================================================
__version__ = "$Revision$"
__author__  = "Vanya BELYAEV Ivan.Belyaev@itep.ru"
__date__    = "2011-06-07"
__all__     = (
    ##
    'num_types'       , ## numerical types (real)
    'integer_types'   , ## integer types
    'string_types'    , ## string  types
    'str_types'       , ## string  types (only string!)
    'listlike_types'  , ## list-like types (list, tuple, set, collections.Iterable)
    'list_types'      , ## list types (list, tuple)
    ##
    'is_integer'      , ## is a value of int-like type?
    'is_number'       , ## is a value of numeric  type?
    'is_good_number'  , ## is a value of numeric  type and not-NaN,no-Inf ?
    ##
    'is_string'       , ## is a value of str-type? 
    'is_string_like'  , ## is a value of string-like type? 
    'is_list'         , ## is a value of list/tuple type?
    'is_list_like'    , ## is a value of list-like type?
    ##
    )
# =============================================================================
import sys, math, collections  

if sys.version_info[0] > 2:
    long          = int
    string_types  = bytes , str 
    integer_types = int   , 
else :
    string_types  = str   , unicode
    integer_types = int   , long

num_types = integer_types + ( float , ) 
str_type  = str,

list_types     = list, tuple
listlike_types = list_types + ( set , collections.Iterable ) 
# =============================================================================
## Is this number of a proper integer?
def is_integer ( v ) :
    """Is this number of a proper integer?"""
    return isinstance ( v , integer_types ) 

# =============================================================================
## Is this number of a proper numeric type
def is_number  ( v ) :
    """Is this number of a proper numeric?"""
    return isinstance ( v , num_types   ) 

# =============================================================================
## good numeric value 
def is_good_number  ( v ) :
    """Is numeric type and good value?"""
    return isinstance ( v , num_types   ) and \
           ( not math.isinf ( v ) ) and ( not math.isnan ( v ) )

# =============================================================================
## is  value of str-type?
def is_string ( v ) :
    """Is  value of str-type?"""
    return isinstance ( v , str_types )

# =============================================================================
## is  value of string-like -type?
def is_string_like ( v ) :
    """Is  value of string-like type?"""
    return isinstance ( v , string_types )

# =============================================================================
## is list type?
def is_list  ( v ) :
    """Is value of list type (list ot tuple)"""
    return isinstance ( v , list_type )

# =============================================================================
## is list-like type?
def is_list_like  ( v ) :
    """Is value of list-like type (list but not a string-like!)"""
    return isinstance ( v , listlike_type ) and not isinstance ( v , string_types )
    
# =============================================================================
if '__main__' == __name__ :
    
    from ostap.utils.docme import docme
    docme ( __name__ , logger = logger )

    
# =============================================================================
##                                                                      The END 
# =============================================================================
