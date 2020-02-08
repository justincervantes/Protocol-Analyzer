#include "filemanager.h"
#include <stdlib.h>
FileManager::FileManager()
{
    qDebug("You've reached me!");
    buffer = (char*) malloc(1024 * sizeof(char));

}

FileManager::~FileManager()
{
    free(buffer);
}
void FileManager::buildBufferFromFile(QString size) {

    int mallocSize;

    if(size == "1024B") {
        mallocSize = 1024;
    } else if(size == "4096B") {
        mallocSize = 4096;
    } else if(size == "20KB") {
        mallocSize = 20000;
    } else if(size == "60KB") {
        mallocSize = 60000;
    } else {
        mallocSize = 100000;
    }
    buffer = (char*)realloc(buffer, static_cast<size_t>(mallocSize * sizeof(char)));
    if(!fs.is_open()) {
        this->fs.open("C:\\Users\\Justin Cervantes\\Documents\\build-ProtocolAnalyzer-Desktop_Qt_5_14_0_MinGW_64_bit-Debug\\alice_in_wonderland.txt");
        this->fs.read(buffer, mallocSize);
    } else {
        fs.clear();
        fs.seekg(0, ios::beg);
        fs.read(buffer, mallocSize);
    }
}
