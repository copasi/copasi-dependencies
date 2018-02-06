#ifndef SBW_HOST_ACCESS_TABLE
#define SBW_HOST_ACCESS_TABLE

#include <vector>
#include "SBW/SBWException.h"

namespace SystemsBiologyWorkbench
{
    namespace Broker
    {
        /**
         * \ingroup C++Broker
         *
         * This class implements a simple host address-based checking scheme,
         * so that certain addresses can be designated as allowed to connect
         * to the local broker.  This is used by the threads that handle incoming
         * module and broker connections (see ConnectionThread.
         * <p>
         * Address patterns must be IPv4 internet addresses such as
         * <code>"123.123.123.123"</code>, optionally using the wildcard character
         * <code>*</code> in place of a block of numbers, e.g.,
         * <code>"123.123.*.*"</code>.
         * <p>
         * The idea employed here is very simple.  The addresses that should be
         * allowed must be added to an internal list using acceptAddress.
         * Addresses that should be explicitly denied must be added using
         * #denyAddress.  These two sets of addresses are kept on separate static
         * lists internally by this class.  When another object needs to check
         * whether a given address is acceptable for a network connection, the
         * object should call #checkAddress, which will return a boolean
         * value of <code>true</code> if the address is both not on the denied list
         * and is on the accept list.
         * <p>
         * This class enforces a singleton design pattern.  Only one instance
         * of HostAccessTable exists at run-time.  This object is created
         * in a static initializer and can be retrieved using #getTable().
         * <p>
         * @see SBWBrokerRPC
         * @see ConnectionThread
         *
         * \date 10-19-2004
         *
         * \author fbergman
         *
         *
         */
        class HostAccessTable
        {

        private:
            /** The list of acceptable addresses. **/
            std::vector<std::string> acceptAddresses;

            /** The list of explicitly denied addresses.  **/
            std::vector<std::string> denyAddresses;

            /** The single instance of this table, created at class loading time.  **/
            static HostAccessTable *singleton;
            /// initialization function
            void init();
            /**
            * Private constructor that suppresses the default public constructor.
            * This, and the following method for returning an instance of this
            * class (#getInstance()), implement the "Singleton" design
            * pattern (named in the Design Patterns book by Gamma et al.).  See
            * <a href="http://developer.java.sun.com/developer/qow/archive/111/">
            * http://developer.java.sun.com/developer/qow/archive/111/</a> for
            * more information.
            **/
            HostAccessTable() { }

        public:
            /**
            * \return the (single) instance of this class.
            **/
            static HostAccessTable *getTable();

      static void deleteTable();

            /**
            * This code is based on similar code in <code>WebServer.java</code>
            * in the Helma XML-RPC implementation by Hannes Wallnoefer.
            * <p>
            * @param address the address to check for permission
            * @return <code>true</code> if the address is allowed
            **/
            bool checkAddress(std::string address);

            /**
            * Add an IP address to the list of accepted clients.  The parameter can
            * contain '*' as wildcard character, e.g., "192.168.*.*".
            * <p>
            * This code is based on similar code in <code>WebServer.java</code>
            * in the Helma XML-RPC implementation by Hannes Wallnoefer.
            * <p>
            * @param address the address to add to the list of accepted addresses
            * @see denyAddress(java.lang.std::string)
            **/
            void acceptAddress(std::string address);

            /**
            * Add an IP address to the list of denied clients. The parameter can
            * contain '*' as wildcard character, e.g. "192.168.*.*".
            * <p>
            * This code is based on similar code in <code>WebServer.java</code>
            * in the Helma XML-RPC implementation by Hannes Wallnoefer.
            * <p>
            * @param address the address to add to the list of accepted addresses
            * @see acceptAddress(java.lang.std::string)
            **/
            void denyAddress(std::string address);
            /**
            * Class that implements an entry in the accept or deny lists
            * for checking addresses.
            *
            * This code is based on similar code in <code>WebServer.java</code>
            * in the Helma XML-RPC implementation by Hannes Wallnoefer.
            **/
            class AddressMatcher
            {
                /** The address pattern for this entry. **/
            public:
                /** Constructor that parses the address and stores it.  **/
                AddressMatcher(std::string address);
                /** Performs a match against the address stored for this instance. **/
                bool matches(std::string address);
            };

            /**
            * Exception class for representing an error in parsing an IP address.
            **/
            class AddressMatcherException : SystemsBiologyWorkbench::SBWException
            {
            public:
                AddressMatcherException(std::string msg);
            };
        };
    }
}
#endif
