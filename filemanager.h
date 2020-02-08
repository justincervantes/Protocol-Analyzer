#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <iostream>
#include <fstream>
#include <QString>

using namespace std;

class FileManager
{
public:
    FileManager();
    ~FileManager();
    QString filename;
    fstream fs;
    char * buffer;
    void buildBufferFromFile(int size);
};


#endif // FILEMANAGER_H
