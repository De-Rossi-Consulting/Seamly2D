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
 **  @file   vtoolcubicbezierpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   18 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Seamly2D project
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

#ifndef VTOOLCUBICBEZIERPATH_H
#define VTOOLCUBICBEZIERPATH_H

#include <qcompilerdetection.h>
#include <QGraphicsItem>
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>

#include "../ifc/xml/vabstractpattern.h"
#include "../vmisc/def.h"
#include "vabstractspline.h"

class VCubicBezierPath;
template <class T> class QSharedPointer;

class VToolCubicBezierPath:public VAbstractSpline
{
    Q_OBJECT
public:
    virtual             ~VToolCubicBezierPath() Q_DECL_EQ_DEFAULT;
    virtual void         setDialog() Q_DECL_OVERRIDE;

    static VToolCubicBezierPath *Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene  *scene,
                                        VAbstractPattern *doc, VContainer *data);
    static VToolCubicBezierPath *Create(const quint32 _id, VCubicBezierPath *path, VMainGraphicsScene *scene,
                                        VAbstractPattern *doc, VContainer *data, const Document &parse,
                                        const Source &typeCreation);

    static const QString ToolType;
    static void          UpdatePathPoints(VAbstractPattern *doc, QDomElement &element, const VCubicBezierPath &path);
    virtual int          type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::CubicBezierPath)};

    VCubicBezierPath     getSplinePath()const;
    void                 setSplinePath(const VCubicBezierPath &splPath);

    virtual void         ShowVisualization(bool show) Q_DECL_OVERRIDE;

protected slots:
    virtual void         showContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) Q_DECL_OVERRIDE;
    
protected:
    virtual void         RemoveReferens() Q_DECL_OVERRIDE;
    virtual void         SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void         SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual void         SetVisualization() Q_DECL_OVERRIDE;
    virtual void         refreshGeometry() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(VToolCubicBezierPath)

                         VToolCubicBezierPath(VAbstractPattern *doc, VContainer *data, quint32 id,
                                              const Source &typeCreation, QGraphicsItem * parent = nullptr);

    static void          AddPathPoint(VAbstractPattern *doc, QDomElement &domElement, const VPointF &splPoint);
    void                 SetSplinePathAttributes(QDomElement &domElement, const VCubicBezierPath &path);
};

#endif // VTOOLCUBICBEZIERPATH_H
