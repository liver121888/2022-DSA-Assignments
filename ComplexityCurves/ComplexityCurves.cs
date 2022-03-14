using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace ComplexityCurves
{
    public partial class ComplexityCurves : Form
    {
        Series N, cNpN0, NLog2N, cNLog2NpN0, TwoPowN, cTowPowNpN0, Nsquare, cNsquarepN0;

        private void nudConstant_ValueChanged( object sender, EventArgs e )
        {

        }

        bool[ ] stops = new bool[ 8 ];
        public ComplexityCurves( )
        {
            InitializeComponent( );
        }

        private void btnUpdate_Click( object sender, EventArgs e )
        {
            if( N == null )
            {
                N = new Series( "n" );
                cNpN0 = new Series( "c*n+n0" );
                NLog2N = new Series( "nLogn" );
                cNLog2NpN0 = new Series( "c*nLogn+n0" );
                Nsquare = new Series( "n^2" );
                cNsquarepN0 = new Series( "c*n^2+n0" );
                TwoPowN = new Series( "2^n" );
                cTowPowNpN0 = new Series( "c*2^n+n0" );
                cht.Series.Add( N );
                cht.Series.Add( cNpN0 );
                cht.Series.Add( NLog2N );
                cht.Series.Add( cNLog2NpN0 );
                cht.Series.Add( Nsquare );
                cht.Series.Add( cNsquarepN0 );
                cht.Series.Add( TwoPowN );
                cht.Series.Add( cTowPowNpN0 );
                foreach( Series s in cht.Series )
                {
                    s.ChartType = SeriesChartType.Line;
                    s.BorderWidth = 2;
                }
                N.Color = cNpN0.Color = Color.Green;
                cNpN0.BorderDashStyle = ChartDashStyle.Dot;

                NLog2N.Color = cNLog2NpN0.Color = Color.Blue;
                cNLog2NpN0.BorderDashStyle = ChartDashStyle.Dot;

                Nsquare.Color = cNsquarepN0.Color = Color.Brown;
                cNsquarepN0.BorderDashStyle = ChartDashStyle.Dot;

                TwoPowN.Color = cTowPowNpN0.Color = Color.Red;
                cTowPowNpN0.BorderDashStyle = ChartDashStyle.Dot;
            }
            for( int i = 0 ; i < stops.Length ; i++ )stops[i] = false;

            foreach( Series s in cht.Series )  s.Points.Clear( );
            double c = (double) nudMultiplier.Value;
            double n0 = (double) nudConstant.Value;

            double max = (double) nudUpper.Value;
            cht.ChartAreas[ 0 ].AxisX.Maximum = max;
            cht.ChartAreas[ 0 ].AxisX.Minimum = 0;
            double ymax = max * 100;
            cht.ChartAreas[ 0 ].AxisY.Maximum = ymax;
            double delta = max / 200;
            double y;

 
            for( double x = 1 ; x < max ; x+= delta )
            {
                try
                {
                    if( !stops[ 0 ] )
                    {
                        y = x;
                        if( y < ymax ) N.Points.AddXY( x, y );
                        else stops[ 0 ] = true;
                    }

                    if( !stops[ 1 ] )
                    {
                        y = x;
                        y = c * y + n0;
                        if( y < ymax ) cNpN0.Points.AddXY( x, y );
                        else stops[ 1 ] = true;
                    }

                    if( !stops[ 2 ] )
                    {
                        y = x * Math.Log( x, 2 );
                        if( y < ymax ) NLog2N.Points.AddXY( x, y );
                        else stops[ 2 ] = true;
                    }


                    if( !stops[ 3 ] )
                    {
                        y = x * Math.Log( x, 2 );
                        y = c * y + n0;
                        if( y < ymax ) cNLog2NpN0.Points.AddXY( x, y );
                        else stops[ 3 ] = true;
                    }

                    if( !stops[ 6 ] )
                    {
                        y = Math.Pow( 2, x );
                        if( y < ymax ) TwoPowN.Points.AddXY( x, y );
                        else stops[ 6 ] = true;
                    }

                    if( !stops[ 7 ] )
                    {
                        y = Math.Pow( 2, x );
                        y = c * y + n0;
                        if( y < ymax ) cTowPowNpN0.Points.AddXY( x, y );
                        else stops[ 7 ] = true;
                    }

                    if( !stops[ 4 ] )
                    {
                        y = x * x;
                        if( y < ymax ) Nsquare.Points.AddXY( x, y );
                        else stops[ 4] = true;
                    }

                    if( !stops[ 5 ] )
                    {
                        y = x * x;
                        y = c * y + n0;
                        if( y < ymax ) cNsquarepN0.Points.AddXY( x, y );
                        else stops[ 5 ] = true;
                    }
                }
                catch
                {
                    break;
                }
            }

        }
    }
}
