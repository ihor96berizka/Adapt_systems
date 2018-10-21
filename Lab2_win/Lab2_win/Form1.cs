using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab2_win
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        List<double> x_val = new List<double>();
        List<double> y_val = new List<double>();
        const double k = 0.618;

        private int fib(int n)
        {
            if (n >= 2)
            {
                return fib(n - 1) + fib(n - 2);
            }
            else
            {
                return 1;
            }
        }
        private double f(double x)
        {
            return Math.Pow(x - 15, 2) + 5;
        }
        private void button1_Click(object sender, EventArgs e)
        {
            double a = Convert.ToDouble(textBox1.Text);
            double b = Convert.ToDouble(textBox2.Text);
            double dx = 0.1;
            while (a <= b)
            {
                x_val.Add(a);
                y_val.Add(f(a));
                a += dx;
            }

            for (int i = 0; i < x_val.Count; i++)
            {
                chart1.Series[0].Points.AddXY(x_val[i], y_val[i]);
            }
            MessageBox.Show("theor min y = " + y_val.Min().ToString());
            x_val.Clear();
            y_val.Clear();
           
        }

        private void button2_Click(object sender, EventArgs e)
        {
            double a = Convert.ToDouble(textBox1.Text);
            double b = Convert.ToDouble(textBox2.Text);
            double esp = Convert.ToDouble(textBox3.Text);
            
            //метод золотого перетину
            if (selectMethodBox.SelectedIndex == 0)
            {
                double x1 = a + (1 - k) * (b - a);
                double x2 = a + k * (b - a);
                int iterCount = 0;
                while (Math.Abs(b-a) > esp)
                {
                    iterCount++;
                    double y1 = f(x1);
                    double y2 = f(x2);
                   // min += "\na = " + a.ToString() + "; b = " + b.ToString() + ";\ny1 = " + y1.ToString() +
                   //     "; y2 = " + y2.ToString();
                    if (y1 < y2)
                    {
                        b = x2;
                        x1 = a + (1 - k) * (b - a);
                        x2 = x1;
                    }
                    else
                    {
                        a = x1;
                        x1 = x2;
                        x2 = a + k * (b - a);
                    }
                }
                double min = (a + b) / 2.0;
                //MessageBox.Show(a.ToString());
                textBox4.Text = f(min).ToString();
                textBox5.Text = min.ToString();
                textBox6.Text = iterCount.ToString();

            }//мутод фібоначчі
            else if (selectMethodBox.SelectedIndex == 1)
            {
                int n = Convert.ToInt32(textBox6.Text);
                double x1 = a + (double)fib(n - 2) / fib(n) * (b - a);
                double x2 = a + (double)fib(n - 1) / fib(n) * (b - a);
               
                for (int k = n; k > 1; k--)
                {
                    double y1 = f(x1);
                    double y2 = f(x2);
                    if (y1 < y2)
                    {
                        b = x2;
                        x1 = a + (double)fib(k - 3) / fib(k - 1) * (b - a);
                        x2 = x1;
                    }
                    else
                    {
                        a = x1;
                        x1 = x2;
                        x2 = a + (double)fib(k - 2) / fib(k - 1) * (b - a);
                    }
                }
                double min = (x1 + x2)/2.0;
                textBox4.Text = f(min).ToString();
                textBox5.Text = min.ToString();

            }
            else
            {
                MessageBox.Show("Виберіть метод");
            }
             x_val.Clear();
             y_val.Clear();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            chart1.Series[0].Points.Clear();
        }
    }
}
