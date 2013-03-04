#ifndef SBW_BROKER_HOST
#define SBW_BROKER_HOST

#include <string>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{

		/**
		 * \ingroup C++Broker
		 *
		 * Instances of this class are used to identify a host computer.  The need
		 * for this arose because the Java implementation on some platforms (e.g.,
		 * Windows) cannot always resolve host names properly.  An example scenario
		 * is the following.  A Windows machine may have a Windows name such as
		 * "HAPPY", but at the same time, the machine may be connected to a TCP/IP
		 * network using DHCP.  The name assigned to the host by DHCP may be
		 * completely different from "HAPPY", say "dhcp-205.cds.caltech.edu".  This
		 * leads to problems with users invoking SBW calls with short names such as
		 * "HAPPY".  The forward mapping works normally (i.e., the Java calls can
		 * resolve "HAPPY" to an IP address), but reverse mappings (on other hosts)
		 * may turn up the alternative name "dhcp-205.cds.caltech.edu".  Now, this
		 * in itself is not a problem; the problem arises because SBW has to track
		 * on which hosts the different modules and Brokers are running.  If users
		 * may request modules using multiple host names for the same physical
		 * computer, then SBW has to have a host name-independent way of tracking
		 * hosts.  The obvious way to do this is to use IP addresses (which is
		 * effectively what SBW uses internally), but users expect to see the host
		 * names that they are familiar with when SBW Brokers return answers to
		 * queries.  Users don't want to request a module descriptor for
		 * "HAPPY:myModule" and get back something that claims the module name is
		 * "dhcp-205:myModule" or even "131.252.234.143:myModule".  So, to record
		 * both the name given to a host by a user and the host's IP address, as
		 * well as the fully qualified host name, the SBW Broker internally refers
		 * to hosts using host objects of class {@link Host}.
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class Host
		{
		private:
			/** The host name used in a query; typically something a user supplied. **/
			std::string givenHostName;

			/** The fully qualified domain name. **/
			std::string fullHostName;

			/** The host IPv4 address as a string; e.g., "123.123.123.123". **/
			std::string hostAddressString;

			/** experimental ... port number of the broker to connect to **/
			int m_nPort;
		public:
			/**
			* Class constructor.  This takes a host name and constructs an object
			* that internally records the name, the fully qualified name, the
			* IP address, and possibly other data.
			* <p>
			* @param name the name of the host
			**/
			Host(std::string  name);

			/**
			* Class constructor.  This takes a host name and IP address and
			* constructs an object that internally records the name, the fully
			* qualified name, the IP address, and possibly other data.
			* <p>
			* @param name the name of the host
			**/
			Host(std::string name, std::string address);
			Host(std::string name, std::string address, int nPort);
			/**
			* Returns the name of this host.
			**/
			std::string getName();
			std::string getClearName();
			void setName(std::string sName) { givenHostName = sName;}
			/**
			* Returns the fully qualified domain name of this host.
			**/
			std::string getFullName();

			/**
			* Returns this host's IP address as a string.
			**/
			std::string getAddress();
			void setAddress(std::string sAddress) { hostAddressString = sAddress; }
			/**
			* Indicates whether some other object is "equal to" this one.  This
			* method implements equality for Host objects,
			* overriding the method of the same name on the basic Java 
			* Object class.  If the given object is a Host
			* object and its various fields are equal to this object's,
			* then this method returns true, else it returns false.
			* <p>
			* @param other the object with which to compare this one.
			* @return true if the argument is a Host object that
			* refers to the same module as this object, false otherwise.
			**/
			bool equals(Host *other);

			/**
			 * For the case that the Broker should be connected to another broker, so far
			 * the connection only worked if the broker ran on the same port ... 
			 * Now it is possible to specify the port in the linkBroker program by giving
			 * <ip>/<port> or <address>/<port>. 
			 * \return the port of the given address ... or -1 if none was given
			 */
			int getPort();
			/* during connection phase of two brokers the port should be exchanged
			 * this will be done using this method.
			 * @param nPort the port for this broker
			 */
			void setPort(int nPort);
		};

	}
}

#endif

