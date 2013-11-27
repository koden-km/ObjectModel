// (C) 2009 Christian.Schladetsch@Gmail.com. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using OmNet;

namespace Console2
{
    public partial class ConsoleControl : UserControl
    {
        Runtime runtime;

        public ConsoleControl()
        {
            InitializeComponent();
            runtime = new OmNet.Runtime();
        }

        public string GetWorkArea()
        {
            return richTextBox1.Text;
        }

        public void SetWorkArea(string text)
        {
            richTextBox1.Text = text;
        }

        void Process(string text)
        {
            try
            {
                Continuation continuation = runtime.compiler.CompileText(text);
                continuation.SetScope(runtime.root);
                runtime.executor.PushContext(continuation);
                runtime.executor.Continue();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Exception", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }

            runtime.compiler.Clear();

            StringBuilder result = new StringBuilder("\n");
            List<OmNet.Object> stack = runtime.executor.GetDataStack();

            int N = 0;
            foreach (OmNet.Object obj in stack)
            {
                result.Append(string.Format("[{0}]: ", N++));
                result.Append(obj.ToString());
                result.Append("\n");
            }

            richTextBox2.Text = result.ToString();
        }

        private void richTextBox1_KeyDown(object sender, KeyEventArgs e)
        {
            bool is_enter = e.Control && e.KeyCode == Keys.Enter;
            if (!is_enter)
                return;

            int original_location = richTextBox1.SelectionStart;
            int line_number = richTextBox1.GetLineFromCharIndex(richTextBox1.SelectionStart);
            int start = richTextBox1.GetFirstCharIndexOfCurrentLine();
            int end = richTextBox1.GetFirstCharIndexFromLine(line_number + 1);
            string text = "";
            if (end == -1)
                text = richTextBox1.Text;       // single line
            else
                text = richTextBox1.Text.Substring(start, end - start);
            richTextBox1.Select(0, 0);
            richTextBox1.SuspendLayout();
            Process(text);
            richTextBox1.SelectionStart = original_location;
            richTextBox1.ResumeLayout();
            e.Handled = true;
        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "";
            runtime.executor.Clear();
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            runtime.executor.Clear();

        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = "";

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_KeyDown(object sender, KeyEventArgs e)
        {
            bool is_enter = e.KeyCode == Keys.Enter;
            if (!is_enter)
                return;
            e.Handled = true;
            string text = comboBox1.Text;
            comboBox1.Text = "";
            Process(text);
            richTextBox1.SelectionStart = richTextBox1.TextLength;
            richTextBox1.ScrollToCaret();
           
        }

        private void richTextBox1_KeyDown_1(object sender, KeyEventArgs e)
        {
            richTextBox1_KeyDown(sender, e);
        }

    }
}

//EOF
