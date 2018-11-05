#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>
#include <random>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createActions();
    createMenus();

    m_inputDataSeries = nullptr;
    m_chart = new QChart();
    m_chartView = new QChartView(m_chart, this);

    m_inputDataSeries = new QScatterSeries();
    m_inputDataSeries->setName("input data");
    m_inputDataSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    m_inputDataSeries->setMarkerSize(10);

    m_chart->addSeries(m_inputDataSeries);
    m_chart->createDefaultAxes();

    setCentralWidget(m_chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenus()
{
    m_dataMenu = menuBar()->addMenu(tr("Data"));
    m_dataMenu->addActions({m_genData, m_clearData});

    m_networkMenu = menuBar()->addMenu(tr("Network"));
    m_networkMenu->addActions({m_teachNetwork, m_aprox});
}

void MainWindow::createActions()
{
    m_genData = new QAction(tr("Generate"), this);
    connect(m_genData, &QAction::triggered, this, &MainWindow::generateDataSlot);

    m_clearData = new QAction(tr("Clear"), this);
    connect(m_clearData, &QAction::triggered, this, &MainWindow::clearDataSlot);

    m_teachNetwork = new QAction(tr("Teach"), this);
    connect(m_teachNetwork, &QAction::triggered, this, &MainWindow::teachNetworkSlot);

    m_aprox = new QAction(tr("Aproximate"), this);
    connect(m_aprox, &QAction::triggered, this, &MainWindow::aproxDataSlot);
}

double MainWindow::gauss(double x, double c)
{
    double arg = -1.0 / (2 * disp);
    arg *= (x - c) * (x - c);
    return exp(arg);
}

bool MainWindow::isDone(std::vector<bool> &vec) const
{
    for (const auto& item : vec)
    {
        if (item == false)
            return false;
    }
    return true;
}

void MainWindow::localError()
{
    std::vector<bool> done(patternCount);

    double E = 0;
    std::vector<double> dEdw(w_count, 0);

    //вектор виходу мережі
    std::vector<double> out(patternCount, 0);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0, 1.0);

    w.reserve(w_count);
    for (int i = 0; i < w_count; ++i)
    {
        w.push_back(distribution(generator));
    }
    int currentIter = 0;
    do{
        currentIter++;
        qDebug() << "Current iter: " << currentIter;
        //для всіх шаблонів
        for (int i = 0; i < patternCount; ++i)
        {
            E = 0;
            //порахувати вихід мережі для і-тої точки
            for (int p = 0; p < w_count; ++p)
            {
                if (p != 0)
                {
                    double g = gauss(t[i], c[p-1]);
                    out[i] += g * w[p];
                    //dEdw[p] = gauss(t[i], c[p]);
                }
                else
                {
                    out[i] += w[p];
                    //dEdw[p] = 1;
                }
            }
            //порахувати похибку
            E = 0.5 * (out[i] - f[i]) * (out[i] - f[i]);
            qDebug() << "out[" << i << "] = " << out[i];
            qDebug() << "local E[" << i << "] = " << E;
            if (E < E0)
            {
                done[i] = true;
            }
            else
            {
                done[i] = false;
            }

            for (int j = 0; j < w_count; ++j)
            {
                if (j != 0)
                {
                    dEdw[j] = (out[i] - f[i]) * gauss(t[i], c[j-1]);
                }//bias
                else
                {
                    dEdw[j] = out[i] - f[i];
                }
                w[j] -= learningRate * dEdw[j];
                qDebug() << "w[" << j << "] = " << w[j];
                qDebug() << "dEdw[" << j << "] = " << dEdw[j];

            }
            qDebug() << "pattern # " << i << "\n";
        }
        qDebug() << "done vector:";
        for (int i = 0; i < patternCount; ++i)
        {
            qDebug() << "done[" << i << "] = " << done[i];
        }
    }while (!isDone(done) && currentIter < maxIter);
    qDebug() <<"current iter < maxIter? " << (currentIter < maxIter);
}

void MainWindow::generateDataSlot()
{
    const double pi = 3.14;
    double a = 0;
    double b = 4;//2 * pi;
    double h = (b - a) / patternCount;
    f.reserve(patternCount);
    t.reserve(patternCount);
    c.reserve(rbf_count);

    for (int i = 0; i < patternCount; ++i)
    {
        f.push_back(/*sin(a)*/a);
        t.push_back(a);
        a += h;
        m_inputDataSeries->append(t[i], f[i]);
        qDebug() << "t = " << t[i] << "; "
                 << "f = " << f[i] << "; "
                 << "i = " << i
                 << "series.size = " << m_inputDataSeries->count();

    }

    if (rbf_count == patternCount)
    {
        c = t;
    }
    m_chart->axisX()->setRange(*std::min_element(t.cbegin(), t.cend()),
                               *std::max_element(t.cbegin(), t.cend()));
    m_chart->axisY()->setRange(*std::min_element(f.cbegin(), f.cend()),
                               *std::max_element(f.cbegin(), f.cend()));
}

void MainWindow::clearDataSlot()
{
    m_inputDataSeries->clear();
    f.clear();
    t.clear();
}

void MainWindow::teachNetworkSlot()
{
/*
    std::vector<bool> isDone(patternCount);

    double E = 0;
    std::vector<double> dEdw(w_count, 0);

    //вектор виходу мережі
    std::vector<double> out(patternCount, 0);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0,1.0);

    w.reserve(w_count);
    for (int i = 0; i < w_count; ++i)
    {
        w.push_back(0);//(distribution(generator));
    }
    int currentIter = 0;
    do{
        currentIter++;
        E = 0;
        //для всіх шаблонів
        for (int i = 0; i < patternCount; ++i)
        {
            //порахувати вихід мережі для і-тої точки
            for (int p = 0; p < w_count; ++p)
            {
                if (p != 0)
                {
                    double g = gauss(t[i], c[p-1]);
                    out[i] += g * w[p];
                    //dEdw[p] = gauss(t[i], c[p]);
                }
                else
                {
                    out[i] += w[p];
                    //dEdw[p] = 1;
                }
            }

        }
        //порахувати градієнт і похибку
        for (int j = 0; j < w_count; j++)
        {

            for (int i = 0; i < patternCount; ++i)
            {
                if (j!=0)
                {
                    dEdw[j] += (out[i] - f[i]) * gauss(t[i], c[j-1]);
                }
                else
                {
                    dEdw[j] += (out[i] - f[i]);
                }
            }
            qDebug() << "dEdw[" << j << "] = " << dEdw[j];
        }
        for (int i = 0; i < patternCount; ++i)
        {
            E += (out[i] - f[i]) * (out[i] - f[i]);
            qDebug() << "out [" << i << "] = " << out[i]
                        << "; f [" << i << "] = " << f[i];
        }
        E /=patternCount;
        qDebug() << "E = " << E;
        for (int j = 0; j < w_count; ++j)
        {
            w[j] -= learningRate * dEdw[j];
            qDebug() << "w[" << j << "] = " << w[j];
        }
    }while (E > E0 && currentIter < maxIter);
    qDebug() <<"current iter < maxIter? " << (currentIter < maxIter);
*/
    localError();
}

void MainWindow::aproxDataSlot()
{

}
