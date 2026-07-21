#include "FxTradeLoader.h"
#include "../Models/FxTradeList.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>

FxTrade* FxTradeLoader::createTradeFromLine(std::string line) {
    std::vector<std::string> items;
    size_t start = 0, pos;

    while ((pos = line.find(separator, start)) != std::string::npos) {
        std::string item = line.substr(start, pos - start);
        if (!item.empty() && item.back() == '\r') {
            item.pop_back();
        }
        items.push_back(item);
        start = pos + separator.size();
    }
    
    FxTrade* trade = new FxTrade(items[8]);
    
    std::tm tm = {};
    // TradeDate
    std::istringstream tradeDateStream(items[1]);
    tradeDateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto tradeTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setTradeDate(tradeTimePoint);
    // ValueDate
    tm = {};
    std::istringstream valueDateStream(items[6]);
    valueDateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto valueTimePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setValueDate(valueTimePoint);
    trade->setTradeType(items[0]);
    trade->setInstrument(items[2]+items[3]);
    trade->setCounterparty(items[7]);
    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));
    
    return trade;
}

void FxTradeLoader::loadTradesFromFile(std::string filename, FxTradeList& tradeList) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be null");
    }
    
    std::ifstream stream(filename);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int lineCount = 0;
    std::string line;
    while (std::getline(stream, line)) {
        if (lineCount == 0 || lineCount == 1) {
        }
        else if (line.rfind("END", 0) == 0) {
        }
        else {
            tradeList.add(createTradeFromLine(line));
        }
        lineCount++;
    }
}

std::vector<ITrade*> FxTradeLoader::loadTrades() {
    FxTradeList tradeList;
    loadTradesFromFile(dataFile_, tradeList);
    
    std::vector<ITrade*> result;
    for (size_t i = 0; i < tradeList.size(); ++i) {
        result.push_back(tradeList[i]);
    }
    return result;
}

std::string FxTradeLoader::getDataFile() const { return dataFile_; }

void FxTradeLoader::setDataFile(const std::string& file) { dataFile_ = file; }