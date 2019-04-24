using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace navilIME_uninstaller
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            if (IsUserAdministrator() == false)
            {
                textBox_out.AppendText("관리자 권한으로 언인스톨러를 다시 시작해 주세요.." + Environment.NewLine);
                textBox_out.AppendText("Please re-run this uninstaller as Administrator.." + Environment.NewLine);
                button_start.Enabled = false;
            }
        }

        public bool IsUserAdministrator()
        {
            bool isAdmin;
            try
            {
                WindowsIdentity user = WindowsIdentity.GetCurrent();
                WindowsPrincipal principal = new WindowsPrincipal(user);
                isAdmin = principal.IsInRole(WindowsBuiltInRole.Administrator);
            }
            catch (UnauthorizedAccessException ex)
            {
                isAdmin = false;
            }
            catch (Exception ex)
            {
                isAdmin = false;
            }
            return isAdmin;
        }

        private void button_start_Click(object sender, EventArgs e)
        {
            textBox_out.Clear();
            UInt32 count = ShowNavilImeDllLoadedProcesses();

            if (count == 0)
            {
                button_reload.Enabled = false;
                button_start.Enabled = false;

                textBox_out.AppendText("나빌 입력기 제거를 시작합니다." + Environment.NewLine);
                textBox_out.AppendText("Uninstaller will be started." + Environment.NewLine);
                textBox_out.AppendText(Environment.NewLine);

                DoUninstall();

                textBox_out.AppendText(Environment.NewLine);
                textBox_out.AppendText("나빌 입력기 제거를 완료했습니다. 프로그램을 종료해 주세요." + Environment.NewLine);
                textBox_out.AppendText("Uninstallation done. Please close this program." + Environment.NewLine);
            }
        }

        private void button_reload_Click(object sender, EventArgs e)
        {
            textBox_out.Clear();
            UInt32 count = ShowNavilImeDllLoadedProcesses();

            if (count == 0)
            {
                textBox_out.AppendText(Environment.NewLine);
                textBox_out.AppendText("나빌 입력기 제거를 시작합니다. Uninstall을 눌러주세요." + Environment.NewLine);
                textBox_out.AppendText("Uninstaller will be started. Please click the below 'Uninstall' button." + Environment.NewLine);
                button_reload.Enabled = false;
                button_start.Enabled = true;
            }
        }

        private void DoUninstall()
        {
            string unregCmd = "regsvr32";

            textBox_out.AppendText("Unregister x32 NavilIME DLL" + Environment.NewLine);
            string x32UnregArg = "/u \"C:\\Program Files (x86)\\NavilIME\\NavilIME.dll\"";
            textBox_out.AppendText("  " + unregCmd + " " + x32UnregArg + Environment.NewLine);
            Process x32unregProc = System.Diagnostics.Process.Start(unregCmd, x32UnregArg);
            x32unregProc.WaitForExit();

            textBox_out.AppendText("Unregister x64 NavilIME DLL" + Environment.NewLine);
            string x64UnregArg = "/u \"C:\\Program Files\\NavilIME\\NavilIME.dll\"";
            textBox_out.AppendText("  " + unregCmd + " " + x64UnregArg + Environment.NewLine);
            Process x64unregProc = System.Diagnostics.Process.Start(unregCmd, x64UnregArg);
            x64unregProc.WaitForExit();

            textBox_out.AppendText(Environment.NewLine);

            textBox_out.AppendText("Remove x32 NavilIME DLL" + Environment.NewLine);
            string x32RemoveCmd = "rmdir /S /Q \"C:\\Program Files (x86)\\NavilIME\\\"";
            textBox_out.AppendText("  " + x32RemoveCmd + Environment.NewLine);
            System.Diagnostics.Process.Start("cmd.exe", "/C " + x32RemoveCmd);

            textBox_out.AppendText("Remove x64 NavilIME DLL" + Environment.NewLine);
            string x64RemoveCmd = "rmdir /S /Q \"C:\\Program Files\\NavilIME\\\"";
            textBox_out.AppendText("  " + x64RemoveCmd + Environment.NewLine);
            System.Diagnostics.Process.Start("cmd.exe", "/C " + x64RemoveCmd);
        }

        private UInt32 ShowNavilImeDllLoadedProcesses()
        {
            UInt32 count = 0;

            string[] x64procList = GetProcList("x64");
            string[] x32procList = GetProcList("x32");

            foreach (string x64proc in x64procList)
            {
                textBox_out.AppendText("64bit app: " + x64proc);
                textBox_out.AppendText(Environment.NewLine);
                count++;
            }

            foreach (string x32proc in x32procList)
            {
                textBox_out.AppendText("32bit app: " + x32proc);
                textBox_out.AppendText(Environment.NewLine);
                count++;
            }

            if (count > 0)
            {
                textBox_out.AppendText(Environment.NewLine + "위 프로그램을 종료하고 Reload를 눌러주세요.." + Environment.NewLine);
                textBox_out.AppendText(Environment.NewLine + "Please close the above programs and click 'Reload' button.." + Environment.NewLine);
                button_reload.Enabled = true;
                button_start.Enabled = false;
            }
            

            return count;
        }

        private string[] GetProcList(string platform)
        {
            string dllFinderName = "DllFinder_" + platform + ".exe";

            List<string> procList = new List<string>();

            Process dllFinder = new Process();

            dllFinder.StartInfo.UseShellExecute = false;
            dllFinder.StartInfo.FileName = dllFinderName;

            dllFinder.StartInfo.CreateNoWindow = true;
            dllFinder.StartInfo.RedirectStandardOutput = true;
            dllFinder.StartInfo.RedirectStandardInput = true;
            dllFinder.Start();

            StreamReader myStreamReader = dllFinder.StandardOutput;

            string procName = myStreamReader.ReadLine();
            while (procName != null)
            {
                procList.Add(procName);
                procName = myStreamReader.ReadLine();
            }

            return procList.ToArray();
        }
    }
}
