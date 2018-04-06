#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>
#include "stringext.hpp"

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams


class HttpRestClient
{
    private:
        static constexpr bool diagnostic_mode = false;
        template<typename ...Args>
        void printWide(Args&&... args) noexcept
        {
            if constexpr (HttpRestClient::diagnostic_mode)
                (std::wcout << ... << args) << std::endl;
        }
    private:
        template<typename T>
        T getContentByUrl(const std::string& url, std::function <T (http_response&)> getdata)
        {
            T result = {};

            http_client client(url);
            http_request request(methods::GET);

            int i = 0;
            do
            {
                auto requestTask = client.request(request).then([&result, &getdata, &url, &i, this](http_response response)
                {
                    // Perform actions here to inspect the HTTP response...
                    if(response.status_code() == status_codes::OK)
                    {
                        result = getdata(response);
                        i = 10;
                        //printWide(L"Content Page( ", toWideString(url), " ) Size : ", result.size());
                    }
                });

                try
                {
                    requestTask.wait();
               }
                catch (const std::exception &e)
                {
                    i++;
                    if(i >= 3)
                    {
                        std::wcerr << L"Error Content Page - ( " << toWideString(url) << " )" << std::endl;
                        std::wcerr << L"Error exception: " << e.what() << std::endl;
                        throw;
                    }
                    else
                    {
                        std::wcerr << L"Retry Content Page - ( " << toWideString(url) << " )" << std::endl;
                    }
                }
            }
            while(i < 3);

            return result;
        }
    public:
         auto getPageByUrl(const std::string& url, const std::function <std::string (http_response&)> getdata = [] (http_response& response)
            {
                return response.extract_string().get();
            })
        {
            return getContentByUrl<std::string>(url, getdata);
        }

        auto getJsonValueByUrl(const std::string& url, const std::function <json::value (http_response&)> getdata = [] (http_response& response)
            {
                return response.extract_json().get();
            })
        {
            return getContentByUrl<web::json::value>(url, getdata);
        }
};
