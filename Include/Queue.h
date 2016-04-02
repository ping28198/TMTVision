///<proj_info>
//==============================================================================
// ��Ŀ�� �����ܼ��
// �ļ��� ��Queue.h
// ��  �� ������
// ��  ; ����ѭ������ģ����
// ��  Ȩ ����������
//==============================================================================
///</proj_info>

///<ver_info>
// �汾��¼	
//==============================================================================
//�汾��  ������Ա      ʱ��      ����
//1.0     ����        2013.2.20   ����
//2.0     ����        2016.4.2   ���Ͼɰ汾
//==============================================================================
///</ver_info>

///<class_info>
//==============================================================================
//��������:��ѭ������ģ����
//         ���б�������(��ָ��),��ӵ�������ʱ���ø�ֵ����
//         ֧���޸�ֵ�������������
//         ���ڲ��������ӵ���,���з���д��ͷ�ļ���
#pragma once
template <typename T>
class Queue
{
public:
	enum {DEFAULTNUM = 32};
	//��������
	T* p_DataList;
	int m_DataNum;
	//����ͷβ
	int p_Head;
	int p_Tail;
	//���캯���������в���ʼ��Ϊ��,�ݲ�֧�ֿ���
	Queue()//int dataNum=DEFAULTNUM)
	{
		p_Head=0;
		p_Tail=0;
		p_DataList=0;
		m_DataNum=0;
	}
	~Queue(void)
	{
		if (p_DataList!=0)
		{
			delete[] p_DataList;
			p_DataList=0;
		}
	}
	Initial(int dataNum = DEFAULTNUM)
	{
		if (p_DataList != 0)
		{
			delete[] p_DataList;
			p_DataList = 0;
		}
		p_Head = 0;
		p_Tail = 0;
		if (dataNum<1) dataNum = DEFAULTNUM;
		p_DataList = new T[dataNum];
		m_DataNum = dataNum;
	}
	Unitial()
	{
		if (p_DataList != 0)
		{
			delete[] p_DataList;
			p_DataList = 0;
		}
		p_Head = 0;
		p_Tail = 0;
		m_DataNum = 0;
	}
	//����
	int SetSize(int dataNum)
	{
		if (p_DataList != 0)
		{
			delete[] p_DataList;
			p_DataList = 0;
		}
		p_Head = 0;
		p_Tail = 0;
		if (dataNum<1) dataNum = DEFAULTNUM;
		p_DataList = new T[dataNum];
		m_DataNum = dataNum;
		return m_DataNum;
	}
	int GetSize()
	{
		return m_DataNum;
	}
	int GetLength()
	{
		int num=p_Tail-p_Head;
		num=num<0?num+m_DataNum:num;
		return num;
		//return m_DataNum;
	}
	void Clear()
	{
		p_Head=0;
		p_Tail=0;
		//m_DataNum=0;
	}
	bool IsEmpty()
	{
		return p_Head==p_Tail;
	}
	bool IsFull()
	{
		return p_Head-p_Tail==1||(p_Head==0&&p_Tail==m_DataNum-1);
	}

	//ɾ��ͷ����
	bool DelHead()
	{
		if (IsEmpty()) return false;
		p_Head++;
		if (p_Head>=m_DataNum)
		{
			p_Head-=m_DataNum;
		}
		return true;
	}
	//����ͷ����ǰ�������β����
	bool AddTail(T data)
	{
		if (IsFull()) return false;
		memcpy((p_DataList+p_Tail),&data,sizeof(T));
		//*(p_DataList+p_Tail)=data;
		p_Tail++;
		if (p_Tail>=m_DataNum)
		{
			p_Tail-=m_DataNum;
		}
		return true;
	}
	//ǿ�����β����
	bool ForcTail(T data)
	{
		if (IsFull()) DelHead();
		memcpy((p_DataList+p_Tail),&data,sizeof(T));
		//*(p_DataList+p_Tail)=data;
		p_Tail++;
		if (p_Tail>=m_DataNum)
		{
			p_Tail-=m_DataNum;
		}
		return true;
	}
	//���β����
	bool GetTail(T& data)
	{
		if (IsEmpty()) return false;
		if (p_Tail>0)
		{
			memcpy(&data,(p_DataList+p_Tail-1),sizeof(T));
			//data=*(p_DataList+p_Tail-1);
		}
		else memcpy(&data,(p_DataList+m_DataNum-1),sizeof(T));
//data=*(p_DataList+m_DataNum-1);
		return true;
	}
	//���β����
	T* GetTail()
	{
		if (IsEmpty()) return 0;
		if (p_Tail>0)
		{
			//memcpy(&data,(p_DataList+p_Tail-1),sizeof(T));
			//data=*(p_DataList+p_Tail-1);
			return (p_DataList+p_Tail-1);
		}
		else return (p_DataList+m_DataNum-1);
		//memcpy(&data,(p_DataList+m_DataNum-1),sizeof(T));
		//data=*(p_DataList+m_DataNum-1);
	}
	//���ͷ����
	bool GetHead(T& data)
	{
		if (IsEmpty()) return false;
		memcpy(&data,(p_DataList+p_Head),sizeof(T));
		//data=*(p_DataList+p_Head);
		return true;
	}
	//���ͷ����
	T* GetHead()
	{
		if (IsEmpty()) return 0;
		return (p_DataList+p_Head);
		//memcpy(&data,(p_DataList+p_Head),sizeof(T));
		//data=*(p_DataList+p_Head);
		//return true;
	}
	//�������
	bool GetData(T& data,int index)
	{
		if (IsEmpty()) return false;
		if (index>=GetLength()|| index<0) return false;
		int pRead=p_Head+index-1;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		memcpy(&data,(p_DataList+pRead),sizeof(T));
		//data=*(p_DataList+pRead);
		return true;
	}
	//�������
	T* GetData(int index)
	{
		if (IsEmpty()) return 0;
		if (index>=GetLength()|| index<0) return 0;
		int pRead=p_Head+index-1;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		return (p_DataList+pRead);
		//memcpy(&data,(p_DataList+pRead),sizeof(T));
		//data=*(p_DataList+pRead);
		//return true;
	}
	//�������
	bool GetLast(T& data,int index)
	{
		if (IsEmpty()) return false;
		if (index>=GetLength()|| index<0) return false;
		int pRead=p_Tail-index-1;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		memcpy(&data,(p_DataList+pRead),sizeof(T));
		data=*(p_DataList+pRead);
		return true;
	}
	//�������
	T* GetLast(int index)
	{
		if (IsEmpty()) return 0;
		if (index>=GetLength()|| index<0) return 0;
		int pRead=p_Tail-index-1;
		if (pRead>=m_DataNum) pRead-=m_DataNum;
		if (pRead<0) pRead+=m_DataNum;
		return (p_DataList+pRead);
		//memcpy(&data,(p_DataList+pRead),sizeof(T));
		//data=*(p_DataList+pRead);
		//return true;
	}
};
//==============================================================================
///</class_info>