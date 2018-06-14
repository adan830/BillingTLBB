#include "billing/Log.hpp"

Log::Log()
{

}

Log& Log::getInstance()
{
  static Log m_instance;
  return m_instance;
}

