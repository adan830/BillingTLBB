#ifndef __BILLING_CONFIG_HPP__
#define __BILLING_CONFIG_HPP__

#include <string>

class Config final
{
  private:
    struct Data
    {
      // Listen IP
      std::string ip;

      // Listen Port
      unsigned short port;

      // Web Database Info
      std::string webDatabaseIp;
      unsigned short webDatabasePort;
      std::string webDatabaseName;
      std::string webDatabaseUsername;
      std::string webDatabasePassword;

      // Other
      std::size_t maxAccountPerIp;
      std::size_t maxAccountPerMac;
      std::size_t maxAccountPerHardware;
    };

  private:
    std::string m_configFile;
    Data* m_data;

  private:
    Config();
    ~Config();

  public:
    void readData();

  public:
    static Config& getInstance();
    const Data* getData() const;
};

#endif

