using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Mappert
{
    public partial class Form1 : Form
    {
        private readonly Pen _penCircles = new Pen(Color.IndianRed, 1);
        private List<Wolkje> _wolkjes;
        private readonly Random _random;
        public Form1()
        {
            _random = new Random();
            InitializeWolkjes();
            InitializeComponent();

            this.pictureBox1.Invalidate();
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            var graphics = e.Graphics;
            graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            for (var i = 0; i < _wolkjes.Count; i++)
            {
                var wolkje = _wolkjes[i];
                var rect = new Rectangle(wolkje.Left, wolkje.Top, wolkje.Radius * 2, wolkje.Radius * 2);
                graphics.DrawEllipse(_penCircles, rect);
            }
        }

        private void InitializeWolkjes()
        {
            _wolkjes = new List<Wolkje>();
            for (int i = 0; i < 40; i++)
            {
                var center = new Point(_random.Next(this.Width), _random.Next(this.Height));
                var wolkje = new Wolkje(center,3);
                _wolkjes.Add(wolkje);
            }
        }
    }
}
