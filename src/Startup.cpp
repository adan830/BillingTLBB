#include "billing/Startup.hpp"

#include "billing/Log.hpp"

Startup::Startup(const int argc, char* argv[])
{
  LOG->info("Startup is constructing with {} param(s)", argc);

  LOG->info("Startup is constructed");
}

Startup::~Startup()
{
  LOG->info("Startup is destructing...");

  LOG->info("Startup is destructed");
}

void Startup::start()
{
}

