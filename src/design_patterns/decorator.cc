#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>

struct HTTPRequest {
    std::string url;
    std::unordered_map<std::string, std::string> headers{};
};

struct HTTPResponse {
    int code;
    std::string message;
};

class HTTPClient {
    public:
        virtual ~HTTPClient() = default;
        virtual HTTPResponse request(HTTPRequest http_request) = 0;
};

class HTTPClientDecorator : public HTTPClient
{
public:
    virtual ~HTTPClientDecorator() = default;
protected:
    HTTPClientDecorator(std::unique_ptr<HTTPClient> http_client) {
        client = std::move(http_client);
    }
    std::unique_ptr<HTTPClient> client;
};

class LoggingDecorator: public HTTPClientDecorator {
    public:
        using HTTPClientDecorator::HTTPClientDecorator;
    
        HTTPResponse request(HTTPRequest http_request) {
            std::cout << "Logging Request" << http_request.url << std::endl;
            HTTPResponse response = client -> request(http_request);
            std::cout << "Response Code: " << response.code << std::endl;
            std::cout << "Response Message: " << response.message << std::endl;
        }
};

class RetryDecorator: public HTTPClientDecorator {
    private:
        int retries = 3;
    public:
        using HTTPClientDecorator::HTTPClientDecorator;
        HTTPResponse request(HTTPRequest http_request) {
            int i = 0;
            HTTPResponse response;
            while (i < retries) {
                response = client -> request(http_request);
                if(response.code == 200) break;
                ++i;
            }
            return response;
        }
};

// Simple Template Example for Decorator
template <typename Client>
class AuthenticationDecorator {
    Client client;
    HTTPResponse request(HTTPRequest http_request) {
        http_request.headers["Auth"] = "token";
        return client.request(http_request);
    }
};