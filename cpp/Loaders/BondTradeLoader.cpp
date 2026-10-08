#include "BondTradeLoader.h"
#include "../Helpers/StringParsing.cpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

std::unique_ptr<BondTrade>
BondTradeLoader::createTradeFromLine(std::string line) {
  std::vector<std::string> items;
  std::stringstream ss(line);
  std::string item;

  while (std::getline(ss, item, separator)) {
    rm_cntrlchars(item);
    items.push_back(item);
  }

  if (items.size() < 7) {
    throw std::runtime_error("Invalid line format");
  }

  std::unique_ptr<BondTrade> trade =
      std::make_unique<BondTrade>(items[6], items[0]);

  std::tm tm = {};
  std::istringstream dateStream(items[1]);
  dateStream >> std::get_time(&tm, "%Y-%m-%d");
  auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  trade->setTradeDate(timePoint);

  trade->setInstrument(items[2]);
  trade->setCounterparty(items[3]);
  trade->setNotional(std::stod(items[4]));
  trade->setRate(std::stod(items[5]));

  return trade;
}

void BondTradeLoader::loadTradesFromFile(std::string filename,
                                         BondTradeList &tradeList) {
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
    if (lineCount == 0) {
    } else {
      tradeList.add(createTradeFromLine(line));
    }
    lineCount++;
  }
}

std::vector<std::unique_ptr<ITrade>> BondTradeLoader::loadTrades() {
  BondTradeList tradeList;
  loadTradesFromFile(dataFile_, tradeList);

  std::vector<std::unique_ptr<ITrade>> result;
  for (auto &trade : tradeList) {
    result.push_back(std::move(trade));
  }
  return result;
}

std::string BondTradeLoader::getDataFile() const { return dataFile_; }

void BondTradeLoader::setDataFile(const std::string &file) { dataFile_ = file; }
