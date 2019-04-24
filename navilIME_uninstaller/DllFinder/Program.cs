using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DllFinder
{
    class Program
    {
        static void Main(string[] args)
        {
            Process[] Processes = Process.GetProcesses();

            foreach (Process Proc in Processes)
            {
                try
                {
                    try
                    {
                        foreach (ProcessModule ProcMod in Proc.Modules)
                        {
                            if (ProcMod.FileName.ToString().Contains("NavilIME.dll"))
                            {
                                Console.WriteLine(Proc.MainModule.ModuleName);
                                break;
                            }
                        }
                    }
                    catch (Exception e)
                    {
                        Debug.WriteLine("--------------------" + e.Message + "------" + Proc.ProcessName);
                    }
                }
                catch (Exception e)
                {
                    Debug.WriteLine("==================" + e.Message + "=====" + Proc.ProcessName);
                }
            }
        }
    }
}
