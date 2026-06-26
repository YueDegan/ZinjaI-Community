#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/settings.h>
#include "Language.h"
#include "mxCommandFinder.h"
#include "mxMainWindow.h"
#include "MenusAndToolsConfig.h"
#include "ProjectManager.h"
#include "DebugManager.h"

BEGIN_EVENT_TABLE(mxCommandFinderText,wxTextCtrl)
	EVT_TEXT(wxID_ANY,mxCommandFinderText::OnText)
	EVT_KEY_DOWN(mxCommandFinderText::OnKey)
END_EVENT_TABLE();

mxCommandFinder::mxCommandFinder ( ) 
	: wxDialog(main_window,wxID_ANY,LANG(FINDCOMMAND_CAPTION,"Buscar comando"),
			   wxPoint((3*main_window->GetScreenRect().GetLeft()+main_window->GetScreenRect().GetRight())/4,
					   (3*main_window->GetScreenRect().GetTop()+main_window->GetScreenRect().GetBottom())/4),
			   wxDefaultSize)
{
	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	text = new mxCommandFinderText(this);
	sizer->Add(text,wxSizerFlags().Proportion(0).Expand());
	SetSizerAndFit(sizer);
	text->SetFocus();
}


static wxString normalize(wxString s) {
	static wxString to = _T("aaaaaaeeeeeeiiiiiioooooouuuuuunn"), 
					from = _T( "\u00E0\u00E1\u00E4\u00C0\u00C1\u00C4\u00E8\u00E9\u00Eb\u00C8\u00C9\u00CB\u00EC\u00ED\u00EF\u00CC\u00CD\u00CF\u00F2\u00F3\u00F6\u00D2\u00D3\u00D6\u00F9\u00FA\u00FC\u00D9\u00DA\u00DC\u00F1\u00D1"); // áàäÁÀÄéèëÉÈËíìïÍÌÏóòöÓÒÖúùüÚÙÜñÑ
	for( wxString::iterator it0=s.begin(); it0!=s.end(); ++it0 ) {
		auto it1 = from.begin(), it2 = to.begin();
		while (it1!=from.end()) {
			if (*it0==*it1) { 
				*it0=*it2; break; 
			}
			++it1; ++it2;
		}
	}
	return s.Lower();
}

static bool matches(const wxArrayString &array, wxString item) {
	for(unsigned int i=0;i<array.GetCount();i++)
		if (item.Contains(array[i])) return true;
	return false;
}

void mxCommandFinderList::SetPattern (wxString str, int x, int y) {
	if (str.Len()<3) { Hide(); return; }
	list->Clear(); ids.clear();
	wxArrayString array;
	mxUT::Split(normalize(str),array,true,false);
	if (!array.GetCount()) { Hide(); return; }
	for(unsigned int i=0;i<MenusAndToolsConfig::mnCOUNT;i++) { 
		MenusAndToolsConfig::myMenu &menu = menu_data->GetMyMenu(i);
		wxString label = menu.label + "->" , full_label = menu.label + "->";
		for(unsigned int j=0;j<menu.items.size();j++) { 
			int props = menu.items[j].properties;
			if (props&MenusAndToolsConfig::maSEPARATOR) {
				continue;
			} else if (props&MenusAndToolsConfig::maBEGIN_SUBMENU) { 
				if (full_label!=label) {
					while (!(menu.items[++j].properties&MenusAndToolsConfig::maEND_SUBMENU))
						;
					continue;
				} // gprof's sub-sub-menu
				full_label = label + menu.items[j].label + " -> "; continue; 
			} else if (props&MenusAndToolsConfig::maEND_SUBMENU) { 
				full_label = label; continue; 
			}
			wxString item_label = full_label + menu.items[j].label;
			item_label.Replace("&","",true);
			if (!project && menu.items[j].properties&MenusAndToolsConfig::maPROJECT) continue;
			if (!debug->IsDebugging() && menu.items[j].properties&MenusAndToolsConfig::maDEBUG) continue;
			if (debug->IsDebugging() && menu.items[j].properties&MenusAndToolsConfig::maNODEBUG) continue;
			if (matches(array,normalize(item_label))) { list->Append(item_label); ids.push_back(menu.items[j].wx_id); }
		}
	}
	if (!list->GetCount()) list->Append("<<no results>>"); else list->SetSelection(0);
	list->Append("");
	Show();
	pos_and_size = wxRect(x,y,list->GetBestSize().GetWidth()+10,list->GetBestSize().GetHeight()+wxSystemSettings::GetMetric(wxSYS_VSCROLL_X)+10);
	SetSize(pos_and_size); timer.Start(100,true);
	list->Delete(list->GetCount()-1);
}

void mxCommandFinderList::KeyUp ( ) {
	int sel = list->GetSelection();
	if (sel>0&&sel<int(list->GetCount()))
		list->SetSelection(sel-1);
}

void mxCommandFinderList::KeyDown ( ) {
	int sel = list->GetSelection();
	if (sel>=0&&sel<int(list->GetCount())-1)
		list->SetSelection(sel+1);
}

void mxCommandFinderList::Select ( ) {
	int sel = list->GetSelection();
	if (sel>=0&&sel<int(list->GetCount())) {
		Hide(); GetParent()->Close();
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED,ids[sel]);
		main_window->GetEventHandler()->ProcessEvent(evt);
	}
}

void mxCommandFinderText::OnText (wxCommandEvent & evt) {
	int x = GetScreenRect().GetBottomLeft().x;
	int y = GetScreenRect().GetBottomLeft().y;
	list.SetPattern(GetValue(),x,y);
	GetParent()->Raise();
}

mxCommandFinderList::mxCommandFinderList (wxWindow * parent) 
	: wxDialog(parent,wxID_ANY,"",wxDefaultPosition,wxDefaultSize,wxNO_BORDER|wxSTAY_ON_TOP)
	, timer(GetEventHandler())
{
	list = new wxListBox(this,wxID_ANY);
	wxSizer * sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(list,wxSizerFlags().Proportion(1).Expand());
	SetSizerAndFit(sizer);
	Connect(wxEVT_TIMER,wxTimerEventHandler(mxCommandFinderList::OnTimer),NULL,this);
}

void mxCommandFinderText::OnKey (wxKeyEvent &evt) {
	if (evt.GetKeyCode()==WXK_RETURN) list.Select();
	else if (evt.GetKeyCode()==WXK_DOWN) list.KeyDown();
	else if (evt.GetKeyCode()==WXK_UP) list.KeyUp();
	else if (evt.GetKeyCode()==WXK_ESCAPE) GetParent()->Close();
	else evt.Skip();
}

mxCommandFinderText::mxCommandFinderText (wxWindow * parent) 
	: wxTextCtrl(parent,wxID_ANY,"",wxDefaultPosition,wxSize(300,-1),wxTE_PROCESS_ENTER), list(parent) 
{
	SetToolTip(LANG(FINDCOMMAND_HELP,"Introduzca una o más palabras claves para buscar, y presione Enter para seleccionar un resultado"));
}

void mxCommandFinderList::OnTimer (wxTimerEvent & evt) {
	SetSize(pos_and_size);
}

