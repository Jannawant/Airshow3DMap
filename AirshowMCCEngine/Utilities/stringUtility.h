#ifndef STRINGUTILITY_H
#define STRINGUTILITY_H

#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <algorithm>

using std::string;
using std::wstring;

#define MAX_DIGITS_IN_INT 12 // max number of digits in an int (-2147483647 = 11 digits, +1 for the '\0')

typedef std::vector<std::string> StringVec;
//extern std::string ToStr(int num, int base = 10);
//extern std::string ToStr(unsigned int num, int base = 10);
//extern std::string ToStr(unsigned long num, int base = 10);
//extern std::string ToStr(float num);
//extern std::string ToStr(double num);
//extern std::string ToStr(bool val);
//extern std::string ToStr(const Vec3& vec);

wstring s2ws(const std::string& str);
string ws2s(const std::wstring& wstr);
string strToLower(string str);
string strToUpper(string str);
void strToLower(char *str, int len);
bool WildcardMatch(const char *pat, const char *str);
#pragma warning(push)
#pragma warning(disable : 4311)

class HashedString
{
public:
    explicit HashedString(const char * const pIdentString) : m_ident(hash_name(pIdentString))
      ,m_identStr(pIdentString)
    {}
    unsigned long getHashValue(void) const
    {
        return reinterpret_cast<unsigned long>(m_ident);
    }
    const std::string & getStr() const
    {
        return m_identStr;
    }
    static void* hash_name(const char * pIdentStr);
    bool operator< (HashedString const &o) const
    {
        bool r = (getHashValue() < o.getHashValue());
        return r;
    }
    bool operator==(HashedString const& o) const
    {
        bool r = (getHashValue() == o.getHashValue());
        return r;
    }
private:
    // note: m_ident is stored as a void* not an int, so that in
    // the debugger it will show up as hex-values instead of
    // integer values. This is a bit more representative of what
    // we're doing here and makes it easy to allow external code
    // to assign event types as desired.
    void * m_ident;
    std::string m_identStr;
};

//Remove the warning for warning #4311...
#pragma warning(pop)


#endif // STRINGUTILITY_H
