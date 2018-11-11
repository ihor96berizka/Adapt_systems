#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>

#include <vector>

using namespace QtCharts;
namespace Ui {
class MainWindow;
}
struct ResponseData
{
    ResponseData() {}
    std::vector<double> f;
    std::vector<double> amp;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum class RESPONSE: uint8_t
    {
        Frequncy = 0,
        Phase = 1
    };
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    double freq_cutoff = 1.5;
    double freq_sampling = 8;
    double attenuation = 50;
    double perBandpass = 0.5;
    std::vector<double> h;
    QMenu* filterMenu;
    QMenu* plotMenu;

    QAction* calcHdAct;
    QAction* freqRespPlotAct;
    QAction* phaseResp;
    QAction* clearPlotAct;

    QLineSeries* freqRespSeries;
    QScatterSeries* phaseRespSeries;
    QChart* m_chart;
    QChartView* m_chartView;
    void createMenus();
    void createActions();
    ResponseData calcResponse(RESPONSE type, double min_freq, double max_freq);
private slots:
    void calculateHSlot();
    void freqRespPlotSlot();
    void clearPlotSlot();
    void phaseRespSlot();
};

#endif // MAINWINDOW_H
