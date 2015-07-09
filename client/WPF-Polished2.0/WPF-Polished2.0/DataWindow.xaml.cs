using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Runtime.CompilerServices;
using System.Windows.Threading;
using System.Data.SqlClient;
using System.ComponentModel;
using System.Drawing;
using System.Net;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Media.Animation;
using TCPns;
using SharedDataNS;
using System.Data;

namespace DataWindowNS
{
    /// <summary>
    /// Interaction logic for DataWindow.xaml
    /// </summary>
    public partial class DataWindow : Window
    {
        enum RECEIVED_PACKET_TYPE { DEFAULT, EMPLOYEE_LIST };
        
        private RECEIVED_PACKET_TYPE currentPacket;
        private SocketObject client;
        private string loginName;
        private string accessLevel;
        private System.Timers.Timer reconnectTimer;
        DataGrid dataGridEmployeeList;

        private void reconnectTimerFunc(Object source, ElapsedEventArgs e)
        {
            if (ConnectToServer()) { reconnectTimer.Enabled = false; }
        }

        private bool ConnectToServer()
        {
            bool returnValue = true;

            //close current connection
            if ((client.socket != null) && client.socket.Connected)
            {
                client.socket.Close();
            }

            //set socket information
            client.socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            client.iep = new IPEndPoint(IPAddress.Parse("172.16.1.68"), 11002);

            //attempt to connect to server
            try
            {
                client.socket.Connect(client.iep);
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }

            //if attempt was successful
            if (client.socket.Connected)
            {
                Console.WriteLine("Successfully connected to server.");
                this.Dispatcher.Invoke((Action)(() =>
                {
                    connectedStatusImage.Fill = (Brush)Resources["CheckMarkImage"];
                    networkStatus.Content = "Connected to server.";
                }));
                receiveAsync();
            }
            else
            {
                Console.WriteLine("Unable to connect to server.");
                returnValue = false;
            }

            //return
            return returnValue;
        }

        private DataTable parseEmployeeList(string packet)
        {
            char[] delimiters = { ',', '(', ')' };
            DataTable dt = new DataTable();
            string[] strings = packet.Split(delimiters);
            string[] stringsWithoutDeliminators;
            int stringsCounter = 0;
            int deletedStrings = 0;
            int rowLen = 0;
            int colLen = 0;
            int colCounter = 0;
            int stringsWithoutDeliminatorsLen = 0;
            string[] tempColEntries;

            //delete unecessary text
            packet = packet.Remove(packet.IndexOf("~"));
            for (int i = 0; i < strings.Length; i++)
            {
                if (!(strings[i] == "(") && !(strings[i] == ",") && !(strings[i] == ")") && !(strings[i] == "") && !(strings[i] == "~") && (i != 1) && (i != 2))
                {
                    deletedStrings++;
                }
            }

            //build string without deliminators
            rowLen = Convert.ToInt32(strings[1]);
            if (rowLen == 0) throw new Exception("Empty table."); 
            colLen = Convert.ToInt32(strings[2]);
            dt.Columns.Add("Username", typeof(string));
	        dt.Columns.Add("AccessLevel", typeof(string));
            stringsWithoutDeliminatorsLen = strings.Length - deletedStrings;
            stringsWithoutDeliminators = new string[stringsWithoutDeliminatorsLen];
            for (int i = 0; i < strings.Length; i++)
            {
                if (!(strings[i] == "(") && !(strings[i] == ",") && !(strings[i] == ")") && !(strings[i] == "") && !(strings[i] == "~") && (i != 1) && (i != 2))
                {
                    stringsWithoutDeliminators[stringsCounter] = strings[i];
                    stringsCounter++;
                }
            }

            //parse strings to create data table for grid
            tempColEntries = new string[colLen];
            for (int i = 0; i < (stringsWithoutDeliminatorsLen-3); i++)
            {
                if(colCounter<colLen){
                    tempColEntries[colCounter] = stringsWithoutDeliminators[i];
                    colCounter++;
                }
                if(colCounter==colLen){
                    dt.Rows.Add(tempColEntries[0], tempColEntries[1]);
                    colCounter = 0;
                } 
            }

            return dt;
            
        }
        
        private void createNewDataGridFromPacket(string packet){
            dataGridEmployeeList = new DataGrid();
            dataGridEmployeeList.Name = "EmployeeList";
            dataGridEmployeeList.RowStyle = this.FindResource("employeeListDataGrid") as Style;
            //dataGridEmployeeList.CellStyle = this.FindResource("employeeListDataGridCell") as Style;
            dataGridEmployeeList.RowHeaderStyle = this.FindResource("employeeListDataGridCellHeader") as Style;
            dataGridEmployeeList.ColumnHeaderStyle = this.FindResource("employeeListDataGridCellColumn") as Style;
            dataGridEmployeeList.HorizontalAlignment = HorizontalAlignment.Left;
            dataGridEmployeeList.FontFamily = new FontFamily("Times New Roman");
            dataGridEmployeeList.FontSize = 15;
            dataGridEmployeeList.VerticalAlignment = VerticalAlignment.Top;
            dataGridEmployeeList.Margin = new Thickness(100, 100, 0, 0);
            dataGridEmployeeList.CanUserAddRows = false;
            dataGridEmployeeList.CanUserDeleteRows = false;
            dataGridEmployeeList.IsReadOnly = true;
            dataGridEmployeeList.BorderThickness = new Thickness(0.0);
            dataGridEmployeeList.Foreground = new SolidColorBrush(Colors.LightGray);
            dataGridEmployeeList.GridLinesVisibility = DataGridGridLinesVisibility.None;
            dataGridEmployeeList.Height = 520;
            dataGridEmployeeList.Width = 1080;
            dataGridEmployeeList.Visibility = Visibility.Visible;
            try
            {
                dataGridEmployeeList.ItemsSource = parseEmployeeList(packet).DefaultView;
            }catch(Exception e){
                Console.WriteLine(e.ToString());
                dataGridEmployeeList.ItemsSource = null;
            }
            GUI.Children.Add(dataGridEmployeeList);
        }

        private async void receiveAsync()
        {
            bool reset = false;
            while (!reset)
            {
                try
                {
                    await TCP.ReceiveAllPacketsSync(client);
                    Console.WriteLine("Received complete packet: {0}", client.packet);

                    if (currentPacket == RECEIVED_PACKET_TYPE.EMPLOYEE_LIST)
                    {
                        createNewDataGridFromPacket(client.packet);
                        hideMenuGUI();
                        showEmployeeListGUI();
                        currentPacket = RECEIVED_PACKET_TYPE.DEFAULT;
                    }
                    else
                    {
                        //do nothing
                    }
                    
                }
                catch (SocketException ex) // connection disconnected
                {
                    Console.WriteLine(ex.ToString());

                    this.Dispatcher.Invoke((Action)(() =>
                    {
                        connectedStatusImage.Fill = (Brush)Resources["XMarkImage"];
                        networkStatus.Content = "Attempting to reconnect...";
                    }));
                    reconnectTimer = new System.Timers.Timer();
                    reconnectTimer.Elapsed += reconnectTimerFunc;
                    reconnectTimer.Enabled = true;
                    reconnectTimer.AutoReset = true;

                    reset = true;
                }
            }
        }

        public DataWindow(SharedData sharedData)
        {
            InitializeComponent();
            this.DataContext = this;
            
            // get data from shared data
            loginName = sharedData._name;
            accessLevel = sharedData._accesslevel.ToString();
            client = sharedData._clientObject;

            // set GUI info
            AccessLevel.Content = "Access level: " + accessLevel;
            UI_Username.Content = "Logged in as: " + loginName;

            // set client variables
            currentPacket = RECEIVED_PACKET_TYPE.DEFAULT;

            // show GUI
            showMenuGUI();

            // receive async
            receiveAsync();
        }

        //////////////////
        /* Socket calls */
        //////////////////

        

        ///////////////////
        /* GUI Callbacks */
        ///////////////////

        private void OrdersButton_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {

        }

        private void CustomersButton_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {

        }

        private void PromotionsButton_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {

        }

        private void InventoryButton_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {

        }

        private void EmployeesButton_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            if(client.socket.Connected){
                string msg = "EmployeeList: Login:";
                msg += loginName;
                TCP.SendTask(client, Encoding.ASCII.GetBytes(msg));
                currentPacket = RECEIVED_PACKET_TYPE.EMPLOYEE_LIST;
            }else{
                Console.WriteLine("EmployeeList message NOT sent.");
            }
        }

        private void ExitButton_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void showMenuGUI()
        {
            OrdersButton.Visibility = Visibility.Visible;
            CustomersButton.Visibility = Visibility.Visible;
            PromotionsButton.Visibility = Visibility.Visible;
            InventoryButton.Visibility = Visibility.Visible;
            EmployeesButton.Visibility = Visibility.Visible;
            ExitButton.Visibility = Visibility.Visible;
        }

        private void hideMenuGUI()
        {
            OrdersButton.Visibility = Visibility.Hidden;
            CustomersButton.Visibility = Visibility.Hidden;
            PromotionsButton.Visibility = Visibility.Hidden;
            InventoryButton.Visibility = Visibility.Hidden;
            EmployeesButton.Visibility = Visibility.Hidden;
            ExitButton.Visibility = Visibility.Hidden;
        }

        private void showEmployeeListGUI()
        {
            dataGridEmployeeList.Visibility = Visibility.Visible;
        }

        private void hideEmployeeListGUI()
        {
            dataGridEmployeeList.Visibility = Visibility.Hidden;
        }

        private void ExitRectangle_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            ExitRectangle.Fill = this.FindResource("ExitImage") as Brush;
            Application.Current.Shutdown();
        }

        private void ExitRectangle_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            ExitRectangle.Fill = this.FindResource("ExitImageTransition") as Brush;
        }

        private void MinimizeRectangle_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            MinimizeRectangle.Fill = this.FindResource("MinimizeImage") as Brush;
            this.WindowState = WindowState.Minimized;
        }

        private void MinimizeRectangle_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            MinimizeRectangle.Fill = this.FindResource("MinimizeImageTransition") as Brush;
        }

        private void BackRectangle_PreviewMouseUp(object sender, MouseButtonEventArgs e)
        {
            BackRectangle.Fill = this.FindResource("BackImage") as Brush;
            hideEmployeeListGUI();
            showMenuGUI();
        }

        private void BackRectangle_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            BackRectangle.Fill = this.FindResource("BackImageTransition") as Brush;
        }

    }

}
