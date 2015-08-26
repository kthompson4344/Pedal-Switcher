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
        PedalList pedalBoard;  //Top tray which contains available pedals on pedalboard
        PedalList pedalConfig; //Bottom tray which contains saved presets
        static System.Windows.Forms.Timer myTimer = new System.Windows.Forms.Timer();
        int[] currentConfig; //array for current preset
        int currentIndex = 0; //index of above array
        int[,] savedConfigs; //array for all presets
        int currentPreset;
        string _myStringValue;


        public Form1()
        {
            InitializeComponent();
            _myStringValue = "";
            pedalBoard = new PedalList(this, pedalBoardHolder);
            pedalConfig = new PedalList(this, pedalConfigHolder);

            pedalBoard.addPanel("none", true); //add built in buffer to begin with

            //configuring a timer for 1ms
            myTimer.Tick += new EventHandler(TimerEventProcessor);
            myTimer.Interval = 1;
            myTimer.Start();

            Application.DoEvents(); //start timer

            currentConfig = new int[14]; //array for current preset
            savedConfigs = new int[100,14]; //array for all presets
            currentPreset = 0;          
        }

        public void mutateString(string str)
        {
            _myStringValue = str;
            //Console.WriteLine(_myStringValue);
            
        }

        //This 1ms timer checks to see if a button to add a pedal from the pedalboard to a preset is clicked
        private void TimerEventProcessor(Object myObject, EventArgs myEventArgs)
        {
            //if (pedalBoard.getAddingPedal() == false)
            //{
            //    for (int i = 0; i <= pedalBoard.getNumPedals(); i++)
            //    {
            //        int clicked = Int32.Parse(pedalBoard.pedalInfos().ElementAt(i).ElementAt(3));
            //        int number = Int32.Parse(pedalBoard.pedalInfos().ElementAt(i).ElementAt(2));
            //        if (clicked == number)
            //        {
            //            bool alreadyUsed = false;   //used to see if a pedal is added as it cannot be used twice
            //            for (int j = 0; j < currentIndex; j++)
            //            {
            //                if (currentConfig[j] == number)
            //                {
            //                    alreadyUsed = true;
            //                }
            //            }

            //            //ignores clicks on pedals that are already added
            //            if (alreadyUsed == false)
            //            {
            //                if (number == 1) //buffer
            //                {
            //                    pedalConfig.addPanel(pedalBoard.pedalInfos().ElementAt(0).ElementAt(1),true,false);
            //                }
            //                else
            //                {
            //                    pedalConfig.addPanel(pedalBoard.pedalInfos().ElementAt(i).ElementAt(1),false,false);
            //                }
            //                currentConfig[currentIndex] = number;
            //                currentIndex++;
            //            }
            //        }
            //    }
            //}

        }

        private void AddPedal_Click(object sender, EventArgs e)
        {     
            pedalBoard.addPanel();   
        }

        //opens file dialog and loads pedalboard from that file
        private void Import_Click(object sender, EventArgs e)
        {
            pedalBoard.removePanels();
            OpenFileDialog openFileDialog1 = new OpenFileDialog();
            openFileDialog1.Filter = "TXT Files|*.txt";
            openFileDialog1.Title = "Select a Pedalboard";
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                // Read each line of the file into a string array. Each element 
                // of the array is one line of the file. 
                string[] lines = System.IO.File.ReadAllLines(openFileDialog1.FileName);
                
                foreach (string line in lines)
                {
                    if (line != "")
                    {
                        pedalBoard.addPanel(line);
                    }
                }
            }
        }

        //Opens a file dialog and saves pedalboard to that location
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

        //receives serial data of presets on the switcher - TODO
        private void Receive_Click(object sender, EventArgs e)
        {

        }

        //sends serial data of the presets to the switcher - TODO
        private void Send_Click(object sender, EventArgs e)
        {

        }

        private void Presets_ValueChanged(object sender, EventArgs e)
        {
            int numPedals = 0;
            pedalConfig.removePanels();
            for (int i = 0; i < 14; i++)
            {
                savedConfigs[currentPreset,i] = currentConfig[i];
                if (savedConfigs[(int)Presets.Value,i] > 0)
                {
                    numPedals++;
                }
            }
            currentPreset = (int)Presets.Value;
            //bool alreadyUsed = false;   //used to see if a pedal is added as it cannot be used twice
            
            if (numPedals > 0)
            {
                for (int i = 0; i < numPedals; i++)
                {
                    //Console.WriteLine(pedalConfig.pedalInfos(false).ElementAt(savedConfigs[currentPreset, i]-1).ElementAt(1));
                    pedalConfig.addPanel(pedalConfig.pedalInfos(false).ElementAt(savedConfigs[currentPreset, i]-1).ElementAt(1), false, false);
                }
            }
            

            //if (number == 1) //buffer
            //{
            //    pedalConfig.addPanel(pedalBoard.pedalInfos().ElementAt(0).ElementAt(1), true, false);
            //}
            //else
            //{
            //    pedalConfig.addPanel(pedalBoard.pedalInfos().ElementAt(i).ElementAt(1), false, false);
            //}
            //currentConfig[currentIndex] = number;
            //currentIndex++;
        }
    }

    ////http://geekswithblogs.net/bosuch/archive/2012/06/12/cndashusing-a-delegate-to-raise-an-event-from-one-class.aspx
    //public delegate void MyHandler1(object sender, MyEvent e);
    //public class MyEvent : EventArgs
    //{
    //    public string message;
    //}

    public class PedalList
    {
        Form1 _form1;
        int numPanels = 0;
        List<Pedal> pedalList;
        Panel panelHolder;
        bool addingPedal = false;
        public PedalList(Form1 someForm, Panel holder)
        {
            _form1 = someForm;
            pedalList = new List<Pedal>();
            panelHolder = holder;
            panelHolder.BorderStyle = BorderStyle.FixedSingle;
         
        }

        //adds a pedal
        public void addPanel(string path = "none", bool buffer = false, bool button = true)
        {
            
            //http://stackoverflow.com/questions/15385921/add-label-to-panel-programmatically
            addingPedal = true;
            numPanels = pedalList.Count;
            if (numPanels < 14)
            {
                string imagePath;      
                     
                Pedal pedal = new Pedal(_form1, numPanels, button);           
                pedal.Name = numPanels.ToString();
                // TODO: May not want this functionality for the pedalConfig, so you'll have to rearragne some stuff
                if (buffer == true)
                {
                    pedal.setLabel("buffer");
                    pedal.setImage(@"c:\Users\Kyle\Desktop\pedal.jpg");
                }
                if (buffer != true)
                {
                    if (path == "none")
                    {
                        OpenFileDialog openFileDialog1 = new OpenFileDialog();
                        openFileDialog1.Filter = "JPG Files|*.jpg";
                        openFileDialog1.Title = "Select a Pedal Image";
                        if (openFileDialog1.ShowDialog() == DialogResult.OK)
                        {
                            imagePath = openFileDialog1.FileName;
                            pedal.setImage(imagePath);
                        }
                    }
                    else
                    {
                        imagePath = path;
                        pedal.setImage(imagePath);       
                    }
                  
                }
                pedalList.Add(pedal);
            
                panelHolder.Controls.Add(pedal);

                panelHolder.Controls.SetChildIndex(pedal, numPanels);
                

            }
            addingPedal = false;
 
        }

        public void removePanels()
        {
            panelHolder.Controls.Clear();
            pedalList.Clear(); //TODO THIS NEEDS TO BE HERE, BUT NOT UNTIL INFINITE CLICKING IS FIXED (PROPERLY RAISE EVENT)
            numPanels = 0;
        }

        public int getNumPedals()
        {
            return numPanels;
        }

        //used to prevent crashing when a file dialog is open
        public bool getAddingPedal()
        {
            return addingPedal;
        }

        public List<List<string>> pedalInfos(bool includeButton = true)
        {
            List<List<string>> infos = new List<List<string>>();
            foreach (Pedal p in pedalList)
            {
                infos.Add(p.getInfo(includeButton));     
            }
            return infos;
        }


    }

    public class Pedal : Panel
    {

        int panelWidth = 70;
        int panelHeight = 120;
        TextBox textBox;
        Button addButton;
        string backgroundImagePath;
        static int numPedals = 0;
        static int clicked = 14;
        Form1 _form1;

        public Pedal(Form1 someForm, int numPedals, bool button = true)
        {
            _form1 = someForm;
            numPedals++;
            Console.WriteLine(numPedals);
            this.AllowDrop = true;
            this.Size = new Size(panelWidth, panelHeight);
            textBox = new TextBox();
            //this.Controls.Add(textBox);
            if (button == true)
            {
                addButton = new Button();
                addButton.Text = "Add";
                addButton.Name = numPedals.ToString();
                addButton.Location = new Point(-2, 98);
                addButton.Click += new EventHandler(addButton_Click);
                this.Controls.Add(addButton);
            }
            this.Visible = true;
            this.BringToFront();
            this.Show();
            this.BringToFront();
        }

        //http://stackoverflow.com/questions/12276641/count-instances-of-the-class

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

        public List<string> getInfo(bool includeButton = true)
        {
            // Put more stuff here...
            List<string> info = new List<string>();
            //info.Add(textBox.Text); // 0
            info.Add(""); // 0
            info.Add(backgroundImagePath); // 1
            if (includeButton == false)
            {
                info.Add("");
                info.Add("");
            }
            else
            {
                info.Add(addButton.Name); // 2

                if (addButton.Name == clicked.ToString())
                {
                    info.Add(clicked.ToString()); // 3

                }
                else
                {
                    //14 signifies that no pedal is clicked (TODO - this may need to be 0 or 15)
                    info.Add("14");

                }
            }
            return info;
        }

        //click handler for button on each pedal in pedalboard
        public void addButton_Click(object sender, EventArgs e)
        {
            clicked = Int32.Parse(addButton.Name);
            _form1.mutateString(addButton.Name);
        }

        public int getButtonClick()
        {
            return clicked;
        }

        

    }

}