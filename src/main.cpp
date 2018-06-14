#include "billing/Billing.hpp"

#include "billing/Startup.hpp"

#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
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

