#ifndef GQ_PARSER
#define GQ_PARSER

#include <iostream>
#include <string>
#include "Document.h"
#include "Node.h"
#include <algorithm>
#include <list>
#include "entities.h"
#include "iparser.h"

using vacancy = entity::vacancy;
using ipageparser = pageparser::IPageParser;

namespace qumbogueryparser
{
    class gqparser : public ipageparser
    {
        public:
            std::pair<int,int> getNumPages(const std::string& page) override
            {
                std::list<int> page_list;
                CDocument doc;

                doc.parse(page.c_str());

                CSelection pages_href = doc.find("a");
                int num = pages_href.nodeNum();

                for( int i = 0; i < num; i++)
                {
                    if(auto val = pages_href.nodeAt(i).attribute("href"); val.find("&pg=") != std::string::npos)
                        page_list.push_back(std::stoi(extract_digitfromendstring(val)));
                }

                auto minmaxelm = std::minmax_element(page_list.begin(), page_list.end());
                return std::make_pair(*minmaxelm.first, *minmaxelm.second);
            }

            std::list<vacancy> getPageVacancies(const std::string& page) override
            {
                std::list<vacancy> vac_list;
                CDocument doc;

                doc.parse(page.c_str());

                CSelection vacs = doc.find("a[class=\"f-visited-enable ga_listing\"]");

                for(int i = vacs.nodeNum(); i--; )
                {
                    CNode node = vacs.nodeAt(i);
                    auto vacname = trim(std::string(node.text()));
                    auto href = node.attribute("href");

                    vac_list.push_front(vacancy { vacname,  href, "", "", L"" });
                }

                return vac_list;
            }
    };
}
#endif