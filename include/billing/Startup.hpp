#ifndef __BILLING_STARTUP_HPP__
#define __BILLING_STARTUP_HPP__

class Startup final
{
  public:
    Startup(const int argc, char* argv[]);
    ~Startup();

  public:
    void start();

  private:
    void stop();
};

#endif

