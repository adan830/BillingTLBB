#ifndef __BILLING_CONFIG_HPP__
#define __BILLING_CONFIG_HPP__

#include <string>

class Config final
{
  public:
    struct Data
    {
      // Listen IP
      std::string ip;

      // Listen Port
      short port;

      // Web Database Info
      std::string webDatabaseIp;
      short webDatabasePort;
      std::string webDatabaseName;
      std::string webDatabaseUsername;
      std::string webDatabasePassword;

      // Game Database Info
      std::string gameDatabaseIp;
      short gameDatabasePort;
      std::string gameDatabaseName;
      std::string gameDatabaseUserName;
      std::string gameDatabasePassword;

      // Constructor
      Data();
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

