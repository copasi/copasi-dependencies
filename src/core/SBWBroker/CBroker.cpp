#include "BrokerApplication.h"

using namespace SystemsBiologyWorkbench::Broker;
/**
 * \mainpage SBW C++ Broker
 *
 * <p>This project is the C++ port of the original Java SBW Broker. The aim was to have
 * an easy installable, light-weight broker that handles all communications of SBW. 
 * </p>
 * <p>
 * The original code contained here was initially developed by:
 * <pre>
 *     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
 *     The Systems Biology Workbench Development Group
 *     ERATO Kitano Systems Biology Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *     http://www.cds.caltech.edu/erato
 *     mailto:sysbio-team@caltech.edu </pre></p>
 * 
 * \date 10-19-2004
 * 
 * \author fbergman@kgi.edu
 *
 **/

#include <SBW/SBW.h>
#include <vector> 

using namespace SystemsBiologyWorkbench;
using namespace std;
int main(int argc, char* argv[])
{
	try
	{       
		BrokerApplication broker;
		broker.run(argc,argv);

	}
  catch(SBWException* ex){
#ifdef _DEBUG
    TRACE("Exception: " << ex->getMessage() << "\n" << ex->getDetailedMessage());
#else
    SBW_UNREFERENCED_PARAMETER(ex);
#endif
  }
	catch (...)
	{		
		//
	}
}

