/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2017  Seamly, LLC                                       *
 *                                                                         *
 *   https://github.com/fashionfreedom/seamly2d                            *
 *                                                                         *
 ***************************************************************************
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 **************************************************************************

 ************************************************************************
 **
 **  @file   vabstractnode.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Seamly2D project
 **  <https://github.com/fashionfreedom/seamly2d> All Rights Reserved.
 **
 **  Seamly2D is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Seamly2D is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Seamly2D.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "vabstractnode.h"

#include <QSharedPointer>
#include <QStaticStringData>
#include <QStringData>
#include <QStringDataPtr>
#include <QUndoStack>
#include <Qt>

#include "../../undocommands/adddetnode.h"
#include "../ifc/ifcdef.h"
#include "../ifc/xml/vabstractpattern.h"
#include "../vgeometry/vgobject.h"
#include "../vmisc/vabstractapplication.h"
#include "../vmisc/def.h"
#include "../vpatterndb/vcontainer.h"
#include "../vabstracttool.h"

const QString VAbstractNode::AttrIdTool = QStringLiteral("idTool");

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief VAbstractNode constructor.
 * @param doc dom document container.
 * @param data container with variables.
 * @param id object id in container.
 * @param idNode object id in containerNode.
 * @param idTool id tool.
 * @param parent parent object.
 */
VAbstractNode::VAbstractNode(VAbstractPattern *doc, VContainer *data, const quint32 &id, const quint32 &idNode,
                             const QString &blockName, const quint32 &idTool, QObject *parent)
    : VAbstractTool(doc, data, id, parent)
    , parentType(ParentType::Item)
    , idNode(idNode)
    , idTool(idTool)
    , m_blockName(blockName)
    , m_exluded(false)
{
    _referens = 0;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::ShowVisualization(bool show)
{
    Q_UNUSED(show)
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::incrementReferens()
{
    VAbstractTool::incrementReferens();
    if (_referens == 1)
    {
        if (idTool != NULL_ID)
        {
            doc->IncrementReferens(idTool);
        }
        else
        {
            const QSharedPointer<VGObject> node = VAbstractTool::data.GetGObject(idNode);
            doc->IncrementReferens(node->getIdTool());
        }
        ShowNode();
        QDomElement domElement = doc->elementById(m_id, getTagName());
        if (domElement.isElement())
        {
            doc->SetParametrUsage(domElement, AttrInUse, NodeUsage::InUse);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief decrementReferens decrement reference for all parent objects.
 */
void VAbstractNode::decrementReferens()
{
    VAbstractTool::decrementReferens();
    if (_referens == 0)
    {
        if (idTool != NULL_ID)
        {
            doc->DecrementReferens(idTool);
        }
        else
        {
            const QSharedPointer<VGObject> node = VAbstractTool::data.GetGObject(idNode);
            doc->DecrementReferens(node->getIdTool());
        }
        HideNode();
        QDomElement domElement = doc->elementById(m_id, getTagName());
        if (domElement.isElement())
        {
            doc->SetParametrUsage(domElement, AttrInUse, NodeUsage::NotInUse);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
ParentType VAbstractNode::GetParentType() const
{
    return parentType;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::SetParentType(const ParentType &value)
{
    parentType = value;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::GroupVisibility(quint32 object, bool visible)
{
    Q_UNUSED(object)
    Q_UNUSED(visible)
}

//---------------------------------------------------------------------------------------------------------------------
bool VAbstractNode::IsExluded() const
{
    return m_exluded;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::SetExluded(bool exluded)
{
    m_exluded = exluded;
}

//---------------------------------------------------------------------------------------------------------------------
void VAbstractNode::ToolCreation(const Source &typeCreation)
{
    if (typeCreation == Source::FromGui || typeCreation == Source::FromTool)
    {
        AddToFile();
    }
    else
    {
        RefreshDataInFile();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief AddToModeling add tag to modeling tag current pattern peace.
 * @param domElement tag.
 */
void VAbstractNode::AddToModeling(const QDomElement &domElement)
{
    AddDetNode *addNode = new AddDetNode(domElement, doc, m_blockName);
    qApp->getUndoStack()->push(addNode);
}
