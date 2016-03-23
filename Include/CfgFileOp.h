#ifndef _CFGFILEOP_H_
#define _CFGFILEOP_H_
#endif

#include <string>
#include <map>

#define COMMENT_CHAR '#'
#define FILE_OPEN_FAIL -1
#define NO_ITEM -2
#define SMALL_BUFFER -3
#define FINISH 1

#include "Err_seq.h"
#include <fstream>
using namespace std;

class CfgFileOp
{
public:
	CfgFileOp();
	CfgFileOp(char* filepath);
	virtual ~CfgFileOp();
	int openfile(char* filepath);
	int closefile();
	int ReadConfig(char* item_str,char* value_str,int bufferlength);
	int ReadConfig(char* item_str,int &value_int);
	int ReadConfig(char* item_str,double &value_db);
	int ReadConfig(char* filename, map<string, string> &m);
	int ReadConfig(char* filename,char* item_str,char* value_str,int bufferlength);
	int ReadConfig(map<string, string> &m);
	int WriteConfig(char* item_str,char* value_str);
	int WriteConfig(char* item_str,int value_int);
	int WriteConfig(char* item_str,double value_db);
	int WriteConfig(char* filename,char* item_str,int value_int);
	int WriteConfig(char* filename,char* item_str,char* value_str);
	int WriteConfig(char* filename, map<string, string> * m);
	int WriteConfig(map<string,string> *m);


private:
	char *mfilepath;
	ifstream infile;
	ofstream otfile;
	map<string,string> mpdata;
	bool IsSpace(char c);
	bool IsCommentChar(char c);
	void Trim(string & str);
	bool AnalyseLine(const string & line, string & key, string & value);






};