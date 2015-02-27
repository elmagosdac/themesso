<?php

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
function form_carga_user_csv($form, &$form_state, $arg=NULL){
     $form['user_upload'] = array(
	 '#type' => 'file',
	 '#title' => t('Selecciona archivo'),
	 '#description' => t('Archivo en CSV para cargar masivo de alumnos y profesores'),
	 '#id'=>'UploadButton'
	 );
	 
	 $form['csv_delimiter'] = array(
	 '#type' => 'select',
	 '#title' => t('CSV Delimitador'),
	 '#options' => array('' => "",',' => ",",';' => ';'),
	 '#description' => t('Seleccionar qué libro se asociará a los usuarios importados'),
	 );
	  
	 
	 $form['button'] = array(
		 '#type' => 'submit',
		 '#value' => t('Importar'),
		 '#id' => 'formmass',
		 // '#ajax' => array(
	 		// 'wrapper' => 'mensaje_csv',
		 	// 'callback' => 'form_carga_user_csv_submit',
		 	// 'method' => 'replace',
		 // ),
	 );
	 
	 return $form;		
}


function form_carga_user_csv_submit($form, &$form_state){
	global $user;
	$file = &$form_state['input']['files']['user_upload'];
	$delimiter = $form_state['values']['csv_delimiter'];
	$validators = array('file_validate_extensions' => array('csv'));
	if ($user_file = file_save_upload('user_upload', $validators, 'public://')) {
 		ini_set('auto_detect_line_endings', TRUE);
	 	$errors = array();
	 	$accept = '';
	    $accountID = $user->idaccount;
		
		if ($fp = fopen($user_file->uri, 'r')) {
	 		while (!feof($fp)) {
	 	 	 		
	 	 	 	$row = fgetcsv($fp, NULL,$delimiter);
			 	
			 	if($row['0']!='Nombre'){
			 	
			    	$dominio = _validate_dominio($row['4']);
				 
			  		if($dominio['result']=='200'){
		 
			      		$valide_uid=_valida_user_duplicados($row['4']);
			  
						if($valide_uid != 1){

							$nw_user = _insert_user($row);/**$nw_user < Objeto que regresa los valores de usuario insertado correctamente **/
							$nuevo[]=array(
									'fullname'=>$row['0'].' '.$row['1'].' '.$row['2'],
									'mail'=>$row['4'],
							);

							if(isset($nw_user->roles[4])){
								$gid = _inserta_grupo($row,$accountID); /** $gid < Identificado unico del grupo creado previamente o en el momento**/
								
							 	if(!empty($gid)){ /** General la relacion entre el grupo y el alumno**/
							 
									_insert_group_user($gid['gid'],$nw_user);
								}
								
								 _insert_parents($nw_user->uid,$row);//Valida que el usuario sea del tipo alumno para insertar tutores
	
							}

							if($row['16']!=''){
									$idApp = _validateApp($row['16']);
									_inserta_app_user($idApp,$nw_user); /** Genera la relacion entre el alumno y el app***/
									
								switch ($idApp) {
									case '1':
										_wsConecta($nw_user,'create');
										_wsMoodle($nw_user,'create');
										break;
									
									case '2':
										_wsSoyLector($nw_user,'create');
										break;
								 }				
									
							}

						}else{
							$duplicados[]=array(
									'fullname'=>$row['0'].' '.$row['1'].' '.$row['2'],
									'mail'=>$row['4'],
							);
						}
						
			  		}else{
			  			
					  	$dominios[]=array(
								'msg'=>$dominio['msg'],
								'mail'=>$row['4']
						);
			  		}
				}

	 	   	}
		}

		//fin de fopen
		
		//arreglo de nuevos usuarios
		if(isset($nuevo)) 
			if(is_array($nuevo)){

				drupal_set_message(t('Los siguientes usuaios Ingresaron correctamente al sistema'));
				foreach($nuevo as $new){
					drupal_set_message($new['fullname'].','.$new['mail'],'status');
				}	
			}
		
		//arreglo de usuarios duplicados
		if(isset($duplicados))
			if(is_array($duplicados)){
				
				drupal_set_message('Los siguentes usuarios ya existen en el sistema','warning');
				foreach($duplicados as $dup){
					drupal_set_message($dup['fullname'].','.$dup['mail'],'warning');
				}	
			}
		
		//arreglo de emails que ya existen en el sistema
		if(isset($dominios))
			if(is_array($dominios)){
				drupal_set_message('Las siguientes direcciones de correo electronico son inv&aacute;lidas','error');
				
				foreach($dominios as $dom){
					drupal_set_message($dom['msg'].','.$dom['mail'],'error');
				}					
			}
	}
}

function _valida_user_duplicados($mail){

	$uid = 0;
	try{
		$query = "Select uid from users where name ='".$mail."' and mail='".$mail."'";
		$result = db_query($query);
		
	
		foreach($result as $record){
			
			if(isset($record->uid))
			{
				$uid = 1;
			}
		}
		
		return $uid;
		
	}catch(exception $e){
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	}
}

function _recupera_accountID($file){

  global $user;

	try{
		$account=$user->license;
		return $account;
		
	}catch(exception $e){
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	}
}


function _inserta_grupo($group, $accountID){
	
	try{
		$grupo = $group['6'];
		
		$gid = db_select('sso_groups','gid')
		->fields('gid')
		->condition('groups',$grupo,'=')
		->condition('college',$accountID,'=')
		->execute()
		->fetchAssoc();
		
		if(!$gid){
			$nw_gid = db_insert('sso_groups')
			->fields(array(
				'groups'=>$grupo,
				'college'=>$accountID,
				'grade'=>1,
				'active'=>1
			))->execute();
			
			$bitacora = "Se da de alta al grupo ".$grupo."de la escuela ".$accountID;
			 bitacora_sso_operation($bitacora,'Usuario');
			 
			return $nw_gid;
		}else{
			
			$bitacora = "Se recupera el id de el grupo ".$grupo."de la escuela ".$accountID;
			 bitacora_sso_operation($bitacora,'Usuario');
			 
		  return $gid;	
		}

	}catch(exception $e){
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	}
}

function _insert_user($row){
	try{

				$idRol =_validateRol($row);
				foreach ($idRol as $key => $value) {
					$idKey = $key;
					$rol = $value;
				};
				$pass = _generaPassRand();
				$insert=array(
					'username'=>$row['0'],
					'lastname'=>$row['1'],
					'middlename'=>$row['2'],
					'fullname'=>$row['0'].' '.$row['1'].' '.$row['2'],
					'enrollment'=>$row['5'],
					'name'=>$row['4'],
					'mail'=>$row['4'],
					'init'=>$row['4'],
					'pass'=>$pass,
					'status'=>1,
					'roles'=>array(
						'2'=>'authenticated user',
						$idKey =>$rol
					)
				);
				
				global $user;

				if($rol == 5){
					$insert['license'];
				}
				$insert['idaccount']= $user->idaccount;
				
				$nw_user=user_save(null,$insert); //$nw_user < Retrona el valor del usuario registrado
				
				user_sso_correo($nw_user,$pass,$idKey);
				
				$bitacora = "Ingreso los datos del usuario ".$nw_user->username." con el uid ".$nw_user->uid;
				bitacora_sso_operation($bitacora,'Usuario');				
			return $nw_user;
	}catch(exception $e){
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	}
	
}

function _insert_parents($uid,$parents){

 try{
 	

		if(isset($parents['11'])){
			$mail=$parents['11'];
		}else{
			$mail='';
		}
		
		if(is_string($parents['9']) || ($parents['9']=='')){
			$phone = 0;
		}else{
			$phone =$parents['9'];
		}
			
			$pid= db_insert('sso_parents')->fields(array(
						'fullname'=>$parents['8'],
						'phone'=>$phone,
						'occupation'=>$parents['10'],
						'mail'=>$mail
					))->execute();
			

			db_insert('sso_user_parents')->fields(array(
						'id_parents'=>$pid,
						'uid'=>$uid
					))->execute();
					
		$bitacora ="Inserta Primer Tutor al Alumno com uid ".$uid;
		$msg = "Relaciono Primer Tutor al Alumno con uid ".$uid;
		bitacora_sso_operation($bitacora,'Usuario');
		bitacora_sso_operation($msg,'Usuario');
		if($parents['12']!='')
		{
			if(isset($parents['15'])){
				$mail=$parents['15'];
			}else{
				$mail='';
			}

		if(is_string($parents['13']) || ($parents['13']=='')){
			$phone = 0;
		}else{
			$phone =$parents['13'];
		}			
			$pid = db_insert('sso_parents')->fields(array(//
						'fullname'=>$parents['12'],
						'phone'=>$phone,
						'occupation'=>$parents['14'],
						'mail'=>$mail
					))->execute();

			db_insert('sso_user_parents')->fields(array(
						'id_parents'=>$pid,
						'uid'=>$uid
					))->execute();	
					
		$bitacora ="Inserta Segundo Tutor al Alumno com uid ".$uid;
		$msg = "Relaciono Segundo Tutor al Alumno con uid ".$uid;
		bitacora_sso_operation($bitacora,'Usuario');
		bitacora_sso_operation($msg,'Usuario');							
		}	
		

				
  }catch(exception $e){
  	drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
  }
}

function _insert_group_user($gid , $us){
	try{
		$uid = $us->uid;
		db_insert('sso_groups_user')
		->fields(array(
			'gid'=>(int) $gid,
			'uid'=>(int) $uid
		))->execute();
		
		$bitacora = "Inserta relacion entre usuario ".$uid." y grupo ".$gid;
		bitacora_sso_operation($bitacora,'Usuario');
		
	}catch(exception $e){
		drupal_set_message($e->getMessage().' '.$e->getCode(),'error');
	}
}


function _inserta_app_user($idApp,$user){
	$s = db_insert('sso_user_token_app')
	->fields(array(
		'uid'=> (int) $user->uid,
		'id_application'=> (int) $idApp
	))->execute();
	
	$bitacora = "Inserta relacion entre usuario ".$user->uid." y Aplicacion ".$idApp;
    bitacora_sso_operation($bitacora,'Usuario');
}



function _validateRol($user){
		
	$idRol = '';
	$rol = $user['7'];	
		switch (strtolower($rol)) {
			case 'alumno':
			case 'estudiante':
					$idRol['4'] = 'Alumno';
				break;
			case 'profesor':
			case 'maestro':
			case 'docente':
					$idRol['5'] = 'Profesor';
				break;		
			case 'contol escolar':
			case 'contolescolar':	
			case 'contol_escolar':
					$idRol['6'] = 'Control Escolar';
				break;			
		 }

		
	return $idRol;
}

function _validateApp($app){
	
	$idApp='';
		switch (strtolower($app)) {
			case 'conecta':
				$idApp = 1;
				break;
			case 'conecta digital':
				$idApp = 1;
				break;
				
			case 'conectadigital':
				$idApp = 1;
				break;
				
			case 'soy lector':
				$idApp = 2;
				break;
				
			case 'soylector':
				$idApp = 2;
				break;
				
			case 'soy_lector':
				$idApp = 2;
				break;
				
	        case 'soy-lector':	
				$idApp = 2;
				break;
		}
	return $idApp;
}

function _generaPassRand(){
 $an = '0123456789abcdefghijkmnpqrstuvwxyzABCDEFGHIJKMNPQRSTUVWYZ';
  $su = strlen($an)-1;
  return substr($an,rand(0,$su),1).
         substr($an,rand(0,$su),1).
         substr($an,rand(0,$su),1).
         substr($an,rand(0,$su),1).
         substr($an,rand(0,$su),1).         
         substr($an,rand(0,$su),1);	
}

/** @brief form_carga_user_edit()
 * formulario Para edición de usuario.
 * 
 * @author Miguel Angel
 * @date 2014-11-06
*/

function form_carga_user_edit($form, &$form_state, $args=NULL){
	
	$user = user_sso_recupera($args);	
	$cmb = user_sso_cmbo_role();
	$roles = user_sso_get_rol($args);
	$apps = user_sso_cmbo_apps();	
	
	$form['description']=array(
    '#type' => 'item',
    '#title' => '',	
	);

	 $form['user'] = array(
	   '#type' => 'fieldset', 
	   '#title' => t('Usuario'),
	   '#collapsible' => TRUE, // Added 
       '#collapsed' => FALSE, // Added
	  );	
		  
	$form['user']['uid'] =array(
  	'#type' => 'hidden',
  	'#default_value' => $user['user']['uid'],
  	);
	
	$form['user']['apps_us'] =array(
  	'#type' => 'hidden',
  	'#value' => $user['user']['apps'],
  	);
	
	$form['user']['role'] =array(
  	'#type' => 'hidden',
  	'#title' => t('Rol'),
  	'#value' => $cmb,
  	);
	
	$form['user']['apps'] = array(
		'#type' => 'hidden',
		'#title' => t('Apps'),
		'#value' => $apps
	);
	
	$form['user']['role_user'] = array(
	'#type' => 'hidden',
	'#title' => t('rol_user'),
	'#value' => $roles[0],
	);

		
	$form['user']['username']=array(
	'#type' => 'textfield',
    '#title' => t('Nombre'),
	'#required' => TRUE,
    '#description' => "Ingresar Nombre(s) del usuario.",
    '#default_value' => !empty($form_state['values']['username']) ? $form_state['values']['username'] : $user['user']['username'],
	);
	
	$form['user']['middlename']=array(
	'#type' => 'textfield',
	'#required' => TRUE,
    '#title' => t('Apellido Parterno'),
    '#description' => "Ingresar Apellido Paterno del usuario.",
    '#default_value' => !empty($form_state['values']['middlename']) ? $form_state['values']['middlename'] : $user['user']['middlename'],    
	);	
	
	$form['user']['lastname']=array(
	'#type' => 'textfield',
    '#title' => t('Apellido Materno'),
	
    '#description' => "Ingresar Apellido Materno del usuario.",
    '#default_value' => !empty($form_state['values']['lastname']) ? $form_state['values']['lastname'] : $user['user']['lastname'],
	);
	
	$form['user']['sex'] = array(
	 '#type' => 'select',
	 '#title' => t('Sexo'),
	 '#options' => array('h' => "H",'m' => 'M'),
	 '#description' => t('Seleccionar Sexo'),
	 '#default_value' => !empty($form_state['values']['sex']) ? $form_state['values']['matricula'] : $user['user']['sex']
	);
	
	$form['user']['matricula']=array(
	'#type' => 'textfield',
    '#title' => t('Matricula o Numero de alumno'),
    '#description' => "Ingresar Número de alumno del usuario.",
    '#default_value' => !empty($form_state['values']['matricula']) ? $form_state['values']['matricula'] : $user['user']['enrollment'],
	);
	
	$form['user']['mail']=array(
	'#type' => 'textfield',
    '#title' => t('Correo de acceso'),
	'#required' => TRUE,
    '#description' => "Ingresar Correo de contacto del usuario.",
    '#default_value' => !empty($form_state['values']['mail']) ? $form_state['values']['mail'] : $user['user']['mail'],
	);
	
	if($user['user']['rid']==4){
	 $form['parents'] = array(
	   '#type' => 'fieldset', 
	   '#title' => t('Tutores'),
       '#collapsible' => TRUE, // Added 
       '#collapsed' => TRUE, // Added	    
	   );
	   if($user['parents']){
	   	$cont = $user['parents'];
	   }else{
	   	$cont = 3;
	   }
	   		//parent 1
			$form['parents']['id1']=array(
			'#type' => 'hidden',
		    '#default_value' => !empty($form_state['values']['id1']) ? $form_state['values']['id1'] : @$user['parents'][0]->id_parents,
			);
			
		    $form['parents']['fullname1']=array(
			'#type' => 'textfield',
		    '#title' => t('Nombre completo Tutor 1'),
		    '#description' => "Ingresar Nombre completo Tutor 1",
		    '#default_value' => !empty($form_state['values']['fullname1']) ? $form_state['values']['fullname1'] : @$user['parents'][0]->fullname,
			);
			
			$form['parents']['occupation1']=array(
			'#type' => 'textfield',
		    '#title' => t('Ocupación ó actividade del Tutor 1'),
		    '#description' => "Ingresar ocupacion de Tutor 1",
		    '#default_value' => !empty($form_state['values']['occupation1']) ? $form_state['values']['occupation1'] : @$user['parents'][0]->occupation,
			);
			
			$form['parents']['phone1']=array(
			'#type' => 'textfield',
		    '#title' => t('Teléfono Tutor 1'),
		    '#description' => "Ingresar teléfono de Tutor 1",
		    '#default_value' => !empty($form_state['values']['phone1']) ? $form_state['values']['phone1'] : @$user['parents'][0]->phone,
			);	
			
			$form['parents']['email1']=array(
			'#type' => 'textfield',
		    '#title' => t('Email Tutor 1'),
		    '#description' => "Ingresar Email de Tutor 1",
		    '#default_value' => !empty($form_state['values']['email1']) ? $form_state['values']['email1'] : @$user['parents'][0]->mail,
			);	
			
			//parent 2
			
			$form['parents']['id2']=array(
			'#type' => 'hidden',
		    '#default_value' => !empty($form_state['values']['id2']) ? $form_state['values']['id2'] : @$user['parents'][1]->id_parents,
			);
			
		    $form['parents']['fullname2']=array(
			'#type' => 'textfield',
		    '#title' => t('Nombre completo Tutor 2'),
		    '#description' => "Ingresar Nombre completo Tutor 2",
		    '#default_value' => !empty($form_state['values']['fullname2']) ? $form_state['values']['fullname2'] : @$user['parents'][1]->fullname,
			);
			
			$form['parents']['occupation2']=array(
			'#type' => 'textfield',
		    '#title' => t('Ocupación ó actividade del Tutor 2'),
		    '#description' => "Ingresar ocupacion de Tutor 2",
		    '#default_value' => !empty($form_state['values']['occupation2']) ? $form_state['values']['occupation2'] : @$user['parents'][1]->occupation,
			);
			
			$form['parents']['phone2']=array(
			'#type' => 'textfield',
		    '#title' => t('Teléfono Tutor 2'),
		    '#description' => "Ingresar teléfono de Tutor 2",
		    '#default_value' => !empty($form_state['values']['phone2']) ? $form_state['values']['phone2'] : @$user['parents'][1]->phone,
			);	
			$form['parents']['email2']=array(
			'#type' => 'textfield',
		    '#title' => t('Email Tutor 2'),
		    '#description' => "Ingresar Email de Tutor 2",
		    '#default_value' => !empty($form_state['values']['email2']) ? $form_state['values']['email2'] : @$user['parents'][1]->mail,
			);	
		}
  $form['submit']=array(
    '#type' => 'submit',
    '#value' => 'Actualiza',  
  );		
	return $form;	
}


/** @brief form_carga_user_edit_submit()
 * funcion para procesar el formulario de edicion de usuario.
 * 
 * @author Miguel Angel
 * @date 2014-11-06
*/


function form_carga_user_edit_submit($form, &$form_state){
	global $user;
	
	$lisence = $form_state['license'];
	$username = $form_state['username'];
	$middlename = $form_state['middlename'];
	$lastname = $form_state['lastname'];
	$fullname = $lastname.' '.$middlename.' '.$username;
	$matricula = $form_state['matricula'];
	$uid = $form_state['input']['uid'];
	// $name = $form_state['input']['name'];	
	$role_user = $form_state['role_user'];
	$apps = $form_state['apps'];
	$apps_us = $form_state['apps_us'];
	$uid = $form_state['uid'];
	$mail = $form_state['mail'];
	$sex = $form_state['sex'];
	//$last_id = db_query('select max(uid) from {users}')->fetchAssoc();
	//$last = $last_id['max(uid)'] + 1 ;
	foreach($apps as $aps){
		$aps_u .=$aps.',';		
	}
	
	$roles['2']='authenticated user';
	foreach ($role as $value) {
		switch($value){
			case 4:
				$roles[$value]='Alumno';
				break;
			case 5:
				$roles[$value]='Profesor';
				$lisence = $user->license;
				break;
			case 6:
				$roles[$value]='Control Escolar';
				break;	
			case 9:
				$roles[$value]='Administrador de Plataforma';
				break;
			case 7:
				$roles[$value]='Administrador de Pais';
				break;	
			case 8:
				$roles[$value]='Editor de Contenido';
				break;
			case 3:
				$roles[$value]='Administrador';
				break;	
		}					
	}	

	$inserta = array(
		'username'=>$username,
		'middlename'=>$middlename,
		'lastname'=>$lastname,
		'fullname'=>$fullname,
		'enrollment'=>$matricula,
		'sex' =>$sex,
	);
	$nw_user = user_load($uid);
	$resultUd = db_update('{users}')
		->fields($inserta)
		->condition('uid',$uid)
		->execute();
	if($resultUd){
		if($role_user == 4){
			$data['parents'] =array(
				'fullname' => $form_state['fullname1'],
				'occupation' => $form_state['occupation1'],
				'phone' => $form_state['phone1'],
			);
			$result=0;
			$result = db_update('sso_parents')
				->fields($data['parents'])
				->condition('id_parents',$form_state['id1'])
				->execute();
						
			if($result){
				$msg = "Se actualizo el primer tutor al usuario ".$nw_user->uid;
				bitacora_sso_operation($msg, 'Usuario');
			}
	
			$data['parents'] =array(
				'fullname' => $form_state['fullname2'],
				'occupation' => $form_state['occupation2'],
				'phone' => $form_state['phone2'],
			);
			$result=0;
			$result = db_update('sso_parents')
				->fields($data['parents'])
				->condition('id_parents',$form_state['id2'])
				->execute();
			
			if($result){
				$msg = "Se actualizo el segundo tutor al usuario ".$nw_user->uid;
				bitacora_sso_operation($msg, 'Usuario');	
			}
			
		}
	}
	
	$user_id = $user->uid;
	$localurl = 'http://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
	$allInfo = json_encode( array( 
		'datauser' => json_encode($data['users']), 
		'callbackurl' => $localurl,
		'status'=>'loading',
		'session_uid' =>$user_id,
		
	)); 
	//se encriptan los datos en base64	
	$send = base64_encode($allInfo);
	//se envia el ws a las aplicaciones a las cuales el usuario esta asignado
	$apps = explode(',',$apps_us);
	
	foreach($apps as $k=>$i){
		if($i != ''){
			switch ($i) {
				case '1':
					$resultConecta = _wsConecta($nw_user,'edit');
					$result = json_decode($resultConecta);
					$msg = "El resultado de Conecta es ".$result->code." ".$result->message;
					bitacora_sso_operation($msg, 'Usuario');

					break;
				case '2':
					//_wsSoyLector($nw_user,'edit');
					break;
				case '3':
					_wsMoodle($nw_user,'edit');
					break;
			}
			$datas[]= array('url'=>$url,'datos' => $send);
		}
	}
	
	$msg = 'Se edito el usuario '.$nw_user->username.' con el uid '.$nw_user->uid;
	bitacora_sso_operation($msg, 'Usuario');
	
	user_sso_correo($nw_user,$pass,$role);
	echo json_encode(array('mensaje'=>'Editado con exito','code'=>200));	
	
}
/** @brief form_carga_user_new()
 * funcion para crear un nuevo usuario
 * 
 * @author Miguel Angel
 * @date 2014-11-06
*/

function form_carga_user_new($form, &$form_state, $args=NULL){
	global $user;
	if($user->idaccount){
		$niveles = get_levelsAcademy($user->idaccount);
		$niveles = explode(',',$niveles[0]->level);
	}
	
	$cmb = user_sso_cmbo_role();
	$apps = user_sso_cmbo_apps();
	
	$form['description']=array(
    '#type' => 'item',
    '#title' => '',	
	);

	 $form['user'] = array(
	   '#type' => 'fieldset', 
	   '#title' => t('Usuario'),
	   '#collapsible' => TRUE, // Added 
       '#collapsed' => FALSE, // Added
	  );	
	 $form['parents'] = array(
	   '#type' => 'fieldset', 
	   '#title' => t('Tutores'),
      '#collapsible' => TRUE, // Added 
      '#collapsed' => TRUE, // Added	    
	  );		  

	$form['user']['role'] =array(
  	'#type' => 'hidden',
  	'#title' => t('Rol'),
  	'#value' => $cmb,
  	);
	
	$form['user']['apps'] = array(
		'#type'=>'hidden',
		'#title'=> t('apps'),
		'#value' => $apps,
	);

	$form['user']['username']=array(
	'#type' => 'textfield',
    '#title' => t('Nombre'),
	'#required' => TRUE,
    '#description' => "Ingresar Nombre(s) del usuario.",
	);
	
	$form['user']['middlename']=array(
	'#type' => 'textfield',
    '#title' => t('Apellido Parterno'),
	'#required' => TRUE,
    '#description' => "Ingresar Apellido Paterno del usuario.",
	);	
	
	$form['user']['lastname']=array(
	'#type' => 'textfield',
    '#title' => t('Apellido Materno'),
    '#description' => "Ingresar Apellido Materno del usuario.",
	);
	
	$form['user']['sex'] = array(
	 '#type' => 'select',
	 '#title' => t('Sexo'),
	 '#options' => array('h' => "H",'m' => 'M'),
	 '#description' => t('Seleccionar Sexo'),
	);
	 
	$form['user']['matricula']=array(
	'#type' => 'textfield',
    '#title' => t('Matricula o Numero de alumno'),
    '#description' => "Ingresar Número de alumno del usuario.",
	);
	
	$form['user']['mail']=array(
	'#type' => 'textfield',
    '#title' => t('Correo de acceso'),
	'#required' => TRUE,
    '#description' => "Ingresar Correo de contacto del usuario.",
	);
	
	$form['user']['country']=array(
	'#type' => 'textfield',
    '#title' => t('Pais'),
    '#description' => "Ingresar el pais de recidencia.",
	);
	
	$form['user']['city']=array(
	'#type' => 'textfield',
    '#title' => t('Ciudad'),
    '#description' => "Ingresar la ciudad de recidencia.",
	);
	
	$form['group']['level']=array(
	  	'#type' => 'select',
	  	'#title' => t('Niveles'),
	  	'#options' => @$niveles,
	);
	
	$form['user']['group']=array(
	'#type' => 'textfield',
    '#title' => t('Grupo'),
    '#description' => "Ingresar el grupo.",
	);
	
	$form['user']['grade']=array(
	'#type' => 'textfield',
    '#title' => t('Grado academico'),
    '#description' => "Ingresar tu ciudad de recidencia.",
	);
	
	$form['parents']['fullname1']=array(
	'#type' => 'textfield',
    '#title' => t('Nombre completo Tutor 1'),
    '#description' => "Ingresar Nombre completo Tutor 1.",
	);
	
	$form['parents']['occupation1']=array(
	'#type' => 'textfield',
    '#title' => t('Ocupación ó actividade del Tutor 1'),
    '#description' => "Ingresar ocupacion de Tutor 1.",
	);
	
	$form['parents']['phone1']=array(
	'#type' => 'textfield',
    '#title' => t('Teléfono Tutor 1'),
    '#description' => "Ingresar teléfono de Tutor 1.",
	);
	$form['parents']['email1']=array(
	'#type' => 'textfield',
    '#title' => t('Email de Tutor 1'),
    '#description' => "Ingresar Email de tutor 1.",
	);	

	$form['parents']['fullname2']=array(
	'#type' => 'textfield',
    '#title' => t('Nombre completo Tutor 2'),
    '#description' => "Ingresar Nombre completo Tutor 2.",
	);
	
	$form['parents']['occupation2']=array(
	'#type' => 'textfield',
    '#title' => t('Ocupación ó actividade del Tutor 2'),
    '#description' => "Ingresar ocupacion de Tutor 2.",
	);
	
	$form['parents']['phone2']=array(
	'#type' => 'textfield',
     '#title' => t('Teléfono Tutor 2'),
    '#description' => "Ingresar teléfono de Tutor 2.",
	);	
	$form['parents']['email2']=array(
	'#type' => 'textfield',
    '#title' => t('Email de Tutor 2'),
    '#description' => "Ingresar Email de tutor 2.",
	);		
	
  $form['submit']=array(
    '#type' => 'submit',
    '#value' => 'Genera',  
  );		
	return $form;
}

/** @brief form_carga_user_new_submit()
 * funcion para procesar informacion y agregar el usuario.
 * 
 * @author Miguel Angel
 * @date 2014-11-06
*/


function form_carga_user_new_submit($form, &$form_state){
	global $user;
	$lisence = $form_state['license'];
	$username = $form_state['username'];
	$middlename = $form_state['middlename'];
	$lastname = $form_state['lastname'];
	$fullname = $lastname.' '.$middlename.' '.$username;
	$matricula = $form_state['matricula'];
	$uid = $form_state['input']['uid'];
	// $name = $form_state['input']['name'];	
	$role = $form_state['role'];
	$apps = $form_state['apps'];
	$mail = $form_state['mail'];
	$sex = $form_state['sex'];
	//$last_id = db_query('select max(uid) from {users}')->fetchAssoc();
	//$last = $last_id['max(uid)'] + 1 ;
	$aps_u = '';
	foreach($apps as $aps){
		$aps_u .=$aps.',';				
	}
	
	if(isset($role)){
		$roles['2']='authenticated user';
		foreach ($role as $value) {
			switch($value){
				case 4:
					$roles[$value]='Alumno';
					break;
				case 5:
					$roles[$value]='Profesor';
					$lisence = $user->license;
					break;
				case 6:
					$roles[$value]='Control Escolar';
					break;	
				case 9:
					$roles[$value]='Administrador de Plataforma';
					break;
				case 7:
					$roles[$value]='Administrador de Pais';
					break;	
				case 8:
					$roles[$value]='Editor de Contenido';
					break;
				case 3:
					$roles[$value]='Administrador';
					break;	
			}					
		}	
	
		$verify = user_verify_exists($form_state['mail']);
		if($verify < 1){
			$pass = _generaPassRand();
			$inserta = array(
				'name'=>$mail,
				'username'=>$username,
				'middlename'=>$middlename,
				'lastname'=>$lastname,
				'fullname'=>$fullname,
				'enrollment'=>$matricula,
				'apps' =>$aps_u,
				'status' => 1,
				'created' =>$_SERVER['REQUEST_TIME'],
				'pass'=>$username,//$pass
				'init'=>$mail,
				'mail'=>$mail,
				'roles'=>$roles,
				'license' =>$lisence,
				'sex'=>$sex
			);
			
			foreach($role as $r){
				if($r == 4){
					$data['parents'] =array(
						'fullname' => $form_state['fullname1'],
						'occupation' => $form_state['occupation1'],
						'phone' => $form_state['phone1'],
						'mail' => $form_state['email1'],
					);
					$result=0;
					$result= db_insert('sso_parents')
						->fields($data['parents'])
						->execute();	
								
					if($result){
						db_insert('sso_user_parents')
						->fields(array(
							'id_parents'=>(int) $result,
							'uid'=>(int) $nw_user->uid
						))
						->execute();
						
						$msg = "Se inserto primer tutor al usuario ".$nw_user->uid;
						$msg1 = "Se relaciono el primer tutor al usuario ".$nw_user->uid;
						bitacora_sso_operation($msg, 'Usuario');
						bitacora_sso_operation($msg1, 'Usuario');
					}
			
					$data['parents'] =array(
						'fullname' => $form_state['fullname2'],
						'occupation' => $form_state['occupation2'],
						'phone' => $form_state['phone2'],
						'mail' => $form_state['email2'],
					);
					$result=0;
					$result= db_insert('sso_parents')
						->fields($data['parents'])
						->execute();
					
					if($result){
						db_insert('sso_user_parents')
							->fields(array(
								'id_parents'=>(int) $result,
								'uid'=>(int) $nw_user->uid
							))
							->execute();
						$msg = "Se inserto segundo tutor al usuario ".$nw_user->uid;
						$msg1 = "Se relaciono el segundo tutor al usuario ".$nw_user->uid;
						bitacora_sso_operation($msg, 'Usuario');
						bitacora_sso_operation($msg1, 'Usuario');							
					}
					
				}else if($r == 6){
					$client = new SoapClient('http://ws.smit.com.mx/xrm/xrmservice.asmx?wsdl',array('soap_version' => SOAP_1_2));
					$data=array(
				  		contactid => $lisence,
				  		username => 'Denumeris',
				  		password => 'D3num3r1s'
				  	);
					
					$result = $client->__call('GetContact',array($data));
					$json = json_decode($result->GetContactResult);
					$inserta['idaccount'] = $json[0]->accountid;
				}
				$nw_user = user_save(null,$inserta); 
			}
			
			$user_id = $user->uid;
			$localurl = 'http://'.$_SERVER['HTTP_HOST'].$_SERVER['REQUEST_URI'];
			$allInfo = json_encode( array( 
				'datauser' => json_encode($data['users']), 
				'callbackurl' => $localurl,
				'status'=>'loading',
				'session_uid' =>$user_id,
				
			)); 
			//se encriptan los datos en base64	
			$send = base64_encode($allInfo);
			//se envia el ws a las aplicaciones a las cuales el usuario esta asignado
			foreach($apps as $app){
				if($app * 2){
					switch ($app) {
						case '1':
							$resultConecta = _wsConecta($nw_user,'create');
							$result = json_decode($resultConecta);
							$msg = "El resultado de Conecta es ".$result->code." ".$result->message;
							bitacora_sso_operation($msg, 'Usuario');
							
							break;
						case '2':
							$resultSoyLector = _wsSoyLector($nw_user,'create');
							$result = json_decode($resultSoyLector);
							$msg = "El resultado de SoyLector es ".$result->code." ".$result->message;
							bitacora_sso_operation($msg, 'Usuario');
							break;

						case '3':
							$resultMoodle = _wsMoodle($nw_user,'create');
							$result = json_decode($resultMoodle);
							$msg = "El resultado de MoodleUDP es ".$result->code." ".$result->message;
							bitacora_sso_operation($msg, 'Usuario');
							break;
					}
					$datas[]= array('url'=>$url,'datos' => $send);
				}
			}
			
			$msg = 'Se inserto el usuario '.$nw_user->username.' con el uid '.$nw_user->uid;
			bitacora_sso_operation($msg, 'Usuario');
			
			user_sso_correo($nw_user,$pass,$role);
			echo json_encode(array('mensaje'=>'Agregado con exito','code'=>200));
		}else{
			$msg = 'EL usuario '.$mail.' ya existe';
			bitacora_sso_operation($msg, 'Usuario');
			echo json_encode(array('mensaje'=>'Email Ya existe'));
		}	
	}else{
			$msg = 'No se selecciono el rol para el usuario '.$mail;
			bitacora_sso_operation($msg, 'Usuario');
		echo json_encode(array('mensaje'=>'Seleccionar un Rol'));
	}
}
