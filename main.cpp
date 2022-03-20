#include "BitOperations.h"
#include "HuffmanCode.h"
#include "AlgorithmProcessing.h"

int main(int argc, char *argv[]) {

    algorithm_processer::Result directions = algorithm_processer::ParseAlgorithm(argc, argv);
    if (directions.first == algorithm_processer::ParsingResult::ERROR) {
        algorithm_processer::ShowInfo(false);
    } else if (directions.first == algorithm_processer::ParsingResult::HELP) {
        algorithm_processer::ShowInfo();
    } else if (directions.first == algorithm_processer::ParsingResult::ENCODE) {
        std::ofstream archive(directions.second[0], std::iostream::binary);
        huffman::Encoder encoder(archive);
        for (size_t i = 1; i < directions.second.size(); ++i) {
            std::string file = std::string(directions.second[i]);
            encoder.EncodeFile(file);
            std::cout << "File " << file << " encoded successfully\n";
        }
        std::cout << "Encoding finished\n";
    } else {
        try {
            std::ifstream archive(directions.second.front(), std::iostream::binary);
            huffman::Decoder decoder(archive);
            decoder.Decode();
        } catch(const std::invalid_argument&) {
            std::cout<<"Unable to read the archive";
        }
    }
}
