using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace navilIME_control
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            if (IsUserAdministrator() == false)
            {
                MessageBox.Show("Please run this program as Administrator");
                Close();
            }

            Load_keyboard_combo();
        }

        private bool IsUserAdministrator()
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

        private void button_Cancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void button_Ok_Click(object sender, EventArgs e)
        {
            Store_keyboard_layout();
        }

        private string keyboardFileName = Environment.ExpandEnvironmentVariables("%ProgramW6432%") + "\\NavilIME\\Keyboard.dat";
        private string keyboardFileName_x86 = Environment.ExpandEnvironmentVariables("%ProgramFiles(x86)%") + "\\NavilIME\\Keyboard.dat";

        /* libhangul 라이브러리에서 바로 읽어 오는 것이 가장 좋으나 귀찮아서 그냥 하드 코딩.
         * 어차피 자주 바뀌는 데이터도 아님.
         * 윈폼 디자인에 comboBox_keyboards 컴포넌트의 아이템 순서와 키보드 레이아웃 아이디 순서가 일치해야함.
         * 수정하기 전에 윈폼 디자인 설정을 먼저 확인할 것.
         */
        private string[] layoutName = { "318Na", "39", "3f", "2", "2y", "32", "3s", "3y", "ro" };
        private int initialKeyboardComboIndex = 0;

        private void Load_keyboard_combo()
        {
            Debug.WriteLine("Keyboard file : " + keyboardFileName);
            Debug.WriteLine("Keyboard file (x86) : " + keyboardFileName_x86);

            string currentKeyboardLayout = System.IO.File.ReadAllText(keyboardFileName);
            currentKeyboardLayout = currentKeyboardLayout.TrimEnd();
            Debug.WriteLine("Current Keyboard layout ID: " + currentKeyboardLayout);

            bool isMatch = false;
            for(int i = 0; i < layoutName.Length; i++)
            {
                Debug.WriteLine("[" + layoutName[i] + "] == [" + currentKeyboardLayout + "]");
                if (layoutName[i] == currentKeyboardLayout)
                {
                    Debug.WriteLine("Match");
                    
                    comboBox_keyboards.SelectedIndex = i;
                    isMatch = true;
                    break;
                }
            }

            if (isMatch == false)
            {
                comboBox_keyboards.SelectedIndex = 0;
            }

            initialKeyboardComboIndex = comboBox_keyboards.SelectedIndex;
        }

        private void Store_keyboard_layout()
        {
            int selectedIndex = comboBox_keyboards.SelectedIndex;

            if (initialKeyboardComboIndex == selectedIndex)
            {
                Debug.WriteLine("No changes");
                return;
            }

            Debug.WriteLine("Selected Keyboard Layout : " + layoutName[selectedIndex]);

            System.IO.File.WriteAllText(keyboardFileName, layoutName[selectedIndex]);
            System.IO.File.WriteAllText(keyboardFileName_x86, layoutName[selectedIndex]);

            initialKeyboardComboIndex = selectedIndex;

            MessageBox.Show("자판 설정을 " + comboBox_keyboards.Items[selectedIndex] + "로 변경했습니다.");
        }
    }
}
