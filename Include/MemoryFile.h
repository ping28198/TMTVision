#pragma once



class CMemoryFile
{
public:
	CMemoryFile();
	bool OpenFile(wchar_t *pFileName);










protected:
private:
	FILE* pFile;
	wchar_t  strFilePath[256];
	~CMemoryFile();
};
