#include "PricingConfigLoader.h"
#include <stdexcept>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

std::string PricingConfigLoader::getConfigFile() const {
    return configFile_;
}

void PricingConfigLoader::setConfigFile(const std::string& file) {
    configFile_ = file;
}

PricingEngineConfig PricingConfigLoader::loadConfig() {
    PricingEngineConfig config;
    std::ifstream stream(configFile_);
    std::stringstream buffer;
    std::string contents;

    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + configFile_);
    }

    // Push filestream into a stringstream
    buffer << stream.rdbuf();

    // Convert to a string to pass to parseXml
    contents = buffer.str();

    // Clean input
    contents.erase(std::remove(contents.begin(), contents.end(), '\r'), contents.end());

    // Now parse XML
    return parseXml(contents);

}

std::string extractAttribute(const std::string& tag, const std::string& attrName) {
    // Define the attribute to search for (name always followed by =")
    std::string search = attrName + "=\"";

    // Finds position of tag
    size_t start = tag.find(search);
    if (start == std::string::npos) {
        // Couldn't find attribute name provided
        throw std::runtime_error("Attribute not found: " + attrName);
    }
    // Move to the start of the value of the attribute (past the pattern)
    start += search.size();

    // Find the end of the tag based on the double quote
    size_t end = tag.find("\"", start);
    if (end == std::string::npos) {
        // Closing quotation mark not found
        throw std::runtime_error("Malformed attribute: " + attrName);
    }

    // Return the string between the start and end (=" <this> ")
    //                                        start ^     end ^
    return tag.substr(start, end - start);

}

PricingEngineConfig PricingConfigLoader::parseXml(const std::string& content) {
    // Define vector for storing configs
    PricingEngineConfig config;

    size_t pos = 0;
    while (true) {
        size_t start = content.find("<Engine", pos);
        if (start == std::string::npos) break;

        size_t end = content.find("/>", start);
        if (end ==std::string::npos) break;

        std::string tag = content.substr(start, end - start);

        PricingEngineConfigItem item;
        // grab each value by tag (tradeType e.g.)
        item.setTradeType(extractAttribute(tag, "tradeType"));
        item.setAssembly(extractAttribute(tag, "assembly"));
        item.setTypeName(extractAttribute(tag, "pricingEngine"));

        config.push_back(item);

        // Update position
        // Must move past XML close symbol ('/>')
        pos = end + 2;
    }

    return config;
}