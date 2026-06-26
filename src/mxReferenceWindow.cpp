#include <fstream>
#include <vector>
#include <cctype>
#include <wx/textfile.h>
#include <wx/treectrl.h>
#include "mxReferenceWindow.h"
#include "mxMessageDialog.h"
#include "ConfigManager.h"
#include "mxComplementInstallerWindow.h"
#include "raii.h"
#include "mxMainWindow.h"
#include "fix_filename.h"

#define _index "index.html"
#define ERROR_PAGE(page) wxString(_T("<I>ERROR</I>: La pagina \""))<<page<<_T("\" no se encuentra.")

mxReferenceWindow *mxReferenceWindow::instance=nullptr;

mxReferenceWindow::mxReferenceWindow(wxString page):mxGenericHelpWindow(LANG(CPPREF_CAPTION,"Referencia C/C++"),true) {
	if (!PopulateInitialTree()) return;
	if (!LoadSearchIndex()) {
		search_text->SetValue("error: index file not found - search disabled");
		search_text->Enable(false);
	}
	LoadHelp(mxFN::Join(config->Help.cppreference_dir,_index));
	Show();
}

mxReferenceWindow::~mxReferenceWindow() {
	instance=nullptr;
}	

static wxString stripTags(const wxString str) {
	wxString ret;
	bool in = false;
	for(auto c : str) {
		if (c=='<') in = false;
		else if (c=='>') in = true;
		else if (in) ret << c;
	}
	return ret;
}

void mxReferenceWindow::ScrollToAnchor(wxString anchor) {
	html->ScrollToAnchor(anchor);
	// this scrolls to the bottom of the title line, so title
	// gets cut.... scroll a bit up to ensure its fully visible
	int x=0, y=0; html->GetViewStart(&x,&y);
	html->Scroll(x,std::max(0,y-2));
}

void mxReferenceWindow::ShowPage(wxString page) {
	if (page=="") page=_index;
	page=mxFN::Join(config->Help.cppreference_dir,page);
	if (instance) {
		if (!instance->PopulateInitialTree()) return;
		if (!instance->IsShown()) instance->Show(); 
		else if (instance->IsIconized()) instance->Maximize(false); 
		else instance->Raise();
		instance->LoadHelp(page); 
	} else {
		instance=new mxReferenceWindow(page);
	}
	instance->search_text->SetFocus();
}

void mxReferenceWindow::ShowAndSearch(const wxString &keyword) {
	ShowPage();
	instance->search_text->SetValue(keyword);
	instance->OnSearch(keyword,false);
}

void mxReferenceWindow::LoadHelp (wxString fname, bool update_history) {
	if (update_history) { 
		while(!history_next.empty()) history_next.pop(); 
		if (current_page.Len()) history_prev.push(current_page);
	}
	wxFileName fn(fname);
	current_page=fname;
	current_path=fn.GetPath();
	wxString old_path=wxGetCwd(); 
	RaiiWorkDirChanger cwd_guard(fn.GetPath());
	html->SetPage(ProcessHTML(fn.GetFullName(),this));
	html->Scroll(0,0);
}

void mxReferenceWindow::ShowIndex ( ) {
	LoadHelp(mxFN::Join(config->Help.cppreference_dir,_index));
}

void mxReferenceWindow::OnSearch (wxString value) {
	OnSearch(value,true);
}

void mxReferenceWindow::OnSearch (wxString value, bool update_history) {
	if (update_history) { 
		while(!history_next.empty()) history_next.pop(); 
		if (value.Len()) history_prev.push(current_page);
	}
	tree->DeleteChildren(page_tree_item); 
	tree->SetItemText(page_tree_item,value);
	current_path=config->Help.cppreference_dir;
	current_page=wxString(">")+value;
	wxArrayString aresults;
	if (!value.Len()) {
		mxMessageDialog(this,LANG(HELPW_SEARCH_ERROR_EMPTY,"Debe introducir al menos una palabra clave para buscar"))
			.Title(LANG(GENERAL_ERROR,"Error")).IconWarning().Run();
		return;
	}
	wxString Value=value.Upper();
	wxString result(wxString("<HTML><HEAD></HEAD><BODY><I><B>")<<LANG(HELPW_SEARCH_RESULTS,"Resultados:")<<"</B></I><UL>");
	for(unsigned int i=0;i<search_index.size();i++) { 
		if (search_index[i].second.Upper().Contains(Value)) {
			wxString pre;
			if (search_index[i].first.StartsWith("c/")) pre=" C: ";
			else if (search_index[i].first.StartsWith("cpp/")) pre=" C++: ";
			aresults.Add(wxString("<!--")<<search_index[i].second<<"--><LI>"<<pre<<"<A href=\""<<search_index[i].first<<"\">"<<search_index[i].second<<"</A></LI>");
		}
	}
	aresults.Sort();
	for (unsigned int i=0;i<aresults.GetCount();i++)
		result<<aresults[i];
	result<<_T("</UL></BODY></HTML>");
	if (aresults.GetCount())		
		html->SetPage(result);
	else
		html->SetPage(wxString("<HTML><HEAD></HEAD><BODY><B>")<<LANG(CPPREF_SEARCH_NO_RESULTS_FOR,"No se encontraron coincidencias para \"")<<value<<"\".</B></BODY></HTML>");
}

bool mxReferenceWindow::OnLink (wxString href) {
	while (href.Contains("%")) {
		wxString pre = href.BeforeFirst('%');
		wxString pos = href.AfterFirst('%');
		if (pos.Len()<2) break;
		std::map<char,int> h2d;
		for(int i=0;i<10;i++) h2d['0'+i]=i;
		for(int i=0;i<6;i++) h2d['a'+i]=10+i;
		int c = h2d[tolower(pos[0])]*16 + h2d[tolower(pos[1])];
		if (c<0||c>255) break;
		href = pre << char(c) << pos.Mid(2);
	}
	if (href.StartsWith("http:")) {
		mxUT::OpenInBrowser(href);
	} else if (href.StartsWith("file:")) {
		wxString fname = fix_filename(href.Mid(5));
		mxUT::OpenInBrowser(mxFN::Join(current_path,fname));
	} else if (href[0]=='#') {
		ScrollToAnchor(href.AfterFirst('#'));
	} else if (href.Contains("#")) {
		LoadHelp(mxFN::Join(current_path,href.BeforeFirst('#')));
		ScrollToAnchor(href.AfterFirst('#'));
	} else {
		LoadHelp(mxFN::Join(current_path,fix_filename(href)));
	}
	return true;
}

void mxReferenceWindow::OnPrev ( ) {
	if (history_prev.empty()) return;
	history_next.push(current_page);
	if (history_prev.top().StartsWith(">")) OnSearch(history_prev.top().Mid(1),false);
	else LoadHelp(history_prev.top(),false);
	history_prev.pop();
}

void mxReferenceWindow::OnNext ( ) {
	if (history_next.empty()) return;
	history_prev.push(current_page);
	if (history_next.top().StartsWith(">")) OnSearch(history_next.top().Mid(1),false);
	else LoadHelp(history_next.top(),false);
	history_next.pop();
}

wxString mxReferenceWindow::ProcessHTML (wxString fname, mxReferenceWindow *w) {
	std::vector<wxTreeItemId> tids(10);
	if (w) { w->tree->DeleteChildren(w->page_tree_item); w->items_page.clear(); tids[0]=w->page_tree_item; }
	wxTextFile fil(fname);
	if (!fil.Exists()) return ERROR_PAGE(fname);
	fil.Open();
	bool ignoring = false, /*on_toc = false,*/ first_top= true, in_index = fname=="index.html";
	int div_deep=0, insert_hr = 0; wxString result;
	for ( wxString str = fil.GetFirstLine(); !fil.Eof(); str = fil.GetNextLine() ) {
		if (!ignoring) {
			int p=str.Find("<title");
			if (w && p!=wxNOT_FOUND) {
				wxString title=str.Mid(p).AfterFirst('>').BeforeFirst('<');
				title.Replace("&lt;","<"); title.Replace("&gt;",">"); title.Replace("&amp;","&");
				title.Replace(" - cppreference.com","");
				w->tree->SetItemText(tids[0],title);
			}
			
			p=str.Find("<div class=\"t-navbar\"");
			if (p==wxNOT_FOUND) p=str.Find("class=\"noprint\"");
			ignoring = p!=wxNOT_FOUND; div_deep=1;
			if (ignoring) str=str.Mid(p).AfterFirst('>');
		}
		if (ignoring) {
			do {
				int p1=str.Find("<div");
				int p2=str.Find("</div");
				int p=p1;
				if (p1==wxNOT_FOUND || (p2!=wxNOT_FOUND && p2<p1) ) p=p2;
				if (p!=wxNOT_FOUND) {
					str=str.Mid(p).AfterFirst('>');
					if (p==p1) div_deep++;
					else if (--div_deep==0) { ignoring=false; break; }
				} else break;
			} while (true);
		}
//		if (!ignoring && str.Find("<table id=\"toc\"")!=wxNOT_FOUND) on_toc=true;
//		else if (on_toc && str.Find("</table>")!=wxNOT_FOUND) on_toc=false;
//		else if (on_toc && w) {
//			static int lev=0;
//			int p=str.Find("class=\"toclevel-"); 
//			if (p!=wxNOT_FOUND) lev=str[p+16]-'0';
//			p=str.Find("href=\"#");
//			if (p!=wxNOT_FOUND) {
//				wxString anchor=str.Mid(p).AfterFirst('#').BeforeFirst('\"');
//				p=str.Find("class=\"toctext\""); 
//				wxString lab=str.Mid(p).AfterFirst('>');
//				if (lab.StartsWith("<span>")) lab=lab.AfterFirst('>');
//				lab=lab.BeforeFirst('<');
//				tids[lev]=w->tree->AppendItem(tids[lev-1],lab);
//				w->items_page.push_back(make_pair(tids[lev],anchor));
//			}
//		}
		if (!ignoring /*&& !on_toc*/) {
			if (wxNOT_FOUND!=str.Find("<span class=\"editsection\"")) {
				str=str.Mid(str.Find("</span"));
			}
			str.Replace("</span>","</span> ",true);
			
			if (in_index and str.Contains("<h1")) str=""; // there's a hidden title "C and C++ reference"
			else if (str.Contains("<div id=\"siteSub\">")) str="";
			else if (str.Contains("<div id=\"contentSub\">")) str="";
			else if (str.Contains("<div class=\"printfooter\">")) {
				str="<BR><BR><BR><I>This help page was generated from content archive ";
				if (!m_doc_version.IsEmpty()) { str+="(version "; str+=m_doc_version; str+=") "; }
				str+="donwloaded from <A href=\"http://www.cppreference.com\">www.cppreference.com</a>.</I>";
				str+="<BR><BR><A href=\"file:"; str+=fname;
				str+="\">Click here</A> to open this page in your default web browser.<BR>";
				fil.GetNextLine(); 
			}
			str.Replace("<span class=\"mw-headline\" id=","<a name=",false);
			str.Replace(".svg\"",".png\"",true);
			
			// wxHtmlWindows ignore allmost all styles and have no support for css classes...
			// the next code is an attempt to recover a little style in order to make this content a bit more readable
			
			// for index
			str.Replace("font-size: 2em","font-size: 20pt; font-weight: bold"); // in index "C++ reference" and "C reference" titles
			if (str.Contains("<tr class=\"row")) {
				if (str.Contains("rowtop")) {
					str.Replace("<tr ","<tr border=1 style=\"background:#e0e0e0;\" ");
				} else {
					str.Replace("<tr ","<tr style=\"background:#f0f0f0;\" ");
				}
				if (first_top) first_top = false;
				else {
					if (str.Contains("rowtop"))
						str.Replace("<tr ","<tr><td><br></td></tr><tr ");
					else
						str.Replace("<tr ","<tr style=\"background:#f0f0f0;\"><td colspan=\"3\"><hr></td></tr><tr ");
				}
			}
			
			// for other pages
			if (str.Contains("<h2") or str.Contains("<h3")) {
				str.Replace("<h","<br><hr><h");
				if (w) {
					auto pid = str.Find("name=\"");
					if (pid!=wxNOT_FOUND) {
						auto item = w->tree->AppendItem(w->page_tree_item,stripTags(str));
						wxString id = str.Mid(pid+6).BeforeFirst('\"');
						w->items_page.push_back(std::make_pair(item,id));
					}
				}
			}
			if (str.Contains("t-example-live-lin")) str="";
			str.Replace("<pre","<table><tr><td style=\"background:#e0e0e0;\"><pre");
			str.Replace("</pre","\n</td></tr></table></pre");
			
			// protoypes table at the beginning of each help page
			str.Replace("<tbody>","<tr><td colspan=\"3\"><hr></td></tr>");			
			str.Replace("<tr class=\"t-dcl-sep","<tr><td colspan=\"3\"><hr></td></tr><tr");
			auto p = str.Find("<tbody");
			if (p!=wxNOT_FOUND) str = str.Mid(0,p) + str.Mid(p).AfterFirst('>');
			str.Replace("</tbody>","");
			str.Replace("rowspan=\"142\"",""); // numbers next to functions protypes when listing overloads (why 142?)
			
			result<<str<<'\n';
		}
	}
	fil.Close();
	if (w) w->tree->ExpandAllChildren(w->page_tree_item);
	return result;
}

void mxReferenceWindow::OnTree (wxTreeItemId item) {
	for(unsigned int i=0;i<items_general.size();i++) { 
		if (items_general[i].first==item) {
			current_path=config->Help.cppreference_dir;
			OnLink(items_general[i].second);
			return;
		}
	}
	for(unsigned int i=0;i<items_page.size();i++) { 
		if (items_page[i].first==item) {
			OnLink(wxString("#")+items_page[i].second);
			return;
		}
	}
//	OnSearch(tree->GetItemText(item));
}

bool mxReferenceWindow::LoadSearchIndex ( ) {
	if (search_index.size()) return true; // already loaded
	
	// index tree
	wxTextFile findex(mxFN::Join(config->Help.cppreference_dir,"zinjai_index"));
	if (!findex.Exists()) return false;
	findex.Open();
	for ( wxString str = findex.GetFirstLine(); !findex.Eof(); str = findex.GetNextLine() ) {
		wxString file=str.BeforeFirst(':'), title=str.AfterFirst(':');
		if (file.StartsWith("./")) file=file.Mid(2);
		if (file.Len()&&title.Len()) search_index.push_back(std::make_pair(file,title));
	}
	findex.Close();
	
	// reference's version
	wxTextFile fver(mxFN::Join(config->Help.cppreference_dir,"version"));
	if (fver.Exists()) { fver.Open(); m_doc_version = fver.GetFirstLine(); fver.Close(); }
	
	return true;
}

bool mxReferenceWindow::PopulateInitialTree ( ) {
	static bool index_loaded=false;
	if (index_loaded) return true;
	wxTextFile fil(mxFN::Join(config->Help.cppreference_dir,_index));
	if (!fil.Exists()) {
		mxMessageDialog::mdAns res = 
			mxMessageDialog(main_window,LANG(CPPREF_NOT_FOUND,""
								 "No se encontró el archivo indice. Es probable que la\n"
								 "referencia no está instalada, ya que no se incluye por\n"
								 "defecto en todas las versiones de ZinjaI.\n\n"
								 "Si dispone de acceso a internet puede descargarla e\n"
								 "instalarla como complemento ahora."))
			.Check1(LANG(CPPREF_INSTALL_NOW,"Descargar e instalar ahora"),true)
			.Title("Referencia C/C++").IconInfo().Run();
		if (res.check1) {
			new mxComplementInstallerWindow(this); wxYield();
			mxUT::OpenZinjaiSite("cppreference.html");
		}
		return false;
	}
	
	wxTreeItemId root=tree->AddRoot("Contents");
	wxTreeItemId index = tree->AppendItem(root,"General Index");
	wxTreeItemId item_cpp= tree->AppendItem(index,"C++ reference");
	wxTreeItemId item_c= tree->AppendItem(index,"C reference");
	page_tree_item = tree->AppendItem(root,"In This Page");
	wxTreeItemId last_parent=index;
	items_general.push_back(std::make_pair(index,_index));
	items_general.push_back(std::make_pair(item_c,wxString("c.html")));
	items_general.push_back(std::make_pair(item_cpp,wxString("cpp.html")));
	fil.Open(); bool on_child=false;
	for ( wxString str = fil.GetFirstLine(); !fil.Eof(); str = fil.GetNextLine() ) {
		if (str.Contains("<div class=\"mainpagediv\">")) on_child=true;
		else if (str.Contains("</div>")) on_child=false;
		int p;
		while((p=str.Find("<a href="))!=wxNOT_FOUND) {
			str=str.Mid(p+5);
			wxString location=str.AfterFirst('\"').BeforeFirst('\"');
			wxString name=str.AfterFirst('>').BeforeFirst('<').Trim();
			if (!name.Len()) name=str.AfterFirst('>').AfterFirst('>').BeforeFirst('<').Trim();
			if (location.StartsWith("c/")) {
				wxTreeItemId new_item;
				if (on_child) new_item=tree->AppendItem(last_parent,name);
				else new_item=last_parent=tree->AppendItem(item_c,name);
				items_general.push_back(std::make_pair(new_item,location));
			} else if (location.StartsWith("cpp/")) {
				wxTreeItemId new_item;
				if (on_child) new_item=tree->AppendItem(last_parent,name);
				else new_item=last_parent=tree->AppendItem(item_cpp,name);
				items_general.push_back(std::make_pair(new_item,location));
			}
		}
	}
	tree->Expand(index);
	fil.Close();

	return index_loaded=true;
}

//bool mxReferenceWindow::CurrentPageIsHome ( ) {
//	return html->getc()==_index;
//}

bool mxReferenceWindow::CanPrev ( ) {
	return !history_prev.empty();
}

bool mxReferenceWindow::CanNext ( ) {
	return !history_next.empty();
}

