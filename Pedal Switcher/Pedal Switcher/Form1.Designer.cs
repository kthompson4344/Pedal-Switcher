namespace Pedal_Switcher
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.AddPedal = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.Import = new System.Windows.Forms.Button();
            this.Save = new System.Windows.Forms.Button();
            this.Presets = new System.Windows.Forms.NumericUpDown();
            this.Receive = new System.Windows.Forms.Button();
            this.Send = new System.Windows.Forms.Button();
            this.pedalBoardHolder = new System.Windows.Forms.FlowLayoutPanel();
            this.pedalConfigHolder = new System.Windows.Forms.FlowLayoutPanel();
            ((System.ComponentModel.ISupportInitialize)(this.Presets)).BeginInit();
            this.SuspendLayout();
            // 
            // AddPedal
            // 
            this.AddPedal.Location = new System.Drawing.Point(12, 128);
            this.AddPedal.Name = "AddPedal";
            this.AddPedal.Size = new System.Drawing.Size(85, 26);
            this.AddPedal.TabIndex = 1;
            this.AddPedal.Text = "Add Pedal";
            this.AddPedal.UseVisualStyleBackColor = true;
            this.AddPedal.Click += new System.EventHandler(this.AddPedal_Click);
            // 
            // Import
            // 
            this.Import.Location = new System.Drawing.Point(12, 59);
            this.Import.Name = "Import";
            this.Import.Size = new System.Drawing.Size(85, 29);
            this.Import.TabIndex = 3;
            this.Import.Text = "Import Pedals";
            this.Import.UseVisualStyleBackColor = true;
            this.Import.Click += new System.EventHandler(this.Import_Click);
            // 
            // Save
            // 
            this.Save.Location = new System.Drawing.Point(12, 94);
            this.Save.Name = "Save";
            this.Save.Size = new System.Drawing.Size(85, 28);
            this.Save.TabIndex = 4;
            this.Save.Text = "Save Pedals";
            this.Save.UseVisualStyleBackColor = true;
            this.Save.Click += new System.EventHandler(this.Save_Click);
            // 
            // Presets
            // 
            this.Presets.Location = new System.Drawing.Point(12, 336);
            this.Presets.Name = "Presets";
            this.Presets.Size = new System.Drawing.Size(85, 20);
            this.Presets.TabIndex = 5;
            // 
            // Receive
            // 
            this.Receive.Location = new System.Drawing.Point(12, 271);
            this.Receive.Name = "Receive";
            this.Receive.Size = new System.Drawing.Size(85, 26);
            this.Receive.TabIndex = 6;
            this.Receive.Text = "Receive";
            this.Receive.UseVisualStyleBackColor = true;
            this.Receive.Click += new System.EventHandler(this.Receive_Click);
            // 
            // Send
            // 
            this.Send.Location = new System.Drawing.Point(12, 303);
            this.Send.Name = "Send";
            this.Send.Size = new System.Drawing.Size(85, 27);
            this.Send.TabIndex = 7;
            this.Send.Text = "Send";
            this.Send.UseVisualStyleBackColor = true;
            this.Send.Click += new System.EventHandler(this.Send_Click);
            // 
            // pedalBoardHolder
            // 
            this.pedalBoardHolder.Location = new System.Drawing.Point(116, 42);
            this.pedalBoardHolder.Name = "pedalBoardHolder";
            this.pedalBoardHolder.Size = new System.Drawing.Size(1083, 128);
            this.pedalBoardHolder.TabIndex = 8;
            this.pedalBoardHolder.Click += new System.EventHandler(this.pedalBoardHolder_Click);
            // 
            // pedalConfigHolder
            // 
            this.pedalConfigHolder.Location = new System.Drawing.Point(116, 246);
            this.pedalConfigHolder.Name = "pedalConfigHolder";
            this.pedalConfigHolder.Size = new System.Drawing.Size(1083, 131);
            this.pedalConfigHolder.TabIndex = 9;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1247, 446);
            this.Controls.Add(this.pedalConfigHolder);
            this.Controls.Add(this.pedalBoardHolder);
            this.Controls.Add(this.Send);
            this.Controls.Add(this.Receive);
            this.Controls.Add(this.Presets);
            this.Controls.Add(this.Save);
            this.Controls.Add(this.Import);
            this.Controls.Add(this.AddPedal);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.Presets)).EndInit();
            this.ResumeLayout(false);
            
        }

        #endregion
        private System.Windows.Forms.Button AddPedal;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.Button Import;
        private System.Windows.Forms.Button Save;
        private System.Windows.Forms.NumericUpDown Presets;
        private System.Windows.Forms.Button Receive;
        private System.Windows.Forms.Button Send;
        private System.Windows.Forms.FlowLayoutPanel pedalBoardHolder;
        private System.Windows.Forms.FlowLayoutPanel pedalConfigHolder;
    }
}

