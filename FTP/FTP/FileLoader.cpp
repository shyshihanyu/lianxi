#include "FileLoader.h"

#include <io.h>
#include <direct.h>
#include <afxinet.h>
#include <cstringt.h>
#include <afxpriv.h>

#include <fstream>
#include <fileapi.h>

#pragma comment(lib, "json_vc71_libmtd.lib") 
using namespace std;
using namespace Json;

FileLoader::FileLoader()
{
}


FileLoader::~FileLoader()
{
}

//void FileLoader::ReadFile( char* filename )
//{
//	Json::Reader reader;
//	Json::Value root;
//	ifstream is( filename, ios::binary );
//	if (!is.is_open())
//	{
//		cout << "open json file failed." << endl;
//		return;
//	}
//     // 解析json内容   
//	string stpath;
//	string ftpurl;
//	string usename;
//	string passwd;
//	string name;
//	m_vFtp.clear();
//	if (reader.parse( is , root ) != 0)
//	{
//		ftpurl = root ["ftpurl"].asString();
//		usename = root ["username"].asString();
//		passwd = root ["passwd"].asString();
//		stpath = root ["stpath"].asString();
//		name = root ["filename"].asString();
//		m_vFtp.push_back( ftpurl );
//		m_vFtp.push_back( usename );
//		m_vFtp.push_back( passwd );
//		m_vFtp.push_back( stpath );
//		m_vFtp.push_back( name );
//	}
//	
//	is.close();
//	return;
//}

void FileLoader::ReadFile( char* filename )
{
	ifstream in( filename );
	string line;

	if (in) // 有该文件  
	{
		while (!in.eof()) // line中不包括每行的换行符  
		{
			getline( in , line );
			cout << line << endl;
			m_vFtp.push_back( line );
		}
	}
	else // 没有该文件  
	{
		cout << "no such file" << endl;
	}
}

void FileLoader::AtCreateDirectory(string directory)
{
	if (0 != _access(directory.c_str(), 0))
	{
		// if this folder not exist, create a new one.
		_mkdir(directory.c_str());

	}
}

void FileLoader::FindFile( string path )
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;
	string pathName , exdName;

	if (( hFile = _findfirst( pathName.assign( path ).append( "\\*" ).c_str() , &fileInfo ) ) == -1)
	{
		return;
	}
	do
	{
		if (fileInfo.attrib&_A_SUBDIR)
		{
			string fname = string( fileInfo.name );
			if (fname != ".." && fname != ".")
			{
				FindFile( path + "\\" + fname );
			}
		}
		else
		{

			char ext [10];
			_splitpath_s( fileInfo.name , NULL , 0 , NULL , 0 , NULL , 0 , ext , 10 );
			
				if (strcmp( ext ,".rar" ) == 0)
				{
					m_filename = fileInfo.name;
					//cout << fileInfo.name << endl;
				}

		}
	} while (_findnext( hFile , &fileInfo ) == 0);
	_findclose( hFile );
}

void FileLoader::ConnectFtp()
{
	FindFile( m_vFtp.at( 3 ) );
	string file = m_vFtp.at( 3 ) + "\\" + m_filename;
	string filename1 = m_vFtp.at( 4 ) + m_filename;
	CString url(m_vFtp.at(0).c_str());
	CString username( m_vFtp.at( 1 ).c_str() );
	CString password( m_vFtp.at( 2 ).c_str() );
	CString path( file.c_str() );
	CString filename(filename1.c_str());
	
	CInternetSession sess(_T("ftp"));
	CFtpConnection* pConnect = NULL;

	try
	{
		// Request a connection to ftp.microsoft.com. Default
		// parameters mean that we'll try with username = ANONYMOUS
		// and password set to the machine name @ domain name
		pConnect = sess.GetFtpConnection(url, username, password, 21, FALSE);

		// use a file find object to enumerate files
		CFtpFileFind finder(pConnect);
		pConnect->PutFile(path,filename, FTP_TRANSFER_TYPE_BINARY , 1 );

	}
	catch (CInternetException* pEx)
	{
		TCHAR sz[1024];
		pEx->GetErrorMessage(sz, 1024);
		_tprintf_s(_T("ERROR!  %s\n"), sz);
		pEx->Delete();
	}

	// if the connection is open, close it
	if (pConnect != NULL)
	{
		pConnect->Close();
		delete pConnect;
	}
}


