using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace SchoolScheduleICSConverter_GUI
{
    public partial class Form1 : Form
    {
        const int ERROR_FILE_NOT_FOUND = 2;
        const int ERROR_ACCESS_DENIED = 5;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            radioButton1.Select();
            checkBox1.Checked = true;
//          consoleAppPath.Text = Application.ExecutablePath;

            int CurrentYear = DateTime.Now.Year;
            int StartYearOfComboBox = 2012;
            int EndYearOfComboBox = 3000;
            for (int i = StartYearOfComboBox; i <= EndYearOfComboBox; ++i)
            {
                StartOfSchoolYear.Items.Add(i.ToString());
                if (i == CurrentYear)
                {
                    StartOfSchoolYear.SelectedIndex = i - StartYearOfComboBox - ((DateTime.Now.Month <= 7)?(1):(0));
                }
            }

            for (int i = 0; i <= 52; ++i)
            {
                SchoolWeekRangeStart.Items.Add(i.ToString());
                SchoolWeekRangeEnd.Items.Add(i.ToString());
            }
            SchoolWeekRangeStart.SelectedIndex = 0;
            SchoolWeekRangeEnd.SelectedIndex = 52;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult res = folderBrowserDialog1.ShowDialog();
            if (res == DialogResult.OK)
            {
                consoleAppPath.Text = folderBrowserDialog1.SelectedPath;
            }
        }

        private void folderBrowserDialog1_HelpRequest(object sender, EventArgs e)
        {
            
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            groupBox3.Enabled = false;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            groupBox3.Enabled = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (consoleAppPath.Text == string.Empty)
            {
                toolStripStatusLabel1.Text = "Select the console application folder";
                button1_Click(null, null);
                if (consoleAppPath.Text == string.Empty)
                {
                    return;
                }
            }

            string ExePath = consoleAppPath.Text + @"\ScheduleICSConverter_CLI.exe";
            if (radioButton1.Checked)
            {
                // Manual mode.
                Process process = new Process();
                process.StartInfo.FileName = ExePath;
                process.StartInfo.ErrorDialog = true;
                process.StartInfo.WorkingDirectory = consoleAppPath.Text;

                foreach (int StudyYearSelectionIndex in YearSelection.SelectedIndices)
                {
                    string StudyYear = YearSelection.Items[StudyYearSelectionIndex].ToString();
                    string classCode = StudyYear + "GA-";
                    foreach (int VariationSelectionIndex in VariationSelection.SelectedIndices)
                    {
                        string Variation = VariationSelection.Items[VariationSelectionIndex].ToString();
                        if (Variation == "IGD")
                        {
                            Variation = "%20IGD";
                        }
                        foreach (int ClassSelectionIndex in ClassSelection.SelectedIndices)
                        {
                            string ClassNumber = ClassSelection.Items[ClassSelectionIndex].ToString();
                            string finalClassCode = classCode + ClassNumber + Variation;

                            process.StartInfo.Arguments =   "1 " +
                                                            finalClassCode +
                                                            " " +
                                                            StartOfSchoolYear.Text.ToString() +
                                                            " " +
                                                            SchoolWeekRangeStart.Text.ToString() +
                                                            " " +
                                                            SchoolWeekRangeEnd.Text.ToString();

                            try
                            {
                                process.Start();
                            }
                            catch (Win32Exception exception)
                            {
                                switch (exception.NativeErrorCode)
                                {
                                    case ERROR_FILE_NOT_FOUND:
                                        {
                                            toolStripStatusLabel1.Text = "Could not find executable file in specified folder";
                                            break;
                                        }
                                    case ERROR_ACCESS_DENIED:
                                        {
                                            toolStripStatusLabel1.Text = "System denied access to executable file";
                                            break;
                                        }
                                    default:
                                        {
                                            toolStripStatusLabel1.Text = "I didn't bother writing a message for this error code (" + exception.NativeErrorCode.ToString() + ")";
                                            break;
                                        }
                                }
                                return;
                            }
                            toolStripStatusLabel1.Text = "In progress...";
                            process.WaitForExit();
                            toolStripStatusLabel1.Text = "Done (Exit code: " + process.ExitCode.ToString() + ")";
                        }
                    }
                }
            }
            else
            {
                // Auto mode.
                Process process = new Process();
                process.StartInfo.FileName = ExePath;
                process.StartInfo.ErrorDialog = true;
                process.StartInfo.WorkingDirectory = consoleAppPath.Text;
                process.StartInfo.Arguments = "2" +
                                                " 0 " +
                                                StartOfSchoolYear.Text.ToString() +
                                                " " +
                                                SchoolWeekRangeStart.Text.ToString() +
                                                " " +
                                                SchoolWeekRangeEnd.Text.ToString();
                try
                {
                    process.Start();
                }
                catch (Win32Exception exception)
                {
                    switch (exception.NativeErrorCode)
                    {
                        case ERROR_FILE_NOT_FOUND:
                            {
                                toolStripStatusLabel1.Text = "Could not find executable file in specified folder";
                                break;
                            }
                        case ERROR_ACCESS_DENIED:
                            {
                                toolStripStatusLabel1.Text = "System denied access to executable file";
                                break;
                            }
                        default:
                            {
                                toolStripStatusLabel1.Text = "I didn't bother writing a message for this error code (" + exception.NativeErrorCode.ToString() + ")";
                                break;
                            }
                    }
                    return;
                }
                toolStripStatusLabel1.Text = "In progress...";
                process.WaitForExit();
                toolStripStatusLabel1.Text = "Done (Exit code: " + process.ExitCode.ToString() + ")";
            }
            if (checkBox1.Checked)
            {
                try
                {
                    Process.Start(consoleAppPath.Text + @"\output");
                }
                catch (Win32Exception exception)
                {
                    switch (exception.NativeErrorCode)
                    {
                        case ERROR_FILE_NOT_FOUND:
                            {
                                toolStripStatusLabel1.Text = "Could not open output folder";
                                break;
                            }
                        case ERROR_ACCESS_DENIED:
                            {
                                toolStripStatusLabel1.Text = "System denied access to output folder";
                                break;
                            }
                        default:
                            {
                                toolStripStatusLabel1.Text = "Something went wrong while opening the output folder (" + exception.NativeErrorCode + ")";
                                break;
                            }
                    }
                }
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            consoleAppPath.Text = Application.StartupPath;
        }
    }
}
