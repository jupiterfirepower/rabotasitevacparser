#include <algorithm>
#include <string>
#include <list>

#include "Gumbo.hpp"
#include "entities.h"
#include "iparser.h"

using namespace EasyGumbo;

using vacancy = entity::vacancy;
using pages = entity::pages;
using ipageparser = pageparser::IPageParser;

using giterator = EasyGumbo::DFSIterator;
using Tag = EasyGumbo::Tag;
using Element = EasyGumbo::Element;

namespace gumboparser
{
    class gparser : public ipageparser
    {
        private:
            decltype(auto) getElement(giterator iter, giterator end, Tag tag, std::string atrName, std::string attributeValue)
            {
                auto iterator = std::find_if(iter, end, And(Tag(tag), HasAttribute(atrName, attributeValue)));

                return Element {*iterator};
            }

            decltype(auto) getElementText(Element element)
            {
                auto text   = element.children()[0];
                auto result = trim(std::string(text->v.text.text));

                return result;
            }

            std::string getTagTitle(giterator iter, giterator end, Tag tag, std::string atrName, std::string attributeValue)
            {
                auto element = getElement(iter, end, tag, atrName, attributeValue);

                return getElementText(element);
            }

            decltype(auto) getTitleOrAttributeValue(giterator iter, giterator end, Tag tag, std::string atrName, std::string attributeValue, std::string attributeValueByName)
            {
                auto element = getElement(iter, end, tag, atrName, attributeValue);

                if(auto elemt = element; attributeValueByName.empty())
                {
                    return std::make_tuple(getElementText(elemt), std::string());
                }
                return std::make_tuple(getElementText(element), attributeValueByName.empty()  ? "" : std::string(element.attribute(attributeValueByName.c_str())->value));
            }
        public:
            std::pair<int,int> getNumPages(const std::string& page) override
            {
                
                std::list<pages> page_list;
                Gumbo parser(&page[0]);
                Gumbo::iterator iter;

                iter = parser.begin();
                while (iter != parser.end()) 
                {
                    iter = std::find_if(iter, parser.end(),And(Tag(GUMBO_TAG_DL),HasAttribute("id", "content_vacancyList_gridList_pagerInnerTable")));
                    if (iter == parser.end()) {
                        break;
                    }
                    auto pagesnum = findAll(iter.fromCurrent(), parser.end(), Tag(GUMBO_TAG_A));
                    for (auto currentp : pagesnum) 
                    {
                        Element pagelink(*currentp);

                        if(auto curtext = pagelink.children()[0]->v.text.text; is_number(curtext))
                        {
                            int pg = std::atoi(curtext);
                            auto link = std::string(pagelink.attribute("href")->value);
                            pages psg = { pg, link };

                            page_list.push_back(psg);
                        }

                    }

                    ++iter;
                }

                decltype(page_list)::iterator minEl, maxEl;
                auto minmaxelm = std::minmax_element(begin(page_list), end(page_list),
                [] (pages const& s1, pages const& s2)
                {
                    return s1.pgnumber < s2.pgnumber;
                });

                return std::make_pair(minmaxelm.first->pgnumber, minmaxelm.second->pgnumber);
            }

            std::list<vacancy> getPageVacancies(const std::string& page) override
            {
                std::list<vacancy> vac_list;
                Gumbo parser(&page[0]);

                Gumbo::iterator iter = parser.begin();
        
                while (iter != parser.end()) 
                {
                    iter = std::find_if(iter, parser.end(),And(Tag(GUMBO_TAG_H3),HasAttribute("class", "fd-beefy-gunso f-vacancylist-vacancytitle")));
                    if (iter == parser.end()) {
                        break;	
                }

                auto [name, href] = getTitleOrAttributeValue(iter, parser.end(), Tag(GUMBO_TAG_A),"class", "f-visited-enable ga_listing", "href");
                auto company = getTagTitle(iter, parser.end(), Tag(GUMBO_TAG_A),"class", "f-text-dark-bluegray f-visited-enable");
                auto desc    = getTagTitle(iter, parser.end(), Tag(GUMBO_TAG_P),"class", "f-vacancylist-shortdescr f-text-gray fd-craftsmen");
                
                vacancy current = { name,  href, company, desc, L"" };

                vac_list.push_back(current);

                ++iter;
            }

            return vac_list;
        }
    };
}
