   M S B A M L   `   `   `  ����O  Kmscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e0891&clr-namespace:System;assembly=mscorlibSystem  5   	   J FWPF-Polished2.0, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null!clr-namespace:LoginNSLoginNS 5   	      LoginNS.LoginWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   6sys&clr-namespace:System;assembly=mscorlib     5   	   'localclr-namespace:LoginNS     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   ?   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     6|     � Title$  LoginWindow��5   	    � WindowStyle$
 None=�6    � AllowsTransparency .  � WindowStartupLocation$ CenterScreen=�5   	   $	��254��6�   $	��438��6�    J� Opacity$ 1Q�6�   ���	#FFB10000��6�   Z�  � 	Resources 5	       �� MergedDictionaries �� Source�� 5   
    5      �� 5      $ Styles.xaml?�6%   %    5      .��5      � $	��400��6   $	��200��6   .+�5   
   �� -   5         ��Name$ LoginRectangle��5      $	��200��6,   $
��Left=�69   $	��Top=�6T   ���#6d6d6d$	��400��6}   	 �� RadiusX$	 40��6�   
 �� RadiusY$
 40��6�   
�������$��4��6�   �� 5   
   -   5        $ ExitRectangle��5      $��25��6+   $��25��67   $��
345,15,0,0q�6B   $
��Left=�6V   $	��Top=�6q      	ExitImage#��[  6�   6�   �� 5   
   -   5        $ MinimizeRectangle��5      $��25��6/   $��25��6;   $��
315,15,0,0q�6F   $
��Left=�6Z   $	��Top=�6u     MinimizeImage#��[ 6�   6�   �� 5   
   -   5         ��Name$ UsernameTextbox��5      $�� ��6+   $
��Left=�63   $��30��6N   $��	64,69,0,0q�6Z   $��14(�6m   ���#6d6d6d$��2q�6�   
����������#6d6d6d$	��Top=�6�   $	��273��6�    �� 	MaxLength$ 15��6!  60  �� 5   
   -   5        $ PasswordTextbox��5      $�� ��6+   $
��Left=�63   $��30��6N   $��
64,104,0,0q�6Z   $��14(�6n   ���#6d6d6d$��2q�6�   
����������#6d6d6d$	��Top=�6�   $	��273��6�   $ 9��6"  6Z  �� 5   
   -   5         ��Name$ LoginButton��5        LoginButton#��[ 6m   ��6&   ��   ServerC �� Source# [  �� Path$ LoginCommand�6m   5      5      