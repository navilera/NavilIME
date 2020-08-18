namespace navilIME_control
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage_Keyboard = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.panel5 = new System.Windows.Forms.Panel();
            this.comboBox_keyboards = new System.Windows.Forms.ComboBox();
            this.panel4 = new System.Windows.Forms.Panel();
            this.panel3 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.button_Ok = new System.Windows.Forms.Button();
            this.button_Cancel = new System.Windows.Forms.Button();
            this.tabControl1.SuspendLayout();
            this.tabPage_Keyboard.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.panel5.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage_Keyboard);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(732, 546);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage_Keyboard
            // 
            this.tabPage_Keyboard.Controls.Add(this.groupBox1);
            this.tabPage_Keyboard.Controls.Add(this.panel1);
            this.tabPage_Keyboard.Location = new System.Drawing.Point(8, 39);
            this.tabPage_Keyboard.Name = "tabPage_Keyboard";
            this.tabPage_Keyboard.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_Keyboard.Size = new System.Drawing.Size(716, 499);
            this.tabPage_Keyboard.TabIndex = 0;
            this.tabPage_Keyboard.Text = "Keyboards";
            this.tabPage_Keyboard.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.panel5);
            this.groupBox1.Controls.Add(this.panel4);
            this.groupBox1.Controls.Add(this.panel3);
            this.groupBox1.Controls.Add(this.panel2);
            this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox1.Location = new System.Drawing.Point(3, 3);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(710, 405);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Keyboards";
            // 
            // panel5
            // 
            this.panel5.BackColor = System.Drawing.Color.Transparent;
            this.panel5.Controls.Add(this.comboBox_keyboards);
            this.panel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel5.Location = new System.Drawing.Point(102, 127);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(503, 275);
            this.panel5.TabIndex = 3;
            // 
            // comboBox_keyboards
            // 
            this.comboBox_keyboards.Dock = System.Windows.Forms.DockStyle.Fill;
            this.comboBox_keyboards.FormattingEnabled = true;
            this.comboBox_keyboards.Items.AddRange(new object[] {
            "세벌식 3-18Na",
            "세벌식 3-90",
            "세벌식 3-91 (최종)",
            "두벌식",
            "두벌식 옛한글",
            "세벌식 두벌 레이아웃",
            "세벌식 no shift",
            "세벌식 옛한글",
            "로마자",
            "안마태"});
            this.comboBox_keyboards.Location = new System.Drawing.Point(0, 0);
            this.comboBox_keyboards.Name = "comboBox_keyboards";
            this.comboBox_keyboards.Size = new System.Drawing.Size(503, 33);
            this.comboBox_keyboards.TabIndex = 0;
            // 
            // panel4
            // 
            this.panel4.BackColor = System.Drawing.Color.Transparent;
            this.panel4.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel4.Location = new System.Drawing.Point(102, 27);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(503, 100);
            this.panel4.TabIndex = 2;
            // 
            // panel3
            // 
            this.panel3.BackColor = System.Drawing.Color.Transparent;
            this.panel3.Dock = System.Windows.Forms.DockStyle.Right;
            this.panel3.Location = new System.Drawing.Point(605, 27);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(102, 375);
            this.panel3.TabIndex = 1;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.Transparent;
            this.panel2.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel2.Location = new System.Drawing.Point(3, 27);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(99, 375);
            this.panel2.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Transparent;
            this.panel1.Controls.Add(this.button_Ok);
            this.panel1.Controls.Add(this.button_Cancel);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(3, 408);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(710, 88);
            this.panel1.TabIndex = 0;
            // 
            // button_Ok
            // 
            this.button_Ok.Location = new System.Drawing.Point(219, 13);
            this.button_Ok.Name = "button_Ok";
            this.button_Ok.Size = new System.Drawing.Size(165, 62);
            this.button_Ok.TabIndex = 1;
            this.button_Ok.Text = "OK";
            this.button_Ok.UseVisualStyleBackColor = true;
            this.button_Ok.Click += new System.EventHandler(this.button_Ok_Click);
            // 
            // button_Cancel
            // 
            this.button_Cancel.Location = new System.Drawing.Point(31, 13);
            this.button_Cancel.Name = "button_Cancel";
            this.button_Cancel.Size = new System.Drawing.Size(165, 62);
            this.button_Cancel.TabIndex = 0;
            this.button_Cancel.Text = "Cancel";
            this.button_Cancel.UseVisualStyleBackColor = true;
            this.button_Cancel.Click += new System.EventHandler(this.button_Cancel_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(12F, 25F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(732, 546);
            this.Controls.Add(this.tabControl1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "NavilIME Control Panel";
            this.tabControl1.ResumeLayout(false);
            this.tabPage_Keyboard.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.panel5.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage_Keyboard;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button button_Ok;
        private System.Windows.Forms.Button button_Cancel;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.ComboBox comboBox_keyboards;
    }
}

