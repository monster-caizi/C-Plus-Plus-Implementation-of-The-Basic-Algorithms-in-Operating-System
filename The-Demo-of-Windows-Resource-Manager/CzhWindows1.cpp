// CzhWindows1.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

//STEP 1 申明窗口类
WNDCLASSEX wcx;

HINSTANCE hInstance1;//全局句柄
HWND  hWndTree;//treeview句柄
HWND  hWndList;//listview句柄
HWND hWnd;//父窗口句柄
HIMAGELIST m_TreeImageList;//treeview图标表
HIMAGELIST m_ListImageList;//listview图标表
CString   LVrood,LVrood1;//公用路径
map<CString,CString>  Map;//本地磁盘信息暂存

//STEP 2 设计窗口类
void CzhWindow(HINSTANCE hinstance);



//STEP 3 定义窗口类中的MainWndProc
LRESULT CALLBACK MainWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK About(HWND,UINT,WPARAM,LPARAM);//说明对话框
void TreeInitialization ();//初始化treeview
void ListInitialization();//初始化listview
void ExpandTreeItem(CString str, HTREEITEM rootItem);//扩展treeview节点
void GetChildItem(LPNMHDR lpnmh);//处理treeview单击事件
void setNewTreeItem(HTREEITEM rootItem,CString str,HWND hwndfrom);//处理treeview 单击事件子节点
void setNewListItemFromTree(CString str);//处理treeview单击事件listview显示
void RefreshList();//窗口改变的listview显示
void setNewListItemFromList(LPNMHDR lpnmh);//listview双击事件处理
void setMemu(HWND hWnd, WPARAM wParam);//为listview添加右键菜单


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	hInstance1=hInstance;
	CzhWindow(hInstance1);//STEP 2 设计窗口类


	//STEP 4: 注册窗口类
	if(!::RegisterClassEx(&wcx))
		return false;
	//STEP 5:创建窗口
	//HWND hWnd;
	hWnd=::CreateWindow("CzhWindows",//窗口类名，注意一致性
						"资源管理器",//标题
						WS_OVERLAPPEDWINDOW,//窗口样式
						CW_USEDEFAULT,//水平默认位置、
						CW_USEDEFAULT,//垂直默认位置
						CW_USEDEFAULT,//宽度默认
						CW_USEDEFAULT,//高度默认
						(HWND)NULL,//无父窗口
						(HMENU)::LoadMenu(hInstance1,MAKEINTRESOURCE(IDR_MENU_MAIN)),//无菜单
						//(HMENU)NULL,//无菜单
						hInstance,//应用程序句柄
						(LPVOID)NULL//窗口创建时无数据
						);
	
	if(!hWnd)
	  return false;
	//STEP 6:显示窗口
	::ShowWindow(hWnd,nCmdShow);
	::UpdateWindow(hWnd);
	
	//::MessageBox(hWnd,TEXT("模式对话框"),TEXT("捉住窗帘"),MB_OK);
	//STEP 7:消息循环

	MSG msg;
	BOOL bGetMessage;//是否成功获取消息
	while((bGetMessage=::GetMessage(&msg,(HWND)NULL,0,0))!=0 && bGetMessage !=-1)//从消息队列中取消息
	{
		::TranslateMessage(&msg);//转换消息
		::DispatchMessage(&msg);//派发消息
	}
		


	return msg.wParam;
}

//STEP 2 设计窗口类，参数hinstance为本应用程序本次运行句柄

void CzhWindow(HINSTANCE hinstance)
{
	//填充wcx的数据结构
	wcx.cbSize=sizeof(wcx);//结构体的大小
	wcx.style=CS_HREDRAW|CS_VREDRAW;//样式，大小改变时重绘界面
	wcx.lpfnWndProc=MainWndProc;//窗口消息处理函数
	wcx.cbClsExtra=0;//无附加窗口类内存
	wcx.cbWndExtra=0;//无附加窗口内存
	wcx.hInstance=hinstance;//所属应用程序实例句柄
	wcx.hIcon=::LoadIcon(NULL,IDI_APPLICATION);//图标默认
	wcx.hCursor=::LoadCursor(NULL,IDC_ARROW);//光标默认
	wcx.hbrBackground=(HBRUSH)::GetStockObject(WHITE_BRUSH);//背景白色
	wcx.lpszMenuName=NULL;//不使用菜单
	wcx.lpszClassName="CzhWindows";//窗口类名
	wcx.hIconSm=(HICON)::LoadImage(hInstance1,
		MAKEINTRESOURCE(5),
		IMAGE_ICON,
		::GetSystemMetrics(SM_CXSMICON),
		::GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);//小图标


	return ;
}

//STEP 3 定义窗口类中的MainWndProc

LRESULT CALLBACK MainWndProc(HWND hWnd,//窗口句柄
							 UINT uMsg,//消息表示符
							 WPARAM wParam,//消息的第一个参数
							 LPARAM lParam)//消息的第二个参数
{//窗口消息类处理函数：
	//TVINSERTSTRUCT tvInst;
	//HTREEITEM hNode1;
	//HTREEITEM hNode2,hNode3;
	//HTREEITEM hChild1,hChild2;
	HDWP hdwp;
	RECT rect;
	CFileFind finder;
	switch(uMsg)
	{
	case WM_CREATE://创建treeview与listview，用于显示资源管理器数据
		GetClientRect(hWnd, &rect);
		hWndTree = CreateWindow(TEXT("SysTreeView32"), TEXT("Simple"),//创建treeview
								WS_BORDER | WS_CHILD | WS_VISIBLE |
								TVS_HASLINES |TVS_HASBUTTONS|TVS_LINESATROOT 
								| TVS_SINGLEEXPAND ,
								0, 0, 
								rect.right/4, rect.bottom,
								hWnd, NULL, hInstance1, 0);
		hWndList = CreateWindow("syslistview32", "", //创建listview
                                     WS_VISIBLE|WS_CHILD|WS_BORDER|
                                    LVS_ICON | LVS_SHOWSELALWAYS, 
                                    (rect.right/4)+5, 0,
                                    (rect.right/4)*3-3,
                                    rect.bottom, 
                                    hWnd, NULL, hInstance1, NULL);																																		
		TreeInitialization ();//调用初始化函数
		ListInitialization ();
		break;
	case WM_DESTROY://窗口销毁，单击右上角的关闭按钮会触发此消息
		::ExitProcess(0);//销毁所有进程与线程
		::ExitThread(0);
		return 0;
		break;
	case WM_COMMAND://处理应用程序菜单
		switch(LOWORD(wParam))
		{
			//在“帮助”菜单中选择“关于”
		case ID_HELP_ABOUT:
			{
				DialogBox(hInstance1,(LPCSTR)IDD_DIALOG_ABOUT,hWnd,(DLGPROC)About);
				return 0;
			}
		case ID_OPENFILE://处理右键菜单中选择“打开文件”
			{
				
				BOOL beWorking = finder.FindFile(LVrood1+"\\*");
				if(beWorking)//判断是否为目录，是则清除listview继续打开文件夹显示，
				{   
					ListView_DeleteAllItems(hWndList);
					setNewListItemFromTree(LVrood1);
				}
				else//否则直接打开文件
					::ShellExecute(hWnd,0,LVrood1,0,0,SW_SHOWNORMAL);
			}

		default:
			return DefWindowProc(hWnd,uMsg,wParam,lParam);
		}
		break;
	case WM_SIZE://处理窗口改变时控件显示问题
		hdwp = BeginDeferWindowPos(2);
		GetClientRect(hWnd, &rect);
		DeferWindowPos(hdwp, hWndTree, NULL, 0, 0,//修改treeview大小
			rect.right/4, rect.bottom, SWP_NOZORDER);
		DeferWindowPos(hdwp, hWndList, NULL, rect.right/4+5, 0,//修改listview大小
			(rect.right/4)*3-3, rect.bottom, SWP_NOZORDER);
		EndDeferWindowPos(hdwp);
		RefreshList();//刷新listview显示布局
		break;
	case WM_NOTIFY:
	{
		LPNMHDR lpnmh = (LPNMHDR)lParam;
		if(NM_CLICK == lpnmh->code)//处理单击事件
		{
			if(lpnmh->hwndFrom==hWndTree)//处理treeview单击事件
				GetChildItem(lpnmh);
		}
		if(NM_DBLCLK == lpnmh->code)//处理双击事件
		{
			if(lpnmh->hwndFrom==hWndList)//处理listview双击事件
				
				setNewListItemFromList(lpnmh);
		}
	}
	break;
	case WM_CONTEXTMENU://处理listview右键菜单显示     
    {    
		setMemu(hWnd,wParam);
    }     
    break;
	default://有许多消息未处理，调用默认窗口消息处理函数处理
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
		return 0;
}


//定义About子窗口的消息处理函数
LRESULT CALLBACK About(HWND  hDlg,
					   UINT  uMsg,
					   WPARAM  wParam,
					   LPARAM  lParawm)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND://单击OK按钮
		if(LOWORD(wParam)==IDOK||LOWORD(wParam)==IDCANCEL)
		{
			::EndDialog(hDlg,LOWORD(wParam));
			return true;
		}
		break;



	}
	return false;

}

void   TreeInitialization ()
{
	TVINSERTSTRUCT tvInst;
	//Root...
	HTREEITEM RootNode;
	SHFILEINFO fileInfo1;
	//获取系统icon列表，并设置给treeview
	m_TreeImageList = (HIMAGELIST) SHGetFileInfo("C:\\",
			0,
			&fileInfo1,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//设置小icon
	TreeView_SetImageList(hWndTree,m_TreeImageList,TVSIL_NORMAL); 
	//设置treeview节点信息，并设置根节点“我的电脑”
	tvInst.item.mask =  TVIF_IMAGE |TVIF_TEXT | TVIF_SELECTEDIMAGE;
	tvInst.item.iImage=0;
	tvInst.item.iSelectedImage=0;
	tvInst.hInsertAfter = TVI_ROOT;
	tvInst.hParent = NULL;
	tvInst.item.pszText = TEXT("我的电脑");
	RootNode= (HTREEITEM)SendMessage(hWndTree, TVM_INSERTITEM, 0, (LPARAM)&tvInst);
	//获取本地磁盘信息
	char buff[1024] = {0};
	GetLogicalDriveStrings(1024, buff);
	char* pTemp = buff;
	while (*(pTemp) != 0)
	{
		CString str,str1="";

		switch(::GetDriveType(pTemp))//判断磁盘类型，添加显示提示
		{
		case DRIVE_FIXED:
			str = "本地磁盘";
			break;
		case DRIVE_REMOVABLE:
			str = "可移动磁盘";
			break;
		case DRIVE_CDROM:
			str = "光驱";
			break;
		default:
			str = "未知磁盘";
			break;
		}
		str = str + "(" + pTemp + ")";
		Map[str]=pTemp;//利用map储存路径信息，方便扩展节点时的路径寻找
		char *s=(LPSTR)(LPCSTR)str;
		//设置新节点
		SHGetFileInfo(pTemp,
			0,
			&fileInfo1,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//选择小icon

		tvInst.hInsertAfter=TVI_LAST;
		tvInst.hParent = RootNode;
		tvInst.item.pszText = s;
		tvInst.item.iImage =fileInfo1.iIcon;
		tvInst.item.iSelectedImage= fileInfo1.iIcon;
		HTREEITEM subItem =(HTREEITEM)SendMessage(hWndTree, TVM_INSERTITEM, 0, (LPARAM)&tvInst);
		//设置磁盘子节点信息，以提供treeview上“+”提示其中存在文件夹
		ExpandTreeItem(pTemp, subItem);
		//遍历所有获得的磁盘信息
		char* newBuff = new char[6];
		strcpy(newBuff, pTemp);
		pTemp += 4;
		delete newBuff;
	}
		return ;
}

void   ListInitialization ()
{
	LVITEM LVitem;//设置listview项信息
	LVitem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	LVitem.iItem=-1;
	LVitem.lParam=0;
	LVitem.state=0;
	LVitem.stateMask=0;
	LVitem.iSubItem=0;
	LVrood="";//设置当前listview显示文件夹路径为空
	//获取系统icon列表，并设置给treeview
	SHFILEINFO fileInfo2;
	m_ListImageList = (HIMAGELIST) SHGetFileInfo("C:\\",
			0,
			&fileInfo2,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_LARGEICON);//设置大icon
	ListView_SetImageList(hWndList,m_ListImageList,TVSIL_NORMAL); 
	//删除listview所有显示
	ListView_DeleteAllItems(hWndList);
	//获取本地磁盘信息
	char buff[1024] = {0};
	GetLogicalDriveStrings(1024, buff);
	char* pTemp = buff;
	while (*(pTemp) != 0)
	{
		CString str,str1="";

		switch(::GetDriveType(pTemp))//判断磁盘类型，添加显示提示
		{
		case DRIVE_FIXED:
			str = "本地磁盘";
			break;
		case DRIVE_REMOVABLE:
			str = "可移动磁盘";
			break;
		case DRIVE_CDROM:
			str = "光驱";
			break;
		default:
			str = "未知磁盘";
			break;
		}
		str = str + "(" + pTemp + ")";
		char *s=(LPSTR)(LPCSTR)str;
		//设置显示项
		SHGetFileInfo(pTemp,
			0,
			&fileInfo2,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_LARGEICON);//设置大icon
		LVitem.iImage=fileInfo2.iIcon;
		LVitem.pszText=s;
		LVitem.iItem++;
		//插入新显示项
		ListView_InsertItem(hWndList,&LVitem);
		//遍历所有获得的磁盘信息
		char* newBuff = new char[6];
		strcpy(newBuff, pTemp);
		pTemp += 4;
		delete newBuff;
	}
		return ;
}

void ExpandTreeItem(CString str, HTREEITEM rootItem)
{
	TVINSERTSTRUCT tvInst;
	tvInst.item.mask =  TVIF_IMAGE |TVIF_TEXT | TVIF_SELECTEDIMAGE;//设置节点显示内容属性
    CString tempStr = str;
	str += "\\*";
	CFileFind finder;
	//获取输入路径的文件信息
	BOOL beWorking = finder.FindFile(str);
	SHFILEINFO fileInfo;
	
	while (beWorking)//循环遍历获得的文件信息，只显示文件夹信息
	{
		beWorking = finder.FindNextFile();//顺序获取文件
		if (finder.IsDots())//去除. ..目录
		{
			continue;
		}
		CString fileName = finder.GetFileName();
		if(!finder.IsDirectory())//只留下目录，并设置新节点
		{
			continue;
 		}	
		HTREEITEM subItem ;
		CString tempStr1 = finder.GetFilePath();
		char* newBuf = new char[1024];
		//设置节点信息
		strcpy(newBuf, tempStr1.GetBuffer(0));
				SHGetFileInfo(tempStr1,
			0,
			&fileInfo,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//设置小icon
		tvInst.hInsertAfter=TVI_LAST;
		tvInst.hParent = rootItem;
		tvInst.item.iImage=fileInfo.iIcon;
		tvInst.item.iSelectedImage=fileInfo.iIcon;
		tvInst.item.pszText =(LPSTR)(LPCSTR)fileName;
		//添加新节点
		subItem =(HTREEITEM)SendMessage(hWndTree, TVM_INSERTITEM, 0, (LPARAM)&tvInst);
		
	}
}

void GetChildItem(LPNMHDR lpnmh)
{
	CString  name,rootroad="";
	//获取点击事件坐标
	DWORD dwPos = GetMessagePos();
	POINT pt;
	pt.x = LOWORD(dwPos);
	pt.y = HIWORD(dwPos);
	//转化为客户区坐标
	ScreenToClient(lpnmh->hwndFrom, &pt);
	//获取点击事件对于treeview的iItem句柄
	TVHITTESTINFO ht = {0};
	ht.pt = pt;
	ht.flags = TVHT_ONITEM;
	HTREEITEM hItem1 = TreeView_HitTest(lpnmh->hwndFrom, &ht);
	HTREEITEM hItem2,hItem=hItem1;	
	//设置节点信息
	TVITEM ti = {0};
	ti.mask = TVIF_HANDLE | TVIF_TEXT;//设置获取节点内容
	TCHAR buf[1024] = {0};
	ti.cchTextMax = 1024;
	ti.pszText = buf;
	ti.hItem = hItem1;
	//tvInst.item=ti;
	TreeView_GetItem(lpnmh->hwndFrom, &ti);//初始化name，获取节点内容
	name=ti.pszText;
	//获取父节点，判断点击事件的节点是否为根节点，是则无需更新treeview
	hItem2=TreeView_GetParent(lpnmh->hwndFrom,hItem1);//获取父节点
	if( hItem2!=0)
	{
		if(Map.find(name)==Map.end())//判断是否为磁盘节点
		{
			rootroad=name;//循环取得节点的文件路径
			while(1)
			{
				ti.hItem = hItem2;
				TreeView_GetItem(lpnmh->hwndFrom, &ti);//获取节点text
				name=ti.pszText;
				if(Map.find(name)!=Map.end())//磁盘节点则退出循环
					break;
				//组成路径
				rootroad="\\"+rootroad;
				rootroad=ti.pszText+rootroad;
				hItem1=hItem2;
				hItem2=TreeView_GetParent(lpnmh->hwndFrom,hItem1);//循环取得父节点
			}
		}
		rootroad=Map[name]+rootroad;//组成整条路径
		setNewTreeItem(hItem,rootroad,lpnmh->hwndFrom);//调用设置设置treeview新节点函数
		setNewListItemFromTree(rootroad);//调用设置listview显示项函数
	}
	else//为根节点则重新刷新listview为初始状态
		ListInitialization ();
}

void setNewTreeItem(HTREEITEM rootItem,CString str,HWND  hwndfrom)
{
	HTREEITEM hItem1,hItem2;
	CString  str1;
	//设置新节点信息
	TVITEM ti = {0};
	str;
	ti.mask = TVIF_HANDLE | TVIF_TEXT;
	TCHAR buf[1024] = {0};
	ti.cchTextMax = 1024;
	ti.pszText = buf;
	//获取传入节点的子节点，更新其节点信息，以增加treeview中传入节点的“+”号信息，提示显示功能
	hItem1=TreeView_GetChild(hwndfrom,rootItem);
	while(hItem1!=NULL)//判断第一次进入时是否为无子文件夹节点，是则无需继续更新其子节点“+”号信息
					   //随后循环判断是否到传入路径下最后一个子文件夹节点
	{
		//判断传入节点是否已经设置子节点，是则无需操作
		hItem2=TreeView_GetChild(hwndfrom,hItem1);
		if(hItem2!=NULL)
			return ;
		ti.hItem = hItem1;
		//组成完整路径，传入ExpandTreeItem用于更新节点信息
		TreeView_GetItem(hwndfrom, &ti);
		str1=str+"\\"+ti.pszText;
		LVrood=str1;//更新listview路径信息
		ExpandTreeItem(str1, hItem1);
		hItem1=TreeView_GetNextVisible(hwndfrom,hItem1);//获取下一个子节点
	}
	return ;
}

void setNewListItemFromTree(CString str)
{
	ListView_DeleteAllItems(hWndList);//删除listview原本显示项
	// 设置新的显示项
	LVITEM LVitem;       
	LVitem.mask=LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM;
	LVitem.iItem=-1;
	LVitem.lParam=0;
	LVitem.state=0;
	LVitem.stateMask=0;
	LVitem.iSubItem=0;
	LVrood=str;
	//设置路径
    CString tempStr = str;
	str += "\\*";
	//获取输入路径的文件信息
	CFileFind finder;
	BOOL beWorking = finder.FindFile(str);
	SHFILEINFO fileInfo2;
	
	while (beWorking)//循环遍历获得的文件信息，显示全部文件
	{
		beWorking = finder.FindNextFile();//顺序获取文件
		CString fileName = finder.GetFileName();
		if (finder.IsDots())//去除. ..目录
		{
			continue;
		}
		CString tempStr1 = finder.GetFilePath();
		//设置显示项
		char *s=(LPSTR)(LPCSTR)fileName;
		char* newBuf = new char[1024];
		strcpy(newBuf, tempStr1.GetBuffer(0));
				SHGetFileInfo(tempStr1,
			0,
			&fileInfo2,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_LARGEICON);//设置为大icon
		LVitem.iImage=fileInfo2.iIcon;
		LVitem.pszText=s;
		LVitem.iItem++;
		ListView_InsertItem(hWndList,&LVitem);//插入显示项
	}
	return ;
}

void setNewListItemFromList(LPNMHDR lpnmh)
{
	CString root;
	CFileFind finder;
	TCHAR buf[1024] = {0};
	//设置被点击项信息
	LVITEM LVitem;// 项
	LVitem.mask=LVIF_TEXT;
	LVitem.cchTextMax=1024;
	LVitem.pszText=buf;
	NM_LISTVIEW* pNMTreeView = (NM_LISTVIEW*)lpnmh;
	LVitem.iItem=pNMTreeView->iItem;
	//获取被点击项文本
	ListView_GetItemText(lpnmh->hwndFrom,LVitem.iItem,0,buf,LVitem.cchTextMax);
	//组成新路径
	if(Map.find(LVitem.pszText)==Map.end())
		root=LVrood+"\\"+LVitem.pszText;
	else
		root=LVrood+Map[LVitem.pszText];
	//判断是否为文件夹
	BOOL beWorking = finder.FindFile(root+"\\*");
	if(beWorking)//是则删除listview中IItem，并显示文件夹中内容
	{   
		ListView_DeleteAllItems(hWndList);
		setNewListItemFromTree(root);
	}
	else//否则直接运行程序
		::ShellExecute(hWnd,0,root,0,0,SW_SHOWNORMAL);
	return ;
}

void RefreshList()
{
	if(LVrood=="")//判断listview中文件路径是否为根节点
		ListInitialization ();//是则重新初始化即可
	else
		setNewListItemFromTree(LVrood);//否则根据当前路径，刷新listview显示布局
	return ;
}

void setMemu(HWND hWnd, WPARAM wParam)
{
	
	HMENU hpop;
	DWORD dwPos = GetMessagePos();
	POINT pt1,pt2;
	pt1.x = LOWORD(dwPos);
	pt1.y = HIWORD(dwPos);
	RECT rect; 
	//获取客户区域大小     
	GetClientRect(hWndList, &rect); 
	//把屏幕坐标转为客户区坐标     
	ScreenToClient(hWndList, &pt1);     
    //判断点是否位于客户区域内     
    if(!PtInRect(&rect, pt1))    
		return ;
    //加载菜单资源
    HMENU hroot1 = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),MAKEINTRESOURCE(IDR_MENU1));
	HMENU hroot2 = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),MAKEINTRESOURCE(IDR_MENU2));
    if(hroot1 && hroot2)     
    {     
           
       // 获取鼠标右击是的坐标          
		pt2.x = LOWORD(dwPos);
		pt2.y = HIWORD(dwPos);
        //显示快捷菜单     
		LVHITTESTINFO lvpi;
		lvpi.pt=pt1;
		lvpi.flags=LVHT_ABOVE;
		int item=ListView_HitTest(hWndList,&lvpi);
		// 获取第一个弹出菜单
		if(item==-1)
			hpop = GetSubMenu(hroot2,0);   
		else 
		{
			hpop = GetSubMenu(hroot1,0);
			//设置右键菜单处理文件时的路径问题
			TCHAR buf[1024] = {0};
			LVITEM LVitem;// 设置当前项属性
			LVitem.mask=LVIF_TEXT;
			LVitem.cchTextMax=1024;
			LVitem.pszText=buf;
			LVitem.iItem=item;
			ListView_GetItemText(hWndList,LVitem.iItem,0,buf,LVitem.cchTextMax);//获取当前项文本
			//组成新路径
			if(Map.find(LVitem.pszText)==Map.end())
				LVrood1=LVrood+"\\"+LVitem.pszText;
			else
				LVrood1=LVrood+Map[LVitem.pszText];
	
			
		}
		//设置显示位置
        TrackPopupMenu(hpop,     
        TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,     
		pt2.x,     
        pt2.y,     
        0,
        (HWND)wParam,     
        NULL);     
        // 用完后要销毁菜单资源     
        DestroyMenu(hroot1); 
		DestroyMenu(hroot2);
     }     
	return ;
}