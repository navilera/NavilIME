namespace navilIME_uninstaller
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
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.textBox_out = new System.Windows.Forms.TextBox();
            this.button_start = new System.Windows.Forms.Button();
            this.button_reload = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.PaleTurquoise;
            this.panel1.Controls.Add(this.button_reload);
            this.panel1.Controls.Add(this.button_start);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.panel1.Location = new System.Drawing.Point(0, 349);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(443, 52);
            this.panel1.TabIndex = 0;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.textBox_out);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel2.Location = new System.Drawing.Point(0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(443, 349);
            this.panel2.TabIndex = 1;
            // 
            // textBox_out
            // 
            this.textBox_out.BackColor = System.Drawing.Color.White;
            this.textBox_out.Dock = System.Windows.Forms.DockStyle.Fill;
            this.textBox_out.Location = new System.Drawing.Point(0, 0);
            this.textBox_out.Multiline = true;
            this.textBox_out.Name = "textBox_out";
            this.textBox_out.ReadOnly = true;
            this.textBox_out.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox_out.Size = new System.Drawing.Size(443, 349);
            this.textBox_out.TabIndex = 0;
            // 
            // button_start
            // 
            this.button_start.Location = new System.Drawing.Point(16, 9);
            this.button_start.Name = "button_start";
            this.button_start.Size = new System.Drawing.Size(75, 34);
            this.button_start.TabIndex = 0;
            this.button_start.Text = "Uninstall";
            this.button_start.UseVisualStyleBackColor = true;
            this.button_start.Click += new System.EventHandler(this.button_start_Click);
            // 
            // button_reload
            // 
            this.button_reload.Enabled = false;
            this.button_reload.Location = new System.Drawing.Point(106, 9);
            this.button_reload.Name = "button_reload";
            this.button_reload.Size = new System.Drawing.Size(75, 34);
            this.button_reload.TabIndex = 1;
            this.button_reload.Text = "Reload";
            this.button_reload.UseVisualStyleBackColor = true;
            this.button_reload.Click += new System.EventHandler(this.button_reload_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(443, 401);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "NavilIME uninstaller";
            this.panel1.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button button_reload;
        private System.Windows.Forms.Button button_start;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox textBox_out;
    }
}

