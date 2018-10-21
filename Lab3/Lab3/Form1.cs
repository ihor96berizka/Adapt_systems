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
            const int maxK = 250000;
            double x1 = -1;//Convert.ToDouble(textBox1.Text);
            double x2 = 1;//Convert.ToDouble(textBox7.Text);
            double eps = 1e-4;//Convert.ToDouble(textBox2.Text);
            double alp = 0.002;//Convert.ToDouble(textBox8.Text);
            //постійний крок
            if (comboBox1.SelectedIndex == 0)
            {
                alp = 0.00001;
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
                double eps_alp = 0.000001;
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
                MessageBox.Show("alp = "+alp.ToString()
                    +"\n"
                    + "abs(old - new) = " + Math.Abs(f_val_old - f_val_new).ToString()
                    + "|grad| = " + f_grad.ToString());
                //f(x1, x2);//f(x1, x2);
                textBox3.Text = y_min.ToString();
                textBox5.Text = k.ToString();
                textBox4.Text = x1.ToString();
                textBox6.Text = x2.ToString();
            }//найшвидший спуск
            else if (comboBox1.SelectedIndex == 2)
            {

            }
        }
    }
}
