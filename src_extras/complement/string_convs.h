#ifndef STRING_CONVS_H
#define STRING_CONVS_H
#include <wx/string.h>

#define Z(x) wxString::From8BitData(x)
#define SP(a,b) (spanish?Z(a):Z(b))

#endif
