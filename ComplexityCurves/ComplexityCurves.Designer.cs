
namespace ComplexityCurves
{
    partial class ComplexityCurves
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose( bool disposing )
        {
            if( disposing && ( components != null ) )
            {
                components.Dispose( );
            }
            base.Dispose( disposing );
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent( )
        {
            System.Windows.Forms.DataVisualization.Charting.ChartArea chartArea1 = new System.Windows.Forms.DataVisualization.Charting.ChartArea();
            System.Windows.Forms.DataVisualization.Charting.Legend legend1 = new System.Windows.Forms.DataVisualization.Charting.Legend();
            this.nudUpper = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.nudMultiplier = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.nudConstant = new System.Windows.Forms.NumericUpDown();
            this.btnUpdate = new System.Windows.Forms.Button();
            this.cht = new System.Windows.Forms.DataVisualization.Charting.Chart();
            ((System.ComponentModel.ISupportInitialize)(this.nudUpper)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMultiplier)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudConstant)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cht)).BeginInit();
            this.SuspendLayout();
            // 
            // nudUpper
            // 
            this.nudUpper.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.nudUpper.Location = new System.Drawing.Point(114, 46);
            this.nudUpper.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.nudUpper.Maximum = new decimal(new int[] {
            100000000,
            0,
            0,
            0});
            this.nudUpper.Name = "nudUpper";
            this.nudUpper.Size = new System.Drawing.Size(45, 23);
            this.nudUpper.TabIndex = 0;
            this.nudUpper.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 48);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(96, 16);
            this.label1.TabIndex = 1;
            this.label1.Text = "N upper bound";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 91);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(74, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "Multiplier  c";
            // 
            // nudMultiplier
            // 
            this.nudMultiplier.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.nudMultiplier.Location = new System.Drawing.Point(114, 89);
            this.nudMultiplier.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.nudMultiplier.Maximum = new decimal(new int[] {
            100000000,
            0,
            0,
            0});
            this.nudMultiplier.Name = "nudMultiplier";
            this.nudMultiplier.Size = new System.Drawing.Size(45, 23);
            this.nudMultiplier.TabIndex = 2;
            this.nudMultiplier.Value = new decimal(new int[] {
            50,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 136);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(75, 16);
            this.label3.TabIndex = 5;
            this.label3.Text = "Constant n0";
            // 
            // nudConstant
            // 
            this.nudConstant.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.nudConstant.Location = new System.Drawing.Point(114, 134);
            this.nudConstant.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.nudConstant.Maximum = new decimal(new int[] {
            100000000,
            0,
            0,
            0});
            this.nudConstant.Name = "nudConstant";
            this.nudConstant.Size = new System.Drawing.Size(45, 23);
            this.nudConstant.TabIndex = 4;
            this.nudConstant.Value = new decimal(new int[] {
            500,
            0,
            0,
            0});
            this.nudConstant.ValueChanged += new System.EventHandler(this.nudConstant_ValueChanged);
            // 
            // btnUpdate
            // 
            this.btnUpdate.Location = new System.Drawing.Point(15, 181);
            this.btnUpdate.Name = "btnUpdate";
            this.btnUpdate.Size = new System.Drawing.Size(144, 65);
            this.btnUpdate.TabIndex = 6;
            this.btnUpdate.Text = "Update Complexity Lines";
            this.btnUpdate.UseVisualStyleBackColor = true;
            this.btnUpdate.Click += new System.EventHandler(this.btnUpdate_Click);
            // 
            // cht
            // 
            this.cht.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            chartArea1.AxisX.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.True;
            chartArea1.AxisX.MajorGrid.LineColor = System.Drawing.Color.Gainsboro;
            chartArea1.AxisX.Title = "n";
            chartArea1.AxisY.Enabled = System.Windows.Forms.DataVisualization.Charting.AxisEnabled.True;
            chartArea1.AxisY.MajorGrid.LineColor = System.Drawing.Color.Gainsboro;
            chartArea1.AxisY.Title = "Complexity Count";
            chartArea1.Name = "ChartArea1";
            this.cht.ChartAreas.Add(chartArea1);
            legend1.Name = "Legend1";
            this.cht.Legends.Add(legend1);
            this.cht.Location = new System.Drawing.Point(175, 12);
            this.cht.Name = "cht";
            this.cht.Size = new System.Drawing.Size(834, 627);
            this.cht.TabIndex = 7;
            this.cht.Text = "chart1";
            // 
            // ComplexityCurves
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1021, 651);
            this.Controls.Add(this.cht);
            this.Controls.Add(this.btnUpdate);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.nudConstant);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.nudMultiplier);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.nudUpper);
            this.Font = new System.Drawing.Font("Microsoft JhengHei", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(136)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "ComplexityCurves";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Complexity Curves";
            ((System.ComponentModel.ISupportInitialize)(this.nudUpper)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudMultiplier)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.nudConstant)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cht)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown nudUpper;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown nudMultiplier;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown nudConstant;
        private System.Windows.Forms.Button btnUpdate;
        private System.Windows.Forms.DataVisualization.Charting.Chart cht;
    }
}

