using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TreeHeapTest
{
    public partial class Form1 : Form
    {


        class Node
        {
            public int priority;
            public int id;
            public int time;
            public Node parent;
            public Node left;
            public Node right;
            public override string ToString()
            {
                return $"p={((parent == null) ? -1 : parent.id )}\n({id},{priority})\n l={((left == null) ? -1 : left.id)} <-> r={((right == null) ? -1 : right.id)}";
            }

            public void Display(StringBuilder sb)
            {
                sb.AppendLine(ToString());
                if (left != null) left.Display( sb );
                if (right != null) right.Display(sb );
            }
        }


        int N, tenN;
        Node root;
        Node temp;

        public Form1()
        {
            InitializeComponent();
        }

        Node[] initialArray;
        void constructInitialTreap(char lr, Node  parent, int left, int right)
        {
            Node temp;
            int idx = (left + right) / 2;
            temp = initialArray[idx];
            temp.priority = parent.priority - 10;
            temp.parent = parent;
            if (lr == 'l') parent.left = temp;
            else parent.right = temp;
            if (right - left == 1) return;
            if (left < idx) constructInitialTreap('l', temp, left, idx);
            if (idx + 1 < right) constructInitialTreap('r', temp, idx + 1, right);
            
        }

        Random rnd = new Random();
        private void button1_Click(object sender, EventArgs e)
        {
            N = (int)numericUpDown1.Value;
            initialArray = new Node[N];
            tenN = 10 * N;
            for (int i = 1; i <= N; i++)
            {
                temp = new Node();
                temp.id = i;
                temp.left = temp.right = temp.parent = null;
                temp.time = rnd.Next() % 10000;
                initialArray[i - 1] = temp;
            }

            int idx = N / 2;
            root = initialArray[idx];
            root.parent = null;
            root.priority = tenN;
            constructInitialTreap('l', root, 0, idx);
            constructInitialTreap('r', root, idx + 1, N);



            //
            trvTree.Nodes.Clear();
            temp = root;
            TreeNode tn = new TreeNode();
            tn.Tag = temp;
            tn.Text = tn.Tag.ToString();
            trvTree.Nodes.Add(tn);

            StringBuilder sb = new StringBuilder();
            root.Display(sb);
            richTextBox1.Clear();
           richTextBox1.Text = sb.ToString();


        }
    }
}
