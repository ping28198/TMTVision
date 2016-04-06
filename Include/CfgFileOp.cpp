
#include "stdafx.h"
#include "CfgFileOp.h"





bool CfgFileOp::IsSpace(char c)
{
	if (' ' == c || '\t' == c || '\r'== c||'\n'==c)
		return true;
	return false;
}

bool CfgFileOp::IsCommentChar(char c)
{
	switch(c) {
	case COMMENT_CHAR:
		return true;
	default:
		return false;
	}
}

void CfgFileOp::Trim(string & str)
{
	//if (str.empty()) return;
	//unsigned int i, start_pos, end_pos;
	//for (i = 0; i < str.size(); ++i) 
	//{
	//	if (!IsSpace(str[i]))
	//	{
	//		break;
	//	}
	//}
	//if (i == str.size()) 
	//{ // 全部是空白字符串
	//	str = "";
	//	return;
	//}

	//start_pos = i;

	//for (i = str.size() - 1; i >= 0; --i) 
	//{
	//	if (!IsSpace(str[i])) 
	//	{
	//		break;
	//	}
	//}
	//end_pos = i;

	//str = str.substr(start_pos, end_pos - start_pos + 1);
}

bool CfgFileOp::AnalyseLine(const string & line, string & key, string & value)
{
	if (line.empty())
		return false;
	int start_pos = 0, end_pos = line.size() - 1, pos;
	if ((pos = line.find(COMMENT_CHAR)) != -1) {
		if (0 == pos) {  // 行的第一个字符就是注释字符
			return false;
		}
		end_pos = pos - 1;
	}
	string new_line = line.substr(start_pos, start_pos + 1 - end_pos);  // 预处理，删除注释部分

	if ((pos = new_line.find('=')) == -1)
		return false;  // 没有=号

	key = new_line.substr(0, pos);
	value = new_line.substr(pos + 1, end_pos + 1- (pos + 1));

	Trim(key);
	if (key.empty()) {
		return false;
	}
	Trim(value);
	return true;
}


CfgFileOp::CfgFileOp()
{

}

CfgFileOp::CfgFileOp(char* filepath)
{
	mfilepath = filepath;
	infile.open(mfilepath);
}

CfgFileOp::~CfgFileOp()
{

	infile.close();
}

int CfgFileOp::ReadConfig(const char* item_str,char* value_str,int bufferlength)
{
	map<string,string>::iterator it;
	for (it=mpdata.begin();it!=mpdata.end();it++)
	{
		if (it->first.compare(item_str)==0)
		{
			strcpy_s(value_str,bufferlength,it->second.data());
			break;
		}
	}
	if (it==mpdata.end())
	{
		return NO_ITEM;
	}
	return FINISH;
}

int CfgFileOp::ReadConfig(const char* item_str,int &value_int)
{
	char tmp_str[256];
	int backcode = ReadConfig(item_str,tmp_str,sizeof(tmp_str));
	if (backcode==NO_ITEM) return ERR;
	value_int = atoi(tmp_str);
	return FINISH;
}

int CfgFileOp::ReadConfig(const char* item_str,double &value_db)
{
	char tmp_str[256];
	int backcode = ReadConfig(item_str,tmp_str,sizeof(tmp_str));
	if (backcode==NO_ITEM) return ERR;
	_CRT_DOUBLE ctr_double;
	_atodbl(&ctr_double,tmp_str);
	value_db = ctr_double.x;
	return FINISH;
}

int CfgFileOp::ReadConfig(const char* filename, map<string, string> &m)
{
	m.clear();
	ifstream infile(filename);
	if (!infile) {
		return FILE_OPEN_FAIL;
	}
	string line, key, value;
	while (getline(infile, line)) 
	{
		if (AnalyseLine(line, key, value)) 
		{
			m[key] = value;
		}
	}
	infile.close();
	return FINISH;
}

int CfgFileOp::ReadConfig(const char* filename, const char* item_str,char* value_str,int bufferlength)
{
	ifstream minfile(filename);
	if (!minfile) 
	{
		return FILE_OPEN_FAIL;
	}
	if(value_str==NULL) return SMALL_BUFFER;
	string line, key, value;
	bool okflag = false;
	while (getline(minfile, line)) 
	{
		if (AnalyseLine(line, key, value))
		{
			if (key.compare(item_str)==0)
			{
				strcpy_s(value_str,bufferlength,value.data());
				okflag = true;
				break;
			}
		}
	}
	minfile.close();
	if(okflag == false) return NO_ITEM;
	return FINISH;
}

int CfgFileOp::ReadConfig(map<string, string> &m)
{
	m = mpdata;
	return FINISH;
}

int CfgFileOp::WriteConfig(const char* item_str, const char* value_str)
{
	mpdata[item_str]=value_str;
	WriteConfig(mfilepath,&mpdata);
	return FINISH;
}

int CfgFileOp::WriteConfig(const char* item_str, const double value_db)
{
	char tmp[256];
	sprintf_s(tmp,sizeof(tmp),"%.5f",value_db);
	WriteConfig(item_str,tmp);
	return FINISH;
}
int CfgFileOp::WriteConfig(const char* item_str, const int value_int)
{
	char tmp[256];
	sprintf_s(tmp,sizeof(tmp),"%d",value_int);
	WriteConfig(item_str,tmp);
	return FINISH;
}

int CfgFileOp::WriteConfig(const char* filename, const char* item_str, const int value_int)
{
	char tmpstr[256];
	sprintf_s(tmpstr,sizeof(tmpstr),"%d",value_int);
	WriteConfig(filename,item_str,tmpstr);
	return FINISH;
}

int CfgFileOp::WriteConfig(const char* filename, map<string, string> *m)
{
	ofstream motfile(filename);
	if (!motfile)
	{
		return FILE_OPEN_FAIL;
	}
	string tmpstr;
	tmpstr.clear();
	map<string,string>::iterator it;
	for (it=m->begin();it!= m->end();it++)
	{
		tmpstr.append(it->first);
		tmpstr.append("=");
		tmpstr.append(it->second);
		tmpstr.append("\n");
	}
	motfile<<tmpstr.data();
	tmpstr.clear();
	motfile.close();
	return FINISH;
}

int CfgFileOp::WriteConfig(map<string,string> *m)
{

	return FINISH;
}

int CfgFileOp::WriteConfig(const char* filename, const char* item_str, const char* value_str)
{
	map<string,string> maptmp;
	int backcode = ReadConfig(filename,maptmp);
	if (backcode!=FINISH)
	{
		return ERR;
	}
	maptmp[item_str] = value_str;
	backcode = WriteConfig(filename,&maptmp);
	if (backcode!=FINISH)
	{
		return ERR;
	}
	return FINISH;
}



int CfgFileOp::openfile(char* filepath)
{
	mfilepath = filepath;
	mpdata.clear();
	int backcode = this->ReadConfig(mfilepath,mpdata);
	if(backcode != FINISH) return ERR;
	return FINISH;
}

int CfgFileOp::closefile()
{
	if (infile)
	{
		infile.close();
	}
	if (otfile)
	{
		otfile.close();
	}
	return FINISH;
}



