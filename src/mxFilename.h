#ifndef MXFILENAME_H
#define MXFILENAME_H

#include <wx/string.h>

class mxFN {
public:
	/** @brief Concatena una ruta y un nombre de archivo **/
	static wxString Join(const wxString &dir, const wxString &fil);
	static wxString Join(const wxString &dir_p1, const wxString &dir_p2, const wxString &fil);
	
	/** @brief Convierte un path absoluto en relativo **/
	static wxString MakeRelative(wxString fname, wxString path);
	
	/** @brief Contrae los ".." de los paths **/
	static wxString Normalize(const wxString &path);
	/** @brief Extrae solo el nombre del archivo, sin la ruta **/
	static wxString GetFileName(const wxString &fullpath, bool with_extension=true);
	/** @brief Extrae solo la ruta, sin el nombre del archivo **/
	static wxString GetPath(const wxString &fullpath, bool or_dot=false);
	/** @brief Si el path termina en '/' o '\', pero no es el raiz, lo remueve**/
	static wxString RemoveTrailingSlash(const wxString &fullpath);
	
	static int NORMALIZE_FLAGS;
};

#endif

