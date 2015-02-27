<?php
/**@file token_sso_form.inc
 * Archivo donde se generan los formularios y se ejecutan las las funciones de operación
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
 */
 
 /**@brief form_crea_token
  * Funcion para generar el formulario de generacion de tokens
  * @param $form es la variable que recibe los valores del formulario en caso de que exista una falla en la captura
  * @param $form_state es la variable que recibe los datos del formulario cuando se procesa correctamente.
  * @return $form el formato del forumario
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
  */
  
function form_crea_token($form, &$form_state) {

  $cmb = token_sso_cat_application();
  
  $form['description']=array(
    '#type' => 'item',
    '#title' => '',	
    '#suffix' => '<div id="resultado_aviso">'.t('Crea Tokens para los libros').'</div>',
	);

  $form['idInterno']=array(
    '#type' => 'textfield',
    '#title' => t('Libro'),
    '#ajax' => array(
      'callback' => '_valida_e_cat_callback',
      'event' => 'blur',
	),
	'#required' => TRUE,
    '#description' => "Favor de ingresar el Código que desea consultar.",
	);

  $form['ecat']=array(
   '#type'=> 'item',
   '#title' => t('Resultado E-Cat'),
   '#suffix' => '<div id="resutado_e_cat"></div>',
  );
  	
  $form['cantidad']=array(
    '#type' => 'textfield',
    '#title' => t('Cantidad'),
    '#required' => TRUE,
    '#description' => "Favor de ingresar el número de registros que desea.",
	);
	
  $form['permanent']= array(
    '#type'=> 'checkbox',
    '#title'=>t('Permanente'),
    '#description'=>t('En caso que el tipo de obra no sea del ciclo escolarizado puede seleccionar esta opción')
  );
  
  $form['application']=array(
  	'#type' => 'select',
  	'#title' => t('Aplicación'),
  	'#options' => $cmb,
  );
	
  $form['submit']=array(
    '#type' => 'submit',
    '#value' => 'Genera',  
  );
  
  return $form;
}

/**@brief form_crea_token_submit
 *General los tokens para los libros
 *El modulo valida la existencia de los tokens en la base de datos
 *y en el arreglo con lo cual se evita la duplicidad de los mismos.
 * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-09
 */
 function form_crea_token_submit($form, &$form_state) {
 	try{
	   $isbn = $form_state['input']['isbn'];
	   $idInterno = $form_state['input']['idInterno'];
	   $cantidad = $form_state['input']['cantidad'];
	   $application = $form_state['input']['application'];
	   $permanent = (is_null($form_state['input']['permanent']))? '0' : '1';
	 
	    _proccess_token_books($cantidad, $isbn, $idInterno, $application, $permanent) ;
		
	 $bitacora = "Se mandaron procesar " . $cantidad ." Tokens del libro " . $idInterno;
      $msg=t('Al termino del proceso se enviara un correo para notificar la creacion de los tokens solicitados');   
	  drupal_set_message($msg,'status');
	  bitacora_sso_operation($bitacora,'Token');
	}catch(exception $e) {
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	} 
}
  

 /**@brief form_valida_token
  * 
  * Pagina que muestra el formulario de validación del token ingresado para su activación.
  * 
  * @param $form es la variable que recibe los valores del formulario en caso de que exista una falla en la captura
  * @param $form_state es la variable que recibe los datos del formulario cuando se procesa correctamente.
  * @return $form el formato del forumario
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
  */
 function form_valida_token($form, &$form_state) {

   $form['description']=array(
    '#type' => 'item',
    '#title' => t('Valida Tokens'),   
    '#suffix' => '<br/><div id="resultado_aviso"></div><br/>',
    );
    
  $form['token']=array(
    '#type' => 'textfield',
    '#title' => t('Token'),
    '#required' => TRUE,
    '#description' => "Favor de ingresar el token que desea activar.",
    );
    
  $form['submit']=array(
    '#type' => 'submit',
    '#value' => 'Submit', 
    '#ajax' => array(
    	'callback'=>'form_valida_token_callback',
	  ),
	
  );
  
  return $form;    
 }
  
 /**@brief form_valida_token_callback
  * Función que procesa la validacion del token ingresado y regresa el resultado
  * a la pantalla correspondiente.
  * 
  * @param $form es la variable que recibe los valores del formulario en caso de que exista una falla en la captura
  * @param $form_state es la variable que recibe los datos del formulario cuando se procesa correctamente.
  * @return array con la la respuesta para mostrar la respuesta del evento.
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
  */
 function form_valida_token_callback($form, &$form_state) {
      $selector ='#resultado_aviso';	
      $token=$form_state['input']['token'];
      $result = token_sso_valida_token($token);
	  $commands = array();
	  $commands[] = ajax_command_replace($selector,"<div id='resultado_aviso'>".$result['msg']."</div>");
	  return array('#type' => 'ajax', '#commands' => $commands);  	  
  }

  /**@brief form_genera_cvs
  *Función que genera la pantalla para la generacion de tokens
  * 
  * @param $form es la variable que recibe los valores del formulario en caso de que exista una falla en la captura
  * @param $form_state es la variable que recibe los datos del formulario cuando se procesa correctamente.
  * @return array con la la respuesta para mostrar la respuesta del evento.
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
  */ 
  function form_genera_cvs_token($form, &$form_state) {
  	
	$cmb = token_sso_cat_application();
	
  	$form['description']=array(
    '#type' => 'item',
    '#title' => t('Descarga Tokens'),   
    '#suffix' => '<br/><div id="resultado_aviso"></div><br/>',
    );
	
	$form['isbn']=array(
    '#type' => 'textfield',
    '#maxlength'=>16,
    '#size'=>16,
    '#title' => t('ISBN'),
    '#required' => TRUE,
    '#description' => "Favor de ingresar el ISBN de los tokens a recuperar.",
	);
	 
	 $form['application']=array(
  	'#type' => 'select',
  	'#title' => t('Aplicación'),
  	'#options' => $cmb,
  	);
  
	$form['anio']=array(
    '#type' => 'textfield',
    '#maxlength'=>4,
    '#size'=>4,
    '#title' => t('Año'),
    '#required' => TRUE,
    '#description' => "",
	);
	
	$form['mes']=array(
    '#type' => 'textfield',
    '#maxlength'=>2,
    '#size'=>2,
    '#title' => t('Mes'),
    '#required' => TRUE,
    '#description' => "",
	);
	
	$form['dia']=array(
    '#type' => 'textfield',
    '#maxlength'=>2,
    '#size'=>2,
    '#title' => t('Dia'),
    '#required' => TRUE,
    '#description' => "",
	);		
	$form['submit']=array(
	  '#type' => 'submit',
      '#value' => 'Descarga', 
	);
	return $form;
  }
  
    /**@brief form_genera_cvs_token_submit
  * 
  * @fn Función que genera el archivo csv con los tokens creados, mediante el ISBN
  * el nombre del archivo se arma con el ISBN, mas la fecha de creacion del archivo.
  * se cambia el estatus del cada uno de los tokens que estan seleccionados.
  * 
  * @param $form es la variable que recibe los valores del formulario en caso de que exista una falla en la captura
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
  */
  function form_genera_cvs_token_submit($form, &$form_state) {
  	
  	$data['fielname']= $form_state['input']['isbn'];
  	$data['anio'] = $form_state['input']['anio'];
	$data['application'] = $form_state['input']['application'];
  	$data['mes'] = $form_state['input']['mes'];
  	$data['dia'] = $form_state['input']['dia'];
  	token_sso_descarga($data);
  }
  
    /**@brief _valida_e_cat_callback
  * 
  * @fn Función Para recueperar los datos del libro desde el eCaT
  * 
  * @param $form es la variable que recibe los valores del formulario en caso de que exista una falla en la captura
  * @param $form_state es la variable que recibe los datos del formulario cuando se procesa correctamente.
  * @return array con la la respuesta para mostrar los datos del libro solicitado.
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-09-20
  */ 
  function _valida_e_cat_callback($form, &$form_state) {
     
   $valor=$form_state['input']['idInterno'];//'ES125032';
 	$book=_consulta_e_cat($valor);
	
    $selector ='#resutado_e_cat';
    $commands[] = ajax_command_replace($selector,"<div id='resutado_e_cat'><div><input type='hidden' name='isbn' value='".$book->ISBN."'/></div>
    <div> ".t('Titulo: ').t($book->Titulo)."</div><div>".t('ISBN: ').t($book->ISBN)."</div><div>".t('Portada:')."<img src='".$book->GraficoBig."'/></div></div>");
    return array('#type' => 'ajax', '#commands' => $commands);   
 } 




   /**@brief form_token_qr
  * 
  * Funcion para validar el registro del token mediante QR 
  * @param $form es la variable que recibe los valores del formulario en caso de que exista una falla en la captura
  * @param $form_state es la variable que recibe los datos del formulario cuando se procesa correctamente.
  * @param $args son argumentos que se pasan adicionalmente
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-30
  */ 
function form_token_qr($form, &$form_state,$arg ='NULL') {
	$result = token_sso_valida_token($arg);

	switch ($result['result']) {
		case '100':
		case '400':
				$status = 'warning';
			break;
		case '200':
			$status = 'status';
			break;
		case '300':
			$status = 'error';
			break;
	}
	drupal_set_message(t($result['msg']),$status);
	bitacora_sso_operation(t($result['msg']),'Token');
}

  /**@brief form_carga_token
  * 
  * Funcion para cargar de forma masiva registros para realizar tokens
   * 
  * @param $form es la variable que recibe los valores del formulario en caso de que exista una falla en la captura
  * @param $form_state es la variable que recibe los datos del formulario cuando se procesa correctamente.
  * @param $args son argumentos que se pasan adicionalmente
  * 
 * @author Miguel Angel Chávez Obregón
 * @date 2014-10-30
  */ 
function form_carga_token($form,$form_state,$arg=NULL){

			
	$form['token_upload'] = array(
	 '#type' => 'file',
	 '#title' => t('Selecciona archivo'),
	 '#description' => t('Archivo en CSV para importar generacion de tokens'),
	 );
	 
	 $form['csv_delimiter'] = array(
	 '#type' => 'select',
	 '#title' => t('CSV Delimitador'),
	 '#options' => array(',' => ",",';' => ';'),
	 '#description' => t('Seleccionar qué libro se asociará a los tokens importados'),
	 );
	  
	 $form['actions'] = array('#type' => 'actions');
	 $form['submit']=array(
	  '#type' => 'submit',
      '#value' => 'Importar', 
	);

	 return $form;
}


function form_carga_token_validate($form, &$form_state){
	global $user;
	
	$file = &$form_state['values']['token_upload'];
	$delimiter = $form_state['values']['csv_delimiter'];
	$validators = array('file_validate_extensions' => array('csv'));
	 if ($token_file = file_save_upload('token_upload', $validators, 'public://')) {

	 ini_set('auto_detect_line_endings', TRUE);
	 
	 $errors = array();
	 $accept = '';
	 if ($fp = fopen($token_file->uri, 'r')) {
	 	
		 while (!feof($fp)) {
		 	
			 $row = fgetcsv($fp, NULL,$delimiter);
	
			 $idApp = _validaApp($row['4']);
		     $idPermanent = _validaPermanent($row['3']);	
			 $book = _consulta_e_cat(trim($row['0']));
			 
			 
			 if($row['0']!='idInterno'){
			 	
				if($row['0'] != ''){
				
				if($row['1'] != ''){
				
				if(($idApp) || ($idApp !=0 )){ 		

				 if($book->ISBN != ''){
				 	
				 	if($book->ISBN == $row['1']){
				 	
					 $uid = db_query("SELECT isbn FROM {sso_procesa_token} WHERE idInterno  ='" . $row['0'] . "' and status = '0'")->fetchField();
					  
					 if($uid) {
					 	$errors[] = t('Libro con ISBN') . ' ' . $row[1] . ' ' . t('Ya existe peticion');
						 $bitacora = t('Libro con ISBN') . ' ' . $row[1] . ' ' . t('Ya existe peticion');
					 } else {

					 	$insert = array(
							'idInterno'=>$row['0'],
							'isbn'=>$row['1'],
							'amount'=>$row['2'],
							'permanent'=>$idPermanent,
							'id_application'=>$idApp,
							'mail'=>$user->mail,
							'status'=>0
						);
						
						db_insert('sso_procesa_token')
						->fields($insert)
						->execute();
						$accept.='<li>' . t('Libro con ISBN:') .' '. $row['1']. ' ' . t('Cantidad:') .' '.$row['2'] . ' ' .t('Aceptados para generar') . '</li>'; 
					 }
				      $bitacora = "Se mandaron a proceso " . $row['2'] . " Tokens del Libro " . $row['0'];
				  }else{
				  	$errors[]= t('El ISBN del idInterno ') . ' '.$row['0'] .' '.t('es incorrecto');
					$bitacora = t('El ISBN del idInterno ') . ' '.$row['0'] .' '.t('es incorrecto');
				  }
				  
				  }else{
				  	$errors[]= t('El idInterno') . ' '.$row['0'] .' '.t('es incorrecto');
					$bitacora = t('El idInterno') . ' '.$row['0'] .' '.t('es incorrecto');
				  }
					
			  	}else{
			   		$errors[]= t('En el idInteno') . ' '.$row['0'] .' '.t('el nombre de aplicacion no es correcta');
			    	$bitacora = t('En el idInteno') . ' '.$row['0'] .' '.t('el nombre de aplicacion no es correcta');
				   }
				}else{
			   	$errors[]= t('El ISBN  del  idInterno').' '.$row['0'].' '.t('No puede ser vacio');
				$bitacora = t('El ISBN  del  idInterno').' '.$row['0'].' '.t('No puede ser vacio');
			     }
			  }else{
			  	$errors[]= t('El idInterno No puede ser vacio');
				$bitacora = t('El idInterno No puede ser vacio');
			  }
		    }
		    if(!empty($bitacora)){
		 		 bitacora_sso_operation($bitacora,'Token'); 
		  }
		 }
		 if(!empty($errors)) {
		 form_set_error('token_upload', '<ul><li>' . implode('</li><li>', $errors) . '</li></ul>');

		 }
		 if(!empty($accept)) {
		  drupal_set_message('<ul>' .$accept . '</ul>' ,'status');	
		 }
		
    }
 
 }

}

function _validaPermanent($permanent){
	
	$idPermanent='';
	switch($permanent){
		case 's':
		case 'S':
			$idPermanent=1;
			break;
		case 'n':
        case 'N':
			$idPermanent=0;
			break;		
	}
	return $idPermanent;	
}

function _validaApp($app){
	
	if(is_string($app)){
		
	$idApp='';
		switch (strtolower(trim($app))) {
			case 'conecta':
			case 'Conecta':	
				$idApp = 1;
				break;
			case 'soy lector':
			case 'soylector':
			case 'soy_lector':
	        case 'soy-lector':	
				$idApp = 2;
				break;
			default:
				$idApp = 0;
		}
	 return $idApp;
	}else{
		return false;
	}
}