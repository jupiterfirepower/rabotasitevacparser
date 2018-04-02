#include <fstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <gumbo.h>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>
#include <list>
#include <stdio.h>
#include "Gumbo.hpp"
#include "stringext.hpp"
#include "gumboparser.hpp"
#include "gqparser.hpp"
#include "utility.hpp"
#include "webresource.hpp"
#include "entities.h"
#include <regex>
#include <sstream>
#include <codecvt>
#include <clocale>
#include <chrono>
#include "Document.h"
#include "Node.h"

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
    std::cerr << ex.what() << endl;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    std::list<vacancy> vac_list;

    try
    {
	Resource::SiteRabota rb;
	// Get starting timepoint
	const auto start = high_resolution_clock::now();

	auto result = rb.searchVacancies();

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
	std::cerr << "error ..." << endl;
    }

    return 0;
}
