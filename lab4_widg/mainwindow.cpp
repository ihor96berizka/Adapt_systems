#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <vector>
#include <cmath>
#include <QDebug>
#include <QChart>
#include <QChartView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    createActions();
    createMenus();

    freqRespSeries = nullptr;
    m_chart = new QChart();
    m_chartView = new QChartView(m_chart, this);

    freqRespSeries = new QLineSeries();
    freqRespSeries->setName("Frequency response");
    phaseRespSeries = new QScatterSeries();
    phaseRespSeries->setName("Phase response");
    phaseRespSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    phaseRespSeries->setMarkerSize(10);

    m_chart->addSeries(freqRespSeries);
    m_chart->addSeries(phaseRespSeries);
    m_chart->createDefaultAxes();

    setCentralWidget(m_chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{
    filterMenu = menuBar()->addMenu(tr("Filter"));
    filterMenu->addAction(calcHdAct);

    plotMenu = menuBar()->addMenu(tr("Plot"));
    plotMenu->addAction(freqRespPlotAct);
    plotMenu->addAction(phaseResp);
    plotMenu->addAction(clearPlotAct);
}

void MainWindow::calculateHSlot()
{
    double dF = perBandpass / freq_sampling;
    qDebug() << "dF = " << dF;
    size_t N = 3.3 / dF + 1;
    qDebug() << "N = " << N;

    double freq_cutoff_new = freq_cutoff + perBandpass / 2;
    qDebug() << "freq_cutoff_new = " << freq_cutoff_new;

    double freq_cutoff_norm = freq_cutoff_new / freq_sampling;
    qDebug() << "freq_cutoff_norm = " << freq_cutoff_norm;

    std::vector<double> w(N/2+1, 0);
    for (size_t n = 0; n < N/2+1; n++)
    {
        w[n] = 0.54 + 0.46 * cos(2 * M_PI * n / 53);
    }

    std::vector<double> hd(N/2+1, 0);
    hd[0] = 2 * freq_cutoff_norm;
    //qDebug() << "hd[0] = " << hd[0];
    for (size_t n = 1; n < N/2+1; ++n)
    {
        hd[n] = 2 * freq_cutoff_norm * sin(n * 2 * M_PI * freq_cutoff_norm) / (n * 2 * M_PI * freq_cutoff_norm);
        // qDebug() << "hd[" << n << "] = " << hd[n];
    }

    std::vector<double> h(N/2+1, 0);
    for (size_t n = 0; n < N/2+1; ++n)
    {
        h[n] = hd[n] * w[n];
    }

    std::vector<double> h_fin(h.crbegin(), h.crend());
    h_fin.insert(h_fin.end(), h.cbegin()+1, h.cend());
    for (size_t n = 0; n < N; ++n)
    {
        qDebug() << "h_[" << n << "] = " << h_fin[n];
    }
    this->h = h_fin;
}

void MainWindow::freqRespPlotSlot()
{
    double min_freq = 0;
    double max_freq = 3;
    auto data = calcResponse(RESPONSE::Frequncy, min_freq, max_freq);
    for (size_t i = 0; i < data.amp.size(); ++i)
    {
        freqRespSeries->append(data.f[i], data.amp[i]);
    }
    m_chart->axisX()->setRange(min_freq, max_freq),
    m_chart->axisY()->setRange(*std::min_element(data.amp.cbegin(), data.amp.cend()),
                               (*std::max_element(data.amp.cbegin(), data.amp.cend()))* 1.1);
}

void MainWindow::clearPlotSlot()
{
    freqRespSeries->clear();
    phaseRespSeries->clear();
}

void MainWindow::phaseRespSlot()
{
    double min_freq = 0;
    double max_freq = 3;
    auto data = calcResponse(RESPONSE::Phase, min_freq, max_freq);
    for (size_t i = 0; i <data.f.size(); ++i)
    {
        phaseRespSeries->append(data.f[i], data.amp[i]);
    }
    m_chart->axisX()->setRange(min_freq, max_freq);
    m_chart->axisY()->setRange(*std::min_element(data.amp.cbegin(), data.amp.cend()),
                               (*std::max_element(data.amp.cbegin(), data.amp.cend())) * 1.1);
}

void MainWindow::createActions()
{
    calcHdAct = new QAction(tr("Calculate h(d)"), this);
    connect(calcHdAct, &QAction::triggered, this, &MainWindow::calculateHSlot);

    freqRespPlotAct = new QAction(tr("|H(jw)|"), this);
    connect(freqRespPlotAct, &QAction::triggered, this, &MainWindow::freqRespPlotSlot);

    clearPlotAct = new QAction(tr("Clear"), this);
    connect(clearPlotAct, &QAction::triggered, this, &MainWindow::clearPlotSlot);

    phaseResp = new QAction(tr("Phase response"), this);
    connect(phaseResp, &QAction::triggered, this, &MainWindow::phaseRespSlot);
}

ResponseData MainWindow::calcResponse(MainWindow::RESPONSE type, double min_freq, double max_freq)
{
    size_t N = 500;
    double df = (max_freq - min_freq) / N;
    std::vector<double> data(N, 0);
    std::vector<double> w(N, 0);
    std::vector<double> freq(N, 0);

    double Ts = 1/freq_sampling;
    qDebug() << "Ts = " << Ts;
    for (size_t k = 0; k < N; ++k)
    {
        double currentFreq = (min_freq + df * k);
        freq[k] = currentFreq;
        w[k] = 2 * M_PI * currentFreq;
        double sin_sum = 0;
        double cos_sum = 0;
        for (size_t n = 0; n < h.size(); ++n)
        {
            sin_sum += (h[n] * sin(n * Ts * w[k]));
            cos_sum += (h[n] * cos(n * Ts * w[k]));
        }
        if (type == RESPONSE::Phase)
        {
            data[k] = atan2(sin_sum , cos_sum);
        }
        else if (type == RESPONSE::Frequncy)
        {
            data[k] = sqrt(sin_sum * sin_sum + cos_sum * cos_sum);
        }
      //
        qDebug() << "f[" << k << "] = " << freq[k] <<
                    "data[" << k << "] = " << data[k];
    }
    //m_chart->axisX()->setRange(min_freq, max_freq);
    //m_chart->axisY()->setRange(*std::min_element(data.cbegin(), data.cend()),
                      //         *std::max_element(data.cbegin(), data.cend()));
    ResponseData ret_data;
    ret_data.amp = data;
    ret_data.f = freq;
    return ret_data;
}
