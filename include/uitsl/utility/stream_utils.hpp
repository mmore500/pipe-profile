#pragma once
#ifndef UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE
#define UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE

#include <fstream>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace uitsl {

class Line {
    std::string str;
public:
    Line() = default;

    operator std::string() const { return str; }
    friend std::istream& operator>>(std::istream&, Line&);
};

std::istream& operator>>(std::istream& is, Line& line) {
    std::getline(is, line.str);
    return is;
}

template <typename Out>
void read_lines(std::istream & is, Out out) {
    using In = std::istream_iterator<uitsl::Line>;
    std::copy(In(is), In(), out);
}

bool compare_streams(std::istream& s1, std::istream& s2) {
    // seek back to beginning and use std::equal to compare contents
    s1.seekg(0, std::istream::beg);
    s2.seekg(0, std::istream::beg);

    if (s1.fail() || s2.fail()) {
        std::cout << "stream problem" << std::endl;
        return false; // stream problem
    }

    if (s1.tellg() != s2.tellg()) {
        std::cout << "size mismatch: " << s1.tellg() << " vs " << s2.tellg() << std::endl;
        return false; //size mismatch
    }

    s1.seekg(0, std::istream::beg);
    s2.seekg(0, std::istream::beg);

    return std::equal(std::istreambuf_iterator<char>(s1.rdbuf()),
                        std::istreambuf_iterator<char>(),
                        std::istreambuf_iterator<char>(s2.rdbuf()));
}

// This function will tell us if the file generated by DataFile is identitcal
// to the expected file.
bool compare_files(const std::string& p1, const std::string& p2) {
    // adapted from https://stackoverflow.com/a/37575457
    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    return compare_streams(f1, f2);
}

} // namespace uitsl
#endif // #ifndef UITSL_UTILITY_STREAM_UTILS_HPP_INCLUDE
