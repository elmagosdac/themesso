<?php
define('DOMINIO_LOCAL', 'mxsm.net');

/**
 *@brief
 * 
 * Implementando wsUserAuth()
 * 
 * Funcion tiene como objetivo dar acceso al usuario desde cualquier plataforma a su contenido.
 * Valida el dominio u con respecto al mismo trae la información de la sesion
 * 
 * @param $dato registro encriptado el cual se debe procesar para poder ser operado
 * 
 * @return user_load valores de la session del usuario
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-20
 */

function wsUserAuth($data){
  if(strpos($data['mail'], 'mxsm.net')){
  	$ldap_data = array(
		'user'=>$data['mail'],
		'password'=>$data['password'],
	);
  	$ldap_user =  user_ldap_verify($ldap_data,'ws');
	$uidLdap = login_sso_getUserLdap($ldap_user);
	
	$result =array('mensaje'=> array('uid'=> $uidLdap[0]));
	return $result;
  }else{
  	
  	$result = _wsdl_sso_validate_domain($data['mail']);
	if($result['result'] == 200){
	  	if(!empty($data['mail'])&& !empty($data['password'])){
			$account = user_load_by_mail($data['mail']);
			if(!empty($account)){
				 require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
				 if (user_check_password($data['password'], $account)) {
					 $result =array('mensaje'=> array('uid'=> $account->uid));
				 }else{
					$result =array('mensaje'=> 'Password invalido');
				 }
			}else{
				$result =array('mensaje'=> 'Email no encontrado en la base de datos');
			}
		}else{
			$result =array('mensaje'=> array('los campos no deben estar vacios'));
  		}	
	}else{
		$result =array('mensaje'=> array('Dominio de Email inexistente'));
	}
  }
	return $result;
	
}

/**
 *@brief
 * 
 * Implementando wsConfirm()
 * 
 * Funcion que tiene como objeto confirmar la alta/modificación de un usuario en las distintas plataformas 
 * a las cuales se solicitara el proceso.
 * 
 * @param $response  registro encriptado el cual se debe procesar para poder ser operado.
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-20
 */

function wsConfirm($response){
	
	 $data = json_decode(base64_decode(trim($response)));
	$user='';
	
	$confirm = array(
		'1'=>'El sistema no pueda ejecutar el alta de usuario',
		'2'=>'El sistema no pueda asignar y verificar un usuario',
		'3'=>'El sistema no pueda asignar un producto al usuario ',
		'4'=>'El sistema no pueda asignar un curso al usuario ',
		'5'=>'El sistema no pueda asignar un grupo al usuario ',
		'6'=>'Verifica que el usuario se haya creado correctamente',
		'7'=>'Una vez creado el usuario se asigna y verifica dicha acción',
		'8'=>'Producto Asignado',
		'9'=>'Curso Asignado',
		'10'=>'Grupo Asignado'
	);

	if(array_key_exists($data->result, $confirm)){
		$operacion = $confirm[$data->result];
	}else{
		$error['result'] = 'Sin valor o valor no correcto para result';
	}

	if($data->app!=''){
		$sql="Select application from sso_cat_applications where id_application=".$data->app;
 		$app = db_query($sql)->fetchField();	
	 	$operacion.= ' en la aplicación '.$app;
	 }else{
	 	$error['app'] = 'Sin valor o valor no correcto en app';
	 }
	
	if($data->uid==''){
		$error['uid'] = 'Sin valor o valor no correcto en uid';
	}else{
		
		$sql ="Select fullname from users where uid=".$data->uid;
	     $fullname = db_query($sql)->fetchField();
		if($fullname==''){
			$error['uid']='El id de usuario no existe';
		}else{
			
			$operacion.= ' para el usuario '.$fullname;
		}
		
	}
	
	if($data->action==''){
		$error['action'] = 'Sin valor de action';		
	}else{
		$operacion.= ' durante la acción '.$data->action;
	}

	if(isset($data->session_uid)){
		if($data->session_uid==''){
			$error['session_uid'] = 'Sin valor usuario que realiza la operacion';
		}else{
			 $query="Select uid, name,fullname, mail from users where uid=".$data->session_uid;
				$user = db_query($query)->fetchAll();
	
			if(empty($user)){
				
				$error['session_uid'] = 'El id de usuario no existe';
			}	
	
		}
	
	}
	
	 if(!empty($error))
	 {
	 	
	 	$error['result']=400;
		
	 	return $error;
	 }else{
	 	$user = user_load($user[0]->uid);
		bitacora_sso_wsoperation($user,$operacion,$data->action);
		
		$correct['result']=200;
		$correct['msg'] ='El registro se realizo correctamente';
		
		return $correct;
	 }
	
}

/**
 *@brief
 * 
 * Implementando wseditUser()
 * 
 * Funcion que tiene como objeto confirmar la alta/modificación de un usuario en las distintas plataformas 
 * a las cuales se solicitara el proceso.
 * 
 * @param $dato  registro encriptado el cual se debe procesar para poder ser operado.
 * @return retorna el resultado de la accion de forma encriptada.
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-20
 */
function wseditUser($dato){
	
	$user = base64_decode($dato);
	$update = array(
		'name'=>$user['name'],
  		'username'=>$user['username'],
  		'lastname'=>$user['lastname'],
  		'middlename'=>$user['middlename'],
  		'matricula'=>$user['matricula']
	);
	
	 $result = db_update('users')
	->fields($update)
	->condition('uid',$user['uid'])
	->execute();
	
	if(!empty($result)){
		
		$response=array(
			'response'=>200,
			'msg'=>'Actualizado Correctamente'
		);
	}else{
		$response=array(
			'response'=>400,
			'msg'=>'Fallo actualizacion internar mas tarde'
		);		
	}
	return $response;
}


/**
 *@brief
 * 
 * _wsdl_sso_validate_domainr()
 * 
 * Funcion que valida el dominio que se ingreso para logearse, se valida que el dominio exista dentro de los servidores 
 * de dns a nivel mundial
 * 
 * 
 * @param $email Es el domino del correo que se ingreso en el wsAuth
 * 
 * @return $data regesa un arreglo con los resultados de la operación
 * 
 * En caso que la operación sea correcta regresa aviso 200 => $dominio
 * 
 * En caso de no ser valido se regresea un aviso 300 => Correo invalido
 * 
 * En caso de que llege vacio el dato regresa aviso 400 => Vacio
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-20
 */

function _wsdl_sso_validate_domain($email){
    $email_text = str_replace(' ','',$email);
    $email_array =  explode("@", $email_text);
    
    if($email_array[1]==null){
        $email_text.='@mxsm.net';
    }
	
    $exp = "^[a-z'0-9]+([._-][a-z'0-9]+)*@([a-z0-9]+([._-][a-z0-9]+))+$";

    if(eregi($exp,$email_text)){
        $domain = array_pop(explode("@",$email_text));
        if(checkdnsrr($domain,"MX")){
           $data = array('result'=>"200", 'msg'=>$domain);
        }else{
            $data = array('result'=>"300", 'msg'=>'Correo Invalido');
        }

    }else{
        $data = array('result'=>"400", 'msg'=>'Vacio');
    }

    return $data;
}

/**
 *@brief
 * 
 * Implementando hook_server()
 * 
 * Funcion que genera el webserver basado en no-WSDL, usando SOAP 1.2
 * AL ser invocadao el sistema muestra los diferentes servicios que estan entregando.
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-20
 */

function _wsdl_sso_validate_pass($pass_text){
    
require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');

    if($pass_text==null){
        $data =array('result'=>400, 'msg'=>'El password no puede ser vacio');
    }else{
        $pass_result = user_check_password($pass_text);
        $data = json_encode(array('result'=>200, 'msg'=>$pass_result));
    }
    return $data;
}

/**
 *@brief
 * 
 * _decrypt()
 * 
 * Funcion para desencriptar la informacion enviada.
 * 
 * @param recibe registro encriptado
 * 
 * @return regresa arreglo con la informacion para ser procesada
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-20
 */
// 
// function _decrypt($data){
//    
   // $data_plain = base64_decode($data);
// 
   // return json_decode($data_plain);
// }


/**
 *@brief
 * 
 * _decrypt()
 * 
 * Funcion para encriptar la informacion a enviar.
 * 
 * @param recibe registro sin encriptado
 * 
 * @return regresa registro encriptado
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-20
 */
// function _encrypt($data){
 // $param = json_encode($data);	
  // return $plain_text = base64_encode($param);	
// }

/**
 *@brief
 * 
 * _encrypt_pass()
 * 
 * Funcion para encriptar password.
 * 
 * @param pass password sin encriptado
 * 
 * @return output password encriptado
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-12-09
 */
function _encrypt_pass($pass){
	$Key = 'sso';
   	 $output = base64_encode(mcrypt_encrypt(MCRYPT_RIJNDAEL_256, md5($Key), $input, MCRYPT_MODE_CBC, md5(md5($Key))));
        return $output;	
}

/**
 *@brief
 * 
 * _decrypt_pass()
 * 
 * Funcion para encriptar password.
 * 
 * @param pass password encriptado
 * 
 * @return output password sin encriptado
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-12-09
 */
function _decrypt_pass($pass){
   	$Key = 'sso';
   	$output = rtrim(mcrypt_decrypt(MCRYPT_RIJNDAEL_256, md5($Key), base64_decode($input), MCRYPT_MODE_CBC, md5(md5($Key))), "\0");
        return $output;
   }

/**
 *@brief
 * 
 * _cat_apps()
 * 
 * Funcion que genera regresa el nombre de la aplicacion que opero la información
 * 
 * @param $id_app ingresa el id de la aplicacion solicitada
 * 
 * @return $app retorna el nombre de la aplicacion relacionada al $id_app
 * 
  * @author Miguel Angel Chávez Obregón
  * @date 2014-10-20
 */

function _cat_apps($id_app){

	return $sql="Select application from sso_cat_applications where id_application=".$id_app;

  $app = db_query($sql)->fetchField();		

 return $app;		
}
