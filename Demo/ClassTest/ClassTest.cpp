// ClassTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "CommonInclude.h"
#include "DirWatcher.h"






int main()
{
	DirWatchServer DirWatchSvr;
	LONGWSTR myDir=L"D:\\a";
	DirWatchSvr.RegPath(myDir);
	DirWatchSvr.Create();
	DirWatchSvr.Resume();
	while (true)
	{
		cout<<DirWatchSvr.m_fileNameQueue.GetLength()<<endl;
		if (DirWatchSvr.m_fileNameQueue.GetTail()==NULL)
		{
			Sleep(1000);
			continue;
		}
		cout << DirWatchSvr.m_fileNameQueue.GetTail()->m_fileName<<endl;
		cout << DirWatchSvr.m_fileNameQueue.GetTail()->m_fileAction<<endl;

		cout << DirWatchSvr.m_fileNameQueue.GetData(DirWatchSvr.m_fileNameQueue.GetLength()-2)->m_fileName << endl;
		cout << DirWatchSvr.m_fileNameQueue.GetData(DirWatchSvr.m_fileNameQueue.GetLength() - 2)->m_fileAction << endl;
		Sleep(1000);
	}
	


    return 0;
}

