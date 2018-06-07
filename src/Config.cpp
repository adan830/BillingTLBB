#include "billing/Config.hpp"

#include <iostream>
#include <fstream>

Config::Data::Data(const std::string& filename)
{
  std::cout << "Parsing " << filename.data() << std::endl;

  std::ifstream ifConfig(filename, std::ios::in);

  if (!ifConfig.is_open())
  {
    std::cout << "Parse " << filename.data() << " error" << std::endl;
  }
  else
  {
    std::string line;
    while(std::getline(ifConfig, line))
    {
    }
    std::cout << "Parsed " << filename.data() << std::endl;
  }
}

Config::Config() :
  m_configFile("config.ini"),
  m_data(new Config::Data(m_configFile))
{
}

Config::~Config()
{
  delete m_data;
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

