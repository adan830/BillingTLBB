#ifndef __BILLING_BILLING_HPP__
#define __BILLING_BILLING_HPP__

class BillingSocket;

class Billing final
{
  private:
    BillingSocket* m_socket;

  public:
    Billing();
    ~Billing();

  public:
    // On first
    void start();

    /**
     *
     */
    void restart();
};


#endif

