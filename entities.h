#ifndef ENTITY_ST
#define ENTITY_ST

#include <string>

namespace entity
{
    struct vacancy
    {
        std::string vacancy_title;
        std::string url;
        std::string company;
        std::string shortdescription;
        std::wstring description;
        //vacancy( const vacancy &obj)=default;  // copy constructor
    };

    struct pages
    {
        int pgnumber;
        std::string link;
    };

    struct vacinfo
    {
        std::wstring vacUrl;
        std::wstring vacName;
        std::wstring companyName;
        std::wstring logo;
        std::wstring contactPerson;
        std::wstring contactPhone;
        std::wstring contactURL;
        std::wstring description;
        std::wstring cityName;
        std::wstring cityId;
        std::wstring isAgency;
        std::wstring vacancyAddress;
        std::wstring dateTxt;
        std::wstring branchId;
        std::wstring branchName;
        std::wstring isActive;
        std::wstring verifiedCompany; 
    };
}

#endif
