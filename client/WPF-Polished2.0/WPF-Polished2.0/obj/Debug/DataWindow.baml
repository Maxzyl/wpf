   M S B A M L   `   `   `  ����J  FWPF-Polished2.0, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null+clr-namespace:DataWindowNSDataWindowNS  5   	       DataWindowNS.DataWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   ,localclr-namespace:DataWindowNS     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     6W     � Title$  
DataWindow��5   	    � WindowStyle$
 None=�6    � AllowsTransparency . $
��1280��6I   $	��720��6V    � WindowStartupLocation$ CenterScreen=�6c    J� Opacity$ 1Q�5   �   ��6�   Z�  � 	Resources 5       �� MergedDictionaries �� Source�� 5
   
    5      �� 5      $ Styles.xaml?�6%   %    5      .��5      � -   5         �Name$	 GUI��5      .+�5   
   �� 
�������$
��Left=�6$   $	��Top=�6?   $
��1280��6W   $	��720��6d   	 �� RadiusX$	 10��6q   
 �� RadiusY$
 10��6~   �� 5   
   -   5         ��Name$ ExitRectangle��5      $��25��6+   $��25��67   $��	0,15,10,0q�6B   $��Right=�6U   $	��Top=�6q      	ExitImage#��[  6�   6�   �� 5   
   -   5        $ MinimizeRectangle��5      $��25��6/   $��25��6;   $��	0,15,40,0q�6F   $��Right=�6Y   $	��Top=�6u     MinimizeImage#��[ 6�   6�   �� 5   
   -   5        $ BackRectangle��5      $��25��6+   $��25��67   $��
15,15,70,0q�6B   $
��Left=�6V   $	��Top=�6q     	BackImage#��[ 6�   6�   �� 5   
   $��100,100,100,100q�6   .+�5      �� -   5         ��Name$ OrdersButton��5        OrdersButton#��[ 6+   $y�Hidden=�6~   �� 5      -   5        $ CustomersButton��5        CustomersButton#��[ 6.   $y�Hidden=�6�   �� 5      -   5        $ PromotionsButton��5        PromotionsButton#��[ 6/   $y�Hidden=�6�   5      �� 5   
   $��100,100,100,100q�6   .+�5      �� -   5        $ InventoryButton��5        InventoryButton#��[ 6.   $y�Hidden=�6�   �� 5      -	   5        $ EmployeesButton��5        EmployeesButton#��[ 6.   $y�Hidden=�6�   �� 5      -
   5        $ 
ExitButton��5        
ExitButton#��[ 6)   $y�Hidden=�6x   5       �� 5!   
   $��Center=�6   $��30��61   $��Bottom=�6=   $
��1280��6X   .@�5"      �� -   5        $ connectedStatusImage��5"      $��20��66   $��20��6B   $	��Top=�6M   $
��Left=�6e    	 CheckMarkImage#��[	 6�   �� 5#      -   5         ��Name$ networkStatus��5#      $��Connected to server.��63   $��30��6R   $	��Top=�6^   $
��Left=�6v   �� 5$      $��20��6   $��10��6$   $	��Top=�6/   $
��Left=�6G    
 DividerImage#��[
 6b   �� 5%      -   5        $ AccessLevel��5%      $��Access level: ��61   $��30��6J   $	��Top=�6V   $
��Left=�6n   �� 5&      $��20��6   $��10��6$   $	��Top=�6/   $
��Left=�6G   #��[
 6b   �� 5'      -   5        $ UI_Username��5'      $��Logged in as: ��61   $��30��6J   $	��Top=�6V   $	��159��6n   $
��Left=�6z   5(      5)      5*      