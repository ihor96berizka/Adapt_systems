#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

using Func = double(double);

size_t fib(size_t n)
{
    size_t previous = 1;
    size_t current = 1;
    size_t next = 1;
    for (size_t i = 3; i <= n; ++i)
    {
        next = current + previous;
        previous = current;
        current = next;
    }
    return next;
}

double f1(double x)
{
    return (x-15) * (x-15) + 5.0;
}
double f(double x1, double x2)
{
    return 100 * (x2 - x1 * x1) * (x2 - x1 * x1) + (1 - x1) * (1 - x1);
}
double dF_dX(double x1, double x2)
{
    double res = (-4 * x1 * x2 + 4 * x1 * x1 * x1);
    return 100 * res - 2 + 2 * x1;
}
double dF_dY(double x1, double x2)
{
    double res = (2 * x2 - 2 * x1 * x1);
    return 100 * res;
}
void grad_min()
{
    const int maxK = 250000;
    double x1 = -1;
    double x2 = 1;
    double eps = 1e-5;
    double alp = 1e-3;
    int k = 0;
    double f_grad = 1000000;
    do
    {
        f_grad = sqrt(dF_dX(x1, x2) * dF_dX(x1, x2) + dF_dY(x1, x2) * dF_dY(x1, x2));
        k++;
        double dx = dF_dX(x1, x2);
        double dy = dF_dY(x1, x2);
        x1 -= alp * dx;
        x2 -= alp * dy;
    } while (abs(f_grad) > eps && abs(f(x1, x2)) > eps);
    double y_min = f(x1, x2);
    cout << "y_min = " << y_min <<
            "\n k = " << k <<
            "\n x1 = " << x1 <<
            "\n x2 = " << x2 << endl;
}
std::pair<double, double> fib_min(double a, double b, size_t n, Func callable)
{
    double x1 = a + double(fib(n-2)) / fib(n) * (b-a);
    double x2 = a + double(fib(n-1)) / fib(n) * (b-a);

    double y1 = callable(x1);
    double y2 = callable(x2);

    for (size_t k = n-1; k>2; k--)
    {
        if (y1 < y2)
        {
            b = x2;
            x2 = x1;
            x1 = a + double(fib(k-3)) / fib(k-1)*(b-a);
        }
        else
        {
            a = x1;
            x1 = x2;
            x2 = a + double(fib(k-2)) / fib(k-1) * (b-a);
        }
        y1 = callable(x1);
        y2 = callable(x2);
    }
    double x = (x1 + x2) / 2.0;
    double y = callable(x);
    cout << "\nIter = " << n-2;
    return {x, y};
}
std::pair<double, double> phi_min(double a, double b, double eps, Func callable)
{
    double k = 0.618;

    double x1 = a + (1-k) * (b-a);
    double x2 = a + k *(b-a);

    double y1 = callable(x1);
    double y2 = callable(x2);

    size_t iter = 1;
    while(abs(b-a) > eps)
    {
        iter++;
        if (y1 < y2)
        {
            b = x2;
            x2 = x1;
            x1 = a + (1-k) * (b-a);
        }
        else
        {
            a = x1;
            x1 = x2;
            x2 = a + k*(b-a);
        }
        y1 = callable(x1);
        y2 = callable(x2);
    }
    double x = (a+b)/2.0;
    double y = callable(x);

    std::cout << "\nIter: " << iter;
    return {x, y};
}
int main()
{
    cout << "Hello World!" << endl;
    double a = 12;
    double b = 20;
    double eps = 1e-12;
    size_t n = 90;

    cout << "fib(" << n << ") = " << fib(n);
    auto res = fib_min(a, b, n, f1);
    cout << "\nfib_method: " <<
            "y = " << res.second <<
            "\tx = " << res.first << "\n";

    cout << "eps = " << eps;
    res = phi_min(a, b, eps, f1);
        cout << "\nphi_method: " <<
                "y = " << res.second <<
                "\tx = " << res.first << "\n";

    return 0;
}
