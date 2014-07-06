#include "stdafx.h"
#include "ConnInfoUI.h"
#include "Base/CharacterSet.h"
#include "UserMessage.h"
#include "Redis/RedisClient.h"
#include "ConnInfoSubWhd.h"
#include "rapidjson/document.h"†† † // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"††⼀⼀ 昀漀爀 猀琀爀椀渀最椀昀礀 䨀匀伀一ഀ਀⌀椀渀挀氀甀搀攀 ∀爀愀瀀椀搀樀猀漀渀⼀昀椀氀攀猀琀爀攀愀洀⸀栀∀ † // wrapper of C stream for prettywriter as output
using namespace rapidjson;

DUI_BEGIN_MESSAGE_MAP(ConnInfoUI, CNotifyPump)
DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK,OnItemClick)
DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMACTIVATE,OnItemActive)
DUI_END_MESSAGE_MAP()

static const char* const kConfigFilePath = "Config.json" ;
bool ConnInfoUI::m_isConecting = false;

ConnInfoUI::ConnInfoUI(const CDuiString& strXML,CPaintManagerUI* pm):AbstraceUI(pm)
{
††䌀䐀椀愀氀漀最䈀甀椀氀搀攀爀 戀甀椀氀搀攀爀㬀ഀ਀ † CControlUI* pContainer = builder.Create(strXML.GetData(), NULL, NULL, GetPaintMgr(), NULL); // 这里必须传入m_PaintManager，不然子XML不能使用默认滚动条等信息。
††椀昀⠀ 瀀䌀漀渀琀愀椀渀攀爀 ⤀ 笀ഀ਀ † ††琀栀椀猀ⴀ㸀䄀搀搀⠀瀀䌀漀渀琀愀椀渀攀爀⤀㬀ഀ਀ † }
††攀氀猀攀 笀ഀ਀ † ††琀栀椀猀ⴀ㸀刀攀洀漀瘀攀䄀氀氀⠀⤀㬀ഀ਀ † ††爀攀琀甀爀渀㬀ഀ਀ † }
}

void ConnInfoUI::Initialize()
{
††洀开瀀刀昀栀䈀琀渀 㴀 猀琀愀琀椀挀开挀愀猀琀㰀䌀䈀甀琀琀漀渀唀䤀⨀㸀⠀䜀攀琀倀愀椀渀琀䴀最爀⠀⤀ⴀ㸀䘀椀渀搀䌀漀渀琀爀漀氀⠀开吀⠀∀戀琀渀开挀漀渀渀开爀昀栀∀⤀⤀⤀㬀ഀ਀ † m_pAddBtn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("btn_conn_add")));
††洀开瀀䐀攀氀䈀琀渀 㴀 猀琀愀琀椀挀开挀愀猀琀㰀䌀䈀甀琀琀漀渀唀䤀⨀㸀⠀䜀攀琀倀愀椀渀琀䴀最爀⠀⤀ⴀ㸀䘀椀渀搀䌀漀渀琀爀漀氀⠀开吀⠀∀戀琀渀开挀漀渀渀开搀攀氀∀⤀⤀⤀㬀ഀ਀ † m_pAltBtn = static_cast<CButtonUI*>(GetPaintMgr()->FindControl(_T("btn_conn_alt")));
††洀开瀀䰀椀猀琀唀䤀 㴀 猀琀愀琀椀挀开挀愀猀琀㰀䌀䰀椀猀琀唀䤀⨀㸀 ⠀䜀攀琀倀愀椀渀琀䴀最爀⠀⤀ⴀ㸀䘀椀渀搀䌀漀渀琀爀漀氀⠀开吀⠀∀猀攀爀瘀攀爀氀椀猀琀∀⤀⤀⤀㬀ഀ਀紀ഀ਀ഀ਀椀渀琀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀䜀攀琀䤀渀搀攀砀⠀⤀ഀ਀笀ഀ਀ † return 0;
}

DuiLib::CDuiString ConnInfoUI::GetVirtualwndName()
{
††爀攀琀甀爀渀 开吀⠀∀䌀漀渀渀䤀渀昀漀∀⤀㬀ഀ਀紀ഀ਀ഀ਀瘀漀椀搀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀刀攀昀爀攀猀栀圀渀搀⠀⤀ഀ਀笀ഀ਀ † LoadConfig(kConfigFilePath);
}


bool ConnInfoUI::LoadConfig( const std::string& path )
{††ഀ਀ † m_pListUI->RemoveAll();
††⼀⼀⼀ 븀溋⥿⡒�͖殌䕘灑湥ൣ਀ † m_pListUI->SetTextCallback(this);//[1]
††洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀⸀挀氀攀愀爀⠀⤀㬀ഀ਀ † 
††䘀䤀䰀䔀⨀ 昀瀀 㴀 昀漀瀀攀渀⠀欀䌀漀渀昀椀最䘀椀氀攀倀愀琀栀Ⰰ ∀爀∀⤀㬀ഀ਀ † if (!fp) return false;

††䘀椀氀攀匀琀爀攀愀洀 椀猀⠀昀瀀⤀㬀ഀ਀ † Document document;††⼀⼀ 䐀攀昀愀甀氀琀 琀攀洀瀀氀愀琀攀 瀀愀爀愀洀攀琀攀爀 甀猀攀猀 唀吀䘀㠀 愀渀搀 䴀攀洀漀爀礀倀漀漀氀䄀氀氀漀挀愀琀漀爀⸀ഀ਀ † if (document.ParseStream<0>(is).HasParseError())
††笀ഀ਀ † ††唀猀攀爀䴀攀猀猀愀最攀䈀漀砀⠀䜀攀琀䠀圀一䐀⠀⤀Ⰰ ㄀　　㄀㐀Ⰰ 䈀愀猀攀㨀㨀䌀栀愀爀愀挀琀攀爀匀攀琀㨀㨀䄀一匀䤀吀漀唀渀椀挀漀搀攀⠀搀漀挀甀洀攀渀琀⸀䜀攀琀倀愀爀猀攀䔀爀爀漀爀⠀⤀⤀⸀挀开猀琀爀⠀⤀Ⰰ 䴀䈀开䤀䌀伀一䔀刀刀伀刀⤀㬀ഀ਀ † ††昀挀氀漀猀攀⠀昀瀀⤀㬀ഀ਀ † ††爀攀琀甀爀渀 昀愀氀猀攀㬀ഀ਀ † }

††挀漀渀猀琀 嘀愀氀甀攀☀ 猀攀爀瘀攀爀䌀漀渀昀椀最 㴀 搀漀挀甀洀攀渀琀嬀∀匀攀爀瘀攀爀∀崀㬀ഀ਀ † int idx = 0;
††昀漀爀 ⠀嘀愀氀甀攀㨀㨀䌀漀渀猀琀嘀愀氀甀攀䤀琀攀爀愀琀漀爀 椀琀爀 㴀 猀攀爀瘀攀爀䌀漀渀昀椀最⸀䈀攀最椀渀⠀⤀㬀 椀琀爀 ℀㴀 猀攀爀瘀攀爀䌀漀渀昀椀最⸀䔀渀搀⠀⤀㬀 ⬀⬀椀琀爀⤀ഀ਀ † {
†† † const Value& subConfig = *itr;
†† † CDuiString name = Base::CharacterSet::UTF8ToUnicode(subConfig["name"].GetString()).c_str();
†† † m_dicServerInfo[kServerNameIndex].push_back(Base::CharacterSet::UnicodeToANSI(name.GetData()));
†† † m_dicServerInfo[kServerIpIndex].push_back(subConfig["ip"].GetString());
†† † std::ostringstream os;
†† † os<<subConfig["port"].GetInt();
†† † m_dicServerInfo[kServerPortIndex].push_back(os.str());
†† † m_dicServerInfo[kServerAuthIndex].push_back(subConfig["auth"].GetString());

†† † CListTextElementUI* pListElement = new CListTextElementUI;
†† † pListElement->SetTag(idx++);
†† † /// 设置工具提示
†† † pListElement->SetToolTip(m_pListUI->GetToolTip());
†† † m_pListUI->Add(pListElement);
††紀ഀ਀ † fclose(fp);

††爀攀琀甀爀渀 琀爀甀攀㬀ഀ਀紀ഀ਀ഀ਀ഀ਀戀漀漀氀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀匀愀瘀攀䌀漀渀昀椀最⠀ 挀漀渀猀琀 猀琀搀㨀㨀猀琀爀椀渀最☀ 瀀愀琀栀 ⤀ഀ਀笀ഀ਀ † std::size_t theSize = m_dicServerInfo[kServerNameIndex].size();
††⼀⼀䨀猀漀渀㨀㨀嘀愀氀甀攀 猀攀爀瘀攀爀䌀漀渀昀椀最㬀ഀ਀ † //Json::Value rootConfig;

††䘀䤀䰀䔀⨀ 昀瀀 㴀 昀漀瀀攀渀⠀欀䌀漀渀昀椀最䘀椀氀攀倀愀琀栀Ⰰ ∀眀⬀∀⤀㬀ഀ਀ † if (!fp) return false;
††䘀椀氀攀匀琀爀攀愀洀 漀猀⠀昀瀀⤀㬀ഀ਀ † PrettyWriter<FileStream> writer(os);
††眀爀椀琀攀爀⸀匀琀愀爀琀伀戀樀攀挀琀⠀⤀㬀ഀ਀ † writer.String("Server");
††眀爀椀琀攀爀⸀匀琀愀爀琀䄀爀爀愀礀⠀⤀㬀ഀ਀ † for (std::size_t idx=0; idx<theSize; ++idx)
††笀ഀ਀ † ††眀爀椀琀攀爀⸀匀琀愀爀琀伀戀樀攀挀琀⠀⤀㬀ഀ਀ † ††眀爀椀琀攀爀⸀匀琀爀椀渀最⠀∀渀愀洀攀∀⤀㬀ഀ਀ † ††䌀䐀甀椀匀琀爀椀渀最 渀愀洀攀 㴀 䈀愀猀攀㨀㨀䌀栀愀爀愀挀琀攀爀匀攀琀㨀㨀䄀一匀䤀吀漀唀渀椀挀漀搀攀⠀洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀一愀洀攀䤀渀搀攀砀崀嬀椀搀砀崀⤀⸀挀开猀琀爀⠀⤀㬀ഀ਀ഀ਀ † ††眀爀椀琀攀爀⸀匀琀爀椀渀最⠀䈀愀猀攀㨀㨀䌀栀愀爀愀挀琀攀爀匀攀琀㨀㨀唀渀椀挀漀搀攀吀漀唀吀䘀㠀⠀渀愀洀攀⸀䜀攀琀䐀愀琀愀⠀⤀⤀⸀挀开猀琀爀⠀⤀⤀㬀ഀ਀ † ††眀爀椀琀攀爀⸀匀琀爀椀渀最⠀∀椀瀀∀⤀㬀ഀ਀ † ††眀爀椀琀攀爀⸀匀琀爀椀渀最⠀洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀䤀瀀䤀渀搀攀砀崀嬀椀搀砀崀⸀挀开猀琀爀⠀⤀Ⰰ 洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀䤀瀀䤀渀搀攀砀崀嬀椀搀砀崀⸀氀攀渀最琀栀⠀⤀⤀㬀ഀ਀ † ††眀爀椀琀攀爀⸀匀琀爀椀渀最⠀∀瀀漀爀琀∀⤀㬀ഀ਀ † ††眀爀椀琀攀爀⸀䤀渀琀⠀愀琀漀椀⠀洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀倀漀爀琀䤀渀搀攀砀崀嬀椀搀砀崀⸀挀开猀琀爀⠀⤀⤀⤀㬀ഀ਀ † ††眀爀椀琀攀爀⸀匀琀爀椀渀最⠀∀愀甀琀栀∀⤀㬀ഀ਀ † ††眀爀椀琀攀爀⸀匀琀爀椀渀最⠀洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀䄀甀琀栀䤀渀搀攀砀崀嬀椀搀砀崀⸀挀开猀琀爀⠀⤀⤀㬀ഀ਀ † ††眀爀椀琀攀爀⸀䔀渀搀伀戀樀攀挀琀⠀⤀㬀ഀ਀ † }
††眀爀椀琀攀爀⸀䔀渀搀䄀爀爀愀礀⠀⤀㬀ഀ਀ † writer.EndObject();
††昀挀氀漀猀攀⠀昀瀀⤀㬀ഀ਀ † return true;
}


LPCTSTR ConnInfoUI::GetItemText( CControlUI* pControl, int iIndex, int iSubItem )
{
††挀漀渀猀琀 猀琀搀㨀㨀猀琀爀椀渀最☀ 猀琀爀 㴀 洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀椀匀甀戀䤀琀攀洀崀嬀椀䤀渀搀攀砀崀㬀ഀ਀ † pControl->SetUserData(Base::CharacterSet::ANSIToUnicode(str).c_str());
††爀攀琀甀爀渀 瀀䌀漀渀琀爀漀氀ⴀ㸀䜀攀琀唀猀攀爀䐀愀琀愀⠀⤀㬀ഀ਀紀ഀ਀ഀ਀瘀漀椀搀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀伀渀䌀氀椀挀欀⠀吀一漀琀椀昀礀唀䤀☀ 洀猀最⤀ഀ਀笀ഀ਀ † if (msg.pSender == m_pRfhBtn)
††笀ഀ਀ † ††伀渀刀昀栀䌀漀渀渀䤀渀昀漀⠀洀猀最⤀㬀ഀ਀ † }
††攀氀猀攀 椀昀 ⠀洀猀最⸀瀀匀攀渀搀攀爀 㴀㴀 洀开瀀䄀搀搀䈀琀渀⤀ഀ਀ † {
†† † OnAddConnInfo(msg);
††紀ഀ਀ † else if (msg.pSender == m_pDelBtn)
††笀ഀ਀ † ††伀渀䐀攀氀䌀漀渀渀䤀渀昀漀⠀洀猀最⤀㬀ഀ਀ † }
††攀氀猀攀 椀昀 ⠀洀猀最⸀瀀匀攀渀搀攀爀 㴀㴀 洀开瀀䄀氀琀䈀琀渀⤀ഀ਀ † {
†† † OnAltConnInfo(msg);
††紀ഀ਀紀ഀ਀ഀ਀瘀漀椀搀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀伀渀匀攀氀攀挀琀䌀栀愀渀最攀搀⠀ 吀一漀琀椀昀礀唀䤀 ☀洀猀最 ⤀ഀ਀笀ഀ਀ഀ਀紀ഀ਀ഀ਀瘀漀椀搀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀伀渀䤀琀攀洀䌀氀椀挀欀⠀ 吀一漀琀椀昀礀唀䤀 ☀洀猀最 ⤀ഀ਀笀ഀ਀ഀ਀紀ഀ਀ഀ਀瘀漀椀搀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀伀渀䤀琀攀洀䄀挀琀椀瘀攀⠀ 吀一漀琀椀昀礀唀䤀 ☀洀猀最 ⤀ഀ਀笀ഀ਀ † if (m_isConecting)
††笀ഀ਀ † ††唀猀攀爀䴀攀猀猀愀最攀䈀漀砀⠀䜀攀琀䠀圀一䐀⠀⤀Ⰰ ㄀　　㄀㌀Ⰰ 一唀䰀䰀Ⰰ 䴀䈀开䤀䌀伀一䤀一䘀伀刀䴀䄀吀䤀伀一⤀㬀ഀ਀ † ††爀攀琀甀爀渀 㬀ഀ਀ † }
††椀渀琀 椀搀砀 㴀 洀开瀀䰀椀猀琀唀䤀ⴀ㸀䜀攀琀䌀甀爀匀攀氀⠀⤀㬀ഀ਀ † CDuiString* s = new CDuiString(Base::CharacterSet::ANSIToUnicode(m_dicServerInfo[kServerNameIndex][idx]).c_str());
††猀琀搀㨀㨀猀琀爀椀渀最 椀瀀 㴀 洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀䤀瀀䤀渀搀攀砀崀嬀椀搀砀崀㬀ഀ਀ † int         port = atoi(m_dicServerInfo[kServerPortIndex][idx].c_str());
††猀琀搀㨀㨀猀琀爀椀渀最 愀甀琀栀 㴀 洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀䄀甀琀栀䤀渀搀攀砀崀嬀椀搀砀崀㬀ഀ਀ † RedisClient::GetInstance().SetServerInfo(*s, ip, port, auth);
††䐀漀䌀漀渀渀攀挀琀⠀⤀㬀ഀ਀紀ഀ਀ഀ਀瘀漀椀搀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀伀渀䄀搀搀䌀漀渀渀䤀渀昀漀⠀吀一漀琀椀昀礀唀䤀☀ 洀猀最⤀ഀ਀笀ഀ਀ † ConnInfoSubWhd* pWhd = new ConnInfoSubWhd(&m_dicServerInfo, ConnInfoSubWhd::Type_Add);
††椀昀⠀ 瀀圀栀搀 㴀㴀 一唀䰀䰀 ⤀ 笀 爀攀琀甀爀渀㬀 紀ഀ਀ † pWhd->Create(GetHWND(), NULL, UI_CLASSSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
††瀀圀栀搀ⴀ㸀䌀攀渀琀攀爀圀椀渀搀漀眀⠀⤀㬀ഀ਀ † pWhd->ShowModal();
††匀愀瘀攀䌀漀渀昀椀最⠀欀䌀漀渀昀椀最䘀椀氀攀倀愀琀栀⤀㬀ഀ਀ † LoadConfig(kConfigFilePath);
}

void ConnInfoUI::OnDelConnInfo(TNotifyUI& msg)
{
††椀昀 ⠀洀开瀀䰀椀猀琀唀䤀ⴀ㸀䜀攀琀䌀甀爀匀攀氀⠀⤀ 㰀 　⤀ഀ਀ † {
†† † UserMessageBox(GetHWND(), 20002, NULL, MB_ICONINFORMATION);
†† † return;
††紀ഀ਀ † int idx = m_pListUI->GetCurSel();
††洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀一愀洀攀䤀渀搀攀砀崀⸀攀爀愀猀攀⠀洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀一愀洀攀䤀渀搀攀砀崀⸀戀攀最椀渀⠀⤀⬀椀搀砀⤀㬀ഀ਀ † m_dicServerInfo[kServerIpIndex].erase(m_dicServerInfo[kServerIpIndex].begin()+idx);
††洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀倀漀爀琀䤀渀搀攀砀崀⸀攀爀愀猀攀⠀洀开搀椀挀匀攀爀瘀攀爀䤀渀昀漀嬀欀匀攀爀瘀攀爀倀漀爀琀䤀渀搀攀砀崀⸀戀攀最椀渀⠀⤀⬀椀搀砀⤀㬀ഀ਀ † m_dicServerInfo[kServerAuthIndex].erase(m_dicServerInfo[kServerAuthIndex].begin()+idx);
††匀愀瘀攀䌀漀渀昀椀最⠀欀䌀漀渀昀椀最䘀椀氀攀倀愀琀栀⤀㬀ഀ਀ † LoadConfig(kConfigFilePath);
}

void ConnInfoUI::OnAltConnInfo(TNotifyUI& msg)
{
††椀昀 ⠀洀开瀀䰀椀猀琀唀䤀ⴀ㸀䜀攀琀䌀甀爀匀攀氀⠀⤀ 㰀 　⤀ഀ਀ † {
†† † UserMessageBox(GetHWND(), 20002, NULL, MB_ICONINFORMATION);
†† † return;
††紀ഀ਀ † ConnInfoSubWhd* pWhd = new ConnInfoSubWhd(&m_dicServerInfo, ConnInfoSubWhd::Type_Alt,m_pListUI->GetCurSel());
††椀昀⠀ 瀀圀栀搀 㴀㴀 一唀䰀䰀 ⤀ 笀 爀攀琀甀爀渀㬀 紀ഀ਀ † pWhd->Create(GetHWND(), NULL, UI_CLASSSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
††瀀圀栀搀ⴀ㸀䌀攀渀琀攀爀圀椀渀搀漀眀⠀⤀㬀ഀ਀ † pWhd->ShowModal();
††匀愀瘀攀䌀漀渀昀椀最⠀欀䌀漀渀昀椀最䘀椀氀攀倀愀琀栀⤀㬀ഀ਀ † LoadConfig(kConfigFilePath);
}

void ConnInfoUI::OnRfhConnInfo(TNotifyUI& msg)
{
††䰀漀愀搀䌀漀渀昀椀最⠀欀䌀漀渀昀椀最䘀椀氀攀倀愀琀栀⤀㬀ഀ਀紀ഀ਀ഀ਀瘀漀椀搀 䌀漀渀渀䤀渀昀漀唀䤀㨀㨀䐀漀䌀漀渀渀攀挀琀⠀⤀ഀ਀笀ഀ਀ † m_isConecting = true;
††䐀圀伀刀䐀 搀眀吀栀爀攀愀搀䤀䐀 㴀 　㬀ഀ਀ † HANDLE hThread = CreateThread(NULL, 0, &ConnInfoUI::BackgroundWork, GetHWND(),  0, &dwThreadID);
}


DWORD WINAPI ConnInfoUI::BackgroundWork( LPVOID lpParameter )
{††ഀ਀ † HWND hwnd = (HWND) lpParameter;
††刀攀搀椀猀䌀氀椀攀渀琀☀ 漀戀樀 㴀 刀攀搀椀猀䌀氀椀攀渀琀㨀㨀䜀攀琀䤀渀猀琀愀渀挀攀⠀⤀㬀ഀ਀ † CDuiString* s = new CDuiString(obj.GetName());
††㨀㨀倀漀猀琀䴀攀猀猀愀最攀⠀栀眀渀搀Ⰰ 圀䴀开唀匀䔀刀开䌀伀一一䔀䌀吀䤀一䜀Ⰰ ⠀圀倀䄀刀䄀䴀⤀猀Ⰰ 一唀䰀䰀⤀㬀ഀ਀ † s = new CDuiString(obj.GetName());
††漀戀樀⸀䌀漀渀渀攀挀琀⠀⤀㬀ഀ਀ † 
††椀昀 ⠀漀戀樀⸀䤀猀䌀漀渀渀攀挀琀攀搀⠀⤀⤀ഀ਀ † {
†† † ::PostMessage(hwnd, WM_USER_CONNECTED, (WPARAM)s, NULL);
††紀ഀ਀ † else 
††笀ഀ਀ † ††㨀㨀倀漀猀琀䴀攀猀猀愀最攀⠀栀眀渀搀Ⰰ 圀䴀开唀匀䔀刀开唀一䌀伀一一䔀䌀吀Ⰰ ⠀圀倀䄀刀䄀䴀⤀猀Ⰰ 一唀䰀䰀⤀㬀ഀ਀ † }
††洀开椀猀䌀漀渀攀挀琀椀渀最 㴀 昀愀氀猀攀㬀ഀ਀ † return 0;
}
