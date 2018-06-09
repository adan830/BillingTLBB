#include "billing/Billing.hpp"

#include <iostream>
#include <memory>
#include <thread>

int main(int argc, char* argv[])
{
  std::shared_ptr<Billing> billing(new Billing());

  std::thread billingThread{[billing](){
    std::cout << "Making new thread!" << std::endl;
    billing->start();
  }};

  if (billingThread.joinable())
  {
    billingThread.join();
  }

  std::cout << "Billing is stoped!" << std::endl;

  return 0;
}

