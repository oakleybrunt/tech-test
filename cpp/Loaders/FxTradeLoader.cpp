#include "FxTradeLoader.h"
#include "../Helpers/StringParsing.cpp"
#include "../Models/FxTradeList.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::unique_ptr<FxTrade> FxTradeLoader::createTradeFromLine(std::string line) {
  std::vector<std::string> items;
  std::stringstream ss(line);
  std::string item;

  while (std::getline(ss, item, separator)) {
    rm_nonprinting(item);
    items.push_back(item);
  }

  if (items.size() < 9) {
    throw std::runtime_error("Invalid line format");
  }

  std::unique_ptr<FxTrade> trade =
      std::make_unique<FxTrade>(items[8], items[0]);

  std::tm tm = {};
  std::istringstream tradeDateStream(items[1]);
  tradeDateStream >> std::get_time(&tm, "%Y-%m-%d");
  auto tradeTimePoint =
      std::chrono::system_clock::from_time_t(std::mktime(&tm));
  trade->setTradeDate(tradeTimePoint);

  std::tm tm2 = {};
  std::istringstream valueDateStream(items[6]);
  valueDateStream >> std::get_time(&tm2, "%Y-%m-%d");
  auto valueTimePoint =
      std::chrono::system_clock::from_time_t(std::mktime(&tm2));
  trade->setValueDate(valueTimePoint);

  trade->setInstrument(items[2] + items[3]);
  trade->setCounterparty(items[7]);
  trade->setNotional(std::stod(items[4]));
  trade->setRate(std::stod(items[5]));

  return trade;
}

void FxTradeLoader::loadTradesFromFile(std::string filename,
                                       ITradeReceiver &tradeList) {
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
    } else if (line.find("END") != std::string::npos) {
    } else {
      tradeList.add(createTradeFromLine(line));
    }
    lineCount++;
  }
}

std::vector<std::unique_ptr<ITrade>> FxTradeLoader::loadTrades() {
  FxTradeList tradeList;
  loadTradesFromFile(dataFile_, tradeList);

  std::vector<std::unique_ptr<ITrade>> result;
  for (auto &trade : tradeList) {
    result.push_back(std::move(trade));
  }
  return result;
}

void FxTradeLoader::streamTrades(ITradeReceiver &receiver) {
  loadTradesFromFile(dataFile_, receiver);
}

std::string FxTradeLoader::getDataFile() const { return dataFile_; }

void FxTradeLoader::setDataFile(const std::string &file) { dataFile_ = file; }
