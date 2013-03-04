#ifdef USE_GROUPS

/**
 * Adds DownCastBase(long cPtr, boolean owner) method for the layout package extension
 */
%typemap(javacode) GroupsExtension
%{
	public SBasePlugin DowncastSBasePlugin(long cPtr, boolean owner)
	{
		if (cPtr == 0) return null;
		
		SBasePlugin sbp = new SBasePlugin(cPtr, false);
		SBase sb = sbp.getParentSBMLObject();
		
		switch( sb.getTypeCode() )
		{
			case (int) libsbml.SBML_MODEL:
				return new GroupsModelPlugin(cPtr,owner);
			default:
				return new SBasePlugin(cPtr,owner);
		}
	}
	
	public SBase DowncastSBase(long cPtr, boolean owner)
	{
		if (cPtr == 0) return null;
		
		SBase sb = new SBase(cPtr, false);
		switch( sb.getTypeCode() )
		{
			case (int) libsbml.SBML_LIST_OF:
				String name = sb.getElementName();
				if(name =="listOfMembers")
				{
					return new ListOfMembers(cPtr, owner);
				}
				else if(name =="listOfGroups")
				{
					return new ListOfGroups(cPtr, owner);
				}
				
				return new ListOf(cPtr, owner);
				
			case (int) libsbml.SBML_GROUPS_MEMBER:
				return new Member(cPtr, owner);
				
			case (int) libsbml.SBML_GROUPS_GROUP:
				return new Group(cPtr, owner);
				
			default:
				return new SBase(cPtr, owner);
		}
	}
	
	%}


COVARIANT_RTYPE_CLONE(Member)
COVARIANT_RTYPE_CLONE(Group)
COVARIANT_RTYPE_CLONE(ListOfMembers)
COVARIANT_RTYPE_CLONE(ListOfGroups)

COVARIANT_RTYPE_LISTOF_GET_REMOVE(GraphicalObject)
COVARIANT_RTYPE_LISTOF_GET_REMOVE(Layout)

SBMLCONSTRUCTOR_EXCEPTION(Group)
SBMLCONSTRUCTOR_EXCEPTION(GroupsPkgNamespaces)
SBMLCONSTRUCTOR_EXCEPTION(ListOfGroups)
SBMLCONSTRUCTOR_EXCEPTION(ListOfMembers)
SBMLCONSTRUCTOR_EXCEPTION(Member)

#endif  /* USE_GROUPS */
