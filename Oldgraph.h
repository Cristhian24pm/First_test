#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QDockWidget>
#include <QComboBox>
#include <QPushButton>
#include <QFont>
#include <QFontDatabase>
#include <QDoubleSpinBox>
#include <QMessageBox>

#include "modules/interfaces/snom_if_ui.h"
#include "base/datamanager/datamanager.h"
#include "base/log/log.h"
#include "qcustomplot.h"
#include "base/snom_helper.h"

namespace Ui {
    class SNOM_GRAPH;
}

/**
 * @brief The PlotTypes enum
 */
enum SNOM_PlotTypes {
    TIME        = 0,
    FFTAMP      = 1,
    FFTPHASE    = 2,
    FFTPHASEL   = 3,
    OSC1        = 4,
    OSC2        = 5
};

/**
 * @brief The SNOM_PlotWidth enum
 *
 * enum for line-widths
 */
enum SNOM_PlotWidth {
    NORMAL = 1,
    LARGE  = 2,
    LARGER = 3
};

/**
 * @brief The Graph class
 *
 * provides an UI element, that displays plots
 */
class SNOM_GRAPH: public SNOM_UI {

    Q_OBJECT
    /**
     * @brief ui
     * the ui element
     */
    Ui::SNOM_GRAPH *ui;

private:
    /**
     * @brief _liveView
     *
     * a live dataset
     */
    SNOM_DataSet* _liveView;
    /**
     * @brief ampDialog
     * a dialog for showing current amplitude values
     */
    QDialog* ampDialog;
    /**
     * @brief _lookupTable
     * the lookuptable stores the uid of all graphs in the order they are plotted in the plotting backend
     */
    QVector<QUuid> _lookupTable;

    /**
     * @brief dragged
     * the currently dragged element
     */
    QCPAbstractItem* dragged;

    /**
     * @brief draggedY
     *
     * flag indicating, whether the dragging process is done along x or y axis
     */
    bool draggedY;

    /**
     * @brief _type
     *
     * the current plotType
     */
    SNOM_PlotTypes _type;

    /**
     * @brief _lineWidth
     * stores the current linewidth of the plot
     */
    SNOM_PlotWidth _lineWidth;

    /**
     * @brief _fftLines
     * a map which holds user creatable straight lines and assignes them to the corresponding plot uid
     */
    QMap<QUuid, QCPItemStraightLine*> _fftLines;

    /**
     * @brief _ranges
     * list storing the 4 range bars
     */
    QVector<QCPItemStraightLine*> _ranges;

    /**
     * @brief _rangesLabel
     * the label for the range deltas
     */
    QCPItemText* _rangesLabel;

    /**
     * @brief addLine
     * @param id the plot id
     *
     * adds the plot to the graph. internally calls drawGraph
     */
    void addLine(QUuid id);
    /**
     * @brief updateLine
     * @param id the plot id
     *
     * updates the plot in the graph
     */
    void updateLine(QUuid id);

    /**
     * @brief updateLines
     *
     * updates all plots of the graph
     */
    void updateLines();

    /**
     * @brief removeLine
     * @param id the plot id
     *
     * removes the plot from the graph
     */
    void removeLine(QUuid id);

    /**
     * @brief drawGraph
     * @param grID the internal plot id
     * @param set the dataset to plot
     *
     * adds the graph to the plotting backend
     */
    void drawGraph(int grID, SNOM_DataSet* set);

    /**
     * @brief rescaleToFit
     *
     * rescales the axes to fit all loaded graphs
     */
    void rescaleToFit();

    /**
     * @brief handleMousePress
     * @param event the mouse event
     *
     * mouse press/down handler
     */
    void handleMousePress(QMouseEvent *event);
    /**
     * @brief handleMouseMove
     * @param event the mouse event
     *
     * mouse move handler
     */
    void handleMouseMove(QMouseEvent *event);
    /**
     * @brief handleMouseRelease
     * @param event the mouse event
     */
    void handleMouseRelease(QMouseEvent *event);

    /**
     * @brief renameAxis
     * @param id the plot type
     *
     * sets the labels of the axes to the corresponding plot type
     */
    void renameAxis(int id);

    /**
     * @brief createLine
     * @param id plot id
     * @param uid data uid
     *
     * creates a line for the given dataset uid
     */
    void createLine(int id, QUuid uid);


    /**
     * @brief constrainAngle
     * @param x the angle
     * @return the constrained angle
     *
     * helper function to constrain the angle between -pi and pi
     */
    inline double constrainAngle(double x){
        x = fmod(x + M_PI,2*M_PI);
        if (x < 0)
            x += 2*M_PI;
        return x - M_PI;
    }
// convert to [-360,360]
    /**
     * @brief angleConv
     * @param angle the angle
     * @return the converted angle
     *
     *
     */
    inline double angleConv(double angle){
        return fmod(constrainAngle(angle),2*M_PI);
    }

    /**
     * @brief angleDiff
     * @param a the first angle
     * @param b the second angle
     * @return the difference between both angles
     */
    inline double angleDiff(double a,double b){
        double dif = fmod(b - a + M_PI,2*M_PI);
        if (dif < 0)
            dif += 2*M_PI;
        return dif - M_PI;
    }
    /**
     * @brief unwrap
     * @param previousAngle angle of the previous array position
     * @param newAngle angle of the current array position
     * @return the unwrapped angle
     *
     * unwraps the angles of e.g. the phase.
     */
    inline double unwrap(double previousAngle,double newAngle){
        //return newAngle;
        return previousAngle - angleDiff(newAngle,angleConv(previousAngle));
    }

    /**
     * @brief changeLineWidth
     * @param w the new line width
     *
     * changes the linewidth of all elements to the specified one
     */
    void changeLineWidth(SNOM_PlotWidth w);


public:
    /**
     * @brief SNOM_GRAPH
     * @param parent the parent widget
     */
    SNOM_GRAPH(QWidget *parent = nullptr);
    /**
     * @brief displayItem
     * @param id the item to display
     */
    void displayItem(int id);
    /**
     * @brief displayDataSet
     * @param set the dataset to display
     * @param live flag, whether the dataset is to be treated as live value
     */
    void displayDataSet(SNOM_DataSet* set, bool live = false);
    /**
     * @brief updateLive
     * @param set the live dataset
     *
     * updates the live dataset
     */
    void updateLive(SNOM_DataSet* set);

    /**
     * @brief removeItem
     * @param id the id of the item
     *
     * removes the item from the list
     */
    void removeItem(int id);
    //void removeItem(QUuid id);


    void setHWHandler(SNOM_LOOP* handler){};
    QString getType(){ return "Datamanager"; };
    void handleParameterChanged(QString par, QVariant var){};


    void setAmplitude(double amp);

    /**
     * @brief setXRange
     * @param r the range to set to
     *
     * sets the x-axis range to the given range
     */
    void setXRange(QCPRange r);
    /**
     * @brief setYRange
     * @param r the range to set to
     *
     * sets the y-axis range to the given range
     */
    void setYRange(QCPRange r);
};

#endif // GRAPH_H
