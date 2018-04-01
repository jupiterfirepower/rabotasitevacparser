#ifndef STRING_EXT
#define STRING_EXT

#include <string>
#include <algorithm>    // find_if
#include <regex>

using namespace std;

inline const string trim_chars = "\t\n\v\f\r ";
inline const string digits = "0123456789";

static string& ltrim(string& str, const string& chars = trim_chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

static string& rtrim(string& str, const string& chars = trim_chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

static string& trim(string& str, const string& chars = trim_chars)
{
    return ltrim(rtrim(str, chars), chars);
}

static string& trim(string&& str, const string& chars = trim_chars)
{
    return trim(str, chars);
}

static inline bool is_number(const string& s)
{
    return !s.empty() && find_if(s.begin(), 
        s.end(), [](char c) { return !isdigit(c); }) == s.end();
}

static wstring trimw(const wstring& str)
{
    const auto newline = L"\r\n\t\v\f ";
    const auto strBegin = str.find_first_not_of(newline);
    if (strBegin == string::npos)
        return L""; // no content

    const auto strEnd = str.find_last_not_of(newline);
    const auto strRange = strEnd - strBegin + 1;

    return wstring(str.substr(strBegin, strRange));
}

[[maybe_unused]] static bool is_digits(const string &str)
{
	return str.find_first_not_of(digits) == string::npos;
}

inline bool ends_with(string const & value, string const & ending)
{
    if (ending.size() > value.size()) return false;
    return equal(ending.rbegin(), ending.rend(), value.rbegin());
}

static wstring html_to_formattedtext(const wstring htmlcontent) noexcept
{
    //L"<[^>]*(>|$)" //match any character between '<' and '>', even when end tag is missing
    wstring result = regex_replace(htmlcontent, 
                                        wregex(L"</?(?:script|embed|object|frameset|frame|iframe|meta|link|style)(.|\\n|\\s)*?>", 
                                        regex_constants::ECMAScript | regex_constants::icase) , L"");

    result = regex_replace(htmlcontent, 
                                wregex(L"</?(h1|h2|h3|h4|h5|h6|p|b|strong|em|mark|small|del|ins|sub|sup|ul|ol|li|i|u|dl|table|div|br|hr|td|tr|pre|address|blockquote|fieldset|form|a|img)[^>]*>",
                                regex_constants::ECMAScript | regex_constants::icase) , L"\n");

    result = regex_replace(result, wregex(L"(?:\r\n|\n)+"), L"\n");
    result = regex_replace(result, wregex(L"\\&nbsp;"), L" ");

    return trimw(result);
}

static auto extract_digitfromendstring(const string str)
{
    auto last_index = str.find_last_not_of(digits);
    return str.substr(last_index + 1);
}

static auto get_string(const wstring wstring)
{
    string str;
    str.assign(wstring.begin(), wstring.end());
    return str;
}

static auto toWideString(const string str)
{
    wstring wstr;
    wstr.assign(str.begin(), str.end());
    return wstr;
}

#endif    