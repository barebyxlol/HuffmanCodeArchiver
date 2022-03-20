#include "AlgorithmProcessing.h"

namespace algorithm_processer {

    void ShowInfo(bool full) {
        if (full) {
            std::cout << "This is the archiver programme created by Filatov Egor\n"
                         "List of available commands:\n"
                         "archiver -c archive_name file1 [file2 ...] :\n"
                         "Creates archive *archive name* in current directory"
                         "with the listed files.\n"
                         "All of the files should be in the same directory as the archiver\n"
                         "archiver -d archive_name :\n"
                         "Unzips files from archive *archive name* and saves them in current"
                         "directory.\n"
                         "The archive should be in the same directory as the archiver.\n"
                         "archiver -h :\n"
                         "Shows this message.";
        } else {
            std::cout << "To see the documentation type archiver -h";
        }
    }

    Result ParseAlgorithm(int argc, char *argv[]) {
        Result result;
        if (argc <= 1) {
            result.first = ParsingResult::ERROR;
            std::cout << "Too few arguments\n";
        }
        std::string mode = std::string(argv[1]);
        if (mode == "-h") {
            result.first = ParsingResult::HELP;
        } else if (mode == "-c") {
            if (argc < 4) {
                result.first = ParsingResult::ERROR;
                std::cout << "Too few arguments\n";
            } else {
                result.first = ParsingResult::ENCODE;
                result.second.emplace_back(argv[2]);
                for (int i = 3; i < argc; ++i) {
                    if (!std::filesystem::exists(argv[i])) {
                        result.first = ParsingResult::ERROR;
                        std::cout << "No file " << argv[i] << " in current directory\n";
                        break;
                    } else {
                        result.second.emplace_back(argv[i]);
                    }
                }
                return result;
            }
        } else if (mode == "-d") {
            if (argc < 3) {
                result.first = ParsingResult::ERROR;
                std::cout << "Too few arguments\n";
            } else if (argc > 3) {
                result.first = ParsingResult::ERROR;
                std::cout << "Too many arguments\n";
            } else {
                result.first = ParsingResult::DECODE;
                result.second = {argv[2]};
                if (!std::filesystem::exists(argv[2])) {
                    result.first = ParsingResult::ERROR;
                    std::cout << "No file " << argv[2] << " in current directory\n";
                }
            }
        } else {
            result.first = ParsingResult::ERROR;
            std::cout << "Unknown command\n";
        }
        return result;
    }

}  // namespace algorithm_processer
