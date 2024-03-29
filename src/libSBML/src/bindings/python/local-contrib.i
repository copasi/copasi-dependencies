/**
 * @file    local-contrib.i
 * @brief   Contributed class that makes using libSBML from python more convenient
 *          for the time being this only works for python 2.x, but won't break python 3
 * @author  Gordon Ball
 * @author  Brett G. Olivier
 *
 *<!---------------------------------------------------------------------------
 * This file is part of libSBML.  Please visit http://sbml.org for more
 * information about SBML, and the latest version of libSBML.
 *
 * Copyright (C) 2020 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *     3. University College London, London, UK
 *
 * Copyright (C) 2019 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2013-2018 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *     3. University of Heidelberg, Heidelberg, Germany
 *
 * Copyright (C) 2009-2013 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. EMBL European Bioinformatics Institute (EMBL-EBI), Hinxton, UK
 *
 * Copyright (C) 2006-2008 by the California Institute of Technology,
 *     Pasadena, CA, USA
 *
 * Copyright (C) 2002-2005 jointly by the following organizations:
 *     1. California Institute of Technology, Pasadena, CA, USA
 *     2. Japan Science and Technology Agency, Japan
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation.  A copy of the license agreement is provided
 * in the file named "LICENSE.txt" included with this software distribution
 * and also available online as http://sbml.org/software/libsbml/license.html
 *----------------------------------------------------------------------- -->
 *
 * @class AutoProperty
 * @sbmlbrief{core} Metaclass to convert getX/setX methods into Python
 * properties.
 *
 * This class is attached to SBase and automatically applies for all classes
 * which inherit from it.  Its purpose is to make libSBML more convenient to
 * use from Python.  It is a metaclass for automatically detecting getX/setX
 * methods at class creation time (not instantiation), and adding properties
 * (directly calling C methods where possible) to the class dictionary.
 *
 * @note Currently this class only works for Python 2.x, but should not break
 * in Python 3.
 */

%pythoncode %{

## enable optional new API2 functionality by configure or environment
if 'USE_LIBSBML_PYTHON_API2_WARNINGS' in os.environ:
    try:
        _etest = int(os.environ['USE_LIBSBML_PYTHON_API2_WARNINGS'])
    except ValueError:
        _etest = 0
        print('\nThe environment variable "USE_LIBSBML_PYTHON_API2_WARNINGS" should have the value 1 or 0\n')

    if _etest:
        USE_LIBSBML_PYTHON_API2_WARNINGS = True
    else:
        USE_LIBSBML_PYTHON_API2_WARNINGS = False

# class decorator, from six, that will be used to solve Python2/3 metaclass issues
# usage is NewClass(with_metaclass(MetaClass, BaseClasses)):
#def with_metaclass(meta, *bases):
#    """Create a base class with a metaclass.
#Usage is: NewClass(with_metaclass(MetaClass, BaseClass*)"""
# This requires a bit of explanation: the basic idea is to make a dummy
# metaclass for one level of class instantiation that replaces itself with
# the actual metaclass.
#    class metaclass(type):
#
#        def __new__(cls, name, this_bases, d):
#            return meta(name, bases, d)
#
#        @classmethod
#        def __prepare__(cls, name, this_bases):
#            return meta.__prepare__(name, bases)
#    return type.__new__(metaclass, 'temporary_class', (), {})

if USE_LIBSBML_PYTHON_API2_WARNINGS:
    import warnings
    class PythonAPI2Warning(UserWarning):
        """ Custom warning class for warnings generated by libSBML Python API2. """

        def __init__(self, msg):
            """ x.__init__(...) initializes x; see help(type(x)) for signature """
            print(msg)

    def p_func_warning_decorator(F):
        try:
            incoming_name = F.__name__
        except:
            return F
        from functools import wraps
        @wraps(F)
        def warning_wrap(*args, **kwargs):
            res = msg = None
            res = F(*args, **kwargs)
            if type(res) == int and res < 0:
                msg = '\"{}\" returns an error ({}): {}'.format(incoming_name, res,\
                                            OperationReturnValue_toString(res))
                warnings.warn(msg, category=PythonAPI2Warning)
            return res
        return warning_wrap

class AutoProperty(type):
    """
    Auto-detect Python class getX/setX methods.

    This class is attached to SBase and automatically applies for all classes
    which inherit from it.  Its purpose is to make libSBML more convenient to
    use from Python.  It works by analyzing classes at class creation time
    (not at instantiation) and adding corresponding properties (directly
    calling C methods where possible) to the class dictionary.

    @note The code should work for python 2.6 upwards, however for python 3 it
          needs to be attached via constructors.
    """
    def __new__(cls, classname, bases, classdict):
        """
        Iterate over the items in the classdict looking for get/set pairs
        and declaring them as properties.
        """
        import re
        import keyword
        import sys
        if sys.version_info < (3, 0):
          from inspect import getargspec as mygetargspec
        else:
          from inspect import getfullargspec as mygetargspec

        re_mangle = re.compile(r'[A-Za-z][a-z]+|[A-Z]+(?=$|[A-Z0-9])|\d+')
        re_id = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*$')
        re_getdoc = re.compile(r'^\s*[A-Za-z_][A-Za-z0-9_]*\(self\)')
        re_setdoc = re.compile(r'^\s*[A-Za-z_][A-Za-z0-9_]*\(self,[^,)]+\)')

        #convert ACapitalCasedStringXYZ123 -> a_capital_cased_string_xyz_123
        mangle_name = lambda x: ('_'.join(re_mangle.findall(x))).lower()

        get_methods = set()
        set_methods = set()
        swig_setter = classdict.get('__swig_setmethods__', {})

        if USE_LIBSBML_PYTHON_API2_WARNINGS:
            # explicitly collect unsetX methods separately for method decorator
            unset_methods = set()
            # collect addX and methods for method decorator
            re_addfunc = re.compile(r'add[A-Z][A-Za-z0-9_]*')
            add_methods = set()

        allowed_methods = [
            'Annotation',
            'AnnotationString',
            'Notes',
            'NotesString',
        ]

        #only search for get/set methods
        #we assume that any unset method also has either get or set
        #a small number of set without get cases exist so we can't only
        #search for get methods
        for k, v in classdict.items():
            name = k[3:]
            prefix = k[:3]
            mangled = mangle_name(name)
            if name:
                if callable(v):
                    if re_id.match(mangled) and mangled not in keyword.kwlist:
                        if prefix == 'get':
                            get_methods.add(name)
                        elif prefix == 'set':
                            set_methods.add(name)
            if USE_LIBSBML_PYTHON_API2_WARNINGS:
                # find addX methods for decoration
                if re_addfunc.match(k) is not None:
                    add_methods.add(k)

        for name in get_methods | set_methods:

            mangled = mangle_name(name)

            #ListOfFoobars -> foobars
            if mangled.startswith('list_of_'):
                mangled = mangled[8:]

            getter = setter = deleter = None
            if name in get_methods:
                getter = classdict['get'+name]

                #this is a very dirty way of checking if the get method
                #requires extra arguments (and hence cannot be a property)
                #it should be possible to do this properly in SWIG?
                try:
                    argspec = mygetargspec(getter)
                    numargs = len(argspec.args)
                    if numargs > 1 or (numargs == 1 and argspec.args[0] != 'self')  \
                       or (argspec.varargs!=None and name not in allowed_methods and not name.startswith('ListOf') ):
                        continue
                except Exception:
                    continue

                #use the c-level get function if the python function
                #only consists of a call to it
                cname = classname + '_get' + name
                #test if function is "return _libsbml.CLASS_getNAME(__args__)"
                try:
                    if getter.func_code.co_names == ('_libsbml', cname):
                        getter = getattr(_libsbml, cname)
                except:
                    if getter.__code__.co_names == ('_libsbml', cname):
                        getter = getattr(_libsbml, cname)

            if name in set_methods:
                setter = classdict['set'+name]
                try:
                    argspec = mygetargspec(getter)
                    numargs = len(argspec.args)
                    if numargs > 1 and argspec.args[0] == 'self':
                        cname = classname + '_set' + name
                        try:
                            if setter.func_code.co_names == ('_libsbml', cname):
                                setter = getattr(_libsbml, cname)
                        except:
                            if setter.__code__.co_names == ('_libsbml', cname):
                                setter = getattr(_libsbml, cname)

                        #property fget does not get intercepted by __getattr__
                        #but fset does, so we implement property setting via
                        #the __swig_setmethods__ dict
                        if USE_LIBSBML_PYTHON_API2_WARNINGS:
                            swig_setter[mangled] = p_func_warning_decorator(setter)
                        else:
                            swig_setter[mangled] = setter
                        continue
                except:
                    pass

            if 'unset' + name in classdict:
                deleter = classdict['unset'+name]

                try:
                    argspec = mygetargspec(getter)
                    numargs = len(argspec.args)
                    if numargs == 1 and argspec.args[0] == 'self' and \
                       (argspec.varargs==None or name in allowed_methods):
                        cname = classname + '_unset' + name
                        try:
                            if deleter.func_code.co_names == ('_libsbml', cname):
                                deleter = getattr(_libsbml, cname)
                        except:
                            if deleter.__code__.co_names == ('_libsbml', cname):
                                deleter = getattr(_libsbml, cname)
                except:
                    pass

                # wrap mangled deleters
                if USE_LIBSBML_PYTHON_API2_WARNINGS:
                    unset_methods.add(name)

            if getter or setter or deleter:
                #fset is technically redundant since the method is dispatched
                #via _swig_setattr rather than through the property due to that
                #function not delegating to object.__setattr__ which properly
                #handles properties
                classdict[mangled] = property(fget=getter, fset=setter, fdel=deleter)


            def __repr__(self):
                desc = self.__class__.__name__
                if hasattr(self, '__len__'):
                    desc += '[%s]' % self.__len__()
                if hasattr(self, 'id') and self.id:
                    desc += ' %s' % self.id
                if hasattr(self, 'name') and self.name:
                    desc += ' "%s"' % self.name
                return '<' + desc + '>'

            if classdict.get('__repr__', None) in (_swig_repr, None):
                classdict['__repr__'] = __repr__

        if USE_LIBSBML_PYTHON_API2_WARNINGS:
            # decorate get methods
            for gm in get_methods:
                if 'get'+gm in classdict:
                    classdict['get'+gm] = p_func_warning_decorator(classdict['get'+gm])

            # decorate set methods
            for sm in set_methods:
                if 'set'+sm in classdict:
                    classdict['set'+sm] = p_func_warning_decorator(classdict['set'+sm])

            # decorate unset methods
            for dm in unset_methods:
                if 'unset'+dm in classdict:
                    classdict['unset'+dm] = p_func_warning_decorator(classdict['unset'+dm])

            # decorate unset methods
            if len(add_methods) > 0:
                for am in add_methods:
                    if am in classdict:
                        classdict[am] = p_func_warning_decorator(classdict[am])

        return type.__new__(cls, classname, bases, classdict)

%}


%extend SBase
{
    %pythoncode
    {
        __metaclass__ = AutoProperty
    }
}


%extend SBasePlugin
{
    %pythoncode
    {
        __metaclass__ = AutoProperty
    }
}


%extend CVTerm
{
    %pythoncode
    {
        __metaclass__ = AutoProperty
    }
}


%extend Date
{
    %pythoncode
    {
        __metaclass__ = AutoProperty
    }
}


%extend ModelCreator
{
    %pythoncode
    {
        __metaclass__ = AutoProperty
    }
}

%extend ModelHistory
{
    %pythoncode
    {
        __metaclass__ = AutoProperty
    }
}


%extend SBMLNamespaces
{
    %pythoncode
    {
        __metaclass__ = AutoProperty
    }
}

%extend IdList
{
    %pythoncode
    {
        def __len__(self):
           return self.size()
        
        def __getitem__(self, index):
            return self.at(index)

        def __iter__(self):
            for i in range(self.size()):
                yield self.at(i)        
    }
}
