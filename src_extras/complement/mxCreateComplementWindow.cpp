#include "mxCreateComplementWindow.h"
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include "ComplementArchive.h"
#include <wx/stattext.h>
#include <wx/msgdlg.h>
#include <wx/filesys.h>
#include <wx/fs_arc.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/wfstream.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/sstream.h>
#include <iostream>
#include "Application.h"
#include "string_convs.h"
using namespace std;

BEGIN_EVENT_TABLE(mxCreateComplementWindow,wxFrame)
	EVT_BUTTON(wxID_OK,mxCreateComplementWindow::OnButtonCreate)
	EVT_BUTTON(wxID_CANCEL,mxCreateComplementWindow::OnButtonCancel)
	EVT_BUTTON(wxID_FIND,mxCreateComplementWindow::OnButtonDest)
	EVT_BUTTON(wxID_REPLACE,mxCreateComplementWindow::OnButtonFolder)
	EVT_CLOSE(mxCreateComplementWindow::OnClose)
END_EVENT_TABLE()

mxCreateComplementWindow *create_win=NULL;

#ifdef __WIN32__
#define sep "\\"
#else
#define sep "/"
#endif

	
enum STEPS_CREATE {STEP_ASKING,STEP_ANALYSING,STEP_BUILDING,STEP_DONE,STEP_ABORTING};

// devuelve true si el compresor puede seguir, false si debe abortar
bool callback_create(wxString message, int progress) {
	if (message.Len()) create_win->Notify(message);
	return create_win->GetStep()!=STEP_ABORTING || wxYES!=wxMessageBox(SP("¿Desea interrumpir el proceso?","Abort the process?"),SP("Generación de Complementos","Complement Generation"),wxYES_NO);
}
	

void FindFiles(wxArrayString &array, wxString where, wxString sub, wxArrayString &bins) {
	
	if (where.EndsWith(sep)) where.RemoveLast();
	if (sub.EndsWith(sep)) sub.RemoveLast();
	
	wxDir fil(sub.Len()?where+sep+sub:where);
	if ( fil.IsOpened() ) {
		wxString filename;
		bool cont = fil.GetFirst(&filename, "*" , wxDIR_FILES);
		while ( cont ) {
			array.Add(sub+sep+filename);
			if (wxFileName::IsFileExecutable(where+array.Last()))
				bins.Add(array.Last());
			cont = fil.GetNext(&filename);
		}	
	}
	
	wxDir dir(sub.Len()?where+sep+sub:where);
	if ( dir.IsOpened() ) {
		wxString filename, spec;
		bool cont = dir.GetFirst(&filename, spec , wxDIR_DIRS);
		while ( cont ) {
			FindFiles(array,where,sub+sep+filename,bins);
			cont = dir.GetNext(&filename);
		}	
	}
	
}

wxCheckBox *addCheck(wxFrame *frame, wxSizer *sizer, wxString label) {
	wxCheckBox *ctrl = new wxCheckBox(frame,wxID_ANY,label);
	sizer->Add(ctrl,wxSizerFlags().Proportion(0).Expand().Border(wxALL,5));
	return ctrl;
}
	
wxTextCtrl *addShortText(wxFrame *frame, wxSizer *sizer, const wxString &label) {
	wxBoxSizer *inner_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *label_ctrl = new wxStaticText(frame,wxID_ANY,label);
	inner_sizer->Add(label_ctrl,wxSizerFlags().Center());
	wxTextCtrl *text_ctrl = new wxTextCtrl(frame,wxID_ANY,"",wxDefaultPosition,wxDefaultSize);
	inner_sizer->Add(text_ctrl,wxSizerFlags().Proportion(1).Expand());
	sizer->Add(inner_sizer,wxSizerFlags().Proportion(0).Expand().Border(wxALL,5));
	return text_ctrl;
}

wxTextCtrl *addPath(wxFrame *frame, wxBoxSizer *sizer, const wxString &label, wxWindowID id) {
	wxBoxSizer *inner_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *sdest= new wxStaticText(frame,wxID_ANY,label);
	sizer->Add(sdest,wxSizerFlags().Proportion(0).Expand().Border(wxTOP|wxLEFT|wxRIGHT,5));
	wxTextCtrl *text_ctrl = new wxTextCtrl(frame,wxID_ANY,"",wxDefaultPosition,wxDefaultSize);
	inner_sizer->Add(text_ctrl,wxSizerFlags().Proportion(1).Expand());
	wxButton *but_dest = new wxButton(frame,id,"...",wxDefaultPosition,wxSize(30,text_ctrl->GetSize().GetHeight()));
	inner_sizer->Add(but_dest);
	sizer->Add(inner_sizer,wxSizerFlags().Proportion(0).Expand().Border(wxALL,5));
	return text_ctrl;
}

wxTextCtrl *addLongText(wxFrame *frame, wxBoxSizer *sizer, const wxString &label) {
	wxStaticText *static_text_ctrl = new wxStaticText(frame,wxID_ANY,label);
	sizer->Add(static_text_ctrl,wxSizerFlags().Proportion(0).Expand().Border(wxTOP|wxLEFT|wxRIGHT,5));
	wxTextCtrl *text_ctrl = new wxTextCtrl(frame,wxID_ANY,"",wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_WORDWRAP);
	text_ctrl->SetMinSize(wxSize(400,100));
	sizer->Add(text_ctrl,wxSizerFlags().Proportion(1).Expand().Border(wxALL,5));
	return text_ctrl;
}

mxCreateComplementWindow::mxCreateComplementWindow(wxString in_path, wxString out_fname, bool write_desc) 
	: wxFrame(NULL,wxID_ANY,SP("Generación de complementos","Complement Generation"),wxDefaultPosition,wxDefaultSize)
	, autoclose(false), step(STEP_ASKING), write_desc_ini(write_desc)
{
	create_win=this;
	wxFileSystem::AddHandler(new wxArchiveFSHandler);
	
	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	folder = addPath(this,sizer,SP("Directorio con archivos a incluir:","Directory with files to be included:"),wxID_REPLACE);
	text_en = addLongText(this,sizer,SP("Descripción del complemento (ingles):","Complement's description (english):"));
	text_es = addLongText(this,sizer,SP("Descripción del complemento (español):","Complement's description (spanish):"));
	
	text = new wxTextCtrl(this,wxID_ANY,"",wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	text->SetMinSize(wxSize(400,200)); sizer->Add(text,wxSizerFlags().Proportion(1).Expand().Border(wxALL,5));
	text->Hide();
	
	short_name = addShortText(this,sizer,SP("Nombre Corto: ","Short Name: "));
	comp_version = addShortText(this,sizer,SP("Versión del complemento: ","Complemnt's version: "));
	req_version = addShortText(this,sizer,SP("Versión de ZinjaI requerida: ","Required ZinjaI version: "));
	close = addCheck(this,sizer,SP("Requiere cerrar ZinjaI durante la instalación","Requires closing ZinjaI for correct installation"));
	reset = addCheck(this,sizer,SP("Requiere reiniciar ZinjaI para completar la instalación","Requires restarting ZinjaI after installation"));
	check_details = addCheck(this,sizer,SP("Mostrar detalles durante la compresión","Show details while generating the zcp file"));
	dest = addPath(this,sizer,SP("Archivo de complemento a generar:","Ouput file:"),wxID_FIND);
	
	wxBoxSizer *but_sizer = new wxBoxSizer(wxHORIZONTAL);
	but_create = new wxButton(this,wxID_OK,SP("Generar...","Generate..."));
	but_cancel = new wxButton(this,wxID_CANCEL,SP("Cancelar","Cancel"));
	but_sizer->Add(but_cancel,wxSizerFlags().Border(wxALL,5));
	but_sizer->AddStretchSpacer();
	but_sizer->Add(but_create,wxSizerFlags().Border(wxALL,5));
	sizer->Add(but_sizer,wxSizerFlags().Proportion(0).Expand());
	SetSizerAndFit(sizer);

	Show(); 
//	text->SetFocus();
	
	if (in_path.Len()) {
		SetFolder(in_path);
		if (out_fname.Len()) {
			autoclose = true;
			dest->SetValue(out_fname);
			wxCommandEvent evt;
			OnButtonCreate(evt);
		}
	}


}

#define _return \
but_create->Enable(true);\
step=STEP_ASKING;\
text_es->Show();\
text_en->Show();\
text->Hide();\
but_create->SetLabel(SP("Generar...","Generate...")); \
but_cancel->SetLabel(SP("Cancelar","Cancel")); \
but_create->Enable(); \
Layout();\
return

void mxCreateComplementWindow::OnButtonCreate (wxCommandEvent & evt) {
	if (step==STEP_DONE) { _return; }
	if (step==STEP_ASKING) {
		wxString desc_en=text_en->GetValue(), desc_es=text_es->GetValue();
		if (!desc_es.Len() && !desc_en.Len()) { wxMessageBox(SP("La descripción no debe quedar en blanco.","You must complete at least one description."));  _return; }
		if (!desc_es.Len()) desc_es=desc_en;
		else if (!desc_en.Len()) desc_en=desc_es;
		if (!folder->GetValue().Len()) { wxMessageBox(SP("Falta especificar la carpeta de donde tomar los archivos.","You must specify the origin folder"));  _return; }
		if (!dest->GetValue().Len()) { wxMessageBox(SP("Falta especificar el archivo de destino.","You must specify the complement filename."));  _return; }
		text->SetValue(SP("Generando...\n","Generating...\n"));
		but_create->Enable(false);
		details = check_details->GetValue();
		text->Show(); text_en->Hide(); text_es->Hide(); Layout();
		step=STEP_ANALYSING;
		info.desc_english=desc_en;
		info.desc_spanish=desc_es;
		if (!req_version->GetValue().Len()) {
			info.reqver=0;
		} else if (!req_version->GetValue().ToLong(&info.reqver)) {
			wxMessageBox(SP("El número de versión requerida no es correcto.","Wrong required version number.")); 
			_return;
		}
		info.closereq = close->GetValue();
		info.short_name = short_name->GetValue();
		if (!comp_version->GetValue().Len()) {
			info.version=0;
		} else if (!comp_version->GetValue().ToLong(&info.version)) {
			wxMessageBox(SP("El número de versión del complemento no es correcto.","Wrong complement's version number."));
			_return;
		}
		
		wxArrayString files,bins;
		info.bins.Clear();
		FindFiles(files,folder->GetValue(),"",info.bins);
		
		if (write_desc_ini) {
			wxString desc_text;
			desc_merge(info,desc_text);
			wxString where=folder->GetValue();
			if (!wxFileName::DirExists(where+sep+"temp"))
				wxFileName::Mkdir(where+sep+"temp");
			if (where.EndsWith(sep)) where.RemoveLast();
			wxFile file(where+sep+"temp"+sep+"desc.ini",wxFile::write);
			if (!file.IsOpened()) {
				wxMessageBox(SP("No se pudo crear el archivo desc.ini (debe tener\npermisos de escritura en el directorio seleccionado).","Couldn't create desc.ini file (you must\nhave write privilege in the origin folder).")); 
				_return;
			}
			wxFileOutputStream fos(file);
			string desc_str = _W2S(desc_text);
			fos.Write(desc_str.c_str(),desc_str.size());
			fos.Close();
			file.Close();
		}

		step=STEP_BUILDING;
		if (!CreateZip(callback_create,dest->GetValue(),folder->GetValue(),files)) { 
			wxMessageBox(SP("Ha ocurrido un error durante la compresión (CreateZip).","There's been a error while creating the complement file (CreateZip).")); 
			_return;
		}
		if (step==STEP_ABORTING) { _return; }
		
		Notify(SP("Generación Finalizada.","Generation completed."));
		if (autoclose) Close();
		step=STEP_DONE; but_create->SetLabel(SP("Aceptar","Ok")); but_cancel->SetLabel(SP("Cerrar","Close")); but_create->Enable();
	}
}

void mxCreateComplementWindow::OnButtonCancel (wxCommandEvent & evt) {
	if (step==STEP_ASKING || step==STEP_DONE) Close();
	else step=STEP_ABORTING;
}

void mxCreateComplementWindow::OnClose (wxCloseEvent & evt) {
	Destroy();
}

void mxCreateComplementWindow::Notify(const wxString &message) {
	if (details) {
		text->AppendText(message+"\n");
		int i=text->GetValue().Len()-1;
		text->SetSelection(i,i);
	}
	wxYield();
}

void mxCreateComplementWindow::OnButtonFolder (wxCommandEvent & evt) {
	wxDirDialog dlg(this,SP("Directorio con archivos para el complemento:","Directory with files to be included:"),folder->GetValue());
	if (wxID_OK==dlg.ShowModal()) SetFolder(dlg.GetPath());
}

void mxCreateComplementWindow::OnButtonDest (wxCommandEvent & evt) {
	wxFileDialog dlg(this,SP("Archivo a generar:","File to generate:"),"",dest->GetValue(),"*.zcp;*.ZCP",wxFD_SAVE);
	if (wxID_OK==dlg.ShowModal()) { dest->SetValue(dlg.GetPath()); }
}

int mxCreateComplementWindow::GetStep ( ) {
	return step;
}

void mxCreateComplementWindow::SetFolder (wxString where) {
	info=complement_info();
	folder->SetValue(where);
	if (where.EndsWith(sep)) where.RemoveLast();
	if (wxFileName::FileExists(where+sep+"temp"+sep+"desc.ini")) {
		wxFile file(where+sep+"temp"+sep+"desc.ini");
		wxFileInputStream fis(file);
		wxString desc;
		wxStringOutputStream sos(&desc,w2s_cs);
		fis.Read(sos);
		desc_split(desc,info);
		text_en->SetValue(info.desc_english);
		text_es->SetValue(info.desc_spanish);
		close->SetValue(info.closereq);
		reset->SetValue(info.resetreq);
		req_version->SetValue(wxString()<<info.reqver);
		dest->SetValue(folder->GetValue()+"-"+wxDateTime::Today().Format("%Y%m%d")+".zcp");
	}
}

