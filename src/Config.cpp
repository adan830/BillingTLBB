#include "billing/Config.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

Config::Config() :
  m_configFile("config.ini"),
  m_data(new Config::Data())
{
  this->readData();
}

Config::~Config()
{
  std::cout << "Config is destructing..." << std::endl;

  delete m_data;

  std::cout << "Config is destructed!" << std::endl;
}

void Config::readData()
{
  std::cout << "Parsing file " << m_configFile.data() << std::endl;

  std::ifstream ifConfig(m_configFile, std::ios::in);

  if (!ifConfig.is_open())
  {
    std::cout << "Parse file " << m_configFile.data() << " error" << std::endl;
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
        std::cout << "Key " << configKey << " is empty!!!" << std::endl;
        continue;
      }

      std::cout << "LineInfo: " << configKey << tempChar << configValue
      << std::endl;

      if (configKey == "IP")
      {
        m_data->ip = m_data->ip.empty() ? configValue : m_data->ip;
      }
      else if (configKey == "LISTEN_PORT")
      {
        m_data->port = std::stoi(configValue);
      }
      else if (configKey == "WEB_DATABASE_IP")
      {
        m_data->webDatabaseIp = configKey;
      }
      else if (configKey == "WEB_DATABASE_PORT")
      {
        m_data->webDatabasePort = std::stoi(configValue);
      }
      else if (configKey == "WEB_DATABASE_NAME")
      {
        m_data->webDatabaseName = configValue;
      }
      else if (configKey == "WEB_DATABASE_USERNAME")
      {
        m_data->webDatabaseUsername = configValue;
      }
      else if (configKey == "WEB_DATABASE_PASSWORD")
      {
        m_data->webDatabasePassword = configValue;
      }
      else if (configKey == "GAME_DATABASE_IP")
      {
        m_data->gameDatabaseIp = configValue;
      }
      else if (configKey == "GAME_DATABASE_PORT")
      {
        m_data->gameDatabasePort = std::stoi(configValue);
      }
      else if (configKey == "GAME_DATABASE_NAME")
      {
        m_data->gameDatabaseName = configValue;
      }
      else if (configKey == "GAME_DATABASE_USERNAME")
      {
        m_data->gameDatabaseUserName = configValue;
      }
      else if (configKey == "GAME_DATABASE_PASSWORD")
      {
        m_data->gameDatabasePassword = configValue;
      }
      else
      {
        std::cout << "Config::readData Waring: " << configKey
        << " is not a valid key"
        << std::endl;
      }

      configKey.clear();
      configValue.clear();
    }

    std::cout << "Parsed file " << m_configFile.data() << std::endl;

    ifConfig.close();
  }
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

