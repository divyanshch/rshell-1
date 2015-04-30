#include <pwd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <string>
#include "ls.h"

using namespace std;

int main(int argc, char *argv[]) {
	DIR *current;
	char *arg = new char[2];
	strcpy(arg, ".");
	argv[0] = arg;
	string vflags;
	vector<string> file_param;
	flag_separator(argv, file_param, vflags, argc); //separates flags from directories
	string path;
	if(file_param.size() == 1) {
		if(NULL == (current = opendir(file_param.at(0).c_str()))) {
			perror("Error in opening directory");
			exit(1);
		}
	}
	else {
		if(NULL == (current = opendir(file_param.at(1).c_str()))) {
			perror("Error in opening directory");
				exit(1);
                }
	}
	struct dirent *filespecs;
	errno = 0;
	filespecs = readdir(current);//
	vector<string> directories; //vector holds directories for display



	while(filespecs != NULL) {
		string dire(filespecs->d_name);
		directories.push_back(dire);
		filespecs = readdir(current);
	}
	if(errno) {
		perror("Error in reading directory");
		exit(1);                 
	}
	struct stat file;
	string permissions;
	sort(directories.begin(), directories.end(), alphabetical);
				
	for(unsigned int i = 0; i < directories.size(); i++) {
		Path_Creator(file_param, path, directories.at(i));
		if(path.find("home") != string::npos)
			path.erase(0, 2);
		if(stat(path.c_str(), &file) == -1)
			perror("stat");
		if(vflags.find("a") == string::npos && directories.at(i).at(0) == '.');
		else {
			if(vflags.find('l') != string::npos) {
				l_flag(file, permissions);
				cout << directories.at(i) << endl;
				R_flag(path, file);
			}
			else {
				cout << directories.at(i) << " ";
				R_flag(path, file);
			}
		}
		path = "";
	}
	cout << endl;
	if(-1 == closedir(current)) { 
		perror("Error in closing the directory");
		exit(1);
	}
	return 0;
}
