//---------------------------------------------------------------------------
#include "ArWatch.h"
//---------------------------------------------------------------------------

ArWatch::ArWatch()
{
    QueryPerformanceFrequency(&m_liFrequency);
    m_dCycle = 1000 * 1.0 / m_liFrequency.QuadPart;
    QueryPerformanceCounter(&m_liStart);
    m_pTime = NULL;
}

void ArWatch::SetData(double* pTime)
{

    m_pTime = pTime;

}
//---------------------------------------------------------------------------

ArWatch::~ArWatch()
{
    QueryPerformanceCounter(&m_liEnd);

    double dTime =  m_dCycle * (m_liEnd.QuadPart - m_liStart.QuadPart); 
    wchar_t strInfo[50];
    wsprintf(strInfo, L"�㷨����ʱ�� %d����!", (int)dTime);
    if(NULL ==  m_pTime)
    {
		::MessageBox(NULL,strInfo, L"��ʾ", MB_OK);
    }else
    {
        *m_pTime = dTime;
    }
}

