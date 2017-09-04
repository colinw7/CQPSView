#include <CPSViewI.h>
#include <CMathRound.h>

const int MAX_ARRAY_DIMENSION      = 65535;
const int MAX_DICTIONARY_DIMENSION = 65535;

struct PSViewOperatorData {
  const char         *name;
  PSViewOperatorProc  proc;
};

static PSViewOperatorData
system_operators[] = {
  /*   0 */
  { "abs"                     , PSViewOperatorMgr::absOp                      },
  { "add"                     , PSViewOperatorMgr::addOp                      },
  { "aload"                   , PSViewOperatorMgr::aloadOp                    },
  { "anchorsearch"            , PSViewOperatorMgr::anchorSearchOp             },
  { "and"                     , PSViewOperatorMgr::andOp                      },
  /*   5 */
  { "arc"                     , PSViewOperatorMgr::arcOp                      },
  { "arcn"                    , PSViewOperatorMgr::arcNOp                     },
  { "arct"                    , PSViewOperatorMgr::arcTOp                     },
  { "arcto"                   , PSViewOperatorMgr::arcToOp                    },
  { "array"                   , PSViewOperatorMgr::arrayOp                    },
  /*  10 */
  { "ashow"                   , PSViewOperatorMgr::ashowOp                    },
  { "astore"                  , PSViewOperatorMgr::astoreOp                   },
  { "awidthshow"              , PSViewOperatorMgr::awidthShowOp               },
  { "begin"                   , PSViewOperatorMgr::beginOp                    },
  { "bind"                    , PSViewOperatorMgr::bindOp                     },
  /*  15 */
  { "bitshift"                , PSViewOperatorMgr::bitShiftOp                 },
  { "ceiling"                 , PSViewOperatorMgr::ceilingOp                  },
  { "charpath"                , PSViewOperatorMgr::charPathOp                 },
  { "clear"                   , PSViewOperatorMgr::clearOp                    },
  { "cleartomark"             , PSViewOperatorMgr::clearToMarkOp              },
  /*  20 */
  { "clip"                    , PSViewOperatorMgr::clipOp                     },
  { "clippath"                , PSViewOperatorMgr::clipPathOp                 },
  { "closepath"               , PSViewOperatorMgr::closePathOp                },
  { "concat"                  , PSViewOperatorMgr::concatOp                   },
  { "concatmatrix"            , PSViewOperatorMgr::concatMatrixOp             },
  /*  25 */
  { "copy"                    , PSViewOperatorMgr::copyOp                     },
  { "count"                   , PSViewOperatorMgr::countOp                    },
  { "counttomark"             , PSViewOperatorMgr::countToMarkOp              },
  { "currentcmykcolor"        , PSViewOperatorMgr::currentCMYKColorOp         },
  { "currentdash"             , PSViewOperatorMgr::currentDashOp              },
  /*  30 */
  { "currentdict"             , PSViewOperatorMgr::currentDictOp              },
  { "currentfile"             , PSViewOperatorMgr::currentFileOp              },
  { "currentfont"             , PSViewOperatorMgr::currentFontOp              },
  { "currentgray"             , PSViewOperatorMgr::currentGrayOp              },
  { "currentgstate"           , PSViewOperatorMgr::currentGStateOp            },
  /*  35 */
  { "currenthsbcolor"         , PSViewOperatorMgr::currentHSBColorOp          },
  { "currentlinecap"          , PSViewOperatorMgr::currentLineCapOp           },
  { "currentlinejoin"         , PSViewOperatorMgr::currentLineJoinOp          },
  { "currentlinewidth"        , PSViewOperatorMgr::currentLineWidthOp         },
  { "currentmatrix"           , PSViewOperatorMgr::currentMatrixOp            },
  /*  40 */
  { "currentpoint"            , PSViewOperatorMgr::currentPointOp             },
  { "currentrgbcolor"         , PSViewOperatorMgr::currentRGBColorOp          },
  { "currentshared"           , PSViewOperatorMgr::unimplementedOp            },
  { "curveto"                 , PSViewOperatorMgr::curveToOp                  },
  { "cvi"                     , PSViewOperatorMgr::cviOp                      },
  /*  45 */
  { "cvlit"                   , PSViewOperatorMgr::cvlitOp                    },
  { "cvn"                     , PSViewOperatorMgr::cvnOp                      },
  { "cvr"                     , PSViewOperatorMgr::cvrOp                      },
  { "cvrs"                    , PSViewOperatorMgr::cvrsOp                     },
  { "cvs"                     , PSViewOperatorMgr::cvsOp                      },
  /*  50 */
  { "cvx"                     , PSViewOperatorMgr::cvxOp                      },
  { "def"                     , PSViewOperatorMgr::defOp                      },
  { "defineusername"          , PSViewOperatorMgr::unimplementedOp            },
  { "dict"                    , PSViewOperatorMgr::dictOp                     },
  { "div"                     , PSViewOperatorMgr::divOp                      },
  /*  55 */
  { "dtransform"              , PSViewOperatorMgr::dtransformOp               },
  { "dup"                     , PSViewOperatorMgr::dupOp                      },
  { "end"                     , PSViewOperatorMgr::endOp                      },
  { "eoclip"                  , PSViewOperatorMgr::eoclipOp                   },
  { "eofill"                  , PSViewOperatorMgr::eofillOp                   },
  /*  60 */
  { "eoviewclip"              , PSViewOperatorMgr::unimplementedOp            },
  { "eq"                      , PSViewOperatorMgr::eqOp                       },
  { "exch"                    , PSViewOperatorMgr::exchOp                     },
  { "exec"                    , PSViewOperatorMgr::execOp                     },
  { "exit"                    , PSViewOperatorMgr::exitOp                     },
  /*  65 */
  { "file"                    , PSViewOperatorMgr::fileOp                     },
  { "fill"                    , PSViewOperatorMgr::fillOp                     },
  { "findfont"                , PSViewOperatorMgr::findFontOp                 },
  { "flattenpath"             , PSViewOperatorMgr::flattenPathOp              },
  { "floor"                   , PSViewOperatorMgr::floorOp                    },
  /*  70 */
  { "flush"                   , PSViewOperatorMgr::flushOp                    },
  { "flushfile"               , PSViewOperatorMgr::flushFileOp                },
  { "for"                     , PSViewOperatorMgr::forOp                      },
  { "forall"                  , PSViewOperatorMgr::forAllOp                   },
  { "ge"                      , PSViewOperatorMgr::geOp                       },
  /*  75 */
  { "get"                     , PSViewOperatorMgr::getOp                      },
  { "getinterval"             , PSViewOperatorMgr::getIntervalOp              },
  { "grestore"                , PSViewOperatorMgr::grestoreOp                 },
  { "gsave"                   , PSViewOperatorMgr::gsaveOp                    },
  { "gstate"                  , PSViewOperatorMgr::gstateOp                   },
  /*  80 */
  { "gt"                      , PSViewOperatorMgr::gtOp                       },
  { "identmatrix"             , PSViewOperatorMgr::identMatrixOp              },
  { "idiv"                    , PSViewOperatorMgr::idivOp                     },
  { "idtransform"             , PSViewOperatorMgr::idtransformOp              },
  { "if"                      , PSViewOperatorMgr::ifOp                       },
  /*  85 */
  { "ifelse"                  , PSViewOperatorMgr::ifelseOp                   },
  { "image"                   , PSViewOperatorMgr::imageOp                    },
  { "imagemask"               , PSViewOperatorMgr::imageMaskOp                },
  { "index"                   , PSViewOperatorMgr::indexOp                    },
  { "ineofill"                , PSViewOperatorMgr::unimplementedOp            },
  /*  90 */
  { "infill"                  , PSViewOperatorMgr::unimplementedOp            },
  { "initviewclip"            , PSViewOperatorMgr::unimplementedOp            },
  { "inueofill"               , PSViewOperatorMgr::unimplementedOp            },
  { "inufill"                 , PSViewOperatorMgr::unimplementedOp            },
  { "invertmatrix"            , PSViewOperatorMgr::invertMatrixOp             },
  /*  95 */
  { "itransform"              , PSViewOperatorMgr::itransformOp               },
  { "known"                   , PSViewOperatorMgr::knownOp                    },
  { "le"                      , PSViewOperatorMgr::leOp                       },
  { "length"                  , PSViewOperatorMgr::lengthOp                   },
  { "lineto"                  , PSViewOperatorMgr::lineToOp                   },
  /* 100 */
  { "load"                    , PSViewOperatorMgr::loadOp                     },
  { "loop"                    , PSViewOperatorMgr::loopOp                     },
  { "lt"                      , PSViewOperatorMgr::ltOp                       },
  { "makefont"                , PSViewOperatorMgr::makeFontOp                 },
  { "matrix"                  , PSViewOperatorMgr::matrixOp                   },
  /* 105 */
  { "maxlength"               , PSViewOperatorMgr::maxLengthOp                },
  { "mod"                     , PSViewOperatorMgr::modOp                      },
  { "moveto"                  , PSViewOperatorMgr::moveToOp                   },
  { "mul"                     , PSViewOperatorMgr::mulOp                      },
  { "ne"                      , PSViewOperatorMgr::neOp                       },
  /* 110 */
  { "neg"                     , PSViewOperatorMgr::negOp                      },
  { "newpath"                 , PSViewOperatorMgr::newPathOp                  },
  { "not"                     , PSViewOperatorMgr::notOp                      },
  { "null"                    , PSViewOperatorMgr::nullOp                     },
  { "or"                      , PSViewOperatorMgr::orOp                       },
  /* 115 */
  { "pathbbox"                , PSViewOperatorMgr::pathBBoxOp                 },
  { "pathforall"              , PSViewOperatorMgr::pathForAllOp               },
  { "pop"                     , PSViewOperatorMgr::popOp                      },
  { "print"                   , PSViewOperatorMgr::printOp                    },
  { "printobject"             , PSViewOperatorMgr::unimplementedOp            },
  /* 120 */
  { "put"                     , PSViewOperatorMgr::putOp                      },
  { "putinterval"             , PSViewOperatorMgr::putIntervalOp              },
  { "rcurveto"                , PSViewOperatorMgr::rcurveToOp                 },
  { "read"                    , PSViewOperatorMgr::readOp                     },
  { "readhexstring"           , PSViewOperatorMgr::readHexStringOp            },
  /* 125 */
  { "readline"                , PSViewOperatorMgr::readLineOp                 },
  { "readstring"              , PSViewOperatorMgr::readStringOp               },
  { "rectclip"                , PSViewOperatorMgr::rectClipOp                 },
  { "rectfill"                , PSViewOperatorMgr::rectFillOp                 },
  { "rectstroke"              , PSViewOperatorMgr::rectStrokeOp               },
  /* 130 */
  { "rectviewclip"            , PSViewOperatorMgr::unimplementedOp            },
  { "repeat"                  , PSViewOperatorMgr::repeatOp                   },
  { "restore"                 , PSViewOperatorMgr::restoreOp                  },
  { "rlineto"                 , PSViewOperatorMgr::rlineToOp                  },
  { "rmoveto"                 , PSViewOperatorMgr::rmoveToOp                  },
  /* 135 */
  { "roll"                    , PSViewOperatorMgr::rollOp                     },
  { "rotate"                  , PSViewOperatorMgr::rotateOp                   },
  { "round"                   , PSViewOperatorMgr::roundOp                    },
  { "save"                    , PSViewOperatorMgr::saveOp                     },
  { "scale"                   , PSViewOperatorMgr::scaleOp                    },
  /* 140 */
  { "scalefont"               , PSViewOperatorMgr::scaleFontOp                },
  { "search"                  , PSViewOperatorMgr::searchOp                   },
  { "selectfont"              , PSViewOperatorMgr::selectFontOp               },
  { "setbbox"                 , PSViewOperatorMgr::unimplementedOp            },
  { "setcachedevice"          , PSViewOperatorMgr::setCacheDeviceOp           },
  /* 145 */
  { "setcachedevice2"         , PSViewOperatorMgr::unimplementedOp            },
  { "setcharwidth"            , PSViewOperatorMgr::setCharWidthOp             },
  { "setcmykcolor"            , PSViewOperatorMgr::setCMYKColorOp             },
  { "setdash"                 , PSViewOperatorMgr::setDashOp                  },
  { "setfont"                 , PSViewOperatorMgr::setFontOp                  },
  /* 150 */
  { "setgray"                 , PSViewOperatorMgr::setGrayOp                  },
  { "setgstate"               , PSViewOperatorMgr::setGStateOp                },
  { "sethsbcolor"             , PSViewOperatorMgr::setHSBColorOp              },
  { "setlinecap"              , PSViewOperatorMgr::setLineCapOp               },
  { "setlinejoin"             , PSViewOperatorMgr::setLineJoinOp              },
  /* 155 */
  { "setlinewidth"            , PSViewOperatorMgr::setLineWidthOp             },
  { "setmatrix"               , PSViewOperatorMgr::setMatrixOp                },
  { "setrgbcolor"             , PSViewOperatorMgr::setRGBColorOp              },
  { "setshared"               , PSViewOperatorMgr::unimplementedOp            },
  { "shareddict"              , PSViewOperatorMgr::globalDictOp               },
  /* 160 */
  { "show"                    , PSViewOperatorMgr::showOp                     },
  { "showpage"                , PSViewOperatorMgr::showPageOp                 },
  { "stop"                    , PSViewOperatorMgr::stopOp                     },
  { "stopped"                 , PSViewOperatorMgr::stoppedOp                  },
  { "store"                   , PSViewOperatorMgr::storeOp                    },
  /* 165 */
  { "string"                  , PSViewOperatorMgr::stringOp                   },
  { "stringwidth"             , PSViewOperatorMgr::stringWidthOp              },
  { "stroke"                  , PSViewOperatorMgr::strokeOp                   },
  { "strokepath"              , PSViewOperatorMgr::strokePathOp               },
  { "sub"                     , PSViewOperatorMgr::subOp                      },
  /* 170 */
  { "systemdict"              , PSViewOperatorMgr::systemDictOp               },
  { "token"                   , PSViewOperatorMgr::tokenOp                    },
  { "transform"               , PSViewOperatorMgr::transformOp                },
  { "translate"               , PSViewOperatorMgr::translateOp                },
  { "truncate"                , PSViewOperatorMgr::truncateOp                 },
  /* 175 */
  { "type"                    , PSViewOperatorMgr::typeOp                     },
  { "uappend"                 , PSViewOperatorMgr::unimplementedOp            },
  { "ucache"                  , PSViewOperatorMgr::unimplementedOp            },
  { "ueofill"                 , PSViewOperatorMgr::unimplementedOp            },
  { "ufill"                   , PSViewOperatorMgr::unimplementedOp            },
  /* 180 */
  { "undef"                   , PSViewOperatorMgr::undefOp                    },
  { "upath"                   , PSViewOperatorMgr::unimplementedOp            },
  { "userdict"                , PSViewOperatorMgr::userDictOp                 },
  { "ustroke"                 , PSViewOperatorMgr::unimplementedOp            },
  { "viewclip"                , PSViewOperatorMgr::unimplementedOp            },
  /* 185 */
  { "viewclippath"            , PSViewOperatorMgr::unimplementedOp            },
  { "where"                   , PSViewOperatorMgr::whereOp                    },
  { "widthshow"               , PSViewOperatorMgr::widthShowOp                },
  { "write"                   , PSViewOperatorMgr::writeOp                    },
  { "writehexstring"          , PSViewOperatorMgr::writeHexStringOp           },
  /* 190 */
  { "writeobject"             , PSViewOperatorMgr::unimplementedOp            },
  { "writestring"             , PSViewOperatorMgr::writeStringOp              },
  { "wtranslation"            , PSViewOperatorMgr::unimplementedOp            },
  { "xor"                     , PSViewOperatorMgr::xorOp                      },
  { "xshow"                   , PSViewOperatorMgr::xshowOp                    },
  /* 195 */
  { "xyshow"                  , PSViewOperatorMgr::xyshowOp                   },
  { "yshow"                   , PSViewOperatorMgr::yshowOp                    },

  /* ... */

  /* 212 */
  { "execuserobject"          , PSViewOperatorMgr::execUserObjectOp           },
  { "currentcolor"            , PSViewOperatorMgr::currentColorOp             },
  { "currentcolorspace"       , PSViewOperatorMgr::currentColorSpaceOp        },
  /* 215 */
  { "currentglobal"           , PSViewOperatorMgr::currentGlobalOp            },
  { "execform"                , PSViewOperatorMgr::execFormOp                 },
  { "filter"                  , PSViewOperatorMgr::filterOp                   },
  { "findresource"            , PSViewOperatorMgr::findResourceOp             },
  { "globaldict"              , PSViewOperatorMgr::globalDictOp               },
  /* 220 */
  { "makepattern"             , PSViewOperatorMgr::makePatternOp              },
  { "setcolor"                , PSViewOperatorMgr::setColorOp                 },
  { "setcolorspace"           , PSViewOperatorMgr::setColorSpaceOp            },
  { "setglobal"               , PSViewOperatorMgr::setGlobalOp                },
  { "setpagedevice"           , PSViewOperatorMgr::unimplementedOp            },
  /* 225 */
  { "setpattern"              , PSViewOperatorMgr::setPatternOp               },
  { "="                       , PSViewOperatorMgr::print1Op                   },
  { "=="                      , PSViewOperatorMgr::print2Op                   },

  /* ... */

  /* 260 */
  { "["                       , PSViewOperatorMgr::startArrayOp               },
  { "]"                       , PSViewOperatorMgr::endArrayOp                 },
  { "atan"                    , PSViewOperatorMgr::atanOp                     },
  { "banddevice"              , PSViewOperatorMgr::unimplementedOp            },
  { "bytesavailable"          , PSViewOperatorMgr::bytesAvailableOp           },
  /* 265 */
  { "cachestatus"             , PSViewOperatorMgr::unimplementedOp            },
  { "closefile"               , PSViewOperatorMgr::closeFileOp                },
  { "colorimage"              , PSViewOperatorMgr::unimplementedOp            },
  { "condition"               , PSViewOperatorMgr::unimplementedOp            },
  { "copypage"                , PSViewOperatorMgr::unimplementedOp            },
  /* 270 */
  { "cos"                     , PSViewOperatorMgr::cosOp                      },
  { "countdictstack"          , PSViewOperatorMgr::countDictStackOp           },
  { "countexecstack"          , PSViewOperatorMgr::countExecStackOp           },
  { "cshow"                   , PSViewOperatorMgr::unimplementedOp            },
  { "currentblackgeneration"  , PSViewOperatorMgr::currentBlackGenerationOp   },
  /* 275 */
  { "currentcacheparams"      , PSViewOperatorMgr::unimplementedOp            },
  { "currentcolorscreen"      , PSViewOperatorMgr::unimplementedOp            },
  { "currentcolortransfer"    , PSViewOperatorMgr::unimplementedOp            },
  { "currentcontext"          , PSViewOperatorMgr::unimplementedOp            },
  { "currentflat"             , PSViewOperatorMgr::currentFlatOp              },
  /* 280 */
  { "currenthalftone"         , PSViewOperatorMgr::unimplementedOp            },
  { "currenthalftonephase"    , PSViewOperatorMgr::unimplementedOp            },
  { "currentmiterlimit"       , PSViewOperatorMgr::currentMitreLimitOp        },
  { "currentobjectformat"     , PSViewOperatorMgr::unimplementedOp            },
  { "currentpacking"          , PSViewOperatorMgr::currentPackingOp           },
  /* 285 */
  { "currentscreen"           , PSViewOperatorMgr::unimplementedOp            },
  { "currentstrokeadjust"     , PSViewOperatorMgr::currentStrokeAdjustOp      },
  { "currenttransfer"         , PSViewOperatorMgr::unimplementedOp            },
  { "currentundercolorremoval", PSViewOperatorMgr::currentUnderColorRemovalOp },
  { "defaultmatrix"           , PSViewOperatorMgr::defaultMatrixOp            },
  /* 290 */
  { "definefont"              , PSViewOperatorMgr::defineFontOp               },
  { "deletefile"              , PSViewOperatorMgr::deleteFileOp               },
  { "detach"                  , PSViewOperatorMgr::unimplementedOp            },
  { "deviceinfo"              , PSViewOperatorMgr::unimplementedOp            },
  { "dictstack"               , PSViewOperatorMgr::dictStackOp                },
  /* 295 */
  { "echo"                    , PSViewOperatorMgr::echoOp                     },
  { "erasepage"               , PSViewOperatorMgr::erasePageOp                },

  /* ... */

  /* 298 */
  { "execstack"               , PSViewOperatorMgr::execStackOp                },
  { "executeonly"             , PSViewOperatorMgr::executeOnlyOp              },
  /* 300 */
  { "exp"                     , PSViewOperatorMgr::expOp                      },
  { "false"                   , PSViewOperatorMgr::falseOp                    },
  { "filenameforall"          , PSViewOperatorMgr::filenameForAllOp           },
  { "fileposition"            , PSViewOperatorMgr::filePositionOp             },
  { "fork"                    , PSViewOperatorMgr::unimplementedOp            },
  /* 305 */
  { "framedevice"             , PSViewOperatorMgr::unimplementedOp            },
  { "grestoreall"             , PSViewOperatorMgr::grestoreAllOp              },
  { "handleerror"             , PSViewOperatorMgr::handleErrorOp              },
  { "initclip"                , PSViewOperatorMgr::initClipOp                 },
  { "initgraphics"            , PSViewOperatorMgr::initGraphicsOp             },
  /* 310 */
  { "initmatrix"              , PSViewOperatorMgr::initMatrixOp               },
  { "instroke"                , PSViewOperatorMgr::unimplementedOp            },
  { "inustroke"               , PSViewOperatorMgr::unimplementedOp            },
  { "join"                    , PSViewOperatorMgr::unimplementedOp            },
  { "kshow"                   , PSViewOperatorMgr::kshowOp                    },
  /* 315 */
  { "ln"                      , PSViewOperatorMgr::lnOp                       },
  { "lock"                    , PSViewOperatorMgr::unimplementedOp            },
  { "log"                     , PSViewOperatorMgr::logOp                      },
  { "mark"                    , PSViewOperatorMgr::markOp                     },
  { "monitor"                 , PSViewOperatorMgr::unimplementedOp            },
  /* 320 */
  { "noaccess"                , PSViewOperatorMgr::noAccessOp                 },
  { "notify"                  , PSViewOperatorMgr::unimplementedOp            },
  { "nulldevice"              , PSViewOperatorMgr::unimplementedOp            },
  { "packedarray"             , PSViewOperatorMgr::packedArrayOp              },
  { "quit"                    , PSViewOperatorMgr::quitOp                     },
  /* 325 */
  { "rand"                    , PSViewOperatorMgr::randOp                     },
  { "rcheck"                  , PSViewOperatorMgr::rcheckOp                   },
  { "readonly"                , PSViewOperatorMgr::readOnlyOp                 },
  { "realtime"                , PSViewOperatorMgr::realTimeOp                 },
  { "renamefile"              , PSViewOperatorMgr::renameFileOp               },
  /* 330 */
  { "renderbands"             , PSViewOperatorMgr::unimplementedOp            },
  { "resetfile"               , PSViewOperatorMgr::resetFileOp                },
  { "reversepath"             , PSViewOperatorMgr::reversePathOp              },
  { "rootfont"                , PSViewOperatorMgr::rootFontOp                 },
  { "rrand"                   , PSViewOperatorMgr::rrandOp                    },
  /* 335 */
  { "run"                     , PSViewOperatorMgr::runOp                      },
  { "scheck"                  , PSViewOperatorMgr::gcheckOp                   },
  { "setblackgeneration"      , PSViewOperatorMgr::setBlackGenerationOp       },
  { "setcachelimit"           , PSViewOperatorMgr::unimplementedOp            },
  { "setcacheparams"          , PSViewOperatorMgr::unimplementedOp            },
  /* 340 */
  { "setcolorscreen"          , PSViewOperatorMgr::unimplementedOp            },
  { "setcolortransfer"        , PSViewOperatorMgr::unimplementedOp            },
  { "setfileposition"         , PSViewOperatorMgr::setFilePositionOp          },
  { "setflat"                 , PSViewOperatorMgr::setFlatOp                  },
  { "sethalftone"             , PSViewOperatorMgr::unimplementedOp            },
  /* 345 */
  { "sethalftonephase"        , PSViewOperatorMgr::unimplementedOp            },
  { "setmiterlimit"           , PSViewOperatorMgr::setMitreLimitOp            },
  { "setobjectformat"         , PSViewOperatorMgr::unimplementedOp            },
  { "setpacking"              , PSViewOperatorMgr::setPackingOp               },
  { "setscreen"               , PSViewOperatorMgr::unimplementedOp            },
  /* 350 */
  { "setstrokeadjust"         , PSViewOperatorMgr::setStrokeAdjustOp          },
  { "settransfer"             , PSViewOperatorMgr::unimplementedOp            },
  { "setucacheparams"         , PSViewOperatorMgr::unimplementedOp            },
  { "setundercolorremoval"    , PSViewOperatorMgr::setUnderColorRemovalOp     },
  { "sin"                     , PSViewOperatorMgr::sinOp                      },
  /* 355 */
  { "sqrt"                    , PSViewOperatorMgr::sqrtOp                     },
  { "srand"                   , PSViewOperatorMgr::srandOp                    },
  { "stack"                   , PSViewOperatorMgr::stackOp                    },
  { "status"                  , PSViewOperatorMgr::statusOp                   },

  /* ... */

  /* 360 */
  { "true"                    , PSViewOperatorMgr::trueOp                     },
  { "ucachestatus"            , PSViewOperatorMgr::unimplementedOp            },
  { "undefinefont"            , PSViewOperatorMgr::undefineFontOp             },
  { "usertime"                , PSViewOperatorMgr::userTimeOp                 },
  { "ustrokepath"             , PSViewOperatorMgr::unimplementedOp            },
  /* 365 */
  { "version"                 , PSViewOperatorMgr::versionOp                  },
  { "vmreclaim"               , PSViewOperatorMgr::unimplementedOp            },
  { "vmstatus"                , PSViewOperatorMgr::vmStatusOp                 },
  { "wait"                    , PSViewOperatorMgr::unimplementedOp            },
  { "wcheck"                  , PSViewOperatorMgr::wcheckOp                   },
  /* 370 */
  { "xcheck"                  , PSViewOperatorMgr::xcheckOp                   },
  { "yield"                   , PSViewOperatorMgr::unimplementedOp            },
  { "defineuserobject"        , PSViewOperatorMgr::defineUserObjectOp         },
  { "undefineuserobject"      , PSViewOperatorMgr::undefineUserObjectOp       },

  /* ... */

  /* 375 */
  { "cleardictstack"          , PSViewOperatorMgr::clearDictStackOp           },

  /* ... */

  /* 428 */
  { "setvmthreshold"          , PSViewOperatorMgr::unimplementedOp            },
  { "<<"                      , PSViewOperatorMgr::startDictionaryOp          },
  /* 430 */
  { ">>"                      , PSViewOperatorMgr::endDictionaryOp            },
  { "currentcolorrendering"   , PSViewOperatorMgr::unimplementedOp            },
  { "currentdevparams"        , PSViewOperatorMgr::unimplementedOp            },
  { "currentoverprint"        , PSViewOperatorMgr::unimplementedOp            },
  { "currentpagedevice"       , PSViewOperatorMgr::unimplementedOp            },
  /* 435 */
  { "currentsystemparams"     , PSViewOperatorMgr::unimplementedOp            },
  { "currentuserparams"       , PSViewOperatorMgr::unimplementedOp            },
  { "defineresource"          , PSViewOperatorMgr::defineResourceOp           },
  { "findencoding"            , PSViewOperatorMgr::unimplementedOp            },
  { "gcheck"                  , PSViewOperatorMgr::gcheckOp                   },
  /* 440 */
  { "glyphshow"               , PSViewOperatorMgr::glyphShowOp                },
  { "languagelevel"           , PSViewOperatorMgr::languageLevelOp            },
  { "product"                 , PSViewOperatorMgr::productOp                  },
  { "pstack"                  , PSViewOperatorMgr::pstackOp                   },
  { "resourceforall"          , PSViewOperatorMgr::resourceForAllOp           },
  /* 445 */
  { "resourcestatus"          , PSViewOperatorMgr::unimplementedOp            },
  { "revision"                , PSViewOperatorMgr::revisionOp                 },
  { "serialnumber"            , PSViewOperatorMgr::serialNumberOp             },
  { "setcolorrendering"       , PSViewOperatorMgr::unimplementedOp            },
  { "setdevparams"            , PSViewOperatorMgr::unimplementedOp            },
  /* 450 */
  { "setoverprint"            , PSViewOperatorMgr::unimplementedOp            },
  { "setsystemparams"         , PSViewOperatorMgr::unimplementedOp            },
  { "setuserparams"           , PSViewOperatorMgr::unimplementedOp            },
  { "startjob"                , PSViewOperatorMgr::unimplementedOp            },
  { "undefineresource"        , PSViewOperatorMgr::unimplementedOp            },

  /* ... */

  /* 478 */
  { "eexec"                   , PSViewOperatorMgr::eexecOp                    },
  { "executive"               , PSViewOperatorMgr::executiveOp                },
  { "internaldict"            , PSViewOperatorMgr::internalDictOp             },
  { "prompt"                  , PSViewOperatorMgr::promptOp                   },
};

static uint num_system_operators = sizeof(system_operators)/sizeof(system_operators[0]);

static PSViewOperatorData
private_operators[] = {
  { "charpathfill"       , PSViewOperatorMgr::charPathFillOp        },
  { "charpathstroketrue" , PSViewOperatorMgr::charPathStrokeTrueOp  },
  { "charpathstrokefalse", PSViewOperatorMgr::charPathStrokeFalseOp },
};

static uint num_private_operators = sizeof(private_operators)/sizeof(private_operators[0]);

static char base_chars_[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

PSViewOperatorMgr::
PSViewOperatorMgr(CPSView *psview) :
 psview_               (psview),
 random_seed_          (0),
 system_operators_     (),
 private_operators_    (),
 num_private_operators_(0)
{
}

PSViewOperatorMgr::
~PSViewOperatorMgr()
{
  uint num_system_operators = system_operators_.size();

  for (uint i = 0; i < num_system_operators; ++i)
    delete system_operators_[i];

  uint num_private_operators = private_operators_.size();

  for (uint i = 0; i < num_private_operators; ++i)
    delete private_operators_[i];
}

void
PSViewOperatorMgr::
init()
{
  random_seed_ = 1;

  system_operators_.resize(num_system_operators);

  for (uint i = 0; i < num_system_operators; ++i)
    system_operators_[i] =
      new PSViewOperator(this, system_operators[i].name, system_operators[i].proc);

  private_operators_.resize(num_private_operators);

  for (uint i = 0; i < num_private_operators; ++i)
    private_operators_[i] =
      new PSViewOperator(this, private_operators[i].name, private_operators[i].proc);
}

PSViewOperator *
PSViewOperatorMgr::
lookup(const string &name)
{
  uint num_system_operators = system_operators_.size();

  for (uint i = 0; i < num_system_operators; ++i)
    if (system_operators_[i]->getName().getString() == name)
      return system_operators_[i];

  uint num_private_operators = private_operators_.size();

  for (uint i = 0; i < num_private_operators; ++i)
    if (private_operators_[i]->getName().getString() == name)
      return private_operators_[i];

  CTHROW("Unknown Operator Name");

  return NULL;
}

void
PSViewOperatorMgr::
addSystemOperators(PSViewDictionaryToken *dictionary)
{
  uint num_system_operators = system_operators_.size();

  for (uint i = 0; i < num_system_operators; ++i) {
    PSViewOperatorToken *value = new PSViewOperatorToken(psview_, system_operators_[i]);

    dictionary->addValue(system_operators_[i]->getName(), value);
  }
}

void
PSViewOperatorMgr::
startArrayOp(PSViewOperatorMgr *mgr)
{
  markOp(mgr);
}

void
PSViewOperatorMgr::
endArrayOp(PSViewOperatorMgr *mgr)
{
  mgr->endArrayOp();
}

void
PSViewOperatorMgr::
endArrayOp()
{
  int num_tokens = psview_->getOperandStack()->countToMark();

  if (num_tokens < 0) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNMATCHED_MARK);
    return;
  }

  PSViewArrayToken *array_token = new PSViewArrayToken(psview_, num_tokens);

  for (int i = num_tokens; i > 0; i--) {
    PSViewToken *sub_token = psview_->getOperandStack()->pop();

    array_token->setValue(i, sub_token);
  }

  psview_->getOperandStack()->pop();

  psview_->getOperandStack()->push(array_token);
}

void
PSViewOperatorMgr::
startDictionaryOp(PSViewOperatorMgr *mgr)
{
  markOp(mgr);
}

void
PSViewOperatorMgr::
endDictionaryOp(PSViewOperatorMgr *mgr)
{
  mgr->endDictionaryOp();
}

void
PSViewOperatorMgr::
endDictionaryOp()
{
  PSVinteger num_tokens = psview_->getOperandStack()->countToMark();

  if (num_tokens < 0) {
    psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNMATCHED_MARK);
    return;
  }

  if (num_tokens > 0) {
    if (num_tokens % 2 == 1) {
      psview_->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
      return;
    }

    num_tokens /= 2;

    PSViewDictionaryToken *token =
      new PSViewDictionaryToken(psview_->getDictionaryMgr(), num_tokens);

    vector<PSViewToken *> keys;
    vector<PSViewToken *> values;

    keys  .resize(num_tokens);
    values.resize(num_tokens);

    for (int i = num_tokens - 1; i >= 0; i--) {
      values[i] = psview_->getOperandStack()->pop();
      keys  [i] = psview_->getOperandStack()->pop();
    }

    for (int i = 0; i < num_tokens; i++)
      token->addValue(keys[i], values[i]);

    psview_->getOperandStack()->pop();

    psview_->getOperandStack()->push(token);
  }
  else
    psview_->getOperandStack()->pop();
}

void
PSViewOperatorMgr::
print1Op(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  CStrUtil::printf("%s\n", token->toString().c_str());
}

void
PSViewOperatorMgr::
print2Op(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  token->print();

  CStrUtil::printf("\n");
}

void
PSViewOperatorMgr::
absOp(PSViewOperatorMgr *mgr)
{
  PSViewToken  *token1;

  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  if (token->isInteger()) {
    PSVinteger integer = token->getIntegerValue();

    if (integer != LONG_MIN)
      token1 = new PSViewIntegerToken(mgr->getPSView(), labs(integer));
    else
      token1 = new PSViewRealToken(mgr->getPSView(), fabs((double) integer));
  }
  else {
    PSVreal real = token->getRealValue();

    token1 = new PSViewRealToken(mgr->getPSView(), fabs(real));
  }

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
addOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewToken *token;

  if (token1->isInteger() && token2->isInteger()) {
    PSVinteger result = token1->getIntegerValue() + token2->getIntegerValue();

    token = new PSViewIntegerToken(mgr->getPSView(), result);
  }
  else {
    PSVreal result = token1->getRealValue() + token2->getRealValue();

    token = new PSViewRealToken(mgr->getPSView(), result);
  }

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
aloadOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if ((! token->isArray() && ! token->isPackedArray()) || ! token->isLiteral()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  if (token->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token;

    PSVinteger num_tokens = array_token->getNumValues();

    for (int i = 1; i <= num_tokens; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      mgr->getPSView()->getOperandStack()->push(sub_token);
    }
  }
  else {
    PSViewPackedArrayToken *array_token = (PSViewPackedArrayToken *) token;

    PSVinteger num_tokens = array_token->getNumValues();

    for (int i = 1; i <= num_tokens; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      mgr->getPSView()->getOperandStack()->push(sub_token);
    }
  }

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
anchorSearchOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isString() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  PSVinteger len1 = string_token1->getLength();
  PSVinteger len2 = string_token2->getLength();

  if (len1 >= len2 && string_token1->compareN(string_token2, len2) == 0) {
    PSViewToken *token3 = new PSViewStringToken(*string_token1, 1, len2);
    PSViewToken *token4 = new PSViewStringToken(*string_token1, len2 + 1);

    mgr->getPSView()->getOperandStack()->push(token4);
    mgr->getPSView()->getOperandStack()->push(token3);

    trueOp(mgr);
  }
  else {
    mgr->getPSView()->getOperandStack()->push(token1);

    falseOp(mgr);
  }
}

void
PSViewOperatorMgr::
andOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if ((! token1->isBoolean() && ! token1->isInteger()) ||
      (! token2->isBoolean() && ! token2->isInteger()) ||
      ! token1->isType(token2->getType())) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  if (token1->isBoolean()) {
    PSVboolean boolean1 = token1->getBooleanValue();
    PSVboolean boolean2 = token2->getBooleanValue();

    if (boolean1 & boolean2)
      trueOp(mgr);
    else
      falseOp(mgr);
  }
  else {
    PSVinteger integer1 = token1->getIntegerValue();
    PSVinteger integer2 = token2->getIntegerValue();

    PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), integer1 & integer2);

    mgr->getPSView()->getOperandStack()->push(token);
  }
}

void
PSViewOperatorMgr::
arcOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL || token4 == NULL || token5 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token3->isNumber() || ! token4->isNumber() ||
      ! token5->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSVreal x      = token1->getRealValue();
  PSVreal y      = token2->getRealValue();
  PSVreal r      = token3->getRealValue();
  PSVreal angle1 = token4->getRealValue();
  PSVreal angle2 = token5->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->arc(x, y, r,
                    CMathGen::DegToRad(angle1),
                    CMathGen::DegToRad(angle2));
}

void
PSViewOperatorMgr::
arcNOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL || token4 == NULL || token5 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token3->isNumber() || ! token4->isNumber() ||
      ! token5->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }

  PSVreal x      = token1->getRealValue();
  PSVreal y      = token2->getRealValue();
  PSVreal r      = token3->getRealValue();
  PSVreal angle1 = token4->getRealValue();
  PSVreal angle2 = token5->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->arcN(x, y, r,
                     CMathGen::DegToRad(angle1),
                     CMathGen::DegToRad(angle2));
}

void
PSViewOperatorMgr::
arcTOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL || token4 == NULL || token5 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token3->isNumber() || ! token4->isNumber() ||
      ! token5->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSVreal x1 = token1->getRealValue();
  PSVreal y1 = token2->getRealValue();
  PSVreal x2 = token3->getRealValue();
  PSVreal y2 = token4->getRealValue();
  PSVreal r  = token5->getRealValue();

  PSVreal xt1, yt1, xt2, yt2;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->arcTo(x1, y1, x2, y2, r, &xt1, &yt1, &xt2, &yt2);
}

void
PSViewOperatorMgr::
arcToOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL || token4 == NULL || token5 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token3->isNumber() || ! token4->isNumber() ||
      ! token5->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSVreal x1 = token1->getRealValue();
  PSVreal y1 = token2->getRealValue();
  PSVreal x2 = token3->getRealValue();
  PSVreal y2 = token4->getRealValue();
  PSVreal r  = token5->getRealValue();

  PSVreal xt1, yt1, xt2, yt2;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->arcTo(x1, y1, x2, y2, r, &xt1, &yt1, &xt2, &yt2);

  token1 = new PSViewRealToken(mgr->getPSView(), xt1);
  token2 = new PSViewRealToken(mgr->getPSView(), yt1);
  token3 = new PSViewRealToken(mgr->getPSView(), xt2);
  token4 = new PSViewRealToken(mgr->getPSView(), yt2);

  mgr->getPSView()->getOperandStack()->push(token1);
  mgr->getPSView()->getOperandStack()->push(token2);
  mgr->getPSView()->getOperandStack()->push(token3);
  mgr->getPSView()->getOperandStack()->push(token4);
}

void
PSViewOperatorMgr::
arrayOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL)
    return;

  if (! token1->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger dimension = token1->getIntegerValue();

  if (dimension < 0 || dimension > MAX_ARRAY_DIMENSION) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewArrayToken *array_token = new PSViewArrayToken(mgr->getPSView(), dimension);

  for (int i = 1; i <= dimension; i++) {
    PSViewNullToken *sub_token = new PSViewNullToken(mgr->getPSView());

    array_token->setValue(i, sub_token);
  }

  mgr->getPSView()->getOperandStack()->push(array_token);
}

void
PSViewOperatorMgr::
ashowOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() || ! token3->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double ax = token1->getRealValue();
  double ay = token2->getRealValue();

  PSViewStringToken *string_token3 = (PSViewStringToken *) token3;

  string str = string_token3->toString();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->ashow(ax, ay, str);
}

void
PSViewOperatorMgr::
astoreOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (token == NULL || ! token->isArray() || ! token->isLiteral()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) token;

  PSVinteger num_tokens = array_token->getNumValues();

  for (int i = num_tokens; i >= 1; i--) {
    PSViewToken *sub_token = mgr->getPSView()->getOperandStack()->pop();

    if (sub_token == NULL)
      return;

    array_token->setValue(i, sub_token);
  }

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
atanOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal real1 = token1->getRealValue();
  PSVreal real2 = token2->getRealValue();

  if (real1 == 0.0 && real2 == 0.0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

    return;
  }

  PSVreal result;

  if (real2 == 0.0) {
    if (real1 > 0.0)
      result = 90.0;
    else
      result = 270.0;
  }
  else if (real2 < 0.0)
    result = CMathGen::RadToDeg(atan(real1/real2)) + 180.0;
  else if (real1 < 0.0)
    result = CMathGen::RadToDeg(atan(real1/real2)) + 360.0;
  else
    result = CMathGen::RadToDeg(atan(real1/real2));

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), result);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
awidthShowOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token6 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL || token4 == NULL ||
      token5 == NULL || token6 == NULL)
    return;

  if (! token1->isNumber () || ! token2->isNumber() ||
      ! token3->isInteger() || ! token4->isNumber() ||
      ! token4->isNumber () || ! token6->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double cx = token1->getRealValue();
  double cy = token2->getRealValue();

  int c = token3->getIntegerValue();

  double ax = token4->getRealValue();
  double ay = token5->getRealValue();

  PSViewStringToken *string_token6 = (PSViewStringToken *) token6;

  string str = string_token6->toString();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->awidthShow(cx, cy, c, ax, ay, str);
}

void
PSViewOperatorMgr::
beginOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isDictionary()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewDictionaryToken *dict = (PSViewDictionaryToken *) token;

  mgr->getPSView()->getDictionaryMgr()->beginDictionary(dict);
}

void
PSViewOperatorMgr::
bindOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  mgr->bind1(token);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
bind1(PSViewToken *token)
{
  if (token->isArray() && ! token->getWritable())
    return;

  PSViewArrayToken *array_token = (PSViewArrayToken *) token;

  PSVinteger num_tokens = array_token->getNumValues();

  for (int i = 1; i <= num_tokens; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (sub_token->isName() && sub_token->isExecutable()) {
      PSViewToken *token1 = getPSView()->getDictionaryMgr()->lookup(sub_token);

      if (token1 != NULL && token1->isOperator())
        array_token->setValue(i, token1);
    }
    else if (sub_token->isArray() && sub_token->isExecutable()) {
      bind1(sub_token);

      sub_token->setReadOnly();
    }
    else if (sub_token->isPackedArray() && sub_token->isExecutable())
      bind1(sub_token);
  }
}

void
PSViewOperatorMgr::
bitShiftOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isInteger() || ! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger integer1 = token1->getIntegerValue();
  PSVinteger integer2 = token2->getIntegerValue();

  PSVinteger result;

  if (integer2 >= 0)
    result = integer1 <<  integer2;
  else
    result = integer1 >> -integer2;

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), result);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
bytesAvailableOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken *file_token = (PSViewFileToken *) token;

  PSVinteger size = file_token->bytesAvailable();

  PSViewIntegerToken *token1 = new PSViewIntegerToken(mgr->getPSView(), size);

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
ceilingOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token->isReal()) {
    PSVreal real = token->getRealValue();

    PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), ceil(real));

    mgr->getPSView()->getOperandStack()->push(token1);
  }
  else {
    PSViewToken *token1 = token->dup();

    mgr->getPSView()->getOperandStack()->push(token1);
  }
}

void
PSViewOperatorMgr::
charPathOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isString() || ! token2->isBoolean()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

  string str = string_token1->toString();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->charPath(str, token2->getBooleanValue());
}

void
PSViewOperatorMgr::
clearOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getOperandStack()->clear();
}

void
PSViewOperatorMgr::
clearDictStackOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getDictionaryMgr()->emptyDictionaryStack();
}

void
PSViewOperatorMgr::
clearToMarkOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  while (token != NULL && token != mgr->getPSView()->getMarkToken())
    token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNMATCHED_MARK);

    return;
  }
}

void
PSViewOperatorMgr::
clipOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->clip();
}

void
PSViewOperatorMgr::
clipPathOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->clipPath();
}

void
PSViewOperatorMgr::
closeFileOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken *file_token = (PSViewFileToken *) token;

  file_token->close();
}

void
PSViewOperatorMgr::
closePathOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->closePath();
}

void
PSViewOperatorMgr::
concatOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  CAutoPtr<CMatrix2D> matrix;

  matrix = token->getMatrix();

  if (matrix == NULL)
    return;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->postMultiplyCTMMatrix(matrix);
}

void
PSViewOperatorMgr::
concatMatrixOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();

  if (token3 == NULL)
    return;

  if (! token3->isMatrix()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();

  if (token2 == NULL)
    return;

  CAutoPtr<CMatrix2D> matrix2;

  matrix2 = token2->getMatrix();

  if (matrix2 == NULL)
    return;

  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL)
    return;

  CAutoPtr<CMatrix2D> matrix1;

  matrix1 = token1->getMatrix();

  if (matrix1 == NULL)
    return;

  CMatrix2D matrix = (*matrix2) * (*matrix1);

  token3->setMatrix(&matrix);

  mgr->getPSView()->getOperandStack()->push(token3);
}

void
PSViewOperatorMgr::
copyOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();

  if (token2 == NULL)
    return;

  if (token2->isInteger()) {
    PSVinteger num_copies = token2->getIntegerValue();

    if (num_copies < 0) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    if (num_copies == 0)
      return;

    PSViewToken **tokens = new PSViewToken * [num_copies*2];

    for (int i = num_copies - 1; i >= 0; i--) {
      tokens[i] = mgr->getPSView()->getOperandStack()->pop();

      if (tokens[i] == NULL) {
        delete [] tokens;
        return;
      }

      tokens[i + num_copies] = tokens[i]->dup();
    }

    for (int i = 0; i < num_copies*2; i++)
      mgr->getPSView()->getOperandStack()->push(tokens[i]);
  }
  else if (token2->isArray() || token2->isDictionary() ||
           token2->isString() || token2->isGState()) {
    PSViewToken  *token1;

    token1 = mgr->getPSView()->getOperandStack()->pop();

    if (! token1->isType(token2->getType()) &&
        (! token1->isPackedArray() || ! token2->isArray())) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    if (token1->isArray()) {
      PSViewArrayToken *array_token1 = (PSViewArrayToken *) token1;
      PSViewArrayToken *array_token2 = (PSViewArrayToken *) token2;

      PSVinteger num_tokens1 = array_token1->getNumValues();
      PSVinteger num_tokens2 = array_token2->getNumValues();

      if (num_tokens1 > num_tokens2) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      for (int i = 1; i <= num_tokens1; i++) {
        PSViewToken *sub_value1 = array_token1->getValue(i);

        PSViewToken *sub_value2 = sub_value1->dup();

        array_token2->setValue(i, sub_value2);
      }

      array_token2->setBounds(1, num_tokens1);

      mgr->getPSView()->getOperandStack()->push(token2);
    }
    else if (token1->isDictionary()) {
      PSViewDictionaryToken *dictionary_token1 = (PSViewDictionaryToken *) token1;
      PSViewDictionaryToken *dictionary_token2 = (PSViewDictionaryToken *) token2;

      PSVinteger num_tokens1 = dictionary_token1->getNumValues();
      PSVinteger num_tokens2 = dictionary_token2->getMaxValues();

      if (num_tokens1 > num_tokens2) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      dictionary_token2->clear();

      for (int i = 1; i <= num_tokens1; i++) {
        PSViewKeyValue *keyval = dictionary_token1->getKeyValue(i);

        PSViewToken *new_key   = keyval->getKey  ()->dup();
        PSViewToken *new_value = keyval->getValue()->dup();

        dictionary_token2->addValue(new_key, new_value);
      }

      mgr->getPSView()->getOperandStack()->push(token2);
    }
    else if (token1->isString()) {
      PSViewStringToken *string_token1 = (PSViewStringToken *) token1;
      PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

      PSVinteger length1 = string_token1->getLength();
      PSVinteger length2 = string_token1->getLength();

      if (length1 > length2) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      for (int i = 1; i <= length1; i++) {
        int c = string_token1->getChar(i);

        string_token2->setChar(i, c);
      }

      string_token2->setBounds(1, length1);

      mgr->getPSView()->getOperandStack()->push(token2);
    }
    else if (token1->isPackedArray()) {
      PSViewPackedArrayToken *array_token1 = (PSViewPackedArrayToken *) token1;
      PSViewArrayToken       *array_token2 = (PSViewArrayToken *) token2;

      PSVinteger num_tokens1 = array_token1->getNumValues();
      PSVinteger num_tokens2 = array_token2->getNumValues();

      if (num_tokens1 > num_tokens2) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      for (int i = 1; i <= num_tokens1; i++) {
        PSViewToken *sub_value1 = array_token1->getValue(i);
        PSViewToken *sub_value2 = sub_value1->dup();

        array_token2->setValue(i, sub_value2);
      }

      array_token2->setBounds(1, num_tokens1);

      mgr->getPSView()->getOperandStack()->push(token2);
    }
    else {
      CStrUtil::eprintf("PSView: Copy not Implemented for Token\n");

      return;
    }
  }
  else
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
}

void
PSViewOperatorMgr::
cosOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal real = token->getRealValue();

  PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), cos(CMathGen::DegToRad(real)));

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
countOp(PSViewOperatorMgr *mgr)
{
  int num_stack = mgr->getPSView()->getOperandStack()->size();

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), num_stack);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
countDictStackOp(PSViewOperatorMgr *mgr)
{
  int num_dictionaries = mgr->getPSView()->getDictionaryMgr()->getNumDictionaries();

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), num_dictionaries);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
countExecStackOp(PSViewOperatorMgr *mgr)
{
  int num_stack = mgr->getPSView()->getExecutionStack()->size();

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), num_stack);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
countToMarkOp(PSViewOperatorMgr *mgr)
{
  int count = mgr->getPSView()->getOperandStack()->countToMark();

  if (count < 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNMATCHED_MARK);

    return;
  }

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), count);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentBlackGenerationOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewToken *token = gstate_token->getBlackGeneration();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentCMYKColorOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token;

  CCMYK cmyk;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->getCMYKColor(cmyk);

  token = new PSViewRealToken(mgr->getPSView(), cmyk.getCyan());

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewRealToken(mgr->getPSView(), cmyk.getMagenta());

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewRealToken(mgr->getPSView(), cmyk.getYellow());

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewRealToken(mgr->getPSView(), cmyk.getBlack());

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentColorOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  const PSViewName &color_space = gstate_token->getColorSpace();

  if      (color_space.compare(mgr->getPSView()->getGStateMgr()->getCMYKColorSpace()) == 0)
    currentCMYKColorOp(mgr);
  else if (color_space.compare(mgr->getPSView()->getGStateMgr()->getRGBColorSpace()) == 0)
    currentRGBColorOp(mgr);
  else if (color_space.compare(mgr->getPSView()->getGStateMgr()->getGrayColorSpace()) == 0)
    currentGrayOp(mgr);
}

void
PSViewOperatorMgr::
currentColorSpaceOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  bool pattern = gstate_token->getPatternColor();

  PSViewNameToken *color_space_token = gstate_token->getColorSpaceToken();

  if (pattern) {
    PSViewArrayToken *array_token = new PSViewArrayToken(mgr->getPSView(), 2);

    array_token->setValue(2, color_space_token);

    PSViewNameToken *sub_token = new PSViewNameToken(mgr->getPSView(), "Pattern");

    array_token->setValue(1, sub_token);

    mgr->getPSView()->getOperandStack()->push(array_token);
  }
  else {
    PSViewArrayToken *array_token = new PSViewArrayToken(mgr->getPSView(), 1);

    array_token->setValue(1, color_space_token);

    mgr->getPSView()->getOperandStack()->push(array_token);
  }
}

void
PSViewOperatorMgr::
currentDashOp(PSViewOperatorMgr *mgr)
{
  int           num_dashes;
  const double *dash_array;
  double        dash_offset;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->getDashPattern(&dash_array, &num_dashes, &dash_offset);

  PSViewArrayToken *array_token =
    new PSViewArrayToken(mgr->getPSView(), num_dashes);

  for (int i = 1; i <= num_dashes; i++) {
    PSViewRealToken *sub_token = new PSViewRealToken(mgr->getPSView(), dash_array[i - 1]);

    array_token->setValue(i, sub_token);
  }

  mgr->getPSView()->getOperandStack()->push(array_token);

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), dash_offset);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentDictOp(PSViewOperatorMgr *mgr)
{
  PSViewDictionaryToken *token = mgr->getPSView()->getDictionaryMgr()->getCurrentDictionary();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentFileOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getCurrentFile();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentFlatOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  double flat = gstate_token->getFlat();

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), flat);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentFontOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewToken *token = gstate_token->getCurrentFont();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentGlobalOp(PSViewOperatorMgr *mgr)
{
  if (mgr->getPSView()->getMemoryMgr()->getGlobal())
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
currentGrayOp(PSViewOperatorMgr *mgr)
{
  double gray;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->getGray(&gray);

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), gray);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentGStateOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isGState()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  delete token;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewGStateToken *gstate_token1 = new PSViewGStateToken(*gstate_token);

  mgr->getPSView()->getOperandStack()->push(gstate_token1);
}

void
PSViewOperatorMgr::
currentHSBColorOp(PSViewOperatorMgr *mgr)
{
  CHSB hsb;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->getHSBColor(hsb);

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), hsb.getHue());

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewRealToken(mgr->getPSView(), hsb.getSaturation());

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewRealToken(mgr->getPSView(), hsb.getBrightness());

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentLineCapOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  CLineCapType line_cap = gstate_token->getLineCap();

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), line_cap - 1);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentLineJoinOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  CLineJoinType line_join = gstate_token->getLineJoin();

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), line_join);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentLineWidthOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  double line_width = gstate_token->getLineWidth();

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), line_width);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentMatrixOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isMatrix()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  CMatrix2D *matrix = gstate_token->getCTMMatrix();

  token->setMatrix(matrix);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentMitreLimitOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  double mitre_limit = gstate_token->getMitreLimit();

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), mitre_limit);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentPackingOp(PSViewOperatorMgr *mgr)
{
  int packing = mgr->getPSView()->getTokenMgr()->getPacking();

  if (packing)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
currentPointOp(PSViewOperatorMgr *mgr)
{
  double x, y;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  bool flag = gstate_token->getCurrentPoint(&x, &y);

  if (! flag) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);

    return;
  }

  PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), x);
  PSViewRealToken *token2 = new PSViewRealToken(mgr->getPSView(), y);

  mgr->getPSView()->getOperandStack()->push(token1);
  mgr->getPSView()->getOperandStack()->push(token2);
}

void
PSViewOperatorMgr::
currentRGBColorOp(PSViewOperatorMgr *mgr)
{
  CRGBA rgba;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->getRGBColor(rgba);

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), rgba.getRed());

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewRealToken(mgr->getPSView(), rgba.getGreen());

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewRealToken(mgr->getPSView(), rgba.getBlue());

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
currentStrokeAdjustOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  bool flag = gstate_token->getStrokeAdjust();

  if (flag)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
currentUnderColorRemovalOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewToken *token = gstate_token->getUnderColorRemoval();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
curveToOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token6 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL ||
      token4 == NULL || token5 == NULL || token6 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token2->isNumber() || ! token3->isNumber() ||
      ! token3->isNumber() || ! token4->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double x1 = token1->getRealValue();
  double y1 = token2->getRealValue();
  double x2 = token3->getRealValue();
  double y2 = token4->getRealValue();
  double x3 = token5->getRealValue();
  double y3 = token6->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->curveTo(x1, y1, x2, y2, x3, y3);
}

void
PSViewOperatorMgr::
cviOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (token->isInteger())
    mgr->getPSView()->getOperandStack()->push(token);
  else if (token->isReal()) {
    PSVreal real = token->getRealValue();

    PSViewIntegerToken *token1 = new PSViewIntegerToken(mgr->getPSView(), (long) real);

    mgr->getPSView()->getOperandStack()->push(token1);
  }
  else if (token->isString()) {
    PSViewStringToken *string_token = (PSViewStringToken *) token;

    string str = string_token->toString();

    PSVinteger pos = 0;

    PSViewToken *token1 = mgr->readStringFileToken(str, &pos);

    if (token1 == NULL || ! token1->isNumber()) {
      mgr->getPSView()->getErrorMgr()->
        raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

      return;
    }

    PSVreal real = token1->getRealValue();

    PSViewIntegerToken *token2 = new PSViewIntegerToken(mgr->getPSView(), (long) real);

    mgr->getPSView()->getOperandStack()->push(token2);
  }
  else
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
}

void
PSViewOperatorMgr::
cvlitOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  token->setLiteral();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
cvnOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token = (PSViewStringToken *) token;

  string str = string_token->toString();

  PSViewNameToken *token1 = new PSViewNameToken(mgr->getPSView(), str);

  if (token->isLiteral())
    token1->setLiteral();
  else
    token1->setExecutable();

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
cvrOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (token->isReal())
    mgr->getPSView()->getOperandStack()->push(token);
  else if (token->isInteger()) {
    PSVinteger integer = token->getIntegerValue();

    PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), (double) integer);

    mgr->getPSView()->getOperandStack()->push(token1);
  }
  else if (token->isString()) {
    PSViewStringToken *string_token = (PSViewStringToken *) token;

    string str = string_token->toString();

    PSVinteger pos = 0;

    PSViewToken *token1 = mgr->readStringFileToken(str, &pos);

    if (token1 == NULL || ! token1->isNumber()) {
      mgr->getPSView()->getErrorMgr()->
        raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

      return;
    }

    PSVreal real = token1->getRealValue();

    PSViewRealToken *token2 = new PSViewRealToken(mgr->getPSView(), real);

    mgr->getPSView()->getOperandStack()->push(token2);
  }
  else
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
}

void
PSViewOperatorMgr::
cvrsOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (! token1->isNumber() || ! token2->isInteger() || ! token3->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  int base = token2->getIntegerValue();

  if (base < 1 || base > 36) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  if (base == 10) {
    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token3);

    cvsOp(mgr);

    return;
  }

  PSViewStringToken *string_token3 = (PSViewStringToken *) token3;

  size_t length = string_token3->getLength();

  string str;

  uint num = (uint) token1->getRealValue();

  while (num > (uint) base) {
    uint num1 = num/base;
    uint num2 = num - num1*base;

    if (str.size() > length) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    str = base_chars_[num2] + str;

    num = num1;
  }

  if (str.size() > length) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  str = base_chars_[num] + str;

  string_token3->setChars(str, 1);

  PSViewToken *token = string_token3->subString(1, str.size());

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
cvsOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  PSVinteger length = string_token2->getLength();

  string str = token1->toString();

  if (str.size() > (uint) length) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  string_token2->setChars(str, 1);

  PSViewToken *token = string_token2->subString(1, str.size());

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
cvxOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  token->setExecutable();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
defOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  mgr->getPSView()->getDictionaryMgr()->addToCurrent(token1, token2);
}

void
PSViewOperatorMgr::
defaultMatrixOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isMatrix()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  CMatrix2D *matrix = gstate_token->getDefaultMatrix();

  token->setMatrix(matrix);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
defineFontOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token2->isDictionary()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewDictionaryToken *dictionary = (PSViewDictionaryToken *) token2;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->defineFont(token1, dictionary);
}

void
PSViewOperatorMgr::
defineResourceOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL)
    return;

  if (! token3->isName()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewNameToken *name_token = (PSViewNameToken *) token3;

  const PSViewName &name = name_token->getValue();

  if (! mgr->getPSView()->getResourceMgr()->checkInstance(token2, name))
    return;

  mgr->getPSView()->getResourceMgr()->define(token1, token2, name);
}

void
PSViewOperatorMgr::
defineUserObjectOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger ind = token1->getIntegerValue();

  if (ind < 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewDictionaryToken *dict = mgr->getPSView()->getDictionaryMgr()->getUserDictionary();

  PSViewNameToken *key = new PSViewNameToken(mgr->getPSView(), "UserObjects");

  PSViewToken *user_objects = dict->getValue(key);

  if (user_objects != NULL && ! user_objects->isArray()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) user_objects;

  PSVinteger len = 0;

  if (user_objects != NULL)
    len = array_token->getNumValues();

  if (ind >= len) {
    PSViewArrayToken *user_objects1 = new PSViewArrayToken(mgr->getPSView(), ind + 10);

    if (user_objects1 == NULL) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_VM_ERROR);

      return;
    }

    for (int i = 1; i <= len; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      user_objects1->setValue(i, sub_token);
    }

    delete user_objects;

    user_objects = user_objects1;

    array_token = (PSViewArrayToken *) user_objects;

    dict->addValue(key, user_objects);
  }
  else
    delete key;

  array_token->setValue(ind + 1, token2);
}

void
PSViewOperatorMgr::
deleteFileOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token = (PSViewStringToken *) token;

  string filename = string_token->toString();

  remove(filename.c_str());
}

void
PSViewOperatorMgr::
dictOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL)
    return;

  if (! token1->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger max_entries = token1->getIntegerValue();

  if (max_entries < 0 || max_entries > MAX_DICTIONARY_DIMENSION) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewDictionaryToken *token =
    new PSViewDictionaryToken(mgr->getPSView()->getDictionaryMgr(), max_entries);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
dictStackOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL)
    return;

  if (! token1->isArray()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewArrayToken *array_token1 = (PSViewArrayToken *) token1;

  PSVinteger array_size = array_token1->getNumValues();

  int num_dictionaries = mgr->getPSView()->getDictionaryMgr()->getNumDictionaries();

  if (array_size < num_dictionaries) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewArrayToken *array_token = new PSViewArrayToken(mgr->getPSView(), num_dictionaries);

  for (int i = 1; i <= num_dictionaries; i++) {
    PSViewToken *sub_token = mgr->getPSView()->getDictionaryMgr()->getDictionary(i);

    array_token->setValue(i, sub_token);
  }

  mgr->getPSView()->getOperandStack()->push(array_token);
}

void
PSViewOperatorMgr::
divOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal real1 = token1->getRealValue();
  PSVreal real2 = token2->getRealValue();

  if (real2 == 0.0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

    return;
  }

  PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), real1/real2);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
dtransformOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token2->isNumber()) {
    PSVreal x = token1->getRealValue();
    PSVreal y = token2->getRealValue();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    CMatrix2D *matrix = gstate_token->getCTMMatrix();

    PSVreal x1, y1, x2, y2;

    matrix->multiplyPoint(0.0, 0.0, &x1, &y1);
    matrix->multiplyPoint(  x,   y, &x2, &y2);

    token1 = new PSViewRealToken(mgr->getPSView(), x2 - x1);
    token2 = new PSViewRealToken(mgr->getPSView(), y2 - y1);

    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token2);
  }
  else {
    CAutoPtr<CMatrix2D> matrix;

    matrix = token2->getMatrix();

    if (matrix == NULL)
      return;

    PSViewToken *token0 = mgr->getPSView()->getOperandStack()->pop();

    if (! token0->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x = token0->getRealValue();
    PSVreal y = token1->getRealValue();

    PSVreal x1, y1, x2, y2;

    matrix->multiplyPoint(0.0, 0.0, &x1, &y1);
    matrix->multiplyPoint(  x,   y, &x2, &y2);

    token1 = new PSViewRealToken(mgr->getPSView(), x2 - x1);
    token2 = new PSViewRealToken(mgr->getPSView(), y2 - y1);

    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token2);
  }
}

void
PSViewOperatorMgr::
dupOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  PSViewToken *token1 = token->dup();

  mgr->getPSView()->getOperandStack()->push(token);
  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
echoOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isBoolean()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

/*
  PSVboolean boolean = token->getBooleanValue();

  PSViewSetEcho(boolean);
*/
}

void
PSViewOperatorMgr::
endOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getDictionaryMgr()->endDictionary();
}

void
PSViewOperatorMgr::
eexecOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isString() && ! token->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken *file_token;

  if (token->isString()) {
    PSViewStringToken *string_token = (PSViewStringToken *) token;

    string str = string_token->toString();

    file_token = new PSViewStringFileToken(mgr->getPSView(), str);
  }
  else
    file_token = (PSViewFileToken *) token;

  PSViewFilterFileToken *filter_token = new PSViewFilterFileToken(file_token, "EExecDecode");

  mgr->getPSView()->setCurrentFile(filter_token);

  PSViewDictionaryToken *dict = mgr->getPSView()->getDictionaryMgr()->getSystemDictionary();

  mgr->getPSView()->getDictionaryMgr()->beginDictionary(dict);

  mgr->getPSView()->executeCurrentFile();

  mgr->getPSView()->closeCurrentFile();

  mgr->getPSView()->getDictionaryMgr()->endDictionary();
}

void
PSViewOperatorMgr::
eoclipOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->eoclip();
}

void
PSViewOperatorMgr::
eofillOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->eofill();
}

void
PSViewOperatorMgr::
eqOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (token1->compare(token2) == 0)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
erasePageOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->erasePage();
}

void
PSViewOperatorMgr::
exchOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getOperandStack()->exch();
}

void
PSViewOperatorMgr::
execOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  token->execute();
}

void
PSViewOperatorMgr::
execFormOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isDictionary()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewDictionaryToken *dict = (PSViewDictionaryToken *) token;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->execForm(dict);
}

void
PSViewOperatorMgr::
execStackOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL)
    return;

  if (! token1->isArray()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewArrayToken *array_token1 = (PSViewArrayToken *) token1;

  PSVinteger array_size = array_token1->getNumValues();

  PSVinteger num_stack = mgr->getPSView()->getExecutionStack()->size();

  if (array_size < num_stack) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewArrayToken *array_token = new PSViewArrayToken(mgr->getPSView(), num_stack);

  for (int i = 1; i <= num_stack; i++) {
    PSViewToken *sub_token = mgr->getPSView()->getExecutionStack()->peek(i);

    array_token->setValue(i, sub_token);
  }

  mgr->getPSView()->getOperandStack()->push(array_token);
}

void
PSViewOperatorMgr::
execUserObjectOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger ind = token->getIntegerValue();

  if (ind < 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewDictionaryToken *dict = mgr->getPSView()->getDictionaryMgr()->getUserDictionary();

  PSViewNameToken *key = new PSViewNameToken(mgr->getPSView(), "UserObjects");

  PSViewToken *user_objects = dict->getValue(key);

  if (user_objects == NULL || ! user_objects->isArray()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) user_objects;

  PSVinteger len = array_token->getNumValues();

  if (ind >= len) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewToken *sub_token = array_token->getValue(ind + 1);

  sub_token->execute();

  delete key;
}

void
PSViewOperatorMgr::
executeOnlyOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isArray() && ! token->isPackedArray() &&
      ! token->isFile () && ! token->isString     ()){
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  token->setExecuteOnly();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
executiveOp(PSViewOperatorMgr *mgr)
{
  CStrUtil::printf("%s\n", "PSView Executive");
  CStrUtil::printf("\n");

  while (true) {
    PSViewNameToken *key = new PSViewNameToken(mgr->getPSView(), "prompt");

    PSViewToken *token = mgr->getPSView()->getDictionaryMgr()->lookup(key);

    token->execute();

    if (mgr->getPSView()->getErrorMgr()->getError()) {
      mgr->getPSView()->getErrorMgr()->setError(false);
      break;
    }

    string line;

    if (! mgr->getPSView()->getLine(line))
      break;

    PSViewStringFileToken *file_token = new PSViewStringFileToken(mgr->getPSView(), line);

    PSViewToken *token1 = file_token->readToken();

    while (token1 != NULL) {
      if (token1->isProcedure())
        mgr->getPSView()->getOperandStack()->push(token1);
      else
        token1->execute();

      if (mgr->getPSView()->getErrorMgr()->getError()) {
        mgr->getPSView()->getErrorMgr()->setError(false);
        break;
      }

      token1 = file_token->readToken();
    }

    file_token->close();
  }
}

void
PSViewOperatorMgr::
exitOp(PSViewOperatorMgr *mgr)
{
  PSViewOperatorToken *token = new PSViewOperatorToken(mgr->getPSView(), "exit");

  mgr->getPSView()->getExecutionStack()->push(token);

  mgr->getPSView()->setExitFlag(true);
}

void
PSViewOperatorMgr::
expOp(PSViewOperatorMgr *mgr)
{
  PSViewToken  *token;

  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal base     = token1->getRealValue();
  PSVreal exponent = token2->getRealValue();

  bool integer_exponent = false;

  if (token2->isInteger())
    integer_exponent = true;
  else {
    if (exponent == (double)((long) exponent))
      integer_exponent = true;
  }

  if (base == 0.0 && exponent <= 0.0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

    return;
  }

  if (base < 0.0 && ! integer_exponent) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

    return;
  }

  token = new PSViewRealToken(mgr->getPSView(), pow(base, exponent));

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
falseOp(PSViewOperatorMgr *mgr)
{
  PSViewBooleanToken *token = new PSViewBooleanToken(mgr->getPSView(), false);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
fileOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL && token2 == NULL)
    return;

  if (! token1->isString() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  string filename = string_token1->toString();
  string access   = string_token2->toString();

  PSViewTextFileToken *token = new PSViewTextFileToken(mgr->getPSView(), filename, access);

  if (token == NULL) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_FILENAME);

    return;
  }

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
fillOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->fill();
}

void
PSViewOperatorMgr::
filterOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (! token1->isFile() || ! token2->isName()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewFileToken *file_token = (PSViewFileToken *) token1;
  PSViewNameToken *name_token = (PSViewNameToken *) token2;

  const PSViewName &name = name_token->getValue();

  PSViewFilterFileToken *token = new PSViewFilterFileToken(file_token, name);

  if (token == NULL) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);
    return;
  }

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
filenameForAllOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (! token1->isString() || ! token2->isProcedure() || ! token3->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;
  PSViewStringToken *string_token3 = (PSViewStringToken *) token3;

  string templ   = string_token1->toString();
  string scratch = string_token3->toString();

  size_t scratch_len = scratch.size();

  void   *dir;
  string  prefix;

  if (templ.find("%font%") == 0) {
    string font_dir;

    if (! CEnvInst.get("PSVIEW_FONT_DIR", font_dir))
      font_dir = mgr->getPSView()->getGStateMgr()->getFontDir();

    dir = COSFile::openDir(font_dir);

    templ = templ.substr(6);

    prefix = "%font%";
  }
  else
    dir = COSFile::openDir(".");

  CGlob pattern(templ);

  string filename;

  while (COSFile::readDir(dir, filename)) {
    if (filename == "." || filename == "..")
      continue;

    if (! pattern.compare(filename))
      continue;

    size_t len1 = prefix.size();
    size_t len2 = filename.size();

    if (len1 + len2 > scratch_len) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      break;
    }

    size_t j = 0;

    for (size_t i = 0; i < len1; i++)
      scratch[j++] = prefix[i];

    for (size_t i = 0; i < len2; i++)
      scratch[j++] = filename[i];

    string_token3->setChars(scratch, 1);

    PSViewStringToken *string_token = new PSViewStringToken(*string_token3, 1, len1 + len2);

    mgr->getPSView()->getOperandStack()->push(string_token);

    token2->execute();
  }

  COSFile::closeDir(dir);
}

void
PSViewOperatorMgr::
filePositionOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken *file_token = (PSViewFileToken *) token;

  uint pos;

  if (file_token->getPosition(&pos)) {
    PSViewIntegerToken *token1 = new PSViewIntegerToken(mgr->getPSView(), pos);

    mgr->getPSView()->getOperandStack()->push(token1);
  }
}

void
PSViewOperatorMgr::
findFontOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isName()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewToken *font = gstate_token->findFont(token);

  mgr->getPSView()->getOperandStack()->push(font);
}

void
PSViewOperatorMgr::
findResourceOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token2->isName()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewNameToken *name_token = (PSViewNameToken *) token2;

  const PSViewName &name = name_token->getValue();

  PSViewToken *token = mgr->getPSView()->getResourceMgr()->findInstance(token1, name);

  if (token != NULL)
    mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
flattenPathOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->flattenPath();
}

void
PSViewOperatorMgr::
floorOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token->isReal()) {
    PSVreal real = token->getRealValue();

    PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), floor(real));

    mgr->getPSView()->getOperandStack()->push(token1);
  }
  else {
    PSViewToken *token1 = token->dup();

    mgr->getPSView()->getOperandStack()->push(token1);
  }
}

void
PSViewOperatorMgr::
flushOp(PSViewOperatorMgr *)
{
  fflush(stdout);
}

void
PSViewOperatorMgr::
flushFileOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken *file_token = (PSViewFileToken *) token;

  file_token->flush();
}

void
PSViewOperatorMgr::
forOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL || token4 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token3->isNumber() || ! token4->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  if (token1->isReal() || token2->isReal() || token3->isReal()) {
    PSVreal initial   = token1->getRealValue();
    PSVreal increment = token2->getRealValue();
    PSVreal limit     = token3->getRealValue();

    if (increment >= 0) {
      for (PSVreal i = initial; i <= limit; i += increment) {
        PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), i);

        mgr->getPSView()->getOperandStack()->push(token);

        token4->execute();

        if (mgr->getPSView()->getExitFlag()) {
          mgr->getPSView()->getExecutionStack()->popToExit();
          break;
        }
      }
    }
    else {
      for (PSVreal i = initial; i >= limit; i += increment) {
        PSViewRealToken *token = new PSViewRealToken(mgr->getPSView(), i);

        mgr->getPSView()->getOperandStack()->push(token);

        token4->execute();

        if (mgr->getPSView()->getExitFlag()) {
          mgr->getPSView()->getExecutionStack()->popToExit();
          break;
        }
      }
    }
  }
  else {
    PSVinteger initial   = token1->getIntegerValue();
    PSVinteger increment = token2->getIntegerValue();
    PSVinteger limit     = token3->getIntegerValue();

    if (increment >= 0) {
      for (PSVinteger i = initial; i <= limit; i += increment) {
        PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), i);

        mgr->getPSView()->getOperandStack()->push(token);

        token4->execute();

        if (mgr->getPSView()->getExitFlag()) {
          mgr->getPSView()->getExecutionStack()->popToExit();
          break;
        }
      }
    }
    else {
      for (PSVinteger i = initial; i >= limit; i += increment) {
        PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), i);

        mgr->getPSView()->getOperandStack()->push(token);

        token4->execute();

        if (mgr->getPSView()->getExitFlag()) {
          mgr->getPSView()->getExecutionStack()->popToExit();
          break;
        }
      }
    }
  }
}

void
PSViewOperatorMgr::
forAllOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token2->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token1->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token1;

    PSVinteger num_tokens = array_token->getNumValues();

    for (int i = 1; i <= num_tokens; i++) {
      PSViewToken *sub_value = array_token->getValue(i);

      mgr->getPSView()->getOperandStack()->push(sub_value);

      token2->execute();

      if (mgr->getPSView()->getExitFlag()) {
        mgr->getPSView()->getExecutionStack()->popToExit();
        break;
      }
    }
  }
  else if (token1->isPackedArray()) {
    PSViewPackedArrayToken *array_token = (PSViewPackedArrayToken *) token1;

    PSVinteger num_tokens = array_token->getNumValues();

    for (int i = 1; i <= num_tokens; i++) {
      PSViewToken *sub_value = array_token->getValue(i);

      mgr->getPSView()->getOperandStack()->push(sub_value);

      token2->execute();

      if (mgr->getPSView()->getExitFlag()) {
        mgr->getPSView()->getExecutionStack()->popToExit();
        break;
      }
    }
  }
  else if (token1->isString()) {
    PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

    PSVinteger num_tokens = string_token1->getLength();

    for (int i = 1; i <= num_tokens; i++) {
      PSVchar c = string_token1->getChar(i);

      PSViewIntegerToken *sub_value = new PSViewIntegerToken(mgr->getPSView(), c & 0xFF);

      mgr->getPSView()->getOperandStack()->push(sub_value);

      token2->execute();

      if (mgr->getPSView()->getExitFlag()) {
        mgr->getPSView()->getExecutionStack()->popToExit();
        break;
      }
    }
  }
  else if (token1->isDictionary()) {
    PSViewDictionaryToken *dictionary_token1 = (PSViewDictionaryToken *) token1;

    PSVinteger num_tokens = dictionary_token1->getNumValues();

    for (int i = 1; i <= num_tokens; i++) {
      PSViewKeyValue *keyval = dictionary_token1->getKeyValue(i);

      mgr->getPSView()->getOperandStack()->push(keyval->getKey()  );
      mgr->getPSView()->getOperandStack()->push(keyval->getValue());

      token2->execute();

      if (mgr->getPSView()->getExitFlag()) {
        mgr->getPSView()->getExecutionStack()->popToExit();
        break;
      }
    }
  }
  else {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }
}

void
PSViewOperatorMgr::
gcheckOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isComposite() || token->getGlobal())
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
geOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  PSVinteger flag = token1->compare(token2);

  if (flag >= 0)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
getOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (token1->isArray() || token1->isPackedArray() || token1->isString()) {
    if (! token1->getReadable()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);

      return;
    }

    if (! token2->isInteger()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVinteger ind = token2->getIntegerValue();

    PSViewToken *sub_token;

    if (token1->isArray()) {
      PSViewArrayToken *array_token = (PSViewArrayToken *) token1;

      PSVinteger num_tokens = array_token->getNumValues();

      if (ind < 0 || ind >= num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      sub_token = array_token->getValue(ind + 1);
    }
    else if (token1->isPackedArray()) {
      PSViewPackedArrayToken *array_token = (PSViewPackedArrayToken *) token1;

      PSVinteger num_tokens = array_token->getNumValues();

      if (ind < 0 || ind >= num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      sub_token = array_token->getValue(ind + 1);
    }
    else {
      PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

      PSVinteger num_tokens = string_token1->getLength();

      if (ind < 0 || ind >= num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      int c = string_token1->getChar(ind + 1);

      sub_token = new PSViewIntegerToken(mgr->getPSView(), c);
    }

    mgr->getPSView()->getOperandStack()->push(sub_token);
  }
  else if (token1->isDictionary()) {
    PSViewDictionaryToken *dictionary_token1 = (PSViewDictionaryToken *) token1;

    PSViewToken *sub_token = dictionary_token1->getValue(token2);

    if (sub_token == NULL) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

      return;
    }

    mgr->getPSView()->getOperandStack()->push(sub_token);
  }
  else {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }
}

void
PSViewOperatorMgr::
getIntervalOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL)
    return;

  if (! token1->isArray() && ! token1->isPackedArray() &&
      ! token1->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (! token2->isInteger() || ! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger ind   = token2->getIntegerValue();
  PSVinteger count = token3->getIntegerValue();

  if (token1->isArray()) {
    PSViewArrayToken *array_token1 = (PSViewArrayToken *) token1;

    PSVinteger num_tokens = array_token1->getNumValues();

    if (ind < 0 || count < 0 || ind + count > num_tokens) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    PSViewArrayToken *array_token = new PSViewArrayToken(mgr->getPSView(), count);

    for (int i = 1; i <= count; i++) {
      PSViewToken *sub_token = array_token1->getValue(ind + i);

      array_token->setValue(i, sub_token);
    }

    mgr->getPSView()->getOperandStack()->push(array_token);
  }
  else if (token1->isPackedArray()) {
    PSViewPackedArrayToken *array_token1 = (PSViewPackedArrayToken *) token1;

    PSVinteger num_tokens = array_token1->getNumValues();

    if (ind < 0 || count < 0 || ind + count > num_tokens) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    PSViewPackedArrayToken *array_token = new PSViewPackedArrayToken(mgr->getPSView(), count);

    for (int i = 1; i <= count; i++) {
      PSViewToken *sub_token = array_token1->getValue(ind + i);

      array_token->setValue(i, sub_token);
    }

    mgr->getPSView()->getOperandStack()->push(array_token);
  }
  else {
    PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

    PSVinteger num_tokens = string_token1->getLength();

    if (ind < 0 || count < 0 || ind + count > num_tokens) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    PSViewStringToken *token = new PSViewStringToken(mgr->getPSView(), count);

    for (int i = 1; i <= count; i++) {
      int c = string_token1->getChar(ind + i);

      token->setChar(i, c);
    }

    mgr->getPSView()->getOperandStack()->push(token);
  }
}

void
PSViewOperatorMgr::
globalDictOp(PSViewOperatorMgr *mgr)
{
  PSViewDictionaryToken *token = mgr->getPSView()->getDictionaryMgr()->getGlobalDictionary();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
glyphShowOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isName()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewNameToken *name_token = (PSViewNameToken *) token;

  const PSViewName &name = name_token->getValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->glyphShow(name);
}

void
PSViewOperatorMgr::
grestoreOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getGStateTokenMgr()->restore();
}

void
PSViewOperatorMgr::
grestoreAllOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getGStateTokenMgr()->restoreAll();
}

void
PSViewOperatorMgr::
gsaveOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getGStateTokenMgr()->save();
}

void
PSViewOperatorMgr::
gstateOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *token = new PSViewGStateToken(mgr->getPSView());

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
gtOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  PSVinteger flag = token1->compare(token2);

  if (flag > 0)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
handleErrorOp(PSViewOperatorMgr *mgr)
{
  PSViewNameToken *key = new PSViewNameToken(mgr->getPSView(), "errordict");

  PSViewDictionaryToken *dict =
    (PSViewDictionaryToken *) mgr->getPSView()->getDictionaryMgr()->lookup(key);

  PSViewNameToken *key1 = new PSViewNameToken(mgr->getPSView(), "handleerror");

  PSViewToken *value1 = dict->getValue(key1);

  if (value1 != NULL && value1->isNull())
    mgr->getPSView()->getErrorMgr()->defaultErrorHandler();
  else
    value1->execute();
}

void
PSViewOperatorMgr::
identMatrixOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isMatrix()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  CMatrix2D matrix;

  matrix.setIdentity();

  token->setMatrix(&matrix);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
idivOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isInteger() || ! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger integer2 = token2->getIntegerValue();

  if (integer2 == 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

    return;
  }

  PSVinteger integer1 = token1->getIntegerValue();

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), integer1/integer2);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
idtransformOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token2->isNumber()) {
    PSVreal x = token1->getRealValue();
    PSVreal y = token2->getRealValue();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    CMatrix2D *matrix = gstate_token->getInverseCTMMatrix();

    PSVreal x1, y1, x2, y2;

    matrix->multiplyPoint(0.0, 0.0, &x1, &y1);
    matrix->multiplyPoint(  x,   y, &x2, &y2);

    token1 = new PSViewRealToken(mgr->getPSView(), x2 - x1);
    token2 = new PSViewRealToken(mgr->getPSView(), y2 - y1);

    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token2);
  }
  else {
    CAutoPtr<CMatrix2D> matrix;

    matrix = token2->getMatrix();

    if (matrix == NULL)
      return;

    PSViewToken *token0 = mgr->getPSView()->getOperandStack()->pop();

    if (! token0->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x = token0->getRealValue();
    PSVreal y = token1->getRealValue();

    CMatrix2D imatrix;

    if (! matrix->invert(imatrix)) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

      return;
    }

    PSVreal x1, y1, x2, y2;

    matrix->multiplyPoint(0.0, 0.0, &x1, &y1);
    matrix->multiplyPoint(  x,   y, &x2, &y2);

    token1 = new PSViewRealToken(mgr->getPSView(), x2 - x1);
    token2 = new PSViewRealToken(mgr->getPSView(), y2 - y1);

    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token2);
  }
}

void
PSViewOperatorMgr::
ifOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isBoolean() || ! token2->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token1->getBooleanValue())
    token2->execute();
}

void
PSViewOperatorMgr::
ifelseOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL)
    return;

  if (! token1->isBoolean() || ! token2->isProcedure() || ! token3->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  if (token1->getBooleanValue())
    token2->execute();
  else
    token3->execute();
}

void
PSViewOperatorMgr::
imageOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();

  if (token5 == NULL)
    return;

  PSViewToken *token1, *token2, *token3, *token4;

  int decode_size = 0;

  vector<double> decode_array;

  if (token5->isDictionary()) {
    PSViewDictionaryToken *dictionary_token = (PSViewDictionaryToken *) token5;

    PSViewToken *decode = dictionary_token->getValue("Decode");

    token1 = dictionary_token->getValue("Width");
    token2 = dictionary_token->getValue("Height");
    token3 = dictionary_token->getValue("BitsPerComponent");
    token4 = dictionary_token->getValue("ImageMatrix");
    token5 = dictionary_token->getValue("DataSource");

    if (token1 == NULL || token2 == NULL || token3 == NULL ||
        token4 == NULL || token5 == NULL || decode == NULL) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);
      return;
    }

    if (! token1->isInteger() || ! token2->isInteger() || ! token3->isInteger() ||
        ! token4->isMatrix () || ! decode->isArray()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }

    PSViewArrayToken *array_token = (PSViewArrayToken *) decode;

    decode_size = array_token->getNumValues();

    decode_array.resize(decode_size);

    for (int i = 1; i <= decode_size; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isNumber()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
        return;
      }

      decode_array[i - 1] = sub_token->getRealValue();
    }
  }
  else {
    token4 = mgr->getPSView()->getOperandStack()->pop();
    token3 = mgr->getPSView()->getOperandStack()->pop();
    token2 = mgr->getPSView()->getOperandStack()->pop();
    token1 = mgr->getPSView()->getOperandStack()->pop();

    if (token1 == NULL || token2 == NULL || token3 == NULL || token4 == NULL) {
      return;
    }

    if (! token1->isInteger() || ! token2->isInteger() || ! token3->isInteger() ||
        ! token4->isMatrix ()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }
  }

  if (! token5->isProcedure() && ! token5->isString() && ! token5->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSVinteger width           = token1->getIntegerValue();
  PSVinteger height          = token2->getIntegerValue();
  PSVinteger bits_per_sample = token3->getIntegerValue();
  CMatrix2D  matrix          = *token4->getMatrix();

  if (width <= 0 || height <= 0 ||
      (bits_per_sample != 1 && bits_per_sample != 2 && bits_per_sample != 4 &&
       bits_per_sample != 8 && bits_per_sample != 12)) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  if (token5->isFile()) {
    PSViewFileToken *file_token = (PSViewFileToken *) token5;

    string str;

    int c = file_token->readChar();

    while (c != EOF) {
      str += c;

      c = file_token->readChar();
    }

    token5 = new PSViewStringToken(token5->getPSView(), str);
  }

  if (token5->isString()) {
    PSViewStringToken *string_token = (PSViewStringToken *) token5;

    PSVinteger length = string_token->getLength();

    if ((width % 8) != 0)
      width -= (width % 8);

    PSVinteger num_bits = width*height*bits_per_sample;

    PSVinteger num_bytes = num_bits/8;

    char *image_data = new char [num_bytes];

    memset(image_data, 0, num_bytes*sizeof(char));

    int bit_num  = 7;
    int byte_num = 0;

    int bit_mask = 1 << bit_num;

    for (int i = 0; i < length; i++) {
      int c = string_token->getChar(i + 1);

      for (int j = 0; j < 8; j++) {
        int char_mask = 1 << (7 - j);

        if (c & char_mask)
          image_data[byte_num] |= bit_mask;

        bit_num--;

        if (bit_num < 0) {
          bit_num = 7;

          byte_num++;

          if (byte_num == num_bytes)
            break;
        }

        bit_mask = 1 << bit_num;
      }

      if (byte_num == num_bytes)
        break;
    }

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->image(image_data, (int) width, (int) height, (int) bits_per_sample, &matrix,
                        &decode_array[0], decode_size);

    delete [] image_data;
  }
  else {
    PSVinteger width1 = width;

    if ((width % 8) != 0)
      width1 += 8 - (width % 8);

    PSVinteger num_bits = width1*height*bits_per_sample;

    PSVinteger num_bytes = num_bits/8;

    char *image_data = new char [num_bytes];

    for (int i = 0; i < num_bytes; i++)
      image_data[i] = '\0';

    int bit_num  = 7;
    int byte_num = 0;

    int bit_mask = 1 << bit_num;

    while (byte_num < num_bytes) {
      token5->execute();

      PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

      if (token == NULL)
        break;

      if (! token->isString()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
        break;
      }

      PSViewStringToken *string_token = (PSViewStringToken *) token;

      PSVinteger length = string_token->getLength();

      if (length == 0) {
        byte_num = num_bytes;

        break;
      }

      for (int i = 0; i < length; i++) {
        int c = string_token->getChar(i + 1);

        for (int j = 0; j < 8; j++) {
          int char_mask = 1 << (7 - j);

          if (c & char_mask)
            image_data[byte_num] |= bit_mask;

          bit_num--;

          if (bit_num < 0) {
            bit_num = 7;

            byte_num++;

            if (byte_num == num_bytes)
              break;
          }

          bit_mask = 1 << bit_num;
        }

        if (byte_num == num_bytes)
          break;
      }
    }

    if (byte_num == num_bytes) {
      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->image(image_data, (int) width, (int) height, (int) bits_per_sample, &matrix,
                          &decode_array[0], decode_size);
    }

    delete [] image_data;
  }
}

void
PSViewOperatorMgr::
imageMaskOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();

  if (token5 == NULL)
    return;

  PSViewToken *token1, *token2, *token3, *token4;

  if (token5->isDictionary()) {
    PSViewDictionaryToken *dictionary_token = (PSViewDictionaryToken *) token5;

    PSViewToken *decode = dictionary_token->getValue("Decode");

    token1 = dictionary_token->getValue("Width");
    token2 = dictionary_token->getValue("Height");
    token4 = dictionary_token->getValue("ImageMatrix");
    token5 = dictionary_token->getValue("DataSource");

    if (token1 == NULL || token2 == NULL || token4 == NULL || token5 == NULL || decode == NULL) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);
      return;
    }

    if (! token1->isInteger() || ! token2->isInteger() || ! token4->isMatrix() ||
        ! decode->isArray()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }

    PSViewArrayToken *array_token = (PSViewArrayToken *) decode;

    int decode_size = array_token->getNumValues();

    if (decode_size != 2) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }

    vector<double> decode_array;

    decode_array.resize(decode_size);

    for (int i = 1; i <= decode_size; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isNumber()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
        return;
      }

      decode_array[i - 1] = sub_token->getRealValue();
    }

    if      (decode_array[0] == 1 && decode_array[1] == 0)
      token3 = new PSViewBooleanToken(mgr->getPSView(), false);
    else if (decode_array[0] == 0 && decode_array[1] == 1)
      token3 = new PSViewBooleanToken(mgr->getPSView(), true);
    else {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }
  }
  else {
    token4 = mgr->getPSView()->getOperandStack()->pop();
    token3 = mgr->getPSView()->getOperandStack()->pop();
    token2 = mgr->getPSView()->getOperandStack()->pop();
    token1 = mgr->getPSView()->getOperandStack()->pop();

    if (token1 == NULL || token2 == NULL || token3 == NULL || token4 == NULL)
      return;

    if (! token1->isInteger() || ! token2->isInteger() || ! token3->isBoolean() ||
        ! token4->isMatrix ()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }
  }

  if (! token5->isProcedure() && ! token5->isString() && ! token5->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSVinteger width    = token1->getIntegerValue();
  PSVinteger height   = token2->getIntegerValue();
  PSVboolean polarity = token3->getBooleanValue();
  CMatrix2D  matrix   = *token4->getMatrix();

  if (width <= 0 || height <= 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  if (token5->isFile()) {
    PSViewFileToken *file_token = (PSViewFileToken *) token5;

    string str;

    int c = file_token->readChar();

    while (c != EOF) {
      str += c;

      c = file_token->readChar();
    }

    token5 = new PSViewStringToken(token5->getPSView(), str);
  }

  if (token5->isString()) {
    PSViewStringToken *string_token = (PSViewStringToken *) token5;

    PSVinteger length = string_token->getLength();

    if ((width % 8) != 0)
      width -= (width % 8);

    PSVinteger num_bits = width*height;

    PSVinteger num_bytes = num_bits/8;

    char *image_data = new char [num_bytes];

    memset(image_data, 0, num_bytes*sizeof(char));

    int bit_num  = 7;
    int byte_num = 0;

    int bit_mask = 1 << bit_num;

    for (int i = 0; i < length; i++) {
      int c = string_token->getChar(i + 1);

      for (int j = 0; j < 8; j++) {
        int char_mask = 1 << (7 - j);

        if (c & char_mask)
          image_data[byte_num] |= bit_mask;

        bit_num--;

        if (bit_num < 0) {
          bit_num = 7;

          byte_num++;

          if (byte_num == num_bytes)
            break;
        }

        bit_mask = 1 << bit_num;
      }

      if (byte_num == num_bytes)
        break;
    }

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->imageMask(image_data, (int) width, (int) height, (int) polarity, &matrix);

    delete [] image_data;
  }
  else {
    PSVinteger width1 = width;

    if ((width % 8) != 0)
      width1 += 8 - (width % 8);

    PSVinteger num_bits = width1*height;

    PSVinteger num_bytes = num_bits/8;

    char *image_data = new char [num_bytes];

    for (int i = 0; i < num_bytes; i++)
      image_data[i] = '\0';

    PSVinteger bit_num  = 7;
    PSVinteger byte_num = 0;

    int bit_mask = 1 << bit_num;

    while (byte_num < num_bytes) {
      token5->execute();

      PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

      if (token == NULL)
        break;

      if (! token->isString()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

        break;
      }

      PSViewStringToken *string_token = (PSViewStringToken *) token;

      PSVinteger length = string_token->getLength();

      if (length == 0) {
        byte_num = num_bytes;

        break;
      }

      for (int i = 0; i < length; i++) {
        int c = string_token->getChar(i + 1);

        for (int j = 0; j < 8; j++) {
          int char_mask = 1 << (7 - j);

          if (c & char_mask)
            image_data[byte_num] |= bit_mask;

          bit_num--;

          if (bit_num < 0) {
            bit_num = 7;

            byte_num++;

            if (byte_num == num_bytes)
              break;
          }

          bit_mask = 1 << bit_num;
        }

        if (byte_num == num_bytes)
          break;
      }
    }

    if (byte_num == num_bytes) {
      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->imageMask(image_data, (int) width, (int) height, (int) polarity, &matrix);
    }

    delete [] image_data;
  }
}

void
PSViewOperatorMgr::
indexOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger count = token->getIntegerValue();

  if (count < 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSVinteger num_stack = mgr->getPSView()->getOperandStack()->size();

  if (count >= num_stack) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_STACK_UNDERFLOW);

    return;
  }

  token = mgr->getPSView()->getOperandStack()->peek((int) (num_stack - count));

  PSViewToken *token1 = token->dup();

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
initClipOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->initClip();
}

void
PSViewOperatorMgr::
initGraphicsOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->initGraphics();
}

void
PSViewOperatorMgr::
initMatrixOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->initMatrix();
}

void
PSViewOperatorMgr::
internalDictOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  int integer = token->getIntegerValue();

  if (integer != 1183615869) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_INVALID_ACCESS);

    return;
  }

  PSViewDictionaryToken *dict = mgr->getPSView()->getDictionaryMgr()->getInternalDictionary();

  mgr->getPSView()->getOperandStack()->push(dict);
}

void
PSViewOperatorMgr::
invertMatrixOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();

  if (token2 == NULL)
    return;

  if (! token2->isMatrix()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL)
    return;

  CAutoPtr<CMatrix2D> matrix1;

  matrix1 = token1->getMatrix();

  if (matrix1 == NULL)
    return;

  CMatrix2D imatrix;

  if (! matrix1->invert(imatrix)) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

    return;
  }

  token2->setMatrix(&imatrix);

  mgr->getPSView()->getOperandStack()->push(token2);
}

void
PSViewOperatorMgr::
itransformOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token2->isNumber()) {
    PSVreal x = token1->getRealValue();
    PSVreal y = token2->getRealValue();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    CMatrix2D *matrix = gstate_token->getInverseCTMMatrix();

    PSVreal x1, y1;

    matrix->multiplyPoint(x, y, &x1, &y1);

    token1 = new PSViewRealToken(mgr->getPSView(), x1);
    token2 = new PSViewRealToken(mgr->getPSView(), y1);

    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token2);
  }
  else {
    CAutoPtr<CMatrix2D> matrix;

    matrix = token2->getMatrix();

    if (matrix == NULL)
      return;

    PSViewToken *token0 = mgr->getPSView()->getOperandStack()->pop();

    if (! token0->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x = token0->getRealValue();
    PSVreal y = token1->getRealValue();

    CMatrix2D imatrix;

    if (! matrix->invert(imatrix)) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

      return;
    }

    PSVreal x1, y1;

    imatrix.multiplyPoint(x, y, &x1, &y1);

    token1 = new PSViewRealToken(mgr->getPSView(), x1);
    token2 = new PSViewRealToken(mgr->getPSView(), y1);

    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token2);
  }
}

void
PSViewOperatorMgr::
knownOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isDictionary()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewDictionaryToken *dictionary_token1 = (PSViewDictionaryToken *) token1;

  PSViewToken *token = dictionary_token1->getValue(token2);

  if (token != NULL)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
kshowOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isProcedure() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  string str = string_token2->toString();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->kshow(token1, str);
}

void
PSViewOperatorMgr::
languageLevelOp(PSViewOperatorMgr *mgr)
{
  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), 2);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
leOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  PSVinteger flag = token1->compare(token2);

  if (flag <= 0)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
lengthOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  PSVinteger length;

  if      (token->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token;

    length = array_token->getNumValues();
  }
  else if (token->isPackedArray()) {
    PSViewPackedArrayToken *array_token = (PSViewPackedArrayToken *) token;

    length = array_token->getNumValues();
  }
  else if (token->isDictionary()) {
    PSViewDictionaryToken *dictionary_token = (PSViewDictionaryToken *) token;

    length = dictionary_token->getNumValues();
  }
  else if (token->isString()) {
    PSViewStringToken *string_token = (PSViewStringToken *) token;

    length = string_token->getLength();
  }
  else if (token->isName()) {
    PSViewNameToken *name_token = (PSViewNameToken *) token;

    length = name_token->getLength();
  }
  else {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewIntegerToken *token1 = new PSViewIntegerToken(mgr->getPSView(), length);

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
lineToOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal x = token1->getRealValue();
  PSVreal y = token2->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->lineTo(x, y);
}

void
PSViewOperatorMgr::
lnOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal real = token->getRealValue();

  PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), log(real));

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
loadOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  PSViewToken *token1 = mgr->getPSView()->getDictionaryMgr()->lookup(token);

  if (token1 == NULL) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

    return;
  }

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
logOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal real = token->getRealValue();

  PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), log10(real));

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
loopOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  for (;;) {
    token->execute();

    if (mgr->getPSView()->getExitFlag()) {
      mgr->getPSView()->getExecutionStack()->popToExit();
      break;
    }
  }
}

void
PSViewOperatorMgr::
ltOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  PSVinteger flag = token1->compare(token2);

  if (flag < 0)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
makeFontOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isDictionary() || ! token2->isMatrix()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  CAutoPtr<CMatrix2D> matrix;

  matrix = token2->getMatrix();

  PSViewDictionaryToken *dict = (PSViewDictionaryToken *) token1;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewDictionaryToken *font = gstate_token->makeFont(dict, matrix);

  mgr->getPSView()->getOperandStack()->push(font);
}

void
PSViewOperatorMgr::
makePatternOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isDictionary() || ! token2->isMatrix()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  CAutoPtr<CMatrix2D> matrix;

  matrix = token2->getMatrix();

  PSViewDictionaryToken *dict = (PSViewDictionaryToken *) token1;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewDictionaryToken *pattern = gstate_token->makePattern(dict, matrix);

  mgr->getPSView()->getOperandStack()->push(pattern);
}

void
PSViewOperatorMgr::
markOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getOperandStack()->push(mgr->getPSView()->getMarkToken());
}

void
PSViewOperatorMgr::
matrixOp(PSViewOperatorMgr *mgr)
{
  CMatrix2D matrix;

  matrix.setIdentity();

  PSViewArrayToken *token = new PSViewArrayToken(mgr->getPSView(), &matrix);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
maxLengthOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isDictionary()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewDictionaryToken *dictionary_token = (PSViewDictionaryToken *) token;

  PSVinteger length = dictionary_token->getMaxValues();

  PSViewIntegerToken *token1 = new PSViewIntegerToken(mgr->getPSView(), length);

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
modOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isInteger() || ! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger integer2 = token2->getIntegerValue();

  if (integer2 == 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED_RESULT);

    return;
  }

  PSVinteger integer1 = token1->getIntegerValue();

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), integer1 % integer2);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
moveToOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal x = token1->getRealValue();
  PSVreal y = token2->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->moveTo(x, y);
}

void
PSViewOperatorMgr::
mulOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewToken *token;

  if (token1->isInteger() && token2->isInteger()) {
    PSVinteger result = token1->getIntegerValue() *
                        token2->getIntegerValue();

    token = new PSViewIntegerToken(mgr->getPSView(), result);
  }
  else {
    PSVreal result = token1->getRealValue()*token2->getRealValue();

    token = new PSViewRealToken(mgr->getPSView(), result);
  }

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
neOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (token1->compare(token2) == 0)
    falseOp(mgr);
  else
    trueOp(mgr);
}

void
PSViewOperatorMgr::
negOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewToken *token1;

  if (token->isInteger()) {
    PSVinteger integer = token->getIntegerValue();

    if (integer != LONG_MIN)
      token1 = new PSViewIntegerToken(mgr->getPSView(), -integer);
    else
      token1 = new PSViewRealToken(mgr->getPSView(), -((double) integer));
  }
  else {
    PSVreal real = token->getRealValue();

    token1 = new PSViewRealToken(mgr->getPSView(), -real);
  }

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
newPathOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->newPath();
}

void
PSViewOperatorMgr::
noAccessOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isArray() && ! token->isPackedArray() &&
      ! token->isDictionary() && ! token->isFile() && ! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  token->setNoAccess();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
notOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isBoolean() && ! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token->isBoolean()) {
    if (! token->getBooleanValue())
      trueOp(mgr);
    else
      falseOp(mgr);
  }
  else {
    PSVinteger integer = token->getIntegerValue();

    PSViewIntegerToken *token1 = new PSViewIntegerToken(mgr->getPSView(), ~integer);

    mgr->getPSView()->getOperandStack()->push(token1);
  }
}

void
PSViewOperatorMgr::
nullOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = new PSViewNullToken(mgr->getPSView());

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
orOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if ((! token1->isBoolean() && ! token1->isInteger()) ||
      (! token2->isBoolean() && ! token2->isInteger()) ||
      ! token1->isType(token2->getType())) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token1->isBoolean()) {
    PSVboolean boolean1 = token1->getBooleanValue();
    PSVboolean boolean2 = token2->getBooleanValue();

    if (boolean1 | boolean2)
      trueOp(mgr);
    else
      falseOp(mgr);
  }
  else {
    PSVinteger integer1 = token1->getIntegerValue();
    PSVinteger integer2 = token2->getIntegerValue();

    PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), integer1 | integer2);

    mgr->getPSView()->getOperandStack()->push(token);
  }
}

void
PSViewOperatorMgr::
packedArrayOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL)
    return;

  if (! token1->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger dimension = token1->getIntegerValue();

  if (dimension < 0 || dimension > MAX_ARRAY_DIMENSION) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewPackedArrayToken *packed_array = new PSViewPackedArrayToken(mgr->getPSView(), dimension);

  for (int i = 1; i <= dimension; i++) {
    PSViewToken *sub_token = mgr->getPSView()->getOperandStack()->pop();

    if (sub_token == NULL)
      return;

    packed_array->setValue(dimension - i + 1, sub_token);
  }

  mgr->getPSView()->getOperandStack()->push(packed_array);
}

void
PSViewOperatorMgr::
pathBBoxOp(PSViewOperatorMgr *mgr)
{
  double llx, lly, urx, ury;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->pathBBox(&llx, &lly, &urx, &ury);

  PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), llx);
  PSViewRealToken *token2 = new PSViewRealToken(mgr->getPSView(), lly);
  PSViewRealToken *token3 = new PSViewRealToken(mgr->getPSView(), urx);
  PSViewRealToken *token4 = new PSViewRealToken(mgr->getPSView(), ury);

  mgr->getPSView()->getOperandStack()->push(token1);
  mgr->getPSView()->getOperandStack()->push(token2);
  mgr->getPSView()->getOperandStack()->push(token3);
  mgr->getPSView()->getOperandStack()->push(token4);
}

void
PSViewOperatorMgr::
pathForAllOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL ||
      token3 == NULL || token4 == NULL)
    return;

  if (! token1->isProcedure() || ! token2->isProcedure() ||
      ! token3->isProcedure() || ! token4->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->pathForAll(token1, token2, token3, token4);
}

void
PSViewOperatorMgr::
popOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->getOperandStack()->pop();
}

void
PSViewOperatorMgr::
printOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token = (PSViewStringToken *) token;

  string str = string_token->toString();

  CStrUtil::printf("%s", str.c_str());
}

void
PSViewOperatorMgr::
productOp(PSViewOperatorMgr *mgr)
{
  static string product = "PSView";

  PSViewStringToken *token = new PSViewStringToken(mgr->getPSView(), product);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
promptOp(PSViewOperatorMgr *)
{
  CStrUtil::printf("%s", "PS> ");
}

void
PSViewOperatorMgr::
pstackOp(PSViewOperatorMgr *mgr)
{
  int num_stack = mgr->getPSView()->getOperandStack()->size();

  for (int i = num_stack; i >= 1; i--) {
    PSViewToken *token = mgr->getPSView()->getOperandStack()->peek(i);

    token->print();

    CStrUtil::printf("\n");
  }
}

void
PSViewOperatorMgr::
putOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL)
    return;

  if (token1->isArray() || token1->isString()) {
    if (! token2->isInteger()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVinteger ind = token2->getIntegerValue();

    PSVinteger num_tokens;

    if (token1->isArray()) {
      PSViewArrayToken *array_token = (PSViewArrayToken *) token1;

      num_tokens = array_token->getNumValues();

      if (ind < 0 || ind >= num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      array_token->setValue(ind + 1, token3);
    }
    else if (token1->isPackedArray()) {
      PSViewPackedArrayToken *array_token = (PSViewPackedArrayToken *) token1;

      num_tokens = array_token->getNumValues();

      if (ind < 0 || ind >= num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      array_token->setValue(ind + 1, token3);
    }
    else {
      PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

      num_tokens = string_token1->getLength();

      if (ind < 0 || ind >= num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      if (! token3->isInteger()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

        return;
      }

      PSVinteger c = token3->getIntegerValue();

      string_token1->setChar(ind + 1, c);
    }
  }
  else if (token1->isDictionary()) {
    PSViewDictionaryToken *dict = (PSViewDictionaryToken *) token1;

    dict->addValue(token2, token3);
  }
  else {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }
}

void
PSViewOperatorMgr::
putIntervalOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL)
    return;

  if (! token1->isArray() && ! token1->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token1->isArray()) {
    if (! token3->isArray() && ! token3->isPackedArray()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }
  }
  else {
    if (! token3->isString()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }
  }

  PSVinteger ind = token2->getIntegerValue();

  PSVinteger num_tokens1;

  if (token1->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token1;

    num_tokens1 = array_token->getNumValues();
  }
  else {
    PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

    num_tokens1 = string_token1->getLength();
  }

  PSVinteger num_tokens3;

  if (token3->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token3;

    num_tokens3 = array_token->getNumValues();
  }
  else if (token3->isPackedArray()) {
    PSViewPackedArrayToken *array_token = (PSViewPackedArrayToken *) token3;

    num_tokens3 = array_token->getNumValues();
  }
  else {
    PSViewStringToken *string_token3 = (PSViewStringToken *) token3;

    num_tokens3 = string_token3->getLength();
  }

  if (ind < 0 || ind + num_tokens3 > num_tokens1) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  if (token1->isArray()) {
    PSViewToken **tokens = new PSViewToken * [num_tokens3 + 1];

    if (token3->isArray()) {
      PSViewArrayToken *array_token3 = (PSViewArrayToken *) token3;

      for (int i = 1; i <= num_tokens3; i++)
        tokens[i - 1] = array_token3->getValue(i);
    }
    else {
      PSViewPackedArrayToken *packed_array_token3 = (PSViewPackedArrayToken *) token3;

      for (int i = 1; i <= num_tokens3; i++)
        tokens[i - 1] = packed_array_token3->getValue(i);
    }

    PSViewArrayToken *array_token1 = (PSViewArrayToken *) token1;

    array_token1->setSubValues(ind + 1, tokens, num_tokens3);
  }
  else {
    PSViewStringToken *string_token3 = (PSViewStringToken *) token3;

    string str;

    for (int i = 1; i <= num_tokens3; i++)
      str += string_token3->getChar(i);

    PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

    string_token1->setChars(str, ind + 1);
  }
}

void
PSViewOperatorMgr::
quitOp(PSViewOperatorMgr *mgr)
{
  mgr->getPSView()->term();

  exit(0);
}

void
PSViewOperatorMgr::
randOp(PSViewOperatorMgr *mgr)
{
  long r = COSRand::rand();

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), r);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
rcheckOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isArray() && ! token->isPackedArray() && ! token->isDictionary() &&
      ! token->isFile() && ! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  if (token->getReadable())
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
rcurveToOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token6 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL ||
      token4 == NULL || token5 == NULL || token6 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token3->isNumber() || ! token4->isNumber() ||
      ! token5->isNumber() || ! token6->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double x1 = token1->getRealValue();
  double y1 = token2->getRealValue();
  double x2 = token3->getRealValue();
  double y2 = token4->getRealValue();
  double x3 = token5->getRealValue();
  double y3 = token6->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->rcurveTo(x1, y1, x2, y2, x3, y3);
}

void
PSViewOperatorMgr::
readOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewFileToken *file_token = (PSViewFileToken *) token;

  int c = file_token->readChar();

  if (c != EOF) {
    PSViewIntegerToken *token1 = new PSViewIntegerToken(mgr->getPSView(), c);

    mgr->getPSView()->getOperandStack()->push(token1);

    trueOp(mgr);
  }
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
readHexStringOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isFile() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken   *file_token1   = (PSViewFileToken   *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  PSVinteger length = string_token2->getLength();

  int i = 1;

  string hex_string;

  while (i <= length) {
    int c = file_token1->readChar();

    if (c == EOF)
      break;

    if (! isxdigit(c))
      continue;

    hex_string += c;

    if (hex_string.size() == 2) {
      int hex_value = CStrUtil::toBaseInteger(hex_string, 16);

      string_token2->setChar(i++, hex_value);

      hex_string = "";
    }
  }

  if (hex_string.size() == 1) {
    hex_string += '0';

    int hex_value = CStrUtil::toBaseInteger(hex_string, 16);

    string_token2->setChar(i++, (PSVchar) hex_value);
  }

  PSViewToken *token;

  if (i - 1 <= length)
    token = string_token2->subString(1, i - 1);
  else
    token = string_token2->subString(1, length);

  mgr->getPSView()->getOperandStack()->push(token);

  if (i - 1 == length)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
readLineOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isFile() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken   *file_token1   = (PSViewFileToken   *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  PSVinteger length = string_token2->getLength();

  int c = file_token1->readChar();

  int i = 1;

  while (c != '\n' && c != EOF) {
    if (i > length) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    string_token2->setChar(i++, c);

    c = file_token1->readChar();
  }

  PSViewToken *token = string_token2->subString(1, i);

  mgr->getPSView()->getOperandStack()->push(token);

  if (c == '\n')
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
readOnlyOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isArray() && ! token->isPackedArray() &&
      ! token->isDictionary() && ! token->isFile() &&
      ! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  token->setReadOnly();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
readStringOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isFile() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken   *file_token1   = (PSViewFileToken   *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  PSVinteger length = string_token2->getLength();

  int i;

  for (i = 1; i <= length; i++) {
    int c = file_token1->readChar();

    if (c == EOF)
      break;

    string_token2->setChar(i, c);
  }

  PSViewToken *token;

  if (i <= length)
    token = string_token2->subString(1, i);
  else
    token = string_token2->subString(1, length);

  mgr->getPSView()->getOperandStack()->push(token);

  if (i == length)
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
realTimeOp(PSViewOperatorMgr *mgr)
{
  time_t real_time = time(NULL);

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), (int) real_time);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
rectClipOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();

  if (token4 == NULL)
    return;

  if (token4->isNumber()) {
    PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
    PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
    PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

    if (token1 == NULL || token2 == NULL || token3 == NULL)
      return;

    if (! token1->isNumber() || ! token2->isNumber() ||
        ! token3->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x      = token1->getRealValue();
    PSVreal y      = token2->getRealValue();
    PSVreal width  = token3->getRealValue();
    PSVreal height = token4->getRealValue();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->rectClip(&x, &y, &width, &height, 1);
  }
  else if (token4->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token4;

    PSVinteger num_tokens = array_token->getNumValues();

    if (num_tokens % 4 != 0) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    for (int i = 1; i <= num_tokens; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isNumber()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

        return;
      }
    }

    PSVinteger num_arrays = num_tokens/4;

    vector<double> x, y, width, height;

    x     .resize(num_arrays);
    y     .resize(num_arrays);
    width .resize(num_arrays);
    height.resize(num_arrays);

    PSViewToken *tokens[4];

    for (int i = 0; i < num_arrays; i++) {
      tokens[0] = array_token->getValue(4*i + 1);
      tokens[1] = array_token->getValue(4*i + 2);
      tokens[2] = array_token->getValue(4*i + 3);
      tokens[3] = array_token->getValue(4*i + 4);

      x     [i] = tokens[0]->getRealValue();
      y     [i] = tokens[1]->getRealValue();
      width [i] = tokens[2]->getRealValue();
      height[i] = tokens[3]->getRealValue();
    }

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->rectClip(&x[0], &y[0], &width[0], &height[0],
                           (int) num_arrays);
  }
  else if (token4->isString())
    CStrUtil::eprintf("PSView: RectClip not implemented for Strings\n");
  else
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
}

void
PSViewOperatorMgr::
rectFillOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();

  if (token4 == NULL)
    return;

  if (token4->isNumber()) {
    PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
    PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
    PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

    if (token1 == NULL || token2 == NULL || token3 == NULL)
      return;

    if (! token1->isNumber() || ! token2->isNumber() ||
        ! token3->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x      = token1->getRealValue();
    PSVreal y      = token2->getRealValue();
    PSVreal width  = token3->getRealValue();
    PSVreal height = token4->getRealValue();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->rectFill(x, y, width, height);
  }
  else if (token4->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token4;

    PSVinteger num_tokens = array_token->getNumValues();

    if (num_tokens % 4 != 0) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    for (int i = 1; i < num_tokens; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isNumber()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

        return;
      }
    }

    PSVinteger num_arrays = num_tokens/4;

    PSViewToken *tokens[4];

    for (int i = 0; i < num_arrays; i++) {
      tokens[0] = array_token->getValue(4*i + 1);
      tokens[1] = array_token->getValue(4*i + 2);
      tokens[2] = array_token->getValue(4*i + 3);
      tokens[3] = array_token->getValue(4*i + 4);

      double x      = tokens[0]->getRealValue();
      double y      = tokens[1]->getRealValue();
      double width  = tokens[2]->getRealValue();
      double height = tokens[3]->getRealValue();

      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->rectFill(x, y, width, height);
    }
  }
  else if (token4->isString())
    CStrUtil::eprintf("PSView: RectFill not implemented for Strings\n");
  else
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
}

void
PSViewOperatorMgr::
rectStrokeOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  CAutoPtr<CMatrix2D> matrix;

  PSViewToken *token4;

  if (token->isMatrix()) {
    matrix = token->getMatrix();

    token4 = mgr->getPSView()->getOperandStack()->pop();

    if (token4 == NULL)
      return;
  }
  else
    token4 = token;

  if (token4->isNumber()) {
    PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
    PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
    PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

    if (token1 == NULL || token2 == NULL || token3 == NULL)
      return;

    if (! token1->isNumber() || ! token2->isNumber() || ! token3->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x      = token1->getRealValue();
    PSVreal y      = token2->getRealValue();
    PSVreal width  = token3->getRealValue();
    PSVreal height = token4->getRealValue();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->rectStroke(x, y, width, height, matrix);
  }
  else if (token4->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token4;

    PSVinteger num_tokens = array_token->getNumValues();

    if (num_tokens % 4 != 0) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    for (int i = 1; i <= num_tokens; i++) {
      token = array_token->getValue(i);

      if (! token->isNumber()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

        return;
      }
    }

    PSVinteger num_arrays = num_tokens/4;

    PSViewToken *tokens[4];

    for (int i = 0; i < num_arrays; i++) {
      tokens[0] = array_token->getValue(4*i + 1);
      tokens[1] = array_token->getValue(4*i + 2);
      tokens[2] = array_token->getValue(4*i + 3);
      tokens[3] = array_token->getValue(4*i + 4);

      double x      = tokens[0]->getRealValue();
      double y      = tokens[1]->getRealValue();
      double width  = tokens[2]->getRealValue();
      double height = tokens[3]->getRealValue();

      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->rectStroke(x, y, width, height, matrix);
    }
  }
  else if (token4->isString())
    CStrUtil::eprintf("PSView: RectStroke not implemented for Strings\n");
  else
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
}

void
PSViewOperatorMgr::
renameFileOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isString() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  string filename1 = string_token1->toString();
  string filename2 = string_token2->toString();

  rename(filename1.c_str(), filename2.c_str());
}

void
PSViewOperatorMgr::
repeatOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isProcedure() || ! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger num_iterations = token2->getIntegerValue();

  if (num_iterations < 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  mgr->getPSView()->getExecutionStack()->push(mgr->getPSView()->getMarkToken());

  for (int i = 0; i < num_iterations; i++)
    mgr->getPSView()->getExecutionStack()->push(token1);

  token1 = mgr->getPSView()->getExecutionStack()->pop();

  while (token1 != NULL && token1 != mgr->getPSView()->getMarkToken()) {
    token1->execute();

    if (mgr->getPSView()->getExitFlag()) {
      mgr->getPSView()->getExecutionStack()->popToExit();
      break;
    }

    token1 = mgr->getPSView()->getExecutionStack()->pop();
  }
}

void
PSViewOperatorMgr::
resetFileOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isFile()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken *file_token = (PSViewFileToken *) token;

  file_token->reset();
}

void
PSViewOperatorMgr::
resourceForAllOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (! token1->isString() || ! token2->isProcedure() ||
      ! token3->isString() || ! token4->isName     ()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;
  PSViewStringToken *string_token3 = (PSViewStringToken *) token3;

  string templ   = string_token1->toString();
  string scratch = string_token3->toString();

  size_t scratch_len = scratch.size();

  CGlob pattern(templ);

  PSViewNameToken *name_token = (PSViewNameToken *) token2;

  vector<PSViewResource *> resources =
    mgr->getPSView()->getResourceMgr()->getResources(name_token->getValue());

  int num = resources.size();

  for (int i = 0; i < num; i++) {
    PSViewToken *resource = resources[i]->getKey();

    string str;

    if (resource->isName()) {
      PSViewNameToken *name_token = (PSViewNameToken *) resource;

      string str = name_token->getString();
    }

    if (! pattern.compare(str))
      continue;

    if (str.size() > scratch_len) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      break;
    }

    if (resource->isName()) {
      scratch = str;

      string_token3->setChars(scratch, 1);

      mgr->getPSView()->getOperandStack()->push(token3);
    }
    else
      mgr->getPSView()->getOperandStack()->push(resource);

    token2->execute();
  }
}

void
PSViewOperatorMgr::
restoreOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (! token->isSave()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  mgr->getPSView()->getMemoryMgr()->restore(token);
}

void
PSViewOperatorMgr::
reversePathOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->reversePath();
}

void
PSViewOperatorMgr::
revisionOp(PSViewOperatorMgr *mgr)
{
  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), 0);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
rlineToOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal x = token1->getRealValue();
  PSVreal y = token2->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->rlineTo(x, y);
}

void
PSViewOperatorMgr::
rmoveToOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal x = token1->getRealValue();
  PSVreal y = token2->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->rmoveTo(x, y);
}

void
PSViewOperatorMgr::
rollOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isInteger() || ! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger integer1 = token1->getIntegerValue();
  PSVinteger integer2 = token2->getIntegerValue();

  mgr->getPSView()->getOperandStack()->roll(integer2, integer1);
}

void
PSViewOperatorMgr::
rootFontOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewToken *token = gstate_token->getRootFont();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
rotateOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL)
    return;

  if (token1->isNumber()) {
    PSVreal angle = token1->getRealValue();

    CMatrix2D matrix;

    matrix.setRotation(-CMathGen::DegToRad(angle));

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->postMultiplyCTMMatrix(&matrix);
  }
  else {
    if (! token1->isMatrix()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }

    PSViewToken *token0 = mgr->getPSView()->getOperandStack()->pop();

    if (! token0->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
      return;
    }

    PSVreal angle = token0->getRealValue();

    CMatrix2D matrix;

    matrix.setRotation(-CMathGen::DegToRad(angle));

    token1->setMatrix(&matrix);

    mgr->getPSView()->getOperandStack()->push(token1);
  }
}

void
PSViewOperatorMgr::
roundOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token->isReal()) {
    PSVreal real = token->getRealValue();

    real = CMathRound::Round(real);

    PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), real);

    mgr->getPSView()->getOperandStack()->push(token1);
  }
  else {
    PSViewToken *token1 = token->dup();

    mgr->getPSView()->getOperandStack()->push(token1);
  }
}

void
PSViewOperatorMgr::
runOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token = (PSViewStringToken *) token;

  string filename = string_token->toString();

  mgr->getPSView()->openCurrentFile(filename);

  mgr->getPSView()->executeCurrentFile();

  mgr->getPSView()->closeCurrentFile();
}

void
PSViewOperatorMgr::
rrandOp(PSViewOperatorMgr *mgr)
{
  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), mgr->getRandomSeed());

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
saveOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getMemoryMgr()->save();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
scaleOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token2->isNumber()) {
    PSVreal x = token1->getRealValue();
    PSVreal y = token2->getRealValue();

    CMatrix2D matrix;

    matrix.setScale(x, y);

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->postMultiplyCTMMatrix(&matrix);
  }
  else {
    if (! token2->isMatrix()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSViewToken *token0 = mgr->getPSView()->getOperandStack()->pop();

    if (! token0->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x = token0->getRealValue();
    PSVreal y = token1->getRealValue();

    CMatrix2D matrix;

    matrix.setScale(x, y);

    token2->setMatrix(&matrix);

    mgr->getPSView()->getOperandStack()->push(token2);
  }
}

void
PSViewOperatorMgr::
scaleFontOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isDictionary() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewDictionaryToken *dict = (PSViewDictionaryToken *) token1;

  PSVreal scale = token2->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  PSViewDictionaryToken *font = gstate_token->scaleFont(dict, scale);

  mgr->getPSView()->getOperandStack()->push(font);
}

void
PSViewOperatorMgr::
searchOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isString() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  PSVinteger pos = string_token1->isSubString(string_token2);

  if (pos > -1) {
    PSVinteger len2 = string_token2->getLength();

    PSViewToken *token3 = string_token1->split(pos + len2 - 1);
    PSViewToken *token4 = string_token1->split(pos - 1);

    mgr->getPSView()->getOperandStack()->push(token3);
    mgr->getPSView()->getOperandStack()->push(token4);
    mgr->getPSView()->getOperandStack()->push(token1);

    trueOp(mgr);
  }
  else {
    mgr->getPSView()->getOperandStack()->push(token1);

    falseOp(mgr);
  }
}

void
PSViewOperatorMgr::
selectFontOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isName()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token2->isNumber()) {
    PSVreal scale = token2->getRealValue();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->selectScaleFont(token1, scale);
  }
  else if (token2->isMatrix()) {
    CMatrix2D *matrix = token2->getMatrix();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->selectMakeFont(token1, matrix);
  }
  else {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }
}

void
PSViewOperatorMgr::
serialNumberOp(PSViewOperatorMgr *mgr)
{
  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), 0);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
setBlackGenerationOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setBlackGeneration(token);
}

void
PSViewOperatorMgr::
setCacheDeviceOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token6 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token5 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL ||
      token4 == NULL || token5 == NULL || token6 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token3->isNumber() || ! token4->isNumber() ||
      ! token5->isNumber() || ! token6->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double wx  = token1->getRealValue();
  double wy  = token2->getRealValue();
  double llx = token3->getRealValue();
  double lly = token4->getRealValue();
  double urx = token5->getRealValue();
  double ury = token6->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setCacheDevice(wx, wy, llx, lly, urx, ury);
}

void
PSViewOperatorMgr::
setCharWidthOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double wx = token1->getRealValue();
  double wy = token2->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setCharWidth(wx, wy);
}

void
PSViewOperatorMgr::
setCMYKColorOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL ||
      token3 == NULL || token4 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() ||
      ! token3->isNumber() || ! token4->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal cyan    = token1->getRealValue();
  PSVreal magenta = token2->getRealValue();
  PSVreal yellow  = token3->getRealValue();
  PSVreal black   = token4->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setColorSpace(mgr->getPSView()->getGStateMgr()->getCMYKColorSpace());

  gstate_token->setPatternColor(false);

  gstate_token->setCMYKColor(CCMYK(cyan, magenta, yellow, black));
}

void
PSViewOperatorMgr::
setColorOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  const PSViewName &color_space = gstate_token->getColorSpace();

  bool pattern = gstate_token->getPatternColor();

  PSViewToken *token;

  if (pattern) {
    token = mgr->getPSView()->getOperandStack()->pop();

    if (token == NULL)
      return;

    if (! token->isDictionary()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }
  }

  if      (color_space.compare(
             mgr->getPSView()->getGStateMgr()->getRGBColorSpace()) == 0)
    setRGBColorOp(mgr);
  else if (color_space.compare(
             mgr->getPSView()->getGStateMgr()->getCMYKColorSpace()) == 0)
    setCMYKColorOp(mgr);
  else if (color_space.compare(
             mgr->getPSView()->getGStateMgr()->getGrayColorSpace()) == 0)
    setGrayOp(mgr);

  if (pattern) {
    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->setPatternColor(true);

    mgr->getPSView()->getOperandStack()->push(token);

    setPatternOp(mgr);
  }
}

void
PSViewOperatorMgr::
setColorSpaceOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isArray() && ! token->isName()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token;

    int num_tokens = array_token->getNumValues();

    if (num_tokens < 1) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

      return;
    }

    PSViewToken *sub_token = array_token->getValue(1);

    if (! sub_token->isName()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSViewNameToken *name_token = (PSViewNameToken *) sub_token;

    const PSViewName &color_space = name_token->getValue();

    if (color_space.compare(
          mgr->getPSView()->getGStateMgr()->getPatternColorSpace()) == 0) {
      if (num_tokens < 2) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      sub_token = array_token->getValue(2);

      if (! sub_token->isName()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

        return;
      }

      PSViewNameToken *name_token = (PSViewNameToken *) sub_token;

      const PSViewName &color_space1 = name_token->getValue();

      if (color_space1.compare(
            mgr->getPSView()->getGStateMgr()->getCMYKColorSpace()) != 0 &&
          color_space1.compare(
            mgr->getPSView()->getGStateMgr()->getRGBColorSpace ()) != 0 &&
          color_space1.compare(
            mgr->getPSView()->getGStateMgr()->getGrayColorSpace()) != 0) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

        return;
      }

      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->setColorSpace(color_space1);
      gstate_token->setPatternColor(true);
    }
    else {
      if (num_tokens != 1) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      if (color_space.compare(
            mgr->getPSView()->getGStateMgr()->getCMYKColorSpace()) != 0 &&
          color_space.compare(
            mgr->getPSView()->getGStateMgr()->getRGBColorSpace ()) != 0 &&
          color_space.compare(
            mgr->getPSView()->getGStateMgr()->getGrayColorSpace()) != 0) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

        return;
      }

      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->setColorSpace(color_space);
      gstate_token->setPatternColor(false);
    }
  }
  else {
    PSViewNameToken *name_token = (PSViewNameToken *) token;

    const PSViewName &color_space = name_token->getValue();

    if (color_space.compare(
          mgr->getPSView()->getGStateMgr()->getCMYKColorSpace()) != 0 &&
        color_space.compare(
          mgr->getPSView()->getGStateMgr()->getRGBColorSpace ()) != 0  &&
        color_space.compare(
          mgr->getPSView()->getGStateMgr()->getGrayColorSpace()) != 0) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

      return;
    }

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->setColorSpace(color_space);
    gstate_token->setPatternColor(false);
  }
}

void
PSViewOperatorMgr::
setDashOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isArray() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) token1;

  PSVinteger num_tokens = array_token->getNumValues();

  int zero_count = 0;

  vector<double> dash_array;

  dash_array.resize(num_tokens);

  for (int i = 1; i <= num_tokens; i++) {
    PSViewToken *sub_token = array_token->getValue(i);

    if (! sub_token->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    dash_array[i - 1] = sub_token->getRealValue();

    if (dash_array[i - 1] < 0.0) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_LIMIT_CHECK);

      return;
    }

    if (dash_array[i - 1] == 0.0)
      zero_count++;
  }

  if (zero_count > 0 && zero_count == num_tokens) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_LIMIT_CHECK);

    return;
  }

  double dash_offset = token2->getRealValue();

  if (dash_offset < 0.0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_LIMIT_CHECK);

    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setDashPattern(&dash_array[0], (int) num_tokens, dash_offset);
}

void
PSViewOperatorMgr::
setFilePositionOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isFile() || ! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger pos = token2->getIntegerValue();

  PSViewFileToken *file_token1 = (PSViewFileToken *) token1;

  file_token1->setPosition(pos);
}

void
PSViewOperatorMgr::
setFlatOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal flat = token->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setFlat(flat);
}

void
PSViewOperatorMgr::
setFontOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isDictionary()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewDictionaryToken *dict_token = (PSViewDictionaryToken *) token;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setFont(dict_token);
}

void
PSViewOperatorMgr::
setGlobalOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isBoolean()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVboolean boolean = token->getBooleanValue();

  mgr->getPSView()->getMemoryMgr()->setGlobal(boolean);
}

void
PSViewOperatorMgr::
setGrayOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal gray = token->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setColorSpace(mgr->getPSView()->getGStateMgr()->getGrayColorSpace());

  gstate_token->setPatternColor(false);

  gstate_token->setGray(gray);
}

void
PSViewOperatorMgr::
setGStateOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isGState()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewGStateToken *gstate_token = (PSViewGStateToken *) token;

  mgr->getPSView()->getGStateTokenMgr()->setCurrent(gstate_token);
}

void
PSViewOperatorMgr::
setHSBColorOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() || ! token3->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal hue        = token1->getRealValue();
  PSVreal saturation = token2->getRealValue();
  PSVreal brightness = token3->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setColorSpace(mgr->getPSView()->getGStateMgr()->getRGBColorSpace());

  gstate_token->setPatternColor(false);

  gstate_token->setHSBColor(CHSB(hue, saturation, brightness));
}

void
PSViewOperatorMgr::
setLineCapOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSVinteger line_cap = token->getIntegerValue();

  if (line_cap < 0 || line_cap > 2) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setLineCap((CLineCapType) (line_cap + 1));
}

void
PSViewOperatorMgr::
setLineJoinOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSVinteger line_join = token->getIntegerValue();

  if (line_join < 0 || line_join > 2) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setLineJoin((CLineJoinType) (line_join + 1));
}

void
PSViewOperatorMgr::
setLineWidthOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal line_width = token->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setLineWidth(line_width);
}

void
PSViewOperatorMgr::
setMatrixOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  CAutoPtr<CMatrix2D> matrix;

  matrix = token->getMatrix();

  if (matrix == NULL)
    return;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setCTMMatrix(matrix);
}

void
PSViewOperatorMgr::
setMitreLimitOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal mitre_limit = token->getRealValue();

  if (mitre_limit < 1.0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setMitreLimit(mitre_limit);
}

void
PSViewOperatorMgr::
setPackingOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isBoolean()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVboolean boolean = token->getBooleanValue();

  mgr->getPSView()->getTokenMgr()->setPacking(boolean);
}

void
PSViewOperatorMgr::
setPatternOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isDictionary()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewDictionaryToken *dict_token = (PSViewDictionaryToken *) token;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setPattern(dict_token);
}

void
PSViewOperatorMgr::
setRGBColorOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL || token3 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber() || ! token3->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal red   = token1->getRealValue();
  PSVreal green = token2->getRealValue();
  PSVreal blue  = token3->getRealValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setColorSpace(mgr->getPSView()->getGStateMgr()->getRGBColorSpace());

  gstate_token->setPatternColor(false);

  gstate_token->setRGBColor(CRGBA(red, green, blue));
}

void
PSViewOperatorMgr::
setStrokeAdjustOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isBoolean()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVboolean boolean = token->getBooleanValue();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setStrokeAdjust(boolean);
}

void
PSViewOperatorMgr::
setUnderColorRemovalOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isProcedure()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->setUnderColorRemoval(token);
}

void
PSViewOperatorMgr::
showOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewStringToken *string_token = (PSViewStringToken *) token;

  string str = string_token->toString();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->show(str);
}

void
PSViewOperatorMgr::
showPageOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->showPage();
}

void
PSViewOperatorMgr::
sinOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal real = token->getRealValue();

  PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), sin(CMathGen::DegToRad(real)));

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
sqrtOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVreal real = token->getRealValue();

  if (real < 0.0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), sqrt(real));

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
srandOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  mgr->setRandomSeed(token->getIntegerValue());

  COSRand::srand((long) mgr->getRandomSeed());
}

void
PSViewOperatorMgr::
stackOp(PSViewOperatorMgr *mgr)
{
  int num_stack = mgr->getPSView()->getOperandStack()->size();

  for (int i = num_stack; i >= 1; i--) {
    PSViewToken *token = mgr->getPSView()->getOperandStack()->peek(i);

    const string &str = token->toString();

    CStrUtil::printf("%s\n", str.c_str());
  }
}

void
PSViewOperatorMgr::
statusOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isFile() && ! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token->isFile()) {
    PSViewFileToken *file_token = (PSViewFileToken *) token;

    if (file_token->isValid())
      trueOp(mgr);
    else
      falseOp(mgr);
  }
  else {
    PSViewStringToken *string_token = (PSViewStringToken *) token;

    string filename = string_token->toString();

    struct stat file_stat;

    int error = stat(filename.c_str(), &file_stat);

    if (error == 0) {
      PSViewIntegerToken *token1 = new PSViewIntegerToken(mgr->getPSView(), 1);

      mgr->getPSView()->getOperandStack()->push(token1);

      token1 = new PSViewIntegerToken(mgr->getPSView(), file_stat.st_size);

      mgr->getPSView()->getOperandStack()->push(token1);

      token1 = new PSViewIntegerToken(mgr->getPSView(), file_stat.st_atime);

      mgr->getPSView()->getOperandStack()->push(token1);

      token1 = new PSViewIntegerToken(mgr->getPSView(), file_stat.st_ctime);

      mgr->getPSView()->getOperandStack()->push(token1);
    }

    if (error == 0)
      trueOp(mgr);
    else
      falseOp(mgr);
  }
}

void
PSViewOperatorMgr::
stopOp(PSViewOperatorMgr *mgr)
{
  int num = mgr->getPSView()->getOperandStack()->size();

  int i = 1;

  for ( ; i <= num; i++) {
    PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

    if (token->isOperator() && token->toString() == "stopped")
      break;
  }

  if (i <= num)
    return;

  PSViewNameToken *key = new PSViewNameToken(mgr->getPSView(), "handleerror");

  PSViewToken *value = mgr->getPSView()->getDictionaryMgr()->lookup(key);

  if (value->isNull())
    mgr->getPSView()->getErrorMgr()->defaultErrorHandler();
  else
    value->execute();
}

void
PSViewOperatorMgr::
stoppedOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  PSViewOperatorToken *token1 = new PSViewOperatorToken(mgr->getPSView(), "stopped");

  mgr->getPSView()->getOperandStack()->push(token1);

  token->execute();

  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();

  if (token2 != NULL && token2->isOperator() &&
      token2->toString() == "stopped")
    falseOp(mgr);
  else {
    if (token2 != NULL)
      mgr->getPSView()->getOperandStack()->push(token2);

    trueOp(mgr);
  }
}

void
PSViewOperatorMgr::
storeOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  PSViewDictionaryToken *dictionary =
    mgr->getPSView()->getDictionaryMgr()->lookupDictionary(token1);

  if (dictionary != NULL)
    dictionary->addValue(token1, token2);
  else
    mgr->getPSView()->getDictionaryMgr()->addToCurrent(token1, token2);
  }

void
PSViewOperatorMgr::
stringOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger length = token->getIntegerValue();

  PSViewStringToken *string_token = new PSViewStringToken(mgr->getPSView(), length);

  mgr->getPSView()->getOperandStack()->push(string_token);
}

void
PSViewOperatorMgr::
stringWidthOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewStringToken *string_token = (PSViewStringToken *) token;

  string str = string_token->toString();

  double x, y;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->stringWidth(str, &x, &y);

  PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), x);

  mgr->getPSView()->getOperandStack()->push(token1);

  token1 = new PSViewRealToken(mgr->getPSView(), y);

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
strokeOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->stroke();
}

void
PSViewOperatorMgr::
strokePathOp(PSViewOperatorMgr *mgr)
{
  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->strokePath();
}

void
PSViewOperatorMgr::
subOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber() || ! token2->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewToken *token;

  if (token1->isInteger() && token2->isInteger()) {
    PSVinteger result = token1->getIntegerValue() - token2->getIntegerValue();

    token = new PSViewIntegerToken(mgr->getPSView(), result);
  }
  else {
    PSVreal result = token1->getRealValue() - token2->getRealValue();

    token = new PSViewRealToken(mgr->getPSView(), result);
  }

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
systemDictOp(PSViewOperatorMgr *mgr)
{
  PSViewDictionaryToken *token = mgr->getPSView()->getDictionaryMgr()->getSystemDictionary();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
tokenOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isFile() && ! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  PSViewToken *token1;

  if (token->isFile()) {
    PSViewFileToken *file_token = (PSViewFileToken *) token;

    token1 = file_token->readToken();
  }
  else {
    PSViewStringToken *string_token = (PSViewStringToken *) token;

    string str = string_token->toString();

    PSVinteger pos = 0;

    token1 = mgr->readStringFileToken(str, &pos);

    if (token1 != NULL) {
      string_token->setBounds(pos + 1, str.size() - pos);

      mgr->getPSView()->getOperandStack()->push(token);
    }
  }

  if (token1 != NULL) {
    mgr->getPSView()->getOperandStack()->push(token1);
    trueOp(mgr);
  }
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
transformOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token2->isNumber()) {
    PSVreal x = token1->getRealValue();
    PSVreal y = token2->getRealValue();

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    CMatrix2D *matrix = gstate_token->getCTMMatrix();

    PSVreal x1, y1;

    matrix->multiplyPoint(x, y, &x1, &y1);

    token1 = new PSViewRealToken(mgr->getPSView(), x1);
    token2 = new PSViewRealToken(mgr->getPSView(), y1);

    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token2);
  }
  else {
    CAutoPtr<CMatrix2D> matrix;

    matrix = token2->getMatrix();

    if (matrix == NULL)
      return;

    PSViewToken *token0 = mgr->getPSView()->getOperandStack()->pop();

    if (! token0->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x = token0->getRealValue();
    PSVreal y = token1->getRealValue();

    PSVreal x1, y1;

    matrix->multiplyPoint(x, y, &x1, &y1);

    token1 = new PSViewRealToken(mgr->getPSView(), x1);
    token2 = new PSViewRealToken(mgr->getPSView(), y1);

    mgr->getPSView()->getOperandStack()->push(token1);
    mgr->getPSView()->getOperandStack()->push(token2);
  }
}

void
PSViewOperatorMgr::
translateOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token2->isNumber()) {
    PSVreal x = token1->getRealValue();
    PSVreal y = token2->getRealValue();

    CMatrix2D matrix;

    matrix.setTranslation(x, y);

    PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

    gstate_token->postMultiplyCTMMatrix(&matrix);
  }
  else {
    if (! token2->isMatrix()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSViewToken *token0 = mgr->getPSView()->getOperandStack()->pop();

    if (! token0->isNumber()) {
      mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

      return;
    }

    PSVreal x = token0->getRealValue();
    PSVreal y = token1->getRealValue();

    CMatrix2D matrix;

    matrix.setTranslation(x, y);

    token2->setMatrix(&matrix);

    mgr->getPSView()->getOperandStack()->push(token2);
  }
}

void
PSViewOperatorMgr::
trueOp(PSViewOperatorMgr *mgr)
{
  PSViewBooleanToken *token = new PSViewBooleanToken(mgr->getPSView(), true);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
truncateOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isNumber()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token->isReal()) {
    PSVreal real = token->getRealValue();

    if (real >= 0)
      real = (double) ((long) real);
    else
      real = (double) ((long) real);

    PSViewRealToken *token1 = new PSViewRealToken(mgr->getPSView(), real);

    mgr->getPSView()->getOperandStack()->push(token1);
  }
  else {
    PSViewToken *token1 = token->dup();

    mgr->getPSView()->getOperandStack()->push(token1);
  }
}

void
PSViewOperatorMgr::
typeOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  PSViewToken *token1 = token->toName();

  mgr->getPSView()->getOperandStack()->push(token1);
}

void
PSViewOperatorMgr::
undefOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isDictionary()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewDictionaryToken *dictionary_token1 = (PSViewDictionaryToken *) token1;

  dictionary_token1->deleteValue(token2);
}

void
PSViewOperatorMgr::
undefineFontOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->undefineFont(token);
}

void
PSViewOperatorMgr::
undefineUserObjectOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSVinteger ind = token->getIntegerValue();

  if (ind < 0) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewDictionaryToken *dict = mgr->getPSView()->getDictionaryMgr()->getUserDictionary();

  PSViewNameToken *key = new PSViewNameToken(mgr->getPSView(), "UserObjects");

  PSViewToken *user_objects = dict->getValue(key);

  if (user_objects == NULL || ! user_objects->isArray()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_UNDEFINED);

    return;
  }

  PSViewArrayToken *array_token = (PSViewArrayToken *) user_objects;

  PSVinteger len = array_token->getNumValues();

  if (ind >= len) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

    return;
  }

  PSViewNullToken *sub_token = new PSViewNullToken(mgr->getPSView());

  array_token->setValue(ind + 1, sub_token);

  delete key;
}

void
PSViewOperatorMgr::
userDictOp(PSViewOperatorMgr *mgr)
{
  PSViewDictionaryToken *token = mgr->getPSView()->getDictionaryMgr()->getUserDictionary();

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
userTimeOp(PSViewOperatorMgr *mgr)
{
  time_t real_time = time(NULL);

  int dt = (int) (real_time - mgr->getPSView()->getStartTime());

  PSViewToken *token = new PSViewIntegerToken(mgr->getPSView(), dt);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
versionOp(PSViewOperatorMgr *mgr)
{
  static string version = "1.0";

  PSViewStringToken *token = new PSViewStringToken(mgr->getPSView(), version);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
vmStatusOp(PSViewOperatorMgr *mgr)
{
  int depth, used, max;

  mgr->getPSView()->getMemoryMgr()->getStatus(&depth, &used, &max);

  PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), depth);

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewIntegerToken(mgr->getPSView(), used);

  mgr->getPSView()->getOperandStack()->push(token);

  token = new PSViewIntegerToken(mgr->getPSView(), max);

  mgr->getPSView()->getOperandStack()->push(token);
}

void
PSViewOperatorMgr::
wcheckOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (! token->isArray() && ! token->isPackedArray() &&
      ! token->isDictionary() && ! token->isFile() && ! token->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token->getWritable())
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
whereOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  PSViewDictionaryToken *token1 = mgr->getPSView()->getDictionaryMgr()->lookupDictionary(token);

  if (token1 != NULL) {
    mgr->getPSView()->getOperandStack()->push(token1);

    trueOp(mgr);
  }
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
widthShowOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token4 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token3 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL ||
      token3 == NULL || token4 == NULL)
    return;

  if (! token1->isNumber () || ! token2->isNumber() ||
      ! token3->isInteger() || ! token4->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double cx = token1->getRealValue();
  double cy = token2->getRealValue();

  int c = token3->getIntegerValue();

  PSViewStringToken *string_token4 = (PSViewStringToken *) token4;

  string str = string_token4->toString();

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  gstate_token->widthShow(cx, cy, c, str);
}

void
PSViewOperatorMgr::
writeOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isFile() || ! token2->isInteger()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken *file_token1 = (PSViewFileToken *) token1;

  PSVinteger c = token2->getIntegerValue();

  file_token1->writeChar((PSVchar) c);
}

void
PSViewOperatorMgr::
writeHexStringOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isFile() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken   *file_token1   = (PSViewFileToken   *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  PSVinteger length = string_token2->getLength();

  for (PSVinteger i = 1; i <= length; i++) {
    PSVinteger c = string_token2->getChar(i);

    char hex_string[16];

    sprintf(hex_string, "%02x", (int) c);

    file_token1->writeChar(hex_string[0]);
    file_token1->writeChar(hex_string[1]);
  }
}

void
PSViewOperatorMgr::
writeStringOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isFile() || ! token2->isString()) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  PSViewFileToken   *file_token1   = (PSViewFileToken   *) token1;
  PSViewStringToken *string_token2 = (PSViewStringToken *) token2;

  PSVinteger length = string_token2->getLength();

  for (PSVinteger i = 1; i <= length; i++) {
    PSVinteger c = string_token2->getChar(i);

    file_token1->writeChar((PSVchar) c);
  }
}

void
PSViewOperatorMgr::
xcheckOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token = mgr->getPSView()->getOperandStack()->pop();

  if (token == NULL)
    return;

  if (token->isExecutable())
    trueOp(mgr);
  else
    falseOp(mgr);
}

void
PSViewOperatorMgr::
xorOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if ((! token1->isBoolean() && ! token1->isInteger()) ||
      (! token2->isBoolean() && ! token2->isInteger()) ||
      ! token1->isType(token2->getType())) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  if (token1->isBoolean()) {
    PSVboolean boolean1 = token1->getBooleanValue();
    PSVboolean boolean2 = token2->getBooleanValue();

    if (boolean1 ^ boolean2)
      trueOp(mgr);
    else
      falseOp(mgr);
  }
  else {
    PSVinteger integer1 = token1->getIntegerValue();
    PSVinteger integer2 = token2->getIntegerValue();

    PSViewIntegerToken *token = new PSViewIntegerToken(mgr->getPSView(), integer1 ^ integer2);

    mgr->getPSView()->getOperandStack()->push(token);
  }
}

void
PSViewOperatorMgr::
xshowOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isString() ||
      (! token2->isArray() && ! token2->isString())) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double current_x, current_y;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  bool flag = gstate_token->getCurrentPoint(&current_x, &current_y);

  if (! flag) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);

    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

  if (token2->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token2;

    PSVinteger num_tokens = array_token->getNumValues();

    vector<double> x;

    x.resize(num_tokens);

    for (int i = 1; i <= num_tokens; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isNumber()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

        return;
      }

      x[i - 1] = sub_token->getRealValue();
    }

    string str = " ";

    PSVinteger length = string_token1->getLength();

    for (int i = 1; i <= length; i++) {
      if (i > num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);

        return;
      }

      str[0] = string_token1->getChar(i);

      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->moveTo(current_x, current_y);

      gstate_token->show(str);

      current_x += x[i - 1];
    }
  }
  else
    CStrUtil::eprintf("PSView: NumString not supported for XShow\n");
}

void
PSViewOperatorMgr::
xyshowOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isString() ||
      (! token2->isArray() && ! token2->isString())) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

    return;
  }

  double current_x, current_y;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  bool flag = gstate_token->getCurrentPoint(&current_x, &current_y);

  if (! flag) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);

    return;
  }

  if (token2->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token2;

    PSVinteger num_tokens = array_token->getNumValues();

    vector<double> x, y;

    x.resize(num_tokens/2 + 1);
    y.resize(num_tokens/2 + 1);

    for (int i = 1; i <= num_tokens; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isNumber()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);

        return;
      }

      if (i & 1)
        x[i/2    ] = sub_token->getRealValue();
      else
        y[i/2 - 1] = sub_token->getRealValue();
    }

    string str = " ";

    PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

    PSVinteger length = string_token1->getLength();

    for (int i = 1; i <= length; i++) {
      if (2*(i - 1) > num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
        return;
      }

      str[0] = string_token1->getChar(i);

      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->moveTo(current_x, current_y);

      gstate_token->show(str);

      current_x += x[i - 1];
      current_y += y[i - 1];
    }
  }
  else
    CStrUtil::eprintf("PSView: NumString not supported for XYShow\n");
}

void
PSViewOperatorMgr::
yshowOp(PSViewOperatorMgr *mgr)
{
  PSViewToken *token2 = mgr->getPSView()->getOperandStack()->pop();
  PSViewToken *token1 = mgr->getPSView()->getOperandStack()->pop();

  if (token1 == NULL || token2 == NULL)
    return;

  if (! token1->isString() || (! token2->isArray() && ! token2->isString())) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
    return;
  }

  double current_x, current_y;

  PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

  bool flag = gstate_token->getCurrentPoint(&current_x, &current_y);

  if (! flag) {
    mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_NO_CURRENT_POINT);
    return;
  }

  PSViewStringToken *string_token1 = (PSViewStringToken *) token1;

  if (token2->isArray()) {
    PSViewArrayToken *array_token = (PSViewArrayToken *) token2;

    PSVinteger num_tokens = array_token->getNumValues();

    vector<double> y;

    y.resize(num_tokens);

    for (int i = 1; i <= num_tokens; i++) {
      PSViewToken *sub_token = array_token->getValue(i);

      if (! sub_token->isNumber()) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_TYPE_CHECK);
        return;
      }

      y[i - 1] = sub_token->getRealValue();
    }

    string str = " ";

    PSVinteger length = string_token1->getLength();

    for (int i = 1; i <= length; i++) {
      if (i > num_tokens) {
        mgr->getPSView()->getErrorMgr()->raise(PSVIEW_ERROR_TYPE_RANGE_CHECK);
        return;
      }

      str[0] = string_token1->getChar(i);

      PSViewGStateToken *gstate_token = mgr->getPSView()->getGStateTokenMgr()->getCurrent();

      gstate_token->moveTo(current_x, current_y);

      gstate_token->show(str);

      current_y += y[i - 1];
    }
  }
  else
    CStrUtil::eprintf("PSView: NumString not supported for XShow\n");
}

void
PSViewOperatorMgr::
unimplementedOp(PSViewOperatorMgr *mgr)
{
  CStrUtil::eprintf("PSView: Operator Unimplemented\n");

  CStrUtil::eprintf("  Last Executed ");

  PSViewToken *token = mgr->getPSView()->getTokenMgr()->getLastExecuteToken();

  token->print();

  CStrUtil::printf("\n");
}

void
PSViewOperatorMgr::
charPathFillOp(PSViewOperatorMgr *)
{
}

void
PSViewOperatorMgr::
charPathStrokeTrueOp(PSViewOperatorMgr *)
{
  CStrUtil::eprintf(
    "PSView: Conversion from Stroke Path to Fill Path Unimplemented\n");
}

void
PSViewOperatorMgr::
charPathStrokeFalseOp(PSViewOperatorMgr *)
{
}

PSViewToken *
PSViewOperatorMgr::
readStringFileToken(const string &str, PSVinteger *pos)
{
  PSViewStringFileToken *token = new PSViewStringFileToken(getPSView(), str.substr(*pos));

  PSViewToken *token1 = token->readToken();

  *pos += token->getFile()->bytesUsed();

  delete token;

  return token1;
}

//---------------

PSViewOperator::
PSViewOperator(PSViewOperatorMgr *mgr, const string &name,
               PSViewOperatorProc proc) :
 mgr_(mgr), name_(mgr_->getPSView()->getNameMgr()->getName(name)),
 proc_(proc) {
}

int
PSViewOperator::
compare(PSViewOperator *opr)
{
  return name_.compare(opr->name_);
}

void
PSViewOperator::
invoke()
{
  (*proc_)(mgr_);
}
