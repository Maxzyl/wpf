using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.Windows.Media.Animation;
using System.Windows.Threading;
using System.Threading;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.CompilerServices;
using TCPns;
using SharedDataNS;
using System.Net.Sockets;
using System.Net;

namespace LoginNS
{
    
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {

        public SocketObject client;

        public string Username { get; set; }
        public string Password { get; set; }

        public LoginWindow()
        {
            InitializeComponent();
            rect_window_fadein();

            UsernameTextbox.Text = "Username";
            PasswordTextbox.Text = "Password";
            Username = "";
            Password = "";

            client = new SocketObject(new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp), new IPEndPoint(IPAddress.Parse("172.16.1.68"), 11002));

            UIGlobal.LoginWindowUI = this;
            UIGlobal.sharedTCPObject = client;

            Connect();
        }

        private async void Connect()
        {
            bool result = await TCP.ConnectTask(client);
            if(result){
                Console.WriteLine("Successfully connect to server.");
            }else{
                MessageBox.Show("The server is unavailable. Please try again later.");
                Application.Current.Shutdown();
            }
        }

        private void rect_window_fadein()
        {
            DoubleAnimation fadingAnimation = new DoubleAnimation();
            fadingAnimation.From = 0;
            fadingAnimation.To = 1;
            fadingAnimation.Duration = new Duration(TimeSpan.FromSeconds(2));

            LoginRectangle.BeginAnimation(Rectangle.OpacityProperty, fadingAnimation);
            LoginButton.BeginAnimation(Button.OpacityProperty, fadingAnimation);
            ExitRectangle.BeginAnimation(Button.OpacityProperty, fadingAnimation);
            UsernameTextbox.BeginAnimation(TextBox.OpacityProperty, fadingAnimation);
            PasswordTextbox.BeginAnimation(TextBox.OpacityProperty, fadingAnimation);
        }

        public void rect_window_close()
        {
            this.Close();
        }
                         
        private void UsernameTextbox_MouseDown(object sender, MouseButtonEventArgs e)
        {
            UsernameTextbox.Text = "";
            Username = "";
        }

        private void PasswordTextbox_MouseDown(object sender, MouseButtonEventArgs e)
        {
            PasswordTextbox.Text = "";
            Password = "";
        }

        private void ExitRectangle_MouseUp(object sender, MouseButtonEventArgs e)
        {
            ExitRectangle.Fill = this.FindResource("ExitImage") as Brush;
            Application.Current.Shutdown();
        }

        private void ExitRectangle_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            ExitRectangle.Fill = this.FindResource("ExitImageTransition") as Brush;
        }

        private void MinimizeRectangle_MouseUp(object sender, MouseButtonEventArgs e)
        {
            MinimizeRectangle.Fill = this.FindResource("MinimizeImage") as Brush;
            this.WindowState = WindowState.Minimized;
        }

        private void MinimizeRectangle_PreviewMouseDown(object sender, MouseButtonEventArgs e)
        {
            MinimizeRectangle.Fill = this.FindResource("MinimizeImageTransition") as Brush;
        }

        private bool IsStringAllAsterisk(string str)
        {

            string tempAskteriskStr = "";

            for(int i=0; i<str.Length; i++){
                tempAskteriskStr += '*';
            }

            if (str.Equals(tempAskteriskStr))
            {
                return true;
            }
            else
            {
                return false;
            }

        }

        private void UsernameTextbox_TextChanged(object sender, TextChangedEventArgs e)
        {
            //set username to username's textbox
            Username = UsernameTextbox.Text;
        }

        private void PasswordTextbox_TextChanged(object sender, TextChangedEventArgs e)
        {

            //if textbox contains characters
            if ((PasswordTextbox.Text.Length > 0) && (!PasswordTextbox.Text.Equals("Password")) && (!IsStringAllAsterisk(PasswordTextbox.Text)))
            {

                //append last character in textbox to variable
                Password += PasswordTextbox.Text[PasswordTextbox.Text.Length - 1].ToString();
                //Password.Insert((PasswordTextbox.Text.Length - 1), PasswordTextbox.Text[PasswordTextbox.Text.Length - 1].ToString());

                //change all textbox' characters to asterisks
                int tempNumAsterisks = PasswordTextbox.Text.Length;
                PasswordTextbox.Text = "";
                for (int i = 0; i < tempNumAsterisks; i++)
                {
                    PasswordTextbox.Text += "*";
                }
                
                PasswordTextbox.Focus();
                PasswordTextbox.SelectionStart=9;
                PasswordTextbox.SelectionLength++;
            }

        }

        private void PasswordTextbox_PreviewKeyDown(object sender, KeyEventArgs e)
        {
            //if key is back
            if (e.Key == Key.Back)
            {
                //if there are characters in string, remove last character
                if(Password.Length>0){
                    Password = Password.Remove(Password.Length-1);
                }
            }
        }

    }

    public class SQLServerClass
    {
        //public event PropertyChangedEventHandler PropertyChanged;
        //private string Login = "";
        //private string Password = "";
        private AccessLevel accessLevel;
        public SharedData sharedData;
        
        public ICommand LoginCommand { get; set; }

        //public string _login
        //{
        //    get { return Login; }
        //    set
        //    {
        //        if (value != Login)
        //        {
        //            Login = value;
        //            OnPropertyChanged("_login");
        //        }
        //    }
        //}

        //public string _password
        //{
        //    get { return Password; }
        //    set
        //    {
        //        if (value != Password)
        //        {
        //            Password = value;
        //            OnPropertyChanged("_password");
        //        }
        //    }
        //}

        public SQLServerClass()
        {
            LoginCommand = new RelayCommand(LoginCommandExecute, LoginCommandCanExecute);
        }

        private void LoginCommandExecute(object arg)
        {
            ConnectSQLServer(this, new RoutedEventArgs());
        }

        private bool LoginCommandCanExecute(object arg)
        {
            return true;
        }

        private bool isAuthorizationSuccessful(string packet)
        {
            Console.WriteLine("Complete packet length: {0}", packet.Length.ToString());

            if ((packet.Length>=13) && (packet[0].Equals('A')) && (packet[7].Equals('d')))
            {
                if((packet[9].Equals('A')) && (packet[13].Equals('n'))){
                    accessLevel = AccessLevel.Admin;
                    return true;
                }else if((packet[9].Equals('M')) && (packet[15].Equals('r'))){
                    accessLevel = AccessLevel.Manager;
                    return true;
                }
                else if ((packet[9].Equals('E')) && (packet[16].Equals('e')))
                {
                    accessLevel = AccessLevel.Employee;
                    return true;
                }else{
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        public async void ConnectSQLServer(object sender, RoutedEventArgs e)
        {
            try
            {
                if (!UIGlobal.sharedTCPObject.socket.Connected){
                    MessageBox.Show("Server is not available.");
                    throw new SocketException();
                }
                int sentBytes = await TCP.SendTask(UIGlobal.sharedTCPObject, Encoding.UTF8.GetBytes("Login:" + UIGlobal.LoginWindowUI.Username + "| Password:" + UIGlobal.LoginWindowUI.Password + "|"));
                await TCP.ReceiveAllPacketsSync(UIGlobal.sharedTCPObject);

                Console.WriteLine("Received message: {0} ", UIGlobal.sharedTCPObject.packet);

                if (isAuthorizationSuccessful(UIGlobal.sharedTCPObject.packet))
                {
                    sharedData = new SharedData(UIGlobal.LoginWindowUI.Username, accessLevel, UIGlobal.sharedTCPObject);
                    DataWindowNS.DataWindow dataWindow = new DataWindowNS.DataWindow(sharedData);
                    dataWindow.Show();
                    UIGlobal.LoginWindowUI.rect_window_close();
                    Console.WriteLine("Logged onto server with username: {0} and password: {1}", UIGlobal.LoginWindowUI.Username, UIGlobal.LoginWindowUI.Password);
                }
                else
                {
                    MessageBox.Show("Incorrect username or password, please try again.");
                }
                
            }
            catch (SocketException ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        //// this method raises PropertyChanged event
        //protected void OnPropertyChanged(string propertyName)
        //{
        //    if (PropertyChanged != null) // if there is any subscribers 
        //        PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        //}

    }

    public class RelayCommand : ICommand
    {
        #region Fields

        readonly Action<object> _execute;
        readonly Predicate<object> _canExecute;        

        #endregion // Fields

        #region Constructors

        public RelayCommand(Action<object> execute)
        : this(execute, null)
        {
        }

        public RelayCommand(Action<object> execute, Predicate<object> canExecute)
        {
            if (execute == null)
                throw new ArgumentNullException("execute");

            _execute = execute;
            _canExecute = canExecute;           
        }
        #endregion // Constructors

        #region ICommand Members

        public bool CanExecute(object parameter)
        {
            return _canExecute == null ? true : _canExecute(parameter);
        }

        public event EventHandler CanExecuteChanged
        {
            add { CommandManager.RequerySuggested += value; }
            remove { CommandManager.RequerySuggested -= value; }
        }

        public void Execute(object parameter)
        {
            _execute(parameter);
        }

        #endregion // ICommand Members
    }

    public static class UIGlobal
    {
        public static LoginWindow LoginWindowUI { get; set; }
        public static SocketObject sharedTCPObject { get; set; }
    }

}