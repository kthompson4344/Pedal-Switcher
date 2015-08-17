using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;//For file dialog
using System.Windows.Forms;

namespace Pedal_Switcher
{

    public partial class Form1 : Form
    {

        PedalList pedalBoard;
        PedalList pedalConfig;
        
        public Form1()
        {
            InitializeComponent();

            pedalBoard = new PedalList(pedalBoardHolder);
            pedalConfig = new PedalList(pedalConfigHolder);
            pedalBoard.addPanel(); 
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void AddPedal_Click(object sender, EventArgs e)
        {     
            pedalBoard.addPanel();   
        }

        private void label1_Click(object sender, EventArgs e)
        {
            
        }

        private void Import_Click(object sender, EventArgs e)
        {

        }

        private void Save_Click(object sender, EventArgs e)
        {
            string[] paths;
            paths = new string[14]; //Image File Paths
            for (int i = 0; i <= pedalBoard.getNumPedals(); i++)
            {
                paths[i] = pedalBoard.pedalInfos().ElementAt(i).ElementAt(1);
            }
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.Filter = "Text file (*.txt)|*.txt|All files (*.*)|*.*";
            saveFileDialog1.Title = "Select a Save Location";
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {
                System.IO.File.WriteAllLines(saveFileDialog1.FileName, paths);
            }
            // List<string> pedal12Info = pedalInfos().ElementAt(11);
            // string pedal12Label = pedal12Info.ElementAt(0);
        }

        private void Receive_Click(object sender, EventArgs e)
        {

        }

        private void Send_Click(object sender, EventArgs e)
        {

        }
    }

    public class PedalList
    {
        int numPanels = 0;
        List<Pedal> pedalList;
        Panel panelHolder;
        public PedalList(Panel holder)
        {
            pedalList = new List<Pedal>();
            panelHolder = holder;
            panelHolder.BorderStyle = BorderStyle.FixedSingle;
        }

        public void addPanel()
        {
            //http://stackoverflow.com/questions/15385921/add-label-to-panel-programmatically

            numPanels = pedalList.Count;
            if (numPanels < 14)
            {
                string imagePath;
                Pedal pedal = new Pedal();

                // TODO: You may not want this functionality for the pedalConfig, so you'll have to rearragne some stuff
                if (numPanels == 0)
                {
                    pedal.setLabel("buffer");
                    pedal.setImage(@"c:\Users\Kyle\Desktop\pedal.jpg");
                }
                if (numPanels != 0)
                {
                    OpenFileDialog openFileDialog1 = new OpenFileDialog();
                    openFileDialog1.Filter = "JPG Files|*.jpg";
                    openFileDialog1.Title = "Select a Pedal Image";
                    if (openFileDialog1.ShowDialog() == DialogResult.OK)
                    {
                        imagePath = openFileDialog1.FileName;
                        pedal.setImage(imagePath);
                    }
                    //pedal.setImage(@"c:\Users\Kyle\Desktop\pedal.jpg"); // TODO: Open a dialog and select image
                }
                pedalList.Add(pedal);
                panelHolder.Controls.Add(pedal);
                if (numPanels > 0) {
                    panelHolder.Controls.SetChildIndex(pedal, numPanels - 1);
                }
            }

 
        }

        public int getNumPedals()
        {
            return numPanels;
        }

        // So somewhere else...
        // List<string> pedal12Info = pedalInfos().ElementAt(11);
        // string pedal12Label = pedal12Info.ElementAt(0);

        public List<List<string>> pedalInfos()
        {
            List<List<string>> infos = new List<List<string>>();
            foreach (Pedal p in pedalList)
            {
                infos.Add(p.getInfo());
            }
            return infos;
        }
    }

    public class Pedal : Panel
    {

        int panelWidth = 70;
        int panelHeight = 120;
        TextBox textBox;
        string backgroundImagePath;

        public Pedal()
        {
            this.AllowDrop = true;

            this.Size = new Size(panelWidth, panelHeight);
            textBox = new TextBox();
            this.Controls.Add(textBox);
            this.Visible = true;
            this.BringToFront();
            this.Show();
        }

        public void setLabel(string text)
        {
            textBox.Text = text;
        }

        public void setImage(string path)
        {
            backgroundImagePath = path;
            this.BackgroundImage = Image.FromFile(path);
            this.BackgroundImageLayout = ImageLayout.Zoom;
        }

        
        public List<string> getInfo()
        {
            // Put more stuff here...
            List<string> info = new List<string>();
            info.Add(textBox.Text); // 0
            info.Add(backgroundImagePath); // 1
            return info;
        }
    }

}