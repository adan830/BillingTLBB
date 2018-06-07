#include "billing/Config.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

Config::Data::Data(const std::string& filename)
{
  this->ip = "0.0.0.0";
  this->port = 12680;

  std::cout << "Parsing file " << filename.data() << std::endl;

  std::ifstream ifConfig(filename, std::ios::in);

  if (!ifConfig.is_open())
  {
    std::cout << "Parse file " << filename.data() << " error" << std::endl;
  }
  else
  {
    std::string line;
    std::string configKey;
    std::string configValue;
    char tempChar;
    while(std::getline(ifConfig, line))
    {
      if (line.empty())
      {
        continue;
      }

      std::stringstream(line) >> configKey >> tempChar >> configValue;

      if (configKey.empty())
      {
        continue;
      }

      if (configValue.empty())
      {
        std::cout << "Key " << configKey << " is empty!!!";
        continue;
      }

      std::cout << "LineInfo: "
      << configKey
      << tempChar
      << configValue
      << std::endl;

      if (configKey == "IP")
      {
        this->ip = configValue;
      }
      else if (configKey == "LISTEN_PORT")
      {
        this->port = std::stoi(configValue);
      }
      else
      {
        std::cout
        << "Config::Data Waring: "
        << configKey
        << " is not a valid key"
        << std::endl;
      }
    }

    std::cout << "Parsed file " << filename.data() << std::endl;

    ifConfig.close();
  }
}

Config::Config() :
  m_configFile("config.ini"),
  m_data(new Config::Data(m_configFile))
{
}

Config::~Config()
{
  std::cout << "Config is destructing..." << std::endl;

  delete m_data;

  std::cout << "Config is destructed!" << std::endl;
}

Config& Config::getInstance()
{
  static Config s_config;
  return s_config;
}

const Config::Data* Config::getData() const
{
  return m_data;
}

