<?php

function webservers_configuracion(){
	
}

function webservers_rutas_admin(){
	
	$form['webserver_settings']=array(
	'#title' => 'Saludo que se muestra al usuario',  
    '#type' => 'textfield',
    '#default_value' => variable_get('saludar_saludo', 'Hola Mundo'),
    '#required' => TRUE,    
	);
	
	return system_settings_form($form);
	
}
