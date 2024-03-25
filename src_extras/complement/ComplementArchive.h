#ifndef COMPLEMENTARCHIVE_H
#define COMPLEMENTARCHIVE_H
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/textfile.h>

struct complement_info {
	wxString short_name; // internal name (for complements/installed)
	long version=0; // complement version (info only, not used for any logic yet)
	wxString desc_english; // english description
	wxString desc_spanish; // spanish description
	wxArrayString bins; // files to set executable permission
	bool resetreq; // requires restarting zinjai after installation
	bool closereq; // requires closing zinjai before intallation
	long reqver; // minimun zinjai's version required
	wxString toolchain; // required toolchain (or empty)
	long files; // cantidad de archivos, para la barra de progreso, solo al descomprimir
	complement_info():resetreq(true),closereq(false),reqver(0),files(0){}
};
bool desc_split(const wxString &text, complement_info &info);
bool desc_merge(const complement_info &info, wxString &text);

bool GetFilesAndDesc(bool (*callback)(wxString message, int progress), const wxString aZipFile, int &fcount, int &dcount, wxString &desc);
bool CreateDirectories(bool (*callback)(wxString message, int progress), const wxString aZipFile, const wxString aTargetDir, wxTextFile *log_file);
bool ExtractFiles(bool (*callback)(wxString message, int progress), const wxString aZipFile, const wxString aTargetDir, wxTextFile *log_file);
bool SetBins(const wxArrayString &files, const wxString aTargetDir, wxTextFile *log_file);

bool CreateZip(bool (*callback)(wxString message, int progress), const wxString aZipFile, const wxString aTargetDir, const wxArrayString &files);


#endif

