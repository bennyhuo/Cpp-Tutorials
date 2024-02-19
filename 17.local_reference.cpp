//
// Created by benny on 2024/2/20.
//
#include <future>
#include <iostream>
#include <type_traits>

class Sup {
public:
    virtual ~Sup() = default;

    virtual int GetWidth() = 0;

    void Log() {
        std::printf("sup: %p\n", this);
        // std::printf("sup: %d\n", GetWidth());
        // std::printf("sup: %s, %d\n", s.c_str(), GetWidth());
    }

private:
    std::string s = "hello";
};

class Sub: public Sup {
public:
    int GetWidth() override {
        return 1;
    }
};

class H {
public:
    Sub sub{};
};

std::future<void> test() {
    auto h = H();
    auto f = std::async([&]() {
        h.sub.Log();
    });

    return f;
}

int main (int argc, char *argv[], char *envp[]) {
    test().get();
    

    return 0;
}