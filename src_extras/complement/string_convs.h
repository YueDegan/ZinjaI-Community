#ifndef STRING_CONVS_H
#define STRING_CONVS_H
#include <wx/string.h>

#define Z(x) wxString::From8BitData(x)
#define SP(a,b) (spanish?Z(a):Z(b))
#define _W2S(wxstring) ((const char*)w2s_cs.cWX2MB(wxstring))
static wxCSConv w2s_cs("ISO-8859-1");

#endif
