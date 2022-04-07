// TrigPlusServerModule.cpp 
//

#include <math.h>
#include "SBW/SBW.h"

using namespace SystemsBiologyWorkbench ;
using namespace std;
/*
* Trignometry
* this class provides an implementation of the Trignometry service
*/
class Trignometry
{
public :
	/// this method provides the implementation of an SBW method
	///  - in this case the sin method
	/// arguments:	from - module calling this method
	///				reader - object containing argument data
	/// returns:	object containing result data
	DataBlockWriter sin(Module from, DataBlockReader reader)
	{
		Double arg ;

		// extract double argument from argument data
		reader >> arg ;
		
		// create result data object, calculate result, store result in result object
		// and return result object
		return DataBlockWriter() << (Double) ::sin(arg);
	}

	/// this method provides the implementation of an SBW method
	/// - in this case the cos method
	/// arguments:	from - module calling this method
	///				reader - object containing argument data
	/// returns:	object containing result data
	DataBlockWriter cos(Module from, DataBlockReader reader)
	{
		Double arg ;

		// extract double argument from argument data
		reader >> arg ;
		
		// create result data object, calculate result, store result in result object
		// and return result object
		return DataBlockWriter() << (Double) ::cos(arg);
	}

	/**
	 * register methods of this class with a module implementation represented by a given MethodTable
	 * argument - table object that provides a simple interface to a module implementation
	 */
	static void registerMethods(MethodTable<Trignometry> &table)
	{
		table.addMethod(&Trignometry::sin, "double sin(double)");
		table.addMethod(&Trignometry::cos, "double cos(double)");
	}
};

/*
* module entry point WinMain/main
* On windows using WinMain as an entry point means that the executable can be an
* invisible process rather than a console application.
* arguments: in windows case all are ignored, see windows documentation
* for details of arguments.  In linux case standard main arguments
* result: int : -1 if error occurred 0 otherwise.
*/
int main(int argc, char* argv[])
{

	try
	{
		// in the following calls help text is an optional argument which has not
		// been supplied in this example

		ModuleImpl modImpl(
			"Trig", // module identification
			"trig written in C++",  // humanly readable name
			SelfManagedModule); // management scheme

		modImpl.addServiceObject(
			"trig",					// service identification
			"trig written in C++",	// humanly readable name
			"trig",					// category
			new Trignometry());     // service implementation
		//modImpl.runOnHost("orsono.acad.kgi.edu");
		// connect to broker providing services
		modImpl.run(argc, argv);
	}
	catch (SBWException *e)
	{
		printf("TrigPlus exception: %s\n", e->getMessage().c_str());
		return -1;
	}

	return 0;
}

