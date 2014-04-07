/**
 * Filename    : requiredElements.i
 * Description : SBML RequiredElements swig file for bindings.
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

#ifdef USE_REQUIREDELEMENTS

%include sbml/packages/req/extension/RequiredElementsSBasePlugin.h
%include sbml/packages/req/extension/RequiredElementsExtension.h

%template(RequiredElementsPkgNamespaces) SBMLExtensionNamespaces<RequiredElementsExtension>;

#endif