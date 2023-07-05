#include "mxFilename.h"
#include <wx/filename.h>
#include "mxUtils.h"

static char path_sep = wxFileName::GetPathSeparator();

/** 
* Concatena una ruta y un nombre de archivo. Si este es relativo, agregando la 
* barra si es necesario. Si el "archivo" era una ruta absoluta, la devuelve sin cambios.
* Esta función se creó para encapsular las diferencias entre Windows y GNU/Linux,
* particularmente, el caracter de separación, y la presencia/ausencia de la unidad.
* La macro DIR_PLUS_FILE encapsula esta llamada utilizando el objeto utils.
* @param dir El directorio base
* @param fil El archivo. Puede ser solo un nombre, la parte final de una ruta, o una ruta completa
* @return Un wxString con la ruta resultante completa
**/
wxString mxFN::Join(const wxString &dir, const wxString &fil) {
	if (dir.Len()==0 || (fil.Len()>1 && (fil[0]=='\\' || fil[0]=='/' || fil[1]==':')))
		return fil;
	else if (dir.Last()==path_sep)
		return dir+fil;
	else
		return dir+path_sep+fil;
}

wxString mxFN::Join(const wxString &dir1, const wxString &dir2, const wxString &fil) {
	return Join(Join(dir1,dir2),fil);
}
/** 
* Convierte un path absoluto en relativo, siempre y cuando no deba subir más
* de 2 niveles desde el path de referencia
**/
wxString mxFN::MakeRelative(wxString name, wxString path) {
	if (path.Last()=='\\' || path.Last()=='/') path.RemoveLast();
	bool rr=false;
	if (rr) name.RemoveLast();
	wxFileName fname(name);
	if (fname==path) return ".";
	fname.MakeRelativeTo(path);
	wxString rname = fname.GetFullPath();
	if (rname.StartsWith(_T("../../..")) || rname.StartsWith(_T("..\\..\\..")))
		return name;
	else 
		return rname;
}

#ifdef __WIN32__
#	define _is_path_char(str,p) (str[p]=='\\'||str[p]=='/'||p==1&&str[p]==':')
#else
#	define _is_path_char(str,p) (str[p]=='/')
#endif

int mxFN::NORMALIZE_FLAGS = wxPATH_NORM_DOTS|wxPATH_NORM_TILDE|wxPATH_NORM_LONG;

wxString mxFN::Normalize (const wxString &path) {
	wxFileName fname(path);
	fname.Normalize(mxFN::NORMALIZE_FLAGS);
	return fname.GetFullPath();
}

wxString mxFN::GetFileName (const wxString &fullpath, bool with_extension) {
	if (fullpath.IsEmpty()) return "";
	int i = fullpath.Len()-1, pdot=-1;
	while (i>=0 && !_is_path_char(fullpath,i)) {
		if (!with_extension && pdot==-1 && fullpath[i]=='.') pdot=i;
		--i;
	}
	return fullpath.Mid(i+1,pdot==-1?wxString::npos:(pdot-i-1));
}

wxString mxFN::GetPath (const wxString &fullpath, bool or_dot) {
	if (fullpath.IsEmpty()) return or_dot?".":"";
	int i = fullpath.Len()-1;
	while (i>=0 && !_is_path_char(fullpath,i)) --i;
	if (or_dot && i<0) return ".";
	return fullpath.Mid(0,(i<0||(i==1&&fullpath[i]==':'))?i+1:i);
}

wxString mxFN::RemoveTrailingSlash (const wxString & fullpath) {
	if (fullpath.IsEmpty()) return fullpath;
	if (fullpath.Last()!='/' _if_win32(&& fullpath.Last()!='\\',)) return fullpath;
	if (fullpath.Len()==1 _if_win32(|| (fullpath.Len()==3&&fullpath[1]==':'),)) return fullpath;
	return fullpath.Mid(0,fullpath.Len()-1);
}

