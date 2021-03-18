/******************************************************************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_plot_layout.h"
#include "qwt_text.h"
#include "qwt_text_label.h"
#include "qwt_scale_widget.h"
#include "qwt_abstract_legend.h"
#include "qwt_math.h"

#include <qmargins.h>

namespace
{
    class LayoutData
    {
      public:
        void init( const QwtPlot*, const QRectF& rect );

        struct t_legendData
        {
            int frameWidth;
            int hScrollExtent;
            int vScrollExtent;
            QSize hint;
        } legend;

        struct t_titleData
        {
            QwtText text;
            int frameWidth;
        } title;

        struct t_footerData
        {
            QwtText text;
            int frameWidth;
        } footer;

        struct t_scaleData
        {
            bool isEnabled;
            const QwtScaleWidget* scaleWidget;
            QFont scaleFont;
            int start;
            int end;
            int baseLineOffset;
            double tickOffset;
            int dimWithoutTitle;
        } scale[ QwtAxis::AxisCount ];

        struct t_canvasData
        {
            int contentsMargins[ QwtAxis::AxisCount ];

        } canvas;
    };

    /*
       Extract all layout relevant data from the plot components
     */
    void LayoutData::init( const QwtPlot* plot, const QRectF& rect )
    {
        // legend

        if ( plot->legend() )
        {
            legend.frameWidth = plot->legend()->frameWidth();
            legend.hScrollExtent =
                plot->legend()->scrollExtent( Qt::Horizontal );
            legend.vScrollExtent =
                plot->legend()->scrollExtent( Qt::Vertical );

            const QSize hint = plot->legend()->sizeHint();

            const int w = qMin( hint.width(), qwtFloor( rect.width() ) );

            int h = plot->legend()->heightForWidth( w );
            if ( h <= 0 )
                h = hint.height();

            legend.hint = QSize( w, h );
        }

        // title

        title.frameWidth = 0;
        title.text = QwtText();

        if ( plot->titleLabel() )
        {
            const QwtTextLabel* label = plot->titleLabel();
            title.text = label->text();
            if ( !( title.text.testPaintAttribute( QwtText::PaintUsingTextFont ) ) )
                title.text.setFont( label->font() );

            title.frameWidth = plot->titleLabel()->frameWidth();
        }

        // footer

        footer.frameWidth = 0;
        footer.text = QwtText();

        if ( plot->footerLabel() )
        {
            const QwtTextLabel* label = plot->footerLabel();
            footer.text = label->text();
            if ( !( footer.text.testPaintAttribute( QwtText::PaintUsingTextFont ) ) )
                footer.text.setFont( label->font() );

            footer.frameWidth = plot->footerLabel()->frameWidth();
        }

        // scales

        for ( int axis = 0; axis < QwtAxis::AxisCount; axis++ )
        {
            if ( plot->isAxisVisible( axis ) )
            {
                const QwtScaleWidget* scaleWidget = plot->axisWidget( axis );

                scale[axis].isEnabled = true;

                scale[axis].scaleWidget = scaleWidget;

                scale[axis].scaleFont = scaleWidget->font();

                scale[axis].start = scaleWidget->startBorderDist();
                scale[axis].end = scaleWidget->endBorderDist();

                scale[axis].baseLineOffset = scaleWidget->margin();
                scale[axis].tickOffset = scaleWidget->margin();
                if ( scaleWidget->scaleDraw()->hasComponent(
                    QwtAbstractScaleDraw::Ticks ) )
                {
                    scale[axis].tickOffset +=
                        scaleWidget->scaleDraw()->maxTickLength();
                }

                scale[axis].dimWithoutTitle = scaleWidget->dimForLength(
                    QWIDGETSIZE_MAX, scale[axis].scaleFont );

                if ( !scaleWidget->title().isEmpty() )
                {
                    scale[axis].dimWithoutTitle -=
                        scaleWidget->titleHeightForWidth( QWIDGETSIZE_MAX );
                }
            }
            else
            {
                scale[axis].isEnabled = false;
                scale[axis].start = 0;
                scale[axis].end = 0;
                scale[axis].baseLineOffset = 0;
                scale[axis].tickOffset = 0.0;
                scale[axis].dimWithoutTitle = 0;
            }
        }

        // canvas

        const QMargins m = plot->canvas()->contentsMargins();

        canvas.contentsMargins[ QwtAxis::YLeft ] = m.left();
        canvas.contentsMargins[ QwtAxis::XTop ] = m.top();
        canvas.contentsMargins[ QwtAxis::YRight ] = m.right();
        canvas.contentsMargins[ QwtAxis::XBottom ] = m.bottom();
    }
}

class QwtPlotLayout::PrivateData
{
  public:
    PrivateData()
        : legendRatio( 1.0 )
        , spacing( 5 )
    {
    }

    QRectF titleRect;
    QRectF footerRect;
    QRectF legendRect;
    QRectF scaleRect[ QwtAxis::AxisCount ];
    QRectF canvasRect;

    LayoutData layoutData;

    QwtPlot::LegendPosition legendPos;
    double legendRatio;
    unsigned int spacing;
    unsigned int canvasMargin[ QwtAxis::AxisCount ];
    bool alignCanvasToScales[ QwtAxis::AxisCount ];
};

/*!
   \brief Constructor
 */

QwtPlotLayout::QwtPlotLayout()
{
    m_data = new PrivateData;

    setLegendPosition( QwtPlot::BottomLegend );
    setCanvasMargin( 4 );
    setAlignCanvasToScales( false );

    invalidate();
}

//! Destructor
QwtPlotLayout::~QwtPlotLayout()
{
    delete m_data;
}

/*!
   Change a margin of the canvas. The margin is the space
   above/below the scale ticks. A negative margin will
   be set to -1, excluding the borders of the scales.

   \param margin New margin
   \param axisPos One of QwtAxis::Position. Specifies where the position of the margin.
              -1 means margin at all borders.
   \sa canvasMargin()

   \warning The margin will have no effect when alignCanvasToScale() is true
 */

void QwtPlotLayout::setCanvasMargin( int margin, int axisPos )
{
    if ( margin < -1 )
        margin = -1;

    if ( axisPos == -1 )
    {
        for ( axisPos = 0; axisPos < QwtAxis::AxisCount; axisPos++ )
            m_data->canvasMargin[axisPos] = margin;
    }
    else if ( QwtAxis::isValid( axisPos ) )
        m_data->canvasMargin[axisPos] = margin;
}

/*!
    \param axisPos Axis position
    \return Margin around the scale tick borders
    \sa setCanvasMargin()
 */
int QwtPlotLayout::canvasMargin( int axisPos ) const
{
    if ( !QwtAxis::isValid( axisPos ) )
        return 0;

    return m_data->canvasMargin[axisPos];
}

/*!
   \brief Set the align-canvas-to-axis-scales flag for all axes

   \param on True/False
   \sa setAlignCanvasToScale(), alignCanvasToScale()
 */
void QwtPlotLayout::setAlignCanvasToScales( bool on )
{
    for ( int axisPos = 0; axisPos < QwtAxis::AxisCount; axisPos++ )
        m_data->alignCanvasToScales[axisPos] = on;
}

/*!
   Change the align-canvas-to-axis-scales setting. The canvas may:

   - extend beyond the axis scale ends to maximize its size,
   - align with the axis scale ends to control its size.

   The axisId parameter is somehow confusing as it identifies a border
   of the plot and not the axes, that are aligned. F.e when QwtAxis::YLeft
   is set, the left end of the the x-axes ( QwtAxis::XTop, QwtAxis::XBottom )
   is aligned.

   \param axisId Axis index
   \param on New align-canvas-to-axis-scales setting

   \sa setCanvasMargin(), alignCanvasToScale(), setAlignCanvasToScales()
   \warning In case of on == true canvasMargin() will have no effect
 */
void QwtPlotLayout::setAlignCanvasToScale( int axisPos, bool on )
{
    if ( QwtAxis::isValid( axisPos ) )
        m_data->alignCanvasToScales[axisPos] = on;
}

/*!
   Return the align-canvas-to-axis-scales setting. The canvas may:
   - extend beyond the axis scale ends to maximize its size
   - align with the axis scale ends to control its size.

   \param axisPos Axis position
   \return align-canvas-to-axis-scales setting
   \sa setAlignCanvasToScale(), setAlignCanvasToScale(), setCanvasMargin()
 */
bool QwtPlotLayout::alignCanvasToScale( int axisPos ) const
{
    if ( !QwtAxis::isValid( axisPos ) )
        return false;

    return m_data->alignCanvasToScales[ axisPos ];
}

/*!
   Change the spacing of the plot. The spacing is the distance
   between the plot components.

   \param spacing New spacing
   \sa setCanvasMargin(), spacing()
 */
void QwtPlotLayout::setSpacing( int spacing )
{
    m_data->spacing = qMax( 0, spacing );
}

/*!
   \return Spacing
   \sa margin(), setSpacing()
 */
int QwtPlotLayout::spacing() const
{
    return m_data->spacing;
}

/*!
   \brief Specify the position of the legend
   \param pos The legend's position.
   \param ratio Ratio between legend and the bounding rectangle
               of title, footer, canvas and axes. The legend will be shrunk
               if it would need more space than the given ratio.
               The ratio is limited to ]0.0 .. 1.0]. In case of <= 0.0
               it will be reset to the default ratio.
               The default vertical/horizontal ratio is 0.33/0.5.

   \sa QwtPlot::setLegendPosition()
 */

void QwtPlotLayout::setLegendPosition( QwtPlot::LegendPosition pos, double ratio )
{
    if ( ratio > 1.0 )
        ratio = 1.0;

    switch ( pos )
    {
        case QwtPlot::TopLegend:
        case QwtPlot::BottomLegend:
        {
            if ( ratio <= 0.0 )
                ratio = 0.33;
            m_data->legendRatio = ratio;
            m_data->legendPos = pos;
            break;
        }
        case QwtPlot::LeftLegend:
        case QwtPlot::RightLegend:
        {
            if ( ratio <= 0.0 )
                ratio = 0.5;
            m_data->legendRatio = ratio;
            m_data->legendPos = pos;
            break;
        }
        default:
            break;
    }
}

/*!
   \brief Specify the position of the legend
   \param pos The legend's position. Valid values are
      \c QwtPlot::LeftLegend, \c QwtPlot::RightLegend,
      \c QwtPlot::TopLegend, \c QwtPlot::BottomLegend.

   \sa QwtPlot::setLegendPosition()
 */
void QwtPlotLayout::setLegendPosition( QwtPlot::LegendPosition pos )
{
    setLegendPosition( pos, 0.0 );
}

/*!
   \return Position of the legend
   \sa setLegendPosition(), QwtPlot::setLegendPosition(),
      QwtPlot::legendPosition()
 */
QwtPlot::LegendPosition QwtPlotLayout::legendPosition() const
{
    return m_data->legendPos;
}

/*!
   Specify the relative size of the legend in the plot
   \param ratio Ratio between legend and the bounding rectangle
               of title, footer, canvas and axes. The legend will be shrunk
               if it would need more space than the given ratio.
               The ratio is limited to ]0.0 .. 1.0]. In case of <= 0.0
               it will be reset to the default ratio.
               The default vertical/horizontal ratio is 0.33/0.5.
 */
void QwtPlotLayout::setLegendRatio( double ratio )
{
    setLegendPosition( legendPosition(), ratio );
}

/*!
   \return The relative size of the legend in the plot.
   \sa setLegendPosition()
 */
double QwtPlotLayout::legendRatio() const
{
    return m_data->legendRatio;
}

/*!
   \brief Set the geometry for the title

   This method is intended to be used from derived layouts
   overloading activate()

   \sa titleRect(), activate()
 */
void QwtPlotLayout::setTitleRect( const QRectF& rect )
{
    m_data->titleRect = rect;
}

/*!
   \return Geometry for the title
   \sa activate(), invalidate()
 */
QRectF QwtPlotLayout::titleRect() const
{
    return m_data->titleRect;
}

/*!
   \brief Set the geometry for the footer

   This method is intended to be used from derived layouts
   overloading activate()

   \sa footerRect(), activate()
 */
void QwtPlotLayout::setFooterRect( const QRectF& rect )
{
    m_data->footerRect = rect;
}

/*!
   \return Geometry for the footer
   \sa activate(), invalidate()
 */
QRectF QwtPlotLayout::footerRect() const
{
    return m_data->footerRect;
}

/*!
   \brief Set the geometry for the legend

   This method is intended to be used from derived layouts
   overloading activate()

   \param rect Rectangle for the legend

   \sa legendRect(), activate()
 */
void QwtPlotLayout::setLegendRect( const QRectF& rect )
{
    m_data->legendRect = rect;
}

/*!
   \return Geometry for the legend
   \sa activate(), invalidate()
 */
QRectF QwtPlotLayout::legendRect() const
{
    return m_data->legendRect;
}

/*!
   \brief Set the geometry for an axis

   This method is intended to be used from derived layouts
   overloading activate()

   \param axisId Axis
   \param rect Rectangle for the scale

   \sa scaleRect(), activate()
 */
void QwtPlotLayout::setScaleRect( QwtAxisId axisId, const QRectF& rect )
{
    if ( QwtAxis::isValid( axisId ) )
        m_data->scaleRect[axisId] = rect;
}

/*!
   \param axisId Axis
   \return Geometry for the scale
   \sa activate(), invalidate()
 */
QRectF QwtPlotLayout::scaleRect( QwtAxisId axisId ) const
{
    if ( QwtAxis::isValid( axisId ) )
        return m_data->scaleRect[axisId];

    return QRectF();
}

/*!
   \brief Set the geometry for the canvas

   This method is intended to be used from derived layouts
   overloading activate()

   \sa canvasRect(), activate()
 */
void QwtPlotLayout::setCanvasRect( const QRectF& rect )
{
    m_data->canvasRect = rect;
}

/*!
   \return Geometry for the canvas
   \sa activate(), invalidate()
 */
QRectF QwtPlotLayout::canvasRect() const
{
    return m_data->canvasRect;
}

/*!
   Invalidate the geometry of all components.
   \sa activate()
 */
void QwtPlotLayout::invalidate()
{
    m_data->titleRect = m_data->footerRect
            = m_data->legendRect = m_data->canvasRect = QRect();

    for ( int axis = 0; axis < QwtAxis::AxisCount; axis++ )
        m_data->scaleRect[axis] = QRect();
}

/*!
   \return Minimum size hint
   \param plot Plot widget

   \sa QwtPlot::minimumSizeHint()
 */
QSize QwtPlotLayout::minimumSizeHint( const QwtPlot* plot ) const
{
    using namespace QwtAxis;

    class ScaleData
    {
      public:
        ScaleData()
        {
            w = h = minLeft = minRight = tickOffset = 0;
        }

        int w;
        int h;
        int minLeft;
        int minRight;
        int tickOffset;
    } scaleData[AxisCount];

    int canvasBorder[AxisCount];

    const int fw = plot->canvas()->contentsMargins().left();

    int axis;
    for ( axis = 0; axis < AxisCount; axis++ )
    {
        if ( plot->isAxisVisible( axis ) )
        {
            const QwtScaleWidget* scl = plot->axisWidget( axis );
            ScaleData& sd = scaleData[axis];

            const QSize hint = scl->minimumSizeHint();
            sd.w = hint.width();
            sd.h = hint.height();
            scl->getBorderDistHint( sd.minLeft, sd.minRight );
            sd.tickOffset = scl->margin();
            if ( scl->scaleDraw()->hasComponent( QwtAbstractScaleDraw::Ticks ) )
                sd.tickOffset += qwtCeil( scl->scaleDraw()->maxTickLength() );
        }

        canvasBorder[axis] = fw + m_data->canvasMargin[axis] + 1;
    }

    for ( axis = 0; axis < AxisCount; axis++ )
    {
        ScaleData& sd = scaleData[axis];
        if ( sd.w && isXAxis( axis ) )
        {
            if ( ( sd.minLeft > canvasBorder[YLeft] ) && scaleData[YLeft].w )
            {
                int shiftLeft = sd.minLeft - canvasBorder[YLeft];
                if ( shiftLeft > scaleData[YLeft].w )
                    shiftLeft = scaleData[YLeft].w;

                sd.w -= shiftLeft;
            }

            if ( ( sd.minRight > canvasBorder[YRight] ) && scaleData[YRight].w )
            {
                int shiftRight = sd.minRight - canvasBorder[YRight];
                if ( shiftRight > scaleData[YRight].w )
                    shiftRight = scaleData[YRight].w;

                sd.w -= shiftRight;
            }
        }

        if ( sd.h && ( isYAxis( axis ) ) )
        {
            if ( ( sd.minLeft > canvasBorder[XBottom] ) && scaleData[XBottom].h )
            {
                int shiftBottom = sd.minLeft - canvasBorder[XBottom];
                if ( shiftBottom > scaleData[XBottom].tickOffset )
                    shiftBottom = scaleData[XBottom].tickOffset;

                sd.h -= shiftBottom;
            }

            if ( ( sd.minLeft > canvasBorder[XTop] ) && scaleData[XTop].h )
            {
                int shiftTop = sd.minRight - canvasBorder[XTop];
                if ( shiftTop > scaleData[XTop].tickOffset )
                    shiftTop = scaleData[XTop].tickOffset;

                sd.h -= shiftTop;
            }
        }
    }

    const QWidget* canvas = plot->canvas();

    const QMargins m = canvas->contentsMargins();

    const QSize minCanvasSize = canvas->minimumSize();

    int w = scaleData[YLeft].w + scaleData[YRight].w;
    int cw = qMax( scaleData[XBottom].w, scaleData[XTop].w )
        + m.left() + 1 + m.right() + 1;
    w += qMax( cw, minCanvasSize.width() );

    int h = scaleData[XBottom].h + scaleData[XTop].h;
    int ch = qMax( scaleData[YLeft].h, scaleData[YRight].h )
        + m.top() + 1 + m.bottom() + 1;
    h += qMax( ch, minCanvasSize.height() );

    const QwtTextLabel* labels[2];
    labels[0] = plot->titleLabel();
    labels[1] = plot->footerLabel();

    for ( int i = 0; i < 2; i++ )
    {
        const QwtTextLabel* label = labels[i];
        if ( label && !label->text().isEmpty() )
        {
            // If only YLeft or YRight is showing, we center on the plot canvas.
            const bool centerOnCanvas = !( plot->isAxisVisible( YLeft )
                && plot->isAxisVisible( YRight ) );

            int labelW = w;
            if ( centerOnCanvas )
            {
                labelW -= scaleData[YLeft].w + scaleData[YRight].w;
            }

            int labelH = label->heightForWidth( labelW );
            if ( labelH > labelW ) // Compensate for a long title
            {
                w = labelW = labelH;
                if ( centerOnCanvas )
                    w += scaleData[YLeft].w + scaleData[YRight].w;

                labelH = label->heightForWidth( labelW );
            }
            h += labelH + m_data->spacing;
        }
    }

    // Compute the legend contribution

    const QwtAbstractLegend* legend = plot->legend();
    if ( legend && !legend->isEmpty() )
    {
        if ( m_data->legendPos == QwtPlot::LeftLegend
            || m_data->legendPos == QwtPlot::RightLegend )
        {
            int legendW = legend->sizeHint().width();
            int legendH = legend->heightForWidth( legendW );

            if ( legend->frameWidth() > 0 )
                w += m_data->spacing;

            if ( legendH > h )
                legendW += legend->scrollExtent( Qt::Horizontal );

            if ( m_data->legendRatio < 1.0 )
                legendW = qMin( legendW, int( w / ( 1.0 - m_data->legendRatio ) ) );

            w += legendW + m_data->spacing;
        }
        else
        {
            int legendW = qMin( legend->sizeHint().width(), w );
            int legendH = legend->heightForWidth( legendW );

            if ( legend->frameWidth() > 0 )
                h += m_data->spacing;

            if ( m_data->legendRatio < 1.0 )
                legendH = qMin( legendH, int( h / ( 1.0 - m_data->legendRatio ) ) );

            h += legendH + m_data->spacing;
        }
    }

    return QSize( w, h );
}

/*!
   Find the geometry for the legend

   \param options Options how to layout the legend
   \param rect Rectangle where to place the legend

   \return Geometry for the legend
   \sa Options
 */
QRectF QwtPlotLayout::layoutLegend( Options options,
    const QRectF& rect ) const
{
    const QSize hint( m_data->layoutData.legend.hint );

    int dim;
    if ( m_data->legendPos == QwtPlot::LeftLegend
        || m_data->legendPos == QwtPlot::RightLegend )
    {
        // We don't allow vertical legends to take more than
        // half of the available space.

        dim = qMin( hint.width(), int( rect.width() * m_data->legendRatio ) );

        if ( !( options & IgnoreScrollbars ) )
        {
            if ( hint.height() > rect.height() )
            {
                // The legend will need additional
                // space for the vertical scrollbar.

                dim += m_data->layoutData.legend.hScrollExtent;
            }
        }
    }
    else
    {
        dim = qMin( hint.height(), int( rect.height() * m_data->legendRatio ) );
        dim = qMax( dim, m_data->layoutData.legend.vScrollExtent );
    }

    QRectF legendRect = rect;
    switch ( m_data->legendPos )
    {
        case QwtPlot::LeftLegend:
            legendRect.setWidth( dim );
            break;
        case QwtPlot::RightLegend:
            legendRect.setX( rect.right() - dim );
            legendRect.setWidth( dim );
            break;
        case QwtPlot::TopLegend:
            legendRect.setHeight( dim );
            break;
        case QwtPlot::BottomLegend:
            legendRect.setY( rect.bottom() - dim );
            legendRect.setHeight( dim );
            break;
    }

    return legendRect;
}

/*!
   Align the legend to the canvas

   \param canvasRect Geometry of the canvas
   \param legendRect Maximum geometry for the legend

   \return Geometry for the aligned legend
 */
QRectF QwtPlotLayout::alignLegend( const QRectF& canvasRect,
    const QRectF& legendRect ) const
{
    QRectF alignedRect = legendRect;

    if ( m_data->legendPos == QwtPlot::BottomLegend
        || m_data->legendPos == QwtPlot::TopLegend )
    {
        if ( m_data->layoutData.legend.hint.width() < canvasRect.width() )
        {
            alignedRect.setX( canvasRect.x() );
            alignedRect.setWidth( canvasRect.width() );
        }
    }
    else
    {
        if ( m_data->layoutData.legend.hint.height() < canvasRect.height() )
        {
            alignedRect.setY( canvasRect.y() );
            alignedRect.setHeight( canvasRect.height() );
        }
    }

    return alignedRect;
}

/*!
   Expand all line breaks in text labels, and calculate the height
   of their widgets in orientation of the text.

   \param options Options how to layout the legend
   \param rect Bounding rectangle for title, footer, axes and canvas.
   \param dimTitle Expanded height of the title widget
   \param dimFooter Expanded height of the footer widget
   \param dimAxis Expanded heights of the axis in axis orientation.

   \sa Options
 */
void QwtPlotLayout::expandLineBreaks( Options options, const QRectF& rect,
    int& dimTitle, int& dimFooter, int dimAxis[QwtAxis::AxisCount] ) const
{
    using namespace QwtAxis;

    dimTitle = dimFooter = 0;
    for ( int axis = 0; axis < AxisCount; axis++ )
        dimAxis[axis] = 0;

    int backboneOffset[AxisCount];
    for ( int axis = 0; axis < AxisCount; axis++ )
    {
        backboneOffset[axis] = 0;
        if ( !( options & IgnoreFrames ) )
            backboneOffset[axis] += m_data->layoutData.canvas.contentsMargins[ axis ];

        if ( !m_data->alignCanvasToScales[axis] )
            backboneOffset[axis] += m_data->canvasMargin[axis];
    }

    bool done = false;
    while ( !done )
    {
        done = true;

        // the size for the 4 axis depend on each other. Expanding
        // the height of a horizontal axis will shrink the height
        // for the vertical axis, shrinking the height of a vertical
        // axis will result in a line break what will expand the
        // width and results in shrinking the width of a horizontal
        // axis what might result in a line break of a horizontal
        // axis ... . So we loop as long until no size changes.

        if ( !( ( options & IgnoreTitle ) ||
            m_data->layoutData.title.text.isEmpty() ) )
        {
            double w = rect.width();

            if ( m_data->layoutData.scale[YLeft].isEnabled
                != m_data->layoutData.scale[YRight].isEnabled )
            {
                // center to the canvas
                w -= dimAxis[YLeft] + dimAxis[YRight];
            }

            int d = qwtCeil( m_data->layoutData.title.text.heightForWidth( w ) );
            if ( !( options & IgnoreFrames ) )
                d += 2 * m_data->layoutData.title.frameWidth;

            if ( d > dimTitle )
            {
                dimTitle = d;
                done = false;
            }
        }

        if ( !( ( options & IgnoreFooter ) ||
            m_data->layoutData.footer.text.isEmpty() ) )
        {
            double w = rect.width();

            if ( m_data->layoutData.scale[YLeft].isEnabled
                != m_data->layoutData.scale[YRight].isEnabled )
            {
                // center to the canvas
                w -= dimAxis[YLeft] + dimAxis[YRight];
            }

            int d = qwtCeil( m_data->layoutData.footer.text.heightForWidth( w ) );
            if ( !( options & IgnoreFrames ) )
                d += 2 * m_data->layoutData.footer.frameWidth;

            if ( d > dimFooter )
            {
                dimFooter = d;
                done = false;
            }
        }

        for ( int axis = 0; axis < AxisCount; axis++ )
        {
            const struct LayoutData::t_scaleData& scaleData =
                m_data->layoutData.scale[axis];

            if ( scaleData.isEnabled )
            {
                double length;
                if ( isXAxis( axis ) )
                {
                    length = rect.width() - dimAxis[YLeft] - dimAxis[YRight];
                    length -= scaleData.start + scaleData.end;

                    if ( dimAxis[YRight] > 0 )
                        length -= 1;

                    length += qMin( dimAxis[YLeft],
                        scaleData.start - backboneOffset[YLeft] );
                    length += qMin( dimAxis[YRight],
                        scaleData.end - backboneOffset[YRight] );
                }
                else // YLeft, YRight
                {
                    length = rect.height() - dimAxis[XTop] - dimAxis[XBottom];
                    length -= scaleData.start + scaleData.end;
                    length -= 1;

                    if ( dimAxis[XBottom] <= 0 )
                        length -= 1;

                    if ( dimAxis[XTop] <= 0 )
                        length -= 1;

                    if ( dimAxis[XBottom] > 0 )
                    {
                        length += qMin(
                            m_data->layoutData.scale[XBottom].tickOffset,
                            double( scaleData.start - backboneOffset[XBottom] ) );
                    }

                    if ( dimAxis[XTop] > 0 )
                    {
                        length += qMin(
                            m_data->layoutData.scale[XTop].tickOffset,
                            double( scaleData.end - backboneOffset[XTop] ) );
                    }

                    if ( dimTitle > 0 )
                        length -= dimTitle + m_data->spacing;
                }

                int d = scaleData.dimWithoutTitle;
                if ( !scaleData.scaleWidget->title().isEmpty() )
                {
                    d += scaleData.scaleWidget->titleHeightForWidth( qwtFloor( length ) );
                }


                if ( d > dimAxis[axis] )
                {
                    dimAxis[axis] = d;
                    done = false;
                }
            }
        }
    }
}

/*!
   Align the ticks of the axis to the canvas borders using
   the empty corners.

   \param options Layout options
   \param canvasRect Geometry of the canvas ( IN/OUT )
   \param scaleRect Geometries of the scales ( IN/OUT )

   \sa Options
 */

void QwtPlotLayout::alignScales( Options options,
    QRectF& canvasRect, QRectF scaleRect[QwtAxis::AxisCount] ) const
{
    using namespace QwtAxis;

    int backboneOffset[AxisCount];
    for ( int axis = 0; axis < AxisCount; axis++ )
    {
        backboneOffset[axis] = 0;

        if ( !m_data->alignCanvasToScales[axis] )
        {
            backboneOffset[axis] += m_data->canvasMargin[axis];
        }

        if ( !( options & IgnoreFrames ) )
        {
            backboneOffset[axis] +=
                m_data->layoutData.canvas.contentsMargins[axis];
        }
    }

    for ( int axis = 0; axis < AxisCount; axis++ )
    {
        if ( !scaleRect[axis].isValid() )
            continue;

        const int startDist = m_data->layoutData.scale[axis].start;
        const int endDist = m_data->layoutData.scale[axis].end;

        QRectF& axisRect = scaleRect[axis];

        if ( isXAxis( axis ) )
        {
            const QRectF& leftScaleRect = scaleRect[YLeft];
            const int leftOffset = backboneOffset[YLeft] - startDist;

            if ( leftScaleRect.isValid() )
            {
                const double dx = leftOffset + leftScaleRect.width();
                if ( m_data->alignCanvasToScales[YLeft] && dx < 0.0 )
                {
                    /*
                       The axis needs more space than the width
                       of the left scale.
                     */
                    const double cLeft = canvasRect.left(); // qreal -> double
                    canvasRect.setLeft( qwtMaxF( cLeft, axisRect.left() - dx ) );
                }
                else
                {
                    const double minLeft = leftScaleRect.left();
                    const double left = axisRect.left() + leftOffset;
                    axisRect.setLeft( qwtMaxF( left, minLeft ) );
                }
            }
            else
            {
                if ( m_data->alignCanvasToScales[YLeft] && leftOffset < 0 )
                {
                    canvasRect.setLeft( qwtMaxF( canvasRect.left(),
                        axisRect.left() - leftOffset ) );
                }
                else
                {
                    if ( leftOffset > 0 )
                        axisRect.setLeft( axisRect.left() + leftOffset );
                }
            }

            const QRectF& rightScaleRect = scaleRect[YRight];
            const int rightOffset = backboneOffset[YRight] - endDist + 1;

            if ( rightScaleRect.isValid() )
            {
                const double dx = rightOffset + rightScaleRect.width();
                if ( m_data->alignCanvasToScales[YRight] && dx < 0 )
                {
                    /*
                       The axis needs more space than the width
                       of the right scale.
                     */
                    const double cRight = canvasRect.right(); // qreal -> double
                    canvasRect.setRight( qwtMinF( cRight, axisRect.right() + dx ) );
                }

                const double maxRight = rightScaleRect.right();
                const double right = axisRect.right() - rightOffset;
                axisRect.setRight( qwtMinF( right, maxRight ) );
            }
            else
            {
                if ( m_data->alignCanvasToScales[YRight] && rightOffset < 0 )
                {
                    canvasRect.setRight( qwtMinF( canvasRect.right(),
                        axisRect.right() + rightOffset ) );
                }
                else
                {
                    if ( rightOffset > 0 )
                        axisRect.setRight( axisRect.right() - rightOffset );
                }
            }
        }
        else // YLeft, YRight
        {
            const QRectF& bottomScaleRect = scaleRect[XBottom];
            const int bottomOffset = backboneOffset[XBottom] - endDist + 1;

            if ( bottomScaleRect.isValid() )
            {
                const double dy = bottomOffset + bottomScaleRect.height();
                if ( m_data->alignCanvasToScales[XBottom] && dy < 0 )
                {
                    /*
                       The axis needs more space than the height
                       of the bottom scale.
                     */
                    const double cBottom = canvasRect.bottom(); // qreal -> double
                    canvasRect.setBottom( qwtMinF( cBottom, axisRect.bottom() + dy ) );
                }
                else
                {
                    const double maxBottom = bottomScaleRect.top() +
                        m_data->layoutData.scale[XBottom].tickOffset;
                    const double bottom = axisRect.bottom() - bottomOffset;
                    axisRect.setBottom( qwtMinF( bottom, maxBottom ) );
                }
            }
            else
            {
                if ( m_data->alignCanvasToScales[XBottom] && bottomOffset < 0 )
                {
                    canvasRect.setBottom( qwtMinF( canvasRect.bottom(),
                        axisRect.bottom() + bottomOffset ) );
                }
                else
                {
                    if ( bottomOffset > 0 )
                        axisRect.setBottom( axisRect.bottom() - bottomOffset );
                }
            }

            const QRectF& topScaleRect = scaleRect[XTop];
            const int topOffset = backboneOffset[XTop] - startDist;

            if ( topScaleRect.isValid() )
            {
                const double dy = topOffset + topScaleRect.height();
                if ( m_data->alignCanvasToScales[XTop] && dy < 0 )
                {
                    /*
                       The axis needs more space than the height
                       of the top scale.
                     */
                    const double cTop = canvasRect.top(); // qreal -> double
                    canvasRect.setTop( qwtMaxF( cTop, axisRect.top() - dy ) );
                }
                else
                {
                    const double minTop = topScaleRect.bottom() -
                        m_data->layoutData.scale[XTop].tickOffset;

                    const double top = axisRect.top() + topOffset;
                    axisRect.setTop( qwtMaxF( top, minTop ) );
                }
            }
            else
            {
                if ( m_data->alignCanvasToScales[XTop] && topOffset < 0 )
                {
                    canvasRect.setTop( qwtMaxF( canvasRect.top(),
                        axisRect.top() - topOffset ) );
                }
                else
                {
                    if ( topOffset > 0 )
                        axisRect.setTop( axisRect.top() + topOffset );
                }
            }
        }
    }

    /*
       The canvas has been aligned to the scale with largest
       border distances. Now we have to realign the other scale.
     */

    for ( int axis = 0; axis < AxisCount; axis++ )
    {
        QRectF& sRect = scaleRect[axis];

        if ( !sRect.isValid() )
            continue;

        if ( isXAxis( axis ) )
        {
            if ( m_data->alignCanvasToScales[YLeft] )
            {
                double y = canvasRect.left() - m_data->layoutData.scale[axis].start;
                if ( !( options & IgnoreFrames ) )
                    y += m_data->layoutData.canvas.contentsMargins[YLeft];

                sRect.setLeft( y );
            }

            if ( m_data->alignCanvasToScales[YRight] )
            {
                double y = canvasRect.right() - 1 + m_data->layoutData.scale[axis].end;
                if ( !( options & IgnoreFrames ) )
                    y -= m_data->layoutData.canvas.contentsMargins[YRight];

                sRect.setRight( y );
            }

            if ( m_data->alignCanvasToScales[ axis ] )
            {
                if ( axis == XTop )
                    sRect.setBottom( canvasRect.top() );
                else
                    sRect.setTop( canvasRect.bottom() );
            }
        }
        else
        {
            if ( m_data->alignCanvasToScales[XTop] )
            {
                double x = canvasRect.top() - m_data->layoutData.scale[axis].start;
                if ( !( options & IgnoreFrames ) )
                    x += m_data->layoutData.canvas.contentsMargins[XTop];

                sRect.setTop( x );
            }

            if ( m_data->alignCanvasToScales[XBottom] )
            {
                double x = canvasRect.bottom() - 1 + m_data->layoutData.scale[axis].end;
                if ( !( options & IgnoreFrames ) )
                    x -= m_data->layoutData.canvas.contentsMargins[XBottom];

                sRect.setBottom( x );
            }

            if ( m_data->alignCanvasToScales[ axis ] )
            {
                if ( axis == YLeft )
                    sRect.setRight( canvasRect.left() );
                else
                    sRect.setLeft( canvasRect.right() );
            }
        }
    }
}

/*!
   \brief Recalculate the geometry of all components.

   \param plot Plot to be layout
   \param plotRect Rectangle where to place the components
   \param options Layout options

   \sa invalidate(), titleRect(), footerRect()
      legendRect(), scaleRect(), canvasRect()
 */
void QwtPlotLayout::activate( const QwtPlot* plot,
    const QRectF& plotRect, Options options )
{
    invalidate();

    QRectF rect( plotRect );  // undistributed rest of the plot rect

    // We extract all layout relevant parameters from the widgets,
    // and save them to m_data->layoutData.

    m_data->layoutData.init( plot, rect );

    if ( !( options & IgnoreLegend )
        && plot->legend() && !plot->legend()->isEmpty() )
    {
        m_data->legendRect = layoutLegend( options, rect );

        // subtract m_data->legendRect from rect

        const QRegion region( rect.toRect() );
        rect = region.subtracted( m_data->legendRect.toRect() ).boundingRect();

        switch ( m_data->legendPos )
        {
            case QwtPlot::LeftLegend:
            {
                rect.setLeft( rect.left() + spacing() );
                break;
            }
            case QwtPlot::RightLegend:
            {
                rect.setRight( rect.right() - spacing() );
                break;
            }
            case QwtPlot::TopLegend:
            {
                rect.setTop( rect.top() + spacing() );
                break;
            }
            case QwtPlot::BottomLegend:
            {
                rect.setBottom( rect.bottom() - spacing() );
                break;
            }
        }
    }

    /*
     +---+-----------+---+
     |       Title       |
     +---+-----------+---+
     |   |   Axis    |   |
     +---+-----------+---+
     | A |           | A |
     | x |  Canvas   | x |
     | i |           | i |
     | s |           | s |
     +---+-----------+---+
     |   |   Axis    |   |
     +---+-----------+---+
     |      Footer       |
     +---+-----------+---+
     */

    // title, footer and axes include text labels. The height of each
    // label depends on its line breaks, that depend on the width
    // for the label. A line break in a horizontal text will reduce
    // the available width for vertical texts and vice versa.
    // expandLineBreaks finds the height/width for title, footer and axes
    // including all line breaks.

    using namespace QwtAxis;

    int dimTitle, dimFooter, dimAxes[AxisCount];
    expandLineBreaks( options, rect, dimTitle, dimFooter, dimAxes );

    if ( dimTitle > 0 )
    {
        m_data->titleRect.setRect(
            rect.left(), rect.top(), rect.width(), dimTitle );

        rect.setTop( m_data->titleRect.bottom() + m_data->spacing );

        if ( m_data->layoutData.scale[YLeft].isEnabled !=
            m_data->layoutData.scale[YRight].isEnabled )
        {
            // if only one of the y axes is missing we align
            // the title centered to the canvas

            m_data->titleRect.setX( rect.left() + dimAxes[YLeft] );
            m_data->titleRect.setWidth( rect.width()
                - dimAxes[YLeft] - dimAxes[YRight] );
        }
    }

    if ( dimFooter > 0 )
    {
        m_data->footerRect.setRect(
            rect.left(), rect.bottom() - dimFooter, rect.width(), dimFooter );

        rect.setBottom( m_data->footerRect.top() - m_data->spacing );

        if ( m_data->layoutData.scale[YLeft].isEnabled !=
            m_data->layoutData.scale[YRight].isEnabled )
        {
            // if only one of the y axes is missing we align
            // the footer centered to the canvas

            m_data->footerRect.setX( rect.left() + dimAxes[YLeft] );
            m_data->footerRect.setWidth( rect.width()
                - dimAxes[YLeft] - dimAxes[YRight] );
        }
    }

    m_data->canvasRect.setRect(
        rect.x() + dimAxes[YLeft],
        rect.y() + dimAxes[XTop],
        rect.width() - dimAxes[YRight] - dimAxes[YLeft],
        rect.height() - dimAxes[XBottom] - dimAxes[XTop] );

    for ( int axis = 0; axis < AxisCount; axis++ )
    {
        // set the rects for the axes

        if ( dimAxes[axis] )
        {
            int dim = dimAxes[axis];
            QRectF& scaleRect = m_data->scaleRect[axis];

            scaleRect = m_data->canvasRect;
            switch ( axis )
            {
                case YLeft:
                {
                    scaleRect.setX( m_data->canvasRect.left() - dim );
                    scaleRect.setWidth( dim );
                    break;
                }
                case YRight:
                {
                    scaleRect.setX( m_data->canvasRect.right() );
                    scaleRect.setWidth( dim );
                    break;
                }
                case XBottom:
                {
                    scaleRect.setY( m_data->canvasRect.bottom() );
                    scaleRect.setHeight( dim );
                    break;
                }
                case XTop:
                {
                    scaleRect.setY( m_data->canvasRect.top() - dim );
                    scaleRect.setHeight( dim );
                    break;
                }
            }
            scaleRect = scaleRect.normalized();
        }
    }

    // +---+-----------+---+
    // |  <-   Axis   ->   |
    // +-^-+-----------+-^-+
    // | | |           | | |
    // |   |           |   |
    // | A |           | A |
    // | x |  Canvas   | x |
    // | i |           | i |
    // | s |           | s |
    // |   |           |   |
    // | | |           | | |
    // +-V-+-----------+-V-+
    // |   <-  Axis   ->   |
    // +---+-----------+---+

    // The ticks of the axes - not the labels above - should
    // be aligned to the canvas. So we try to use the empty
    // corners to extend the axes, so that the label texts
    // left/right of the min/max ticks are moved into them.

    alignScales( options, m_data->canvasRect, m_data->scaleRect );

    if ( !m_data->legendRect.isEmpty() )
    {
        // We prefer to align the legend to the canvas - not to
        // the complete plot - if possible.

        m_data->legendRect = alignLegend( m_data->canvasRect, m_data->legendRect );
    }
}
