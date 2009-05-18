using System;
using System.Windows.Forms;

namespace BugEngine
{

    public class Editor
    {
        public class Nested
        {
            public Nested()
            {
				System.Windows.Forms.MessageBox.Show("Hello nested :)");
            }

            ~Nested()
            {
				System.Windows.Forms.MessageBox.Show("Goodbye nested :(");
            }
        }

        public Editor()
        {
            System.Windows.Forms.MessageBox.Show("Hello world :)");
        }

        ~Editor()
        {
            System.Windows.Forms.MessageBox.Show("Goodbye world!! :(");
        }

        public void test()
        {
            System.Windows.Forms.MessageBox.Show("doing some test");
        }
        public static void Main()
        {
            Editor editor = new Editor();
            editor.test();
        }
    }

}
