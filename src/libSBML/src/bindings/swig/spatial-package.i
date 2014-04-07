/**
 * Filename    : spatial.i
 * Description : SBML Spatial swig file for bindings.
 * Organization: UConn Health Center
 * Created     : 2010-08-24
 *
 * Copyright 2010 UConn Health Center
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, WITHOUT EVEN THE IMPLIED WARRANTY OF
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  The software and
 * documentation provided hereunder is on an as is" basis, and the European
 * Media Laboratories gGmbH have no obligations to provide maintenance,
 * support, updates, enhancements or modifications.  In no event shall the
 * European Media Laboratory gGmbH be liable to any party for direct,
 * indirect, special, incidental or consequential damages, including lost
 * profits, arising out of the use of this software and its documentation,
 * even if the European Media Laboratories gGmbH have been advised of the
 * possibility of such damage.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * The original code contained here was initially developed by:
 *
 *     Anu and Jim
 *     UCHC
 *     400 Farmington Ave
 *     Farmington, CT 06030
 *     USA
 *
 *     http://www.vcell.org
 *     mailto:anu@uchc.edu
 *
 * Contributor(s):
 *     Fei Gao
 */

#ifdef USE_SPATIAL

%template(SpatialPkgNamespaces) SBMLExtensionNamespaces<SpatialExtension>;

%include sbml/packages/spatial/sbml/Geometry.h
%include sbml/packages/spatial/sbml/CoordinateComponent.h
%include sbml/packages/spatial/sbml/Boundary.h
%include sbml/packages/spatial/sbml/DomainType.h
%include sbml/packages/spatial/sbml/CompartmentMapping.h
%include sbml/packages/spatial/sbml/Domain.h
%include sbml/packages/spatial/sbml/InteriorPoint.h
%include sbml/packages/spatial/sbml/AdjacentDomains.h
%include sbml/packages/spatial/sbml/GeometryDefinition.h
%include sbml/packages/spatial/sbml/AnalyticGeometry.h
%include sbml/packages/spatial/sbml/AnalyticVolume.h
%include sbml/packages/spatial/sbml/SpatialPoint.h
%include sbml/packages/spatial/sbml/SampledFieldGeometry.h
%include sbml/packages/spatial/sbml/SampledVolume.h
%include sbml/packages/spatial/sbml/SampledField.h
%include sbml/packages/spatial/sbml/ImageData.h

%include sbml/packages/spatial/sbml/ParametricGeometry.h

%include sbml/packages/spatial/sbml/ParametricObject.h

%include sbml/packages/spatial/sbml/PolygonObject.h

%include sbml/packages/spatial/sbml/CSGeometry.h

%include sbml/packages/spatial/sbml/CSGObject.h

%include sbml/packages/spatial/sbml/CSGNode.h

%include sbml/packages/spatial/sbml/CSGPrimitive.h

%include sbml/packages/spatial/sbml/CSGPseudoPrimitive.h

%include sbml/packages/spatial/sbml/CSGSetOperator.h

%include sbml/packages/spatial/sbml/CSGTransformation.h

%include sbml/packages/spatial/sbml/CSGTranslation.h

%include sbml/packages/spatial/sbml/CSGRotation.h

%include sbml/packages/spatial/sbml/CSGScale.h

%include sbml/packages/spatial/sbml/CSGHomogeneousTransformation.h

%include sbml/packages/spatial/sbml/TransformationComponents.h
%include sbml/packages/spatial/sbml/SpatialSymbolReference.h
%include sbml/packages/spatial/sbml/DiffusionCoefficient.h
%include sbml/packages/spatial/sbml/AdvectionCoefficient.h
%include sbml/packages/spatial/sbml/BoundaryCondition.h

%include sbml/packages/spatial/extension/SpatialExtension.h
%include sbml/packages/spatial/extension/SpatialModelPlugin.h
%include sbml/packages/spatial/extension/SpatialCompartmentPlugin.h
%include sbml/packages/spatial/extension/SpatialParameterPlugin.h
%include sbml/packages/spatial/extension/SpatialSpeciesRxnPlugin.h

#endif
