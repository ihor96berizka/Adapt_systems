using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Lab3
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        double f(double x1, double x2)
        {
            //return 100 * (x2 * x2 - 2 * x1 * x1 * x2 + x1 * x1 * x1 * x1) + 1 - 2 * x1 + x1 * x1;
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
        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            const int maxK = 25000;
            double x1 = Convert.ToDouble(textBox1.Text);
            double x2 = Convert.ToDouble(textBox7.Text);
            double eps = Convert.ToDouble(textBox2.Text);
            double alp = Convert.ToDouble(textBox8.Text);
            //постійний крок
            if (comboBox1.SelectedIndex == 0)
            {
                //alp = 0.00001;
                int k = 0;
                double f_grad = 1000000;
                do
                {
                    f_grad = Math.Sqrt(dF_dX(x1, x2) * dF_dX(x1, x2) + dF_dY(x1, x2) * dF_dY(x1, x2));
                    k++;
                    double dx = dF_dX(x1, x2);
                    double dy = dF_dY(x1, x2);
                    x1 -= alp * dx;
                    x2 -= alp * dy;
                } while (Math.Abs(f_grad) > eps && Math.Abs(f(x1, x2)) > eps);
                double y_min = f(x1, x2);
                textBox3.Text = y_min.ToString();
                textBox5.Text = k.ToString();
                textBox4.Text = x1.ToString();
                textBox6.Text = x2.ToString();
            }//змінний крок alp
            else if (comboBox1.SelectedIndex == 1)
            {
                double y_min = double.MaxValue;
                double delta = 0.1;
                int k = 0;
                double f_grad = 1000000;
                double f_val_old = f(x1, x2);
                double eps_alp = 1e-8;
                double dx = dF_dX(x1, x2);
                double dy = dF_dY(x1, x2);
                x1 -= alp * dx;
                x2 -= alp * dy;
                double f_val_new = f(x1, x2);
                f_grad = Math.Sqrt(dF_dX(x1, x2) * dF_dX(x1, x2) + dF_dY(x1, x2) * dF_dY(x1, x2));
                while (/*Math.Abs(Math.Abs(f_val_old) - Math.Abs(f_val_new)) > eps
                     k < maxK && */Math.Abs(f_grad) > eps)
                {
                    if (Math.Abs(f_val_new) > Math.Abs(f_val_old))
                    {
                        // MessageBox.Show("div alp!");
                        if (alp < eps_alp)
                        {
                            MessageBox.Show("alp < esp(alp)\n");
                            y_min = f_val_old;
                            break;
                        }
                        alp /= 2;
                    }
                    else
                    {
                        f_grad = Math.Sqrt(dF_dX(x1, x2) * dF_dX(x1, x2) + dF_dY(x1, x2) * dF_dY(x1, x2));
                        dx = dF_dX(x1, x2);
                        dy = dF_dY(x1, x2);
                        x1 -= alp * dx;
                        x2 -= alp * dy;
                        f_val_old = f_val_new;
                        f_val_new = f(x1, x2);
                        y_min = f_val_new;
                    }
                    k++;
                }
                MessageBox.Show("alp = " + alp.ToString()
                    + "\n"
                    + "abs(old - new) = " + Math.Abs(f_val_old - f_val_new).ToString()
                    + "|grad| = " + f_grad.ToString());
                //f(x1, x2);//f(x1, x2);
                textBox3.Text = y_min.ToString();
                textBox5.Text = k.ToString();
                textBox4.Text = x1.ToString();
                textBox6.Text = x2.ToString();
            }
            //найшвидший спуск
            else if (comboBox1.SelectedIndex == 2)
            {
                //alp = 0.00001;
                int k = 0;
                double alp_min = 75e-6;
                double alp_max = 5e-5;

                double f_grad = 1000000;
                do
                {
                    f_grad = Math.Sqrt(dF_dX(x1, x2) * dF_dX(x1, x2) + dF_dY(x1, x2) * dF_dY(x1, x2));
                    k++;
                    double dx = dF_dX(x1, x2);
                    double dy = dF_dY(x1, x2);
                    //tab f(x1, x2) to find optimal alp
                    double f_opt = 1000000;
                    double f_current = 1000000;
                    for (double alp_current = alp_min; alp_current <= alp_max; alp_current *= 5)
                    {
                        f_current = f(x1 + alp_current, x2 + alp_current);
                        if (f_current < f_opt)
                        {
                            f_opt = f_current;
                            alp = alp_current;
                        }
                    }
                    x1 -= alp * dx;
                    x2 -= alp * dy;
                } while (Math.Abs(f_grad) > eps && Math.Abs(f(x1, x2)) > eps);
                double y_min = f(x1, x2);
                textBox3.Text = y_min.ToString();
                textBox5.Text = k.ToString();
                textBox4.Text = x1.ToString();
                textBox6.Text = x2.ToString();
            }
            else if (comboBox1.SelectedIndex == 3)//метод ньютона
            {
                alp = 0.0000025;
                int k = 0;
                double f_grad = 1000000;
                double H = 202 * 200 - 200 * 200;
                do
                {
                    f_grad = Math.Sqrt(dF_dX(x1, x2) * dF_dX(x1, x2) + dF_dY(x1, x2) * dF_dY(x1, x2));
                    k++;
                    double dx = dF_dX(x1, x2);
                    double dy = dF_dY(x1, x2);
                    x1 -= alp * H * dx;
                    x2 -= alp * H * dy;
                } while (Math.Abs(f_grad) > eps && Math.Abs(f(x1, x2)) > eps);
                double y_min = f(x1, x2);
                textBox3.Text = y_min.ToString();
                textBox5.Text = k.ToString();
                textBox4.Text = x1.ToString();
                textBox6.Text = x2.ToString();
            }
        }
    }
}
