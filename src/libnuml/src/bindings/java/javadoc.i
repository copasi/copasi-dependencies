%javamethodmodifiers getLibNUMLVersion "
/**
 * Returns the libNUML version as an integer: version 1.2.3 becomes 10203.
 * <p>
 * @return the libNUML version as an integer: version 1.2.3 becomes 10203.
 */
 public";

%javamethodmodifiers getLibNUMLDottedVersion "
/**
 * Returns the libNUML version as a string of the form \'1.2.3\'.
 * <p>
 * @return the libNUML version as a string of the form \'1.2.3\'.
 */
 public";

%javamethodmodifiers getLibNUMLVersionString "
/**
 * Returns the libNUML version as a string: version 1.2.3 becomes \'10203\'.
 * <p>
 * @return the libNUML version as a string: version 1.2.3 becomes \'10203\'.
 */
 public";

%javamethodmodifiers LIBNUML_EXTERN NUMLReader::NUMLReader "
  /**
   * Creates a new {@link NUMLReader} and returns it.
   */
 public";

%javamethodmodifiers LIBNUML_EXTERN NUMLReader::readNUML(const std::string& filename) "
  /**
   * Reads an NUML document from a file.
   * <p>
   * If the file named <code>filename</code> does not exist or its content is not
   * valid NUML, one or more errors will be logged with the {@link NUMLDocument}
   * object returned by this method.  Callers can use the methods on
   * {@link NUMLDocument} such as NUMLDocument.getNumErrors() and
   * {@link NUMLDocument#getError(long n)}  to get the errors.  The object returned by
   * {@link NUMLDocument#getError(long n)}  is an {@link NUMLError} object, and it has methods to
   * get the error code, category, and severity level of the problem, as
   * well as a textual description of the problem.  The possible severity
   * levels range from informational messages to fatal errors; see the
   * documentation for {@link NUMLError} for more information.
   * <p>
   * If the file <code>filename</code> could not be read, the file-reading error will
   * appear first.  The error code can provide a clue about what happened.
   * For example, a file might be unreadable (either because it does not
   * actually exist or because the user does not have the necessary access
   * priviledges to read it) or some sort of file operation error may have
   * bee reported by the underlying operating system.  Callers can check
   * for these situations using code such as the following:
   * <div class=\'fragment\'><pre>
   * {@link NUMLReader}  reader = new {@link NUMLReader}();
   * {@link NUMLDocument}  doc  = reader.readNUML(filename);
   * 
   * if (doc-&gt;getNumErrors() &gt; 0)
   * {
   *   if (doc-&gt;getError(0)-&gt;getId() == XMLError.FileUnreadable)
   *   {
   *     // Handle case of unreadable file here.
   *   } 
   *   else if (doc-&gt;getError(0)-&gt;getId() == XMLError.FileOperationError)
   *   {
   *     // Handle case of other file error here.
   *   }
   *   else
   *   {
   *     // Handle other cases -- see error codes defined in XMLErrorCode_t
   *     // for other possible cases to check.
   *   }
   * }</pre></div>
   * <p>
   * If the given filename ends with the suffix <code>\'</code>.gz\' (for example, 
   * <code>\'myfile</code>.xml.gz\'), the file is assumed to be compressed in <em>gzip</em>
   * format and will be automatically decompressed upon reading.
   * Similarly, if the given filename ends with <code>\'</code>.zip\' or <code>\'</code>.bz2\', the
   * file is assumed to be compressed in <em>zip</em> or <em>bzip2</em> format
   * (respectively).  Files whose names lack these suffixes will be read
   * uncompressed.  Note that if the file is in <em>zip</em> format but the
   * archive contains more than one file, only the first file in the
   * archive will be read and the rest ignored.
   * <p>
   * @param filename the name or full pathname of the file to be read.
   * <p>
   * @return a pointer to the {@link NUMLDocument} created from the NUML content.
   * <p>
   * @see NUMLError
   * <p>
   * @note LibNUML versions 2.x and 3.x behave differently in error
   * handling in several respects.  One difference is how early some errors
   * are caught and whether libNUML continues processing a file in the face
   * of some early errors.  In general, libNUML 3.x stops parsing NUML
   * inputs sooner than libNUML 2.x in the face of XML errors because the
   * errors may invalidate any further NUML content.  For example, a
   * missing XML declaration at the beginning of the file was ignored by
   * libNUML 2.x but in version 3.x, it will cause libNUML to stop parsing
   * the rest of the input altogether.  While this behavior may seem more
   * severe and intolerant, it was necessary in order to provide uniform
   * behavior regardless of which underlying XML parser (Expat, Xerces,
   * libxml2) is being used by libNUML.  The XML parsers themselves behave
   * differently in their error reporting, and sometimes libNUML has to
   * resort to the lowest common denominator.
   * <p>
   * @note To read a gzip/zip file, libNUML needs to be configured and
   * linked with the <a href=\'http://www.zlib.net/\'>zlib</a> library at
   * compile time.  It also needs to be linked with the <a
   * href=\'\'>bzip2</a> library to read files in <em>bzip2</em> format.  (Both of
   * these are the default configurations for libNUML.)  Errors about
   * unreadable files will be logged if a compressed filename is given and
   * libNUML was <em>not</em> linked with the corresponding required library.
   * <p>
   * @note NUMLReader.hasZlib() and NUMLReader.hasBzip2() can be used to
   * check whether libNUML has been linked with each library.
   */
 public";

%javamethodmodifiers LIBNUML_EXTERN NUMLReader::readNUMLFromFile(const std::string& filename) "
  /**
   * Reads an NUML document from a file.
   * <p>
   * If the file named <code>filename</code> does not exist or its content is not
   * valid NUML, one or more errors will be logged with the {@link NUMLDocument}
   * object returned by this method.  Callers can use the methods on
   * {@link NUMLDocument} such as NUMLDocument.getNumErrors() and
   * {@link NUMLDocument#getError(long n)}  to get the errors.  The object returned by
   * {@link NUMLDocument#getError(long n)}  is an {@link NUMLError} object, and it has methods to
   * get the error code, category, and severity level of the problem, as
   * well as a textual description of the problem.  The possible severity
   * levels range from informational messages to fatal errors; see the
   * documentation for {@link NUMLError} for more information.
   * <p>
   * If the file <code>filename</code> could not be read, the file-reading error will
   * appear first.  The error code can provide a clue about what happened.
   * For example, a file might be unreadable (either because it does not
   * actually exist or because the user does not have the necessary access
   * priviledges to read it) or some sort of file operation error may have
   * bee reported by the underlying operating system.  Callers can check
   * for these situations using code such as the following:
   * <div class=\'fragment\'><pre>
   * {@link NUMLReader}  reader = new {@link NUMLReader}();
   * {@link NUMLDocument}  doc  = reader.readNUML(filename);
   * 
   * if (doc-&gt;getNumErrors() &gt; 0)
   * {
   *   if (doc-&gt;getError(0)-&gt;getId() == XMLError.FileUnreadable)
   *   {
   *     // Handle case of unreadable file here.
   *   } 
   *   else if (doc-&gt;getError(0)-&gt;getId() == XMLError.FileOperationError)
   *   {
   *     // Handle case of other file error here.
   *   }
   *   else
   *   {
   *     // Handle other cases -- see error codes defined in XMLErrorCode_t
   *     // for other possible cases to check.
   *   }
   * }</pre></div>
   * <p>
   * If the given filename ends with the suffix <code>\'</code>.gz\' (for example, 
   * <code>\'myfile</code>.xml.gz\'), the file is assumed to be compressed in <em>gzip</em>
   * format and will be automatically decompressed upon reading.
   * Similarly, if the given filename ends with <code>\'</code>.zip\' or <code>\'</code>.bz2\', the
   * file is assumed to be compressed in <em>zip</em> or <em>bzip2</em> format
   * (respectively).  Files whose names lack these suffixes will be read
   * uncompressed.  Note that if the file is in <em>zip</em> format but the
   * archive contains more than one file, only the first file in the
   * archive will be read and the rest ignored.
   * <p>
   * @param filename the name or full pathname of the file to be read.
   * <p>
   * @return a pointer to the {@link NUMLDocument} created from the NUML content.
   * <p>
   * @see NUMLError
   * <p>
   * @note LibNUML versions 2.x and 3.x behave differently in error
   * handling in several respects.  One difference is how early some errors
   * are caught and whether libNUML continues processing a file in the face
   * of some early errors.  In general, libNUML 3.x stops parsing NUML
   * inputs sooner than libNUML 2.x in the face of XML errors because the
   * errors may invalidate any further NUML content.  For example, a
   * missing XML declaration at the beginning of the file was ignored by
   * libNUML 2.x but in version 3.x, it will cause libNUML to stop parsing
   * the rest of the input altogether.  While this behavior may seem more
   * severe and intolerant, it was necessary in order to provide uniform
   * behavior regardless of which underlying XML parser (Expat, Xerces,
   * libxml2) is being used by libNUML.  The XML parsers themselves behave
   * differently in their error reporting, and sometimes libNUML has to
   * resort to the lowest common denominator.
   * <p>
   * @note To read a gzip/zip file, libNUML needs to be configured and
   * linked with the <a href=\'http://www.zlib.net/\'>zlib</a> library at
   * compile time.  It also needs to be linked with the <a
   * href=\'\'>bzip2</a> library to read files in <em>bzip2</em> format.  (Both of
   * these are the default configurations for libNUML.)  Errors about
   * unreadable files will be logged if a compressed filename is given and
   * libNUML was <em>not</em> linked with the corresponding required library.
   * <p>
   * @note NUMLReader.hasZlib() and NUMLReader.hasBzip2() can be used to
   * check whether libNUML has been linked with each library.
   */
 public";

%javamethodmodifiers LIBNUML_EXTERN NUMLReader::readNUMLFromString(const std::string& xml) "
  /**
   * Reads an NUML document from the given XML string.
   * <p>
   * This method is flexible with respect to the presence of an XML
   * declaration at the beginning of the string.  In particular, if the
   * string in <code>xml</code> does not begin with the XML declaration
   * <code>&lt;?xml version=\'1.0\' encoding=\'UTF-8\'?&gt;</code>, then this
   * method will automatically prepend the declaration to <code>xml</code>.
   * <p>
   * This method will log a fatal error if the content given in the
   * parameter <code>xml</code> is not NUML.  See the method documentation for
   * {@link NUMLReader#readNUML(String filename)}  for an example of code for testing the returned
   * error code.
   * <p>
   * @param xml a string containing a full NUML model
   * <p>
   * @return a pointer to the {@link NUMLDocument} created from the NUML content.
   * <p>
   * @see NUMLReader#readNUML(String filename) 
   */
 public";

%javamethodmodifiers LIBNUML_EXTERN NUMLReader::hasZlib "
  /**
   * Predicate returning <code>true</code> or <code>false</code> depending on whether
   * libNUML is linked with zlib at compile time.
   * <p>
   * @return <code>true</code> if zlib is linked, <code>false</code> otherwise.
   */
 public";

%javamethodmodifiers LIBNUML_EXTERN NUMLReader::hasBzip2 "
  /**
   * Predicate returning <code>true</code> or <code>false</code> depending on whether
   * libNUML is linked with bzip2 at compile time.
   * <p>
   * @return <code>true</code> if bzip2 is linked, <code>false</code> otherwise.
   */
 public";

%javamethodmodifiers LIBNUML_EXTERN NUMLReader::readInternal(const char* content, bool isFile = true) "
  /**
   * Used by readNUML() and readNUMLFromString().
   * <p>
   * @docnote The native C++ implementation of this method defines a
   * default argument value.  In the documentation generated for different
   * libNUML language bindings, you may or may not see corresponding
   * arguments in the method declarations.  For example, in Java, a default
   * argument is handled by declaring two separate methods, with one of
   * them having the argument and the other one lacking the argument.
   * However, the libNUML documentation will be <em>identical</em> for both
   * methods.  Consequently, if you are reading this and do not see an
   * argument even though one is described, please look for descriptions of
   * other variants of this method near where this one appears in the
   * documentation.
   <p>
 * @deprecated libNUML internal
 */
 public";

%javamethodmodifiers readNUML(const char *filename) "
/**
 * Reads an NUML document from the given file <code>filename</code>.
 * <p>
 * If <code>filename</code> does not exist, or it is not an NUML file, an error will
 * be logged in the error log of the {@link NUMLDocument} object returned by this
 * method.  Calling programs can inspect this error log to determine
 * the nature of the problem.  Please refer to the definition of
 * {@link NUMLDocument} for more information about the error reporting mechanism.
 * <p>
 * @return a pointer to the {@link NUMLDocument} read.
 */
 public";

%javamethodmodifiers readNUMLFromFile(const char *filename) "
/**
 * Reads an NUML document from the given file <code>filename</code>.
 * <p>
 * If <code>filename</code> does not exist, or it is not an NUML file, an error will
 * be logged in the error log of the {@link NUMLDocument} object returned by this
 * method.  Calling programs can inspect this error log to determine
 * the nature of the problem.  Please refer to the definition of
 * {@link NUMLDocument} for more information about the error reporting mechanism.
 * <p>
 * @return a pointer to the {@link NUMLDocument} read.
 */
 public";

%javamethodmodifiers readNUMLFromString(const char *xml) "
/**
 * Reads an NUML document from a string assumed to be in XML format.
 * <p>
 * If the string does not begin with XML declaration,
 *<div class=\'fragment\'><pre>
&lt;?xml version=\'1.0\' encoding=\'UTF-8\'?&gt;
</pre></div>
 * <p>
 * an XML declaration string will be prepended.
 * <p>
 * This method will report an error if the given string <code>xml</code> is not NUML.
 * The error will be logged in the error log of the {@link NUMLDocument} object
 * returned by this method.  Calling programs can inspect this error log to
 * determine the nature of the problem.  Please refer to the definition of
 * {@link NUMLDocument} for more information about the error reporting mechanism.
 * <p>
 * @return a pointer to the {@link NUMLDocument} read.
 */
 public";

