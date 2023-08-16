#include <coroutine>
#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "co_requests.hpp"

static constexpr char NEWLINE {'\n'};

httplib::Result make_request(const std::string& host, const std::string& uri)
{
    // HTTP
    httplib::Client cli(host);
    return cli.Get(uri);
}

template <typename Payload>
httplib::Result make_post(const std::string& host, const std::string& uri, Payload& payload)
{
    // HTTP
    httplib::Client cli(host);
    httplib::Params params({
        {"body", payload},
        {"a", "7"},
        {"b", "4"},
        });
    std::string json_body{"{\"a\": 1, \"b\": 2}"}; // The basic_example crow server POST endpoint wants a json with field 'a' and 'b'
                                                   // Would be nice if `Params` helped us here, but not going to fight it as the http backed may change

    return cli.Post(uri, json_body, "json");
}

struct promise;
 
struct coroutine : std::coroutine_handle<promise>
{
    using promise_type = ::promise;
};
 
struct promise
{
    coroutine get_return_object() { return {coroutine::from_promise(*this)}; }
    std::suspend_always initial_suspend() noexcept { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void return_void() {}
    // void return_value() {}
    void unhandled_exception() {}
};
 
struct S
{
    int i;
    coroutine f()
    {
        std::cout << i;
        co_return;
    }
};

namespace crq{

void get(const std::string& host, const std::string& val)
{
    coroutine h = [](const std::string& host, const std::string& uri) -> coroutine //
    {
        const auto result = make_request(host, uri);
        std::cout << result->status << NEWLINE;
        std::cout << result->body << NEWLINE;

        std::cout << uri << NEWLINE;
        co_return;
    }(host, val);
    std::cout << "doing things" << NEWLINE;

    // lambda destroyed
    h.resume(); // no problem, i has been copied to the coroutine
                // frame as a by-value parameter
    h.destroy();
}

void post(const std::string& host, const std::string& val)
{
    std::string pl {"payload"};
    coroutine h = [](const std::string& host, const std::string& uri, const std::string& payload) -> coroutine //
    {
        const auto result = make_post(host, uri, payload);
        std::cout << result->status << NEWLINE;

        std::cout << uri << NEWLINE;
        co_return;
    }(host, val, pl);
    std::cout << "doing things" << NEWLINE;

    // lambda destroyed
    h.resume(); // no problem, i has been copied to the coroutine
                // frame as a by-value parameter
    h.destroy();
}

}

int main() {
    try {
        // get("https://reqres.in","/api/users/2");
        crq::get("http://localhost:18080",R"(/)");
        crq::get("http://localhost:18080",R"(/about)");
        crq::get("http://localhost:18080",R"(/json)");
        crq::get("http://localhost:18080",R"(/json_list)");

        crq::post("http://localhost:18080", R"(/add_json)");
    } catch (...) {
        std::cout << "unknown exception" << NEWLINE;
    }
}
