#include <iostream>
#include <wx/msgdlg.h>
#include "Application.h"
#include "mxInfoWindow.h"
#include "ComplementArchive.h"
#include "mxCreateComplementWindow.h"
#include "mac-stuff.h"

bool spanish=false;

// sample running args for debugging:

// create:
//   workdir: /mnt/ex/complements/mingw64-gcc12/dist/
//   args: --build sfml2 /mnt/rm/borrar.zcp

// install:
//   workdir: /mnt/ex/zinjai
//   args: --lang=spanish /mnt/ex/zinjai/bin/.. /mnt/ex/complements/mingw64-gcc12/packs/zinjai-add-sfml2-w64-20240131.zcp


bool mxApplication::OnInit() {
	wxString zpath,fname;
	bool for_autobuilding = false;
	for(int i=1;i<argc;i++) { 
		wxString argvi(argv[i]);
		if (argvi.StartsWith("--lang=")) {
			spanish=argvi=="--lang=spanish";
		} else if (argvi=="--build") {
			for_autobuilding = true;
		} else {
			if (zpath.Len()==0) zpath=argvi; else fname=argvi;
		}
	}
	
	fix_mac_focus_problem();
	
#ifndef __WIN32__
	std::cerr<<(spanish?"\nNo cierre esta ventana.\n":"\nDo not close this window.\n");
#endif
	if (fname.Len() && !for_autobuilding )
		new mxInfoWindow(zpath,fname);
	else
		new mxCreateComplementWindow(zpath,fname,false);
	return true;
}

IMPLEMENT_APP(mxApplication)

