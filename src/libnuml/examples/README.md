
                            l i b N U M L

		     Example programs and numerical data

	    			Joseph O. Dada
        More information about libNUML is available online at
                 http://code.google.com/p/numl/

			    The NUML Team
			 http://code.google.com/p/numl/
		     mailto:josephodada@gmail.com

There are separate subdirectories for different programming languages:
"c" for example programs in the C language, "c++" for examples in C++,
"java" for examples in Java, etc.  You will first need to change your
working directory to one of these subdirectories.

To build the examples, please configure with the option `-DWITH_EXAMPLES=ON`.

Next, you may wish to try running the example programs on some NUML
files.  There are a number of sample NuML/numerical dada provided for this purpose
in the "example-data" subdirectory here.  So for example, you could
do try the following (if you were in the "c++" subdirectory):

  % printNUML     ../example-data/TwoDimensionNuMLData.xml
  % readNUML      ../example-data/TwoDimensionNuMLData.xml
  % createNUML 
  % echoNUML  ../example-data/TwoDimensionNuMLData.xml twoDimensionData.xml
  %

More example data may be obtained from a number of other sources, including
the COPASI Web services (CopasiWS: http://www.comp-sys-bio.org/CopasiWS/).

