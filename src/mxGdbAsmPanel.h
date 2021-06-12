#ifndef MXGDBASMPANEL_H
#define MXGDBASMPANEL_H
#include <wx/panel.h>
#include "Inspection.h"
#include <map>
#include "StringConv.h"

class mxStyledOutput;
class wxCheckBox;

class mxGdbAsmPanel : public wxPanel, public myDIGlobalEventHandler {
	struct AddressRange {
		typedef long long addr_t;
		addr_t m_from, m_to;
		static addr_t Parse(const wxString &address) {
			addr_t result = 0;
			for (int l = address.Len(), i = 2; i<l; ++i) { // 2 por saltear el 0x
				result *= 16; 
				char c = GetCharLowerCase(address,i);
				if (c>='0'&&c<='9') result += address[i]-'0';
				else /*(c>='a'&&c<='f')*/ result += (address[i]-'a'+10);
			}
			return result;
		}
		AddressRange() { m_from = m_to = -1; }
		AddressRange(addr_t adr) { m_from = m_to = adr; }
		bool operator<(const AddressRange &o) const { return m_to<o.m_from; }
		bool Contains(addr_t ad) const { return ad>=m_from && ad<=m_to; }
	};
	int m_marker_handle;
	mxStyledOutput *m_code;
	wxCheckBox *m_asm_step_mode;
	AddressRange m_current_range;
	map<AddressRange,wxString> m_cache;
	map<AddressRange::addr_t,int> m_addr_to_line;
	AddressRange ParseCode(wxString ans); 
public:
	mxGdbAsmPanel(wxWindow *parent);
	void Update() override;
	void OnDebugPausePost() override { Update(); }
	void OnDebugStart()override ;
	void OnCheckStepMove(wxCommandEvent &evt);
	DECLARE_EVENT_TABLE();
};

#endif

