#include <iostream>
#include <sstream>
#include <clocale>
#include <chrono>
#include <gumbo.h>
#include "Gumbo.hpp"
#include "stringext.hpp"
#include "gumboparser.hpp"
#include "gqparser.hpp"
#include "utility.hpp"
#include "webresource.hpp"
#include "entities.h"

using namespace std;
using namespace chrono;
using vacancy = entity::vacancy;
using pages = entity::pages;
using vacinfo = entity::vacinfo;

using std::string;
using std::ostringstream;

template<typename T>
void printError(T& ex) noexcept(is_base_of<exception, T>::value)
{
    std::wcerr << ex.what() << endl;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    std::list<vacancy> vac_list;

    int regionId;
    std::string searchKeys = "";

    if(argc < 2)
    {
	std::wcout << L"Usage: [parsevac regionId searchKey] eaxample: parsevac 1 C#" << std::endl;
        std::wcout << L"Used default parameters: regionId = 1, searchKeys = C++" << std::endl;
        regionId = 1;
	searchKeys = "C++";
    }
    else
    {
        regionId = is_number(argv[0]) ? std::atoi(argv[0]) : 1;
        for(int i = 1; i < argc; i++)
	{
	    searchKeys += std::string(argv[i]);
	}
    }

    try
    {
	Resource::SiteRabota rb;
	// Get starting timepoint
	const auto start = high_resolution_clock::now();

	auto result = rb.searchVacancies(regionId, searchKeys);

        std::wcout << std::endl << L"Vacancy Short List Size : "<< result.size() << std::endl;
	//auto tmplist = first_n(result, 50);
        auto tmplist = result;

        auto vresult = rb.getVacanciesInfo(tmplist);
	const auto stop = high_resolution_clock::now();

        std::wcout << L"Vacancy Info Extended List Size : "<< vresult.size() << std::endl;
	// Get duration. Substart timepoints to get durarion. 
	// To cast it to proper unit use duration cast method
	const auto duration = duration_cast<seconds>(stop - start);
	std::wcout << std:: endl << "Time taken by function load and parse: " << duration.count() << " seconds" << std::endl << std::endl;

	rb.writeVacanciesInfoInFile(vresult,"vacinfo.txt");
	std::wcout << L"Vac Info Saved in File : vacinfo.txt" << std::endl;
        rb.writeVacanciesInfoAsHtml(vresult,"vacinfo.html");
	std::wcout << L"Vac Info Saved as HTML in File : vacinfo.html" << std::endl;
    }
    catch (const runtime_error& ex)
    {
	printError(ex);
    }
    catch (exception& ex)
    {
	printError(ex);
    }
    catch (...)
    {
	std::wcerr << "error ..." << endl;
    }

    return 0;
}
