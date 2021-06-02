using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Mappert
{
    class Wolkje
    {
        private List<Point> _points;
        private Point _center;
        private int _radius;

        public Wolkje(Point center, int radius)
        {
            _points = new List<Point>();
            _center = center;
            _radius = radius;
        }

        public Point Get(int index)
        {
            if (index < 0) throw new ArgumentException("Wolkjes has 11 pixels, Index can not be smaller then 0");
            if (index >= 11) throw new ArgumentException("Wolkjes has 11 pixels, Index ca not be bigger then 10");
            return _points[index];
        }

        public Point Center
        {
            get { return _center; }
        }

        public int Radius
        {
            get { return _radius; }
        }

        public int Left
        {
            get
            {
                return _center.X - _radius;
            }
        }

        public int Right
        {
            get
            {
                return _center.X + _radius;
            }
        }

        public int Top
        {
            get
            {
                return _center.Y - Radius;
            }
        }

        public int Bottom
        {
            get
            {
                return _center.Y + Radius;
            }
        }
    }
}
