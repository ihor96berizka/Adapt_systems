#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>

#include <vector>

#include <QtCharts/QScatterSeries>
#include <QtCharts/QChartView>

using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QMenu* m_dataMenu;
    QMenu* m_networkMenu;
    QAction* m_genData;
    QAction* m_clearData;
    QAction* m_teachNetwork;
    QAction* m_aprox;

    QScatterSeries* m_inputDataSeries;
    QScatterSeries* m_aproxDataSeries;
    QChart* m_chart;
    QChartView* m_chartView;

    void createMenus();
    void createActions();

    const double disp = 1.5;
    const double E0 = 0.1;
    const double learningRate = 0.1;
    const int maxIter = 50;
    const int patternCount = 2;
    const int rbf_count = 2;
    const int w_count = rbf_count + 1;
    //вектор ваг
    std::vector<double> w;
    // вхідні точки і бажаний вихід
    std::vector<double> t;
    std::vector<double> f;
    //центри рбф функцій
    std::vector<double> c;

    double gauss(double x, double c);
    bool isDone(std::vector<bool>& vec) const;
    void localError();
private slots:
    void generateDataSlot();
    void clearDataSlot();
    void teachNetworkSlot();
    void aproxDataSlot();
};

#endif // MAINWINDOW_H
