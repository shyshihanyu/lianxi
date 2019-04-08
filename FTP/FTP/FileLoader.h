#pragma once
#ifndef FILELOADER_H
#define FILELOADER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../json/json.h"

using namespace std;

class FileLoader
{
public:
	FileLoader();
	~FileLoader();

public:
	void ConnectFtp();
	void AtCreateDirectory( string directory );
	void ReadFile( char* filename );
	void FindFile(string path);
public:
	vector<string> m_vFtp;
	string m_filename;
};

#endif // !FILELOADER_H