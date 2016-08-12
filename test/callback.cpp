#include <args.hpp>
#include "test.hpp"

struct callback_cmd
{
    int count;
    std::string name;
    std::string final_name;

    template<class F>
    void parse(F f)
    {
        auto cb = [this](auto& count, const args::context&, const args::argument& arg) {
            if (arg.has_value) for(int i=0;i<count;i++) final_name += name;
        };
        f(count, "--count", "-C", args::callback(cb));
        f(name, "--name", "-N");
    }

    void run()
    {}
};

PROVE_CASE()
{
    callback_cmd cmd;
    args::parse(cmd, {"--count", "5", "--name", "hello"});
    PROVE_CHECK(cmd.count == 5);
    PROVE_CHECK(cmd.name == "hello");
    PROVE_CHECK(cmd.final_name == "hellohellohellohellohello");
}

PROVE_CASE()
{
    callback_cmd cmd{};
    args::parse(cmd, {"--name", "hello"});
    PROVE_CHECK(cmd.count == 0);
    PROVE_CHECK(cmd.name == "hello");
    PROVE_CHECK(cmd.final_name == "");
}

struct eager_callback_cmd
{
    int count;
    std::string name;
    std::string final_out;

    template<class F>
    void parse(F f)
    {
        auto cb = [this](auto& count, const args::context&, const args::argument&) {
            final_out = std::to_string(count);
        };
        f(count, "--count", "-C", args::eager_callback(cb));
        f(name, "--name", "-N");
    }

    void run()
    {}
};

PROVE_CASE()
{
    eager_callback_cmd cmd;
    args::parse(cmd, {"--count", "5", "--name", "hello"});
    PROVE_CHECK(cmd.count == 5);
    PROVE_CHECK(cmd.name == "");
    PROVE_CHECK(cmd.final_out == "5");
}

struct eager_callback_null_cmd
{
    int count = 0;
    std::string name;
    std::string final_out;

    template<class F>
    void parse(F f)
    {
        auto cb = [this](auto&&, const args::context&, const args::argument&) {
            final_out = "hello";
        };
        f(nullptr, "--out", "-O", args::eager_callback(cb));
        f(count, "--count", "-C");
        f(name, "--name", "-N");
    }

    void run()
    {}
};

PROVE_CASE()
{
    eager_callback_null_cmd cmd;
    args::parse(cmd, {"--out", "--count", "5", "--name", "hello"});
    PROVE_CHECK(cmd.count == 0);
    PROVE_CHECK(cmd.name == "");
    PROVE_CHECK(cmd.final_out == "hello");
}