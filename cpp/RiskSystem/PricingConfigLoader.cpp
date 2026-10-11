#include "PricingConfigLoader.h"
#include "PricingEngineConfigItem.h"
#include "pugixml.hpp"

std::string PricingConfigLoader::getConfigFile() const { return configFile_; }

void PricingConfigLoader::setConfigFile(const std::string &file) {
  configFile_ = file;
}

PricingEngineConfig PricingConfigLoader::loadConfig() {
  // Make the PricingEngineConfig
  PricingEngineConfig config;
  // Open the file and read it
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(configFile_.c_str());

  if (!result) {
    throw std::runtime_error("Failed to load " + configFile_ + ": " +
                             result.description());
  }

  // then for each entry create a PricingConfigItem
  pugi::xml_node root = doc.child("PricingEngines");
  for (pugi::xml_node engine : root.children("Engine")) {
    PricingEngineConfigItem item;
    item.setTradeType(engine.attribute("tradeType").as_string());
    item.setAssembly(engine.attribute("assembly").as_string());
    item.setTypeName(engine.attribute("pricingEngine").as_string());
    config.push_back(std::move(item));
  }

  return config;
}
