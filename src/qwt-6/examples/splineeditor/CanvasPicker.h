/*****************************************************************************
 * Qwt Examples - Copyright (C) 2002 Uwe Rathmann
 * This file may be used under the terms of the 3-clause BSD License
 *****************************************************************************/

#pragma once

#include <QwtGlobal>
#include <QObject>

class QPoint;
class QwtPlot;
class QwtPlotCurve;

class CanvasPicker : public QObject
{
  public:
    CanvasPicker( bool sortedX, QwtPlot* plot );
    virtual bool eventFilter( QObject*, QEvent* ) QWT_OVERRIDE;

  private:
    void select( const QPoint& );
    void move( const QPoint& );
    void moveBy( int dx, int dy );
    void release();

    QwtPlot* plot();
    const QwtPlot* plot() const;

    QwtPlotCurve* m_selectedCurve;
    int m_selectedPoint;
    const bool m_sortedX;
};
