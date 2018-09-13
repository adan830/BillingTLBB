#include "billing/Billing.hpp"

#include "billing/Startup.hpp"

#include <iostream>
#include <memory>

#if !defined(__WIN32) && !defined(WIN32)
#  include <sys/unistd.h>
#  include <sys/types.h>
#endif


int main(int argc, char* argv[])
{
#if !defined(__WIN32) && !defined(WIN32)
  if (::getuid() != 0)
  {
    std::cerr << "Must be run as root" << std::endl;
    return 1;
  }
#endif
  try
  {
    {
      std::make_shared<Startup>(argc, argv)->start();
    }

    std::make_shared<Billing>()->start();
  }
  catch (const std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
  catch (...)
  {
    std::cerr << "Exeption error" << std::endl;
    return 1;
  }

  std::cout << "Billing is stoped!" << std::endl;

  return 0;
}

