#include "FileLoader.h"
#include <io.h>
#include <direct.h>
#include <afxinet.h>
#include <cstringt.h>
#include <afxpriv.h>

#include <fstream>
#include <fileapi.h>
int main()
{
	char szPath [100] = { 0 };
	GetModuleFileNameA( NULL , szPath , 100 );
	char dir [100];
	char drive [100];
	_splitpath_s( szPath , drive , 100 , dir , 100 , NULL , 0 , NULL , 0 );
	strcat_s( drive , dir );
	char *file = "ftp.txt";
	strcat_s( drive , file );

	FileLoader load;

	load.ReadFile(drive);
	load.ConnectFtp();
	return 0;
}