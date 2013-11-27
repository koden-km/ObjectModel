// (C) 2009 Christian.Schladetsch@Gmail.com. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Controls
{
    public partial class ConsoleControl : UserControl
    {
        public ConsoleControl()
        {
            InitializeComponent();
        }

        private void richTextBox1_KeyDown(object sender, KeyEventArgs e)
        {
            bool is_enter = e.Control && e.KeyCode == Keys.Enter;
            if (!is_enter)
                return;
            string text = richTextBox1.SelectedText;
            richTextBox1.Select(0, 0);
            System.Console.WriteLine("exec: " + text);
        }
    }
}
