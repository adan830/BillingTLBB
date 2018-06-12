#include "billing/Billing.hpp"

#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
  try
  {
    std::shared_ptr<Billing>(new Billing())->start();
  }
  catch (...)
  {
    std::cerr << "Exeption error" << std::endl;
    return 1;
  }

  std::cout << "Billing is stoped!" << std::endl;

  return 0;
}

