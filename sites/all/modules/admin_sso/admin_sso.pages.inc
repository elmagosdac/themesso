<?php

function applications_edit_form($arg){
	
	$data = recupera_aplicacion($arg);
	
	$type = lista_type_communication();

	$cmb_type_communication=cmb_type_communication($type);
	


  $form['personalization'] = array(
   		 '#type' => 'fieldset',
   		 '#title' => t('Edicion'),
  );

	$form['personalization']['application']=array(
		 '#type' => 'textfield',
		 '#title' =>t('Nombre de la Aplicacion'),
		 '#value'=>$data[0]->application,
		 '#size' => 30,
   		 '#maxlength' => 255,
		 
	);
	
	$form['personalization']['description']=array(
		 '#type' => 'textfield',
		 '#title' =>t('Descripcion de la Aplicacion'),
		 '#value'=>$data[0]->description,
		 '#size' => 30,
   		 '#maxlength' => 255,
		 
	);	
	
	$form['personalization']['cmb_type_communication']=array(
  	'#type' => 'select',
  	'#title' => t('Tipo Comunicacion'),
  	'#options' => $cmb_type_communication,
	'#value'=>$data[0]->id_cat_type_communication);
	
     $form['personalization']['cmb_status']=array(
  	'#type' => 'select',
  	'#title' => t('Status'),
  	'#options' => array(t('Inactivo'),t('Activo')),
	'#value'=>$data[0]->status);
	
	/*$form['action'] = array('#type' => 'actions');
	 $form['personalization']['actions']['submit'] = array(
    '#type' => 'submit',
  	'#value' => t('Editar Aplicacion'),
    );*/

  //$form['#validate'][] = 'user_register_validate';
  // Add the final user registration form submit handler.
  $form['#submit'][] = 'applications_edit_submit';
  
  return system_settings_form($form);
   
}

function applications_edit_submit($form, $form_state){
	
	///print_r($form_state).die();
	
}

function applications_add_form(){

	  $form['personalization'] = array(
   		 '#type' => 'fieldset',
   		 '#title' => t('Nueva Aplicacion'),
  );
  
	$form['personalization']['application']=array(
		 '#type' => 'textfield',
		 '#title' =>t('Nombre de la Aplicacion'),
		 '#size' => 30,
   		 '#maxlength' => 255,
   		 '#required' => TRUE,
		 
	);
	
	$form['personalization']['description']=array(
		 '#type' => 'textfield',
		 '#title' =>t('Descripcion de la Aplicacion'),
		 '#size' => 30,
   		 '#maxlength' => 255,
		 
	);
	
	// return system_settings_form($form);
	$form['submit']=array(
     '#type' => 'submit',
     '#value' => 'Guarda',   
	 );
	$form['#submit'][] = 'applications_add_submit';  	
	
	return $form;
}



function applications_add_submit($form, $form_state){
	try{
		global $user;
	
		$app = array(
			'application'=>$form_state['values']['application'],
			'description'=>$form_state['values']['description'],
			'status'=>1,
			'id_cat_type_communication'=>1
		);
		
		$result = db_insert('sso_cat_applications')
		->fields($app)
		->execute();
		
		if(!empty($result)){
			$msg = 'La aplicación '.$form_state['values']['application'].' se agrego correctamente';
			drupal_set_message($msg);
		}else{
			$msg = 'La aplicación '.$form_state['values']['application'].' no se agrego correctamente';
			form_set_error($msg);
		}
		
	}catch(exception $e) {
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	} 
}

function comnunication_add_form()
{
	
	  $form['personalization'] = array(
   		 '#type' => 'fieldset',
   		 '#title' => t('Nueva Comunicacion'),
  );
  
	$form['personalization']['communication']=array(
		 '#type' => 'textfield',
		 '#title' =>t('Nombre de la comunicacion (CURL, REST, SOAP)'),
		 '#size' => 30,
   		 '#maxlength' => 30,
   		 '#required' => TRUE,
		 
	);

	$form['submit']=array(
     '#type' => 'submit',
     '#value' => 'Guarda',   
	 );
	$form['#submit'][] = 'comnunication_add_submit';  	
	
	return $form;	
}

function comnunication_add_submit($form, $form_state){
	
  try{
  		$communication = array(
			'communication' => $form_state['values']['communication'],
			'status' => 1
		);

		$result = db_insert('sso_cat_type_communication')
		->fields($communication)
		->execute();
		
		if(!empty($result)){
			$msg = 'El tipo de comunicacion '.$form_state['values']['communication'].' se agrego correctamente';
			drupal_set_message($msg);
		}else{
			$msg = 'El tipo de comunicacion '.$form_state['values']['communication'].' no se agrego correctamente';
			form_set_error($msg);
		}	
		
	}catch(exception $e) {
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	} 	
}

function webservice_add_form()
{
	$app =lista_application();
	$comm =lista_type_communication();
	 
	$cmb_application = cmb_application($app);
	$cmb_type_communication=cmb_type_communication($comm);
	
	  $form['personalization'] = array(
   		 '#type' => 'fieldset',
   		 '#title' => t('Nuevo Comunicacion'),
  );
  
	$form['personalization']['url']=array(
		 '#type' => 'textfield',
		 '#title' =>t('URL de conexion'),
		 '#size' => 30,
   		 '#maxlength' => 150,
   		 '#required' => TRUE,
		 
	);

	$form['personalization']['cmb_application']=array(
  	'#type' => 'select',
  	'#title' => t('Aplicacion'),
  	'#options' => $cmb_application);
	
	$form['personalization']['cmb_type_communication']=array(
  	'#type' => 'select',
  	'#title' => t('Tipo Comunicacion'),
  	'#options' => $cmb_type_communication);
	
	$form['submit']=array(
     '#type' => 'submit',
     '#value' => 'Guarda',   
	 );
	$form['#submit'][] = 'webservice_add_submit';  	
	
	return $form;	
}

function webservice_add_submit($form, $form_state){
	
  try{
  		$communication = array(
  			'url'=>$form_state['values']['url'],
  			'id_application'=>$form_state['values']['cmb_application'],
			'id_cat_type_communication' => $form_state['values']['cmb_type_communication'],
			'status' => 1
		);

		$result = db_insert('sso_webservers')
		->fields($communication)
		->execute();
		
		if(!empty($result)){
			$msg = 'El tipo de comunicacion '.$form_state['values']['url'].' se agrego correctamente';
			drupal_set_message($msg);
		}else{
			$msg = 'El tipo de comunicacion '.$form_state['values']['url'].' no se agrego correctamente';
			form_set_error($msg);
		}	
		
	}catch(exception $e) {
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	} 	
}
