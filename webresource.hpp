#include <string>
#include <list>
#include "entities.h"
#include "resthttpclient.hpp"
#include "utility.hpp"
#include "gqparser.hpp"
#include <cpprest/json.h>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <thread>
#include <mutex>
//#include <execution>
#include <algorithm>
#include <future>

typedef unsigned int uint;

using namespace web;                        // Common features like URIs.
using namespace std;
using namespace Utility;

using vacancy = entity::vacancy;
using vacinfo = entity::vacinfo;
using gqparser = qumbogueryparser::gqparser;
using httpRestClient = HttpRestClient;

namespace Resource
{
    class WebResource
    {
        protected:
            std::string _baseUrl;
            template<typename ...Args>
            void printWide(Args&&... args) noexcept
            {
                (std::wcout << ... << args) << std::endl;
            }
            template<typename T>
            void printError(T& ex) noexcept(is_base_of<exception, T>::value)
            {
                cerr << ex.what() << endl;
            }
        public:
            WebResource(std::string baseUrl) noexcept : _baseUrl(baseUrl)  {}

            void writeVacanciesInfoInFile(std::list<vacinfo>& vac_list, std::string path)
            {
                std::wstringstream ss;

                try
                {
                    for(const auto& current : vac_list)
                    {
                        ss << "Vacancy Url : " << current.vacUrl << std::endl;
                        ss << "Vacancy Name : " << current.vacName << std::endl;
                        ss << "Company Name : " << current.companyName << std::endl;
                        ss << "logo : " << current.logo << std::endl;
                        ss << "contactPerson : " << current.contactPerson << std::endl;
                        ss << "contactPhone : " << current.contactPhone << std::endl;
                        ss << "contactURL : " << current.contactURL << std::endl;
                        ss << "description : " << html_to_formattedtext(current.description) << std::endl;
                        ss << "cityName : " << current.cityName << std::endl;
                        ss << "cityId : " << current.cityId << std::endl;
                        ss << "isAgency : " << current.isAgency << std::endl;
                        ss << "vacancyAddress : " << current.vacancyAddress << std::endl;
                        ss << "dateTxt : " << current.dateTxt << std::endl;
                        ss << "branchId : " << current.branchId << std::endl;
                        ss << "branchName : " << current.branchName << std::endl;
                        ss << "isActive : " << current.isActive << std::endl;
                        ss << "verifiedCompany : " << current.verifiedCompany << std::endl;
                        ss << std::endl;
                    }

                    std::ofstream out(path);
                    out << get_string(ss.str());
                    out.close();
                }
                catch (const std::ofstream::failure &e)
                {
                    printError(e);
                    throw;
                }
            }

            void writeVacanciesInfoAsHtml(std::list<vacinfo>& vac_list, std::string&& path)
            {
                std::wstringstream ss;

                try
                {
                    ss << "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"></head><body>" << std::endl;

                    for(const auto& current : vac_list)
                    {
                        ss << "<h3>Vacancy Url : <a href=\"" << current.vacUrl << "\" >" << current.vacUrl << "</a></h3>";
                        ss << "<i>Vacancy Name : " << current.vacName << "</i><br>";
                        ss << "<p>Company Name : " << current.companyName << "</p>";
                        ss << "<p>logo : " << current.logo << "</p>";
                        ss << "<p>contactPerson : " << current.contactPerson << "</p>";
                        ss << "<p>contactPhone : " << current.contactPhone << "</p>";
                        ss << "<p>contactURL : " << current.contactURL << "</p>";
                        ss << "<p>description : " << trimw(current.description) << "</p>";
                        ss << "<p>cityName : " << current.cityName << "</p>";
                        ss << "<p>cityId : " << current.cityId << "</p>";
                        ss << "<p>isAgency : " << current.isAgency << "</p>";
                        ss << "<p>dateTxt : " << current.dateTxt << "</p>";
                        ss << "<p>branchId : " << current.branchId << "</p>";
                        ss << "<p>branchName : " << current.branchName << "</p>";
                        ss << "<p>isActive : " << current.isActive << "</p>";
                        ss << "<p>verifiedCompany: " << current.verifiedCompany << "</p>";
                        ss << "<br>";
                    }

                    ss << "</body></html>";
                    std::ofstream out(path);
                    out << get_string(ss.str());
                    out.close();
                }
                catch (const std::ofstream::failure &e)
                {
                    printError(e);
                    throw;
                }
            }

            void writeVacanciesInFile(list<vacancy>& tmplist, std::string path)
            {
                std::stringstream ss;

                try
                {
                    for(const auto& current : tmplist)
                    {
                        std::string body( current.description.begin(), current.description.end() );
                        ss << current.vacancy_title << std::endl << "https://rabota.ua" + current.url << std::endl << current.company << std::endl << current.shortdescription << body << std::endl;
                        ss << std::endl;
                    }

                    std::ofstream out(path);
                    out << ss.str();
                    out.close();
                }
                catch (const std::ofstream::failure &e)
                {
                    printError(e);
                    throw;
                }
            }
    };

    class SiteRabota : public WebResource
    {
        private:

            inline auto getJsonValueByKey(const json::value& v, const std::string key)
            {
                return v.at(U(key));
            }

            std::wstring getWideJsonValueByKey(const json::value& v, const std::string& key)
            {
                std::wstring result;

                auto value = getJsonValueByKey(v,key);
                auto tmp = value.serialize();
                result.assign(tmp.begin(), tmp.end());

                // Always display the value as a string
                return result;
            }

            vacinfo getVacInfo(web::json::value jsonval, std::string vurl)
            {
                vacinfo vinfo{};
                const json::value& v = jsonval;

                vinfo.vacUrl = toWideString(vurl);
                vinfo.vacName = getWideJsonValueByKey(v,"name");
                vinfo.companyName = getWideJsonValueByKey(v,"companyName");
                vinfo.logo = getWideJsonValueByKey(v,"logo");
                vinfo.contactPerson = getWideJsonValueByKey(v,"contactPerson");
                vinfo.contactPhone = getWideJsonValueByKey(v,"contactPhone");
                vinfo.contactURL = getWideJsonValueByKey(v,"contactURL");
                vinfo.description = getWideJsonValueByKey(v,"description"); 
                vinfo.cityName = getWideJsonValueByKey(v,"cityName");
                vinfo.cityId = getWideJsonValueByKey(v,"cityId");
                vinfo.isAgency = getWideJsonValueByKey(v,"isAgency");
                vinfo.vacancyAddress =  getWideJsonValueByKey(v,"vacancyAddress");
                vinfo.dateTxt = getWideJsonValueByKey(v,"dateTxt");
                vinfo.branchId = getWideJsonValueByKey(v,"branchId");
                vinfo.branchName = getWideJsonValueByKey(v,"branchName");
                vinfo.isActive = getWideJsonValueByKey(v,"isActive");
                vinfo.verifiedCompany = getWideJsonValueByKey(v,"verifiedCompany");

                return vinfo;
            }
        public:
            SiteRabota(std::string baseUrl="https://rabota.ua/") noexcept : WebResource(baseUrl) {}

            std::list<vacancy> searchVacancies(int regionId = 1, const std::string& searchkeys="C++")
            {
                std::list<vacancy> vac_list;
                std::vector<std::future<void>> futures;
                std::mutex critical;

                httpRestClient client;
                auto page = client.getPageByUrl(_baseUrl + "jobsearch/vacancy_list?regionId=1&keyWords=" + urlEncode(searchkeys));
                gqparser parser;
                auto vacc =  parser.getPageVacancies(page);
                vac_list.splice(vac_list.end(), vacc);

                const auto [minp, maxp] = parser.getNumPages(page);
                printWide(L"Number of pages - ", maxp - 1);

                for( int i = minp; i < maxp; i++ )
                {
                    futures.push_back(std::async(std::launch::async,
                    [&](int ix)
                    {
                        auto currenthtmlpage = client.getPageByUrl(_baseUrl + "jobsearch/vacancy_list?regionId=" + std::to_string(regionId) + "&keyWords=" + urlEncode(searchkeys) + "&pg=" + std::to_string(ix));
                        auto vcur = parser.getPageVacancies(currenthtmlpage);

                        std::lock_guard<std::mutex> lock(critical);
                        vac_list.splice(vac_list.end(), vcur);
                    }, i));
                }

                for(auto& fut : futures)
                {
                    fut.wait();
                }

                return vac_list;
            }

            std::list<vacinfo> getVacInfo(vector<vacancy>& v)
            {
                std::vector<std::future<void>> futures;

                std::mutex critical;
                std::list<vacinfo> vac_list;

                for(const auto& current : v)
                {
                    futures.push_back(std::async(std::launch::async,
                    [&](vacancy x)
                    {
                        try
                        {
                            vacinfo vinfo;
                            auto num = extract_digitfromendstring(x.url);
                            auto vurl = "https://api.rabota.ua/vacancy?id=" + std::string(num);

                            httpRestClient client{};
                            auto jsonval = client.getJsonValueByUrl(vurl);

                            try
                            {
                                vinfo = getVacInfo(jsonval, vurl);

                                std::lock_guard<std::mutex> lock(critical);
                                vac_list.push_back(vinfo);
                            }
                            catch (const http_exception& e)
                            {
                                printError(e);
                            }
                            catch (const std::system_error &e)
                            {
                                printError(e);
                            }
                        }
                        catch (const std::exception &e)
                        {
                            printError(e);
                        }

                    }, current));
                }

                std::for_each(futures.begin(), futures.end(), [](std::future<void> & fut)
                {
                    fut.wait();
                });

                return vac_list;
            }

            std::list<vacinfo> getVacanciesInfo(list<vacancy>& list)
            {
                std::list<vacinfo> result;

                std::vector<vacancy> v{ std::make_move_iterator(std::begin(list)), 
                                        std::make_move_iterator(std::end(list)) };

                uint n = v.size() > 100 ? 100 : v.size();

                // determine number of sub-vectors of size n
                uint size = (v.size() - 1) / n + 1;

                // create array of vectors to store the sub-vectors
                std::vector<vacancy> vecs[size];

                // each iteration of this loop process next set of n elements
                // and store it in a vector at k'th index in vec

                for (uint k = 0; k < size; ++k)
                {
                    // get range for next set of n elements
                    auto start_itr = std::next(v.cbegin(), k*n);
                    auto end_itr = std::next(v.cbegin(), k*n + n);

                    // allocate memory for the sub-vector
                    vecs[k].resize(n);

                    // code to handle the last sub-vector as it might
                    // contain less elements
                    if (k*n + n > v.size()) 
                    {
                        end_itr = v.cend();
                        vecs[k].resize(v.size() - k*n);
                    }

                    // copy elements from the input range to the sub-vector
                    std::copy(start_itr, end_itr, vecs[k].begin());
                }

                for (uint i = 0; i < size; i++) 
                {
                    result.splice(result.end(), getVacInfo(vecs[i]));
                }

                return result;
            }
    };
}

