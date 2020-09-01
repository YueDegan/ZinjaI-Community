#ifndef STRINGCONV_H
#define STRINGCONV_H
inline char GetChar(const wxString &s, int i) { return s[i].GetValue(); }
inline char GetCharLowerCase(const wxString &s, int i) { return s[i].GetValue()|32; }
inline bool CharIsInRange(const wxString &s, int i, char f, char t) { return GetChar(s,i)>=f and GetChar(s,i)<=t; }
inline bool CharIsDigit(const wxString &s, int i) { return CharIsInRange(s,i,'0','9'); }
inline int GetCharAsDigit(const wxString &s, int i) { return GetChar(s,i)-'0'; }
inline const char *ToCStyleString(const wxString &wx_str) { return static_cast<const char*>(wx_str.To8BitData()); }
inline std::string ToStdString(const wxString &wx_str) { return ToCStyleString(wx_str); }
#endif

//á: 00E1
//é: 00E9
//í: 00ED
//ó: 00F3
//ú: 00FA
//ñ: 00F1
//¿: 00BF
//¡: 00A1
//Ü: 00FC
