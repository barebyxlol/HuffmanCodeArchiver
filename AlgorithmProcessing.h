#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace algorithm_processer {

    enum class ParsingResult {
        ENCODE,
        DECODE,
        HELP,
        ERROR,
    };

    using Result = std::pair<ParsingResult, std::vector<std::string> >;

    Result ParseAlgorithm(int argc, char *argv[]);

    void ShowInfo(bool full = true);

}  // namespace algorithm_processer
