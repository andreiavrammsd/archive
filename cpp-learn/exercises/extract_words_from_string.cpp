/*
 * https://www.fluentcpp.com/2020/04/17/how-to-extract-words-among-spaces-in-a-c-string/
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <cassert>
#include <forward_list>

template<typename OutputIterator>
void extractWords(std::string const &s, OutputIterator out) {
    auto lastExaminedPosition = begin(s);
    while (lastExaminedPosition != end(s)) {
        auto const beginWord = std::find_if_not(lastExaminedPosition, end(s), isspace);
        auto const endWord = std::find_if(beginWord, end(s), isspace);
        if (beginWord != endWord) {
            *out = std::string(beginWord, endWord);
            ++out;
        }
        lastExaminedPosition = endWord;
    }
}

auto extractWordsVector(std::string const &s) {
    std::vector<std::string> results;
    extractWords(s, back_inserter(results));
    return results;
}

bool testsPassed() {
    bool passed = true;
    passed = passed && (extractWordsVector("word1  word2  ") == std::vector<std::string>{"word1", "word2"});
    passed = passed && (extractWordsVector("  word1  word2  ") == std::vector<std::string>{"word1", "word2"});
    passed = passed && (extractWordsVector("  word1  word2") == std::vector<std::string>{"word1", "word2"});
    passed = passed && (extractWordsVector("  ") == std::vector<std::string>{});
    passed = passed && (extractWordsVector("") == std::vector<std::string>{});
    return passed;
}

template<typename Iterator>
void print(const Iterator it) {
    for (const auto i : it) {
        std::cout << i;
        std::cout << '\n';
    }
}

int main() {
    assert(testsPassed());

    std::string s = "word1  word2  ";

    // ostream
    std::stringstream str;
    std::ostream_iterator<std::string> results(str, ",");
    extractWords(s, results);
    assert(str.str() == "word1,word2,");

    // forward_list
    std::forward_list<std::string> fl;
    extractWords(s, std::front_inserter(fl));
    assert(fl.front() == "word2");
    fl.pop_front();

    assert(fl.front() == "word1");
    fl.pop_front();

    assert(fl.empty());
}
