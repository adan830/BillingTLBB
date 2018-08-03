#include "billing/Config.hpp"

#include "billing/Log.hpp"

#include <sstream>
#include <fstream>

Config::Config() :
  m_configFile("Config.ini"),
  m_data(new Config::Data())
{
  this->readData();
}

Config::~Config()
{
  LOG->warning("Config is destructing...");

  delete m_data;

  LOG->warning("Config is destructed!");
}

void Config::readData()
{
  LOG->info("Parsing file {}", m_configFile);

  std::ifstream ifConfig(m_configFile, std::ifstream::in);

  if (!ifConfig.is_open())
  {
    LOG->info("Parse file {} error", m_configFile);
  }
  else
  {
    std::string line;
    std::string configKey;
    std::string configValue;
    char operatorChar;
    while(std::getline(ifConfig, line))
    {
      if (line.empty())
      {
        continue;
      }

      if (line.find(';') == 0)
      {
        continue;
      }

      if (line.find('#') == 0)
      {
        continue;
      }

      std::stringstream(line) >> configKey >> operatorChar >> configValue;

      if (configKey.empty())
      {
        continue;
      }

      if (configValue.empty())
      {
        LOG->info("configKey {} is empty", configKey);
        continue;
      }

      if (operatorChar != '=')
      {
        LOG->info("Invalid operator {}", operatorChar);
        continue;
      }

      LOG->warning("LineInfo: {}={}", configKey, configValue);

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
        m_data->webDatabaseIp = configValue;
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
#if defined(__BILLING_WITH_ANTI_CLONE_MAC__)
      else if (configKey == "MAX_ACCOUNT_PER_MAC")
      {
        m_data->maxAccountPerMac = std::stoi(configValue);
      }
#endif
#if defined(__BILLING_WITH_ANTI_CLONE_HARDWARE__)
      else if (configKey == "MAX_ACCOUNT_PER_HARDWARE")
      {
        m_data->maxAccountPerHardware = std::stoi(configValue);
      }
#endif
      else if (configKey == "WEB_DATABASE_POINT_COLUMN_NAME")
      {
        m_data->webDatabasePointColumnName = configValue;
      }
      else
      {
        LOG->warning("configKey: {} is not a valid key", configKey);
      }

      configKey.clear();
      configValue.clear();
    }

    LOG->warning("Parsed file {}", m_configFile);

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

