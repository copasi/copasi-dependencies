#include "HostAccessTable.h"
#include <string>
#include <cstdlib>
#include "SBW/SBWApplicationException.h"

using namespace SystemsBiologyWorkbench::Broker;
using namespace std;
HostAccessTable *HostAccessTable::singleton = NULL;

void HostAccessTable::init(void)
{

    // We always accept the local host.  If the user wants to
    // explicitly disallow it (not sure why they'd want to) then they
    // can use a deny pattern.

    acceptAddress("127.0.0.1");

    // TODO: find out how to read localhost ...
    //try
    //{
    //	hostAccess.acceptAddress(InetAddress.getLocalHost().getHostAddress());
    //}
    //catch (UnknownHostException e)
    //{
    //	// Ignore this one; it's impossible in this context.
    //}
    // TODO: get defaults ...
    std::string acceptPattern = "";//Config.getBrokerConnectionsAccept();
    std::string denyPattern = "";//Config.getBrokerConnectionsDeny();

    if (acceptPattern != "")
        acceptAddress(acceptPattern);
    if (denyPattern != "")
        denyAddress(denyPattern);

}

HostAccessTable *HostAccessTable::getTable()
{
  if (HostAccessTable::singleton == NULL)
  {
    HostAccessTable::singleton = new HostAccessTable;
    HostAccessTable::singleton->init();
    std::atexit(&deleteTable);
  }
  return HostAccessTable::singleton;
}

void HostAccessTable::deleteTable()
{
  delete HostAccessTable::singleton;
  HostAccessTable::singleton = NULL;
}

bool HostAccessTable::checkAddress(std::string address)
{
    // accept everything in accept list
    for (unsigned int i = 0; i < acceptAddresses.size(); i++)
    {
        if (address == (std::string) (acceptAddresses[i]))
            return true;
    }
    // accept everything in not in deny list
    for (unsigned int j = 0; j < denyAddresses.size(); j++)
    {
        if (address == (std::string) (denyAddresses[j]))
            return false;
    }

    return true;
}

void HostAccessTable::denyAddress(std::string address)
{
    denyAddresses.push_back(address);
}
void HostAccessTable::acceptAddress(std::string address)
{
    acceptAddresses.push_back(address);
}
bool HostAccessTable::AddressMatcher::matches(std::string address)
{
    throw new SBWApplicationException("not yet implemented");
//	return true;
}
HostAccessTable::AddressMatcher::AddressMatcher(std::string address)
{
    throw new SBWApplicationException("not yet implemented");
}

HostAccessTable::AddressMatcherException::AddressMatcherException(string msg) : SBWException(msg)
{ }
