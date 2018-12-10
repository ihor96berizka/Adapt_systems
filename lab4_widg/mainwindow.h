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
    double freq_cutoff = 2;
    double freq_sampling = 16;
    double attenuation = 45;
    double perBandpass = 0.5;
    std::vector<double> h;
    const size_t n = 128*2;
    const double alp = 1e-6;

    std::vector<double> data_signal;
    std::vector<double> filtered_signal;
    std::vector<double> ideal_signal;

    QMenu* filterMenu;
    QMenu* plotMenu;
    QMenu* signalMenu;

    QAction* calcHdAct;

    QAction* freqRespPlotAct;
    QAction* phaseResp;
    QAction* clearPlotAct;

    QAction* generateSignalAct;
    QAction* filterSignalAct;
    QAction* noiseSignalAct;
    QAction* lmsFilterAct;
    QAction* nlmsFilterAct;

    QLineSeries* freqRespSeries;
    QScatterSeries* phaseRespSeries;
    QLineSeries* signalSeries;

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
    void generateSignalSlot();
    void filterSignalSlot();
    void noiseSignalSlot();
    void lmsFilterSlot();
    void nlmsFilterSlot();
};

#endif // MAINWINDOW_H
