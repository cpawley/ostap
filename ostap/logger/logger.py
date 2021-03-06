#!/usr/bin/env python 
# -*- coding: utf-8 -*-
# =============================================================================
# Copyright Ostap developers
# =============================================================================
"""Ostap simple logger.

Bsed on the logging of the Gaudi software project of CERN:
- Simple control (global and local)  over logging threshold.
 Primitive utilities for colorized logging.
"""
# =============================================================================
import logging
# =============================================================================
__all__ = (
    'getLogger'      , ## get (configured) logger
    'setLogging'     , ## set disable level according to MSG.Level
    'LogLevel'       , ## context manager to control output level 
    'logLevel'       , ## helper function to control output level
    'logVerbose'     , ## helper function to control output level
    'logDebug'       , ## helper function to control output level
    'logInfo'        , ## helper function to control output level
    'logWarning'     , ## helper function to control output level
    'logError'       , ## helper function to control output level
    'logColor'       , ## context manager to switch on  color logging locally  
    'logNoColor'     , ## context manager to switch off color logging locally  
    'noColor'        , ## context manager to switch off color logging locally  
    'make_colors'    , ## force colored logging 
    'reset_colors'   , ## reset colored logging
    'colored_string' , ## make a colored string
    'attention'      , ## make "attention" string
    'allright'       , ## make "allright" string
    ##
    'ALL', 'VERBOSE', 'DEBUG', 'INFO', 'WARNING', 'ERROR', 'FATAL' ,
    )
# =============================================================================
# Message levels
ALL     = 0
VERBOSE = 1
DEBUG   = 2
INFO    = 3
WARNING = 4
ERROR   = 5
FATAL   = 6

# =============================================================================
## some manipulations with logging module
if not hasattr ( logging , 'VERBOSE' ) : logging.VERBOSE = 5

# =============================================================================
## Log message with severity 'VERBOSE'
def _verbose2_(msg, *args, **kwargs):
    """Log a message with severity 'VERBOSE' on the root logger.
    """
    if len(logging.root.handlers) == 0: logging.basicConfig()
    logging.root.verbose (msg, *args, **kwargs)

# =============================================================================
## Log message with severity 'VERBOSE'
def _verbose1_(self, msg, *args, **kwargs):
    """Log 'msg % args' with severity 'VERBOSE'.
    """
    if self.isEnabledFor(logging.VERBOSE):
            self._log(logging.VERBOSE, msg, args, **kwargs)

# =============================================================================
## Log message with severity 'VERBOSE'
def _verbose2_(msg, *args, **kwargs):
    """Log a message with severity 'VERBOSE' on the root logger.
    """
    if len(logging.root.handlers) == 0:
        logging.basicConfig()
    logging.root.verbose (msg, *args, **kwargs)

# =============================================================================
## add method 'verbose' to logger 
logging.Logger.verbose = _verbose1_

# =============================================================================
## add method 'verbose' to root logger 
logging.verbose        = _verbose2_

# =============================================================================
## convert MSG::Level into logging level 
def setLogging ( output_level ) :
    """Convert MSG::Level into logging level 
    """
    if   FATAL   <= output_level : logging.disable ( logging.FATAL   - 1 )
    elif ERROR   <= output_level : logging.disable ( logging.ERROR   - 1 )
    elif WARNING <= output_level : logging.disable ( logging.WARNING - 1 )
    elif INFO    <= output_level : logging.disable ( logging.INFO    - 1 )
    elif DEBUG   <= output_level : logging.disable ( logging.DEBUG   - 1 )
    elif VERBOSE <= output_level : logging.disable ( logging.VERBOSE - 1 )
    
## define standard logging names 
logging.addLevelName ( logging.CRITICAL  , 'FATAL  '  )
logging.addLevelName ( logging.WARNING   , 'WARNING'  )
logging.addLevelName ( logging.DEBUG     , 'DEBUG  '  )
logging.addLevelName ( logging.INFO      , 'INFO   '  )
logging.addLevelName ( logging.ERROR     , 'ERROR  '  )
logging.addLevelName ( logging.VERBOSE   , 'VERBOSE'  )



# =============================================================================
# - is sys.stdout attached to terminal or not ?
# - do we run IPYTHON ? 
from ostap.utils.basic import isatty, with_ipython

# =============================================================================
# COLORS: 
# =============================================================================
## global flag to indicate if we use colored logging
__with_colors__ = False
# =============================================================================
## Is colorization enabled ? 
def with_colors() :
    """Is colorization enabled ?"""
    global __with_colors__
    return bool(__with_colors__) and isatty() 
# =============================================================================
## reset colorization of logging 
def reset_colors() :
    """Reset colorization of logging 
    >>> reset_colors()
    """
    logging.addLevelName ( logging.CRITICAL  , 'FATAL  '  )
    logging.addLevelName ( logging.WARNING   , 'WARNING'  )
    logging.addLevelName ( logging.DEBUG     , 'DEBUG  '  )
    logging.addLevelName ( logging.INFO      , 'INFO   '  )
    logging.addLevelName ( logging.ERROR     , 'ERROR  '  )
    logging.addLevelName ( logging.VERBOSE   , 'VERBOSE'  )
    #
    global __with_colors__
    __with_colors__ = False 
    return with_colors()  
    
# =============================================================================
## get configured logger
#  @code
#  logger1 = getLogger ( 'LOGGER1' )
#  logger2 = getLogger ( 'LOGGER2' , level = logging.INFO )
#  @endcode 
def getLogger ( name                                                 ,
                fmt    = '# %(name)-25s %(levelname)-7s %(message)s' ,
                level  = logging.VERBOSE - 2                         ,
                stream = None                                        ) :  
    """Get the proper logger
    >>> logger1 = getLogger ( 'LOGGER1' )
    >>> logger2 = getLogger ( 'LOGGER2' , level = logging.INFO )
    """
    #
    logger = logging.getLogger ( name )
    logger.propagate =  False 
    ##logger.propagate =  True
    #
    while logger.handlers :
        logger.removeHandler ( logger.handlers[0] )
    #
    if not stream :
        import sys
        stream = sys.stdout
        
    lh  = logging.StreamHandler ( stream ) 
    fmt = logging.Formatter     ( fmt    )
    lh  . setFormatter          ( fmt    )
    logger.addHandler           ( lh     ) 
    #
    logger.setLevel             ( level  )
    #
    return logger


# =============================================================================
## @class LogLevel
#  Temporarily enable/disable certain logger levels
#  @code
#  with LogLevel( logging.CRITICAL ) :
#       ...do something... 
#  @endcode
class LogLevel(object) :
    """Temporarily enable/disable certain logger levels
    >>> with LogLevel( logging.CRITICAL ) :
    ...  do something here ...
    """
    def __init__  ( self , level = logging.INFO - 1 ) :
        self.new_level = level 
        self.old_level = logging.root.manager.disable

    ## context manager: ENTER 
    def __enter__ ( self ) :
        self.old_level = logging.root.manager.disable
        logging.disable ( self.new_level )
        return self

    ## context manager: EXIT 
    def __exit__ ( self , *_ ) :        
        logging.disable ( self.old_level )

# =============================================================================
#  Temporarily enable/disable certain logger levels
#  @code
#  with logLevel( logging.CRITICAL ) :
#       ...do something... 
#  @endcode
def logLevel ( level = logging.INFO - 1 ) :
    """Temporarily enable/disable certain logger levels
    >>> with logLevel( logging.CRITICAL ) :
    >>>  ...do something...
    """
    return LogLevel ( level )

# =============================================================================
#  Temporarily enable/disable all loggers with level less then DEBUG 
#  @code
#  with logVerbose() :
#       ...do something... 
#  @endcode
def logVerbose () :
    """Temporarily disable all loggers with level less then INFO 
    >>> with logVerbose() :
    >>>  ...do something...
    """    
    return logLevel (  logging.VERBOSE   - 1 )

# =============================================================================
#  Temporarily enable/disable all loggers with level less then DEBUG 
#  @code
#  with logInfo() :
#       ...do something... 
#  @endcode
def logDebug   () :
    """Temporarily disable all loggers with level less then INFO 
    >>> with logDebug() :
    >>>  ...do something...
    """    
    return logLevel (  logging.DEBUG   - 1 )
# =============================================================================
#  Temporarily enable/disable all loggers with level less then INFO 
#  @code
#  with logInfo() :
#       ...do something... 
#  @endcode
def logInfo    () :
    """Temporarily disable all loggers with level less then INFO 
    >>> with logInfo() :
    >>>  ...do something...
    """
    return logLevel (  logging.INFO    - 1 )

# =============================================================================
#  Temporarily enable/disable all loggers with level less then WARNING
#  @code
#  with logInfo() :
#       ...do something... 
#  @endcode
def logWarning () : 
    """Temporarily disable all loggers with level less then WARNING
    >>> with logWarning() :
    >>>  ...do something...
    """   
    return logLevel (  logging.WARNING - 1 )

# =============================================================================
#  Temporarily enable/disable all loggers with level less then ERROR 
#  @code
#  with logError() :
#       ...do something... 
#  @endcode
def logError   () :
    """Temporarily disable all loggers with level less then ERROR
    >>> with logWarning() :
    >>>  ...do something...
    """       
    return logLevel (  logging.ERROR   - 1 )


## ASCII colors :
BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE = range(8)
    
# =============================================================================
## provide colored string
#  @code
#  print colored_string ( 'Hello' , foreground = RED , background = YELLOW , bold = True )
#  @endcode 
def colored_string ( what               ,
                     foreground = None  ,
                     background = None  ,
                     bold       = False ,
                     blink      = False ,
                     underline  = False ) :
    """
    >>> print colored_string ( 'Hello' , foreground = RED , background = YELLOW , bold = True , blink = True , underline = True )
    """
    ## nothing to colorize or no coloring is activated
    from ostap.utils.basic import isatty
    if not what or not with_colors() or not isatty() : return what

    ## nothing to do 
    if ( foreground is None ) and ( background is None ) :
        if ( not bold ) and ( not blink ) and ( not underline ) : return what 

    RESET_SEQ = "\033[0m"
    COLOR_SEQ = "\033[1;%dm"
    BOLD_SEQ  = "\033[1m"    if bold      else ''
    BLINK_SEQ = "\033[5m"    if blink     else '' 
    ULINE_SEQ = "\033[4m"    if underline else '' 
    
    fg = COLOR_SEQ % ( 30 + ( foreground % 8 ) ) if not foreground is None else '' 
    bg = COLOR_SEQ % ( 40 + ( background % 8 ) ) if not background is None else '' 
    
    return '{foreground}{background}{underline}{bold}{blink}{what}{reset}'.format (
        foreground = fg        , 
        background = bg        ,
        underline  = ULINE_SEQ ,
        bold       = BOLD_SEQ  , 
        blink      = BLINK_SEQ , 
        what       = what      ,
        reset      = RESET_SEQ )

# =============================================================================
## attention!
def attention ( what ) :
    """Attention string """
    return colored_string ( what                ,
                            foreground = YELLOW ,
                            background = RED    ,
                            bold       = True   ,
                            blink      = True   ,
                            underline  = True   )

# =============================================================================
## allright 
def allright ( what ) :
    """Allright string """
    return colored_string ( what                ,
                            foreground = YELLOW ,
                            background = GREEN  ,
                            bold       = True   ,
                            blink      = False  ,
                            underline  = False  )

# =============================================================================
## make colors 
def make_colors () :
    """Colorize logging
    """
    if with_colors() : return
    if not isatty () : return  ## no colorization for non-TTY output 
    
    global __with_colors__
    __with_colors__ = True 
    
    def  makeName ( level , fg = None  , bg = None , blink = False , underline = False ) :

        name = logging.getLevelName ( level )
        bold = fg is None and bg is None and not uderline 
        return colored_string ( name , fg , bg , bold , blink , underline ) 
    
    logging.addLevelName ( logging.CRITICAL ,  makeName ( logging.CRITICAL , fg = RED    , bg  = BLUE   , blink     = True ) )
    logging.addLevelName ( logging.WARNING  ,  makeName ( logging.WARNING  , fg = RED    , bg  = YELLOW , underline = True ) )
    logging.addLevelName ( logging.ERROR    ,  makeName ( logging.ERROR    , fg = YELLOW , bg  = RED    , blink     = True ) )
    logging.addLevelName ( logging.INFO     ,  makeName ( logging.INFO     , bg = BLUE   , fg  = WHITE  ) )
    logging.addLevelName ( logging.DEBUG    ,  makeName ( logging.DEBUG    , bg = GREEN  , fg  = WHITE  ) )

    return with_colors() 

# =============================================================================
## @class ColorLogging
#  Simple context manager to swicth on coloring
#  @code
#  with ColorLogging():
#      ... do something ... 
#  @endcode 
class ColorLogging(object) :
    """Simple context manager to swith on coloring
    
    >>> with ColorLogging() :
    ...     do something ... 
    """
    def __init__  ( self , color = True ) :
        self.color = color 
        
    def __enter__ ( self ) :
        self.with_color = with_colors() 
        if   self.color      and not self.with_color  : make_colors ()
        elif self.with_color and not self.color       : reset_colors ()
        return self
    
    def __exit__  ( self , *_ ) :
        if   self.color      and not self.with_color  : reset_colors ()
        elif self.with_color and not self.color       : make_colors ()

# =============================================================================
## simple context manager to switch on color logging 
#  @code
#  with logColor() :
#      ... do something ... 
#  @endcode 
def logColor ( color = True ) :
    """Simple context manager to switch on coloring
    
    >>> with logColor () :
    ...     do something ... 
    """
    return ColorLogging ( color )

# =============================================================================
## simple context manager to switch off color logging 
#  @code
#  with logNoColor() :
#      ... do something ... 
#  @endcode 
def logNoColor () :
    """Simple context manager to switch on coloring
    
    >>> with logNoColor () :
    ...     do something ... 
    """
    return ColorLogging ( False )

# =============================================================================
## simple context manager to switch off color logging 
#  @code
#  with noColor() :
#      ... do something ... 
#  @endcode 
def noColor () :
    """Simple context manager to switch on coloring
    
    >>> with noColor () :
    ...     do something ... 
    """
    return ColorLogging ( False )


# =============================================================================
## @class KeepColorLogging
#  Simple context manager to preserve coloring
#  @code
#  with KeepColorLogging():
#      ... do something ... 
#  @endcode 
class KeepColorLogging(object) :
    """Simple context manager to preserve coloring
    
    >>> with KeepColorLogging() :
    ...     do something ... 
    """
    def __enter__ ( self ) :
        self.with_color = with_colors() 
        return self
    
    def __exit__  ( self , *_ ) :
        if   self.with_color and not with_colors()   :  make_colors ()
        elif with_colors()   and not self.with_color : reset_colors ()

# =============================================================================
## simple context manager to preserve color logging 
#  @code
#  with keepColor() :
#      ... do something ... 
#  @endcode 
def keepColor () :
    """Simple context manager to preserve color logging 
    
    >>> with keepColor () :
    ...     do something ... 
    """
    return KeepColorLogging ()


## reset colors
## for ipython mode and TTY output activate colors 
## if with_ipython() and isatty  () :
if isatty  () :
    make_colors()
    
## define default logging thresholds as 'INFO'
setLogging ( 3 )

# =============================================================================
if __name__ == '__main__' :

    setLogging ( 0 )
    
    logger = getLogger ( 'ostap.logger')

    from ostap.utils.docme import docme
    docme ( __name__ , logger = logger )
 
    logger.verbose  ( 'This is VERBOSE  message'  ) 
    logger.debug    ( 'This is DEBUG    message'  ) 
    logger.info     ( 'This is INFO     message'  ) 
    logger.warning  ( 'This is WARNING  message'  ) 
    logger.error    ( 'This is ERROR    message'  ) 
    logger.fatal    ( 'This is FATAL    message'  ) 
    logger.critical ( 'This is CRITICAL message'  ) 

    with logColor() : 
        
        logger.verbose  ( 'This is VERBOSE  message'  ) 
        logger.debug    ( 'This is DEBUG    message'  ) 
        logger.info     ( 'This is INFO     message'  )
        logger.warning  ( 'This is WARNING  message'  ) 
        logger.error    ( 'This is ERROR    message'  ) 
        logger.fatal    ( 'This is FATAL    message'  ) 
        logger.critical ( 'This is CRITICAL message'  )

        with noColor () : 
            logger.verbose  ( 'This is VERBOSE  message'  ) 
            logger.debug    ( 'This is DEBUG    message'  ) 
            logger.info     ( 'This is INFO     message'  )
            logger.warning  ( 'This is WARNING  message'  ) 
            logger.error    ( 'This is ERROR    message'  ) 
            logger.fatal    ( 'This is FATAL    message'  ) 
            logger.critical ( 'This is CRITICAL message'  )
            
        logger.verbose  ( 'This is VERBOSE  message'  ) 
        logger.debug    ( 'This is DEBUG    message'  ) 
        logger.info     ( 'This is INFO     message'  )
        logger.warning  ( 'This is WARNING  message'  ) 
        logger.error    ( 'This is ERROR    message'  ) 
        logger.fatal    ( 'This is FATAL    message'  ) 
        logger.critical ( 'This is CRITICAL message'  )

    with keepColor() :
        logger.verbose  ( 'This is VERBOSE  message'  ) 
        logger.debug    ( 'This is DEBUG    message'  ) 
        logger.info     ( 'This is INFO     message'  ) 
        logger.warning  ( 'This is WARNING  message'  )

        make_colors()
        
        logger.error    ( 'This is ERROR    message'  ) 
        logger.fatal    ( 'This is FATAL    message'  ) 
        logger.critical ( 'This is CRITICAL message'  ) 
        
    logger.verbose  ( 'This is VERBOSE  message'  ) 
    logger.debug    ( 'This is DEBUG    message'  ) 
    logger.info     ( 'This is INFO     message'  ) 
    logger.warning  ( 'This is WARNING  message'  ) 
    logger.error    ( 'This is ERROR    message'  ) 
    logger.fatal    ( 'This is FATAL    message'  ) 
    logger.critical ( 'This is CRITICAL message'  ) 
    logger.info ( 80*'*'  )

# =============================================================================
# The END 
# =============================================================================
