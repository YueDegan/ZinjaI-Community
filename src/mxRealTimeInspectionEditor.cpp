#include <wx/textctrl.h>
#include <wx/sizer.h>
#include "mxRealTimeInspectionEditor.h"
#include "mxMessageDialog.h"
#include "mxMainWindow.h"
#include "mxSizers.h"
#include "mxArt.h"

BEGIN_EVENT_TABLE(mxRealTimeInspectionEditor,wxFrame)
	EVT_CLOSE(mxRealTimeInspectionEditor::OnClose)
	EVT_BUTTON(wxID_ANY,mxRealTimeInspectionEditor::OnButton)
	EVT_TEXT_ENTER(wxID_ANY,mxRealTimeInspectionEditor::OnText)
	EVT_TIMER(wxID_ANY,mxRealTimeInspectionEditor::OnResizeTimer)
END_EVENT_TABLE()

mxRealTimeInspectionEditor::mxRealTimeInspectionEditor(const wxString &expression)
	: wxFrame(main_window,wxID_ANY,expression,wxGetMousePosition()-wxPoint(25,10),wxDefaultSize,wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP)
	, m_resize_timer(GetEventHandler(),wxID_ANY)
{
	SetBackgroundColour(wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ));
	DebuggerInspection *di;
	(di = DebuggerInspection::Create(expression,DIF_DONT_USE_HELPER,this,false))->Init();
	if (di->GetDbiType()!=DIT_VARIABLE_OBJECT) {
		mxMessageDialog(main_window,LANG(REALTIMEINSP_ERROR_CREATING_INSPECTION,"Ha ocurrido un error al registrar la inspección"))
			.Title(LANG(GENERAL_ERROR,"Error")).IconError().Run();
		Destroy(); return;
	}
	
	sizer = new wxFlexGridSizer(2); 
	sizer->AddGrowableCol(1);
	sizer->SetFlexibleDirection(wxBOTH);
	sizer->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_NONE);
	
	Add(0,0,di); if (inspections[0].button) Break(0);
	SetSizer(sizer); Layout(); Resize(false);
	Show();
}

mxRealTimeInspectionEditor::~mxRealTimeInspectionEditor() {
	for(int i=0;i<inspections.GetSize();i++)
		inspections[i].di->Destroy();
	debug->InvalidatePauseEvent(this);
}

void mxRealTimeInspectionEditor::Break (int num) {
	if (!debug->IsPaused()) {
		_DEBUG_LAMBDA_2( lmbBreakInspection, mxRealTimeInspectionEditor,win, int,num, { win->Break(num); } );
		debug->PauseFor(new lmbBreakInspection(this,num));
		return;
	}
	if (inspections[num].broken) {
		int child = num+1;
		while (child<inspections.GetSize() && inspections[child].level > inspections[num].level) {
			AuxRTIE &aux = inspections[child];
			sizer->Detach(aux.label); aux.label->Destroy();
			if (aux.button) { sizer->Detach(aux.button); aux.button->Destroy(); }
			if (aux.text)  { sizer->Detach(aux.text); aux.text->Destroy(); }
			DebuggerInspection *di = aux.di;
			di->Destroy();
			inspections.Remove(child);
		}
	} else {
		DebuggerInspection *di = inspections[num].di;
		SingleList<DebuggerInspection*> children;
		if (!di->Break(children,true,true) || children.GetSize()==0) return;
	//	if (inspections[num].button) inspections[num].button->Enable(false);
		for(int i=0;i<children.GetSize();i++) { 
			Add(num+i+1,inspections[num].level+1,children[i]);
		}
	}
	inspections[num].broken = !inspections[num].broken;
}

void mxRealTimeInspectionEditor::OnClose (wxCloseEvent & evt) {
	Destroy();
}

void mxRealTimeInspectionEditor::Add (int pos, int lev, DebuggerInspection * di) {
	BoolFlagGuard fg(mask_events);
	AuxRTIE aux; aux.di=di; wxString tabs(' ',4*lev); aux.level=lev; aux.broken=false;
	aux.label = new wxStaticText(this,wxID_ANY,tabs+di->GetShortExpression()+": ");
	sizer->Insert(2*pos,aux.label,0,/*wxALIGN_CENTER_HORIZONTAL|*/wxALIGN_CENTER_VERTICAL);
	wxControl *aux_control;
	if (di->AskGDBIfIsEditable()) {
		aux.button = nullptr; 
		aux_control = aux.text = new wxTextCtrl(this,wxID_ANY,di->GetValue(),wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	} else {
		aux.text = nullptr; 
		aux_control = aux.button = new wxButton(this,wxID_ANY,di->GetValue(),wxDefaultPosition,wxDefaultSize,wxNO_BORDER|wxBU_EXACTFIT);
	}
	
	if (pos==0) {
		wxSizer *aux_sizer = new wxBoxSizer(wxHORIZONTAL);
		aux_sizer->Add(aux_control,sizers->Exp1);
		wxString reload_bmp_path = wxString(config->HighDPI()?"dialogs/b24/":"dialogs/b16/")+"button_reload.png";
		aux_sizer->Add(new wxBitmapButton(this,wxID_REDO,bitmaps->GetBitmap(reload_bmp_path)));
		sizer->Insert(2*pos+1,aux_sizer,sizers->Exp1);
	} else {
		sizer->Insert(2*pos+1,aux_control,sizers->Exp1);
	}
	inspections.Insert(pos,aux);
}

void mxRealTimeInspectionEditor::OnButton (wxCommandEvent & evt) {
	if (evt.GetId()==wxID_REDO) { OnUpdateValues(evt); return; }
	for(int i=0;i<inspections.GetSize();i++) { 
		if (evt.GetEventObject()==inspections[i].button) { 
			Break(i); 	
			// no llamar directamente a resize porque si se llega a aca sin que el usuario
			// pause la ejecucion, ya estamos en un yield, tonces el layout y cia no actualizan
			// nada (al menos en linux con wx 2.8)
			m_resize_timer.Start(20,true); 
			return;
		}
	}
}

void mxRealTimeInspectionEditor::OnText (wxCommandEvent & evt) {
	if (mask_events) return;
	if (!debug->IsDebugging()) return;
	for(int i=0;i<inspections.GetSize();i++) {
		if (evt.GetEventObject()==inspections[i].text) {
			_DEBUG_LAMBDA_2(mlbModifyAndUpdateParents,mxRealTimeInspectionEditor,rte,int,i,{
				SingleList<AuxRTIE> &inspections = rte->inspections;
				// cargar el nuevo valor
				rte->inspections[i].di->ModifyValue(inspections[i].text->GetValue());
				// forzar la actualizacion de los padres
				while (true) {
					int p = i-1;
					while(p>=0 && inspections[p].level==inspections[i].level) --p;
					if (p<0) break; 
					i=p; inspections[i].di->UpdateValue(true);
				}
			});
			debug->PauseFor(new mlbModifyAndUpdateParents(this,i));
			return;
		}
	}
}

void mxRealTimeInspectionEditor::Resize(bool only_grow_h) {
	wxSize old_size = WindowToClientSize(GetSize());
	Layout(); /*wxYield();*/ 
	GetSizer()->RecalcSizes();
	wxSize fit_size = GetSizer()->ComputeFittingClientSize(this);
	int h=fit_size.GetHeight(); 
	if  (only_grow_h) {
		if (h>old_size.GetHeight()) {
			SetSize(ClientToWindowSize(wxSize(old_size.GetWidth(),h)));
		}
	} else {
		int w=0;
		for(int i=0;i<inspections.GetSize();i++) { 
			wxSize sz = inspections[i].label->GetSize();
			if (sz.GetWidth()>w) w=sz.GetWidth();
		}
		SetSize(ClientToWindowSize(wxSize(w+100,h)));
	}
 }

void mxRealTimeInspectionEditor::OnDIError (DebuggerInspection * di) {
	for(int i=0;i<inspections.GetSize();i++) { 
		if (inspections[i].di==di) {
			if (inspections[i].button) inspections[i].button->Enable(false);
			if (inspections[i].text) inspections[i].text->Enable(false);
			return;
		}
	}
}

void mxRealTimeInspectionEditor::OnDIValueChanged (DebuggerInspection * di) {
	for(int i=0;i<inspections.GetSize();i++) {
		if (inspections[i].di==di) {
			if (inspections[i].button) inspections[i].button->SetLabel(di->GetValue());
			if (inspections[i].text) inspections[i].text->SetValue(di->GetValue());
			return;
		}
	}
}

void mxRealTimeInspectionEditor::OnDIOutOfScope (DebuggerInspection * di) {
	OnDIError(di);
}


// none of this seems to work if gdb is stopped without a breakpoint (so it stops 
// but do not now where, evaluations gives old value or nothing)
void mxRealTimeInspectionEditor::OnUpdateValues (wxCommandEvent & evt) {
	if (!debug->IsPaused()) {
		_DEBUG_LAMBDA_1( lmbUpdateRTIEditor, mxRealTimeInspectionEditor,win, 
			             { wxCommandEvent evt; win->OnUpdateValues(evt); } );
		debug->PauseFor(new lmbUpdateRTIEditor(this));
	} else {
		inspections[0].di->ForceVOUpdate();
		for(int i=0;i<inspections.GetSize();i++) { 
			if (inspections[i].di->RequiresManualUpdate())
				inspections[i].di->UpdateValue(true);
		}
	}
}

void mxRealTimeInspectionEditor::OnResizeTimer (wxTimerEvent & evt) {
	Resize(true);
}

