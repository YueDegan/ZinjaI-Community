#ifndef RAII_H
#define RAII_H

#include <wx/filefn.h>
#include "ZLog.h"

/**
* @brief RAII wrapper for objects allocated on heap (new), but with a life-cycle
* as if it where on the stack (attached to a local scope)
**/
class BoolFlag;

/**
* RAII wrapper for handling flags, set a flag on creation and reset it on deletion
**/
template<typename T, typename U=bool, U SET_VALUE=true, U RESET_VALUE=false>
class FlagGuard {
	T *m_flag;
public:
	FlagGuard(T &f, bool force):m_flag(&f) { *m_flag=SET_VALUE; }
	FlagGuard(T *f):m_flag(f) { if (!m_flag || *m_flag==SET_VALUE) { m_flag=nullptr; } else *m_flag=SET_VALUE; }
	FlagGuard(T &f):m_flag(&f) { if (*m_flag==SET_VALUE) { m_flag=nullptr; } else *m_flag=SET_VALUE; }
	bool IsOk() { return m_flag!=nullptr; }
	void Release() { if (m_flag) *m_flag=RESET_VALUE; m_flag=nullptr; }
	~FlagGuard() { if (m_flag) *m_flag=RESET_VALUE; }
};

/**
* Class for bool flags (used mostly for masking events) that enforce use with FlagGuard, and ensure correct initialization
**/
class BoolFlag {
public:
	BoolFlag() : value(false) {}
	operator bool() { return value; }
private:
	template<typename T, typename U, U V1, U V2>
	friend class FlagGuard;
	BoolFlag &operator=(bool b) { value=b; return *this; }
	bool value;
};

typedef FlagGuard<BoolFlag,bool,true,false> BoolFlagGuard;
typedef FlagGuard<bool,bool,true,false> boolFlagGuard;


template<class T>
class RaiiRestoreValue {
	T &variable;
	T original_value;
public:
	RaiiRestoreValue(T &var, T value) 
		: variable(var), original_value(var) 
		{ variable = value; }
	RaiiRestoreValue(T &var) 
		: variable(var), original_value(var) {}
	T GetOriginalValue() { return original_value; }
	~RaiiRestoreValue() { variable = original_value; }
};

class RaiiWorkDirChanger {
	wxString old_dir;
public:
	RaiiWorkDirChanger(const wxString &new_dir) 
		: old_dir(wxGetCwd()) 
	{ 
		ZLINF2("RaiiWorkDirChanger","Changing to: "+new_dir);
		wxSetWorkingDirectory(new_dir); 
	}
	void RestoreNow() { 
		if (old_dir.IsEmpty()) return;
		ZLINF2("RaiiWorkDirChanger","Restoring to: "+old_dir);
		wxSetWorkingDirectory(old_dir); 
		old_dir.Clear(); 
	}
	~RaiiWorkDirChanger() { 
		RestoreNow(); 
	}
};

#endif
