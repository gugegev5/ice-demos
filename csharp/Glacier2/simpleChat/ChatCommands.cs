// **********************************************************************
//
// Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;

namespace Glacier2.simpleChat.client
{
    public class ChatCommands
    {
        static ChatCommands()
        {
            // Initialize logout command
            _loginCommand = new RoutedUICommand("Login", "Login", typeof(ChatCommands), null);
            // Initialize logout command
            _logoutCommand = new RoutedUICommand("Logout", "Logout", typeof(ChatCommands), null);
            // Initialize exit commnad
            _exitCommand = new RoutedUICommand("Exit", "&Exit", typeof(ChatCommands), null);
        }

        public static RoutedUICommand Login
        {
            get
            {
                return _loginCommand;
            }
        }

        public static RoutedUICommand Logout
        {
            get
            {
                return _logoutCommand;
            }
        }

        public static RoutedUICommand Exit
        {
            get
            {
                return _exitCommand;
            }
        }

        private static RoutedUICommand _loginCommand;
        private static RoutedUICommand _logoutCommand;
        private static RoutedUICommand _exitCommand;
    }
}
