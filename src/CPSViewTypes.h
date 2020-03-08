#ifndef CPSViewTypes_H
#define CPSViewTypes_H

typedef int          PSVboolean;
typedef long         PSVfont_id;
typedef long         PSVinteger;
typedef int          PSVmark;
typedef int          PSVnull;
typedef double       PSVreal;
typedef uint PSVchar;

class PSViewName;
class PSViewToken;
class PSViewArrayToken;
class PSViewNameToken;
class PSViewStringToken;
class PSViewGStateToken;
class PSViewPath;
class PSViewOperatorMgr;

typedef void  (*PSViewOperatorProc)(PSViewOperatorMgr *);

#endif
