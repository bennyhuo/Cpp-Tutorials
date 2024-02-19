//
// Created by benny on 2024/2/20.
//
#include <string>

struct S {
    std::string s;
};

struct CS {
    const char* s;
};

void Case1() {
    S ss[] = {
        {.s = "https://www.bennyhuo.com/"},
        {.s = "https://youtube.com/@bennyhuo"},
        {.s = "https://github.com/bennyhuo"},
    };

    CS cs[3];

    for (int i = 0; i < 3; i++) {
        auto s = ss[i];
        cs[i] = CS{.s = s.s.c_str()};
    }

    for (auto& c: cs) {
        std::printf("%s\n", c.s);
    }
}

void Case2() {
    S ss[] = {
        {.s = "https://www.bennyhuo.com/"},
        {.s = "https://youtube.com/@bennyhuo"},
        {.s = "https://github.com/bennyhuo"},
    };

    CS cs[3];

    for (int i = 0; i < 3; i++) {
        auto& s = ss[i];
        cs[i] = CS{.s = s.s.c_str()};
    }

    for (auto& c: cs) {
        std::printf("%s\n", c.s);
    }
}

int main(int argc, char* argv[], char* envp[]) {
    Case1();
    Case2();
    
    return 0;
}
