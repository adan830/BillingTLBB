#include "billing/Billing.hpp"

#include <iostream>
#include <memory>

int main(int argc, char* argv[])
{
  std::make_shared<Billing>()->start();

  std::cout << "Billing is stoped!" << std::endl;

  return 0;
}

