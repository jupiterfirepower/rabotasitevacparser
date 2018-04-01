#ifndef IPAGE_PARS
#define IPAGE_PARS

#include <utility>      // std::pair
#include <string>      // std::string
#include <list>      // std::list

#include "entities.h"

using vacancy = entity::vacancy;

namespace pageparser
{
    class IPageParser
    {
        public:
            virtual ~IPageParser() {}
            virtual std::pair<int,int> getNumPages(const std::string& page) { return std::make_pair(0,0); }
            virtual std::list<vacancy> getPageVacancies(const std::string& page) { return {}; }
    };
}
#endif