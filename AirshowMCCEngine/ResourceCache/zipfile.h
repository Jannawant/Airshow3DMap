#ifndef ZIPFILE_H
#define ZIPFILE_H

#include <stdio.h>
#include <map>
#include <string>

const int _MAX_PATH = 80;

typedef std::map<std::string,int> ZipContentsMap;

class ZipFile
{
    struct TZipDirHeader;
    struct TZipDirFileHeader;
    struct TZipLocalHeader;
    FILE *m_pFile; // Zip file
    char* m_pDirData; // Raw data buffer
    int m_nEntries; // Number of entries
    //Pointers to the dir entries in pDirData
    const TZipDirFileHeader **m_papDir;
public:
    ZipFile(){m_nEntries =0; m_pFile=NULL; m_pDirData=NULL;}
    virtual ~ZipFile() { end(); fclose(m_pFile);}
    bool init(const std::wstring &resFileName);
    void end();
    int getNumFiles() const { return m_nEntries;}
    std::string getFilename(int i) const;
    int getFileLen(int i) const;
    bool readFile(int i, void *pBuf);
    //Added to show multi-threaded decompression
    bool readLargeFile(int i, void *pBuf, void(*progressCallback)(int, bool &));
    int find(const std::string &path) const;
    ZipContentsMap m_ZipContentsMap;
};




























#endif // ZIPFILE_H
